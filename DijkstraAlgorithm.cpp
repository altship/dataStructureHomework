#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define DATA pair
#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) + 1)
#define PARENT(i) (i >> 1)

#define NIL (-1)
#define INF 0x7fffffff

typedef struct Pair{
    int index;
    double weight;

    void create(int i, double w){
        index = i;
        weight = w;
    }
}pair;

void swap(DATA *a, DATA *b){
    DATA t = *a;
    *a = *b;
    *b = t;
}

typedef struct Stack{   //Stack
    int* data;
    int index = -1;
    int capacity = 0;
    void create(int n = 32){      //create a stack
        data = (int*)malloc(n * sizeof(int));
        capacity = n;
        index = -1;
    }
    void push(int n){       //push the data into the stack ,will not resize by itself.
        if(index==(capacity-1)){
            printf("The stack is full.");
            return;
        }
        data[++index]=n;
    }
    int top(){
        return data[index];
    }
    void pop(){         //fake delete
        index--;
    }
    bool isEmpty(){
        if(index==-1)return true;
        return false;
    }
    int size(){
        return index+1;
    }
    void destroy(){
        free(data);
    }
}stack;


typedef struct minHeap{

    //Use array to store the heap
    DATA *data;
    int size;
    int length;

    void create(int n = 32){
        data = (DATA*)malloc((n + 1) * sizeof(DATA));    //The first element is not used(data[0])
        size = n;
        length = 0;
    }

    //Maintain the heap, make sure the i position smaller than its children
    void min_heapify(int i){
        int left = LEFT(i), right = RIGHT(i), smallest = i;
        if (left <= length && data[left].weight < data[smallest].weight)
            smallest = left;
        if (right <= length && data[right].weight < data[smallest].weight)
            smallest = right;
        if (smallest != i){
            swap(&data[i], &data[smallest]);
            min_heapify(smallest);
        }
    }

    //Insert a new element into the heap, and keep the heap maintained
    void insert(DATA input){
        if (length == size){
            size <<= 1;
            data = (DATA*)realloc(data, (size + 1)* sizeof(DATA));
        }
        data[++length] = input;
        int i = length;
        while (i > 1 && data[i].weight < data[PARENT(i)].weight){
            swap(&data[i], &data[PARENT(i)]);
            i = PARENT(i);
        }
    }

    DATA min(){
        return data[1];
    }

    //Pop the smallest element in the heap, and keep the heap maintained
    DATA pop_min(){
        DATA smallest = data[1];
        data[1] = data[length--];
        min_heapify(1);
        return smallest;
    }

    bool isEmpty(){
        return length == 0;
    }

    void destroy(){
        free(data);
    }
}minheap;


typedef struct SingleNode{
    Pair value;
    int next_node;
}node;

typedef struct SingleList{
    node* data;
    stack index;
    int size = 0;
    int head;
    int tail;

    void create(int n = 32){
        data = (node*)malloc(n * sizeof(node));
        index.create(n);
        head = NIL;
        tail = NIL;
        for(int i = 0; i < n; i++){
            index.push(i);
        }
        size = 0;
    }

    void insert(int vertex, double weight) {
        if (index.isEmpty()) {
            printf("The list is full.");
            return;
        }
        int i = index.top();
        index.pop();
        data[i].value.create(vertex, weight);
        data[i].next_node = NIL;
        if (head == NIL) {
            head = i;
        } else {
            data[tail].next_node = i;
        }
        tail = i;
        size++;
    }
}list;


typedef struct DijkstraAlgorithm{
    minheap heap;
    list* graph;
    int* path;  //store the path from the start vertex to the vertex i
    double* distance;
    int size;

    void create(int n){
        heap.create(n);
        graph = (list*)malloc(n * sizeof(list));
        path = (int*)malloc(n * sizeof(int));
        distance = (double*)malloc(n * sizeof(double));
        size = n;
        for(int i = 0; i < n; i++){
            graph[i].create(n);
        }
    }

    //give the weight and direction of the edge into the adjacency list
    void insert(int vertex, int next_vertex, double weight){
        graph[vertex].insert(next_vertex, weight);
    }

    void dijkstra(int start){
        for(int i = 0; i < size; i++){
            distance[i] = INF;
            path[i] = NIL;
        }

        distance[start] = 0;
        heap.insert({start, 0});
        while(!heap.isEmpty()){
            Pair curr = heap.pop_min();
            int vertex = curr.index;
            if(distance[vertex] == INF)break;
            for(int i = graph[vertex].head; i != NIL; i = graph[vertex].data[i].next_node){
                int next_vertex = graph[vertex].data[i].value.index;
                double weight = graph[vertex].data[i].value.weight;
                if(distance[next_vertex] > distance[vertex] + weight){
                    distance[next_vertex] = distance[vertex] + weight;
                    path[next_vertex] = vertex;
                    heap.insert({next_vertex, distance[next_vertex]});  //The reason why only
                }   //push the distance-modified vertex into the heap is that the vertex which is not modified have
            }       //no effect on updating the shortest path, so there is no need to push it into the heap.
        }
    }

    void destroy(){
        heap.destroy();
        free(graph);
        free(path);
        free(distance);
    }
}DijAlgo;

int main(){
    DijAlgo A;
    A.create(5);
    A.insert(0, 1, 10);
    A.insert(0, 3, 5);
    A.insert(1, 2, 1);
    A.insert(1, 3, 2);
    A.insert(2, 4, 4);
    A.insert(3, 1, 3);
    A.insert(3, 2, 9);
    A.insert(3, 4, 2);
    A.insert(4, 0, 7);
    A.insert(4, 2, 6);
    A.dijkstra(0);
    for(int i = 0; i < 5; i++){
        printf("%d : %d, ", i, A.path[i]);
    }
    printf("\n");
    for(int i = 0; i < 5; i++){
        printf("%d : %lf, ", i, A.distance[i]);
    }
    A.destroy();
    return 0;
}