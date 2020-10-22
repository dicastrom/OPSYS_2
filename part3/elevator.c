// LIZ'S CODE
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/linkage.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>

typedef struct _passenger{
    int type;
    int start;
    int destination;
    struct list_head list;
} Passenger;

static Passenger Passenger_floor;
static Passenger Passenger_elevator;

static struct _elevator{
    char* status;
    int waiting;
    int serviced;
    int active;
    int passengers;
    int infected;
    int curr_floor;
} Elevator;

struct thread_parameters {
    struct mutex mutex;
    struct task_struct *kthread;
};

// IN PROGRESS - NOW
int thread_run(void *data) {
    struct thread_parameters *parm = data;

    struct list_head* temp;
    struct list_head* dummy;

    Passenger* newitem;

    while (!kthread_should_stop()) {
        if (mutex_lock_interruptible(&parm->mutex) == 0) { 
            if( (Elevator.active && Elevator.waiting > 0) || Elevator.passengers > 0){
                switch(Elevator.passengers){
                    case 0:
                        ssleep(2);
                        if(list_first_entry(&Passenger_floor.list, Passenger, list)->start > Elevator.curr_floor){
                            Elevator.curr_floor++;
                            Elevator.status = "UP";
                        }else if(list_first_entry(&Passenger_floor.list, Passenger, list)->start < Elevator.curr_floor){
                            Elevator.status = "DOWN";
                            Elevator.curr_floor--;
                        }
                        break;
                    default:
                        ssleep(2);
                        printk(KERN_NOTICE "First passenger in the elevator wants to go to floor %d",list_first_entry(&Passenger_elevator.list, Passenger, list)->destination);
                        
                        if(list_first_entry(&Passenger_elevator.list, Passenger, list)->destination > Elevator.curr_floor){
                            Elevator.curr_floor++;
                            Elevator.status = "UP";
                        }else if(list_first_entry(&Passenger_elevator.list, Passenger, list)->destination < Elevator.curr_floor){
                            Elevator.curr_floor--;
                            Elevator.status = "DOWN";
                        }
                        break;
                }
                
                if(Elevator.passengers > 0){
                    list_for_each_safe(temp,dummy,&Passenger_elevator.list){
                        if(list_entry(temp, Passenger, list)->destination == Elevator.curr_floor){
                            Elevator.status = "LOADING";
                            Elevator.passengers--;
                            Elevator.serviced++;
                            
                            switch (Elevator.passengers){
                                case 0:
                                    Elevator.infected = 0;
                                    break;
                            }

                            list_del(temp);
                            kfree(list_entry(temp, Passenger, list));
                        }
                    }
                }

                switch(Elevator.active){
                    case 1:
                        if(Elevator.waiting > 0){
                            ssleep(1);
                            list_for_each_safe(temp,dummy,&Passenger_floor.list){
                                int num = Elevator.passengers + 1;
                                
                                switch(num){
                                    case 10:
                                        if(list_entry(temp, Passenger, list)->start == Elevator.curr_floor){
                                            if(list_entry(temp, Passenger, list)->type == Elevator.infected){
                                                Elevator.status = "LOADING";

                                                newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                if(newitem == NULL){
                                                    return -ENOMEM;
                                                }
                                                
                                                newitem->start = list_entry(temp, Passenger, list)->start;
                                                newitem->type = list_entry(temp, Passenger, list)->type;
                                                newitem->destination = list_entry(temp, Passenger, list)->destination;
                                                
                                                list_add_tail(&newitem->list,&Passenger_elevator.list);
                                                
                                                Elevator.passengers++;
                                                Elevator.waiting--;

                                                if(list_entry(temp, Passenger, list)->type){
                                                    switch (Elevator.infected){
                                                        case 0:
                                                            Elevator.infected = list_entry(temp, Passenger, list)->type;
                                                            break;
                                                    }
                                                }
                                                
                                                list_del(temp);
                                                kfree(list_entry(temp, Passenger, list));
                                            } else if(list_entry(temp, Passenger, list)->type){
                                                Elevator.status = "LOADING";

                                                newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                if(newitem == NULL){
                                                    return -ENOMEM;
                                                }
                                                
                                                newitem->start = list_entry(temp, Passenger, list)->start;
                                                newitem->type = list_entry(temp, Passenger, list)->type;
                                                newitem->destination = list_entry(temp, Passenger, list)->destination;
                                                
                                                list_add_tail(&newitem->list,&Passenger_elevator.list);
                                                
                                                Elevator.passengers++;
                                                Elevator.waiting--;

                                                if(list_entry(temp, Passenger, list)->type){
                                                    switch (Elevator.infected){
                                                        case 0:
                                                            Elevator.infected = list_entry(temp, Passenger, list)->type;
                                                            break;
                                                    }
                                                }

                                                list_del(temp);
                                                kfree(list_entry(temp, Passenger, list));
                                            } 
                                        }
                                        break;
                                    default: 
                                        if(list_entry(temp, Passenger, list)->start == Elevator.curr_floor){
                                            if(num < 10){
                                                if(list_entry(temp, Passenger, list)->type == Elevator.infected){
                                                    Elevator.status = "LOADING";

                                                    newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                    if(newitem == NULL){
                                                        return -ENOMEM;
                                                    }
                                                    
                                                    newitem->start = list_entry(temp, Passenger, list)->start;
                                                    newitem->type = list_entry(temp, Passenger, list)->type;
                                                    newitem->destination = list_entry(temp, Passenger, list)->destination;
                                                    
                                                    list_add_tail(&newitem->list,&Passenger_elevator.list);
                                                    
                                                    Elevator.passengers++;
                                                    Elevator.waiting--;

                                                    if(list_entry(temp, Passenger, list)->type){
                                                        switch (Elevator.infected){
                                                            case 0:
                                                                Elevator.infected = list_entry(temp, Passenger, list)->type;
                                                                break;
                                                        }
                                                    }

                                                    list_del(temp);
                                                    kfree(list_entry(temp, Passenger, list));
                                                } else if(list_entry(temp, Passenger, list)->type){
                                                    Elevator.status = "LOADING";

                                                    newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                    if(newitem == NULL){
                                                        return -ENOMEM;
                                                    }
                                                    
                                                    newitem->start = list_entry(temp, Passenger, list)->start;
                                                    newitem->type = list_entry(temp, Passenger, list)->type;
                                                    newitem->destination = list_entry(temp, Passenger, list)->destination;
                                                    
                                                    list_add_tail(&newitem->list,&Passenger_elevator.list);
                                                    
                                                    Elevator.passengers++;
                                                    Elevator.waiting--;

                                                    if(list_entry(temp, Passenger, list)->type){
                                                        switch (Elevator.infected){
                                                            case 0:
                                                                Elevator.infected = list_entry(temp, Passenger, list)->type;
                                                                break;
                                                        }
                                                    }

                                                    list_del(temp);
                                                    kfree(list_entry(temp, Passenger, list));
                                                } 
                                            }
                                        }
                                        break;
                                }
                            }
                        }
                        break;
                }
            }else{
                switch(Elevator.passengers){
                    case 0: 
                        if(Elevator.active){
                            switch (Elevator.waiting){
                                case 0:
                                    Elevator.status = "IDLE";
                                    break;
                                default:
                                    switch(Elevator.active){
                                        case 0:
                                            Elevator.status = "OFFLINE";
                                            break;
                                    }
                                    break;
                            }
                        } else {
                            if(Elevator.active == 0){
                                Elevator.status = "OFFLINE";
                            }
                        }
                        break;
                }
            }
            
        }
        mutex_unlock(&parm->mutex);
    }

    return 0;
}

// IN PROGRESS - WORKS
long start_elevator(void){
    int same = strcmp(Elevator.status,"OFFLINE");

    switch(same){
        case 0:
            Elevator.status = "IDLE";
            Elevator.curr_floor = 1;
            Elevator.passengers = 0;
            Elevator.passengers = 0;
            Elevator.waiting = 0;
            Elevator.serviced = 0;
            Elevator.active = 1;
            Elevator.infected = 0;
            printk(KERN_NOTICE "Elevator initialized. Status: %s\n", Elevator.status);
            return 0;
            break;
        default:
            if(Elevator.status == NULL){
                Elevator.status = "IDLE";
                Elevator.curr_floor = 1;
                Elevator.passengers = 0;
                Elevator.passengers = 0;
                Elevator.waiting = 0;
                Elevator.serviced = 0;
                Elevator.active = 1;
                Elevator.infected = 0;
                printk(KERN_NOTICE "Elevator initialized. Status: %s\n", Elevator.status);
                return 0;
            } else {
                return 1;
            }
            break;
    }
}

// DONE - WORKS
long stop_elevator(void){
    int same = strcmp("OFFLINE", Elevator.status);

    switch (same){
        case 1:
            Elevator.active = 0;
            return 1;            
            break;
        default:
            return 0;
    }

    return same;
}

// IN PROGRESS - WORKS
long issue_request(int start_floor, int dest_floor, int type){
    Passenger* temp = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

    if(type > 2){
        return 1;
    } else {
        if(type < 0){
            return 1;
        } else {
            if(start_floor < 1){
                return 1;
            } else {
                if(dest_floor < 1){
                    return 1;
                } else {
                    if(start_floor > 10){
                        return 1;
                    } else {
                        if(dest_floor > 10){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    
    if(temp != NULL){
        temp->type = type;
        printk("Loaded a %d passenger.\n",temp->type);

        temp->start = start_floor;
        temp->destination = dest_floor;
        Elevator.waiting += 1;
        
        list_add_tail(&temp->list, &Passenger_floor.list);
    } else {
        return -ENOMEM;
    }

    return 0;
}

static int read;
struct thread_parameters thread;
static char* filestring;
// IN PROGRESS - WORKS
int elevator_proc_open(struct inode *sp_inode, struct file *sp_file) {
    char* representation = kmalloc(sizeof(char) * 1000, __GFP_FS | __GFP_RECLAIM | __GFP_IO);
    filestring = kmalloc(sizeof(char) * 1000, __GFP_IO | __GFP_RECLAIM | __GFP_FS);

    if(representation == ""){
        return -ENOMEM;
    } else if(representation == NULL){
        return -ENOMEM;
        } else {
            if(filestring == NULL){
                return -ENOMEM;
            } else {

                sprintf(filestring, "Elevator state: %s\n", Elevator.status);

                char* result = kmalloc(sizeof(char) * 1000, __GFP_IO | __GFP_RECLAIM | __GFP_FS);

                strcat(filestring, "Elevator status: ");
                switch(Elevator.infected){
                    case 1:
                        strcat(filestring, "Infected\n");
                        break;
                    case 0:
                        strcat(filestring, "Not infected\n");
                        break;
                }
                
                struct list_head* temp;
                int j = 0;
                int i = 0;
                int length = 0;

                strcat(filestring, "Current floor: ");
                sprintf(result, "%d", Elevator.curr_floor);
                strcat(filestring, result);
                strcat(filestring, "\n");
                strcat(filestring, "Number of passengers: ");
                sprintf(result, "%d", Elevator.passengers);
                strcat(filestring, result);
                strcat(filestring, "\n");
                strcat(filestring, "Number of passengers waiting: ");
                sprintf(result, "%d", Elevator.waiting);
                strcat(filestring, result);
                strcat(filestring, "\n");
                strcat(filestring, "Number of passengers serviced: ");
                sprintf(result, "%d", Elevator.serviced);
                strcat(filestring, result);
                strcat(filestring, "\n\n");

                int floor = 0;
                while(i < 10){
                    j=0;
                    floor = 10 - i;
                    memset(representation, 0, strlen(representation));

                    if(Elevator.curr_floor == floor){
                        strcat(filestring, "[ * ] ");

                        strcat(filestring, "Floor ");
                        sprintf(result, "%d: ", floor);
                        strcat(filestring, result);

                        list_for_each(temp, &Passenger_floor.list){
                            if(list_entry(temp, Passenger, list)->start == floor){
                                j++;
                                if(list_entry(temp, Passenger, list)->type){
                                    strcat(representation, "X ");
                                } else {
                                    strcat(representation, "| ");
                                }
                            }
                        }

                        sprintf(result, "%d ", j);
                        strcat(filestring,result);

                        if(strlen(representation) > 0){
                            strcat(filestring,representation);
                        }

                        strcat(filestring,"\n");
                    } else {
                        strcat(filestring, "[   ] ");

                        strcat(filestring, "Floor ");
                        sprintf(result, "%d: ", floor);
                        strcat(filestring, result);

                        list_for_each(temp, &Passenger_floor.list){
                            if(list_entry(temp, Passenger, list)->start == floor){
                                j++;
                                if(list_entry(temp, Passenger, list)->type){
                                    strcat(representation, "X ");
                                } else {
                                    strcat(representation, "| ");
                                }
                            }
                        }

                        sprintf(result, "%d ", j);
                        strcat(filestring,result);

                        if(strlen(representation) > 0){
                            strcat(filestring,representation);
                        }

                        strcat(filestring,"\n");
                    }
                    i++;
                }

                mutex_unlock(&thread.mutex);            
                kfree(representation);
                read = 1;
                strcat(filestring, "\n(\"|\" for human, \"X\" for zombie)\n\n");
            }
    }

    return 0;
}

// DONE - WORKS
ssize_t elevator_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset) {   
    read = !read;

    switch(read){
        case 1:
            return 0;
            break;
        case 0:
            copy_to_user(buf, filestring, strlen(filestring));
            return strlen(filestring);
        break;
    }
}

// IN PROGRESS - WORKS
int elevator_proc_release(struct inode *sp_inode, struct file *sp_file) {
    kfree(filestring);
    return 0;
}

// IN PROGRESS - WORKS
void thread_init_parameter(struct thread_parameters *parm) {
    mutex_init(&parm->mutex);
    parm->kthread = kthread_run(thread_run, parm, "Elevator thread");
}

extern long (*STUB_start_elevator)(void);
extern long (*STUB_stop_elevator)(void);
extern long (*STUB_issue_request)(int,int,int);
static struct file_operations procfile_handler;
// IN PROGRESS - WORKS
static int elevator_init(void) {
    procfile_handler.open = elevator_proc_open;
    procfile_handler.read = elevator_proc_read;
    procfile_handler.release = elevator_proc_release;

    STUB_start_elevator = start_elevator;
    STUB_issue_request = issue_request;
    STUB_stop_elevator = stop_elevator;
    
    if(proc_create("elevator",0644,NULL,&procfile_handler)){
        thread_init_parameter(&thread);

        if (IS_ERR(thread.kthread)) {
            remove_proc_entry("elevator", NULL);
            printk(KERN_WARNING "Could not initialize thread");
            return PTR_ERR(thread.kthread);
        } else {
            printk(KERN_NOTICE "Elevator module started \n");
            INIT_LIST_HEAD(&Passenger_floor.list);
            INIT_LIST_HEAD(&Passenger_elevator.list);
        }
    } else {
        remove_proc_entry("elevator", NULL);
        printk(KERN_WARNING,"No enough memory for PROC creation.\n");
        return -ENOMEM;
    }

    return 0;
}
module_init(elevator_init);

// IN PROGRESS -> WORKS
static void elevator_exit(void) {
    struct list_head* dummy;
    struct list_head* temp;
    Passenger* p;

    STUB_start_elevator = NULL;
    STUB_issue_request = NULL;
    STUB_stop_elevator = NULL;
    
    kthread_stop(thread.kthread);
    mutex_destroy(&thread.mutex);

    remove_proc_entry("elevator", NULL);

    list_for_each_safe(temp,dummy,&Passenger_elevator.list){
        list_del(temp);
        kfree(list_entry(temp,Passenger,list));
    }

    list_for_each_safe(temp,dummy,&Passenger_floor.list){
        list_del(temp);
        kfree(list_entry(temp,Passenger,list));
    }

    printk(KERN_NOTICE "Removed /proc/elevator module\n");
}
module_exit(elevator_exit);

MODULE_LICENSE("Dual BSD/GPL");