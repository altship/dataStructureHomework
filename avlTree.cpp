#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define DATA int
#define inf 0x3f3f3f3f

int max(int a, int b){
    if(a > b)return a;
    return b;
}

typedef struct Node{
    DATA data;
    int height;
    int parent, left, right;
}node;

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
    int size(){
        return index+1;
    }
    bool isEmpty(){
        if(index==-1)return true;
        return false;
    }
    void destroy(){
        free(data);
    }
}stack;

typedef struct AVL_tree{
    node *list;
    stack emptyNode;
    int head = 0;

    void create(int n = 32){
        list = (node*)malloc(n * sizeof(node));
        emptyNode.create(n);
        for(int i = 1; i < n; i++){
            emptyNode.push(i);
        }
        list[0].height = -1;
    }

    void LLrotate(int curr){
        int parent = list[curr].parent;
        int left = list[curr].left;
        if(list[parent].left == curr){
            list[parent].left = left;
        }else{
            list[parent].right = left;
        }
        list[left].parent = parent;
        list[curr].left = list[left].right;
        list[list[left].right].parent = curr;
        list[left].right = curr;
        list[curr].parent = left;
        //update subtree height
        list[curr].height = max(list[list[curr].left].height, list[list[curr].right].height) + 1;
        list[left].height = max(list[list[left].left].height, list[list[left].right].height) + 1;
        if(curr == head)head = left;
    }

    void RRrotate(int curr){
        int parent = list[curr].parent;
        int right = list[curr].right;
        if(list[parent].left == curr){
            list[parent].left = right;
        }else{
            list[parent].right = right;
        }
        list[right].parent = parent;
        list[curr].right = list[right].left;
        list[list[right].left].parent = curr;
        list[right].left = curr;
        list[curr].parent = right;
        //update subtree height
        list[curr].height = max(list[list[curr].left].height, list[list[curr].right].height) + 1;
        list[right].height = max(list[list[right].left].height, list[list[right].right].height) + 1;
        if(curr == head)head = right;
    }

    void LRrotate(int curr){
        int left = list[curr].left;
        RRrotate(left);
        LLrotate(curr);
    }

    void RLrotate(int curr){
        int right = list[curr].right;
        LLrotate(right);
        RRrotate(curr);
    }

    void push(DATA x){
        if(emptyNode.isEmpty())return;
        int i = emptyNode.top();
        emptyNode.pop();
        list[i].data = x;
        list[i].height = 0;
        list[i].left = list[i].right = 0;
        if(head == 0){
            head = i;
            list[i].parent = 0;
            return;
        }
        //insert node
        int curr = head;
        while(1){
            if(list[curr].data > x){
                if(list[curr].left == 0){
                    list[i].parent = curr;
                    list[curr].left = i;
                    break;
                }
                curr = list[curr].left;
            }else{
                if(list[curr].right == 0){
                    list[i].parent = curr;
                    list[curr].right = i;
                    break;
                }
                curr = list[curr].right;
            }
        }
        //maintain balance
        int p1 = 0, p2 = 0, last = i;
        while(curr != 0){
            p2 = p1;
            if(last == list[curr].left){
                p1 = 1;
            }else {
                p1 = -1;
            }
            list[curr].height = max(list[list[curr].left].height, list[list[curr].right].height) + 1;
            if(abs(list[list[curr].left].height - list[list[curr].right].height) > 1)break;
            last = curr;
            curr = list[curr].parent;
        }
        if(curr == 0)return;
        if(p1 == 1){
            if(p2 == 1)LLrotate(curr);
            else LRrotate(curr);
        }else{
            if(p2 == 1)RLrotate(curr);
            else RRrotate(curr);
        }
    }

    void destory(){
        emptyNode.destroy();
        free(list);
    }
}avltree;

//int main(){
//    int a[] = {3, 10, 17, 27, 7, 23, 12, 36, 38, 45, 44};
//    avltree tree;
//    tree.create(12);
//    for(auto i : a){
//        tree.push(i);
//    }
//    tree.destory();
//    return 0;
//}