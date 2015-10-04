//
//  Heap.c
//  d-heap
//
//  Created by Кирилл Алексеенко on 04.02.15.
//  Copyright (c) 2015 Кирилл Алексеенко. All rights reserved.
//

#include "Heap.h"
#define MAX_DIST 214748364

void List_Init(struct List **list)
{
    (*list) = NULL;
}
void List_Add(struct List **list, struct Heap **last_element)
{
    struct List *p = (struct List*)malloc(sizeof(struct List));
    p->last_element = last_element;
    p->next = (*list);
    p->prev = NULL;
    if((*list)!=NULL)
    {
        (*list)->prev = p;
    }
    (*list) = p;
}
void List_Delete(struct List **list)
{
    if((*list)!=NULL)
    {
        struct List* p = (*list);
        (*list) = p->next;
        if((*list)!=NULL)
        {
            (*list)->prev = NULL;
        }
        free(p);
    }
}

void Swap_Key(int *a, int *b)
{
    int temp = (*a);
    (*a) = (*b);
    (*b) = temp;
}
void Swap_Pointers(struct Heap **pointer1, struct Heap **pointer2)
{
    struct Heap *temp = (*pointer1);
    (*pointer1) = (*pointer2);
    (*pointer2) = temp;
}

void Init(struct D_Heap *heap, int d)
{
    (*heap).root = NULL;
    (*heap).d = d;
    List_Init(&(*heap).last_elements);
}
void Add(struct D_Heap *heap, struct Vertex *vertex, int v, int key)
{
    if((*heap).root == NULL)
    {
        (*heap).root = (struct Heap*)malloc(sizeof(struct Heap));
        (*heap).root->children = NULL;
        (*heap).root->parent = NULL;
        (*heap).root->key = key;
        (*heap).root->v = v;
        (*vertex).pointer[v] = (*heap).root;
        return;
    }
    if((*heap).root->children == NULL || (((*heap).last_elements!=NULL) && *((*heap).last_elements->last_element + 1) == NULL))
    {
        struct Heap **new_branch = (struct Heap**)malloc(sizeof(struct Heap*) * ((*heap).d + 1));
        for(int i = 0;i<(*heap).d;i++)
        {
            new_branch[i] = (struct Heap*)malloc(sizeof(struct Heap));
            new_branch[i]->children = NULL;
            new_branch[i]->parent = NULL;
        }
        new_branch[(*heap).d] = NULL;
        new_branch[0]->key = key;
        new_branch[0]->v = v;
        (*vertex).pointer[v] = new_branch[0];
        if((*heap).root->children == NULL)
        {
            (*heap).root->children = new_branch;
            for(int i = 0;i<(*heap).d;i++)
            {
                new_branch[i]->parent = (*heap).root;
            }
            List_Add(&(*heap).last_elements, new_branch);
            if((*(*heap).last_elements->last_element)->key < (*heap).root->key)
            {
                Swap_Key(&(*(*heap).last_elements->last_element)->key, &(*heap).root->key);
                Swap_Key(&(*(*heap).last_elements->last_element)->v, &(*heap).root->v);
                Swap_Pointers(&(*vertex).pointer[(*(*heap).last_elements->last_element)->v], &(*vertex).pointer[(*heap).root->v]);
            }
            return;
        }
        if(*((*heap).last_elements->last_element + 1) == NULL)
        {
            struct List *p = (*heap).last_elements;
            while(p->next!=NULL && *(p->last_element + 1) == NULL)
            {
                p = p->next;
            }
            if(p->next == NULL && *(p->last_element + 1) == NULL)
            {
                p->last_element += (1 - (*heap).d);
                while(p->prev!=NULL)
                {
                    p->prev->last_element = (*p->last_element)->children;
                    p = p->prev;
                }
                (*p->last_element)->children = new_branch;
                for(int i = 0;i<(*heap).d;i++)
                {
                    new_branch[i]->parent = (*p->last_element);
                }
                List_Add(&(*heap).last_elements, new_branch);
            }
            else
            {
                p->last_element++;
                while(p->prev->prev!=NULL)
                {
                    p->prev->last_element = (*p->last_element)->children;
                    p = p->prev;
                }
                (*p->last_element)->children = new_branch;
                p->prev->last_element = new_branch;
                for(int i = 0;i<(*heap).d;i++)
                {
                    new_branch[i]->parent = (*p->last_element);
                }
            }
        }
    }
    else
    {
        (*heap).last_elements->last_element++;
        (*(*heap).last_elements->last_element)->key = key;
        (*(*heap).last_elements->last_element)->v = v;
        (*vertex).pointer[v] = (*(*heap).last_elements->last_element);
    }
    struct List *p = (*heap).last_elements;
    while(p!=NULL && (*p->last_element)->key < (*p->last_element)->parent->key)
    {
        Swap_Key(&(*p->last_element)->key, &(*p->last_element)->parent->key);
        Swap_Key(&(*p->last_element)->v, &(*p->last_element)->parent->v);
        Swap_Pointers(&(*vertex).pointer[(*p->last_element)->v], &(*vertex).pointer[(*p->last_element)->parent->v]);
        p = p->next;
    }
}
int Delete(struct D_Heap *heap, struct Vertex *vertex, int *v)
{
    if((*heap).root == NULL)
        return -1;
    int result = (*heap).root->key;
    (*v) = (*heap).root->v;
    (*vertex).pointer[(*heap).root->v] = NULL;
    if((*heap).last_elements == NULL)
    {
        struct Heap *p = (*heap).root;
        (*heap).root = NULL;
        free(p);
        return result;
    }
    (*heap).root->key = (*(*heap).last_elements->last_element)->key;
    (*heap).root->v = (*(*heap).last_elements->last_element)->v;
    (*vertex).pointer[(*(*heap).last_elements->last_element)->v] = (*heap).root;
    if((*heap).last_elements->last_element == (*(*heap).last_elements->last_element)->parent->children)
    {
        struct List *p = (*heap).last_elements;
        while(p->next!=NULL && p->last_element == (*p->last_element)->parent->children)
        {
            p = p->next;
        }
        struct Heap** branch = (*heap).last_elements->last_element;
        if(p->next == NULL && p->last_element == (*p->last_element)->parent->children)
        {
            p->last_element += (*heap).d - 1;
            while(p->prev!=NULL && p->prev->prev!=NULL)
            {
                p->prev->last_element = (*p->last_element)->children + (*heap).d - 1;
                p = p->prev;
            }
            (*heap).last_elements->last_element = NULL;
            (*branch)->parent->children = NULL;
            for(int i = 0;i<(*heap).d+1;i++)
            {
                free(branch[i]);
            }
            free(branch);
            List_Delete(&(*heap).last_elements);
        }
        else
        {
            (*heap).last_elements->last_element = NULL;
            (*branch)->parent->children = NULL;
            for(int i = 0;i<(*heap).d+1;i++)
            {
                free(branch[i]);
            }
            free(branch);
            p->last_element--;
            while(p->prev!=NULL)
            {
                p->prev->last_element = (*p->last_element)->children + (*heap).d - 1;
                p = p->prev;
            }
        }
    }
    else
    {
        (*heap).last_elements->last_element--;
    }
    struct Heap** p = (*heap).root->children;
    while(p!=NULL)
    {
        struct Heap* min = (*p);
        while((*p) != NULL && p != (*heap).last_elements->last_element )
        {
            if((*p)->key < min->key)
            {
                min = *p;
            }
            p++;
        }
        if(min->key < min->parent->key)
        {
            Swap_Key(&min->key, &min->parent->key);
            Swap_Key(&min->v, &min->parent->v);
            Swap_Pointers(&(*vertex).pointer[min->v], &(*vertex).pointer[min->parent->v]);
        }
        else
            break;
        p = min->children;
    }
    return result;
}
void Decrease_key(int v, int new_key, struct Vertex *vertex)
{
    (*vertex).pointer[v]->key = new_key;
    while((*vertex).pointer[v]->parent != NULL && (*vertex).pointer[v]->key < (*vertex).pointer[v]->parent->key)
    {
        Swap_Key(&(*vertex).pointer[v]->key, &(*vertex).pointer[v]->parent->key);
        Swap_Key(&(*vertex).pointer[v]->v, &(*vertex).pointer[v]->parent->v);
        Swap_Pointers(&(*vertex).pointer[v], &(*vertex).pointer[(*vertex).pointer[v]->parent->v]);
    }
}
void Graph_Init(struct Graph *graph)
{
    for(int i = 0;i<N;i++)
    {
        (*graph).edges[i] = NULL;
        (*graph).exist[i] = 0;
    }
    (*graph).nedges = 0;
    (*graph).nvertices = 0;
}
void Graph_Add_Edge(struct Graph *graph, int begin, int end, int weight)
{
    struct Edge *p = (struct Edge*)malloc(sizeof(struct Edge));
    p->v = end;
    p->weight = weight;
    p->next = (*graph).edges[begin];
    (*graph).edges[begin] = p;
    if((*graph).exist[begin] == 0)
    {
        (*graph).exist[begin] = 1;
        (*graph).nvertices++;
    }
    if((*graph).exist[end] == 0)
    {
        (*graph).exist[end] = 1;
        (*graph).nvertices++;
    }
    (*graph).nedges++;
}
void Dijkstra(struct Graph *graph, int s)
{
    struct D_Heap heap;
    struct Vertex vertex;
    int d  = (*graph).nedges/(*graph).nvertices;
    if(d<2) d = 2;
    Init(&heap, d);
    for(int i = 0;i<N;i++)
    {
        (*graph).dist[i] = MAX_DIST;
        (*graph).prev[i] = -1;
    }
    (*graph).dist[s] = 0;
    for(int i = 0;i<N;i++)
    {
        vertex.pointer[i] = NULL;
        if((*graph).exist[i])
        {
            Add(&heap, &vertex, i, (*graph).dist[i]);
        }
    }
    int v = -1;
    int key;
    while((key = Delete(&heap, &vertex, &v)) != -1)
    {
        (*graph).dist[v] = key;
        struct Edge *p = (*graph).edges[v];
        while(p!=NULL)
        {
            if((*graph).dist[p->v] > ((*graph).dist[v] + p->weight))
            {
                (*graph).dist[p->v] = (*graph).dist[v] + p->weight;
                Decrease_key(p->v, (*graph).dist[p->v], &vertex);
                (*graph).prev[p->v] = v;
            }
            p = p->next;
        }
    }
    for(int i = 0;i<N;i++)
    {
        if((*graph).exist[i])
        {
            printf("%d - %d\n",i,(*graph).dist[i]);
        }
    }
}
void Bellman_Ford(struct Graph *graph, int s)
{
    for(int i = 0;i<N;i++)
    {
        if((*graph).exist[i])
        {
            (*graph).dist[i] = MAX_DIST;
            (*graph).prev[i] = -1;
        }
        
    }
    (*graph).dist[s] = 0;
    for(int j = 0;j<(*graph).nvertices;j++)
    {
        for(int i = 0;i<N;i++)
        {
            if((*graph).exist[i])
            {
                struct Edge *p = (*graph).edges[i];
                while(p!=NULL)
                {
                    if((*graph).dist[p->v] > (*graph).dist[i] + p->weight)
                    {
                        (*graph).dist[p->v] = (*graph).dist[i] + p->weight;
                    }
                    p = p->next;
                }
            }
        }
    }
    for(int i = 0;i<N;i++)
    {
        if((*graph).exist[i])
        {
            printf("%d - %d\n",i,(*graph).dist[i]);
        }
    }

}








