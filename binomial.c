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


typedef struct bnode
{
  DLL *children;
  BNODE *parent;
  BNODE *rsib;
  void *value;
  void (*display)(void *,FILE *);
  int (*compare)(void *, void *);
  void (*update)(void *, void *);
  void (*free)(void *);
} BNODE;


extern BNODE *
newBNODE(void (*d)(void *,FILE *),int (*c)(void *, void *),void (*u)(void *, void *),void (*f)(void *),void *value) {
  BNODE *new = malloc(sizeof(BNODE));
  assert(new != 0);
  new->value     = value;
  new->parent    = new;
  new->rsib      = NULL;
  new->children  = newDLL(displayBNODE,freeBNODE);
  new->display   = d;
  new->compare   = c;
  new->update    = u;
  new->free      = f;
}


/*----------public functions for bnode----------*/
extern void *getBNODEvalue(BNODE *b);
extern void  displayBNODE(void *v, FILE *fp);
extern int   compareBNODE(void *v, void *w);
extern void  updateBNODE(void *v, void *w);
extern void  freeBNODE(void *v);
/*----------------------------------------------*/


extern void *
getBNODEvalue(BNODE *b)
{
  return b->value;
}


extern void
displayBNODE(void *v, FILE *fp)
{
  BNODE *temp = v;
  temp->display(getBNODEvalue(temp), fp);
}


extern int
compareBNODE(void *v, void *w)
{
  BNODE *temp  = v;
  BNODE *temp2 = w;
  return temp->compare(getBNODEvalue(temp), getBNODEvalue(temp2));
}


extern void
updateBNODE(void *v, void *w)
{

}


extern void
freeBNODE(void *v)
{
  free(v);
}


struct binomial
{
  DLL *rootList;
  QUEUE *nodes;
  BNODE *extreme;
  int size;
  void (*display)(void *,FILE *);
  int (*compare)(void *, void *);
  void (*update)(void *, void *);
  void (*free)(void *);
};


extern BINOMIAL *
newBINOMIAL(void (*d)(void *,FILE *),int (*c)(void *,void *),void (*u)(void *,void *),void (*f)(void *))
{
  BINOMIAL *b = malloc(sizeof(BINOMIAL));
  assert(b != 0);
  b->rootList = newDLL(displayBNODE,freeBNODE);
  b->nodes    = newQUEUE(d,f);
  b->extreme  = NULL;
  b->size     = 0;
  b->display  = d;
  b->compare  = c;
  b->update   = u;
  b->free     = f;
}


/*----------public binomial functions----------*/
extern void    incrHeapSize(BINOMIAL *b);
extern int     getHeapSize(BINOMIAL *b);
extern DLL    *getChildren(DLL *d);
extern int     calculateArraySize(BINOMIAL *b);
extern BNODE  *combine(BINOMIAL *b, BNODE *x, BNODE *y);
extern void    updateConsolidationArray(void *D, int size, BNODE *spot);
extern void    consolidate(Binomial *b);
/*---------------------------------------------*/


extern void
incrHeapSize(BINOMIAL *b)
{
  b->size++;
  return;
}


extern int
getHeapSize(BINOMIAL *b)
{
  return b->size;
}


extern DLL *
getChildren(DLL *d)
{
  return d->children;
}


extern int
calculateArraySize(BINOMIAL *b)
{
  return (log((double)getHeapSize(b))/log(2.0))+1;
}


extern BNODE *
combine(BINOMIAL *b, BNODE *x, BNODE *y)
{
  void *temp = getBNODEvalue(x);
  void *temp2 = getBNODEvalue(y);
  if (b->compare(x,y) < 0) {
    insertDLL(x->children, 0, y);
    y->parent = x;
    return x;
  }
  else {
    insertDLL(y->children, 0, x);
    x->parent = y;
    return y;
  }
}


extern void
updateConsolidationArray(void *D, int size, BNODE *spot, BINOMIAL *b)
{
  int degree = sizeDLL(getChildren(spot));
  while (D[degree]) {
    spot = combine(b,spot,D[degree]);
    D[degree] = NULL;
    degree++;
  }
  D[degree] = spot;
  return;
}


extern void
consolidate(BINOMIAL *b)
{
  int size = calculateArraySize(b);
  void *D[size];
  for (int i=0; i<size, i++) {
    D[i] = NULL;
  }

  while (sizeDLL(b->rootList) != 0) {
    firstDLL(b->rootList);
    BNODE *spot = removeDLLnode(currentDLL(b->rootList), temp);
    updateConsolidationArray(D,size,spot,b);
  }
  b->extreme = NULL;
  for (int i=0; i<size; i++) {
    if (D[i]) {
      insertDLL(b->rootList,0,D[i]);
      if (b->extreme == NULL || b->compare(getBNODEvalue(D[i]),getBNODEvalue(b->extreme)) < 0) {
        b->extreme = D[i];
      }
    }
  }
  free(D);
}


extern void *
insertBINOMIAL(BINOMIAL *b,void *value)
{
  BNODE *new = newBNODE(b->display,b->compare,b->update,b->free, value);
  insertDLL(b->rootList, 0, new);
  incrHeapSize(b);
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
