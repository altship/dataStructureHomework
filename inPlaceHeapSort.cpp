#include<stdio.h>
#include<stdlib.h>

#define DATA int
#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) + 1)

typedef struct inPlaceSort{
    DATA* data;
    int size;

    void create(DATA* input, int n){
        data = input;
        size = n;
    }

    void swap(DATA* a, DATA* b){
        DATA t = *a;
        *a = *b;
        *b = t;
    }

    //Maintain the heap, make sure the i position smaller than its children
    void max_heapify(int i){
        int left = LEFT(i), right = RIGHT(i), largest = i;
        if(left < size && data[left] > data[largest])
            largest = left;
        if(right < size && data[right] > data[largest])
            largest = right;
        if(largest != i){
            swap(&data[i], &data[largest]);
            max_heapify(largest);
        }
    }

    //Build a max heap within an unsorted array
    void build_max_heap(){
        for(int i = size / 2; i >= 1; --i){
            max_heapify(i);
        }
    }

    //Heap sort by poping the largest element in the heap into the last position(updating the last position)
    void heap_sort(DATA* input, int n){
        create(input, n);
        build_max_heap();
        for(int i = size - 1; i > 1; --i){
            swap(&data[1], &data[i]);
            size--;
            max_heapify(1);
        }
    }
}sort;

int main(){
    DATA data[21];
    int i = 1;
    for(; i < 21; ++i){
        data[i] = rand();
    }
    sort s;
    s.heap_sort(data, 21);
    for(i = 1; i < 21; i++){
        printf("%d ", data[i]);
        if(i % 5 == 0){
            printf("\n");
        }
    }
    return 0;
}