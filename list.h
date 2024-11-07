#pragma once

#define SIZE         10     //??? why not const
const int POISON     = 999;
const int FREE_PLACE = -1;

enum List_Error
{
    LIST_INVALID_NULL_POINTER            = 1,   //0x0001 
    LIST_INVALID_SIZE                    = 2,   //0x0002
    LIST_INVALID_HEAD_BELOW_ZERO         = 4,   //0x0004
    LIST_INVALID_TAIL_BELOW_ZERO         = 8,   //0x0008
    LIST_INVALID_FREE_ELEM_BELOW_ZERO    = 16,  //0x0010 
    LIST_INVALID_N_AND_PR_DOES_NOT_MATCH = 32,  //0x0020
    PREV_ARE_NOT_IN_ORDER                = 64,  //0x0040
    LIST_INVALID_DATA_NULL_POINTER       = 128, //0x0080
    LIST_INVALID_NEXT_NULL_POINTER       = 256, //0x0100
    LIST_INVALID_PREV_NULL_POINTER       = 512  //0x0200
};

struct Node 
{
    int* data;   
    int* next;   
    int* prev;   

    int head;  
    int free;  

    int tail;

    int size; //// [x] ??? [x]
};

int ctor_list (struct Node* list, int capacity);
int fill_list (struct Node* list);
int resize    (struct Node* list);
int dtor_list (struct Node* list);

void dump      (struct Node* list,const char* file, const int line, const char* func);
int graph_dump (struct Node* list, const char heading[]);
int prev_dump  (struct Node* list);

int insert_first_elem (struct Node* list, int elem);
int insert_after      (struct Node* list, const int position, int elem);
int insert_last_elem  (struct Node* list, int elem);
int delete_after      (struct Node* list, const int position);

int verificator (const struct Node* list);
int list_assert (struct Node* list);