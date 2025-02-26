/**
 * Aleida Diaz-Roque 
 * adiazroq 
 * 2024 Winter CSE101 PA3
 * List.h 
 * Header file for List ADT
*/ 

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>


#define FORMAT "%d"

// Exported types -------------------------------------------------------------
// typedef int ListElement;
typedef struct ListObj* List;

// CONSTRUCTORS - DESTRUCTORS --------------------------------------------------- 

/**
 * newList creates and returns a new empty List.   
 * @param: none
 * @return: none
 * Constructor
 */
List newList(void);

/**
 * freeList frees all heap memory associated with *pL.  
 * @param: pL, a list object pointer
 * @return: none
 * Destructor
 * sets *pL to NULL
 */
void freeList(List* pL);


// ACCESS FUNCTIONS ----------------------------------------------------------- 

/**
 * length returns the number of elements in L.
 * @param: L, a list object
 * @return: size of list
 */
int length(List L);

/**
 * index returns index of cursor element if defined, -1 otherwise.
 * @param: L, a list object
 * @return: index of cursor
 */
int indexL(List L); 


/**
 * front returns front element of L.
 * @param: L, a list object
 * @return: index of front element
 * Pre: length(L) > 0 
 */
int front(List L);

/**
 * back returns back element of L.
 * @param: L, a list object
 * @return: index of back element
 * Pre: length(L) > 0 
 */
int back(List L);  

/**
 * get returns cursor element of L.
 * @param: L, a list object
 * @return: cursor element
 * Pre: length()>0, index()>=0
 */
int get(List L);

/**
 * equals returns true iff Lists A and B contain the same sequence of elements, 
 * returns false otherwise.
 * @param: L, a list object
 * @return: index of cursor
 */
bool equals(List A, List B); 


// MANIPULATION PROCEDURES ---------------------------------------------------- 

/**
 * clear resets L to its original empty state.
 * @param: L, a list object
 * @return: none
 */
void clear(List L);        

/**
 * set overwrites the cursor element’s data with x. 
 * @param: L, a list object and x as index for element to change
 * @return: none
 * Pre: length() > 0, index() >= 0 
 */
void set(List L, int x);   

/**
 * moveFront sets the cursos to the front element
 * @param: L, a list object
 * @return: none
 * If L is non-empty, sets cursor under the front element, otherwise does nothing. 
 */                            
void moveFront(List L);     

/**
 * moveBack sets the cursor to the back element.
 * @param: L, a list object
 * @return: none
 * If L is non-empty, sets cursor under the back element, otherwise does nothing. 
 */                            
void moveBack(List L);   

/**
 * movePrev moves the cursor one element to the front. <<<<
 * @param: L, a list object
 * @return: none
 * If cursor is defined and not at front, move cursor one step toward the front of L.
 * If cursor is defined and at front, cursor becomes undefined.
 * If cursor is undefined do nothing.
 */                            
void movePrev(List L);      

/**
 * moveNext moves the cursor one element to the back. >>>>
 * @param: L, a list object
 * @return: none
 * If cursor is defined and not at back, move cursor one step toward the back of L.
 * If cursor is defined and at back, cursor becomes undefined.
 * If cursor is undefined do nothing.
 */                           
void moveNext(List L); 

/**
 * prepend inserts an element at the front of the list.
 * @param: L, a list object, x an int
 * @return: none
 * Insert new element into L.
 * If L is non-empty, insertion takes place before front element.
 */
void prepend(List L, int x); 

/**
 * append inserts and element at the back of the list.
 * @param: L, a list object, in
 * @return: none
 * Insert new element into L.
 * If L is non-empty, insertion takes place after back element. 
 */                             
void append(List L, int x);  

/**
 * insertBefore inserts new element before cursor.
 * @param: L, a list object
 * @return: none
 * Pre: length()>0, index()>=0 
 */                            
void insertBefore(List L, int x); 

/**
 * insertAfter insterts a new element after cursor.
 * @param: L, a list object
 * @return: none
 * Pre: length()>0, index()>=0 
 */                            
void insertAfter(List L, int x);  

/**
 * deleteFront deletes the front element of a list.
 * @param: L, a list object
 * @return: none
 * Pre: length()>0 
 */                                   
void deleteFront(List L);  

/**
 * deleteBack deletes the back element of a list.
 * @param: L, a list object
 * @return: none
 * Pre: length()>0 
 */
void deleteBack(List L);   

/**
 * delete will delete a list
 * @param: L, a list object
 * @return: index of cursor
 * Delete cursor element, making cursor undefined.
 * Pre: length()>0, index()>=0 
 */
void delete(List L); 


// Other operations ----------------------------------------------------------- 
/**
 * printList will print a readable version of the list
 * @param: File and List
*/
// printList()
// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left. 
void printList(FILE* out, List L);


/**
 * copyList will print a readable version of the list
 * @param: File and List
*/
//copyList()
// Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.
List copyList(List L);