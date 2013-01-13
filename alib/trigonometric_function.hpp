/*
	三角関数のテーブルをtemplateを使って実装.
	<unsigned int N = 10>
		2^N 要素の数

	>> angletype
	角度を表す整数型
	>> pi2
	2^N. 一周の角度
	>> pi
	半周の角度

	>> T sin(const ArgType &a)
	>> T cos(const ArgType &a)
	>> T tan(const ArgType &a)
	角度 a に対する各三角関数の結果を返す.

	>> angletype atan2(const T &y, const T &x)
	2つの座標を成す角度を求める.
*/

template<unsigned int N, typename Alloc = std::allocator<arithmetic_type> > class trigonometric_function;

template<unsigned int N, typename Alloc = std::allocator<arithmetic_type> > class trigonometric_function_TmpRec{
private:
	template<unsigned int N_> struct endholder_0{ typedef trigonometric_function<N_, Alloc> end; };
	template<typename Alloc_> struct endholder_1{ typedef trigonometric_function<N, Alloc_> end; };

public:
	template<unsigned int N_>
	struct precision_0 : public endholder_0<N_>, trigonometric_function_TmpRec<N_, Alloc>{};
	template<typename Alloc_>
	struct precision_1 : public endholder_1<Alloc_>, trigonometric_function_TmpRec<N, Alloc_>{};
};

template<unsigned int N, typename Alloc>
class trigonometric_function : trigonometric_function_TmpRec<N, Alloc>{
#define TTY_PI_ static_cast<arithmetic_type>(3.1415926535897932384626433832795)
#define TTY_PI2_ static_cast<arithmetic_type>(6.283185307179586476925286766559)
public:
	typedef Alloc allocator_type;
	typedef int angle_t;
	enum{
		pi2 = 1 << N,		//一周
		pi = pi2 / 2,		//半周
		mask = pi2 - 1
	};

private:
	typedef trigonometric_function<N> this_t;
	typedef typename allocator_type::template rebind<char>::other char_allocator_type;
	char_allocator_type cal;
	char *mem;
	arithmetic_type *sin_, *tan_;
	angle_t *atan_;

public:
	trigonometric_function() :
		cal(),
		mem(cal.allocate((sizeof(arithmetic_type) * pi2 * 2) + (sizeof(angle_t) * (pi / 4)))),
		sin_	(reinterpret_cast<arithmetic_type*>(mem)),
		tan_	(reinterpret_cast<arithmetic_type*>(mem + sizeof(arithmetic_type) * pi2)),
		atan_	(reinterpret_cast<angle_t*>(mem + sizeof(arithmetic_type) * pi2 * 2))
	{
		//テーブルを作成
		const arithmetic_type
			acc = TTY_PI2_ / static_cast<arithmetic_type>(pi2),	// sin tan の加速度
			omega = static_cast<arithmetic_type>(1) / static_cast<arithmetic_type>(pi / 4),	// atan の加速度
			q = TTY_PI_ / static_cast<arithmetic_type>(4);			// 180 / 4

		arithmetic_type t = static_cast<arithmetic_type>(0);
		for(angle_t i = 0; i < pi2; t += acc, i++){
			sin_[i] = ::sin(t);
			tan_[i] = ::tan(t);
		}

		t = 0;
		for(angle_t i = 0; i < (pi / 4); t += omega, i++){
			atan_[i] = static_cast<angle_t>(static_cast<arithmetic_type>(pi / 4) * ::atan(t) / q);
		}
	}

	virtual ~trigonometric_function(){
		cal.deallocate(mem, (sizeof(arithmetic_type) * pi2 * 2) + (sizeof(angle_t) * (pi / 4)));
	}

	const arithmetic_type &sin(const angle_t &a)const{ return sin_[a & mask]; }
	const arithmetic_type &cos(const angle_t &a)const{ return sin_[(a + pi / 2) & mask]; }
	const arithmetic_type &tan(const angle_t &a)const{ return tan_[a & mask]; }

public:
	angle_t atan2(const arithmetic_type &y, const arithmetic_type &x) const{
#if 1
		//どう頑張っても組み込みatan2の方が速いと思う場合
		angle_t a = static_cast<angle_t>(::atan2(y, x) * static_cast<arithmetic_type>(pi) / TTY_PI_);
		if(a < 0){ return pi2 + a; }else{ return a; }
#else
#define absolute_(a) (a < 0 ? -a : a)

		angle_t s = 0;
		arithmetic_type ax, ay;

		if(x < 0){
			ax = -x;
			s += 1;
		}else{ ax = x; }

		if(y < 0){
			ay = -y;
			s += 2;
		}else{ ay = y; }

		angle_t i;
		bool f;

		if(ax > ay){
			i = static_cast<angle_t>(static_cast<angle_t>(pi / 4) * ay / ax);
			f = (s == 0 || s == 3) ? 1 : 0;
		}else if(ay > ax){
			i = static_cast<angle_t>(static_cast<arithmetic_type>(pi / 4) * ax / ay);
			f = (s == 0 || s == 3) ? 0 : 1;
		}else{//ax == ay
			static const angle_t t[4] = {
				pi / 4,
				pi / 4 * 3,
				pi / 4 * 7,
				pi / 4 * 5,
			};

			return t[s];
		}

		static const angle_t t_[4] = {
			0,
			pi / 2,
			pi / 2 * 3,
			pi / 2 * 2,
		};

		if(f){
			return atan_[i] + t_[s];
		}else{
			return atan_[(pi / 4) - 1] + t_[s] + (pi / 4);
		}

#undef absolute_
#endif
	}

	angle_t atan2_cross(const arithmetic_type &x, const arithmetic_type &y) const{
		return atan2(y, x);
	}

#undef TTY_PI_
#undef TTY_PI2_
};
