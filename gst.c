//  CS201 Assign2 gst.c
//  Matthew Rinne
//  This file implements a Green Search Tree, using a BST
//  April 2018


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "queue.h"
#include "gst.h"


struct gval
{
  void *value;
  int freq;
  void (*display)(void *, FILE *);
  int (*compare)(void *, void *);
  void (*free)(void *);
};


extern GVAL *
newGVAL(void (*d)(void *,FILE *),int (*c)(void *,void *),void (*f)(void *),void *value)
{
  GVAL *new = malloc(sizeof(GVAL));
  assert(new != 0);
  new->value     = value;
  new->freq      = 1;
  new->display   = d;
  new->compare   = c;
  new->free      = f;
  return new;
}


//returns a GVAL's value
extern void *
getGVALvalue(GVAL *temp)
{
    return temp->value;
}


//returns a GVAL's frequency
extern int
getGVALfrequency(GVAL *temp)
{
  return temp->freq;
}


//display function for GVALs
extern void
displayGVAL(void *v, FILE *fp)
{
  GVAL *temp = v;
  temp->display(getGVALvalue(temp), fp);
  if (getGVALfrequency(temp) > 1) {
    fprintf(fp, "[%d]", getGVALfrequency(temp));
  }
  return;
}


//compare function for GVALs
extern int
compareGVAL(void *v, void *w)
{
  GVAL *temp = v;
  GVAL *temp2 = w;
  return temp->compare(getGVALvalue(temp),getGVALvalue(temp2));
}


//free function for GVALs, just the node
extern void
freeGVAL(void *v)
{
  free(v);
  return;
}


//free function for GVALs, node and value
extern void
freeGVALwhole(void *v)
{
  GVAL *temp = v;
  if (temp->free) {
    temp->free(getGVALvalue(temp));
    freeGVAL(temp);
  }
  return;
}


//increments the given GVAL's frequency
extern void
incrGVALfrequency(GVAL *temp)
{
  temp->freq++;
  return;
}


//decrements the given GVAL's frequency
extern void
decrGVALfrequency(GVAL *temp)
{
  temp->freq--;
  return;
}


struct gst
{
  BST *tree;
  int size;
  void (*display)(void *, FILE *);
  int (*compare)(void *, void *);
  void (*free)(void *);
};


extern GST *
newGST(void (*d)(void *,FILE *),int (*c)(void *,void *),void (*f)(void *))
{
  GST *g = malloc(sizeof(GST));
  assert(g != 0);
  g->tree       = newBST(displayGVAL,compareGVAL,NULL,freeGVALwhole);
  g->size       = 0;
  g->display    = d;
  g->compare    = c;
  g->free       = f;
  return g;
}


//inserts a value into the GST
extern void
insertGST(GST *g,void *value)
{
  GVAL *new = newGVAL(g->display, g->compare, g->free, value);
  assert(new != 0);
  if (sizeGST(g) == 0) {
    insertBST(g->tree, new);
    g->size++;
    return;
  }

  BSTNODE *temp = findBST(g->tree, new);
  if (temp) {
    freeGVAL(new);
    GVAL *temp2 = getBSTNODEvalue(temp);
    incrGVALfrequency(temp2);
    g->size++;
    return;
  }
  else {
    insertBST(g->tree, new);
    g->size++;
    return;
  }
}


//returns the frequency of a value, if found in the tree
extern int
findGSTcount(GST *g,void *v)
{
  GVAL *temp = newGVAL(g->display, g->compare, g->free, v);
  BSTNODE *find = findBST(g->tree, temp);
  freeGVAL(temp);
  if (find) {
    GVAL *temp2 = getBSTNODEvalue(find);
    int val = getGVALfrequency(temp2);
    return val;
  }
  return 0;
}


//returns the value of a node, if found in the tree
extern void *
findGST(GST *g,void *v)

{
  GVAL *temp = newGVAL(g->display, g->compare, g->free, v);
  BSTNODE *find = findBST(g->tree, temp);
  freeGVAL(temp);
  if (find) {
    GVAL *temp2 = getBSTNODEvalue(find);
    void *val = getGVALvalue(temp2);
    return val;
  }
  return NULL;
}


//delets a value from the GST
extern void *
deleteGST(GST *g,void *v)
{
  GVAL *temp = newGVAL(g->display, g->compare, g->free, v);
  BSTNODE *find = findBST(g->tree, temp);
  freeGVAL(temp);
  if (find) {
    GVAL *temp2 = getBSTNODEvalue(find);
    if (getGVALfrequency(temp2) > 1) {
      decrGVALfrequency(temp2);
      g->size--;
      return NULL;
    }
    else if (getGVALfrequency(temp2) == 1) {
      BSTNODE *delete = swapToLeafBST(g->tree, find);
      pruneLeafBST(g->tree, delete);
      int s = sizeBST(g->tree);
      s--;
      setBSTsize(g->tree, s);
      temp2 = getBSTNODEvalue(delete);
      void *val = getGVALvalue(temp2);
      freeBSTNODE(delete, g->free);
      g->size--;
      return val;
    }
  }
  else {
    fprintf(stdout, "Value ");
    g->display(v,stdout);
    fprintf(stdout, " not found.\n");
    return NULL;
  }
  return NULL;
}


//returns the sixe of the GST
extern int
sizeGST(GST *g)
{
  return g->size;
}


//returns the number of duplicates in the tree
extern int
duplicates(GST *g)
{
  return sizeGST(g) - sizeBST(g->tree);
}


//prints a statistical analysis of the tree (duplicates, # of nodes, min height, max height)
extern void
statisticsGST(GST *g,FILE *fp)
{
  fprintf(fp, "Duplicates: %d\n", duplicates(g));
  statisticsBST(g->tree, fp);
  return;
}


//calls to displayBSTdecorated to show a level ordered display of the tree
extern void
displayGST(GST *g,FILE *fp)
{
  if (sizeGST(g) == 0) printf("EMPTY\n");
  else displayBSTdecorated(g->tree, fp);
  return;
}


//clals to the display function of the underlying data structure (displayBST)
extern void
displayGSTdebug(GST *g,FILE *fp)
{
  if (sizeGST(g) == 0) printf("EMPTY\n");
  else displayBST(g->tree, fp);
  return;
}


//frees all allocated space for the tree
extern void
freeGST(GST *g)
{
  freeBST(g->tree);
  free(g);
  return;
}
