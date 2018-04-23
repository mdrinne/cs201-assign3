//  CS201 Assign2 avl.c
//  Matthew Rinne
//  This file implements an AVL tree, using a BST
//  April 2018


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "queue.h"
#include "avl.h"

struct aval
{
  void *value;
  int freq;
  int height;
  int bal;
  // int lefth;
  // int righth;
  void (*display)(void *, FILE *);
  int (*compare)(void *, void *);
  void (*free)(void *);
};


extern AVAL *
newAVAL(void (*d)(void *,FILE *),int (*c)(void *,void *),void (*f)(void *), void *value)
{
  AVAL *new = malloc(sizeof(AVAL));
  assert(new != 0);
  new->value       = value;
  new->freq        = 1;
  new->height      = 0;
  new->bal     = 0;
  // new->lefth       = -1;
  // new->righth      = -1;
  new->display     = d;
  new->compare     = c;
  new->free        = f;
  return new;
}


/*---------public AVAL functions---------*/
extern void *getAVALvalue(AVAL *temp);
extern int   getAVALfrequency(AVAL *temp);
extern int   getAVALheight(AVAL *temp);
extern int   getHeight(BSTNODE *temp);
// extern void  setLeftHeight(BSTNODE *temp, int lh);
// extern int   getLeftHeight(AVAL *temp);
// extern void  setRightHeight(BSTNODE *temp, int rh);
// extern int   getRightHeigth(AVAL *temp);
extern int   getAVALbalance(AVAL *temp);
extern int   getBalance(BSTNODE *temp);
extern int   getMax(int a, int b);
extern void  changeHeight(BSTNODE *temp, int h);
extern void  setHeight(BSTNODE *temp);
extern void  setBalance(BSTNODE *temp);
extern void  incrAVALfrequency(AVAL *temp);
extern void  decrAVALfrequency(AVAL *temp);
extern void  displayAVAL(void *v, FILE *fp);
extern int   compareAVAL(void *v, void *w);
extern void  swapperAVAL(BSTNODE *a, BSTNODE *b);
extern void  freeAVAL(void *v);
extern void  freeAVALwhole(void *v);
/*---------------------------------------*/


struct avl
{
  BST *tree;
  int size;
  void (*display)(void *, FILE *);
  int (*compare)(void *, void *);
  void (*free)(void *);
};


extern AVL *
newAVL(void (*d)(void *,FILE *),int (*c)(void *,void *),void (*f)(void *))
{
  AVL *a = malloc(sizeof(AVL));
  assert(a != 0);
  a->tree = newBST(displayAVAL,compareAVAL,swapperAVAL,freeAVALwhole);
  a->size = 0;
  a->display    = d;
  a->compare    = c;
  a->free       = f;
  return a;
}


/*---------public AVL functions---------*/
extern BSTNODE *getSibling(BSTNODE *temp);
extern BSTNODE *getFavoriteChild(BSTNODE *temp);
extern int      checkLinearLeft(BSTNODE *child, BSTNODE *parent, BSTNODE *gparent);
extern int      checkLinearRight(BSTNODE *child, BSTNODE *parent, BSTNODE *gparent);
extern int      checkLinear(BSTNODE *child, BSTNODE *parent, BSTNODE *gparent);
extern void     rotateRight(AVL *a, BSTNODE *y);
extern void     rotateLeft(AVL *a, BSTNODE *y);
extern void     rotate(AVL *a, BSTNODE *y);
extern void     insertionFixup(AVL *a, BSTNODE *curr);
extern void     deleteFixup(AVL *a, BSTNODE *curr);
/*--------------------------------------*/


//returns an AVAL's value
extern void *getAVALvalue(AVAL *temp)
{
  return temp->value;
}


//returns an AVAL's frequency
extern int getAVALfrequency(AVAL *temp)
{
  return temp->freq;
}


//returns an AVAL's height
extern int getAVALheight(AVAL *temp)
{
  if (!temp) return -1;
  else return temp->height;
}


//returns a BSTNODE's height
extern int getHeight(BSTNODE *temp)
{
  if (!temp) return -1;
  else return getAVALheight(getBSTNODEvalue(temp));
}


//returns an AVAL's balance
extern int
getAVALbalance(AVAL *temp)
{
  return temp->bal;
}


//returns a BSTNODE's balance
extern int getBalance(BSTNODE *temp)
{
  return getAVALbalance(getBSTNODEvalue(temp));
}


//returns the maximum of two integers
extern int getMax(int a, int b)
{
  if (a > b) return a;
  else return b;
}


//changes the height of the given node
extern void
changeHeight(BSTNODE *temp, int h)
{
  AVAL *temp2 = getBSTNODEvalue(temp);
  temp2->height = h;
  return;
}


//calculates the height of the given node
extern void
setHeight(BSTNODE *temp)
{
  if (!temp) return;
  BSTNODE *left = getBSTNODEleft(temp);
  BSTNODE *right = getBSTNODEright(temp);
  AVAL *temp2 = getBSTNODEvalue(temp);
  if (!left && !right) temp2->height = 0;
  else if (!left) temp2->height = getHeight(right)+1;
  else if (!right) temp2->height = getHeight(left)+1;
  else temp2->height = getMax(getHeight(left), getHeight(right))+1;
  return;
}


//calculates the balance of the given node
extern void
setBalance(BSTNODE *temp)
{
  BSTNODE *left = getBSTNODEleft(temp);
  BSTNODE *right = getBSTNODEright(temp);
  int l = getHeight(left);
  int r = getHeight(right);
  AVAL *temp2 = getBSTNODEvalue(temp);
  temp2->height = getMax(l, r)+1;
  temp2->bal = l - r;
  return;
}


//increments the given AVAL's frequency
extern void incrAVALfrequency(AVAL *temp)
{
  temp->freq++;
  return;
}


//decrements the given AVAL's frequency
extern void decrAVALfrequency(AVAL *temp)
{
  temp->freq--;
  return;
}


//display function for AVALs
extern void  displayAVAL(void *v, FILE *fp)
{
  AVAL *temp = v;
  temp->display(getAVALvalue(temp), fp);
  if (getAVALfrequency(temp) > 1) {
    fprintf(fp, "[%d]", getAVALfrequency(temp));
  }
  if (getAVALbalance(temp) == 1) {
    fprintf(fp, "+");
  }
  if (getAVALbalance(temp) == -1) {
    fprintf(fp, "-");
  }
  return;
}


//compare function for AVALs
extern int compareAVAL(void *v, void *w)
{
  AVAL *temp  = v;
  AVAL *temp2 = w;
  return temp->compare(getAVALvalue(temp),getAVALvalue(temp2));
}


//swapper function for AVALs
extern void swapperAVAL(BSTNODE *a, BSTNODE *b)
{
  AVAL *ta = getBSTNODEvalue(a);
  AVAL *tb = getBSTNODEvalue(b);

  /* swap the values stored in the AVL value objects */
  void *vtemp = ta->value;
  ta->value = tb->value;
  tb->value = vtemp;

  /* swap the counts stored in the AVL value objects */
  int ctemp = ta->freq;
  ta->freq = tb->freq;
  tb->freq = ctemp;

  return;
}


//free function for AVALs, just the node
extern void freeAVAL(void *v)
{
  free(v);
  return;
}


//free function for AVALs, node and the value
extern void freeAVALwhole(void *v)
{
  AVAL *temp = v;
  if (temp->free) temp->free(getAVALvalue(temp));
  freeAVAL(temp);
  return;
}


//returns the given node's sibling
extern BSTNODE *getSibling(BSTNODE *temp)
{
  BSTNODE *parent = getBSTNODEparent(temp);
  if (parent != temp) {
    if (temp == getBSTNODEleft(parent)) {
      return getBSTNODEright(parent);
    }
    else {
      return getBSTNODEleft(parent);
    }
  }
  else {
    return NULL;
  }
}


//returns the given nodes favorite child
extern BSTNODE *getFavoriteChild(BSTNODE *temp)
{
  if (!temp) {
    return NULL;
  }
  else if (getBalance(temp) < 0) {
    return getBSTNODEright(temp);
  }
  else if (getBalance(temp) > 0) {
    return getBSTNODEleft(temp);
  }
  else {
    return NULL;
  }
}


//checks if the given nodes are linear/all left children
extern int
checkLinearLeft(BSTNODE *child, BSTNODE *parent, BSTNODE *gparent)
{
  return (getBSTNODEleft(gparent) == parent) && (getBSTNODEleft(parent) == child);
}


//checks if the given nodes are linear/all right children
extern int
checkLinearRight(BSTNODE *child, BSTNODE *parent, BSTNODE *gparent)
{
  return (getBSTNODEright(gparent) == parent) && (getBSTNODEright(parent) == child);
}


//checks if the given nodes are linear
extern int checkLinear(BSTNODE *child, BSTNODE *parent, BSTNODE *gparent)
{
  int l = checkLinearLeft(child, parent, gparent);
  int r = checkLinearRight(child, parent, gparent);
  return l || r;
}


//rotates the given node to the right
extern void rotateRight(AVL *a, BSTNODE *y)
{
  BSTNODE *x = getBSTNODEparent(y);
  BSTNODE *yright = getBSTNODEright(y);
  BSTNODE *xparent = getBSTNODEparent(x);
  if (xparent == x) xparent = NULL;
  setBSTNODEleft(x, yright);
  if (yright) {
    setBSTNODEparent(yright, x);
  }
  setBSTNODEparent(y, xparent);
  if (!xparent) {
    setBSTroot(a->tree, y);
    setBSTNODEparent(y, y);
  }
  else if (x == getBSTNODEright(xparent)) {
    setBSTNODEright(xparent, y);
  }
  else {
    setBSTNODEleft(xparent, y);
  }
  setBSTNODEright(y, x);
  setBSTNODEparent(x, y);
  setHeight(x);
  setHeight(y);
  return;
}


//rotates the given node to the left
extern void rotateLeft(AVL *a, BSTNODE *y)
{
  BSTNODE *x = getBSTNODEparent(y);
  BSTNODE *yleft = getBSTNODEleft(y);
  BSTNODE *xparent = getBSTNODEparent(x);
  if (xparent == x) xparent = NULL;
  setBSTNODEright(x, yleft);
  if (yleft) {
    setBSTNODEparent(yleft, x);
  }
  setBSTNODEparent(y, xparent);
  if (!xparent) {
    setBSTroot(a->tree, y);
    setBSTNODEparent(y, y);
  }
  else if (x == getBSTNODEleft(xparent)) {
    setBSTNODEleft(xparent, y);
  }
  else {
    setBSTNODEright(xparent, y);
  }
  setBSTNODEleft(y, x);
  setBSTNODEparent(x, y);
  setHeight(x);
  setHeight(y);
  return;
}


//rotates the given node
extern void rotate(AVL *a, BSTNODE *x)
{
  BSTNODE *left = getBSTNODEleft(getBSTNODEparent(x));
  if (left && left == x) rotateRight(a, x);
  else rotateLeft(a, x);
  return;
}


//helper function for balancing the tree after insertAVL
extern void insertionFixup(AVL *a, BSTNODE *curr)
{
  while (1) {
    setHeight(curr);
    BSTNODE *sibling = getSibling(curr);
    BSTNODE *parent = getBSTNODEparent(curr);
    BSTNODE *fav = getFavoriteChild(parent);
    if (parent == curr) {
      return;
    }
    else if (fav && sibling == fav) {
      setBalance(parent);
      return;
    }
    else if (!fav) {
      setBalance(parent);
      curr = getBSTNODEparent(curr);
    }
    else {
      BSTNODE *y = getFavoriteChild(curr);
      if (y && (checkLinear(y,curr,parent) == 0)) {
        rotate(a, y);
        rotate(a, y);
        setBalance(curr);
        setBalance(parent);
        setBalance(y);
      }
      else {
        rotate(a, curr);
        setBalance(parent);
        setBalance(curr);
      }
      return;
    }
  }
}


//helper function for balancing the tree after deleteAVL
extern void deleteFixup(AVL *a, BSTNODE *curr)
{
  changeHeight(curr, -1);
  while (1) {
    // printf("----------------\n");
    // displayAVL(a,stdout);
    // printf("--------TOP OF WHILE--------\n\n\n");
    BSTNODE *parent = getBSTNODEparent(curr);
    BSTNODE *fav = getFavoriteChild(parent);
    if (parent == curr) {
    //   printf("^^^^^^^^REACHED ROOT^^^^^^^^\n\n");
      return;
    }
    else if (fav && curr == fav) {
      // printf("^^^^^^^^CURR IS FAV^^^^^^^^\n\n\n");
      setBalance(parent);
      curr = parent;
    }
    else if (!fav) {
      // printf("^^^^^^^^NO FAVORITE CHILD^^^^^^^^\n\n\n");
      setBalance(parent);
      return;
    }
    else {
      BSTNODE *sibling = getSibling(curr);
      BSTNODE *y = getFavoriteChild(sibling);
      int l = checkLinear(y, sibling, parent);
      if (y && l == 0) {
        // printf("^^^^^^^^DOUBLE ROTATE IS NEEDED^^^^^^^^\n\n");
        rotate(a, y);
        // displayAVAL(a, stdout);
        // printf("^^^^^^^^FIRST ROTATE COMPLETE^^^^^^^^\n\n");
        rotate(a, y);
        // displayAVL(a, stdout);
        // printf("^^^^^^^^SECOND ROTATE COMPLETE^^^^^^^^\n\n\n");
        setBalance(parent);
        setBalance(sibling);
        setBalance(y);
        curr = y;
      }
      else {
        // printf("^^^^^^^^SINLGE ROTATE NEEDED^^^^^^^^\n\n");
        rotate(a, sibling);
        // displayAVL(a, stdout);
        // printf("^^^^^^^^SINGLE ROTATE COMPLETE^^^^^^^^\n\n\n");
        setBalance(parent);
        setBalance(sibling);
        if (!y) {
          return;
        }
        curr = sibling;
      }
    }
  }
}


//inserts a value into the AVL tree
extern void insertAVL(AVL *a,void *value)
{
  AVAL *new = newAVAL(a->display, a->compare, a->free, value);
  assert(new != 0);
  if (sizeAVL(a) == 0) {
    insertBST(a->tree, new);
    a->size++;
    return;
  }

  BSTNODE *temp = findBST(a->tree, new);
  if (temp) {
    freeAVAL(new);
    AVAL *temp2 = getBSTNODEvalue(temp);
    incrAVALfrequency(temp2);
    a->size++;
    return;
  }
  else {
    BSTNODE *temp2 = insertBST(a->tree, new);
    a->size++;
    insertionFixup(a, temp2);

    return;
  }
}


//returns the frequency of a value, if found in the tree
extern int findAVLcount(AVL *a,void *value)
{
  AVAL *temp = newAVAL(a->display, a->compare, a->free, value);
  BSTNODE *find = findBST(a->tree, temp);
  freeAVAL(temp);
  if (find) return getAVALfrequency(getBSTNODEvalue(find));
  else return 0;
}


//returns the value of the node, if found in the tree
extern void *findAVL(AVL *a,void *value)
{
  AVAL *temp = newAVAL(a->display, a->compare, a->free, value);
  BSTNODE *find = findBST(a->tree, temp);
  freeAVAL(temp);
  if (find) return value;
  else return NULL;
}


//deletes a value from the AVL tree
extern void *deleteAVL(AVL *a,void *value)
{
  AVAL *temp = newAVAL(a->display, a->compare, a->free, value);
  BSTNODE *find = findBST(a->tree, temp);
  freeAVAL(temp);
  if (find) {
    AVAL *temp2 = getBSTNODEvalue(find);
    if (getAVALfrequency(temp2) > 1) {
      decrAVALfrequency(temp2);
      a->size--;
      // displayAVL(a, stdout);
      // printf("^^^^^^^^AFTER DECR^^^^^^^^\n\n\n");
      return NULL;
    }
    else if (getAVALfrequency(temp2) == 1) {
      BSTNODE *delete = swapToLeafBST(a->tree, find);
      deleteFixup(a, delete);
      pruneLeafBST(a->tree, delete);
      // displayAVL(a, stdout);
      // printf("^^^^^^^^AFTER PRUNE^^^^^^^^\n\n\n");
      int s = sizeBST(a->tree);
      s--;
      setBSTsize(a->tree, s);
      // deleteFixup(a, delete);
      temp2 = getBSTNODEvalue(delete);
      void *val = getAVALvalue(temp2);
      freeBSTNODE(delete, a->free);
      a->size--;
      return val;
    }
  }
  else {
    fprintf(stdout, "Value ");
    a->display(value, stdout);
    fprintf(stdout, " not found.\n");
    return NULL;
  }
  return NULL;
}


//returns the size of the AVL/BST
extern int sizeAVL(AVL *a)
{
  return sizeBST(a->tree);
}


//returns the number of duplicates in the tree
extern int duplicatesAVL(AVL *a)
{
  return a->size - sizeBST(a->tree);
}


//prints a statistical analysis of the tree (duplicates, # of nodes, min height, max height)
extern void statisticsAVL(AVL *a,FILE *fp)
{
  fprintf(fp, "Duplicates: %d\n", duplicatesAVL(a));
  statisticsBST(a->tree, fp);
  return;
}


//calls to displayBSTdecorated to show a level ordered display of the tree
extern void displayAVL(AVL *a,FILE *fp)
{
  if (sizeAVL(a) == 0) {
    printf("EMPTY\n");
    return;
  }
  displayBSTdecorated(a->tree, fp);
  return;
}


//calls to the display function of the underlying data structure (displayBST)
extern void displayAVLdebug(AVL *a,FILE *fp)
{
  if (sizeAVL(a) == 0) printf("EMPTY\n");
  else displayBST(a->tree, fp);
  return;
}


//frees all allocated space for the tree
extern void freeAVL(AVL *a)
{
  freeBST(a->tree);
  free(a);
  return;
}
