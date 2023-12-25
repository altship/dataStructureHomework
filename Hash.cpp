#include<stdio.h>
#include<stdlib.h>

#define M 16
#define N 15    //with bitwise operation, N must be 2^n-1(here as 16 - 1 = 15)
#define MAX 0xffff

//hash function, with bitwise operation for better performance
unsigned hash(unsigned a){
    return a & N;
}


//using linked list to solve collision, here use dynamic space allocation to avoid waste of space
//the reason we use double linked list is that we can delete a node in O(1) time(after the element is found)
typedef struct Node {
    unsigned key;
    struct Node *next, *prev;
}node;


typedef struct HashTable {
    node *head[M];
    int count = 0;

    //initialize the hash table to null pointer
    void initialize(){
        for(int i = 0; i < M; i++){
            head[i] = NULL;
        }
    }

    //insert a node into the hash table
    void insert(unsigned key){
        unsigned index = hash(key);
        node *p = (node*)malloc(sizeof(node));
        p->key = key;
        p->next = head[index];
        p->prev = NULL;
        if(head[index] != NULL){
            head[index]->prev = p;
        }
        head[index] = p;
        count++;
    }

    //search a node in the hash table, return the pointer to the node if found, otherwise return null pointer
    node* search(unsigned key){
        unsigned index = hash(key);
        node *p = head[index];
        while(p != NULL){
            if(p->key == key){
                return p;
            }
            p = p->next;
        }
        return NULL;
    }

    //delete a node in the hash table
    //hash function is executed twice(one in search, one in delete part),
    //but since it is O(1) time and fast, it is acceptable
    //if we use another strategy to avoid hash twice, may cost more time
    void delete_node(unsigned key){
        node *p = search(key);
        if(p == NULL){
            printf("No such node!\n");
            return;
        }

        unsigned index = hash(key);
        if(p->prev != NULL){
            p->prev->next = p->next;
        }else{
            head[index] = p->next;  //avoid losing the whole chain
        }
        if(p->next != NULL){
            p->next->prev = p->prev;
        }
        free(p);
        count--;
    }

    //destroy the hash table and free the space, avoid memory leak
    void destroy(){
        for(int i = 0; i < M; i++){
            node *p = head[i];
            while(p != NULL){
                node *temp = p;
                p = p->next;
                free(temp);
            }
        }
    }
}hash_table;


int main(){
    unsigned input[40];
    hash_table table;
    table.initialize();
    for(int i = 0; i < 40; i++){
        input[i] = (unsigned)rand() % MAX;
        table.insert(input[i]);
    }

    //test every key is correctly inserted
    for(int i = 0; i < 40; i++){
        if(table.search(input[i]) == NULL)
            printf("Error in %u!\n", input[i]);
    }

    //show the structure of the hash table
    printf("The structure of the hash table:\n");
    for(int i = 0; i < M; i++){
        node *p = table.head[i];
        printf("hash: %d ;", i);
        while(p != NULL){
            printf("%u, ", p->key);
            p = p->next;
        }
        printf("\n");
    }

    //delete nodes
    for(int i = 0; i < 40; i++){
        table.delete_node(input[i]);
    }

    printf("\n\nAfter deletion:\n");

    //test every key is correctly deleted
    for(int i = 0; i < M; i++){
        node *p = table.head[i];
        printf("hash: %d ;", i);
        while(p != NULL){
            printf("%u, ", p->key);
            p = p->next;
        }
        printf("\n");
    }
    table.destroy();
    return 0;
}