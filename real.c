#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "real.h"

struct REAL
    {
    double value;
    };

extern REAL *
newREAL(double x)
    {
    REAL *p = malloc(sizeof(REAL));
    assert(p != 0);
    p->value = x;
    return p;
    }

extern double
getREAL(REAL *v)
    {
    return v->value;
    }

extern double
setREAL(REAL *v,double x)
    {
    double old = v->value;
    v->value = x;
    return old;
    }

extern void
displayREAL(void *v,FILE *fp)
    {
    fprintf(fp,"%f",getREAL((REAL *) v));
    }

extern int
compareREAL(void *v,void *w)
    {
      if (getREAL(v) > getREAL(w)) {
          return 1;
        }
        else if (getREAL(w) > getREAL(v)) {
          return -1;
        }
        else {
          return 0;
        }
    }

extern int
compareREALdecr(void *v, void *w)
  {
  return getREAL(w) - getREAL(v);
  }

extern void
freeREAL(void *v)
    {
    free((REAL *) v);
    }
