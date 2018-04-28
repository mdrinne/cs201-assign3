//  CS201 Assign3 binomial.c
//  Matthew Rinne
// This file implements a Binomial Heap
// April 2018

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "bst.h"
#include "queue.h"
#include "dll.h"
#include "avl.h"
#include "binomial.h"


struct bnode
{
  DLL *children;
  BNODE *parent;
  BNODE *rsib;
  node1 *owner;
  void *value;
  void (*display)(void *,FILE *);
  int (*compare)(void *, void *);
  void (*update)(void *, void *);
  void (*free)(void *);
} ;


/*----------public functions for bnode----------*/
extern void *getBNODEvalue(BNODE *b);
extern void  displayBNODE(void *v, FILE *fp);
extern int   compareBNODE(void *v, void *w);
extern void  updateBNODE(void *v, void *w);
extern void  freeBNODE(void *v);
/*----------------------------------------------*/


extern BNODE *
newBNODE(void (*d)(void *,FILE *),int (*c)(void *, void *),void (*u)(void *, void *),void (*f)(void *),void *value) {
  BNODE *new = malloc(sizeof(BNODE));
  assert(new != 0);
  new->value     = value;
  new->parent    = new;
  new->rsib      = NULL;
  new->owner     = NULL;
  new->children  = newDLL(displayBNODE,freeBNODE);
  new->display   = d;
  new->compare   = c;
  new->update    = u;
  new->free      = f;
  return new;
}


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
  return b;
}


/*----------public binomial functions----------*/
extern void    incrHeapSize(BINOMIAL *b);
extern int     getHeapSize(BINOMIAL *b);
extern DLL    *getChildren(BNODE *d);
extern int     calculateArraySize(BINOMIAL *b);
extern BNODE  *combine(BINOMIAL *b, BNODE *x, BNODE *y);
extern void    updateConsolidationArray(void *D, BNODE *spot, BINOMIAL *b);
extern void    consolidate(BINOMIAL *b);
extern void    mergeLists(BINOMIAL *d, BINOMIAL *donor);
extern void    setBinomialSize(BINOMIAL *d, int size);
/*---------------------------------------------*/


extern void
incrHeapSize(BINOMIAL *b)
{
  b->size++;
  return;
}


extern DLL *
getChildren(BNODE *bn)
{
  return bn->children;
}


extern int
calculateArraySize(BINOMIAL *b)
{
  return (log((double)sizeBINOMIAL(b))/log(2.0))+1;
}


extern BNODE *
combine(BINOMIAL *b, BNODE *x, BNODE *y)
{
  void *temp = getBNODEvalue(x);
  void *temp2 = getBNODEvalue(y);
  if (b->compare(temp,temp2) < 0) {
    node1 *owner = insertDLL(x->children, 0, y);
    y->owner = owner;
    y->parent = x;
    return x;
  }
  else {
    node1 *owner = insertDLL(y->children, 0, x);
    x->owner = owner;
    x->parent = y;
    return y;
  }
}


extern void
updateConsolidationArray(void *D, BNODE *spot, BINOMIAL *b)
{
  int degree = sizeDLL(getChildren(spot));
  while (D[degree] != NULL) {
    spot = combine(b,spot,D[degree]);
    D[degree] = NULL;
    degree++;
  }
  D[degree] = spot;
  return;
}

/*------2.0------*/
extern void
consolidate(BINOMIAL *b)
{
  int size = calculateArraySize(b);
  void *D[size];
  for (int i=0; i<size; i++) {
    D[i] = NULL;
  }

  while (sizeDLL(b->rootList) != 0) {
    firstDLL(b->rootList);
    BNODE *spot = removeDLLnode(b->rootList, currentDLL(b->rootList));
    updateConsolidationArray(D,spot,b);
  }
  b->extreme = NULL;
  for (int i=size-1; i>-0; i--) {
    if (D[i]) {
      BNODE *temp = insertDLL(b->rootList,0,D[i]);
      if (sizeDLL(b->rootList) == 1) {
        temp->rsib = NULL;
      }
      else {
        firstDLL(b->rootList);
        nextDLL(b->rootList);
        BNODE *next = currentDLL(b->rootList);
        temp->rsib = next;
      }
      if (b->extreme == NULL || b->compare(getBNODEvalue(D[i]),getBNODEvalue(b->extreme)) < 0) {
        b->extreme = D[i];
      }
    }
  }
  free(D);
}


/*------2.0------*/
extern void
mergeLists(DLL *b, DLL *donor)
{
  int count = 0;
  firstDLL(b);
  firstDLL(donor);
  BNODE *temp = removeDLL(donor, 0);
  BNODE *temp2 = currentDLL(b);
  while (sizeDLL(donor) >= 0) {
    if (b->compare(getBNODEvalue(temp), getBNODEvalue(temp2)) < 0) {
      insertDLL(b, count, temp);
      if (sizeDLL(donor) == 0) {
        return;
      }
      firstDLL(donor;
      count++;
    }
    else {
      nextDLL(b);
      count++;
    }
  }
  return;
}


extern void
setBinomialSize(BINOMIAL *d, int size)
{
  d->size = size;
  return;
}


/*------1.0------*/
extern void *
insertBINOMIAL(BINOMIAL *b,void *value)
{
  BNODE *new = newBNODE(b->display,b->compare,b->update,b->free, value);
  node1 *owner = insertDLL(b->rootList, 0, new);
  new->owner = owner;
  incrHeapSize(b);
  consolidate(b);
  return new;
}


extern int
sizeBINOMIAL(BINOMIAL *b)
{
  return b->size;
}


/*------1.0------*/
extern void
unionBINOMIAL(BINOMIAL *b,BINOMIAL *donor)
{
  if (sizeDLL(donor->rootList) == 0) {
    return;
  }
  int donorSize = sizeBINOMIAL(donor);
  mergeLists(b->rootList,donor->rootList);
  setBinomialSize(b,sizeBINOMIAL(b)+donorSize);
  donor->rootList = newDLL(displayBNODE, freeBNODE);
  donor->size = 0;
  donor->extreme = NULL;
  consolidate(b);
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
  return b->extreme;
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
