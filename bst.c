//  CS201 Assign1 bst.c
//  Matt Rinne
//  This file implements a Binary Search tree
//  Febuary 2018

#include "bst.h"
#include "queue.h"
#include <stdlib.h>
#include <assert.h>


struct bstnode
{
  void *value;
  BSTNODE *parent;
  BSTNODE *left;
  BSTNODE *right;
};


//this function creates a new bstnode
extern BSTNODE *
newBSTNODE(void *v)
{
  BSTNODE *node = malloc(sizeof(BSTNODE));
  node->value  = v;
  node->parent = NULL;
  node->left   = NULL;
  node->right  = NULL;
  return node;
}


//this function returns the value of the node passed in
extern void *
getBSTNODEvalue(BSTNODE *n)
{
  return n->value;
}


//this function changes the value of the given node to the value passed in
extern void
setBSTNODEvalue(BSTNODE *n, void *value)
{
  n->value = value;
  return;
}


//this function returns the left child of the given node
extern BSTNODE *
getBSTNODEleft(BSTNODE *n)
{
  if (n->left == NULL) {
    return NULL;
  }
  return n->left;
}


//this function replaces the left child of the given node, to a replacement node passed in
extern void
setBSTNODEleft(BSTNODE *n, BSTNODE *replacement)
{
  n->left = replacement;
  return;
}


//this function returns the right child of the given node
extern BSTNODE *
getBSTNODEright(BSTNODE *n)
{
  if (n->right == NULL) {
    return NULL;
  }
  return n->right;
}


//this function replaces the right child of the gien node, to a replacement node passed in
extern void
setBSTNODEright(BSTNODE *n, BSTNODE *replacement)
{
  n->right = replacement;
  return;
}


//this function returns the parent of the given node
extern BSTNODE *
getBSTNODEparent(BSTNODE *n)
{
  return n->parent;
}


//this function replaces the parent of the given node to a replacement node passed in
extern void
setBSTNODEparent(BSTNODE *n, BSTNODE *replacement)
{
  n->parent = replacement;
  return;
}


//this function frees the given node
extern void
freeBSTNODE(BSTNODE *n, void (*f)(void *))
{
  if (f != NULL) {
    f(getBSTNODEvalue(n));
    free(n);
  }
  return;
}


struct bst
{
  BSTNODE *root;
  QUEUE *nodes;
  int size;
  int min;
  int max;
  void (*display)(void *, FILE *);
  int (*compare)(void *, void *);
  void (*swapper)(BSTNODE *, BSTNODE *);
  void (*free)(void *);
};


//this function creates a new bst
extern BST *
newBST(void (*d)(void *,FILE *),int (*c)(void *,void *),void (*s)(BSTNODE *,BSTNODE *),void (*f)(void *))
{
  BST *tree = malloc(sizeof(BST));
  assert(tree != 0);
  tree->root       = 0;
  tree->size       = 0;
  tree->nodes      = newQUEUE(d,f);
  tree->display    = d;
  tree->compare    = c;
  tree->swapper    = s;
  tree->free       = f;
  return tree;
}


//this function returns the root of the given bst
extern BSTNODE *
getBSTroot(BST *t)
{
  return t->root;
}


//this function replaces the given bst's root with the replacement node passed in
extern void
setBSTroot(BST *t, BSTNODE *replacement)
{
  t->root = replacement;
  return;
}


//this function sets the size of the given tree to the number passed in
extern void
setBSTsize(BST *t,int s)
{
  t->size = s;
  return;
}


//this function is used to insert a node into the tree
extern BSTNODE *
insertBST(BST *t,void *value)
{
  BSTNODE *new = newBSTNODE(value);
  assert(new != 0);
  if (t->size == 0) { //tree is empty, insert at root
    t->root = new;
    new->parent = new;
    t->size++;
    return new;
  }

  BSTNODE *curr = t->root;
  while (new->parent == NULL) {
    if (t->compare(curr->value, value) > 0) { //node to be inserted is less than current node
      if (curr->left == NULL) {
        curr->left = new;
        new->parent = curr;
        t->size++;
        return new;
      }
      else {
        curr = curr->left;
      }
    }
    else {  //node to be inserted is greater than current node
      if (curr->right == NULL) {
        curr->right = new;
        new->parent = curr;
        t->size++;
        return new;
      }
      else {
        curr = curr->right;
      }
    }
  }
  return new;
}


//this function is used to search the tree for the given value
extern BSTNODE *
findBST(BST *t, void *value)
{
  if (t->root == NULL) {
    return NULL;
  }

  BSTNODE *curr = t->root;
  while (curr != NULL) {
    if (t->compare(curr->value, value) == 0) {
      return curr;
    }
    else if (curr->left == NULL && curr->right == NULL) {
      return NULL;
    }
    else if (t->compare(curr->value, value) > 0) {
      curr = curr->left;
    }
    else if (t->compare(curr->value, value) < 0) {
      curr = curr->right;
    }
  }
  return NULL;
}


//this function deletes the node with the given value from the tree
extern BSTNODE *
deleteBST(BST *t, void *value)
{
  BSTNODE *delete = swapToLeafBST(t, findBST(t, value));
  pruneLeafBST(t, delete);
  t->size--;
  return delete;
}


//this function is used to swap a given node to a leaf inorder for it to be pruned
extern BSTNODE *
swapToLeafBST(BST *t,BSTNODE *node) {
  if (node->left == NULL && node->right == NULL) {
    return node;
  }
  else {
    BSTNODE *swapwith = node;
    if (node->right != NULL) {
      node = node->right;
      while (node->left != NULL) {
      node = node->left;
      }
    }
    else {
      node = node->left;
      while (node->right != NULL) {
        node = node->right;
      }
    }
    if (t->swapper == 0) {
      void *temp = swapwith->value;
      swapwith->value = node->value;
      node->value = temp;
      return swapToLeafBST(t, node);
    }
    else {
      t->swapper(swapwith, node);
      return swapToLeafBST(t, node);
    }
  }
}


//this function removes the given leaf node from the tree
extern void
pruneLeafBST(BST *t, BSTNODE *leaf)
{
  if (t->size == 1) {
    t->root = NULL;
    return;
  }

  BSTNODE *parent = leaf->parent;
  if (getBSTNODEright(parent) == leaf) {
    parent->right = NULL;
    return;
  }
  else {
    parent->left = NULL;
    return;
  }
}


//this fucntion returns the size of the tree
extern int
sizeBST(BST *t)
{
  return t->size;
}


//this function is a helper to statisticsBST
//used to find the maximum path to a leaf in the tree
extern int
BSTmaxfinder(BSTNODE *curr, BST *t)
{
  if (curr == NULL) {
    return 0;
  }

  int leftd = BSTmaxfinder(curr->left, t);
  int rightd = BSTmaxfinder(curr->right, t);
  if (leftd > rightd) {
    leftd++;
    return leftd;
  }
  else {
    rightd++;
    return rightd;
  }
}


//this function is a helper to statisticsBST
//used to find the minimum path to a leaf in the tree
extern int
BSTminfinder(BSTNODE *curr, BST *t)
{
  if (curr == NULL) {
    return 0;
  }

  int leftd = BSTminfinder(curr->left, t);
  int rightd = BSTminfinder(curr->right, t);
  if (curr->left == NULL && curr->right != NULL) {
    leftd++;
    return leftd;
  }
  if (curr->right == NULL && curr->left != NULL) {
    rightd++;
    return rightd;
  }
  if (leftd < rightd) {
    leftd++;
    return leftd;
  }
  else {
    rightd++;
    return rightd;
  }
}


//this function prints the number of nodes, as well as max and min heights of the tree
extern void
statisticsBST(BST *t, FILE *fp)
{
  BSTNODE *root = t->root;
  int max = 0;
  int min = 0;
  if (t->root == NULL) {
    min = 0;
    max = 0;
  }
  else if (root->left == NULL && root->right == NULL) {
    min = 2;
    max = 2;
  }
  else {
    max = BSTmaxfinder(root, t);
    min = BSTminfinder(root, t);
  }
  fprintf(fp, "Nodes: %d\n", t->size);
  fprintf(fp, "Minimum depth: %d\n", min-1);
  fprintf(fp, "Maximum depth: %d\n", max-1);
  return;
}


//this function is a helper to displayBST
extern void
displayBSThelper(BST *t, BSTNODE *curr, FILE *fp) {
  printf(" [");
  t->display(curr->value, fp);
  if (curr->left != NULL) {
    displayBSThelper(t, curr->left, fp);
    printf("]");
  }

  if (curr->right != NULL) {
    displayBSThelper(t, curr->right, fp);
    printf("]");
  }
  return;
}


//this function displays the nodes of the tree
extern void
displayBST(BST *t,FILE *fp) {
  if (t->root == NULL) {
    printf("[]");
    return;
  }

  BSTNODE *curr = t->root;
  printf("[");
  t->display(curr->value, fp);
  if (curr->left != NULL) {
    displayBSThelper(t, curr->left, fp);
    printf("]");
  }
  if (curr->right != NULL) {
    displayBSThelper(t, curr->right, fp);
    printf("]");
  }
  printf("]");
  return;
}


//this function is a helper to displayBSTdebug
extern void
displayBSTdebughelper(BST *t, FILE *fp, int count) {
  if (sizeQUEUE(t->nodes) == 0) {
    return;
  }
  while (count > 0) {
    BSTNODE *curr = dequeue(t->nodes);
    t->display(curr->value, fp);
    if (curr->left != NULL) {
      enqueue(t->nodes, curr->left);
    }
    if (curr->right != NULL) {
      enqueue(t->nodes, curr->right);
    }
    if (count == 1) {
      printf("\n");
      if (sizeQUEUE(t->nodes) == 0) {
        break;
      }
      else {
        count--;
        displayBSTdebughelper(t, fp, sizeQUEUE(t->nodes));
      }
    }
    else {
      printf(" ");
      count--;
    }
  }
  return;
}


//this function prints a level ordered display of the tree
extern void
displayBSTdebug(BST *t,FILE *fp) {
  if (t->size == 0) {
    return;
  }
  else {
    BSTNODE *root = t->root;
    enqueue(t->nodes, root);
    displayBSTdebughelper(t, fp, 1);
  return;
  }
}


//this function returns the left child of the nodes parent
extern void *
getParentsLeftChild(BSTNODE *curr)
{
  BSTNODE *parent = getBSTNODEparent(curr);
  BSTNODE *l = getBSTNODEleft(parent);
  if (!l) return NULL;
  else return l;
}


//helper function to level ordered decorated display
extern void
displayBSTdecoratedhelper(BST *t, FILE *fp, int count, int level)
{
  if (sizeQUEUE(t->nodes) == 0) {
    return;
  }
  fprintf(fp, "%d: ", level);
  while (count > 0) {
    BSTNODE *curr = dequeue(t->nodes);
    if (!getBSTNODEleft(curr) && !getBSTNODEright(curr)) {
      fprintf(fp, "=");
    }
    t->display(curr->value, fp);

    fprintf(fp, "(");
    t->display(getBSTNODEvalue(getBSTNODEparent(curr)), fp);
    fprintf(fp, ")");
     // if (t->compare(getBSTNODEvalue(curr), getParentsLeftChild(curr)) == 0) {
    if (curr == getBSTNODEparent(curr)) fprintf(fp, "X");
    else if (curr == getParentsLeftChild(curr)) fprintf(fp, "L");
    else fprintf(fp, "R");

    if (curr->left != NULL) {
      enqueue(t->nodes, curr->left);
    }
    if (curr->right != NULL) {
      enqueue(t->nodes, curr->right);
    }
    if (count == 1) {
      printf("\n");
      if (sizeQUEUE(t->nodes) == 0) {
        break;
      }
      else {
        count--;
        level++;
        displayBSTdecoratedhelper(t, fp, sizeQUEUE(t->nodes), level);
      }
    }
    else {
      printf(" ");
      count--;
    }
  }
  return;
}


//this function displays the avl tree in level order, as well as with all extra statistics
extern void
displayBSTdecorated(BST *t, FILE *fp) {
  if (t->size == 0) {
    return;
  }
  else {
    // QUEUE *used = newQueue(t->display, t->free);
    BSTNODE *root = t->root;
    enqueue(t->nodes, root);
    displayBSTdecoratedhelper(t, fp, 1, 0);
  return;
  }
}


//this function is a helper to freeBST
//it frees the nodes of the given tree
extern void
freeBSThelper(BSTNODE *curr, BST *t) {
  if (curr == NULL) {
    return;
  }

  if (curr->left != NULL && t->free != NULL) {
    freeBSThelper(curr->left, t);
  }
  if (curr->right != NULL && t->free != NULL) {
    freeBSThelper(curr->right, t);
  }
  if (t->free != NULL) {
    freeBSTNODE(curr, t->free);
  }
}


//this function frees the given tree
extern void
freeBST(BST *t) {
  if (t->root == NULL) {
    freeQUEUE(t->nodes);
    free(t);
    return;
  }

  BSTNODE *root = t->root;
  freeBSThelper(root, t);
  freeQUEUE(t->nodes);
  free(t);
  return;
}
