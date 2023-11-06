#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define DATA int
#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) + 1)
#define PARENT(i) (i >> 1)

void swap(int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}

typedef struct minHeap{

    //Use array to store the heap
    DATA *data;
    int size;
    int length;

    void create(int n = 32){
        data = (int*)malloc((n + 1) * sizeof(DATA));    //The first element is not used(data[0])
        size = n;
        length = 0;
    }

    //Maintain the heap, make sure the i position smaller than its children
    void min_heapify(int i){
        int left = LEFT(i), right = RIGHT(i), smallest = i;
        if (left <= length && data[left] < data[smallest])
            smallest = left;
        if (right <= length && data[right] < data[smallest])
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
            data = (int*)realloc(data, (size + 1)* sizeof(DATA));
        }
        data[++length] = input;
        int i = length;
        while (i > 1 && data[i] < data[PARENT(i)]){
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
}minheap;

int main() {
    minheap heap;
    heap.create();
    for (int i = 64; i >= 1; i--)
        heap.insert(i);
    int i = 0;
    while (!heap.isEmpty()){
        if(i == 10){
            printf("\n");
            i = 0;
        }
        printf("%d ", heap.pop_min());
        i++;
    }
    return 0;
}