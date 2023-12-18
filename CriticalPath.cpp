#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) + 1)
#define PARENT(i) (i >> 1)

#define NIL (-1)
#define INF 0x7fffffff

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

//store the topological result(critical path) in a queue(FIFO)
typedef struct Queue{
    int* data;
    int head = 0;
    int tail = 0;
    int capacity = 0;

    void create(int n = 32){
        data = (int*)malloc(n * sizeof(int));
        capacity = n;
    }

    void push(int n){
        if((tail + 1) % capacity == head){
            printf("The queue is full.");
            return;
        }
        data[tail] = n;
        tail = (tail + 1) % capacity;
    }

    int front(){
        return data[head];
    }

    void pop(){
        head = (head + 1) % capacity;
    }

    bool isEmpty(){
        if(head == tail)return true;
        return false;
    }

    int size(){
        return (tail - head + capacity) % capacity;
    }

    void destroy(){
        free(data);
    }

}queue;

typedef struct SingleNode{
    int vertex;
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

    void insert(int vertex) {
        if (index.isEmpty()) {
            printf("The list is full.");
            return;
        }
        int i = index.top();
        index.pop();
        data[i].vertex = vertex;
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


typedef struct CriticalPath{
    list* graph;
    int size;
    int* indegree;
    double* weight;   //store the weight of every vertex
    double* critical; //store the critical weight of every vertex(by using individual array)
    queue q;

    void create(int n){
        graph = (list*)malloc(n * sizeof(list));
        size = n;
        for(int i = 0; i < n; i++){
            graph[i].create(n);
        }
        indegree = (int*)malloc(n * sizeof(int));
        memset(indegree, 0, n * sizeof(int));
        q.create(n + 1);
        weight = (double*)malloc(n * sizeof(double));
        memset(weight, 0, n * sizeof(int));
        critical = (double*)malloc(n * sizeof(double));
        memset(critical, 0, n * sizeof(int));
    }

    //give the direction of the edge into the adjacency list
    void insert_next(int vertex, int next_vertex){
        graph[vertex].insert(next_vertex);
    }

    //give the weight of the edge into the adjacency list, and initialize the critical array
    void insert_weight(int vertex, double w){
        weight[vertex] = w;
        critical[vertex] = w;
    }

    void topological_sort(){
        for(int i = 0; i < size; i++){
            for(int j = graph[i].head; j != NIL; j = graph[i].data[j].next_node){
                indegree[graph[i].data[j].vertex]++;
            }
        }
        for(int i = 0; i < size; i++){
            if(indegree[i] == 0){
                q.push(i);
            }
        }
        int count = 0;
        while(!q.isEmpty()){
            int i = q.front();
            q.pop();
            count++;
            for(int j = graph[i].head; j != NIL; j = graph[i].data[j].next_node){
                indegree[graph[i].data[j].vertex]--;
                if(indegree[graph[i].data[j].vertex] == 0){
                    q.push(graph[i].data[j].vertex);
                }
                if(critical[graph[i].data[j].vertex] < critical[i] + weight[graph[i].data[j].vertex]){
                    critical[graph[i].data[j].vertex] = critical[i] + weight[graph[i].data[j].vertex];
                }
            }
        }
        //if there is a cycle in the graph, the count will be less than the size of the graph
        if(count != size){
            printf("There is a cycle in the graph.");
        }
    }

    void destroy(){
        for(int i = 0; i < size; i++){
            graph[i].index.destroy();
        }
        free(graph);
        free(indegree);
        q.destroy();
    }
}cp;

int main(){
    cp a;
    a.create(6);
    a.insert_next(0, 1);
    a.insert_next(0, 3);
    a.insert_next(1, 2);
    a.insert_next(1, 3);
    a.insert_next(1, 4);
    a.insert_next(2, 3);
    a.insert_next(4, 2);
    a.insert_next(5, 2);
    a.insert_next(5, 4);
    a.insert_weight(0, 5.2);
    a.insert_weight(1, 6.1);
    a.insert_weight(2, 4.7);
    a.insert_weight(3, 8.1);
    a.insert_weight(4, 9.5);
    a.insert_weight(5, 17.1);
    a.topological_sort();

    printf("The critical path to every vertex:\n");
    for(int i = 0; i < a.size; i++) {
        printf("%d: %lf \n", i, a.critical[i]);
    }

    printf("The critical path is: ");
    for(int i = 0; i < a.size - 1; i++) {
        printf("%d, ", a.q.data[i]);
    }
    printf("%d\n", a.q.data[a.size - 1]);
    a.destroy();
    return 0;
}