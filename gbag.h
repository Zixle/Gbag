#ifndef GBAG_H
#define GBAG_H

#include <stdbool.h>

typedef struct bagType *gbag; // a generic bag. Each item (optionally) stores with it two
                              // (pointers to) functions:
                              //   (1) a compare function
                              //       int cmp(void* a,void* b)
                              //       that is passed two pointers to items in the gbag
                              //       and returns n such that
                              //          n < 0  if *a < *b,
                              //          n = 0  if *a == *b, and
                              //          n > 0  if *a > *b
                              //   (2) a item-to-string function
                              //       char *tos(void*)
                              //       that is passed an item and returns the string
                              //       representation of that item

gbag createBag();   // create a new bag
void freeBag(gbag); // deallocates a bag 

void emptyBag(gbag);           // emptys the bag

void bagCopy(gbag b1,gbag b2); // copies b2 to b1, overwriting b1; performs
                               // shallow copy of data,copies functions as well

int size(gbag);                // returns the number of items in bag

int count(gbag b,void* p);     // returns number of items p occurs in b (tests 
                               // if pointers are equal)

//int erase(gbag b,void* p);     // returns index of item it removes if found, -1
                               // otherwise; uses pointer equality

int eraseN(gbag b,int n);      // removes item at position n (where 0 is the first
                               // item); returns index of item removed or -1 if
                               // nothing removed

void add(gbag,void*);          // inserts value in bag; set functions for item to NULL

void setFuncs(gbag b,int n,int (*cmp)(),char* (*tos)());
                               // sets compare and to-string functions for item n in b;
                               // does nothing if n is invalid

void addWithFuncs(gbag b,void *ptr,int (*cmp)(),char* (*tos)());
                               // combination of add and setFuncs

void* get(gbag b,int n);       // returns item at n in b where 0 is first item or
                               // NULL if n is invalid

bool contains(gbag b,void* p); // returns true if p is in b, false otherwise
                               // (compares pointers)
int find(gbag b, void *p);     // returns index of item found or -1 if not found;
                               // uses cmp function of item to test for equality.
                               // if cmp == NULL for item, skip it.

bool isEmpty(gbag);            // return true if bag is empty

bool equal(gbag b1,gbag b2);   // returns true if b1 == b2 (shallow (pointer)
                               // equality, including functions). Empty bags are equal.

bool equalItems(gbag b1,gbag b2); // returns true if all items are equivalent determined
                                  // by compare functions, where functions must be identical.
                                  // Only compares items that both have compare functions;
                                  // skips items otherwise. Two bags of equal sizes with no
                                  // items having compare functions are therefore equal; empty
                                  // bags are equal.

char *bagToString(gbag);       // delimits bag lists with {}; bag items with no toString
                               // functions are noted with NULL, e.g.
                               // {}                    an empty bag
                               // {10,1,230000,hi mom}  a 3-item bag with all items
                               //                       having a toString function
                               // {1,NULL,NULL,8}       a 4-item bag with 2 items
                               //                       not having a toString function

#endif
