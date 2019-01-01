//
// Created by pchen on 12/31/18.
//

#include "stringview.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "hexdump.c"
#include "fmt.h"

int main(int argc, char** argv){
    const char* str = "Hello, World!";
    StringView stringView = StringView_assign(str, strlen(str));
    printf("Value:%s\n", stringView.string);

    StringView strView2 = StringView_put("Hello, World!!");
    printf("strView2(%x):%s\n", strView2.string, strView2.string);
    hexdump(strView2.string-14, strView2.length+28);

    char buf[strView2.length+1];
    StringView_cstr(buf, strView2.length+1, strView2);
    printf("Get from strView2:%s\n", buf);

    StringView sub1 = StringView_sub(stringView, -6, -1);
    const char* cstr1 = StringView_cstr(NULL, 0, sub1);
    printf("StringView_sub('%s', -6, -1) = %s\n", str, cstr1);

    printf("sub1.string address:%x\n", sub1.string);
    printf("'cstr1' address:%x\n", cstr1);
    hexdump(cstr1, sub1.length);

    // StringView_cstr(NULL...) need free
    free((char*)cstr1);
    cstr1 = NULL;
    assert(cstr1==NULL);

    StringView s1 = StringView_put("Hello");
    StringView s2 = StringView_put("World");

    StringView s1_space_s2 = StringView_cat(s1, s2);
    cstr1 = StringView_cstr(NULL, 0, s1);
    printf("StringView_cat(%s, %s)\n", cstr1, s2.string);
    free(cstr1); cstr1 = NULL;
    hexdump(s1_space_s2.string, s1_space_s2.length);

    StringView s2dup1 = StringView_dup(s2, 1);
    cstr1 = StringView_cstr(NULL, 0, s2);
    printf("StringView_dup('%s', 1)=%s\n", cstr1, s2dup1.string);
    free(cstr1); cstr1=NULL;

    StringView s2dup2 = StringView_dup(s2, 2);
    cstr1 = StringView_cstr(NULL, 0, s2);
    printf("StringView_dup('%s', 2)=%s\n", cstr1, s2dup2.string);
    free(cstr1); cstr1 = NULL;

    StringView s2dup3 = StringView_dup(s2, 3);
    cstr1 = StringView_cstr(NULL, 0, s2);
    printf("StringView_dup('%s', 3)=%s\n", cstr1, s2dup3.string);
    free(cstr1); cstr1 = NULL;

    StringView s2_reverse = StringView_reverse(s2);
    cstr1 = StringView_cstr(NULL, 0, s2);
    printf("StringView_reverse('%s')=%s\n", cstr1, s2_reverse.string);
    free(cstr1); cstr1 = NULL;

    StringView s2_map = StringView_map(s2_reverse, &StringView_LCASE, &StringView_UCASE);
    cstr1 = StringView_cstr(NULL, 0, s2_reverse);
    printf("StringView_map('%s', LCASE, UCASE)=%s\n", cstr1, s2_map.string);
    free(cstr1); cstr1 = NULL;

    Fmt_register('T', StringView_fmt);
    Fmt_print("%T\n", &s1);

    int ret = StringView_find(s1, 1, 0, StringView_put("o"));
    cstr1 = StringView_cstr(NULL, 0, s1);
    printf("StringView_find('%s', 1, 0, 'o')=%d\n", cstr1, ret);
    free(cstr1);cstr1=NULL;

    ret = StringView_rfind(s1, 1, 0, StringView_put("e"));
    cstr1 = StringView_cstr(NULL, 0, s1);
    printf("StringView_rfind('%s', 1, 0, 'e')=%d\n", cstr1, ret);
    free(cstr1);cstr1=NULL;

    ret = StringView_match(s1, 3, 0, StringView_put("llo"));
    cstr1 = StringView_cstr(NULL, 0, s1);
    printf("StringView_match('%s', 3, 0, 'llo')=%d\n", cstr1, ret);
    free(cstr1);cstr1=NULL;

    ret = StringView_any(s1, 1, StringView_put("eHlo"));
    assert(ret!=0);
    cstr1 = StringView_cstr(NULL, 0, s1);
    printf("StringView_any('%s', 1, 'eHlo')=%d\n", cstr1, ret);
    free(cstr1);cstr1=NULL;

    ret = StringView_any(s1, 1, StringView_put("Word"));
    cstr1 = StringView_cstr(NULL, 0, s1);
    printf("StringView_any('%s', 1, 'Word')=%d\n", cstr1, ret);
    free(cstr1);cstr1=NULL;

    ret = StringView_many(s1, 3, 0, StringView_put("llo"));
    cstr1 = StringView_cstr(NULL, 0, s1);
    printf("StringView_many('%s', 3, 0, 'llo')=%d\n", cstr1, ret);
    free(cstr1);cstr1=NULL;


}