/* CSCI 401: Lab #7: Memory Management Simulation
 * Programmer (Student ID): Ben Corriette (@02956064)
 * Last modified date: 12/11/2021
 *
 * Summary: Implementation for linked list.
 * 
 * Reference: https://stackoverflow.com/questions/8440816/warning-implicit-declaration-of-function 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

bool compareSize(int a, block_t *b) {  // greater or equal
  
  if(a <= (b->end - b->start) + 1)
     return true;
  
  return false;
}

list_t *list_alloc() { 
  list_t* list = (list_t*)malloc(sizeof(list_t));
  list->head = NULL;
  return list; 
}

node_t *node_alloc(block_t *blk) {   
  node_t* node = malloc(sizeof(node_t));
  node->next = NULL;
  node->blk = blk;
  return node; 
}

void list_free(list_t *l){
  free(l);
  return;
}

void node_free(node_t *node){
  free(node);
  return;
}

void list_print(list_t *l) {
  node_t *current = l->head;
  block_t *b;
  
  if (current == NULL){
    printf("list is empty\n");
  }
  while (current != NULL){
    b = current->blk;
    printf("PID=%d START:%d END:%d", b->pid, b->start, b->end);
    current = current->next;
  }
}

int list_length(list_t *l) { 
  node_t *current = l->head;
  int i = 0;
  while (current != NULL){
    i++;
    current = current->next;
  }
  
  return i; 
}

void list_add_to_back(list_t *l, block_t *blk){  
  node_t* newNode = node_alloc(blk);
  newNode->next = NULL;
  if(l->head == NULL){
    l->head = newNode;
  }
  else{
    node_t *current = l->head;
    while(current->next != NULL){
      current = current->next;
    }
    current->next = newNode;
  }
}

void list_add_to_front(list_t *l, block_t *blk){  
  node_t* newNode = node_alloc(blk);
 
  newNode->next = l->head;
  l->head = newNode;
}

void list_add_at_index(list_t *l, block_t *blk, int index){
  int i = 0;
  
  node_t *newNode = node_alloc(blk);
  node_t *current = l->head;

  if(index == 0){
    newNode->next = l->head->next;
    l->head = newNode;
  }
  else if(index > 0){
    while(i < index && current->next != NULL){
      current = current->next;
      i++;
    }
  newNode->next = current->next;
  current->next = newNode;
  }
}

/*
 * 1. Insert newblk into list l in ascending order based on the START address of the block.
 */ 
void list_add_ascending_by_address(list_t *l, block_t *newblk){
  node_t *current;
  node_t *prev;
  block_t *current_block;
  node_t *new_node = node_alloc(newblk);
  //printf("Variable assignments done: \n");
  if (l->head == NULL){
    //printf("New node assignment done: \n");
    l->head = new_node;
  }
  else {
  /*    node_t *c = l.head; */
  prev = current = l->head;
  current_block = current->blk;
  //printf("Current node assignment done: \n");
  /*    Insert newblk After Current Node if:   newblk->start > c->start  */
  if (current->next == NULL) {  //only one node in list
    //printf("First if statement done: \n");
      //if (newblk->start == 0) {
        //printf("Second if statement done: \n");
        //printf("Memory blocksize: %d\n", newblk->end);
     if (newblk->start > current_block->start) {  // place in front of current node
        current->next = new_node;
     } else {   // place behind current node
        current->next = l->head;
        l->head = new_node;
     }
  } else {  // two or more nodes in list
     if (newblk->start > current_block->start) {  // place in front of current node
        current->next = new_node;
     } else {
        while (current != NULL) {
             prev = current;
             current = current->next;
          
            if (newblk->start > current_block->start) {  // place in front of current node
                current->next = new_node;
                break;
            }
        }
        if (current == NULL) {  // the last one in the list
             prev->next = current;
             current->next = new_node;
             new_node->next = NULL;
        }
     }
   }
  }
}
   
/* 1. Insert newblk into list l in ascending order based on the blocksize. */
void list_add_ascending_by_blocksize(list_t *l, block_t *newblk){
  /*         node_t *c = l.head; */
  node_t *current = l->head;
  node_t *prev;
  node_t *new_node = node_alloc(newblk);
  block_t* current_block = current->blk;
  
  /*    blocksize is calculated :  blocksize = end - start +1
   *    Ex:  blocksize = newblk->end - newblk->start
   *         curr_blocksize = c->blk->end - c->blk->start +1; */
  int newblk_size = newblk->end - newblk->start;
  int curblk_size = current_block->end - current_block->start + 1;
  
  /*    Insert newblk After Current Node if:   blocksize >= curr_blocksize */   
  if (l->head == NULL){
    l->head = new_node;
  }
  else {
    prev = current = l->head;
    
    if (current->next == NULL) {  //only one node in list
       if (newblk_size >= curblk_size) {  // place in front of current node
          current->next = new_node;
       } else {   // place behind current node
          new_node->next = current;
          l->head = new_node;
       }
    }
    else {  // two or more nodes in list
       if(newblk_size >= curblk_size) {  // place in front of current node
          current->next = new_node;
       } else {
          while (current != NULL) {
               prev = current;
               current = current->next;
               
               /*    USE the compareSize()   */
               bool is_new_blocksize_larger = compareSize(curblk_size, newblk);
               if (is_new_blocksize_larger) {
                 current->next = new_node;
                 break;
               }
          }
          if (current == NULL) {  // the last one in the list
             prev->next = current;
             current->next = new_node;
             new_node->next = NULL;
          }
       }
    }
  }
}

void list_add_descending_by_blocksize(list_t *l, block_t *blk){
  node_t *current;
  node_t *prev;
  node_t *newNode = node_alloc(blk);
  int newblk_size = blk->end - blk->start;
  int curblk_size;
  
  if(l->head == NULL){
    l->head = newNode;
  }
  else{
    prev = current = l->head;
    
    curblk_size = current->blk->end - current->blk->start + 1;
    
    if(current->next == NULL) {  //only one node in list
       if(newblk_size >= curblk_size) {  // place in front of current node
          newNode->next = l->head;
          l->head = newNode;
       }
       else {   // place behind current node
          current->next = newNode;
          newNode->next = NULL;
       }
    }
    else {  // two or more nodes in list
      
       if(newblk_size >= curblk_size) {  // place in front of current node
          newNode->next = l->head;
          l->head = newNode;
       }
       else {
      
          while(current != NULL && newblk_size <= curblk_size) {
               prev = current;
               current = current->next;
               
               if(current != NULL)  // the last one in the list
                     curblk_size = current->blk->end - current->blk->start;
          }
          prev->next = newNode;
          newNode->next = current;
       }
    }
  }
}

void list_coalese_nodes(list_t *l){
  node_t *current = l->head;
  node_t *prev;
  block_t * current_block = current->blk;
  block_t * prev_block;
  int prev_node_end_memory_block_address;
  int current_node_start_memory_block_address;

  /* 1. Assuming you have passed in a sorted list of blocks based on addresses in ascending order
   * 2. While list is not empty,*/
    while (list_length(l) > 0) {
      prev = current;
      current = current->next;
      prev_block = prev->blk;
      
      if (current != NULL) { 
        prev_node_end_memory_block_address = prev_block->end + 1;
        current_node_start_memory_block_address = current_block->start;
        
        /*    a. compare two nodes at a time to see if the prev.END + 1 == current.START, if so, they are physically adjacent */
        if (prev_node_end_memory_block_address == current_node_start_memory_block_address) {
          /*    combine them by setting the prev.END = current.END. */
          prev_block->end = current_block->end;
          /* 3. point the prev.NEXT to the current.NEXT to skip over current. */
          prev->next = current->next;
          /* 4. Free current */
          node_free(current);
          /* 5. go back to #2 */
        } else { /*    b. If not adjacent go to #6 */
             /* 6. Advance prev = current, and current = current.NEXT */
             prev = current;
             current = current->next;
             /* 7. go back to #2 */
        }
      }
   }
}

block_t* list_remove_from_back(list_t *l){
  block_t *value = NULL;
  node_t *current = l->head;

  if(l->head != NULL){
    
    if(current->next == NULL) { // one node
         l->head->next = NULL;
         value = current->blk;
         node_free(current);
    }
    else {
         while (current->next->next != NULL){
            current = current->next;
         }
         value = current->blk;
         node_free(current->next);
         current->next = NULL;
    }
  }
  return value;
}

block_t* list_get_from_front(list_t *l) {
  block_t *value = NULL;
  if(l->head == NULL){
    return value;
  }
  else{
    node_t *current = l->head;
    value = current->blk;
  }
  return value; 
}


block_t* list_remove_from_front(list_t *l) { 
  block_t *value = NULL;
  if(l->head == NULL){
    return value;
  }
  else{
    node_t *current = l->head;
    value = current->blk;
    l->head = l->head->next;
    node_free(current);
  }
  return value; 
}

block_t* list_remove_at_index(list_t *l, int index) { 
  int i;
  block_t* value = NULL;
  
  bool found = false;
  
  if(l->head == NULL){
    return value;
  }
  else if (index == 0){
    return list_remove_from_front(l);
  }
  else if (index > 0){
    node_t *current = l->head;
    node_t *prev = current;
    
    i = 0;
    while(current != NULL && !found){
      if(i == index)
          found = true;
      else {
         prev = current;
         current = current->next;
         i++;
      }
    }
    
    if(found) {
      value = current->blk; 
      prev->next = current->next;
      node_free(current);
    }
  }
  return value; 
}

bool compareBlks(block_t* a, block_t *b) {
  
  if(a->pid == b->pid && a->start == b->start && a->end == b->end)
     return true;
  
  return false;
}

bool comparePid(int a, block_t *b) {
  
  if(a == b->pid)
     return true;
  
  return false;
}


bool list_is_in(list_t *l, block_t* value) { 
  node_t *current = l->head;
  while(current != NULL){
    if(compareBlks(value, current->blk)){
      return true;
    }
    current = current->next;
  }
return false; 
}

block_t* list_get_elem_at(list_t *l, int index) { 
 int i;
  block_t* value = NULL;
  if(l->head == NULL){
    return value;
  }
  else if (index == 0){
    return list_get_from_front(l);
  }
  else if (index > 0){
    node_t *current = l->head;
    
    i = 0;
    while(current != NULL){
      if(i == index)
          return(current->blk);
      else {
         current = current->next;
         i++;
      }
    }
  }
  return value; 
}

int list_get_index_of(list_t *l, block_t* value){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (compareBlks(value,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }
  return -1; 
}

/* Checks to see if block of Size or greater exists in the list. */
bool list_is_in_by_size(list_t *l, int Size){ 
  node_t *current = l->head;
  while(current != NULL){
    if(compareSize(Size, current->blk)){
      return true;
    }
    current = current->next;
  }
return false; 
}

/* Checks to see if pid of block exists in the list. */
bool list_is_in_by_pid(list_t *l, int pid){
  /* Iterate through the list to find a node with a blk that has blk->pid = pid  */ 
  node_t *current = l->head;
  while(current != NULL){
    /* USE the comparePID() */ 
    if(comparePid(pid, current->blk)){
      return true;
    }
    current = current->next;
  }
  return false;
}

/* Returns the index at which the given block of Size or greater appears. */
int list_get_index_of_by_Size(list_t *l, int Size){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (compareSize(Size,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }

  return -1; 
}
                   
/* Returns the index at which the pid appears. */
int list_get_index_of_by_Pid(list_t *l, int pid){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (comparePid(pid,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }

  return -1; 
}