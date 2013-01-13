/*
    êîäwånÇÃÇ‡ÇÃ
*/

#ifndef __TTY_ALIB__
#define __TTY_ALIB__

#include <deque>
#include <algorithm>
#include <utility>
#include <complex>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace tty{
    template<typename tty_arithmetic_type_ = double> struct alib{
        template<typename T>
        struct rebind{ typedef alib<T> other; };
        typedef tty_arithmetic_type_ arithmetic_type;
        typedef alib<arithmetic_type> tty_alib;
        template<typename T>
        inline static arithmetic_type num(T a){ return static_cast<arithmetic_type>(a); }
        inline static arithmetic_type epsilon(){ return std::numeric_limits<arithmetic_type>::epsilon(); }
        inline static arithmetic_type infinity(){ return std::numeric_limits<arithmetic_type>::infinity(); }
        inline static arithmetic_type pi(){ static arithmetic_type a = num(6) * std::asin(num(0.5)); return a; }
        inline static arithmetic_type pi2(){ return pi() * num(2); }
        inline static arithmetic_type golden_ratio(){ static arithmetic_type a = (num(1) + std::sqrt(num(5))) / num(2); return a; }
//#include "alib/neville_interpolation.hpp"
//#include "alib/newton_interpolation.hpp"
//#include "alib/bessel.hpp"
//#include "alib/runge_kutta4.hpp"
//#include "alib/composite_simpsons_rule.hpp"
//#include "alib/average_box.hpp"
//#include "alib/logit.hpp"
//#include "alib/log.hpp"
//#include "alib/sigmoid.hpp"
//#include "alib/newtons_method.hpp"
//#include "alib/fft_box.hpp"
//#include "alib/gamma.hpp"
//#include "alib/summation.hpp"
//#include "alib/fact.hpp"
//#include "alib/combination.hpp"
//#include "alib/bernoulli_number.hpp"
//#include "alib/pi.hpp"
//#include "alib/divisor.hpp"
//#include "alib/least_squares.hpp"
#include "alib/trigonometric_function.hpp"
    };
}

#endif // __TTY_ALIB__
