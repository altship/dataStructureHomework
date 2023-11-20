#include<stdio.h>
#include<stdlib.h>
#include<algorithm>

#define DATA int
#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) + 1)
#define inf 0x7fffffff

// Modified from minHeap
typedef struct M_WaySort{
    DATA** data;
    int size;

    void create(int n){
        data = (DATA**)malloc(sizeof(DATA*) * (n + 1));
        size = n;
    }

    //Swap two elements, make sure the swap is only called after the variable "data" is initialized.
    void swap(DATA** a, DATA** b){
        data[0]= *a;
        *a = *b;
        *b = data[0];
    }

    //Maintain the heap, make sure the i position smaller than its children
    void min_heapify(int i){
        int left = LEFT(i), right = RIGHT(i), largest = i;
        if(left <= size && *data[left] < *data[largest])
            largest = left;
        if(right <= size && *data[right] < *data[largest])
            largest = right;
        if(largest != i){
            swap(&data[i], &data[largest]);
            min_heapify(largest);
        }
    }

    //Build a max heap within an unsorted array
    void build_max_heap(){
        for(int i = size / 2; i >= 1; --i){
            min_heapify(i);
        }
    }

    DATA* min(){
        return data[1];
    }

    //Pop the smallest element in the heap, and keep the heap maintained
    DATA* pop_min(){
        DATA* smallest = data[1];
        data[1] = data[size--];
        min_heapify(1);
        return smallest;
    }

    void destroy(){
        free(data);
    }

    //Main sort function
    void mergeSortByMinHeap(DATA* input[], int S, DATA* output){
        create(S);
        int i = 1;
        DATA* curr = output;
        for(; i <= size; ++i){
            data[i] = input[i - 1];
        }
        build_max_heap();  //initialize the heap

        while(size > 0){
            *curr++ = *min();
            data[1]++;
            if(*data[1] == -inf)pop_min();  //When the array is empty, pop this array out of the heap
            else min_heapify(1);
        }
        destroy();
    }
}sort;

int main(){
    int* input[10];
    for(int i = 0; i < 10; i++){
        input[i] = (int*) malloc(11 * sizeof(int));
        for(int j = 0; j < 10; j++){
            input[i][j] = rand() % 100000;
        }
        input[i][10] = -inf;
    }

    for(auto& i : input){
        std::sort(i, i + 10);
    }

    int output[100];
    sort c;
    c.mergeSortByMinHeap((int**)input, 10, output);
    for(int i = 0; i < 100; ++i){
        printf("%d ", output[i]);
        if((i + 1) % 10 == 0)printf("\n");
    }
    for(auto& i : input){
        free(i);
    }
    return 0;
}