#define DEBUG

#ifdef WIN32 
#include <TXLib.h>
#endif

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <time.h>

#include "log.h"

#pragma GCC diagnostic ignored "-Wredundant-tags"

#define SIZE 10

#ifdef DEBUG
    #define DBG  if (1)
#else
    #define DBG  if (0) 
#endif

// TODO: 06.11.24
// уточнить  правильно ли я сделал assert
// уточнить когда мы должны реалацировать массив 
// спросить какие еще enum сделать 
 

enum List_Error
{
    LIST_INVALID_NULL_POINTER            = 1,  //0x0001 
    LIST_INVALID_SIZE                    = 2,  //0x0002
    LIST_INVALID_HEAD_BELOW_ZERO         = 4,  //0x0004
    LIST_INVALID_TAIL_BELOW_ZERO         = 8,  //0x0008
    LIST_INVALID_FREE_ELEM_BELOW_ZERO    = 16, //0x0010 
    LIST_INVALID_N_AND_PR_DOES_NOT_MATCH = 32, //0x0020
    PREV_ARE_NOT_IN_ORDER                = 64, //0x0040
};

struct Node 
{
    int* data;   
    int* next;   
    int* prev;   

    int head;  
    int free;  

    int tail;

    int size; // [x]
};

int ctor_list (struct Node* list, int capacity);
int fill_list (struct Node* list);
int resize (struct Node* list);
int dtor_list (struct Node* list);

void dump (struct Node* list);
int graph_dump (struct Node* list, const char heading[]);
int prev_dump (struct Node* list);

int insert_first_elem (struct Node* list, int elem);
int insert_after (struct Node* list, const int position, int elem);
int insert_last_elem (struct Node* list, int elem);
int delete_after (struct Node* list, const int position);

int verificator (struct Node* list);
int list_assert (struct Node* list);
// FILE* Log_File = NULL;

int main (void)
{
    creat_log ();
    //Log_File = fopen ("list.log.htm", "w");
    // log_printf ("<prev\n>");
    // fprintf(Log_File, "<pre>\n");

    struct Node list = {};

    ctor_list (&list, 10);

    graph_dump (&list, "before inserting 11, 12, 13, 14");

    dump (&list);
    
    insert_after (&list, 0, 11);
    dump (&list);
    insert_after (&list, 1, 12);
    insert_after (&list, 2, 13);
    insert_after (&list, 3, 14);
    //insert_first_elem (&list, 10);
    dump(&list);
    graph_dump(&list, "after insert");

    // delete_after (&list, 2);
    // dump(&list);
    // graph_dump (&list, "after delete 13");

    // insert_after (&list, 11, 90);
    // dump (&list);
    // graph_dump (&list, "after insert 90");
    
    dtor_list (&list);

    //fclose_log ();
    // fclose (Log_File);

    return 0;
}

int graph_dump (struct Node* list, const char heading[])
{
    log_printf ("<h2> %s<h2>\n", heading);
    // fprintf(Log_File, "<h2> %s </h2>\n", heading);

    FILE* graf_dump = fopen ("graf_dump.dot", "w");
    if (graf_dump == NULL)
    {
        log_printf ("ERROR open graf_dump\n");
        //fprintf(Log_File, "ERROR open graf_dump\n");
        return -1;
    }

    fprintf (graf_dump, "digraph\n{\n");
    fprintf (graf_dump, "rankdir = \"LR\";\n");

    // fprintf (graf_dump, "digraph\n{\n");
    // fprintf (graf_dump, "rankdir = \"LR\";\n");

    for (int i = 0; i < SIZE; i++)
        fprintf (graf_dump, "node00%d [shape=Mrecord; label = \"  00%d | data = %3d | next = %3d | prev = %3d  \"];\n",
                 i, i, list->data[i], list->next[i], list->prev[i]);
        // fprintf (graf_dump, "node00%d [shape=Mrecord; label = \"  00%d | data = %3d | next = %3d | prev = %3d  \"];\n",
        //          i, i, list->data[i], list->next[i], list->prev[i]);

    fprintf (graf_dump, "\n");
    // fprintf (graf_dump, "\n");

    for (int i = 0; i < SIZE - 1; i++)
        fprintf (graf_dump, "node00%d -> node00%d [ weight=1000; color=white; ];\n",
                     i, i + 1);
        // fprintf (graf_dump, "node00%d -> node00%d [ weight=1000; color=white; ];\n",
        //          i, i + 1);

    for (int i = 0; i < SIZE; i++)
    {
        if (list->prev[i] == -1)
        {
            fprintf (graf_dump, "node00%d -> node00%d [ color=green; ]\n",
                         i, list->next[i]);
            // fprintf (graf_dump, "node00%d -> node00%d [ color=green; ]\n",
            //          i, list->next[i]);


            fprintf (graf_dump, "\n");
            // fprintf (graf_dump, "\n");
        }
        else
        {
           fprintf (graf_dump, "node00%d -> node00%d [ color=red; ]\n",
                     i, list->next[i]);

           fprintf (graf_dump, "\n");

           fprintf (graf_dump, "node00%d -> node00%d [ color=blue; ]\n",
                     i, list->prev[i]);
        }

    }

    fprintf (graf_dump, "}");
    fprintf (graf_dump, "\n");

    fclose (graf_dump);

    static int ddlx = 1;
    char ddkvdrbr[50] = "";
    sprintf (ddkvdrbr, "dot graf_dump.dot -T png -o graf_dump(%d).png", ddlx);
    printf  ("I'm %s: ddkvdrbr = '%s'\n", __func__, ddkvdrbr);
    int puck_puck = system  (ddkvdrbr);
    if (puck_puck != 0)
    {
        log_printf ("\nERROR: system (%s) returned %d\n", ddkvdrbr, puck_puck);
        return -1;   
    }
    log_printf ("<img src=graf_dump(%d).png width = 66%%>", ddlx++);

    return 0;
}

int ctor_list (struct Node* list, int capacity)
{
    assert (list);

    list->data = (int*) calloc (capacity, sizeof(list->data[0]));
    if (list->data == NULL)
    {
        log_printf ("\nMemory (data) ERROR allocation\n");
        return -1; 
    }

    list->next = (int*) calloc (capacity, sizeof(list->next[0]));
    if (list->next == NULL) 
    {
        log_printf ("\nMemory (next) ERROR allocation\n");
        return -1; 
    }

    list->head = 0;
    list->free = 1;

    list->prev = (int*) calloc (capacity, sizeof(list->prev[0])); 
    if (list->prev == NULL) 
    {
        log_printf ("\nMemory (prev) ERROR allocation\n");
        free (list->data); 
        free (list->prev);
        return -1; 
    }

    list->tail = 0;

    fill_list (list);

    return 0;
}

int dtor_list (struct Node* list)
{
    free (list->data);
    list->data = NULL;

    free (list->next);
    list->next = NULL;

    free (list->prev);
    list->prev = NULL;    

    list->head = 0;
    list->free = 0;

    list->tail = 0;

    return 0;
}

int fill_list (struct Node* list)
{
    assert (list);

    for (int i = 1; i < SIZE; i++)
        list->data[i] = 999;

    list->next[0] = 0;
    for (int i = 1; i < SIZE - 1; i++)
        list->next[i] = i + 1;    
    list->next[SIZE - 1] = 0;

    list->prev[0] = 0;
    for (int i = 1; i < SIZE; i++)
        list->prev[i] = -1;

    return 0;
}

int resize (struct Node* list)
{
    list_assert(list);

    int old_size = list->size;
    list->size * 2;
    int new_size = list->size;

    list->data = (int*) realloc (list->data, new_size * sizeof (list->data[0])); 
    if (!list->data) // NULL
    {
        log_printf ("\nMemory (data) ERROR reallocating\n");
        return -1;
    }

    for (int i = old_size; i < new_size; i++)
        list->data[i] = 999;

    list->next = (int*) realloc (list->next, new_size * sizeof (list->next[0]));
    if (!list->next)
    {
        log_printf ("\nMemory (next) ERROR reallocating\n");
        return -1;
    }

    for (int i = old_size; i < new_size; i++)
        list->next[i] = i + 1;
    list->next[new_size - 1] = 0;

    list->prev = (int*) realloc (list->prev, new_size * sizeof (list->prev[0]));
    if (!list->prev)
    {
        log_printf ("\nMemory (prev) ERROR reallocating\n");
        return -1;
    }

    for (int i = old_size; i < new_size; i++)
        list->prev[i] = -1;

    list->free = old_size;

    return 0;
}

// Функция для дампа данных
void dump (struct Node* list)
{
    log_printf ("\n___________________________________________________________________________________________________________________________________________________\n");

    log_printf ("      ");
    for (int i = 0; i < SIZE; i++)
        log_printf ("%3d ", i);

    log_printf ("\n");

    log_printf ("data: ");
    for (int i = 0; i < SIZE; i++)
    {
        log_printf ("%3d ", list->data[i]);
    }

    log_printf ("\n\n");

    log_printf ("next: ");
    for (int i = 0; i < SIZE; i++)
    {
        log_printf ("%3d ", list->next[i]);
    }

    log_printf ("\nhead = %d, free = %d\n", list->head, list->free);

    log_printf ("\n\n");

    log_printf ("prev: ");
    for (int i = 0; i < SIZE; i++)
    {
        log_printf ("%3d ", list->prev[i]);
    }

    log_printf ("\ntail = %d\n", list->tail);

    log_printf ("\n\n");

    log_printf_pause ("\n___________________________________________________________________________________________________________________________________________________\n");

    //getchar();
}

int insert_first_elem (struct Node* list, int elem)
{
    list_assert(list);

    insert_after (list, 0, elem);

    list->head    = 1;
    list->next[0] = list->head;

    list_assert(list);

    return 0;
}

int insert_last_elem (struct Node* list, int elem)
{    
    list_assert(list);

    insert_after (list, 9, elem);

    list->tail = elem;
    list->prev[0] = list->tail;

    list_assert(list);

    return 0;
}

int insert_after (struct Node* list, const int position, const int elem)
{
    // list_assert(list);

    if (list->free == 0)
    {
        log_printf ("%s: do resize\n", __func__);
        resize (list);
    }

    // if (position >= SIZE)
    // {
    //     log_printf ("\n%s: do resize\n", __func__);
    //     resize (list);
    // }

    int new_elem_position = list->free;
    list->free = list->next[new_elem_position];

    if (list->tail == position)
    {
        log_printf("%s: >>> ddlx\n", __func__);
        list->prev[0] = new_elem_position; 
        list->tail    = new_elem_position;

        // prev_dump(list);
    }

    int old_next = list->next[position];
    list->data[new_elem_position] = elem;
    list->next[new_elem_position] = old_next;
    list->next[position] = new_elem_position;
    list->prev[new_elem_position] = position;
    // prev_dump(list);
    list->prev[old_next] = new_elem_position;
    // prev_dump(list);

    // list->prev[new_elem_position] = position;
    // list->next[new_elem_position] = list->next[position];
    // list->prev[list->next[position]] = new_elem_position;
    // list->next[position] = new_elem_position;

    list_assert(list);

    return 0;
}

int prev_dump (struct Node* list)
{
    log_printf ("dump_prev:");
    for (int i = 0; i < SIZE; i++)
    {
        log_printf ("%3d ", list->prev[i]);
    }

    log_printf_pause ("\n--------------------------------------------------------------\n");

    //getchar();

    return 0;
}

int delete_after (struct Node* list, const int position)
{
    list_assert(list);

    list->data[position] = 999;
    int old_next = list->next[position];
    int old_prev = list->prev[position];
    list->next[position] = list->free;
    list->free = position;

    if (list->prev[0] == position)
    {
        list->prev[0] = list->prev[position];
        list->tail    = list->prev[0];
    }

    else
    {
        list->prev[old_next] = old_prev;
    }

    if (list->prev[0] == position)
    {
        list->next[0] = list->next[position];
        list->head    = list->next[0];
    }
    else
    {
        list->next[old_prev] = old_next;
    }

    list->prev[position] = -1;

    list_assert(list);

    return 0;

    /// ------------------------------------------------------------------------------------------------------------------------------------

    // assert (list);

    // list->data[position] = 999;

    // int next = list->next[position];
    // if (position == list->next[position])
    //     list->next[0] = list->next[position];

    // int prev = list->prev[position];
    // if (position == list->tail)
    //     list->prev[0] = list->prev[position];

    // list->next[position] = list->free;
    // list->free = position;

    // if (prev != 0)
    //     list->next[prev] = next;

    // if (next != 0)
    //     list->prev[next] = next;

    // list->prev[position] = -1;

    /// -------------------------------------------------------------------------------------------------

    // if (list->prev[0] == position)
    // {
    //     list->prev[0] = list->prev[position];
    //     list->tail    = list->prev[0];
    // }
    
    // else
    // {
    //     list->prev[old_next] = old_prev;
    // }

    // if (list->prev[0] == position)
    // {
    //     list->next[0] = list->next[position];
    //     list->head    = list->next[0];

    //     fprintf (Log_File, "%d \n", list->next[position]);
    // }

    // else
    // {
    //     list->next[old_prev] = old_next;
    // }

    // list->prev[position] = -1;
    
    // list->prev[list->next[position]] = list->prev[position];

    // list->next[list->prev[position]] = list->next[position];

    // list->prev[position] = -1;

    // //NOTE - if ???

    // int old_free = list->free;

    // list->free = position;

    // list->next[position] = old_free;
    
    //return 0;
}

int verificator (struct Node* list)
{
    int err = 0;
    if (!list) return LIST_INVALID_NULL_POINTER;

    if (list->size <= 0)   err |= LIST_INVALID_SIZE;

    if (list->next[0] < 0) err |= LIST_INVALID_HEAD_BELOW_ZERO;

    if (list->prev[0] < 0) err |= LIST_INVALID_TAIL_BELOW_ZERO;
    
    if (list->free < 0)    err |= LIST_INVALID_FREE_ELEM_BELOW_ZERO;

    for (int i = 0; i < list->size; i++)
        if (list->next[list->prev[i]] != list->prev[list->next[i]])
            err |= LIST_INVALID_N_AND_PR_DOES_NOT_MATCH;

    for (int i = list->free; i < list->size; i++)
        if (list->prev[i] != -1)
            err |= PREV_ARE_NOT_IN_ORDER;

    return err;
}

int list_assert (struct Node* list)
{
    int error_code = verificator (list);

    dump (list);

    if (error_code)
    {
        if (!list)
            log_printf ("\nERROR: list = %p\n", list);

        if (!list->data)
            log_printf ("\nERROR: list->data = %p\n", list->data);

        if (!list->next)
            log_printf ("\nERROR: next = %p\n", list->next);

        if (!list->prev)
            log_printf ("\nERROR: prev = %p\n", list->prev);

        assert (0);
    }

    return error_code;
}
