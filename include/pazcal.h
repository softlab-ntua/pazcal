#ifndef __PAZCAL_H__
#define __PAZCAL_H__


// Standard header files

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <math.h>


// Types and constants

#undef true
#undef false
#define true  ((bool) 1)
#define false ((bool) 0)

typedef double REAL;

#define REAL_MANT_DIG   DBL_MANT_DIG
#define REAL_DIG        DBL_DIG
#define REAL_MIN_EXP    DBL_MIN_EXP
#define REAL_MIN_10_EXP DBL_MIN_10_EXP
#define REAL_MAX_EXP    DBL_MAX_EXP
#define REAL_MAX_10_EXP DBL_MAX_10_EXP
#define REAL_MIN        DBL_MIN
#define REAL_MAX        DBL_MAX
#define REAL_EPSILON    DBL_EPSILON


// Runtime errors

#define __pazcal_RTERROR(...) ({                        \
      fprintf(stderr, "Pazcal runtime error: ");        \
      fprintf(stderr, __VA_ARGS__);                     \
      fprintf(stderr, "\n");                            \
      exit(1);                                          \
    })

#define __pazcal_SYSERROR(...) ({                       \
      fprintf(stderr, "Pazcal runtime error: ");        \
      fprintf(stderr, __VA_ARGS__);                     \
      fprintf(stderr, "\n");                            \
      perror(NULL);                                     \
      exit(1);                                          \
    })


// Generic argument counting machinery

#define __pazcal_CAT(A, B) __pazcal_CAT2(A, B)
#define __pazcal_CAT2(A, B) A ## B

#define __pazcal_COUNT_ARG(...) \
  __pazcal_COUNT_AUX(42, ## __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __pazcal_COUNT_AUX(_, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, n, ...) n


// WRITE set of macros

enum __pazcal_printf_type {
  __pazcal_printf_string,
  __pazcal_printf_bool,
  __pazcal_printf_int,
  __pazcal_printf_double,
  __pazcal_printf_char,
  __pazcal_printf_unsigned_int,
  __pazcal_printf_long_int,
  __pazcal_printf_unsigned_long_int,
  __pazcal_printf_float,
  __pazcal_printf_long_double,
  __pazcal_printf_format
};

const char * __pazcal_printf_fmt[] = {
  "s",       // string
  "s",       // bool
  "d",       // int
  "lf",      // double
  "c",       // char
  "u",       // unsigned int
  "ld",      // long int
  "lu",      // unsigned long int
  "lf",      // float
  "Lf",      // long double
  "<FORMAT>" // this should never be printed
};

typedef enum {
  __pazcal_format_NONE,
  __pazcal_format_VERBATIM,
  __pazcal_format_FORM2,
  __pazcal_format_FORM3
} __pazcal_format_enum;

typedef struct {
  __pazcal_format_enum kind;
  const char *fmt;
  int width, precision;
} __pazcal_format_struct;

#define FMT(fmt, a)                                                     \
  ((__pazcal_format_struct) {__pazcal_format_VERBATIM, (fmt), 0, 0}), (a)

#define __pazcal_FORM_2(a, w)                                           \
  ((__pazcal_format_struct) {__pazcal_format_FORM2, NULL, (w), 0}), (a)
#define __pazcal_FORM_3(a, w, p)                                        \
  ((__pazcal_format_struct) {__pazcal_format_FORM3, NULL, (w), (p)}), (a)

#define FORM(...) \
  __pazcal_CAT(__pazcal_FORM_, __pazcal_COUNT_ARG(__VA_ARGS__))(__VA_ARGS__)

static inline void __pazcal_printf(enum __pazcal_printf_type t, ...)
{
  static __pazcal_format_struct spec = {__pazcal_format_NONE, NULL, 0, 0};
  va_list arg;
  va_start(arg, t);

  if (t == __pazcal_printf_format)
    spec = va_arg(arg, __pazcal_format_struct);
  else {
    char fmt[32];
    switch (spec.kind) {
    case __pazcal_format_NONE:
      sprintf(fmt, "%%%s", __pazcal_printf_fmt[t]);
      break;
    case __pazcal_format_VERBATIM:
      strncpy(fmt, spec.fmt, sizeof(fmt)-1);
      break;
    case __pazcal_format_FORM2:
      if (spec.width < 0)
        __pazcal_RTERROR("negative width for WRITE format");
      sprintf(fmt, "%%%d%s", spec.width, __pazcal_printf_fmt[t]);
      break;
    case __pazcal_format_FORM3:
      if (spec.width < 0)
        __pazcal_RTERROR("negative width for WRITE format");
      if (spec.precision < 0)
        __pazcal_RTERROR("negative precision for WRITE format");
      sprintf(fmt, "%%%d.%d%s", spec.width, spec.precision,
              __pazcal_printf_fmt[t]);
      break;
    }

    switch (t) {
    case __pazcal_printf_string:
      printf(fmt, va_arg(arg, char *));
      break;
    case __pazcal_printf_bool:
      printf(fmt, va_arg(arg, int) ? "true" : "false");
      break;
    case __pazcal_printf_int:
      printf(fmt, va_arg(arg, int));
      break;
    case __pazcal_printf_double:
      printf(fmt, va_arg(arg, double) ? : 0.0);
      break;
    case __pazcal_printf_char:
      printf(fmt, (char) va_arg(arg, int));
      break;
    case __pazcal_printf_unsigned_int:
      printf(fmt, va_arg(arg, unsigned int));
      break;
    case __pazcal_printf_long_int:
      printf(fmt, va_arg(arg, long int));
      break;
    case __pazcal_printf_unsigned_long_int:
      printf(fmt, va_arg(arg, unsigned long int));
      break;
    case __pazcal_printf_float:
      printf(fmt, va_arg(arg, double) ? : 0.0);
      break;
    case __pazcal_printf_long_double:
      printf(fmt, va_arg(arg, long double) ? : 0.0l);
      break;
    default:
      __pazcal_RTERROR("invalid use of __pazcal_printf");
    }
  }
  va_end(arg);
}

#define __pazcal_WRITE(a) do {                                           \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), char*),                  \
    __pazcal_printf(__pazcal_printf_string, a),                          \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), const char*),            \
    __pazcal_printf(__pazcal_printf_string, a),                          \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), char[]),                 \
    __pazcal_printf(__pazcal_printf_string, a),                          \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), const char[]),           \
    __pazcal_printf(__pazcal_printf_string, a),                          \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), bool),                   \
    __pazcal_printf(__pazcal_printf_bool, a),                            \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), int),                    \
    __pazcal_printf(__pazcal_printf_int, a),                             \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), double),                 \
    __pazcal_printf(__pazcal_printf_double, a),                          \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), char),                   \
    __pazcal_printf(__pazcal_printf_char, a),                            \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), unsigned int),           \
    __pazcal_printf(__pazcal_printf_unsigned_int, a),                    \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), long int),               \
    __pazcal_printf(__pazcal_printf_long_int, a),                        \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), unsigned long int),      \
    __pazcal_printf(__pazcal_printf_unsigned_long_int, a),               \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), float),                  \
    __pazcal_printf(__pazcal_printf_float, a),                           \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), long double),            \
    __pazcal_printf(__pazcal_printf_long_double, a),                     \
  __builtin_choose_expr(                                                 \
    __builtin_types_compatible_p(__typeof__(a), __pazcal_format_struct), \
    __pazcal_printf(__pazcal_printf_format, a),                          \
    printf("<cannot WRITE argument %s>", #a)))))))))))))));              \
  } while(0)

#define __pazcal_WRITE_0() \
  do { /* nothing */ } while(0)
#define __pazcal_WRITE_1(a)                                            \
  do { __pazcal_WRITE(a); } while(0)
#define __pazcal_WRITE_2(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_1(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_3(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_2(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_4(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_3(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_5(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_4(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_6(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_5(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_7(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_6(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_8(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_7(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_9(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_8(__VA_ARGS__); } while(0)
#define __pazcal_WRITE_10(a, ...) \
  do { __pazcal_WRITE(a); __pazcal_WRITE_9(__VA_ARGS__); } while(0)

#define WRITE(...)                                                      \
  __pazcal_CAT(__pazcal_WRITE_, __pazcal_COUNT_ARG(__VA_ARGS__))(__VA_ARGS__)

#define WRITELN(...) do { WRITE(__VA_ARGS__); putchar('\n'); } while(0)

#define __pazcal_WRITESP_0()                    \
  do { /* nothing */ } while(0)
#define __pazcal_WRITESP_1(a)                   \
  do { __pazcal_WRITE(a); } while(0)
#define __pazcal_WRITESP_2(a, ...)                      \
  do { __pazcal_WRITE(a); putchar(' ');                 \
       __pazcal_WRITESP_1(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_3(a, ...)                      \
  do { __pazcal_WRITE(a); putchar(' ');                 \
       __pazcal_WRITESP_2(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_4(a, ...)                      \
  do { __pazcal_WRITE(a);  putchar(' ');                \
       __pazcal_WRITESP_3(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_5(a, ...)                      \
  do { __pazcal_WRITE(a);  putchar(' ');                \
       __pazcal_WRITESP_4(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_6(a, ...)                      \
  do { __pazcal_WRITE(a);  putchar(' ');                \
       __pazcal_WRITESP_5(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_7(a, ...)                      \
  do { __pazcal_WRITE(a);  putchar(' ');                \
       __pazcal_WRITESP_6(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_8(a, ...)                      \
  do { __pazcal_WRITE(a);  putchar(' ');                \
       __pazcal_WRITESP_7(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_9(a, ...)                      \
  do { __pazcal_WRITE(a); putchar(' ');                 \
       __pazcal_WRITESP_8(__VA_ARGS__); } while(0)
#define __pazcal_WRITESP_10(a, ...)                     \
  do { __pazcal_WRITE(a);  putchar(' ');                \
       __pazcal_WRITESP_9(__VA_ARGS__); } while(0)

#define WRITESP(...)                                                    \
  __pazcal_CAT(__pazcal_WRITESP_, __pazcal_COUNT_ARG(__VA_ARGS__))(__VA_ARGS__)

#define WRITESPLN(...) do { WRITESP(__VA_ARGS__); putchar('\n'); } while(0)


// READ set of macros

#define READ_INT() ({ int __i;                                          \
      if (scanf("%d", &__i) != 1)                                       \
        __pazcal_RTERROR("READ_INT found no valid integer number");     \
      __i; })

#define READ_REAL() ({ REAL __r;                                        \
      if (scanf("%lf", &__r) != 1)                                      \
        __pazcal_RTERROR("READ_REAL found no valid real number");       \
      __r; })

#define SKIP_LINE() do {                        \
    int c = getchar();                          \
    if (c == '\n' || c == EOF) break;           \
  } while(1)


// Program and procedures

#define PROGRAM                                 \
  int main () {                                 \
    void MAIN();                                \
    MAIN();                                     \
    return 0;                                   \
  }                                             \
  void

#define PROC void
#define FUNC


// FOR loops

#define __pazcal_FOR_4(var, start, sign, stop)  \
  __pazcal_FOR_5(var, start, sign, stop, 1)
#define __pazcal_FOR_5(var, start, sign, stop, step)                    \
  for (__typeof__(((stop) - (start)) / (step))                          \
         __the_var  = (start),                                          \
         __the_stop = (stop),                                           \
         __the_step = (sign) * ((step) ? :                              \
           (__pazcal_RTERROR("zero step in FOR loop "), 1));            \
       ((var) = __the_var,                                              \
        (var) = (var),                                                  \
        (__the_step >= 0 ? __the_var <= __the_stop                      \
                         : __the_var >= __the_stop));                   \
       __the_var += __the_step)

#define FOR(...) \
  __pazcal_CAT(__pazcal_FOR_, __pazcal_COUNT_ARG(__VA_ARGS__))(__VA_ARGS__)

#define TO     , +1 ,
#define DOWNTO , -1 ,
#define STEP   ,


// Switch

#define NEXT do {} while(0)


// Aliases for operators

#define MOD %
#define AND &&
#define OR  ||
#define NOT !


// MIN and MAX set of macros

#define MIN(a,b)                 \
  ({ __typeof__ (a) _a = (a);    \
     __typeof__ (b) _b = (b);    \
     _a < _b ? _a : _b; })

#define MAX(a,b)                 \
  ({ __typeof__ (a) _a = (a);    \
     __typeof__ (b) _b = (b);    \
     _a > _b ? _a : _b; })


// I/O redirection

#define INPUT(filename) do {                                            \
    if (freopen(filename, "rt", stdin) == NULL) {                       \
      __pazcal_RTERROR("Cannot redirect stdin to %s", filename);      \
      perror(NULL);                                                     \
    }                                                                   \
  } while(0);

#define OUTPUT(filename) do {                                           \
    if (freopen(filename, "wt", stdout) == NULL)                        \
      __pazcal_SYSERROR("Cannot redirect stdout to %s", filename);    \
  } while(0);


#endif
