//  CS201 Assign3 binomial.c
//  Matthew Rinne
// This file implements a Binomial Heap
// April 2018

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "queue.h"
#include "avl.h"
#include "binomial.h"


struct binomial
{
  AVL *atree;
  int size;
  void (*display)(void *,FILE *);
  int (*compare)(void *, void *);
  void (*update)(void *, void *);
  void (*free)(void *);
}


extern BINOMIAL *
newBINOMIAL(void (*d)(void *,FILE *),int (*c)(void *,void *),void (*u)(void *,void *),void (*f)(void *))
{
  BINOMIAL *b = malloc(sizeof(BINOMIAL));
  assert(b != 0);
  b->atree   = newAVL(d,c,f);
  b->size    = 0;
  b->display = d;
  b->compare = c;
  b->update  = u;
  b->free    = f;
}


extern void *
insertBINOMIAL(BINOMIAL *b,void *value)
{

}


extern int
sizeBINOMIAL(BINOMIAL *b)
{

}


extern void
unionBINOMIAL(BINOMIAL *,BINOMIAL *)
{

}


extern void
deleteBINOMIAL(BINOMIAL *b,void *node)
{

}


extern void
decreaseKeyBINOMIAL(BINOMIAL *b,void *node,void *value)
{

}


extern void *
peekBINOMIAL(BINOMIAL *b)
{

}


extern void *
extractBINOMIAL(BINOMIAL *b)
{

}


extern void
statisticsBINOMIAL(BINOMIAL *b,FILE *fp)
{

}


extern void
displayBINOMIAL(BINOMIAL *b,FILE *fp)
{

}


extern void
displayBINOMIALdebug(BINOMIAL *b,FILE *fp)
{

}


extern void
freeBINOMIAL(BINOMIAL *b)
{

}
