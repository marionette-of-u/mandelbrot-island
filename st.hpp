/*
    �╨
*/

#ifndef __TTY_ST__
#define __TTY_ST__

/*
    cstrings.index    -> tty.ta_index
    cstrings.rindex    -> tty.ta_rindex
*/
namespace tty{
    template<typename T> T *ta_index(const T *str, const T &c, const T &tmnl = T()){
        for(; *str != tmnl && *str != c; ++str);
        return const_cast<T*>(*str == tmnl ? reinterpret_cast<T*>(0) : str);
    }

    template<typename T> T *ta_rindex(const T *str, const T &c, const T &tmnl = T()){
        T *r = reinterpret_cast<T*>(0);
        for(; *str != tmnl; ++str)
            if(str == c) r = const_cast<T*>(str);
        return r;
    }
}

/*
    strcmp���C�ӂ̌^�ɑΉ�������I
*/
namespace tty{
    template<typename T> int atcmp(const T *str, const T *tus, const T &trm = T()){
        for(; ; ++str, ++tus){
            if(*str == *tus){
                if(*str == trm)
                    return 0;
                else
                    continue;
            }else{
                return *str > *tus ? 1 : -1;
            }
        }
    }
}

/*
    template�֐��o�[�W������memcpy
*/
#include <cstring>
namespace tty{
    template<typename T, typename V> inline T *memcpy(T *dst, T *src, V s){
        return reinterpret_cast<T*>(std::memcpy(
            reinterpret_cast<void*>(dst),
            reinterpret_cast<const void*>(src),
            sizeof(T) * static_cast<size_t>(s)
        ));
    }

    template<typename T, typename V> inline T *memcpy(T *dst, const T *src, V s){
        return reinterpret_cast<T*>(std::memcpy(
            reinterpret_cast<void*>(dst),
            reinterpret_cast<const void*>(src),
            sizeof(T) * static_cast<size_t>(s)
        ));
    }
}

/*
    a��b����v�Z����
*/
namespace tty{
    template<unsigned int a, unsigned int b> struct pow{
        enum{ value = a * pow<a, b - 1>::value };
    };

    template<unsigned int a> struct pow<a, 1>{
        enum{ value = a };
    };

    template<unsigned int a> struct pow<a, 0>{
        enum{ value = 1 };
    };
}

/*
    �C�ӂ̌^�̔z��̒�����C�ӂ̏I�[�������w�肵�Ē��ׂ�
    �I�[�����̓f�t�H���g�Ń[��
*/
namespace tty{
    template<typename T> inline int ta_length(const T *sequence, const T &tv = 0){
        int i = 0;
        for(; sequence[i] != tv; ++i);
        return i;
    }
}

/*
    ���C�h�����Ή���STRINGIZE
*/
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/stringize.hpp>

# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define TTY_WSTRINGIZE_X(a) TTY_WSTRINGIZE_X_A((a))
#    define TTY_WSTRINGIZE_X_A(arg) TTY_WSTRINGIZE_X_I arg
# elif BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define TTY_WSTRINGIZE_X(text) TTY_WSTRINGIZE_X_OO((text))
#    define TTY_WSTRINGIZE_X_OO(par) TTY_WSTRINGIZE_X_I ## par
# else
#    define TTY_WSTRINGIZE_X(text) TTY_WSTRINGIZE_X_I(text)
# endif

# define TTY_WSTRINGIZE_X_I(a) L ## a

# define TTY_WSTRINGIZE(a) TTY_WSTRINGIZE_X(BOOST_PP_STRINGIZE(a))

/*
    force_cast
*/
namespace tty{
    template<typename T, typename U> inline T &force_cast(U &a){
        return *reinterpret_cast<T*>(&a);
    }
}

/*
    �֐��͔z���Ԃ��Ȃ��̂�POD�Ń��b�v�������̂����
    boost::array�Ȃǂ̗l�ȃ����o�֐��������Ă���\���̂�
    �œK������ɂ����̂Ŏ~�ނ𓾂�����������
*/
namespace tty{
    template<typename T, int N> struct array_t{
        T data[N];
    };
}

/*
    //629��\�������ꍇ
    binary<1001110101>::value == 629; //true
    //1��0�ȊO�̐��������o����ƃR���p�C���G���[
    binary<23456789>; //error
*/
#include <boost/static_assert.hpp>
namespace tty{
    template<unsigned int v, unsigned int n> struct binary_i{
    private:
        BOOST_STATIC_ASSERT((n % 10) <= 1);

    public:
        enum{ value = (n % 10) + (binary_i<v, n / 10>::value) * 2 };
    };

    template<unsigned int a> struct binary_i<a, 0>{
        enum{ value = 0 };
    };

    template<unsigned int v> struct binary{
        enum{ value = binary_i<v, v>::value };
    };
}

/*
    //sequential expr
    //�����܂Ƃ߂ē������Z�q�Ō���
    TTY_SEQUENTIALEXPR(&&, (0 == 0)(1 == 1)(2 == 3))
    TTY_SEQUENTIALEXPR(+, (0)(1)(2)(3)(4)(5)(6)(7)(8)(9))
    //45 * 2 * 2 = 180
    TTY_SEQUENTIALEXPR(
        *,
        (TTY_SEQUENTIALEXPR(+, (0)(1)(2)(3)(4)(5)(6)(7)(8)(9)))(2)(2)
    )
*/
#include <boost/preprocessor.hpp>

#define TTY_SEQUENTIALEXPR_EAT(a)
#define TTY_SEQUENTIALEXPR_VOMIT(a) a

#define TTY_SEQUENTIALEXPR_I(r, op, i, e) \
    BOOST_PP_IF(i, TTY_SEQUENTIALEXPR_VOMIT, TTY_SEQUENTIALEXPR_EAT)(op) (e)

#define TTY_SEQUENTIALEXPR(op, seq) \
    BOOST_PP_SEQ_FOR_EACH_I(TTY_SEQUENTIALEXPR_I, op, seq)

#define TTY_SEQEXPR TTY_SEQUENTIALEXPR

#endif // __TTY_ST__
