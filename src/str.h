//
// Created by pchen on 12/31/18.
//

#ifndef POINT_CBASE_STR_H
#define POINT_CBASE_STR_H

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif

typedef struct va_list_box va_list_box;

char *Str_sub(const char *s, int i, int j);

char *Str_dup(const char *s, int i, int j, int n);

char *Str_cat(const char *s1, int i1, int j1,
              const char *s2, int i2, int j2);

char *Str_catv(const char *s, ...);

char *Str_reverse(const char *s, int i, int j);

char *Str_map(const char *s, int i, int j,
              const char *from, const char *to);

int Str_pos(const char *s, int i);

int Str_len(const char *s, int i, int j);

int Str_cmp(const char *s1, int i1, int j1,
            const char *s2, int i2, int j2);

int Str_chr(const char *s, int i, int j, int c);

int Str_rchr(const char *s, int i, int j, int c);

int Str_upto(const char *s, int i, int j,
             const char *set);

int Str_rupto(const char *s, int i, int j,
              const char *set);

int Str_find(const char *s, int i, int j,
             const char *str);

int Str_rfind(const char *s, int i, int j,
              const char *str);

int Str_any(const char *s, int i,
            const char *set);

int Str_many(const char *s, int i, int j,
             const char *set);

int Str_rmany(const char *s, int i, int j,
              const char *set);

int Str_match(const char *s, int i, int j,
              const char *str);

int Str_rmatch(const char *s, int i, int j,
               const char *str);

void Str_fmt(int code, va_list_box *box,
             int put(int c, void *cl), void *cl,
             unsigned char flags[], int width, int precision);

#undef T

#endif //POINT_CBASE_STR_H
