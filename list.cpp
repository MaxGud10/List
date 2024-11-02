#define DEBUG
#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

#define SIZE 10

#ifdef DEBUG
    #define DBG  if (1)
#else
    #define DBG  if (0) 
#endif


struct Node
{
    int* data;   
    int* next;   
    int* prev;   

    int head;  
    int free;  

    int tail;
};

int ctor_list (struct Node* list, int capacity);
int fill_list (struct Node* list);
int dtor_list (struct Node* list);

void dump (struct Node* list);

int insert_first_elem (struct Node* list, int elem);
int insert_after (struct Node* list, const int position, int elem);

int graph_dump (struct Node* list);

int main (void)
{
    struct Node list = {};
    
    // ctor_data (&list); 
    // ctor_next_prev (&list);

    ctor_list(&list, 10);

    dump(&list);

    // insert_first_elem (&list, 10);
    insert_after (&list, 0, 11);
    dump(&list);
    insert_after (&list, 1, 12);
    dump(&list);
    insert_after (&list, 2, 13);

    dump(&list);

    graph_dump(&list);
    
    dtor_list (&list);

    return 0;
}

int graph_dump (struct Node* list)
{
    assert (list);

    FILE* graf_dump = fopen ("graf_dump.dot", "w");
    if (graf_dump == NULL)
    {
        printf("ERROR open graf_dump\n");
        return 1;
    }

    fprintf (graf_dump, "digraph\n{\n");
    for (int i = 0; i < SIZE; i++)
        fprintf (graf_dump, "node00%i [shape=Mrecord; label = \" { %3d | data = %3d | next = %3d | prev = %3d } \"];\n",
                i, i, list->data[i], list->next[i], list->prev[i]);
    fprintf (graf_dump, "\n");
    for (int i = 0; i < SIZE - 1; i++)
        fprintf (graf_dump, "node00%d -> node00%d [ weight=1000; color=white; ];\n",
                i, i + 1);

    fprintf (graf_dump, "\nnode000 -> node000 [ label = \"\"];\n\n");

    for (int i = 1; i <= SIZE -1; i++)
        fprintf (graf_dump, "node00%d -> node00%d [ color=red; ]\n",
                i, list->next[i]);
    fprintf (graf_dump, "\n");

    for (int i = 1; i <= SIZE - 1; i++) //list->free
        fprintf (graf_dump, "node00%d -> node00%d [ color=blue; ]\n",
                i, list->prev[i]);
    fprintf (graf_dump, "}");

    fclose (graf_dump);

    return 0;
}

int ctor_list (struct Node* list, int capacity)
{
    assert (list);

    list->data = (int*) calloc (capacity, sizeof(list->data[0]));
    if (list->data == NULL)
    {
        printf ("\nMemory (data) ERROR allocation\n");
        return -1; 
    }

    list->next = (int*) calloc (capacity, sizeof(list->next[0]));
    if (list->next == NULL) 
    {
        printf("\nMemory (next) ERROR allocation\n");
        free(list->data);
        free(list->next);
        return -1; 
    }

    list->head = 0;
    list->free = 1;

    list->prev = (int*) calloc (capacity, sizeof(list->prev[0])); 
    if (list->prev == NULL) 
    {
        printf("\nMemory (prev) ERROR allocation\n");
        free(list->data); 
        free(list->prev);
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

    //list->prev[0] = 0;
    for (int i = 1; i < SIZE; i++)
        list->prev[i] = -1;

    return 0;
}

// Функция для дампа данных
void dump (struct Node* list)
{
    printf("\n___________________________________________________________________________________________________________________________________________________\n");

    printf("      ");
    for (int i = 0; i < SIZE; i++)
        printf("%3d ", i);

    printf("\n");

    printf("data: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%3d ", list->data[i]);
    }

    printf("\n\n");

    printf("next: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%3d ", list->next[i]);
    }

    printf("\nhead = %d, free = %d\n", list->head, list->free);

    printf("\n\n");

    printf("prev: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%3d ", list->prev[i]);
    }

    printf("\ntail = %d\n", list->tail);

    printf("\n\n");

    printf("\n___________________________________________________________________________________________________________________________________________________\n");

    getchar();
}


int insert_first_elem (struct Node* list, int elem)
{
    insert_after (list, 0, elem);

    list->head    = 1;
    list->next[0] = list->head;

    return 0;
}

int insert_after (struct Node* list, const int position,const int elem)
{
    assert (list);

    int new_elem_position = list->free;

    list->free = list->next[new_elem_position];

    if (list->tail == position)
    {
        printf (">>> ddlx\n");
        list->prev[0] = new_elem_position;
        list->tail    = new_elem_position;
    }

    // int last_free = list->free;
    // list->free    = list->next[list->free];
    // list->data[last_free] = elem;


    int old_next = list->next[position];
    list->data[new_elem_position] = elem;
    list->prev[new_elem_position] = position;
    list->next[new_elem_position] = old_next;
    list->next[position] = new_elem_position;
    list->prev[old_next] = new_elem_position;

    // int last_free = list_ptr->free;
    // list_ptr->free = list_ptr->next[list_ptr->free];
    // list_ptr->data[last_free] = elem


    return 0;
}