//  CS201 Assign0 sll.c
//  Matt Rinne
//  This file implements a Singlely Linked List
//  January 2018

#include "sll.h"
#include <stdlib.h>
#include <assert.h>


struct node
{
  struct node *next;
  void *data;
};

struct sll
{
  void *head;
  void *tail;
  int size;
  void (*display)(void *, FILE *);
  void (*free)(void *);
} ;


extern SLL *
newSLL(void (*d)(void *, FILE *), void (*f)(void *)) {  //constructor of a new Singlely Linked List
  SLL *items = malloc(sizeof(SLL));
  assert(items != 0);
  items->head = 0;
  items->tail = 0;
  items->size = 0;
  items->display = d;
  items->free = f;
  return items;
}


extern void
insertSLL(SLL *items, int index, void *value) { //insert a node to list
  assert(index >= 0 && index <= items->size);
  struct node *temp = malloc(sizeof(node));
  temp->data = value;
  temp->next = NULL;

  if (items->size == 0) { //add to empty list
    items->head = temp;
    items->tail = temp;
    items->size = 1;
    return;
  }

  if (index == 0) {  //add to head
    temp->next = items->head;
    items->head = temp;
    items->size++;
  }
  else if (index == items->size) { //add to tail
    struct node *last = items->tail;
    last->next = temp;
    items->tail = temp;
    items->size++;
  }
  else {  //add to middle
    struct node *curr = items->head;
    for (int i=0; i<index-1; i++) {
      curr = curr->next;
    }
    temp->next = curr->next;
    curr->next = temp;
    items->size++;
  }

  return;
}


extern void *
removeSLL(SLL *items, int index) {  //remove a node from the lsit
  assert(items->size > 0);
  assert(index >= 0 && index < items->size);
  if (items->size == 1) {
    struct node *rem = items->head;
    void *value = rem->data;
    free(rem);
    items->head = 0;
    items->tail = 0;
    items->size--;
    return value;
  }

  if (index == 0) {                   //remove the head
    struct node *rem = items->head;
    items->head = rem->next;
    items->size--;
    if (items->size == 1) {          //if only one item remains after node is removed, make head and tail the same node
      items->tail = items->head;
    }

    void *value = rem->data;
    free(rem);
    return value;
  }
  else if (index == items->size-1) { //remove the tail
    struct node *rem = items->tail;
    struct node *prev = items->head;
    for (int i=0; i<index-1; i++) {
      prev = prev->next;
    }
    prev->next = NULL;
    items->tail = prev;
    items->size--;
    void *value = rem->data;
    free(rem);
    return value;
  }
  else {   //remove a node from the middle
    struct node *rem = items->head;
    struct node *prev = items->head;
    for (int i=0; i<index; i++) {
      rem = rem->next;
      prev = prev->next;
      if (i == 0) {
        prev = items->head;
      }
    }
    prev->next = rem->next;
    items->size--;
    void *value = rem->data;
    free(rem);
    return value;
  }
}


extern void
unionSLL(SLL *recipient, SLL *donor) {  //joins two lists together
  if (donor->size == 0) {
    return;
  }
  else {
    struct node *rt = recipient->tail;
    rt->next = donor->head;
    recipient->tail = donor->tail;
    if (recipient->size == 0) {
      recipient->head = donor->head;
    }
    recipient->size = recipient->size + donor->size;
    donor->head = 0;
    donor->size = 0;
    donor->size = 0;
    return;
  }
}


extern void *
getSLL(SLL *items, int index) { //returns the data at any given node
  assert(index >= 0 && index <= items->size);
  struct node *get = items->head;
  if (index == 0) {
    return get->data;
  }
  else if (index == items->size-1) {
    get = items->tail;
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
setSLL(SLL *items, int index, void *value) {  //changes the given nodes data
  assert(index >= 0 && index <= items->size);
  if (index == items->size) { //incase of the index being outside of list, adds value to end of list
    struct node *new = malloc(sizeof(node));
    void *empty = NULL;
    new->data = value;
    new->next = NULL;
    struct node *temp = items->tail;
    temp->next = new;
    items->tail = new;
    items->size++;
    return empty;
  }

  struct node *set = items->head;
  void *rep = NULL;
  if (index == 0) {
    rep = set->data;
    set->data = value;
  }
  else if (index == items->size-1) {
    set = items->tail;
    rep = set->data;
    set->data = value;
  }
  else {
    for (int i=0; i<index; i++) {
      set = set->next;
    }
    rep = set->data;
    set->data = value;
  }

  return rep;
}


extern int
sizeSLL(SLL *items) { //returns the size of the list
  return items->size;
}


extern void
displaySLL(SLL *items, FILE *fp) {  //displays the contents of the given list
  printf("{");
  if (items->size == 0) {
    printf("}");
    return;
  }
  struct node *curr = items->head;
  for (int i=0; i<items->size; i++) {
    items->display(curr->data, fp);
    if (i != items->size-1) {
      printf(",");
    }
    curr = curr->next;
  }
  printf("}");
  return;
}


extern void
displaySLLdebug(SLL *items, FILE *fp) { //displays the "debugged" version the the list's contents
  printf("head->{");
  if (items->size == 0) {
    printf("},tail->{}");
    return;
  }
  struct node *curr = items->head;
  for (int i=0; i<items->size; i++) {
    items->display(curr->data, fp);
    if (i != items->size-1) {
      printf(",");
    }
    curr = curr->next;
  }

  printf("},tail->{");
  if (items->size != 0) {
  curr = items->tail;
  items->display(curr->data, fp);
  }
  printf("}");
  return;
}


extern void
freeSLL(SLL *items) { //frees the space allocated for the given list
  if (items->size == 0) {
    free(items);
    return;
  }

  struct node *curr = items->head;
  for (int i=0; i<items->size; i++) {
    struct node *prev = curr;
    if (items->free != NULL) {
      items->free(curr->data);
    }
    curr = curr->next;
    free(prev);
  }
  free(items);

  return;
}
