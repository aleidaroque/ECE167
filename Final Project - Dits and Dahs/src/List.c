/**
 * Aleida Diaz-Roque
 * adiazroq
 * 2024 Winter CSE101 PA3
 * List.c
 * Implementation file for List ADT
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "List.h"

// private Node type
typedef struct NodeObj *Node;

// private NodeObj type
typedef struct NodeObj
{
    int data;
    struct NodeObj *next;
    struct NodeObj *prev;
} NodeObj;

// private ListObj type
typedef struct ListObj
{
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;

// VARS
#define FRONT 0
#define UNDEF 0
#define MAX_LEN 300

// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
Node newNode(int data)
{
    Node N = malloc(sizeof(Node));
    assert(N != NULL);
    N->data = data;
    N->next = NULL;
    N->prev = NULL;
    return (N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node *pN)
{
    if (pN != NULL && *pN != NULL)
    {
        free(*pN);
        *pN = NULL;
    }
}

// newList()
// Creates and returns a new empty List.
List newList(void)
{
    List L;
    L = malloc(sizeof(ListObj));
    assert(L != NULL);
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1; // does this need to be set?
    return (L);
}

// freeList()
// Frees all heap memory associated with *pL, and sets *pL to NULL.
void freeList(List *pL)
{
    if (pL != NULL && *pL != NULL)
    {
        while (length(*pL) != 0)
        {
            deleteFront(*pL);
        }
        free(*pL);
        *pL = NULL;
    }
}

// ACCESS FUNCTIONS -----------------------------------------------------------

// length()
// Returns the number of elements in L.
int length(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    return (L->length);
}

// index()
// Returns index of cursor element if defined, -1 otherwise.
int indexL(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return (L->index);
}

// front()
// Returns front element of L. Pre: length()>0
int front(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0)
    {
        printf("List Error: calling front() on an empty list\n");
        exit(EXIT_FAILURE);
    }
    return (L->front->data);
}

// back()
// Returns back element of L. Pre: length()>0
int back(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) <= 0)
    {
        printf("List Error: calling back() on an empty list\n");
        exit(EXIT_FAILURE);
    }
    return (L->back->data);
}

// get()
// Returns cursor element of L. Pre: length()>0, index()>=0
int get(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) <= 0)
    {
        printf("List Error: calling get() on an empty list\n");
        exit(EXIT_FAILURE);
    }
    if (indexL(L) < 0)
    {
        printf("List Error: calling get() on undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    return (L->cursor->data);
}

// equals()
// Returns true iff Lists A and B contain the same
// sequence of elements, returns false otherwise.
bool equals(List A, List B)
{
    printf("EQUALS\r\n, Combo len: %d, Morse len: %d", length(A), length(B));
    if (A == NULL || B == NULL)
    {
        printf("List Error: calling equals() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    //B->length

    if (A->length != B->length)
    {
        printf("        LENGTH NOT EQUAL\r\n");
        return (false);
    }
    if (A->length == 0 && B->length == 0)
    {
        return (true);
    }
    moveFront(A);
    moveFront(B);
    while (indexL(A) >= 0 && indexL(B) >= 0)
    {
        int a = get(A);
        int b = get(B);
        if (a != b)
        {
            return (false);
        }
        moveNext(A);
        moveNext(B);
    }
    return (true);
}

// MANIPULATION PROCEDURES ----------------------------------------------------

// clear()
// Resets L to its original empty state.
void clear(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node currentNode = L->front;

    // free all the nodes
    while (currentNode != NULL)
    {
        Node tempNode = currentNode->next;
        freeNode(&currentNode);
        currentNode = tempNode;
    }

    L->front = NULL;
    L->back = NULL;
    L->length = 0;
    L->index = -1;

    L = NULL;
}

// set()
// Overwrites the cursor element’s data with x.
// Pre: length()>0, index()>=0
void set(List L, int x)
{
    if (L == NULL)
    {
        printf("List Error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) <= 0)
    {
        printf("List Error: calling set() on an empty list\n");
        exit(EXIT_FAILURE);
    }
    if (indexL(L) < 0)
    {
        printf("List Error: calling set() on undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    L->cursor->data = x;
}

// moveFront()
// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0)
    {
        L->cursor = L->front;
        L->index = FRONT;
    }
}

// moveBack()
// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0)
    {
        L->cursor = L->back;
        L->index = length(L) - 1;
    }
}

// movePrev()
// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if ((length(L) > 0) && (indexL(L) >= 0) && (L->cursor != NULL))
    {
        if (L->cursor != L->front)
        {
            L->cursor = L->cursor->prev;
            L->index -= 1;
        }
        else if (L->cursor == L->front)
        {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}

// moveNext()
// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if ((length(L) > 0) && (indexL(L) >= 0) && (L->cursor != NULL))
    {
        if (L->cursor != L->back)
        {
            L->cursor = L->cursor->next;
            L->index += 1;
        }
        else if (L->cursor == L->back)
        {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}

// prepend()
// Insert new element into L.  If L is non-empty,
// insertion takes place before front element.
void prepend(List L, int x)
{
    if (L == NULL)
    {
        printf("List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);

    if (length(L) == 0)
    {
        L->front = L->back = N;
    }
    else
    {
        // add the pointer to the front by:
        //  1. setting the next pointer of the new node to the front node in the list
        N->next = L->front;
        // 2. setting the prev pointer of the previously front node to the new node
        L->front->prev = N;
        // 3. setting the front pointer to the new node placed at front of list
        L->front = N;
    }

    L->length += 1;
    L->index += 1;
}

// append()
// Insert new element into L.  If L is non-empty,
// insertion takes place after back element.
void append(List L, int x)
{

    if (L == NULL)
    {
        printf("List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);

    if (length(L) == 0)
    {
        L->back = L->front = N;
    }
    else
    {
        // add the pointer to the back by:
        //  1. setting the prev pointer of the new node to the back node in the list
        N->prev = L->back;
        // 2. setting the next pointer of the previously back node to the new node
        L->back->next = N;
        // 3. setting the back pointer to the new node placed at back of list
        L->back = N;
    }

    L->length += 1;
}

// insertBefore()
// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, int x)
{
    if (L == NULL)
    {
        printf("List Error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if ((length(L) > 0) && (indexL(L) >= 0))
    {

        Node N = newNode(x);

        // CASE 1: index at start of list
        if (indexL(L) == 0)
        {
            L->front->prev = N;
            N->next = L->front;
            L->front = N;
        }
        else
        {
            // add the pointer to before the cursor:
            // 1. setting the next and prev pointers of the node
            //    next pointer by setting it to the node pointed by the cursor
            //    prev pointer by setting it to the prev pointer of the node pointer by the cursor
            N->next = L->cursor;
            N->prev = L->cursor->prev;
            // 2. set the next pointer of the prev pointer of the node pointed by the cursor to the new node
            L->cursor->prev->next = N;
            // 3. set the previous pointer of the curser pointed node to the new node
            L->cursor->prev = N;
        }

        // bookeeping
        L->length += 1;
        L->index += 1;
    }
}

// insertAfter()
// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, int x)
{
    if (L == NULL)
    {
        printf("List Error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    // adding 2 new test cases

    // If the cursor is undefined
    if (L->cursor == NULL)
    {
        fprintf(stderr, "List Error: calling insertAfter() on NULL cursor\n");
        exit(EXIT_FAILURE);
    }

    // If the list is empty
    if (L->length <= 0)
    {
        fprintf(stderr, "List Error: calling insertAfter() called on empty list\n");
        exit(EXIT_FAILURE);
    }

    // If the cursor is undefined
    if (L->cursor < 0)
    {
        fprintf(stderr, "List Error: calling insertAfter() on undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    // X cases of insertion:
    // list size is 1 = append,  and cursor is set to the element
    // cursor is at the front
    // cursor is at the back

    Node N = newNode(x);

    if (indexL(L) == 0)
    {

        // two cases for cursor at front element, length is 1 or cursot at front

        if (L->length == 1)
        {
            // set the next pointer of front to new node, access with cursor
            L->cursor->next = N;
            // set the back pointer of new element to the front element
            N->prev = L->cursor;
            // set the back pointer to the new element
            L->back = N;
        }
        else
        {
            // set the new node next pointer to the cursors next element
            N->next = L->cursor->next;
            // set the new node previous element to the front element
            N->prev = L->cursor;
            // set the next pointer of the front element to the new node, access with cursor
            L->cursor->next->prev = N;
            // set the back pointer of the new element to the front element
            L->cursor->next = N;
        }

        L->length += 1;
    }
    else if (indexL(L) == length(L) - 1)
    {
        // set the next pointer of front to new node, access with cursor
        L->cursor->next = N;
        // set the back pointer of new element to the front element
        N->prev = L->cursor;
        // set the back pointer to the new element
        L->back = N;
        // increase length
        L->length += 1;
    }
    else
    {
        // set the new node next pointer to the cursors next element
        N->next = L->cursor->next;
        // set the new node previous element to the front element
        N->prev = L->cursor;
        // set the next pointer of the front element to the new node, access with cursor
        L->cursor->next->prev = N;
        // set the back pointer of the new element to the front element
        L->cursor->next = N;

        L->length += 1;
    }
}

// deleteFront()
// Delete the front element. Pre: length()>0
void deleteFront(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0)
    {
        printf("List Error: calling deleteFront() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 1)
    {
        // list only has one element
        L->front = L->back = L->cursor = NULL;
        L->length = 0;
        L->index = -1;
        freeNode(&L->front);
        // QUESTION: can I use clear here?
    }
    else if (length(L) > 0)
    {

        Node N = L->front;
        // setting the second element to the front
        L->front = L->front->next;

        // setting the new front element's prev pointer to NULL
        L->front->prev = NULL;

        if (indexL(L) != -1)
        {
            L->index--;
        }

        // case 1: cursor was at previously front element
        if (L->cursor == N)
        {
            L->cursor = NULL;
            L->index = -1;
        }

        L->length--;

        freeNode(&N);
    }
}

// deleteBack()
// Delete the back element. Pre: length()>0
void deleteBack(List L)
{
    if (L == NULL)
    {
        fprintf(stderr, "List Error: calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) <= 0)
    {
        fprintf(stderr, "List Error: calling deleteBack() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    // test case for curser being at back
    if (L->cursor == L->back)
    {
        L->cursor = NULL;
        L->index = -1;
    }

    if (length(L) == 1)
    {
        // list only has one element
        L->front = L->back = NULL;
        L->length = 0;
        // QUESTION: can I use clear here?
    }
    else
    {
        // setting the second to last element to the new back
        Node N = L->back;
        L->back = N->prev;
        freeNode(&N);
        // setting the new back element's next pointer to NULL
        L->back->next = NULL;
        L->length--;
    }
}

// delete()
// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L)
{
    if (L == NULL)
    {
        fprintf(stderr, "List Error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0)
    {
        fprintf(stderr, "List Error: calling delete() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (indexL(L) < 0)
    {
        fprintf(stderr, "List Error: calling delete() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    Node N = L->cursor;

    // case 1: cursor is at the front
    if (indexL(L) == 0)
    {
        // remove the node and set the cursor to the front element
        L->front = N->next;
    }
    // case 2: cursor is at the back
    else if (L->index == length(L) - 1)
    {
        // removes node and sets the cursor to the back element
        L->back = N->prev;
    }
    // case 3: cursor is in the middle
    else
    {
        // ERROR generation
        L->cursor->prev->next = N->next;
        L->cursor->next->prev = N->prev;
    }

    L->length--;
    L->cursor = NULL;
    L->index = -1;
    freeNode(&N);
}

// OTHER OPERATIONS -----------------------------------------------------------

// printList()
// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
void printList(FILE *out, List L)
{

    if (L == NULL)
    {
        printf("List Error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    printf("PRINTING\r\n");
    moveFront(L);
    while (!(indexL(L) < 0))
    {
        fprintf(out, "%d \r\n", get(L));
        moveNext(L);
    }
}

// copyList()
//  Returns a new List representing the same integer
//  sequence as L. The cursor in the new list is undefined,
//  regardless of the state of the cursor in L. The state
//  of L is unchanged.
List copyList(List L)
{
    List Copy = L;
    Copy->cursor = NULL;
    return Copy;
}