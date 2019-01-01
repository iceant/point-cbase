//
// Created by pchen on 12/31/18.
//

#ifndef POINT_CBASE_FMT_H
#define POINT_CBASE_FMT_H

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif // INCLUDED_STDARG_H

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif // INCLUDED_STDIO_H

typedef struct va_list_box {
    va_list ap;
} va_list_box;

#define T Fmt_T

typedef void (*T)(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[256], int width, int precision);

void Fmt_fmt(int put(int c, void *cl), void *cl,
             const char *fmt, ...);

void Fmt_vfmt(int put(int c, void *cl), void *cl,
              const char *fmt, va_list_box *box);

void Fmt_print(const char *fmt, ...);

void Fmt_fprint(FILE *stream, const char *fmt, ...);

int Fmt_sfmt(char *buf, int size, const char *fmt, ...);

int Fmt_vsfmt(char *buf, int size, const char *fmt, va_list_box *box);

char *Fmt_string(const char *fmt, ...);

char *Fmt_vstring(const char *fmt, va_list_box *box);

T Fmt_register(int code, T cvt);

void Fmt_putd(const char *str, int len,
              int put(int c, void *cl), void *cl,
              unsigned char flags[256], int width, int precision);

void Fmt_puts(const char *str, int len,
              int put(int c, void *cl), void *cl,
              unsigned char flags[256], int width, int precision);

#undef T

#endif //POINT_CBASE_FMT_H
