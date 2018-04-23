#ifndef __BST_INCLUDED__
#define __BST_INCLUDED__

#include <stdio.h>

typedef struct bstnode BSTNODE;

    extern BSTNODE *newBSTNODE(void *v);
    extern void    *getBSTNODEvalue(BSTNODE *n);
    extern void    setBSTNODEvalue(BSTNODE *n,void *value);
    extern BSTNODE *getBSTNODEleft(BSTNODE *n);
    extern void    setBSTNODEleft(BSTNODE *n,BSTNODE *replacement);
    extern BSTNODE *getBSTNODEright(BSTNODE *n);
    extern void    setBSTNODEright(BSTNODE *n,BSTNODE *replacement);
    extern BSTNODE *getBSTNODEparent(BSTNODE *n);
    extern void    setBSTNODEparent(BSTNODE *n,BSTNODE *replacement);
    extern void    freeBSTNODE(BSTNODE *n,void (*free)(void *));

    typedef struct bst BST;

    extern BST *newBST(
        void (*d)(void *,FILE *),           //display
        int (*c)(void *,void *),            //comparator
        void (*s)(BSTNODE *,BSTNODE *),     //swapper
        void (*f)(void *));                 //free
    extern BSTNODE *getBSTroot(BST *t);
    extern void    setBSTroot(BST *t,BSTNODE *replacement);
    extern void    setBSTsize(BST *t,int s);
    extern BSTNODE *insertBST(BST *t,void *value);
    extern BSTNODE *findBST(BST *t,void *value);
    extern BSTNODE *deleteBST(BST *t,void *value);
    extern BSTNODE *swapToLeafBST(BST *t,BSTNODE *node);
    extern void    pruneLeafBST(BST *t,BSTNODE *leaf);
    extern int     sizeBST(BST *t);
    extern int     BSTmaxfinder(BSTNODE *curr, BST *t);
    extern int     BSTminfinder(BSTNODE *curr, BST *t);
    extern void    statisticsBST(BST *t,FILE *fp);
    extern void    displayBST(BST *t,FILE *fp);
    extern void    displayBSTdebughelper(BST *t, FILE *fp, int count);
    extern void    displayBSTdebug(BST *t,FILE *fp);
    extern void    displayBSTdecoratedhelper(BST *t, FILE *fp, int count, int level);
    extern void    displayBSTdecorated(BST *t, FILE *fp);
    extern void    freeBSThelper(BSTNODE *curr, BST *t);
    extern void    freeBST(BST *t);
    #endif
