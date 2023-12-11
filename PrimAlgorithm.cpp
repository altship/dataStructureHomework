#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define DATA pair
#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) + 1)
#define PARENT(i) (i >> 1)

#define NIL (-1)
#define INF 0x7fffffff

typedef struct Pair{
    int index1;
    int index2;
    double weight;

    void create(int i, int j, double w){
        index1 = i;
        index2 = j;
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

    void insert(int vertex1, int vertex2, double weight) {
        if (index.isEmpty()) {
            printf("The list is full.");
            return;
        }
        int i = index.top();
        index.pop();
        data[i].value.create(vertex1, vertex2, weight);
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


typedef struct PrimAlgorithm{
    minheap heap;
    list* graph;
    int** path;  //store the path from the start vertex to all the other vertex
    double total_weight = 0;
    int size;
    bool *visited;

    void create(int n){
        heap.create(n);
        graph = (list*)malloc(n * sizeof(list));
        path = (int**)malloc(n * sizeof(int*));
        for(int i = 0; i < n; i++){
            path[i] = (int*)malloc(2 * sizeof(int));
        }
        size = n;
        for(int i = 0; i < n; i++){
            graph[i].create(n);
        }
        visited = (bool*)malloc(n * sizeof(bool));
        memset(visited, 0, n * sizeof(bool));
    }

    //give the weight and direction of the edge into the adjacency list
    void insert(int vertex, int next_vertex, double weight){
        graph[vertex].insert(vertex, next_vertex, weight);
    }

    void prim(int start){
        path[0][0] = NIL;
        path[0][1] = start;
        int temp = graph[start].head;
        visited[start] = true;
        //Insert all the edges connected to the start vertex into the heap
        while(temp != NIL){
            heap.insert(graph[start].data[temp].value);
            temp = graph[start].data[temp].next_node;
        }

        for(int i = 1; i < size; i++){
            if(heap.isEmpty())break;
            Pair curr = heap.pop_min();
            int vertex1 = curr.index1;
            int vertex2 = curr.index2;
            double weight = curr.weight;
//            if(visited[vertex1] && visited[vertex2]){
//                i--;    //If the both vertexes are visited, then this edge is useless, so we need to decrease the i
//                continue;
//            }
            path[i][0] = vertex1;
            path[i][1] = vertex2;
            visited[vertex2] = true;
            temp = graph[vertex2].head;
            while(temp != NIL){
                if(!visited[graph[vertex2].data[temp].value.index2])
                    heap.insert(graph[vertex2].data[temp].value);
                temp = graph[vertex2].data[temp].next_node;
            }
            total_weight += weight;
        }
    }

    void destroy(){
        heap.destroy();
        free(graph);
        free(path);
    }
}Prim;

int main(){
    Prim A;
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
    A.prim(4);
    printf("The total weight: %lf\nPath:\n", A.total_weight);
    for(int i = 0; i < A.size; i++){
        printf("%d, %d\n", A.path[i][0], A.path[i][1]);
    }
    A.destroy();
    return 0;
}