/*
    tasklist v2
    2015 09 16
*/

#include <type_traits>

namespace tasklist{
    namespace sst{
        //線形で保持する
        namespace aux{
            template<int N, class... TS>
            struct get_type_helper;

            template<int N, class T, class... TS>
            struct get_type_helper<N, T, TS...>{
                using type = typename get_type_helper<N - 1, TS...>::type;
            };

            template<class T, class... TS>
            struct get_type_helper<0, T, TS...>{
                using type = T;
            };
        }

        template<class... TS>
        struct line{
            static const int num = sizeof...(TS);
            template<int N>
            struct typeholder{
                using type = typename aux::get_type_helper<N, TS...>::type;
            };
        };

        //ネスト
        template<class T, class L>
        struct nest{
            using type = T;
            using line = L;
        };

        //タスク
        template<class T, int N, bool PreCreate = false>
        struct task{
            static const int num = N;
            static const bool pre_create = PreCreate;
            using type = T;
        };
    }

    //ストレージ型
    template<class = void>
    struct xstorage{
        using fn_type = void (*)(xstorage<>*&);
        using dtor_type = void (*)(void*);

        xstorage<> *next, *prev; //次と前のタスク
        void *wa; //ワーキングエリア
        fn_type fn; //処理関数
        dtor_type dtor; //デストラクタ
        int type_id; //型のID

        enum class sp{
            storage,
            begin,
            end
        };

        sp spflag; //storage : 0, begin : 1, end : 2
    };

    using storage = xstorage<>;
    using arstorage = storage*&;

    class task{
        template<class L>
        friend class taskmain;

    private:
        inline static void fn(tasklist::arstorage t){
            static_cast<task*>(t->wa)->update(t);
        }

    public:
        inline virtual void update(tasklist::arstorage){}
    };

    //メイン
    template<class L>
    class taskmain{
    private:
        int *tasknum;
        storage
            xbegin_,
            xend_,
            *sysarray,
            **taskarray;
        void **workingarea;
        using allocator = void *(*)();
        using deallocator = void (*)(void*&);
        allocator *alloc_;
        deallocator *dealloc_;

        //何もしない関数
        inline static void voidfn(arstorage){}

        //デストラクタ呼び出し
        template<class T>
        inline static void destructor(void *ptr){
            static_cast<T*>(ptr)->~T();
        }

        //型別にIDを振り分ける
        inline static int id_(){
            static int i = 0;
            return i++;
        }

        template<class T>
        inline static int id(){
            static int i = id_();
            return i;
        }

        //型操作など
        //ネストされているかどうか
        template<class T>
        struct nested_test;

        template<class T, int i, bool p>
        struct nested_test<sst::task<T, i, p>>{
            using type = T;
            using line = void;
        };

        template<class T, class K>
        struct nested_test<sst::nest<T, K>>{
            using type = T;
            using line = K;
        };

        //タスク型を取り除く
        template<class T>
        struct distasktype{
            using type = T;
        };

        template<class T, int i, bool p>
        struct distasktype<sst::task<T, i, p>>{
            using type = T;
        };

        //ネスト型を取り除く
        template<class T>
        struct disnesttype{
            using type = T;
        };

        template<class T, class U>
        struct disnesttype<sst::nest<T, U>>{
            using type = T;
        };

        //アロケータ―/デアロケータ―
        template<class T, int N>
        inline static void *alloc(){
            return new T[N];
        }

        template<class T>
        inline static void dealloc(void *&ptr){
            delete[] static_cast<T*>(ptr);
            ptr = nullptr;
        }

        struct constructor_frame{
            int *tasknum;
            storage *sysa;
            storage **ta;
            storage *t;
            void **wa;
            allocator **a;
            deallocator **de;
            bool reserve_flag;
        };

        template<class T, int N>
        inline static void ctor_a_common(constructor_frame frame){
            int m = id<T>();
            frame.tasknum[m] = N;
            (*frame.a)[m] = &alloc<typename std::aligned_storage<sizeof(T), alignof(T)>::type, N>;
            (*frame.de)[m] = &dealloc<typename std::aligned_storage<sizeof(T), alignof(T)>::type>;

            if(N){
                int n = m * 3 + 2;
                frame.ta[m] = new storage[N];
                if(frame.reserve_flag){
                    frame.wa[m] = (*frame.a)[m]();
                }else{
                    frame.wa[m] = nullptr;
                }

                storage *taa = frame.ta[m];
                frame.sysa[n].next = &taa[0];
                for(int i = 0, e = N - 1; i < e; ++i){
                    taa[i].next = &taa[i + 1];
                }
                taa[N - 1].next = &frame.sysa[n];

                for(int i = 0; i < N; ++i){
                    taa[i].dtor = &destructor<T>;
                    taa[i].type_id = m;
                }

                if(frame.reserve_flag){
                    for(int i = 0; i < N; ++i){
                        taa[i].wa = static_cast<void*>(&static_cast<T*>(frame.wa[m])[i]);
                    }
                }else{
                    for(int i = 0; i < N; ++i){
                        taa[i].wa = nullptr;
                    }
                }
            }else{
                int n = m * 3 + 2;
                frame.sysa[n].next = frame.sysa[n].prev = &frame.sysa[n];
                frame.ta[m] = nullptr;
                frame.wa[m] = nullptr;
            }

            int n = m * 3;
            frame.sysa[n].spflag = storage::sp::begin;
            frame.sysa[n].fn = voidfn;
            frame.t->next = &frame.sysa[n];
            frame.sysa[n].prev = frame.t;
        }

        template<class T, int N, class = void>
        struct ctor_a{
            inline static storage &fn(constructor_frame frame){
                ctor_a_common<T, N>(frame);
                return frame.sysa[id<T>() * 3];
            }
        };

        template<class T, int i, bool p, int N, class Dummy>
        struct ctor_a<sst::task<T, i, p>, N, Dummy>{
            inline static storage &fn(constructor_frame frame){
                ctor_a_common<T, N>(frame);
                return frame.sysa[id<T>() * 3];
            }
        };

        template<class T>
        inline static void ctor_b_common(storage *sysa, storage &t){
            int m = id<T>() * 3 + 1;
            sysa[m].spflag = storage::sp::end;
            sysa[m].fn = voidfn;

            t.next = &sysa[m];
            sysa[m].next = &t;
        }

        template<class T, class = void>
        struct ctor_b{
            inline  static storage &fn(storage *sysa, storage &t){
                ctor_b_common<T>(sysa, t);
                return sysa[id<T>() * 3 + 1];
            }
        };

        template<class T, int i, bool p, class Dummy>
        struct ctor_b<sst::task<T, i, p>, Dummy>{
            inline  static storage &fn(storage *sysa, storage &t){
                ctor_b_common<T>(sysa, t);
                return sysa[id<T>() * 3 + 1];
            }
        };

        template<class K, int i = 0, bool b = false, class = void>
        struct subst{
            using base_a =
                subst<
                    typename nested_test<
                        typename K::template typeholder<i>::type
                    >::line
                >;
            using base_b = subst<K, i + 1, K::num == i + 1>;
            using plntype = typename K::template typeholder<i>::type::type;
            using tasktype = typename disnesttype<typename K::template typeholder<i>::type>::type;

            inline static int count(){
                return 1 + base_a::count() + base_b::count();
            }

            inline static storage &ctor(constructor_frame frame){
                return base_b::ctor(constructor_frame{
                    frame.tasknum, frame.sysa, frame.ta,
                    &ctor_b<plntype>::fn(
                        frame.sysa,
                        base_a::ctor(constructor_frame{
                            frame.tasknum, frame.sysa, frame.ta,
                            &ctor_a<
                                plntype,
                                tasktype::num
                            >::fn(constructor_frame{ frame.tasknum, frame.sysa, frame.ta, frame.t, frame.wa, frame.a, frame.de, frame.reserve_flag }),
                            frame.wa, frame.a, frame.de, frame.reserve_flag
                        })
                    ),
                    frame.wa, frame.a, frame.de, frame.reserve_flag
                });
            }
        };

        template<class K, int i, class Dummy>
        struct subst<K, i, true, Dummy>{
            inline static int count(){ return 0;}
            inline static storage &ctor(constructor_frame frame){ return *frame.t; }
        };

        template<class Dummy>
        struct subst<void, 0, false, Dummy>{
            inline static int count(){ return 0;}
            inline static storage &ctor(constructor_frame frame){ return *frame.t; }
        };

        //部分的に生成する
        template<class Task, int num, class = void>
        struct ctor_partial_a{
            inline static taskmain &fn(taskmain &obj){
                if(Task::pre_creaet){
                    for(int i = 0; i < num; ++i){
                        obj.reserve<typename Task::type>();
                        obj.create_task<typename Task::type>();
                    }
                }
                return obj;
            }
        };

        template<class T, int num, bool p, class Dummy>
        struct ctor_partial_a<sst::task<T, 0, p>, num, Dummy>{
            inline static taskmain &fn(taskmain &obj){
                return obj;
            }
        };

        template<class T, int i, bool p, int num, class Dummy>
        struct ctor_partial_a<sst::task<T, i, p>, num, Dummy>{
            inline static taskmain &fn(taskmain &obj){
                if(p){
                    for(int n = 0; n < i; ++n){
                        obj.reserve<T>();
                        obj.create_task<T>();
                    }
                }
                return obj;
            }
        };

        template<class K, int i = 0, bool b = false, class = void>
        struct ctor_partial{
            using base_a =
                ctor_partial<
                    typename nested_test<
                        typename K::template typeholder<i>::type
                    >::line
                >;
            using base_b = ctor_partial<K, i + 1, K::num == i + 1>;
            using plntype = typename K::template typeholder<i>::type::type;
            using tasktype = typename disnesttype<typename K::template typeholder<i>::type>::type;

            inline static taskmain &ctor(taskmain &obj){
                base_b::ctor(base_a::ctor(ctor_partial_a<tasktype, tasktype::num>::fn(obj)));
                return obj;
            }
        };

        template<class K, int i, class Dummy>
        struct ctor_partial<K, i, true, Dummy>{
            inline static taskmain &ctor(taskmain &obj){
                return obj;
            }
        };

        template<class Dummy>
        struct ctor_partial<void, 0, false, Dummy>{
            inline static taskmain &ctor(taskmain &obj){
                return obj;
            }
        };

        //全て生成する
        template<class T, int num, class = void>
        struct ctor_all_a{
            inline static taskmain &fn(taskmain &obj){
                for(int i = 0; i < num; ++i){
                    obj.create_task<T>();
                }
                return obj;
            }
        };

        template<class T, int num, bool p, class Dummy>
        struct ctor_all_a<sst::task<T, 0, p>, num, Dummy>{
            inline static taskmain &fn(taskmain &obj){
                return obj;
            }
        };

        template<class T, int i, bool p, int num, class Dummy>
        struct ctor_all_a<sst::task<T, i, p>, num, Dummy>{
            inline static taskmain &fn(taskmain &obj){
                for(int n = 0; n < i; ++n){
                    obj.create_task<T>();
                }
                return obj;
            }
        };

        template<class T, class = void>
        struct ctor_all_b{
            inline static taskmain &fn(taskmain &obj){
                obj.create_task<T>();
                return obj;
            }
        };

        template<class T, bool p, class Dummy>
        struct ctor_all_b<sst::task<T, 0, p>, Dummy>{
            inline static taskmain &fn(taskmain &obj){
                return obj;
            }
        };

        template<class T, int i, bool p, class Dummy>
        struct ctor_all_b<sst::task<T, i, p>, Dummy>{
            inline static taskmain &fn(taskmain &obj){
                for(int n = 0; n < i; ++n){
                    obj.create_task<T>();
                }
                return obj;
            }
        };

        template<class K, int i = 0, bool b = false, class = void>
        struct ctor_all{
            using base_a =
                ctor_all<
                    typename nested_test<
                        typename K::template typeholder<i>::type
                    >::line
                >;
            using base_b = ctor_all<K, i + 1, K::num == i + 1>;
            using plntype = typename K::template typeholder<i>::type::type;
            using tasktype = typename disnesttype<typename K::template typeholder<i>::type>::type;

            inline static taskmain &ctor(taskmain &obj){
                base_b::ctor(ctor_all_b<plntype>::fn(base_a::ctor(ctor_all_a<plntype, tasktype::num>::fn(obj))));
                return obj;
            }
        };

        template<class K, int i, class Dummy>
        struct ctor_all<K, i, true, Dummy>{
            inline static taskmain &ctor(taskmain &obj){
                return obj;
            }
        };

        template<class Dummy>
        struct ctor_all<void, 0, false, Dummy>{
            inline static taskmain &ctor(taskmain &obj){
                return obj;
            }
        };

    public:
        //幾つあるか
        inline static int bcnum(){
            static int i = subst<L>::count();
            return i;
        }

        //タスクを確保する
        template<class T>
        inline void reserve(){
            int n = id<T>();
            dealloc_[n](workingarea[n]);
            workingarea[n] = alloc_[n]();
            storage *t = taskarray[n];
            for(int i = 0; i < tasknum[n]; ++i){
                t[i].wa = static_cast<void*>(&static_cast<T*>(workingarea[n])[i]);
            }
        }

        //確保済みタスクを開放する
        template<class T>
        inline void release(){
            int n = id<T>();
            dealloc_[n](workingarea[n]);
            storage *t = taskarray[n];
            for(int i = 0; i < tasknum[n]; ++i){
                t[i].wa = nullptr;
            }
        }

        //タスクを生成する
        template<class T>
        inline T *create_task(){
            int m = id<T>() * 3;
            storage *t = &sysarray[m + 2];
            if(t->next == t){
                return nullptr;
            }

            storage *r = t->next;
            t->next = r->next;

            r->next = sysarray[m].next;
            sysarray[m].next->prev = r;
            r->prev = &sysarray[m];
            sysarray[m].next = r;

            r->fn = T::fn;
            r->spflag = storage::sp::storage;
            new(r->wa) T;

            return static_cast<T*>(r->wa);
        }

        //タスクを削除する
        inline storage *delete_task(storage *t){
            storage *r = t->next;
            t->dtor(t->wa);

            t->prev->next = t->next;
            t->next->prev = t->prev;

            int m = t->type_id * 3 + 2;
            t->next = sysarray[m].next;
            sysarray[m].next = t;

            return r;
        }

        template<class T>
        inline storage *begin(){
            return sysarray[id<T>() * 3].next;
        }

        template<class T>
        inline storage *end(){
            return &sysarray[id<T>() * 3 + 1];
        }

        inline storage *xbegin(){
            return xbegin_.next;
        }

        inline storage *xend(){
            return &xend_;
        }

        inline void operator()(){
            for(
                storage *t = xbegin(), *e = xend();
                t != e;
                t = t->next
            ){ (t->fn)(t); }

        }

        inline taskmain(bool b = false){
            int n = bcnum();
            tasknum = new int[n];
            sysarray = new storage[n * 3];
            taskarray = new storage*[n];
            workingarea = new void*[n]{ nullptr };
            alloc_ = new allocator[n];
            dealloc_ = new deallocator[n];
            sysarray[0].spflag = storage::sp::begin;

            constructor_frame frame;
            frame.tasknum = tasknum;
            frame.sysa = sysarray;
            frame.ta = taskarray;
            frame.t = &xbegin_;
            frame.wa = workingarea;
            frame.a = &alloc_;
            frame.de = &dealloc_;
            frame.reserve_flag = b;

            storage &e(subst<L>::ctor(frame));
            e.next = &xend_;
            xend_.prev = &e;

            if(b){
                ctor_all<L>::ctor(*this);
            }else{
                ctor_partial<L>::ctor(*this);
            }
        }

        taskmain(const taskmain&) = delete;
        taskmain(taskmain &&) = delete;

        inline virtual ~taskmain(){
            delete[] tasknum;
            delete[] sysarray;

            int n = bcnum();
            for(int i = 0; i < n; ++i){
                delete[] taskarray[i];
                dealloc_[i](workingarea[i]);
            }

            delete[] taskarray;
            delete[] workingarea;
            delete[] alloc_;
            delete[] dealloc_;
        }
    };
}