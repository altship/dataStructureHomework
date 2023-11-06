//
// Created by altship on 2023/9/14.
//
#include<iostream>
#ifndef C__PROJECTS_ALTSHIP_H
#define C__PROJECTS_ALTSHIP_H

#endif //C__PROJECTS_ALTSHIP_H
/*/typedef struct Node_by_array{
    int* data;
    int* index;
    int space=0;
    int end=0;
    void create(int n=32){
        data=(int*)malloc((n+1)*sizeof(int));   //data[0]=how many data have been stored
        index=(int*)calloc(n+1,sizeof(int));    //index[0]=the head of the data
        space=n;
        data[0]=0;
        end=0;
    }
    void push(int n,int x=-1){
        if(data[0]==space){
            printf("The space is insufficient!");
            return;
        }
        if(x==0){
            int temp=index[0];
            for (int i = 1; i <= space; i++) {
                if (index[i] == 0 && i != end) {
                    index[0]=i;
                    data[i]=n;
                    index[i]=temp;
                    break;
                }
            }
        }
        if(x==-1) {
            for (int i = 1; i <= space; i++) {
                if (index[i] == 0 && i != end) {
                    data[i] = n;
                    index[end] = i;
                    end = i;
                    break;
                }
            }
        }
        else{
            int temp=index[x];
            for (int i = 1; i <= space; i++) {
                if (index[i] == 0 && i != end) {
                    data[i] = n;
                    index[i]=temp;
                    index[x]=i;
                }
            }

        }
        data[0]++;
    }
    bool isEmpty(){
        if(data[0]==0)return true;
        return false;
    }
    int top(){
        return data[end];
    }
    void pop(int n=0){
        int temp;
        data[0]--;
        if(n==0)n=end;
        if(n==index[0]){
            temp=index[0];
            index[0]=index[temp];
            index[temp]=0;
            if(n==end)end=0;
            return;
        }
        for(temp=index[0];temp!=end;){
            if(index[temp]==n){
                index[temp]=index[n];
                index[n]=0;
                if(n==end)end=temp;
                return;
            }
            temp=index[temp];
        }
    }
    int find(int n){
        for(int i=index[0];;){
            if(i==0)return -1;
            if(data[i]==n)return i;
            i=index[i];
        }
        return -1;
    }
}Node_a;*/

typedef struct Stack{   //Stack
    int* data;
    int index=-1;
    int capacity=0;
    void create(int n=32){      //create a stack
        data=(int*)malloc(n*sizeof(int));
        capacity=n;
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

typedef struct Node{
    int data[2];
    int next=-1;
}node;

typedef struct Node_by_array{       //Node realizing by array
    node* data;     //data and next pointer stored in here
    stack index;    //empty data slots are stored in here, realizing by stack.
    int head=-1;
    int end=-1;
    int count=0;    //how many data have been stored.
    int space;      //how many space we have.
    void create(int n=32){
        data=(node*)malloc(n*sizeof(node));
        index.create(n);
        bool* used=(bool*)calloc(n,sizeof(bool));
        while(index.size()!=n){
            int i=rand()%n;     //randomly store slot index into stack:index
            if(!used[i]){
                index.push(i);
                used[i]=true;
            }
        }
        free(used);
        space=n;
    }
    int push(int* n,int i=-2){
        if(count==space)return -1;
        count++;
        int temp=index.top();
        index.pop();
        data[temp].data[0]=n[0];
        data[temp].data[1]=n[1];
        if(head==-1){       //when node_by_array is empty
            head=temp;
            end=head;
            return head;
        }
        if(i==-1){          //push before head, use an indicator i==-1.
            data[temp].next=head;
            head=temp;
            return head;
        }
        else if(i==-2||i==end){     //when push index is not given by user or use indicator i==-2, push after last node.
            data[end].next=temp;
            end=temp;
            return temp;
        }
        else{       //push after the given node.
            data[temp].next=data[i].next;
            data[i].next=temp;
            return temp;
        }
    }
    void pop(int i=-1){
        if(count==0)return;
        count--;
        if(i==head){    //pop out head node.
            index.push(i);
            head=data[head].next;
        }
        if(i==-1||i==end){      //pop out the end node
            int j;
            index.push(end);
            for(j=head;;){
                if(data[j].next==end)break;
                j=data[j].next;
            }
            end=j;
        }
        else{       //pop out a specified node
            int j;
            index.push(i);
            for(j=head;;){
                if(data[j].next==i)break;
                j=data[j].next;
            }
            data[j].next=data[i].next;
        }
    }
    void destroy(){
        free(data);
        index.destroy();
    }
}node_by_array;