//
// Created by pchen on 12/31/18.
//

#ifndef POINT_CBASE_STRINGVIEW_H
#define POINT_CBASE_STRINGVIEW_H

//////////////////////////////////////////////////////////////////////////////
//// FORWARD DECL
typedef struct va_list_box va_list_box;

//////////////////////////////////////////////////////////////////////////////
////

typedef struct StringView {
    unsigned long length;
    const char* string;
} StringView;

typedef struct StringView_Save StringView_Save;

const StringView StringView_CSET;
const StringView StringView_ASCII;
const StringView StringView_UCASE;
const StringView StringView_LCASE;
const StringView StringView_DIGITS;
const StringView StringView_NULL;


StringView StringView_put(const char* str);

StringView StringView_assign(const char* str, unsigned int size);

const char* StringView_cstr(char* dst /*free(ret) when dst=NULL*/,
        int size,
        const StringView stringView);
    // Convert stringView to dst[0..size-2], with '\0' at the end
    // Check (size < stringView.length+1)
    // if dst is NULL, will malloc(stringView.length+1) bytes and copy stringView.string to the new memory space and return it.
    // if malloc(stringView.length+1) failed, error will be checked.

StringView StringView_sub(StringView, int idx_from, int idx_to);

int StringView_pos(StringView, int idx_pos);
    // return positive number of idx_pos
    // for example: StringView_pos('Interface', -4) = 6('f')

StringView StringView_dup(StringView stringView, int numOfCpy);
    // duplicate stringView with numOfCpy

StringView StringView_cat(StringView s1 ,StringView s2);
    // cat s2 to s1 at the end 's1s2'

StringView StringView_reverse(StringView stringView);

StringView StringView_map(StringView stringView, const StringView* from, const StringView* to);
    // for example: StringView_map(s, &StringView_UCASE, &StringView_LCASE)

int StringView_cmp(StringView s1, StringView s2);
    // 1: big
    // 0: eq
    // -1: small

//////////////////////////////////////////////////////////////////////////////
//// SEARCH
int StringView_char(StringView stringView, int idx_from, int idx_to, int c);
int StringView_rchr(StringView stringView, int idx_from, int idx_to, int c);
int StringView_upto(StringView stringView, int idx_from, int idx_to, StringView set);
    // search from left to right in stringView[idx_from: idx_to] for any char in set
    // return left idx position if found
    // return 0 if not found
int StringView_rupto(StringView stringView, int idx_from, int idx_to, StringView set);
int StringView_any(StringView stringView, int idx_pos, StringView set);
    // if stringView[idx_pos] == set , return StringView_pos(stringView, idx_pos) + 1
    // else return 0
int StringView_many(StringView stringView, int idx_from, int idx_to, StringView set);
    // search char in range stringView[idx_from:idx_to] in set
    // return 0 if not found
    // return max position in stringView
int StringView_rmany(StringView stringView, int idx_from, int idx_to, StringView set);

int StringView_find(StringView stringView, int idx_from, int idx_to, StringView str);
int StringView_rfind(StringView stringView, int idx_from, int idx_to, StringView str);
int StringView_match(StringView stringView, int idx_from, int idx_to, StringView str);
    // if stringView[idx_from:idx_to] start with str return StringView_pos(stringView, idx_from) + str.length
    // else return 0
int StringView_rmatch(StringView stringView, int idx_from, int idx_to, StringView str);

//////////////////////////////////////////////////////////////////////////////
//// SAVE/RESTORE
StringView_Save* StrigView_save(void);

void StringView_restore(StringView_Save** stringViewSave);

//////////////////////////////////////////////////////////////////////////////
//// Fmt INTERFACE
void StringView_fmt(int code, va_list_box *box,
                    int put(int c, void *cl), void *cl,
                    unsigned char flags[], int width, int precision);

#endif //POINT_CBASE_STRINGVIEW_H
