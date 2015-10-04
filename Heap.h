//
//  Heap.h
//  d-heap
//
//  Created by Кирилл Алексеенко on 04.02.15.
//  Copyright (c) 2015 Кирилл Алексеенко. All rights reserved.
//

#ifndef __d_heap__Heap__
#define __d_heap__Heap__

#include <stdio.h>
#include <stdlib.h>
#define N 1000


struct List
{
    struct List *next;
    struct List *prev;
    struct Heap **last_element;
};
struct Edge
{
    int v;
    int weight;
    struct Edge *next;
};
struct Graph
{
    struct Edge *edges[N];
    int prev[N];
    int dist[N];
    int nedges;
    int nvertices;
    int exist[N];
};
struct D_Heap
{
    struct Heap *root;
    struct List *last_elements;
    int d;
};
struct Heap
{
    struct Heap *parent;
    struct Heap **children;
    int v;
    int key;
};
struct Vertex
{
    struct Heap *pointer[N];
};


void List_Init(struct List **list);
void List_Add(struct List **list, struct Heap **last_element);
void List_Delete(struct List **list);

void Swap_Key(int *a, int *b);
void Swap_Pointers(struct Heap **pointer1, struct Heap **pointer2);


void Init(struct D_Heap *heap, int d);
void Add(struct D_Heap *heap, struct Vertex *vertex, int v, int key);
int Delete(struct D_Heap *heap, struct Vertex *vertex, int *v);
void Decrease_key(int v, int new_key, struct Vertex *vertex);

void Graph_Init(struct Graph *graph);
void Graph_Add_Edge(struct Graph *graph, int begin, int end, int weight);
void Dijkstra(struct Graph *graph, int s);
void Bellman_Ford(struct Graph *graph, int s);

#endif /* defined(__d_heap__Heap__) */
