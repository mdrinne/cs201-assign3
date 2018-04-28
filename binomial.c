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
extern void   *getBNODEvalue(BNODE *b);
extern node1  *getBNODEowner(BNODE *b);
extern void    displayBNODE(void *v, FILE *fp);
extern int     compareBNODE(void *v, void *w);
extern void    updateBNODE(void *v, void *w);
extern void    freeBNODE(void *v);
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


extern node1 *
getBNODEowner(BNODE *b)
{
  if (b) return b->owner;
  else return NULL;
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
extern void    decrHeapSize(BINOMIAL *b);
extern void    setHeapSize(BINOMIAL *b, int size);
extern DLL    *getChildren(BNODE *d);
extern DLL    *getRootList(BINOMIAL *b);
extern void    setRootList(BINOMIAL *b, DLL *list);
extern int     calculateArraySize(BINOMIAL *b);
extern BNODE  *combine(BINOMIAL *b, BNODE *x, BNODE *y);
extern void    updateConsolidationArray(BNODE **D, BNODE *spot, BINOMIAL *b);
extern void    consolidate(BINOMIAL *b);
extern BNODE  *getBinomialExtreme(BINOMIAL *b);
/*---------------------------------------------*/


extern void
incrHeapSize(BINOMIAL *b)
{
  b->size++;
  return;
}


extern void
decrHeapSize(BINOMIAL *b)
{
  b->size--;
  return;
}


extern void
setHeapSize(BINOMIAL *b, int size)
{
  b->size = size;
  return;
}


/*------2.0------*/
extern DLL *
getChildren(BNODE *bn)
{
  if (!bn) return NULL;
  else return bn->children;
}


extern DLL *
getRootList(BINOMIAL *b)
{
  return b->rootList;
}


extern void
setRootList(BINOMIAL *b, DLL *list)
{
  b->rootList = list;
  return;
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
  if (b->compare(temp,temp2) < 0)
  {
    node1 *owner = insertDLL(x->children, sizeDLL(x->children), y);
    y->owner = owner;
    y->parent = x;

    return x;
  }
  else
  {
    node1 *owner = insertDLL(y->children, sizeDLL(y->children), x);
    x->owner = owner;
    x->parent = y;
    return y;
  }
}


extern void
updateConsolidationArray(BNODE **D, BNODE *spot, BINOMIAL *b)
{
  int degree = sizeDLL(getChildren(spot));
  while (D[degree] != NULL)
  {
    spot = combine(b,spot,D[degree]);
    D[degree] = NULL;
    degree++;
  }
  D[degree] = spot;
  return;
}

/*------6.0------*/
extern void
consolidate(BINOMIAL *b)
{
  int size = calculateArraySize(b);
  BNODE **D = malloc(sizeof(BNODE *) * size);
  for (int i=0; i<size; i++)
  {
    D[i] = NULL;
  }

  while (sizeDLL(b->rootList) != 0)
  {
    firstDLL(b->rootList);
    BNODE *spot = removeDLLnode(getRootList(b), getBNODEowner(currentDLL(getRootList(b))));
    updateConsolidationArray(D,spot,b);
  }

  b->extreme = NULL;
  for (int i=size-1; i>=0; i--)
  {
    if (D[i])
    {
      node1 *owner = insertDLL(b->rootList,0,D[i]);
      BNODE *temp = D[i];
      temp->owner = owner;
      if (sizeDLL(b->rootList) == 1)
      {
        temp->rsib = NULL;
      }
      else
      {
        firstDLL(b->rootList);
        nextDLL(b->rootList);
        BNODE *next = currentDLL(b->rootList);
        temp->rsib = next;
      }
      BNODE *temp2 = D[i];
      if ((b->extreme == NULL || b->compare(getBNODEvalue(temp2),getBNODEvalue(b->extreme)) < 0) && temp2)
      {
        b->extreme = temp2;
      }
    }
  }
  free(D);
}


extern BNODE *
getBinomialExtreme(BINOMIAL *b)
{
  return b->extreme;
}


/*------1.0------*/
extern void *
insertBINOMIAL(BINOMIAL *b,void *value)
{
  BNODE *new = newBNODE(b->display,b->compare,b->update,b->free, value);
  node1 *owner = insertDLL(b->rootList, 0, new);
  new->owner = owner;
  incrHeapSize(b);
  if (sizeBINOMIAL(b) == 1) return new;
  else
  {
    firstDLL(b->rootList);
    nextDLL(b->rootList);
    BNODE *next = currentDLL(b->rootList);
    new->rsib = next;
    consolidate(b);
    return new;
  }
}


extern int
sizeBINOMIAL(BINOMIAL *b)
{
  return b->size;
}


/*------2.0------*/
extern void
unionBINOMIAL(BINOMIAL *b,BINOMIAL *donor)
{
  if (sizeDLL(donor->rootList) == 0)
  {
    return;
  }
  int donorSize = sizeBINOMIAL(donor);
  unionDLL(b->rootList,donor->rootList);
  setHeapSize(b,sizeBINOMIAL(b)+donorSize);
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
  BNODE *extreme = getBinomialExtreme(b);
  return getBNODEvalue(extreme);
}


/*------3.0------*/
extern void *
extractBINOMIAL(BINOMIAL *b)
{
  BNODE *extreme = removeDLLnode(b->rootList, getBNODEowner(getBinomialExtreme(b)));
  firstDLL(extreme->children);
  for (int i=0; i<sizeDLL(extreme->children); i++)
  {
    BNODE *temp = currentDLL(extreme->children);
    temp->parent = temp;
    nextDLL(extreme->children);
  }
  unionDLL(b->rootList, extreme->children);
  consolidate(b);
  decrHeapSize(b);
  return getBNODEvalue(extreme);
}


extern void
statisticsBINOMIAL(BINOMIAL *b,FILE *fp)
{
  fprintf(fp, "size: %d\n", sizeBINOMIAL(b));
  fprintf(fp, "rootlist size: %d\n", sizeDLL(b->rootList));
  fprintf(fp, "extreme: ");
  b->display(getBNODEvalue(b->extreme), fp);
  return;
}


/*------2.0------*/
extern void
displayBINOMIAL(BINOMIAL *b,FILE *fp)
{
  DLL *roots = getRootList(b);
  int count = 0;
  int size = calculateArraySize(b);
  firstDLL(roots);
  fprintf(fp, "rootlist:");
  BNODE *temp = NULL;
  while (count < size)
  {
    temp = currentDLL(roots);
    DLL *children = getChildren(temp);
    int degree = sizeDLL(children);
    if (count < degree)
    {
      fprintf(fp, " NULL");
      count++;
    }
    else
    {
      fprintf(fp, " ");
      b->display(getBNODEvalue(temp), fp);
      BNODE *extreme = getBinomialExtreme(b);
      if (b->compare(getBNODEvalue(temp), getBNODEvalue(extreme)) == 0)
      {
        fprintf(fp, "*");
      }
      nextDLL(roots);
      count++;
    }
  }
  fprintf(fp, "\n");
  return;
}


extern void
displayBINOMIALdebug(BINOMIAL *b,FILE *fp)
{

}


extern void
freeBINOMIAL(BINOMIAL *b)
{

}
