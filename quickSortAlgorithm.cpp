#include<stdio.h>
#include<stdlib.h>
#include<algorithm>

#define DATA int

typedef struct Stack{   //Stack
    DATA** data;
    int index=-1;
    int capacity=0;
    void create(int n=32){      //create a stack
        data=(DATA**)malloc(n*sizeof(DATA*));
        capacity=n;
    }
    void push(DATA* n){       //push the data into the stack ,will not resize by itself.
        if(index==(capacity-1)){
            printf("The stack is full.");
            return;
        }
        data[++index]=n;
    }
    DATA* top(){
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

bool __default_cmp(DATA a, DATA b){
    return a < b;
}

void swap(DATA* a, DATA* b){
    DATA temp = *a;
    *a = *b;
    *b = temp;
}

//The usage is the same as std::sort, no recursion
void quick_sort(DATA* BEGIN, DATA* END,bool (*cmp)(DATA, DATA)=__default_cmp){
    stack s;
    s.create(32);
    s.push(BEGIN);
    s.push(END);

    while(!s.isEmpty()){
        DATA *end = s.top();
        s.pop();
        DATA *begin = s.top();
        s.pop();
        DATA temp = *begin;
        DATA* i = begin;
        DATA* j = end - 1;
        while(i != j){
            while(i != j && cmp(temp, *j))j--;
            if(i != j){
                *i = *j;
                i++;
            }
            while(i != j && cmp(*i, temp))i++;
            if(i != j){
                *j = *i;
                j--;
            }
        }
        *i = temp;

        //divide the array into two parts and push their beginning and end into the stack
        if(i + 1 < end){
            s.push(i + 1);
            s.push(end);
        }
        if(begin < i - 1){
            s.push(begin);
            s.push(i);
        }
    }

    s.destroy();
}

int main(){
    int a[20], b[20];
    for(int i = 0; i < 20; i++){
        a[i] = b[i] = rand() % 10000;
    }

    std::sort(b, b + 20);

    quick_sort(a, a + 20);

    for(int i = 0; i < 20; i++){
        printf("%d ", a[i]);
        if(i == 9)printf("\n");
    }
    printf("\n\n");
    for(int i = 0; i < 20; i++){
        printf("%d ", b[i]);
        if(i == 9)printf("\n");
    }
    return 0;
}
