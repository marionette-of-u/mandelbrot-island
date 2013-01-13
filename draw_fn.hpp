static void sin_sin_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void sin_sin_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void sin_sin_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void sin_cos_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void sin_cos_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void sin_cos_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void sin_tan_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void sin_tan_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void sin_tan_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void cos_sin_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void cos_sin_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void cos_sin_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void cos_cos_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void cos_cos_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void cos_cos_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void cos_tan_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void cos_tan_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void cos_tan_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void tan_sin_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void tan_sin_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void tan_sin_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void tan_cos_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void tan_cos_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void tan_cos_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}

static void tan_tan_sin(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.sin(rad_c)));
		color[i] = tc;
	}
}

static void tan_tan_cos(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.cos(rad_c)));
		color[i] = tc;
	}
}

static void tan_tan_tan(rgb_type *color, int maxiter, unsigned int rad_a_add, unsigned int rad_b_add, unsigned int rad_c_add, tf_type &tf){
	rgb_type tc = rgb(0, 0, 0);
	tf_type::angle_t rad_a = 0, rad_b = 0, rad_c = 0;
	for(int i = 0; i < maxiter; ++i){
		rad_a += rad_a_add, rad_b += rad_b_add, rad_c += rad_c_add;
		tc[0] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_a)));
		tc[1] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_b)));
		tc[2] = static_cast<unsigned char>(255.0 * std::abs(tf.tan(rad_c)));
		color[i] = tc;
	}
}
