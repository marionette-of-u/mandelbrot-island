#include <string>
#include <iterator>
#include <random>
#include <fstream>
#include <locale>
#include <vector>
#include <memory>
#include <array>
#include <functional>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <complex>
#include <boost/preprocessor.hpp>
#include <mpirxx.h>
#include "DxLib.h"
#include "bmp.hpp"
#include "alib.hpp"

struct set_locale_jp{
    set_locale_jp(){
        setlocale(LC_CTYPE, "");
        std::locale::global(std::locale("japanese"));
    }
} set_locale_jp_instance;

namespace const_value{
    enum{
        maxiter_min = 0x100,
        maxiter_max = 0x1000000,
        maxiter_addnum = maxiter_min,
        omega_max = 0x200 - 1,
        mandelbrot_width = 512,
        mandelbrot_height = mandelbrot_width,
        ui_margin = 2,
        ui_offset = mandelbrot_height,
        ui_width = mandelbrot_width,
        ui_height = 160,
        char_margin = 2,
        char_width = 5,
        char_height = 7,
        char_box_width = char_width + char_margin,
        char_box_height = char_height + char_margin,
        hyphen_num = 24,
        omega_point_length = 6 * 2,
        omega_offset_y_r = ui_offset + ui_height - omega_point_length * 3,
        omega_offset_y_g = omega_offset_y_r + omega_point_length,
        omega_offset_y_b = omega_offset_y_g + omega_point_length,
        fn_name_offset_y = omega_offset_y_r - omega_point_length / 2 - 7 - ui_margin * 2,
        re_draw_offset_y = ui_offset + ui_margin,
        im_draw_offset_y = ui_offset + ui_margin * 3 + char_box_height * 3,
        zoom_ratio_draw_offset_y = ui_offset + ui_margin * 5 + char_box_height * 6,
        prec_draw_offset_y = zoom_ratio_draw_offset_y + ui_margin * 2 + char_box_height * 2,
        maxiter_draw_offset_y = prec_draw_offset_y + ui_margin * 2 + char_box_height * 2,
        window_width = mandelbrot_width,
        window_height = mandelbrot_height + ui_height,
    };

    const double xmin = -1.0, xmax = 2.0, ymin = -1.5, ymax = 1.5;
    const double pi = 3.1415926535897932384626433832795;
    const double e = 2.7182818284590452353602874713527;
    const double fullcolor_num = 16777216.0;
}

class input_manager_type{
public:
    input_manager_type(){
        for(int i = 0; i < 0x100; ++i){
            key_buffer[i] = 0;
            prev_flag[i] = false;
        }
        GetMousePoint(&cursor_x[0], &cursor_y[0]);
        cursor_x[1] = cursor_x[0];
        cursor_y[1] = cursor_y[0];
        mouse_state = prev_mouse_state = GetMouseInput();
    }

    void operator ()(){
        for(int i = 0; i < 0x100; ++i){
            prev_flag[i] = key_buffer[i] == 1 ? true : false;
        }
        GetHitKeyStateAll(key_buffer);
        cursor_x[1] = cursor_x[0];
        cursor_y[1] = cursor_y[0];
        GetMousePoint(&cursor_x[0], &cursor_y[0]);
        prev_mouse_state = mouse_state;
        mouse_state = GetMouseInput();
    }

    int cursor_coord_x() const{
        return cursor_x[0];
    }

    int prev_cursor_coord_x() const{
        return cursor_x[1];
    }

    int cursor_coord_y() const{
        return cursor_y[0];
    }

    int prev_cursor_coord_y() const{
        return cursor_y[1];
    }

    bool press(int n) const{
        return key_buffer[n] == 1;
    }

    bool push(int n) const{
        return key_buffer[n] == 1 && !prev_flag[n];
    }

    bool release(int n) const{
        return key_buffer[n] != 1 && prev_flag[n];
    }

    bool mouse_press(int n) const{
        return (mouse_state & n) != 0;
    }

    bool mouse_push(int n) const{
        return (mouse_state & n) != 0 && !((prev_mouse_state & n) != 0);
    }

    bool mouse_release(int n) const{
        return !((mouse_state & n) != 0) && (prev_mouse_state & n) != 0;
    }

private:
    char key_buffer[0x100];
    bool prev_flag[0x100];
    int cursor_x[2], cursor_y[2];
    int mouse_state, prev_mouse_state;
} input_manager;

class patter_surface_type{
public:
    void load(){
        pattern_surface = LoadGraph("d/pattern.bmp");
        if(pattern_surface == -1){
            std::abort();
        }
    }

    int get() const{
        return pattern_surface;
    }

private:
    int pattern_surface;
} pattern_surface;

namespace pattern{
    class pattern{
    public:
        pattern(const patter_surface_type &pattern_surface_, int x_, int y_, int w_, int h_) :
            pattern_surface(&pattern_surface_),
            x(x_),
            y(y_),
            w(w_),
            h(h_)
        {}

        void draw(int dest_x, int dest_y) const{
            DrawRectGraph(dest_x, dest_y, x, y, w, h, pattern_surface->get(), FALSE, FALSE);
        }

    private:
        pattern(){}
        const patter_surface_type *pattern_surface;
        int x, y, w, h;
    };

    pattern
        num_dot(pattern_surface, 80, 48, 5, 7),
        num_plus(pattern_surface, 80 + 5, 48, 5, 7),
        num_minus(pattern_surface, 80 + 5 * 2, 48, 5, 7),
        num_e(pattern_surface, 80 + 5 * 3, 48, 5, 7),
        char_x(pattern_surface, 0, 56, 4, 6),
        char_y(pattern_surface, 4, 56, 4, 6),
        char_hyphen(pattern_surface, 8, 56, 4, 6),
        char_p(pattern_surface, 12, 56, 4, 6),
        char_n(pattern_surface, 16, 56, 4, 6),
        char_z(pattern_surface, 20, 56, 4, 6),
        char_s(pattern_surface, 24, 56, 4, 6),
        str_valueislimit(pattern_surface, 0, 62, 129, 7),
        str_failed(pattern_surface, 0, 69, 18, 7),
        str_sin(pattern_surface, 0, 76, 20, 7),
        str_cos(pattern_surface, 20, 76, 20, 7),
        str_tan(pattern_surface, 40, 76, 20, 7);

    pattern gimg_zero(pattern_surface, 0, 0, 16, 16);
#define WRITE(z, i, nil) pattern gimg_ ## i(pattern_surface, 16 + i * 16, 0, 16, 16);
    BOOST_PP_REPEAT(4, WRITE,  nil);
#undef WRITE
    pattern *gimg[4] = {
#define WRITE(z, i, nil) BOOST_PP_COMMA_IF(i) &gimg_ ## i
        BOOST_PP_REPEAT(4, WRITE, nil)
#undef WRITE
    };

#define WRITE(z, i, nil) pattern num_ ## i(pattern_surface, 0 + i * 5, 48, 5, 7);
    BOOST_PP_REPEAT(16, WRITE,  nil);
#undef WRITE
    pattern *num[20] = {
#define WRITE(z, i, nil) BOOST_PP_COMMA_IF(i) &num_ ## i
        BOOST_PP_REPEAT(16, WRITE, nil)
#undef WRITE
        , &num_dot, &num_plus, &num_minus, &num_e
    };
}

template<class T>
std::string value_to_string(const T &a){
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    std::size_t n = 0;
    for(; ; ){
        n = str.find(",", n);
        if(n == std::string::npos){
            break;
        }
        std::distance(str.begin(), str.erase(str.begin() + n));
    }
    return str;
}

std::string make_filename(std::string ext){
    std::time_t time = std::time(nullptr);
    std::tm *tm = std::localtime(&time);
    std::stringstream r;
    r
        << value_to_string(tm->tm_year + 1900).c_str()
        << "_"
        << (tm->tm_mon + 1)
        << "_"
        << (tm->tm_mday)
        << "_"
        << (tm->tm_hour)
        << "_"
        << (tm->tm_min)
        << "_"
        << (tm->tm_sec)
        << "."
        << ext.c_str();
    return r.str();
}

mpf_class mpf_ln(const mpf_class &x){
    mpf_class r;
    mp_bitcnt_t prec = x.get_prec();
    if(x > 0.5){
        mpf_class term = 1.0 - (1.0 / x), term_k = term, last = r;
        r = term_k;
        term_k *= term;
        r += term_k / 2.0;
        for(std::size_t i = 3; !mpf_eq(r.get_mpf_t(), last.get_mpf_t(), prec); ++i){
            last = r;
            term_k *= term;
            r += term_k / i;
        }
    }else if(x > 0.0){
        mpf_class term = x - 1.0, term_k = term, factor = 1.0, last = r;
        r = term;
        int sign = -1;
        term_k *= term;
        r += -term_k / 2.0;
        for(std::size_t i = 3; !mpf_eq(r.get_mpf_t(), last.get_mpf_t(), prec); ++i){
            last = r;
            term_k *= term;
            sign *= -1;
            r += term_k * sign / i;
        }
    }
    return r;
}

std::string make_mpf_exp_str(const mpf_class &a, int radix, int digit_limit, int num_width){
    mp_exp_t e;
    std::string f_str = a.get_str(e, radix);
    if(f_str.find(".") == std::string::npos){
        f_str.insert(a >= 0 ? 1 : 2, ".");
        if(f_str.back() == '.'){
            f_str.append("0");
        }
    }
    if(e == 0){ e = -1; }
    std::string e_str = "E" + std::string(e >= 0 ? "+" : "") + value_to_string(e);
    std::size_t e_size = e_str.size(), f_size = f_str.size();
    std::size_t length = digit_limit / num_width;
    if(f_size > length - e_size){
        f_str.resize(length - e_size);
    }
    return f_str + e_str;
}

std::string make_mpf_format_str(const mpf_class &a, int radix){
    if(a == 0){ return "0.0"; }
    mp_exp_t e;
    std::string r = a.get_str(e, radix), s;
    if(e == 0){
        s = "0.";
        if(a >= 0){
            return r.insert(0, s);
        }else{
            return r.insert(1, s);
        }
    }else if(e > 0){
        s = ".";
        if(r.size() == (e + (a < 0 ? 1 : 0))){ s = ""; }
        if(a >= 0){
            if(r.size() < static_cast<std::size_t>(e)){
                for(std::size_t i = 0, n = e - r.size(); i < n; ++i){
                    r.append("0");
                }
                return r;
            }else{
                return r.insert(e, s);
            }
        }else{
            if(s.size() == 0){
                return r;
            }else{
                return r.insert(e + 1, s);
            }
        }
    }else{
        s = "0.";
        e = -e;
        for(int i = 0; i < e; ++i){ s += '0'; }
        if(a >= 0){
            return r.insert(0, s);
        }else{
            return r.insert(1, s);
        }
    }
    return r;
}

struct plain_thread_draw_data{
    int fn_pattern_r, fn_pattern_g, fn_pattern_b;
    unsigned int rad_a, rad_b, rad_c;
    int maxiter;
    plain_thread_draw_data() :
        fn_pattern_r(0), fn_pattern_g(0), fn_pattern_b(0),
        rad_a(0), rad_b(0), rad_c(0),
        maxiter(const_value::maxiter_min)
    {}

    bool operator ==(const plain_thread_draw_data &other) const{
        return
            fn_pattern_r == other.fn_pattern_r &&
            fn_pattern_g == other.fn_pattern_g &&
            fn_pattern_b == other.fn_pattern_b &&
            rad_a == other.rad_a &&
            rad_b == other.rad_b &&
            rad_c == other.rad_c &&
            maxiter == other.maxiter;
    }

    bool operator !=(const plain_thread_draw_data &other) const{
        return !(*this == other);
    }
};

struct thread_draw_data : plain_thread_draw_data{
    mpf_class xmin, xmax, ymin, ymax;
    thread_draw_data() : plain_thread_draw_data(), xmin(), xmax(), ymin(), ymax(){}

    thread_draw_data &operator =(const thread_draw_data &other){
        xmin = other.xmin;
        xmax = other.xmax;
        ymin = other.ymin;
        ymax = other.ymax;
        static_cast<plain_thread_draw_data&>(*this) = static_cast<const plain_thread_draw_data&>(other);
        return *this;
    }

    void set_prec(mp_bitcnt_t p){
        xmin.set_prec(p);
        xmax.set_prec(p);
        ymin.set_prec(p);
        ymax.set_prec(p);
    }

    bool coord_equal(const thread_draw_data &other) const{
        return xmin == other.xmin && xmax == other.xmax && ymin == other.ymin && ymax == other.ymax;
    }

    bool test_conversion_coloring(const thread_draw_data &other) const{
        return static_cast<const plain_thread_draw_data&>(*this) != static_cast<const plain_thread_draw_data&>(other) && coord_equal(other);
    }

    void save() const{
        std::ofstream ofile("save.txt");
        ofile << value_to_string(xmin.get_prec()) << "\n";
        ofile << make_mpf_format_str(-xmax, 16) << "\n";
        ofile << make_mpf_format_str(-xmin, 16) << "\n";
        ofile << make_mpf_format_str(ymin, 16) << "\n";
        ofile << make_mpf_format_str(ymax, 16) << "\n";
        ofile << value_to_string(fn_pattern_r) << "\n";
        ofile << value_to_string(fn_pattern_g) << "\n";
        ofile << value_to_string(fn_pattern_b) << "\n";
        ofile << value_to_string(rad_a) << "\n";
        ofile << value_to_string(rad_b) << "\n";
        ofile << value_to_string(rad_c) << "\n";
        ofile << value_to_string(maxiter) << "\n";
        if(ofile.fail()){ std::abort(); }
    }

    void load(){
        std::ifstream ifile("save.txt");
        std::string str;
        unsigned int q;
        if(ifile.fail()){ goto err; }
        ifile >> q;
        mpf_set_default_prec(q);
        xmin.set_prec(q), xmax.set_prec(q), ymin.set_prec(q), ymax.set_prec(q);
#define READ_MPF(a) READ_F(str); a.set_str(str, 16)
#define READ_F(a) ifile >> a; if(ifile.fail()){ goto err; }
        READ_MPF(xmax), READ_MPF(xmin), READ_MPF(ymin), READ_MPF(ymax);
        READ_F(fn_pattern_r);
        READ_F(fn_pattern_g);
        READ_F(fn_pattern_b);
        READ_F(rad_a);
        READ_F(rad_b);
        READ_F(rad_c);
        READ_F(maxiter);
#undef READ_F
#undef READ_MPF
        xmin = -xmin;
        xmax = -xmax;
        return;

err:;
        std::ofstream ofile("error.txt");
        if(ofile.fail()){ std::abort(); }
        ofile << "save.txt‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s\n";
        mpf_set_default_prec(0);
#define SET_X(a, b) a.set_prec(0), a = b
        SET_X(xmin, const_value::xmin), SET_X(xmax, const_value::xmax), SET_X(ymin, const_value::ymin), SET_X(ymax, const_value::ymax);
#undef SET_X
        fn_pattern_b = fn_pattern_g = fn_pattern_r = 0;
        rad_c = rad_b = rad_a = const_value::omega_max;
        maxiter = const_value::maxiter_min;
    }
};

typedef std::array<unsigned char, 3> rgb_type;
struct tmp_complex{
    tmp_complex(const mpf_class &re2_, const mpf_class &im2_) :
        re2(re2_), im2(im2_)
    {}

    const mpf_class &re2, &im2;
};
typedef std::function<void(tmp_complex, int, int, int, const rgb_type*)> set_pixel_fn_type;
typedef tty::alib<>::trigonometric_function<16> tf_type;
tf_type tf;

struct common_data{
    set_pixel_fn_type set_pixel;
    std::pair<int, int> size;
    std::pair<int, int> height_begin_end;
};

class disp_data_type{
public:
    enum char_kind{
        number_0 = 0,
        number_1, number_2, number_3,
        number_4, number_5, number_6,
        number_7, number_8, number_9,
        number_a, number_b, number_c,
        number_d, number_e, number_f,
        char_dot, char_plus, char_minus, char_e
    };

    typedef std::vector<char_kind> char_array_type;

public:
    disp_data_type(){
        for(int i = 0; i < 2; ++i){
            ptr_xmin[i] = &xmin[i];
            ptr_xmax[i] = &xmax[i];
            ptr_ymin[i] = &ymin[i];
            ptr_ymax[i] = &ymax[i];
            ptr_zoom_ratio[i] = &zoom_ratio[i];
        }
    }

    void make_disp_data(
        const mpf_class &value_xmin,
        const mpf_class &value_xmax,
        const mpf_class &value_ymin,
        const mpf_class &value_ymax
    ){
        auto to_symbol = [&](const std::string str, char_array_type &vec, int i){
            for(auto iter = str.cbegin(), end = str.cend(); iter != end; ++iter, ++i){
                switch(*iter){
                    case '0': vec.push_back(number_0); break;
                    case '1': vec.push_back(number_1); break;
                    case '2': vec.push_back(number_2); break;
                    case '3': vec.push_back(number_3); break;
                    case '4': vec.push_back(number_4); break;
                    case '5': vec.push_back(number_5); break;
                    case '6': vec.push_back(number_6); break;
                    case '7': vec.push_back(number_7); break;
                    case '8': vec.push_back(number_8); break;
                    case '9': vec.push_back(number_9); break;
                    case 'a': vec.push_back(number_a); break;
                    case 'b': vec.push_back(number_b); break;
                    case 'c': vec.push_back(number_c); break;
                    case 'd': vec.push_back(number_d); break;
                    case 'e': vec.push_back(number_e); break;
                    case 'f': vec.push_back(number_f); break;
                    case '.': vec.push_back(char_dot); break;
                    case '+': vec.push_back(char_plus); break;
                    case '-': vec.push_back(char_minus); break;
                    case 'E': vec.push_back(char_e); break;
                    default: break;
                }
            }
        };
        auto proc = [&](const mpf_class &value, char_array_type &vec, int radix){
            vec.clear();
            std::string str = make_mpf_format_str(value, radix);
            if(str.find('.', 0) == std::string::npos){ str += ".0"; }
            if((str.size() + (value < 0 ? 1 : 0)) * const_value::char_width >= const_value::window_width - const_value::char_width * 16){
                str.clear();
                str = make_mpf_exp_str(value, radix, const_value::ui_width - const_value::char_width * 16, const_value::char_width);
            }
            std::size_t i = 0;
            if(value >= 0){
                str.insert(0, "+");
                ++i;
            }
            to_symbol(str, vec, i);
        };
        auto proc_z = [&](const mpf_class &value, char_array_type &vec, int radix){
            vec.clear();
            std::string str = make_mpf_exp_str(value, radix, const_value::ui_width - const_value::char_width * 16, const_value::char_width);
            std::size_t i = 0;
            if(value >= 0){
                str.insert(0, "+");
                ++i;
            }
            to_symbol(str, vec, i);
        };
        proc(value_xmin, xmin[0], 10);
        proc(value_xmin, xmin[1], 16);
        proc(value_xmax, xmax[0], 10);
        proc(value_xmax, xmax[1], 16);
        proc(value_ymin, ymin[0], 10);
        proc(value_ymin, ymin[1], 16);
        proc(value_ymax, ymax[0], 10);
        proc(value_ymax, ymax[1], 16);
        mpf_class z = ((const_value::xmax - const_value::xmin) * (const_value::ymax - const_value::ymin)) / ((value_xmax - value_xmin) * (value_ymax - value_ymin));
        z = abs(z);
        proc_z(z, zoom_ratio[0], 10);
        proc_z(z, zoom_ratio[1], 16);
    }

public:
    const char_array_type
        *ptr_xmin[2], *ptr_xmax[2], *ptr_ymin[2], *ptr_ymax[2], *ptr_zoom_ratio[2];

private:
    char_array_type xmin[2], xmax[2], ymin[2], ymax[2], zoom_ratio[2];
} disp_data;

volatile bool exit_flag = false;
volatile bool abort_compute_flag = false;
const unsigned int thread_num = std::thread::hardware_concurrency();

class mandelbrot_type{
public:
    mandelbrot_type() :
        ref_color_plt(color_plt),
        ref_data(data),
        ref_plt_img(plt_img),
        bmp(nullptr),
        thread_array(new std::thread[thread_num]),
        common_data_array(new common_data[thread_num]),
        generate_flags(new volatile bool[thread_num]),
        color_plt(nullptr),
        plt_img(new short[const_value::mandelbrot_width * const_value::mandelbrot_width]),
        non_diverge_point_to_black(false),
        prev_non_diverge_point_to_black(false),
        smooth_draw_flag(false)
    {
        data.xmin = const_value::xmin, data.xmax = const_value::xmax;
        data.ymin = const_value::ymin, data.ymax = const_value::ymax;
        data.fn_pattern_r = data.fn_pattern_g = data.fn_pattern_b = 0;
        data.rad_a = data.rad_b = data.rad_c = const_value::omega_max;
        data_cache.maxiter = 0;
        for(unsigned int i = 0; i < thread_num; ++i){
            generate_flags[i] = false;
        }
    }

    inline static rgb_type rgb(unsigned char r, unsigned char g, unsigned char b){
        rgb_type a;
        a[0] = r, a[1] = g, a[2] = b;
        return a;
    }

private:
    struct function_pattern{
        enum{ sin = 0, cos = 1, tan = 2 };
        typedef void (*draw_fn_type)(rgb_type *color, int maxiter, unsigned int rad_a, unsigned int rad_b, unsigned int rad_c, tf_type &tf);
        struct draw_fn{
#include "draw_fn.hpp"
        };

        static draw_fn_type draw_dispatch(int r, int g, int b){
            static draw_fn_type fn_array[3 * 3 * 3] = {
#include "draw_fn_enum.hpp"
            };
            return fn_array[r + g * 3 + b * 3 * 3];
        }
    };

    void join_generate_thread(){
        for(unsigned int i = 0; i < thread_num; ++i){
            if(thread_array[i].joinable()){ thread_array[i].join(); }
        }
    }

    void launch_draw_thread(set_pixel_fn_type draw_pixel, int width, int height){
        mutex_make_rec_thread.lock();
        for(unsigned int i = 0; i < thread_num; ++i){
            generate_flags[i] = true;
        }
        init_generate_thread();
        for(unsigned int i = 0; i < thread_num; ++i){
            common_data_array[i].set_pixel = draw_pixel;
            common_data_array[i].size.first = width;
            common_data_array[i].size.second = height;
            common_data_array[i].height_begin_end.first = i * height / thread_num;
            common_data_array[i].height_begin_end.second = (i + 1) * height / thread_num;
            thread_array[i] = std::thread(&mandelbrot_type::make, std::ref(*this), i, std::ref(common_data_array[i]));
        }
        mutex_make_rec_thread.unlock();
    }

public:
    void set_prec(mp_bitcnt_t n){
        data.xmin.set_prec(n);
        data.xmax.set_prec(n);
        data.ymin.set_prec(n);
        data.ymax.set_prec(n);
    }

    void join_all_thread(){
        if(bmp_thread.joinable()){ bmp_thread.join(); }
        join_generate_thread();
    }

    bool test_joinable_all_thread(){
        mutex_make_rec_thread.lock();
        for(unsigned int i = 0; i < thread_num; ++i){
            if(thread_array[i].joinable()){
                mutex_make_rec_thread.unlock();
                return true;
            }
        }
        mutex_make_rec_thread.unlock();
        return false;
    }

    bool make_color_plt(bool reset_flag = true){
        if(non_diverge_point_to_black == prev_non_diverge_point_to_black && static_cast<const plain_thread_draw_data&>(data) == data_cache){
            return false;
        }
        if(data.maxiter != data_cache.maxiter){
            if(reset_flag){
                color_plt.reset(new rgb_type[data.maxiter]);
                data_cache.maxiter = data.maxiter;
            }else{
                return false;
            }
        }
        data_cache = static_cast<const plain_thread_draw_data&>(data);
        function_pattern::draw_dispatch(data.fn_pattern_r, data.fn_pattern_g, data.fn_pattern_b)
            (&color_plt[0], data.maxiter, data.rad_a, data.rad_b, data.rad_c, tf);
        if(non_diverge_point_to_black){
            for(int i = 0; i < 3; ++i){ color_plt[data.maxiter - 1][i] = 0; }
        }
        prev_non_diverge_point_to_black = non_diverge_point_to_black;
        return true;
    }

    int plt_idx(int x, int y) const{
        return plt_img[y * const_value::mandelbrot_width + x];
    }

    void plt_idx(int x, int y, int n){
        plt_img[y * const_value::mandelbrot_width + x] = static_cast<short>(n);
    }

    bool remake(std::function<void(int, int, rgb_type)> f){
        if(!make_color_plt(false)){ return false; }
        for(int i = 0; i < thread_num; ++i){
            thread_array[i] = std::thread(
                [&](int y_begin, int y_end){
                    for(int y = y_begin; y < y_end; ++y){
                        for(int x = 0; x < const_value::mandelbrot_width; ++x){
                            f(x, y, color_plt[plt_img[y * const_value::mandelbrot_width + x]]);
                        }
                    }
                },
                i * const_value::mandelbrot_height / thread_num,
                (i + 1) * const_value::mandelbrot_height / thread_num
            );
        }
        join_all_thread();
        return true;
    }

    bool next_mandelbrot(set_pixel_fn_type draw_pixel, int width, int height){
        if(
            test_joinable_all_thread() ||
            data.xmin == data.xmax ||
            data.ymin == data.ymax ||
            data.xmax - data.xmin == 0 ||
            data.ymax - data.ymin == 0
        ){ return false; }
        join_all_thread();
        cache_array.push_back(data);
        make_color_plt();
        disp_data.make_disp_data(-data.xmin, -data.xmax, data.ymin, data.ymax);
        launch_draw_thread(draw_pixel, width, height);
        return true;
    }

    bool prev_mandelbrot(set_pixel_fn_type draw_pixel, int width, int height){
        if(test_joinable_all_thread() || cache_array.size() <= 1){ return false; }
        join_all_thread();
        cache_array.pop_back();
        mp_bitcnt_t p = cache_array.back().xmax.get_prec();
        data = cache_array.back();
        data.set_prec(p);
        mpf_set_default_prec(p);
        make_color_plt();
        disp_data.make_disp_data(-data.xmin, -data.xmax, data.ymin, data.ymax);
        launch_draw_thread(draw_pixel, width, height);
        return true;
    }

    bool put_mandelbrot(set_pixel_fn_type draw_pixel, int width, int height){
        if(test_joinable_all_thread()){ return false; }
        try{
            if(bmp_thread.joinable()){
                bmp_thread.join();
            }
            bmp = new tty::bmp(data.maxiter == 256 ? tty::bmp::ColorNum::n256 : tty::bmp::ColorNum::b24, width, height);
            bmp_thread = std::thread(
                [&](int w, int h){
                    join_all_thread();
                    make_color_plt();
                    if(data.maxiter == 256){
                        for(int i = 0; i < 0x100; ++i){
                            rgb_type a = color_plt[i];
                            bmp->setplt(i, tty::bmp::rgb(a[0], a[1], a[2]));
                        }
                    }
                    set_pixel_fn_type
                        f8 = [&](tmp_complex, int x, int y, int idx, const rgb_type*){
                            bmp->pltidx(x, y, idx);
                        },
                        f24 = [&](tmp_complex, int x, int y, int idx, const rgb_type *c){
                            bmp->clr(x, y, tty::bmp::rgb(c[idx][0], c[idx][1], c[idx][2]));
                        };
                    launch_draw_thread(data.maxiter == 256 ? f8 : f24, w, h);
                    for(; ; ){
                        bool a = false;
                        for(unsigned int i = 0; i < thread_num; ++i){
                            if(generate_flags[i]){
                                a = true;
                                break;
                            }
                        }
                        if(!a){ break; }
                    }
                    bmp->write(make_filename("bmp").c_str());
                    delete bmp;
                    bmp = nullptr;
                },
                width,
                height
            );
        }catch(std::domain_error e){
            return false;
        }
        return true;
    }

    void proc(){
        mutex_make_rec_thread.lock();
        for(unsigned int i = 0; i < thread_num; ++i){
            if(!generate_flags[i] && thread_array[i].joinable()){ thread_array[i].detach(); }
        }
        mutex_make_rec_thread.unlock();
    }

    void init_generate_thread(){
        if(abort_compute_flag){
            for(unsigned int i = 0; i < thread_num; ++i){
                if(thread_array[i].joinable()){
                    thread_array[i].join();
                    thread_array[i].detach();
                }
            }
            abort_compute_flag = false;
        }
    }

    void make(
        const int thread_idx,
        const common_data &common
    ){
        const int
            width = common.size.first,
            height = common.size.second,
            height_begin = common.height_begin_end.first,
            height_end = common.height_begin_end.second;
        mpf_class dx = data.xmax - data.xmin, dy = data.ymax - data.ymin;
        generate_flags[thread_idx] = true;
        if(dx == 0 || dy == 0){
            generate_flags[thread_idx] = false;
            return;
        }
        int i_max = width, j_max = height_end;
        std::unique_ptr<int[]> interrupt_array(new int[thread_num]);
        auto recursive_launch_other_thread = [&](const int y, const int end){
            int n = 0;
            for(unsigned int i = 0; i < thread_num; ++i){
                if(i == thread_idx || thread_array[i].joinable()){
                    continue;
                }
                interrupt_array[n++] = i;
            }
            int m = end - y;
            if(m < n){ n = m; }
            if(n == 0){ return end; }
            for(int i = 0; ; ++i){
                if(i + 1 > n){ break; }
                int p = (i + 1) * m / (n + 1), q = (i + 2) * m / (n + 1);
                int idx = interrupt_array[i];
                common_data_array[idx].set_pixel = common.set_pixel;
                common_data_array[idx].size = common.size;
                common_data_array[idx].height_begin_end.first = y + p;
                common_data_array[idx].height_begin_end.second = y + q;
                generate_flags[idx] = true;
                thread_array[idx] = std::thread(
                    &mandelbrot_type::make,
                    std::ref(*this),
                    idx,
                    std::ref(common_data_array[idx])
                );
            }
            int ret = y + m / (n + 1) + 1;
            if(ret >= height){
                ret = height - 1;
            }
            return ret;
        };
        mpf_class x, y, a, b, a2, b2;
        dx /= width, dy /= height;
        const mpf_class two = 2, limit = 4;
        for(int j = height_begin; j <= j_max; ++j){
            if(!abort_compute_flag && !exit_flag){
                mutex_make_rec_thread.lock();
                j_max = recursive_launch_other_thread(j, j_max);
                mutex_make_rec_thread.unlock();
            }
            y = data.ymax - j * dy;
            for(int i = 0; i <= i_max; ++i){
                x = data.xmin + i * dx;
                a = x, b = y;
                a2 = a * a;
                b2 = b * b;
                int count = 0;
                for(; ; ){
                    if(exit_flag){ return; }
                    if(abort_compute_flag){
                        generate_flags[thread_idx] = false;
                        return;
                    }
                    if(!(count != data.maxiter && a2 + b2 <= limit)){ break; }
                    b = a * b * two - y;
                    a = a2 - b2 - x;
                    a2 = a * a;
                    b2 = b * b;
                    ++count;
                }
                if(!(i == width || j == height)){
                    common.set_pixel(tmp_complex(a2, b2), width - i - 1, j, count != data.maxiter ? count : data.maxiter - 1, &color_plt[0]);
                }else{ break; }
            }
        }
        generate_flags[thread_idx] = false;
    }

    void next_red_fn(){
        ++data.fn_pattern_r;
        data.fn_pattern_r %= 3;
    }

    void next_green_fn(){
        ++data.fn_pattern_g;
        data.fn_pattern_g %= 3;
    }

    void next_blue_fn(){
        ++data.fn_pattern_b;
        data.fn_pattern_b %= 3;
    }
    
    void set_red_omega(int n){
        if(n < 0){
            n = 0;
        }else if(n > const_value::omega_max){
            n = const_value::omega_max;
        }
        data.rad_a = n;
    }
    
    void set_green_omega(int n){
        if(n < 0){
            n = 0;
        }else if(n > const_value::omega_max){
            n = const_value::omega_max;
        }
        data.rad_b = n;
    }
    
    void set_blue_omega(int n){
        if(n < 0){
            n = 0;
        }else if(n > const_value::omega_max){
            n = const_value::omega_max;
        }
        data.rad_c = n;
    }

    void add_maxiter(int n){
        data.maxiter += n;
        if(data.maxiter > const_value::maxiter_max){
            data.maxiter = const_value::maxiter_max;
        }else if(data.maxiter < const_value::maxiter_min){
            data.maxiter = const_value::maxiter_min;
        }
    }

    void set_coord(const mpf_class &xmin, const mpf_class &xmax, const mpf_class &ymin, const mpf_class &ymax){
        data.xmin = xmin, data.xmax = xmax, data.ymin = ymin, data.ymax = ymax;
    }

    void save() const{
        data.save();
    }
    
    void load(){
        data.load();
    }

    void clear_cache(){
        cache_array.clear();
    }

    void switch_mset_to_black(){
        prev_non_diverge_point_to_black = non_diverge_point_to_black;
        non_diverge_point_to_black = !non_diverge_point_to_black;
    }

    void switch_smooth_draw(){
        smooth_draw_flag = !smooth_draw_flag;
    }

    bool get_smooth_draw_flag() const{
        return smooth_draw_flag;
    }

public:
    const std::unique_ptr<rgb_type[]> &ref_color_plt;
    const thread_draw_data &ref_data;
    const std::unique_ptr<short[]> &ref_plt_img;

private:
    thread_draw_data data;
    plain_thread_draw_data data_cache;
    tty::bmp *bmp;
    std::unique_ptr<std::thread[]> thread_array;
    std::thread bmp_thread;
    std::unique_ptr<common_data[]> common_data_array;
    std::unique_ptr<volatile bool[]> generate_flags;
    std::unique_ptr<rgb_type[]> color_plt;
    std::unique_ptr<short[]> plt_img;
    std::mutex mutex_make_rec_thread;
    std::vector<thread_draw_data> cache_array;
    bool non_diverge_point_to_black, prev_non_diverge_point_to_black;
    bool smooth_draw_flag;

private:
    mandelbrot_type(const mandelbrot_type&) : ref_color_plt(color_plt), ref_data(data), ref_plt_img(plt_img){}
} mandelbrot;

class ui_type{
private:
    enum mode_enum{
        mode_title,
        mode_mandelbrot
    };

    enum message_enum{
        message_n,
        message_value_is_limit,
        message_failed
    };

    enum dad_enum{
        dad_off,
        dad_on,
        dad_lock,
        dad_drop
    };

    typedef void (ui_type::*begin_draw_type)();
    typedef void (ui_type::*dad_proc_type)();
    typedef void (ui_type::*dad_draw_type)() const;

    void redraw(){
        if(DeleteGraph(draw_buffer_handle) == -1){
            std::abort();
        }
        draw_buffer_handle = CreateGraphFromSoftImage(buffer_handle);
    }

    void remake_draw(){
        int buffer_handle_ = buffer_handle;
        auto f = [buffer_handle_](int x, int y, rgb_type c){
            DrawPixelSoftImage_Unsafe_XRGB8(buffer_handle_, x, y, c[0], c[1], c[2]);
        };
        if(mandelbrot.remake(f)){
            redraw();
        }
    }

    void begin_draw_empty(){ return; }
    void dad_proc_empty(){ return; }
    void dad_draw_empty() const{ return; }

    void begin_draw_ctor(){
        visualization_proc = true;
        int buffer_handle_ = buffer_handle;
        mandelbrot.next_mandelbrot(
            [buffer_handle_](tmp_complex, int x, int y, int idx, const rgb_type *c){
                DrawPixelSoftImage_Unsafe_XRGB8(buffer_handle_, x, y, c[idx][0], c[idx][1], c[idx][2]);
                mandelbrot.plt_idx(x, y, idx);
            },
            const_value::mandelbrot_width,
            const_value::mandelbrot_height
        );
        begin_draw = &ui_type::begin_draw_dtor;
    }

    void begin_draw_dtor(){
        if(mandelbrot.test_joinable_all_thread()){ return; }
        begin_draw = &ui_type::begin_draw_empty;
        visualization_proc = false;
        mode = mode_mandelbrot;
    }

    void dad_proc_omega(){
        using namespace const_value;
        switch(dad){
        case dad_on:
            {
                dad_x_begin = input_manager.cursor_coord_x();
                if(dad_x_begin < 0){
                    dad_x_begin = 0;
                }else if(dad_x_begin >= window_width){
                    dad_x_begin = window_width - 1;
                }
                dad_y_begin = input_manager.cursor_coord_y();
                if(dad_y_begin < 0){
                    dad_y_begin = 0;
                }else if(dad_y_begin >= window_height){
                    dad_y_begin = window_height - 1;
                }
                dad = dad_lock;
            }
            break;

        case dad_lock:
            {
                if(input_manager.mouse_push(MOUSE_INPUT_RIGHT)){
                    dad = dad_off;
                    dad_proc = &ui_type::dad_proc_empty;
                    dad_draw = &ui_type::dad_draw_empty;
                }
                const int rad = input_manager.cursor_coord_x();
                if(dad_y_begin >= omega_offset_y_r - omega_point_length / 2 && dad_y_begin < omega_offset_y_r + omega_point_length / 2){
                    mandelbrot.set_red_omega(rad);
                    remake_draw();
                }else if(dad_y_begin >= omega_offset_y_g - omega_point_length / 2 && dad_y_begin < omega_offset_y_g + omega_point_length / 2){
                    mandelbrot.set_green_omega(rad);
                    remake_draw();
                }else if(dad_y_begin >= omega_offset_y_b - omega_point_length / 2 && dad_y_begin < omega_offset_y_b + omega_point_length / 2){
                    mandelbrot.set_blue_omega(rad);
                    remake_draw();
                }
            }
            break;

        case dad_drop:
            {
                dad = dad_off;
                dad_proc = &ui_type::dad_proc_empty;
                dad_draw = &ui_type::dad_draw_empty;
            }
            break;
        }
    }

    void dad_proc_zoom(){
        using namespace const_value;
        switch(dad){
        case dad_on:
            {
                dad_x_begin = input_manager.cursor_coord_x();
                if(dad_x_begin < 0){
                    dad_x_begin = 0;
                }else if(dad_x_begin >= mandelbrot_width){
                    dad_x_begin = mandelbrot_width - 1;
                }
                dad_y_begin = input_manager.cursor_coord_y();
                if(dad_y_begin < 0){
                    dad_y_begin = 0;
                }else if(dad_y_begin >= mandelbrot_height){
                    dad_y_begin = mandelbrot_height - 1;
                }
                dad = dad_lock;
            }
            break;

        case dad_lock:
            if(input_manager.mouse_push(MOUSE_INPUT_RIGHT)){
                dad = dad_off;
                dad_proc = &ui_type::dad_proc_empty;
                dad_draw = &ui_type::dad_draw_empty;
            }
            break;

        case dad_drop:
            {
                int cursor_x = input_manager.cursor_coord_x(), cursor_y = input_manager.cursor_coord_y();
                if(cursor_x < 0){
                    cursor_x = 0;
                }else if(cursor_x >= mandelbrot_width){
                    cursor_x = mandelbrot_width - 1;
                }
                if(cursor_y < 0){
                    cursor_y = 0;
                }else if(cursor_y >= mandelbrot_height){
                    cursor_y = mandelbrot_height - 1;
                }
                if(dad_x_begin == cursor_x || dad_y_begin == cursor_y){
                    break;
                }
                dad = dad_off;
                dad_proc = &ui_type::dad_proc_empty;
                dad_draw = &ui_type::dad_draw_empty;
                int tx, ty, coex, coey;
                if(dad_x_begin < cursor_x){
                    coex = +1;
                    tx = cursor_x - dad_x_begin;
                }else{
                    coex = -1;
                    tx = dad_x_begin - cursor_x;
                }
                if(dad_y_begin < cursor_y){
                    coey = +1;
                    ty = cursor_y - dad_y_begin;
                }else{
                    coey = -1;
                    ty = dad_y_begin - cursor_y;
                }
                int m = tx < ty ? tx : ty;
                dad_x_end = dad_x_begin + m * coex;
                dad_y_end = dad_y_begin + m * coey;
                if(dad_x_begin > dad_x_end){
                    std::swap(dad_x_begin, dad_x_end);
                }
                dad_x_begin -= mandelbrot_width / 2;
                dad_x_end -= mandelbrot_width / 2;
                dad_x_begin *= -1;
                dad_x_end *= -1;
                dad_x_begin += mandelbrot_width / 2;
                dad_x_end += mandelbrot_width / 2;
                if(dad_x_begin > dad_x_end){
                    std::swap(dad_x_begin, dad_x_end);
                }
                dad_y_begin -= mandelbrot_height / 2;
                dad_y_end -= mandelbrot_height / 2;
                dad_y_begin *= -1;
                dad_y_end *= -1;
                dad_y_begin += mandelbrot_height / 2;
                dad_y_end += mandelbrot_height / 2;
                if(dad_y_begin > dad_y_end){
                    std::swap(dad_y_begin, dad_y_end);
                }
                tx = dad_x_end - dad_x_begin, ty = dad_y_end - dad_y_begin;
                thread_draw_data tmp_data = mandelbrot.ref_data;
                for(; ; ){
                    int length = tx < ty ? tx : ty;
                    mpf_class
                        quantity_x = (mandelbrot.ref_data.xmax - mandelbrot.ref_data.xmin) / mandelbrot_width,
                        quantity_y = (mandelbrot.ref_data.ymax - mandelbrot.ref_data.ymin) / mandelbrot_height,
                        nxmin = mandelbrot.ref_data.xmin + dad_x_begin * quantity_x,
                        nymin = mandelbrot.ref_data.ymin + dad_y_begin * quantity_y,
                        nxmax = nxmin + length * quantity_x,
                        nymax = nymin + length * quantity_y;
                    mandelbrot.set_coord(nxmin, nxmax, nymin, nymax);
                    int buffer_handle_ = buffer_handle;
                    auto draw_idx_fn = [buffer_handle_](tmp_complex, int x, int y, int idx, const rgb_type *c){
                        DrawPixelSoftImage_Unsafe_XRGB8(buffer_handle_, x, y, c[idx][0], c[idx][1], c[idx][2]);
                        mandelbrot.plt_idx(x, y, idx);
                    };
                    if(mandelbrot.next_mandelbrot(draw_idx_fn, mandelbrot_width, mandelbrot_height)){
                        message = message_n;
                        break;
                    }else{
                        if(quantity_x == 0 || quantity_y == 0){
                            message = message_failed;
                            break;
                        }
                        mandelbrot.set_coord(tmp_data.xmin, tmp_data.xmax, tmp_data.ymin, tmp_data.ymax);
                        mpf_set_default_prec(mpf_get_default_prec() + GMP_LIMB_BITS);
                        mandelbrot.set_prec(mpf_get_default_prec());
                        continue;
                    }
                }
            }
            break;
        }
    }

    void dad_draw_zoom() const{
        using namespace const_value;
        int cursor_x = input_manager.cursor_coord_x(), cursor_y = input_manager.cursor_coord_y();
        if(dad != dad_lock){ return; }
        if(cursor_x < 0){
            cursor_x = 0;
        }else if(cursor_x >= mandelbrot_width){
            cursor_x = mandelbrot_width - 1;
        }
        if(cursor_y < 0){
            cursor_y = 0;
        }else if(cursor_y >= mandelbrot_height){
            cursor_y = mandelbrot_height - 1;
        }
        int tx, ty, coex, coey;
        if(dad_x_begin < cursor_x){
            coex = 1;
            tx = cursor_x - dad_x_begin;
        }else{
            coex = -1;
            tx = dad_x_begin - cursor_x;
        }
        if(dad_y_begin < cursor_y){
            coey = 1;
            ty = cursor_y - dad_y_begin;
        }else{
            coey = -1;
            ty = dad_y_begin - cursor_y;
        }
        int m = (std::min)(tx, ty);
        DWORD red = GetColor(0xFF, 0x00, 0x00);
        DrawLine(dad_x_begin, dad_y_begin, dad_x_begin + m * coex, dad_y_begin, red);
        DrawLine(dad_x_begin, dad_y_begin + m * coey, dad_x_begin + m * coex + (coex >= 0 ? 1 : -1), dad_y_begin + m * coey, red);
        DrawLine(dad_x_begin, dad_y_begin, dad_x_begin, dad_y_begin + m * coey, red);
        DrawLine(dad_x_begin + m * coex, dad_y_begin, dad_x_begin + m * coex, dad_y_begin + m * coey, red);
        DrawLine(dad_x_begin, dad_y_begin, cursor_x, cursor_y, red);
    }

public:
    ui_type() :
        draw_pixel(),
        gcount(0),
        buffer_handle(MakeXRGB8ColorSoftImage(const_value::mandelbrot_width, const_value::mandelbrot_height)),
        draw_buffer_handle(CreateGraphFromSoftImage(buffer_handle)),
        disp_dec_hex_switch(0),
        create_draw_buffer_flag(false),
        visualization_proc(false),
        mode(mode_title),
        message(message_n),
        dad(dad_off),
        begin_draw(&ui_type::begin_draw_ctor),
        dad_proc(&ui_type::dad_proc_empty),
        dad_draw(&ui_type::dad_draw_empty)
    {
        if(buffer_handle == -1){
            std::abort();
        }
        draw_pixel = [&](tmp_complex, int x, int y, int idx, const rgb_type *c){
            DrawPixelSoftImage_Unsafe_XRGB8(buffer_handle, x, y, c[idx][0], c[idx][1], c[idx][2]);
            mandelbrot.plt_idx(x, y, idx);
        };
        mandelbrot.set_red_omega(const_value::mandelbrot_width - 1);
        mandelbrot.set_green_omega(const_value::mandelbrot_width - 1);
        mandelbrot.set_blue_omega(const_value::mandelbrot_width - 1);
    }

    ~ui_type(){ DeleteSoftImage(buffer_handle); }

    void proc(){
        using namespace const_value;
        (this->*begin_draw)();
        if(mandelbrot.test_joinable_all_thread()){
            create_draw_buffer_flag = true;
            if(input_manager.push(KEY_INPUT_X)){
                abort_compute_flag = true;
            }
            ++gcount;
        }else{
            if(create_draw_buffer_flag){
                create_draw_buffer_flag = false;
                if(DeleteGraph(draw_buffer_handle) == -1){
                    std::abort();
                }
                draw_buffer_handle = CreateGraphFromSoftImage(buffer_handle);
                if(draw_buffer_handle == -1){
                    std::abort();
                }
            }
            if(dad == dad_lock && input_manager.mouse_release(MOUSE_INPUT_LEFT)){
                dad = dad_drop;
            }
            if(input_manager.mouse_push(MOUSE_INPUT_LEFT)){
                {
                    int fn_y_min = fn_name_offset_y, fn_y_max = fn_y_min + char_height;
                    if(input_manager.cursor_coord_x() >= ui_margin + 20 * 0 && input_manager.cursor_coord_x() < ui_margin + 20 * 1 && input_manager.cursor_coord_y() >= fn_y_min && input_manager.cursor_coord_y() <= fn_y_max){
                        mandelbrot.next_red_fn();
                        remake_draw();
                    }
                    if(input_manager.cursor_coord_x() >= ui_margin + 20 * 1 && input_manager.cursor_coord_x() < ui_margin + 20 * 2 && input_manager.cursor_coord_y() >= fn_y_min && input_manager.cursor_coord_y() <= fn_y_max){
                        mandelbrot.next_green_fn();
                        remake_draw();
                    }
                    if(input_manager.cursor_coord_x() >= ui_margin + 20 * 2 && input_manager.cursor_coord_x() < ui_margin + 20 * 3 && input_manager.cursor_coord_y() >= fn_y_min && input_manager.cursor_coord_y() <= fn_y_max){
                        mandelbrot.next_blue_fn();
                        remake_draw();
                    }
                }
                if(
                    dad == dad_off &&
                    input_manager.cursor_coord_y() >= omega_offset_y_r - omega_point_length / 2 &&
                    input_manager.cursor_coord_y() < omega_offset_y_b + omega_point_length / 2
                ){
                    dad = dad_on;
                    dad_proc = &ui_type::dad_proc_omega;
                    dad_draw = &ui_type::dad_draw_empty;
                }
                if(mode != mode_title){
                    if(
                        dad == dad_off &&
                        input_manager.cursor_coord_x() >= 0 &&
                        input_manager.cursor_coord_x() < mandelbrot_width &&
                        input_manager.cursor_coord_y() >= 0 &&
                        input_manager.cursor_coord_y() < mandelbrot_height
                    ){
                        dad = dad_on;
                        dad_proc = &ui_type::dad_proc_zoom;
                        dad_draw = &ui_type::dad_draw_zoom;
                    }
                }
            }
            int buffer_handle_ = buffer_handle;
            const double
                log2 = std::log(2.0);
            mpf_class two = 2.0;
            std::function<void(tmp_complex, int, int, int, const rgb_type*)> draw_fn;
            if(mandelbrot.get_smooth_draw_flag()){
                draw_fn = [buffer_handle_, log2, two](tmp_complex c, int x, int y, int idx, const rgb_type *color){
                    auto rainbow = [](double p, int *rgb){
                        if(p > 1.0){
                            if(p - static_cast<int>(p) == 0.0){
                                p = 1.0;
                            }else{
                                p = p - static_cast<int>(p);
                            }
                        }
                        int seg_n = 6;
                        double m = seg_n * p;
                        int n = static_cast<int>(m);
                        double f = m - n;
                        int t = static_cast<int>(0xFF * f);
                        switch(n){
                        case 0:
                            rgb[0] = 0xFF;
                            rgb[1] = t;
                            rgb[2] = 0;
                            break;

                        case 1:
                            rgb[0] = 0xFF - t;
                            rgb[1] = 0xFF;
                            rgb[2] = 0;
                            break;

                        case 2:
                            rgb[0] = 0;
                            rgb[1] = 0xFF;
                            rgb[2] = t;
                            break;

                        case 3:
                            rgb[0] = 0;
                            rgb[1] = 0xFF - t;
                            rgb[2] = 0xFF;
                            break;

                        case 4:
                            rgb[0] = t;
                            rgb[1] = 0;
                            rgb[2] = 0xFF;
                            break;

                        case 5:
                            rgb[0] = 0xFF;
                            rgb[1] = 0;
                            rgb[2] = 0xFF - t;
                            break;

                        default:
                            rgb[0] = 0;
                            rgb[1] = 0;
                            rgb[2] = 0;
                            break;
                        }
                    };
                    int rgb[3];
                    double nu = (idx - mpf_ln(mpf_ln(sqrt(c.re2 + c.im2))).get_d() / log2) / mandelbrot.ref_data.maxiter;
                    rainbow(nu, rgb);
                    DrawPixelSoftImage_Unsafe_XRGB8(buffer_handle_, x, y, rgb[0], rgb[1], rgb[2]);
                    mandelbrot.plt_idx(x, y, idx);
                };
            }else{
                draw_fn = [buffer_handle_](tmp_complex, int x, int y, int idx, const rgb_type *c){
                    DrawPixelSoftImage_Unsafe_XRGB8(buffer_handle_, x, y, c[idx][0], c[idx][1], c[idx][2]);
                    mandelbrot.plt_idx(x, y, idx);
                };
            }
            if(mode == mode_mandelbrot){
                if(input_manager.push(KEY_INPUT_A)){
                    mode = mode_mandelbrot;
                    mpf_set_default_prec(0);
                    mandelbrot.set_coord(xmin, xmax, ymin, ymax);
                    int buffer_handle_ = buffer_handle;
                    if(mandelbrot.next_mandelbrot(draw_fn, mandelbrot_width, mandelbrot_height)){
                        message = message_n;
                    }else{
                        message = message_failed;
                    }
                }else if(input_manager.push(KEY_INPUT_T)){
                    mandelbrot.clear_cache();
                }else if(input_manager.push(KEY_INPUT_G)){
                    mandelbrot.save();
                }else if(input_manager.push(KEY_INPUT_B)){
                    mandelbrot.load();
                    if(mandelbrot.next_mandelbrot(draw_fn, mandelbrot_width, mandelbrot_height)){
                        message = message_n;
                    }else{
                        message = message_failed;
                    }
                }else if(input_manager.push(KEY_INPUT_Z)){
                    if(mandelbrot.next_mandelbrot(draw_fn, mandelbrot_width, mandelbrot_height)){
                        message = message_n;
                    }else{
                        message = message_failed;
                    }
                }else if(input_manager.push(KEY_INPUT_X)){
                    if(mandelbrot.prev_mandelbrot(draw_fn, mandelbrot_width, mandelbrot_height)){
                        message = message_n;
                    }else{
                        message = message_failed;
                    }
                }else if(input_manager.push(KEY_INPUT_W)){
                    mandelbrot.switch_mset_to_black();
                    remake_draw();
                }else if(input_manager.push(KEY_INPUT_S)){
                    mandelbrot.switch_smooth_draw();
                }else if(input_manager.push(KEY_INPUT_0)){
                    if(mandelbrot.put_mandelbrot(draw_fn, mandelbrot_width / 2, mandelbrot_height / 2)){
                        message = message_n;
                    }else{
                        message = message_failed;
                    }
                }else if(input_manager.push(KEY_INPUT_1) || input_manager.push(KEY_INPUT_2) || input_manager.push(KEY_INPUT_3) || input_manager.push(KEY_INPUT_4)){
                    unsigned int key[] = {
                        KEY_INPUT_1,
                        KEY_INPUT_2,
                        KEY_INPUT_3,
                        KEY_INPUT_4
                    };
                    int size[] = {
                        mandelbrot_width * 1, mandelbrot_height * 1,
                        mandelbrot_width * 2, mandelbrot_height * 2,
                        mandelbrot_width * 3, mandelbrot_height * 3,
                        mandelbrot_width * 4, mandelbrot_height * 4
                    };
                    for(int i = 0; i < 4; ++i){
                        if(!input_manager.push(key[i])){ continue; }
                        if(mandelbrot.put_mandelbrot(draw_fn, size[i * 2], size[i * 2 + 1])){
                            message = message_n;
                        }else{
                            message = message_failed;
                        }
                        break;
                    }
                }else if(input_manager.push(KEY_INPUT_E)){
                    mpf_set_default_prec(mpf_get_default_prec() - GMP_LIMB_BITS);
                    mandelbrot.set_prec(mpf_get_default_prec());
                }else if(input_manager.push(KEY_INPUT_R)){
                    mpf_set_default_prec(mpf_get_default_prec() + GMP_LIMB_BITS);
                    mandelbrot.set_prec(mpf_get_default_prec());
                }else if(input_manager.push(KEY_INPUT_D)){
                    mandelbrot.add_maxiter(-maxiter_addnum);
                }else if(input_manager.push(KEY_INPUT_F)){
                    mandelbrot.add_maxiter(+maxiter_addnum);
                }
            }
            (this->*dad_proc)();
            if(input_manager.push(KEY_INPUT_V)){
                ++disp_dec_hex_switch;
                disp_dec_hex_switch %= 2;
            }
            if(input_manager.push(KEY_INPUT_Q)){
                visualization_proc = !visualization_proc;
            }
        }
    }

    void draw() const{
        using namespace const_value;
        auto draw_colorbar = [&](int ur, int ug, int ub){
            int ar[] = { ur, ug, ub };
            int y[] = { omega_offset_y_r, omega_offset_y_g, omega_offset_y_b };
            for(int i = 0; i < 3; ++i){
                DrawLine(0, y[i], ui_width, y[i], GetColor(0x00, 0x00, 0x00));
                DrawLine(ar[i], y[i] - omega_point_length / 2, ar[i], y[i] + omega_point_length / 2 + 1, GetColor(0xFF, 0x00, 0x00));
            }
        };
        auto draw_float = [&](int x, int y, const disp_data_type::char_array_type &char_array){
            int i = 0;
            for(disp_data_type::char_array_type::const_iterator iter = char_array.cbegin(), end = char_array.cend(); iter != end; ++iter, ++i){
                std::size_t n = x + i * char_width;
                pattern::num[*iter]->draw(n, y);
            }
        };
        auto draw_hyphen = [&](int x, int y, int n){
            for(int i = 0; i < n; ++i){
                pattern::char_hyphen.draw(x + 4 * i, y);
            }
        };
        bool test_result = mandelbrot.test_joinable_all_thread();
        DrawBox(0, 0, window_width, window_height, GetColor(0xFF, 0xFF, 0xFF), TRUE);
        if(visualization_proc && (test_result || create_draw_buffer_flag)){
            DrawSoftImage(0, 0, buffer_handle);
        }else{
            DrawGraph(0, 0, draw_buffer_handle, FALSE);
        }
        draw_hyphen(ui_margin + char_box_width, re_draw_offset_y, hyphen_num);
        pattern::char_x.draw(ui_margin, re_draw_offset_y);
        draw_float(ui_margin, re_draw_offset_y + char_box_height * 1, *disp_data.ptr_xmax[disp_dec_hex_switch]);
        draw_float(ui_margin, re_draw_offset_y + char_box_height * 2, *disp_data.ptr_xmin[disp_dec_hex_switch]);
        draw_hyphen(ui_margin + char_box_width, im_draw_offset_y, hyphen_num);
        pattern::char_y.draw(ui_margin, im_draw_offset_y);
        draw_float(ui_margin, im_draw_offset_y + char_box_height * 1, *disp_data.ptr_ymin[disp_dec_hex_switch]);
        draw_float(ui_margin, im_draw_offset_y + char_box_height * 2, *disp_data.ptr_ymax[disp_dec_hex_switch]);
        draw_hyphen(ui_margin + char_box_width, zoom_ratio_draw_offset_y, hyphen_num);
        pattern::char_z.draw(ui_margin, zoom_ratio_draw_offset_y);
        draw_float(ui_margin, zoom_ratio_draw_offset_y + char_box_height, *disp_data.ptr_zoom_ratio[disp_dec_hex_switch]);
        draw_hyphen(ui_margin + char_box_width, prec_draw_offset_y, hyphen_num);
        pattern::char_p.draw(ui_margin, prec_draw_offset_y);
        draw_value(ui_margin, prec_draw_offset_y + char_box_height, mpf_get_default_prec());
        draw_hyphen(ui_margin * 2 + char_box_width * 3 + 4 * hyphen_num, prec_draw_offset_y, hyphen_num);
        if(mandelbrot.get_smooth_draw_flag()){
            pattern::char_s.draw(ui_margin * 2 + char_box_width * 2 + 4 * hyphen_num, prec_draw_offset_y);
        }else{
            pattern::char_n.draw(ui_margin * 2 + char_box_width * 2 + 4 * hyphen_num, prec_draw_offset_y);
        }
        draw_value(ui_margin * 2 + char_box_width * 2 + 4 * hyphen_num, prec_draw_offset_y + char_box_height, mandelbrot.ref_data.maxiter);
        draw_fn(
            ui_margin,
            fn_name_offset_y,
            mandelbrot.ref_data.fn_pattern_r,
            mandelbrot.ref_data.fn_pattern_g,
            mandelbrot.ref_data.fn_pattern_b
        );
        draw_colorbar(mandelbrot.ref_data.rad_a, mandelbrot.ref_data.rad_b, mandelbrot.ref_data.rad_c);
        if(test_result){
            pattern::gimg[(gcount / 4) % 4]->draw(ui_width - 16, mandelbrot_height);
        }else{
            (this->*dad_draw)();
            pattern::gimg_zero.draw(ui_width - 16, ui_offset);
        }
    }

private:
    template<class T>
    static void draw_value(int x, int y, const T &v){
        auto p10 = [](T d){
            T r = 1;
            for(T i = 0; i < d; ++i){ r *= 10; }
            return r;
        };
        int d = 0;
        static std::vector<int> vec;
        for(; ; ++d){
            T r = p10(d);
            if(v < r){ break; }
        }
        vec.resize(d);
        for(int i = 0; i < d; ++i){
            vec[d - i - 1] = (v / p10(i)) % 10;
        }
        for(int i = 0; i < d; ++i){
            pattern::num[vec[i]]->draw(x + static_cast<int>(i) * 5, y);
        }
    }

    static void draw_fn(int x, int y, int a, int b, int c){
        int ar[] = { a, b, c };
        for(int i = 0; i < 3; ++i){
            switch(ar[i]){
                case 0: pattern::str_sin.draw(x + i * 20, y); break;
                case 1: pattern::str_cos.draw(x + i * 20, y); break;
                case 2: pattern::str_tan.draw(x + i * 20, y); break;
            }
        }
    }
    
    set_pixel_fn_type draw_pixel;
    int
        gcount,
        dad_x_begin, dad_x_end,
        dad_y_begin, dad_y_end;
    int buffer_handle, draw_buffer_handle, disp_dec_hex_switch;
    bool create_draw_buffer_flag, visualization_proc;
    mode_enum mode;
    message_enum message;
    dad_enum dad;
    begin_draw_type begin_draw; 
    dad_proc_type dad_proc;
    dad_draw_type dad_draw;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
    if(
        ChangeWindowMode(TRUE) != DX_CHANGESCREEN_OK ||
        SetGraphMode(const_value::window_width, const_value::window_height, 32) != DX_CHANGESCREEN_OK ||
        SetMainWindowText("mandelbrot island ver 0.2.0") != 0 ||
        SetOutApplicationLogValidFlag(FALSE) != 0 ||
        DxLib_Init() != 0
    ){ return -1; }
    {
        pattern_surface.load();
        ui_type ui;
        for(; ; ){
            input_manager();
            if(ProcessMessage() == -1){
                exit_flag = true;
                mandelbrot.join_all_thread();
                break;
            }
            ui.proc();
            mandelbrot.proc();
            SetDrawScreen(DX_SCREEN_BACK);
            ui.draw();
            ScreenFlip();
        }
    }
    DxLib_End();
    return 0;
}
