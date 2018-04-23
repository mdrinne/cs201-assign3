//  CS201 Assign0 stack.c
//  Matt Rinne
//  This file implements a Stack with a Doubly Linked List underlying data structure
//  January 2018

#include "stack.h"
#include "dll.h"
#include <stdlib.h>
#include <assert.h>


struct stack
{
  DLL *pile;
  int size;
  void (*display)(void *, FILE *);
  void (*free)(void *);
} ;


extern STACK *
newSTACK(void (*d)(void *,FILE *),void (*f)(void *)) {  //constructor of a new Stack
  STACK *items = malloc(sizeof(STACK));
  assert(items != 0);
  items->pile = newDLL(d, f);
  items->size = 0;
  items->display = d;
  items->free = f;
  return items;
}


extern void
push(STACK *items,void *value) {  //pushes a value onto the front of the Stack
  insertDLL(items->pile, 0, value);
  items->size++;
  return;
}


extern void *
pop(STACK *items) { //removes the value at the fron of the Stack
  assert(items != 0);
  void *popped = removeDLL(items->pile, 0);
  items->size--;
  return popped;
}


extern void *
peekSTACK(STACK *items) { //returns the value at the front of the Stack
  assert(items != 0);
  void *peeked = getDLL(items->pile, 0);
  return peeked;
}


extern int
sizeSTACK(STACK *items) { //returns the size of the Stack
  return items->size;
}


extern void
displaySTACK(STACK *items,FILE *fp) { //displays the contents of the Stack
  printf("|");
  for (int i=0; i<items->size; i++){
    void *disp = getDLL(items->pile, i);
    items->display(disp, fp);
    if (i != items->size-1) {
      printf(",");
    }
  }
  printf("|");
  return;
}


extern void
displaySTACKdebug(STACK *items,FILE *fp) {  //displays the "debugged" version of the Stack
  displayDLLdebug(items->pile, fp);
  return;
}


extern void
freeSTACK(STACK *items) { //frees the space allovated for the given Stack
  freeDLL(items->pile);
  free(items);
  return;
}
