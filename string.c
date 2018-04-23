#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "string.h"

struct STRING
    {
    char *value;
    };

extern STRING *
newSTRING(char *x)
    {
    STRING *p = malloc(sizeof(STRING));
    assert(p != 0);
    p->value = x;
    return p;
    }

extern char *
getSTRING(STRING *v)
    {
    return v->value;
    }

extern char *
setSTRING(STRING *v,char *x)
    {
    char *old = v->value;
    v->value = x;
    return old;
    }

extern void
displaySTRING(void *v,FILE *fp)
    {
    fprintf(fp,"%s",getSTRING((STRING *) v));
    }

extern int
compareSTRING(void *v,void *w)
    {
    return strcmp(getSTRING(v),getSTRING(w));
    }

extern int
compareSTRINGdecr(void *v, void *w)
    {
    return compareSTRING(w, v);
    }

extern void
freeSTRING(void *v)
    {
    free((STRING *) v);
    }
