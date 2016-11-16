// RUN: %clang_analyze_cc1 -analyzer-checker=core.builtin,debug.ExprInspection -verify %s
// REQUIRES: z3

void clang_analyzer_eval(int);

void float1() {
  float z1 = 0.0, z2 = -0.0;
  clang_analyzer_eval(z1 == z2); // expected-warning{{TRUE}}
}

void float2(float a, float b) {
  clang_analyzer_eval(a == b); // expected-warning{{UNKNOWN}}
  clang_analyzer_eval(a != b); // expected-warning{{UNKNOWN}}
}

void float3(float a, float b) {
  if (__builtin_isnan(a) || __builtin_isnan(b) || a < b) {
    clang_analyzer_eval(a > b); // expected-warning{{FALSE}}
    clang_analyzer_eval(a == b); // expected-warning{{FALSE}}
    return;
  }
  clang_analyzer_eval(a >= b); // expected-warning{{TRUE}}
  clang_analyzer_eval(a == b); // expected-warning{{UNKNOWN}}
}

void float4(float a) {
  if (__builtin_isnan(a) || a < 0.0f)
    return;
  clang_analyzer_eval(a >= 0.0Q); // expected-warning{{TRUE}}
  clang_analyzer_eval(a >= 0.0F); // expected-warning{{TRUE}}
  clang_analyzer_eval(a >= 0.0); // expected-warning{{TRUE}}
  clang_analyzer_eval(a >= 0); // expected-warning{{TRUE}}
}

void float5() {
  double value = 1.0;
  double pinf = __builtin_inf();
  double ninf = -__builtin_inf();
  double nan = __builtin_nan("");

  /* NaN */
  clang_analyzer_eval(__builtin_isnan(value)); // expected-warning{{FALSE}}
  clang_analyzer_eval(__builtin_isnan(nan)); // expected-warning{{TRUE}}

  clang_analyzer_eval(__builtin_isnan(0.0 / 0.0)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(pinf / ninf)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(pinf / pinf)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(ninf / pinf)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(ninf / ninf)); // expected-warning{{TRUE}}

  clang_analyzer_eval(__builtin_isnan(0.0 * ninf)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(0.0 * pinf)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(ninf * 0.0)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(pinf * 0.0)); // expected-warning{{TRUE}}

  clang_analyzer_eval(__builtin_isnan(nan + value)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(value - nan)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(nan * value)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isnan(value / nan)); // expected-warning{{TRUE}}

  /* Infinity */
  clang_analyzer_eval(__builtin_isinf(value)); // expected-warning{{FALSE}}
  clang_analyzer_eval(__builtin_isinf(pinf)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isinf(ninf)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isinf(value / 0.0)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isinf(pinf / 0.0)); // expected-warning{{TRUE}}
  clang_analyzer_eval(__builtin_isinf(ninf / 0.0)); // expected-warning{{TRUE}}
  clang_analyzer_eval(value / pinf == 0.0); // expected-warning{{TRUE}}
  clang_analyzer_eval(value / ninf == 0.0); // expected-warning{{TRUE}}

  /* Zero */
  clang_analyzer_eval(0.0 == -0.0); // expected-warning{{TRUE}}
}

void float6(float a, float b) {
  clang_analyzer_eval(a != 4.0 || a == 4.0); // expected-warning{{UNKNOWN}}
  clang_analyzer_eval(a < 4.0 || a >= 4.0); // expected-warning{{UNKNOWN}}
  clang_analyzer_eval((a != b) == !(a == b)); // expected-warning{{UNKNOWN}}
  clang_analyzer_eval((a != 4.0) == !(a == 4.0)); // expected-warning{{UNKNOWN}}
}

void mixed() {
  clang_analyzer_eval(0.0 * 0 == 0.0); // expected-warning{{TRUE}}
  clang_analyzer_eval(1.0 * 0 == 0.0); // expected-warning{{TRUE}}
  clang_analyzer_eval(1.0 * 1 == 1.0); // expected-warning{{TRUE}}
  clang_analyzer_eval((5 * 5) * 1.0 == 25); // expected-warning{{TRUE}}
}

void nan1(double a) {
  if (a == a)
    return;
  clang_analyzer_eval(__builtin_isnan(a)); // expected-warning{{TRUE}}
}
