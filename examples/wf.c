//
// Created by PCHEN on 2019/1/3.
//

static char rcsid[] = "$Id: wf.c 6 2007-01-22 00:45:22Z drhanson $";

#include <table.h>
#include "getword.h"
#include <atom.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define FREE(p)\
    free((p)); (p) = NULL;

void wf(char *, FILE *);

int first(int c);

int rest(int c);

int compare(const void *x, const void *y);

void vfree(const void *, void **, void *);


int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: can't open '%s' (%s)\n",
                    argv[0], argv[i], strerror(errno));
            return EXIT_FAILURE;
        } else {
            wf(argv[i], fp);
            fclose(fp);
        }
    }
    if (argc == 1) wf(NULL, stdin);
    return EXIT_SUCCESS;
}

void wf(char *name, FILE *fp) {
    Table *table = Table_new(0, NULL, NULL);
    char buf[128];
    while (getword(fp, buf, sizeof buf, first, rest)) {
        const char *word;
        int i, *count;
        for (i = 0; buf[i] != '\0'; i++)
            buf[i] = tolower(buf[i]);
        word = Atom_string(buf);
        count = Table_get(table, word);
        if (count)
            (*count)++;
        else {
            count = malloc(sizeof(*count));
            assert(count);
            *count = 1;
            Table_put(table, word, count);
        }
    }
    if (name)
        printf("%s:\n", name);
    {
        int i;
        void **array = Table_toArray(table, NULL);
        qsort(array, Table_length(table), 2 * sizeof(*array), compare);
        for (i = 0; array[i]; i += 2)
            printf("%d\t%s\n", *(int *) array[i + 1],
                   (char *) array[i]);
        FREE(array);
    }
    Table_each(table, vfree, NULL);
    Table_delete(&table);
}

int first(int c) {
    return isalpha(c);
}

int rest(int c) {
    return isalpha(c) || c == '_';
}

int compare(const void *x, const void *y) {
    return strcmp(*(char **) x, *(char **) y);
}

void vfree(const void *key, void **count, void *cl) {
    FREE(*count);
}
