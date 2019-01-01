//
// Created by pchen on 12/31/18.
//

#include "stringview.h"
#include "fmt.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
//// DEFINE(s)

#define DEFAULT_CHUNK_EXPEND_SIZE 10*1024

#define idx(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1)

#define isatend(s, n) ((s).string + (s).length == current->avail\
	&& current->avail + (n) <= current->limit)

#define equal(s, i, t) \
	(memcmp(&(s).string[i], (t).string, (t).length) == 0)

struct StringView_Save {
    struct chunk *current;
    char *avail;
};

//////////////////////////////////////////////////////////////////////////////
//// CONSTANTS
static char cset[] =
        "\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
        "\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
        "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057"
        "\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077"
        "\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117"
        "\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137"
        "\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157"
        "\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176\177"
        "\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
        "\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237"
        "\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257"
        "\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277"
        "\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317"
        "\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337"
        "\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357"
        "\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377"
;
const StringView StringView_CSET   = { 256, cset };
const StringView StringView_ASCII  = { 127, cset };
const StringView StringView_UCASE  = {  26, cset + 'A' };
const StringView StringView_LCASE  = {  26, cset + 'a' };
const StringView StringView_DIGITS = {  10, cset + '0' };
const StringView StringView_NULL   = {   0, cset };

//////////////////////////////////////////////////////////////////////////////
//// STATIC

static struct chunk {
    struct chunk *link;
    char *avail;
    char *limit;
} head = { NULL, NULL, NULL }, *current = &head;

static char *alloc(int len) {
    assert(len >= 0);
    if (current->avail + len > current->limit) {
        current = current->link =
                malloc(sizeof (*current) + DEFAULT_CHUNK_EXPEND_SIZE + len);
        current->avail = (char *)(current + 1);
        current->limit = current->avail + DEFAULT_CHUNK_EXPEND_SIZE + len;
        current->link = NULL;
    }
    current->avail += len;
    return current->avail - len;
}
//////////////////////////////////////////////////////////////////////////////
//// STRINGVIEW

StringView StringView_assign(const char *str, unsigned int size) {
    assert(str);
    assert(size>=0);
    StringView result;
    result.length = size;
    result.string = str;
    return result;
}

const char *StringView_cstr(char *dst, int size, const StringView s) {
    assert(s.length>=0 && s.string);
    if(dst==NULL){
        dst = malloc(s.length+1);
    }else{
        assert(size>=s.length+1);
    }
    memcpy(dst, s.string, s.length);
    dst[s.length]='\0';
    return dst;
}

StringView StringView_put(const char *str) {
    StringView result;
    result.length = strlen(str);
    result.string = memcpy(alloc(result.length), str, result.length);
    return result;
}

StringView StringView_sub(StringView stringView, int idx_from, int idx_to) {
    StringView result;
    assert(stringView.length>=0 && stringView.string);
    idx_from = idx(idx_from, stringView.length);
    idx_to = idx(idx_to, stringView.length);
    if(idx_from > idx_to){ int t = idx_from; idx_from=idx_to ; idx_to = t;}
    assert(idx_from>=0 && idx_to<=stringView.length);
    result.length = idx_to - idx_from;
    result.string = stringView.string + idx_from;
    return result;
}

int StringView_pos(StringView stringView, int idx_pos) {
    assert(stringView.length>=0 && stringView.string);
    idx_pos= idx(idx_pos, stringView.length);
    assert(idx_pos>=0 && idx_pos<=stringView.length);
    return idx_pos + 1;
}

StringView StringView_dup(StringView s, int n) {
    assert(s.length >= 0 && s.string);
    assert(n >= 0);
    if (n == 0 || s.length == 0)
        return StringView_NULL;
    if (n == 1)
        return s;
    {
        StringView text;
        char *p;
        text.length = n*s.length;
        if (isatend(s, text.length - s.length)) {
            text.string = s.string;
            p = alloc(text.length - s.length);
            n--;
        } else
            text.string = p = alloc(text.length);
        for ( ; n-- > 0; p += s.length)
            memcpy(p, s.string, s.length);
        return text;
    }
}

StringView StringView_cat(StringView s1, StringView s2) {
    assert(s1.length >= 0 && s1.string);
    assert(s2.length >= 0 && s2.string);
    if (s1.length == 0)
        return s2;
    if (s2.length == 0)
        return s1;
    if (s1.string + s1.length == s2.string) {
        s1.length += s2.length;
        return s1;
    }
    {
        StringView text;
        text.length = s1.length + s2.length;
        if (isatend(s1, s2.length)) {
            text.string = s1.string;
            memcpy(alloc(s2.length), s2.string, s2.length);
        } else {
            char *p;
            text.string = p = alloc(s1.length + s2.length);
            memcpy(p,          s1.string, s1.length);
            memcpy(p + s1.length, s2.string, s2.length);
        }
        return text;
    }
}

StringView StringView_reverse(StringView s) {
    assert(s.length >= 0 && s.string);
    if (s.length == 0)
        return StringView_NULL;
    else if (s.length == 1)
        return s;
    else {
        StringView text;
        char *p;
        int i = s.length;
        text.length = s.length;
        text.string = p = alloc(s.length);
        while (--i >= 0)
            *p++ = s.string[i];
        return text;
    }
}

StringView StringView_map(StringView s, const StringView *from, const StringView *to) {
    static char map[256];
    static int inited = 0;
    assert(s.length >= 0 && s.string);
    if (from && to) {
        int k;
        for (k = 0; k < (int)sizeof map; k++)
            map[k] = k;
        assert(from->length == to->length);
        for (k = 0; k < from->length; k++)
            map[(unsigned char)from->string[k]] = to->string[k];
        inited = 1;
    } else {
        assert(from == NULL && to == NULL);
        assert(inited);
    }
    if (s.length == 0)
        return StringView_NULL;
    else {
        StringView text;
        int i;
        char *p;
        text.length = s.length;
        text.string = p = alloc(s.length);
        for (i = 0; i < s.length; i++)
            *p++ = map[(unsigned char)s.string[i]];
        return text;
    }
}

int StringView_cmp(StringView s1, StringView s2) {
    assert(s1.length >= 0 && s1.string);
    assert(s2.length >= 0 && s2.string);
    if (s1.string == s2.string)
        return s1.length - s2.length;
    else if (s1.length < s2.length) {
        int cond = memcmp(s1.string, s2.string, s1.length);
        return cond == 0 ? -1 : cond;
    } else if (s1.length > s2.length) {
        int cond = memcmp(s1.string, s2.string, s2.length);
        return cond == 0 ? +1 : cond;
    } else
        return memcmp(s1.string, s2.string, s1.length);
}

int StringView_char(StringView s, int idx_from, int idx_to, int c) {
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    for ( ; idx_from < idx_to; idx_from++)
        if (s.string[idx_from] == c)
            return idx_from + 1;
    return 0;
}

int StringView_rchr(StringView s, int idx_from, int idx_to, int c) {
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    while (idx_to > idx_from)
        if (s.string[--idx_to] == c)
            return idx_to + 1;
    return 0;
}

int StringView_upto(StringView s, int idx_from, int idx_to, StringView set) {
    assert(set.length >= 0 && set.string);
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    for ( ; idx_from < idx_to; idx_from++)
        if (memchr(set.string, s.string[idx_from], set.length))
            return idx_from + 1;
    return 0;
}

int StringView_rupto(StringView s, int idx_from, int idx_to, StringView set) {
    assert(set.length >= 0 && set.string);
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    while (idx_to > idx_from)
        if (memchr(set.string, s.string[--idx_to], set.length))
            return idx_to + 1;
    return 0;
}

int StringView_any(StringView s, int i, StringView set) {
    assert(s.length >= 0 && s.string);
    assert(set.length >= 0 && set.string);
    i = idx(i, s.length);
    assert(i >= 0 && i <= s.length);
    if (i < s.length && memchr(set.string, s.string[i], set.length))
        return i + 2;
    return 0;
}

int StringView_many(StringView s, int idx_from, int idx_to, StringView set) {
    assert(set.length >= 0 && set.string);
    assert(s.length>= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    if (idx_from < idx_to && memchr(set.string, s.string[idx_from], set.length)) {
        do
            idx_from++;
        while (idx_from < idx_to
               && memchr(set.string, s.string[idx_from], set.length));
        return idx_from + 1;
    }
    return 0;
}

int StringView_rmany(StringView s, int idx_from, int idx_to, StringView set) {
    assert(set.length >= 0 && set.string);
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    if (idx_to > idx_from && memchr(set.string, s.string[idx_to-1], set.length)) {
        do
            --idx_to;
        while (idx_to >= idx_from
               && memchr(set.string, s.string[idx_to], set.length));
        return idx_to + 2;
    }
    return 0;
}

int StringView_find(StringView s, int idx_from, int idx_to, StringView str) {
    assert(str.length >= 0 && str.string);
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    if (str.length == 0)
        return idx_from + 1;
    else if (str.length == 1) {
        for ( ; idx_from < idx_to; idx_from++)
            if (s.string[idx_from] == *str.string)
                return idx_from + 1;
    } else
        for ( ; idx_from + str.length <= idx_to; idx_from++)
            if (equal(s, idx_from, str))
                return idx_from + 1;
    return 0;
}

int StringView_rfind(StringView s, int idx_from, int idx_to, StringView str) {
    assert(str.length >= 0 && str.string);
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    if (str.length == 0)
        return idx_to + 1;
    else if (str.length == 1) {
        while (idx_to > idx_from)
            if (s.string[--idx_to] == *str.string)
                return idx_to + 1;
    } else
        for ( ; idx_to - str.length >= idx_from; idx_to--)
            if (equal(s, idx_to - str.length, str))
                return idx_to - str.length + 1;
    return 0;
}

int StringView_match(StringView s, int idx_from, int idx_to, StringView str) {
    assert(str.length >= 0 && str.string);
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    if (str.length == 0)
        return idx_from + 1;
    else if (str.length == 1) {
        if (idx_from < idx_to && s.string[idx_from] == *str.string)
            return idx_from + 2;
    } else if (idx_from + str.length <= idx_to && equal(s, idx_from, str))
        return idx_from + str.length + 1;
    return 0;
}

int StringView_rmatch(StringView s, int idx_from, int idx_to, StringView str) {
    assert(str.length >= 0 && str.string);
    assert(s.length >= 0 && s.string);
    idx_from = idx(idx_from, s.length);
    idx_to = idx(idx_to, s.length);
    if (idx_from > idx_to) { int t = idx_from; idx_from = idx_to; idx_to = t; }
    assert(idx_from >= 0 && idx_to <= s.length);
    if (str.length == 0)
        return idx_to + 1;
    else if (str.length == 1) {
        if (idx_to > idx_from && s.string[idx_to -1] == *str.string)
            return idx_to;
    } else if (idx_to - str.length >= idx_from
               && equal(s, idx_to - str.length, str))
        return idx_to - str.length + 1;
    return 0;
}

StringView_Save *StrigView_save(void) {
    StringView_Save* save;
    save = malloc(sizeof(*save));
    assert(save);
    save->current = current;
    save->avail = current->avail;
    alloc(1);
    return save;
}

void StringView_restore(StringView_Save **save) {
    struct chunk *p, *q;
    assert(save && *save);
    current = (*save)->current;
    current->avail = (*save)->avail;
    free(*save); *save = NULL;
    for (p = current->link; p; p = q) {
        q = p->link;
        free(p); p = NULL;
    }
    current->link = NULL;
}

void StringView_fmt(int code, va_list_box *box,
              int put(int c, void *cl), void *cl,
              unsigned char flags[], int width, int precision) {
    StringView *s;
    assert(box && flags);
    s = va_arg(box->ap, StringView*);
    assert(s && s->length >= 0 && s->string);
    Fmt_puts(s->string, s->length, put, cl, flags,
             width, precision);
}