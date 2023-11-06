#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define SPACE 1024

typedef struct Grid{
    int front_address;
    int space;
    Grid *prev, *next;
}grid;

typedef struct Task_Node{
    int id;
    int task_address;
    int space_occupied;
    Task_Node *next;
}task;

typedef struct Partitioning_Simulator{
    grid *grid_head;
    task *task_head;

    //Initialize the storage grid and task list
    void initializing(){
        grid_head = (grid*)malloc(sizeof(grid));
        grid_head -> front_address = 0;
        grid_head -> space = SPACE;
        grid_head -> prev = grid_head;
        grid_head -> next = grid_head;
        task_head = NULL;
    }

    //Erase one storage grid where the curr points to
    void erase_grid(grid* curr){
        if(curr -> next == curr){
            free(curr);
            grid_head = NULL;
            return;
        }
        if(curr == grid_head){
            grid_head = curr -> next;
        }
        curr -> next -> prev = curr -> prev;
        curr -> prev -> next = curr -> next;
        free(curr);
    }

    //Create a task
    void create_task(int task_id, int task_space){
        //Check the space if it is sufficient
        grid *curr = grid_head;
        bool is_space = false;
        do{
            if(curr == NULL)break;
            if(curr -> space >= task_space){
                is_space = true;
                break;
            }
            curr = curr -> next;
        }while(curr != grid_head);
        if(!is_space){
            printf("The space is insufficient for this task(ID: %d)!\n", task_id);
            return;
        }

        //create a task for the task list
        task* newTask = (task*)malloc(sizeof(Task_Node));
        newTask -> id = task_id;
        newTask -> task_address = curr -> front_address;
        newTask -> space_occupied = task_space;

        //find a place for the task
        if(task_head == NULL){
            task_head = newTask;
            newTask -> next = NULL;
        }else{
            if(task_head -> id > task_id){
                newTask -> next = task_head;
                task_head = newTask;
            }else{
                task *currTask = task_head;
                while(currTask -> next != NULL){
                    if(currTask -> next -> id < task_id){
                        currTask = currTask -> next;
                    }else break;
                }
                newTask -> next = currTask -> next;
                currTask -> next = newTask;
            }
        }

        //extract the grid just used and repositions it(to keep the grid list ordered)
        curr -> front_address += task_space;
        curr -> space -= task_space;
        if(curr -> space == 0){
            erase_grid(curr);
            return;
        }

        grid *newGrid = curr;
        if(grid_head == curr){
            if(grid_head -> next == grid_head)grid_head = NULL;
            else grid_head = curr -> next;
        }
        curr -> next -> prev = curr -> prev;
        curr -> prev -> next = curr -> next;

        grid *currGrid;
        if(grid_head == NULL) {
            grid_head = newGrid;
            newGrid->prev = newGrid;
            newGrid->next = newGrid;
        }
        currGrid = grid_head;
        if(currGrid -> space > newGrid -> space){
            currGrid = currGrid -> prev;
            grid_head = newGrid;
        }else while(currGrid -> next != grid_head){
                if(currGrid -> next -> space > newGrid -> space)break;
                currGrid = currGrid -> next;
            }
        newGrid -> next = currGrid -> next;
        newGrid -> prev = currGrid;
        currGrid -> next -> prev = newGrid;
        currGrid -> next = newGrid;
    }

    void end_task(int task_id){
        task *curr = task_head;
        if(task_head == NULL){
            printf("The task(ID: %d) is not found!\n", task_id);
            return;
        }
        if(task_head -> id == task_id){
            task_head = task_head -> next;
        }else while(true){
            if(curr -> next == NULL){
                printf("The task(ID: %d) is not found!\n", task_id);
                return;
            }
            if(curr -> next -> id == task_id){
                    task *temp = curr -> next;
                    curr -> next = curr -> next -> next;
                    curr = temp;
                    break;
                }
                curr = curr -> next;
            }

        //give back the space to the grid list
        grid *newGrid = (grid*)malloc(sizeof(grid));
        newGrid -> front_address = curr -> task_address;
        newGrid -> space = curr -> space_occupied;
        free(curr);

        //merge the grid list
        bool flag = true;
        grid *currGrid;
        while(flag){
            currGrid = grid_head;
            flag = false;
            if(currGrid == NULL)break;
            do{
                if(currGrid -> front_address < newGrid -> front_address){
                    if(currGrid -> front_address + currGrid -> space == newGrid -> front_address){
                        newGrid -> front_address = currGrid -> front_address;
                        newGrid -> space += currGrid -> space;
                        erase_grid(currGrid);
                        flag = true;
                        break;
                    }
                }else{
                    if(currGrid -> front_address == newGrid -> front_address + newGrid -> space){
                        newGrid -> space += currGrid -> space;
                        erase_grid(currGrid);
                        flag = true;
                        break;
                    }
                }
                currGrid = currGrid -> next;
            }while(currGrid != grid_head);
        }

        //insert the new grid into the grid list
        if(grid_head == NULL) {
            grid_head = newGrid;
            newGrid->prev = newGrid;
            newGrid->next = newGrid;
        }
        currGrid = grid_head;
        if(currGrid -> space > newGrid -> space){
            currGrid = currGrid -> prev;
            grid_head = newGrid;
        }else {
            while(currGrid -> next != grid_head){
                if(currGrid -> next -> space > newGrid -> space)break;
                currGrid = currGrid -> next;
            }
        }
        newGrid -> next = currGrid -> next;
        newGrid -> prev = currGrid;
        currGrid -> next -> prev = newGrid;
        currGrid -> next = newGrid;
    }

    void destory(){
        while(grid_head != NULL){
            erase_grid(grid_head);
        }
        task *curr = task_head;
        while(curr != NULL){
            task *temp = curr;
            curr = curr -> next;
            free(temp);
        }
    }
}partition;

//int main(){
//    partition ps;
//    ps.initializing();
//    task *head = ps.task_head;
//    grid *g = ps.grid_head;
//    int x = 0;
//    int y = 0;
//    int in_time[10]{0, 2, 10 ,50 ,100, 500, 700, 750, 900, 950};
//    int id_in[10]{2, 1, 4, 9, 8, 3, 6, 5, 7, 10};
//    int space[10]{500, 10, 50, 70, 200, 511, 800, 20, 60, 1};
//    int out_time[10]{50, 100, 200, 300, 400, 800, 850, 950, 960, 970};
//    int id_out[10]{2, 1, 4, 9, 8, 3, 6, 5, 7, 10};
//    for(int i = 0; i < 1024; i++){
//        if(in_time[x] == i){
//            ps.create_task(id_in[x], space[x]);
//            x++;
//            head = ps.task_head;
//            g = ps.grid_head;
//            printf("In Action\n");
//            while(head != NULL){
//                printf("The id: %d, address: %d, space occupied: %d\n", head -> id, head -> task_address, head -> space_occupied);
//                head = head -> next;
//            }
//            do{
//                printf("The front address: %d, space: %d\n", g -> front_address, g -> space);
//                g = g -> next;
//            }while(g != ps.grid_head);
//            printf("\n");
//        }
//        if(out_time[y] == i){
//            ps.end_task(id_out[y]);
//            y++;
//            head = ps.task_head;
//            g = ps.grid_head;
//            printf("Out Action\n");
//            while(head != NULL){
//                printf("The id: %d, address: %d, space occupied: %d\n", head -> id, head -> task_address, head -> space_occupied);
//                head = head -> next;
//            }
//            do{
//                printf("The front address: %d, space: %d\n", g -> front_address, g -> space);
//                g = g -> next;
//            }while(g != ps.grid_head);
//            printf("\n");
//        }
//    }
//    ps.destory();
//    return 0;
//}

//