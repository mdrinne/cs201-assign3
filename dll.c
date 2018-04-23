//  CS201 Assign0 dll.c
//  Matt Rinne
//  This file implements a Doubly Linked list
//  January 2018

#include "dll.h"
#include <stdlib.h>
#include <assert.h>


struct node1
{
  struct node1 *prev;
  struct node1 *next;
  void *data;
} ;

struct dll
{
  void *head;
  void *tail;
  int size;
  void (*display)(void *, FILE *);
  void (*free)(void *);
} ;


extern DLL *
newDLL(void (*d)(void *,FILE *),void (*f)(void *)) {  //constructor of a new Doubly Linked List
  DLL *items = malloc(sizeof(DLL));
  assert(items != 0);
  items->head = 0;
  items->tail = 0;
  items->size = 0;
  items->display = d;
  items->free = f;
  return items;
}


extern void
insertDLL(DLL *items,int index,void *value) { //insert a node to list
  assert(index >= 0 && index <= items->size);
  struct node1 *temp = malloc(sizeof(node1));
  temp->data = value;
  temp->prev = NULL;
  temp->next = NULL;

  if(items->size == 0) {  //add to empty list
    items->head = temp;
    items->tail = temp;
    items->size = 1;
    return;
  }

  if (index == 0) { //add to head
    struct node1 *curr = items->head;
    curr->prev = temp;
    temp->next = curr;
    items->head = temp;
    items->size++;
  }
  else if (index == items->size) {  //add to tail
    struct node1 *curr = items->tail;
    curr->next = temp;
    temp->prev = curr;
    temp->next = NULL;
    items->tail = temp;
    items->size++;
  }
  else if (index > items->size/2) { //add to second half
    struct node1 *curr = items->tail;
    int ind = items->size-1;
    while (index != ind) {
      curr = curr->prev;
      ind--;
    }
    struct node1 *prev = curr->prev;
    prev->next = temp;
    temp->prev = prev;
    temp->next = curr;
    curr->prev = temp;
    items->size++;
  }
  else {  //add to first half
    struct node1 *curr = items->head;
    for (int i=0; i<index-1; i++) {
      curr = curr->next;
    }
    struct node1 *next = curr->next;
    temp->prev = curr;
    temp->next = next;
    curr->next = temp;
    next->prev = temp;
    items->size++;
  }

  return;
}


extern void *
removeDLL(DLL *items,int index) { //remove a node from the list
  assert(items->size > 0 && index >= 0 && index < items->size);
  if (items->size == 1) {
    struct node1 *rem = items->head;
    void *value = rem->data;
    free(rem);
    items->head = 0;
    items->tail = 0;
    items->size--;
    return value;
  }

  if (index == 0) { //remove the head
    struct node1 *rem = items->head;
    struct node1 *newhead = rem->next;
    newhead->prev = NULL;
    items->head = newhead;
    items->size--;
    if (items->size == 1) {  //if only one item remains after node is removed, make head and tail the same node
      items->tail = items->head;
    }

    void *value = rem->data;
    free(rem);
    return value;
  }
  else if (index == items->size-1) {  //remove the tail
    struct node1 *rem = items->tail;
    struct node1 *newtail = rem->prev;
    newtail->next = NULL;
    items->tail = newtail;
    items->size--;
    void *value = rem->data;
    free(rem);
    return value;
  }
  else if (index > items->size/2) { //remove node from the second half
    struct node1 *rem = items->tail;
    int ind = items->size-1;
    while (index != ind) {
      rem = rem->prev;
      ind--;
    }
    struct node1 *prev = rem->prev;
    struct node1 *next = rem->next;
    prev->next = next;
    next->prev = prev;
    items->size--;
    void *value = rem->data;
    free(rem);
    return value;
  }
  else {  //remove a node from the first half
    struct node1 *rem = items->head;
    for (int i=0; i<index; i++) {
      rem = rem->next;
    }
    struct node1 *prev = rem->prev;
    struct node1 *next = rem->next;
    prev->next = next;
    next->prev = prev;
    items->size--;
    void *value = rem->data;
    free(rem);
    return value;
  }
}


extern void
unionDLL(DLL *recipient,DLL *donor) { //joins two list together
  if (donor->size == 0){
    return;
  }
  else {
    struct node1 *rt = recipient->tail;
    struct node1 *dh = donor->head;
    struct node1 *dt = donor->tail;
    rt->next = dh;
    dh->prev = rt;
    dt->next = NULL;
    recipient->tail = donor->tail;
    if (recipient->size == 0) {
      recipient->head = donor->head;
    }
    recipient->size = recipient->size + donor->size;
    donor->size = 0;
    return;
  }
}


extern void *
getDLL(DLL *items,int index) {  //returns the data at any given node
  assert(index >= 0 && index <items->size);
  struct node1 *get = items->head;
  if (index == 0) {
    return get->data;
  }
  else if (index == items->size-1) {
    get = items->tail;
    return get->data;
  }
  else if (index > items->size/2) {
    get = items->tail;
    int ind = items->size-1;
    while (index != ind) {
      get = get->prev;
      ind--;
    }
    return get->data;
  }
  else {
    for (int i=0; i<index; i++) {
      get = get->next;
    }
    return get->data;
  }
}


extern void *
setDLL(DLL *items,int index,void *value) {  //changes the given nodes data
  assert(index >= 0 && index <= items->size);
  if (index == items->size) { //if case of the index being outside the list, adds value to end of list
    struct node1 *new = malloc(sizeof(node1));
    struct node1 *tail = items->tail;
    void *empty = NULL;
    new->data = value;
    new->next = NULL;
    new->prev = tail;
    tail->next = new;
    items->tail = new;
    items->size++;
    return empty;
  }

  struct node1 *set = items->head;
  void *replaced = NULL;
  if (index == 0) {
    replaced = set->data;
    set->data = value;
  }
  else if (index == items->size-1) {
    set = items->tail;
    replaced = set->data;
    set->data = value;
  }
  else if (index > items->size/2) {
    set = items->tail;
    int ind = items->size-1;
    while (index != ind) {
      set = set->prev;
      ind--;
    }
    replaced = set->data;
    set->data = value;
  }
  else {
    for (int i=0; i<index; i++) {
      set = set->next;
    }
    replaced = set->data;
    set->data = value;
  }

  return replaced;
}


extern int
sizeDLL(DLL *items) { //returns the size of list
  return items->size;
}


extern void
displayDLL(DLL *items,FILE *fp) { //displays the contents of the given list
  printf("{{");
  if (items->size == 0) {
    printf("}}");
    return;
  }
  struct node1 *curr = items->head;
  while (curr != NULL) {
    items->display(curr->data, fp);
    if (curr->next != NULL) {
      printf(",");
    }
    curr = curr->next;
  }
  printf("}}");
  return;
}


extern void
displayDLLdebug(DLL *items,FILE *fp) {  //displays the "debugged" version of the list's contents
  printf("head->{{");
  if (items->size == 0) {
    printf("}},tail->{{}}");
    return;
  }
  struct node1 *curr = items->head;
  for (int i=0; i<items->size; i++) {
    items->display(curr->data, fp);
    if (curr->next != NULL) {
      printf(",");
    }
    curr = curr->next;
  }
  printf("}},tail->{{");
  if (items->size != 0) {
    curr = items->tail;
    items->display(curr->data, fp);
  }
  printf("}}");
  return;
}

extern void
freeDLL(DLL *items) { //frees the space allocated for the given list
  if (items->size == 0) {
    free(items);
    return;
  }
  struct node1 *curr = items->head;
  for (int i=0; i<items->size; i++) {
    struct node1 *prev = curr;
    if (items->free != NULL) {
      items->free(curr->data);
    }
    curr = curr->next;
    free(prev);
  }
  free(items);

  return;
}
