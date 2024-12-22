#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gbag.h"

char *intToS(int*);
char *floatToS(float*);
char *strToStr(char*);
int intCmp(int *, int *);
int floatCmp(float *, float *);


struct ItemInfo{ 
	int (*cmp)(void* a,void* b); // hold address of function(pointer), passes two void pointers// compare function
	char* (*tos)(void*); // hold adress of a function, will pass a void* but return char*/ convert to string function	
	
};

 struct bagType { 
	struct ItemInfo (*info); 
	void * (*items);  //array of addresses
	int size;
	int count;//number of items currently in  bag 
};
gbag createBag(){ // create a new bag
	
	gbag newbag = malloc(sizeof(struct bagType));
	
	newbag->info = malloc(sizeof(struct ItemInfo) * 5);
	newbag->items = malloc(sizeof(void*) * 5); // nothing in bag currently
	newbag->size = 5; // initial number of slots
	newbag->count = 0;// nothing in bag currently
	
	return newbag; 
}  
void freeBag(gbag mybag){// deallocates a bag 
	free(mybag->info);
	free(mybag->items);
	free(mybag);
} 

void emptyBag(gbag mybag){ // emptys the bag
gbag newbag = createBag();
free(mybag->info);
free(mybag->items);
free(mybag);
mybag =  malloc(sizeof(struct bagType));
mybag= newbag;

}          

void bagCopy(gbag b1,gbag b2){ 
if (!isEmpty(b1)){
	emptyBag(b1);
}

	for (int i=0; i < b2->count; i++){
		add(b1, b2->items[i]);
		setFuncs(b1, i,b2->info[i].cmp,  b2->info[i].tos);
		
	}	

} // copies b2 to b1, overwriting b1; performs
                               // shallow copy of data,copies functions as well

int size(gbag mybag){    // returns the number of items in bag

	return mybag->count;

}

int count(gbag b,void* p){
int count=0;
	for (int i =0; i< b->count; i++){
		if ( b->items[i] == p) {
			count++;
		}
	}
	return count;

}     // returns number of items p occurs in b (tests 
                               // if pointers are equal)

int erase(gbag b, void *p) {
    int track = -1;

   
    for (int i = 0; i < b->count; i++) {
        if (b->items[i] == p) {
            track = i;
            break;
        }
    }
  
//track is the index
    if (track == -1) {
       
       return -1;
       
       }
       else {
        for (int i = track; i < b->count -1 ; i++) {
        	//printf(" track %d",i+1);
            b->items[i] = b->items[i + 1];
            b->info[i].cmp =  b->info[i + 1].cmp;
            b->info[i].tos =  b->info[i + 1].tos;
        }
		
		
     	b->count--;
    

    return track;
    }
}
     // returns index of item it removes if found, -1
                               // otherwise; uses pointer equality

int eraseN(gbag b,int n){
	if ( isEmpty(b)|| n < 0 || n >= b->count){
		return -1;
	}
	
	for (int i = n; i < b->count -1 ; i++) {
        	//printf(" track %d",i+1);
            b->items[i] = b->items[i + 1];
            b->info[i].cmp =  b->info[i + 1].cmp;
            b->info[i].tos =  b->info[i + 1].tos;
        }
		
		b->items = realloc (b->items, sizeof(void*) * (b->count -1));
     	b->size = b->count - 1;
     	b->count--;
	return n;
}      // removes item at position n (where 0 is the first
                               // item); returns index of item removed or -1 if
                               // nothing removed

void add(gbag mybag ,void* obj){
	mybag->count++;
	//printf("BAG COUNT %d",mybag->count );
	if(mybag->count  >= mybag->size ){//meaning its full, 
		mybag->items = realloc(mybag->items, sizeof(void*) * (mybag->size + 5)); // add 5 more slots
		mybag->info = realloc(mybag->info, sizeof(struct ItemInfo) * (mybag->size + 5));
		mybag->size+= 5;	
	} 
	
	//printf("OB %d \n\n", *(int*)obj);
	mybag->items[mybag->count-1] = obj;
	
	/*DEBUG*/
	//printf("**DEBUG: %d \n\n\n ", *((int*)mybag->items[mybag->count - 1]));
	
}     // inserts value in bag; set functions for item to NULL

void setFuncs(gbag b,int n,int (*cmp)(),char* (*tos)()){ // n is the position
	int i =1;

        b->info[n].cmp = cmp;
        b->info[n].tos = tos;
    
}
                               // sets compare and to-string functions for item n in b;
                               // does nothing if n is invalid

void addWithFuncs(gbag mybag,void *ptr,int (*cmp)(),char* (*tos)()){

	mybag->count++;
	if(mybag->count  >= mybag->size ){//meaning its full, 
	
		mybag->items = realloc(mybag->items, sizeof(void*) * (mybag->size + 5)); // add 5 more slots
		mybag->info = realloc(mybag->info, sizeof(struct ItemInfo) * (mybag->size + 5));
		mybag->size+= 5;	
	} 
	mybag->items[mybag->count - 1]= ptr;
	// set functions
        mybag->info[mybag->count-1].cmp = cmp;
        mybag->info[mybag->count -1 ].tos = tos;


}
                               // combination of add and setFuncs

void* get(gbag b,int n){
	if (n >= b->count){
		return NULL;
	}
return b->items[n];

}    // returns item at n in b where 0 is first item or
                               // NULL if n is invalid

bool contains(gbag b,void* p){
	for (int i =0; i < b->count; i ++){
		if ( b->items[i]== p ){
			return true; 
		
		}
	
	}

	return false; 
} // returns true if p is in b, false otherwise
                               // (compares pointers)
int find(gbag b, void *p) {

    for (int i = 0; i < b->count; i++) {
        if (b->info[i].cmp == NULL) {
            continue; // Skip items with no compare function
        } else {
            
            if (b->info[i].cmp(b->items[i], p) == 0) {
                return i;
            }
        }
    }
    return -1;
}
   // returns index of item found or -1 if not found;
                               // uses cmp function of item to test for equality.
                               // if cmp == NULL for item, skip it.

bool isEmpty(gbag mybag){// return true if bag is empty

	return mybag->count == 0; //will return true if this is true

}          

bool equal(gbag b1,gbag b2){
	if (isEmpty(b1) && isEmpty(b2) ){
		return true;
	}
	for (int i =0; i < b1->count; i++){
		if (b1->info[i].cmp != b2->info[i].cmp){
			return false;
		}
		else { //same function
			if (b1->info[i].cmp(b1->items[i], b2->items[i]) == 0 ){
					continue;
			}
			else{
				return false;
			}
		}
	
	}
	return true; // if it got out of the loop that means its equal
}   // returns true if b1 == b2 (shallow (pointer)
                               // equality, including functions). Empty bags are equal.

bool equalItems(gbag b1,gbag b2){
if (b1->count != b2->count){
return false;
}
	for (int i =0 ; i < b1->count; i++){
		if (b1->info[i].cmp ==NULL ||  b2->info[i].cmp == NULL){
				continue; 
			}
			else {
				if (b1->info[i].cmp != b2->info[i].cmp ){
						return false;
				}
			
			}
	
	}
	// if it made it out of the loop that means it either has all null cmp funcs or everything is equal

	return true;

}// returns true if all items are equivalent determined
                                  // by compare functions, where functions must be identical.
                                  // Only compares items that both have compare functions;
                                  // skips items otherwise. Two bags of equal sizes with no
                                  // items having compare functions are therefore equal; empty
                                  // bags are equal.

char *bagToString(gbag mybag) {
int loopnum=0;
int i =0;

    if (mybag->count == 0) {
        return "{}"; // bag is empty
    }

    char *final = malloc(1);
    int finalsize =1;
    final[0] = '{';
    char *str = NULL;

    for (int i = 0; i < mybag->count; i++) {
    	loopnum++;
        if (mybag->info[i].tos == NULL) {
        	if (loopnum>1){
        		final = realloc(final, strlen(final) + 7); // the word null, comma, },and \0
				final[strlen(final)] = ',';
				strcat(final, "NULL");
				
        	}
        	else {
        	//first loop
            final = realloc(final, strlen(final) + 6);
            finalsize = strlen(final) + 6;
            strcat(final, "NULL");
        	}
            
        } else {
            str = mybag->info[i].tos(mybag->items[i]);

             if (loopnum > 1){
             	final = realloc(final, strlen(final) + strlen(str) + 3); // comma, }, and \0
             	final[strlen(final)] = ',';
             	strcat(final, str);
             	
             }
            else{
            final = realloc(final, strlen(final) + strlen(str) + 2); 
           	finalsize = strlen(final) + strlen(str) + 2;
            strcat(final, str);
            }
            free(str); // Free the memory allocated by tos
        }
        
    }
    
    
	final[strlen(final)] = '}';
	final[strlen(final) + 1] = '\0';
    return final;
}
   // delimits bag lists with {}; bag items with no toString
                               // functions are noted with NULL, e.g.
                               // {}                    an empty bag
                               // {10,1,230000,hi mom}  a 3-item bag with all items
                               //                       having a toString function
                               // {1,NULL,NULL,8}       a 4-item bag with 2 items
                               //                       not having a toString function


