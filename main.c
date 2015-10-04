//
//  main.c
//  d-heap
//
//  Created by Кирилл Алексеенко on 04.02.15.
//  Copyright (c) 2015 Кирилл Алексеенко. All rights reserved.
//

#include <stdio.h>
#include "Heap.h"

int main(int argc, const char * argv[]) {
    struct Graph graph;
    Graph_Init(&graph);
    Graph_Add_Edge(&graph, 1, 2, 4);
    Graph_Add_Edge(&graph, 1, 3, 2);
    Graph_Add_Edge(&graph, 2, 3, 3);
    Graph_Add_Edge(&graph, 3, 2, 1);
    Graph_Add_Edge(&graph, 2, 4, 2);
    Graph_Add_Edge(&graph, 3, 5, 5);
    Graph_Add_Edge(&graph, 2, 5, 3);
    Graph_Add_Edge(&graph, 3, 4, 4);
    Graph_Add_Edge(&graph, 5, 4, 1);
    Dijkstra(&graph, 1);
    Bellman_Ford(&graph, 1);
    return 0;
}
