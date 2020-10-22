// LIZ'S & DIEGO'S CODE FOR OPSYS PROJ 2 PART 3
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

static Passenger pfloor;
static Passenger elev;

static struct _elevator{
    char* state;
    int wait;
    int serviced;
    int active;
    int passengers;
    int infected;
    int current_floor;
} Elevator;

struct thread_parameters {
    struct mutex mutex;
    struct task_struct *kthread;
};

int thread_run(void *data) {
    struct thread_parameters *tp = data;

    struct list_head* rep;
    struct list_head* mock;

    Passenger* npass;

    while (!kthread_should_stop()) {
        if (!mutex_lock_interruptible(&tp->mutex)) { 
            if( Elevator.passengers > 0 || (Elevator.active && Elevator.wait > 0)){
                switch(Elevator.passengers){
                    case 0:
                        ssleep(2);
                        if(list_first_entry(&pfloor.list, Passenger, list)->start > Elevator.current_floor){
                            Elevator.current_floor++;
                            Elevator.state = "UP";
                        }else if(list_first_entry(&pfloor.list, Passenger, list)->start < Elevator.current_floor){
                            Elevator.state = "DOWN";
                            Elevator.current_floor--;
                        }
                        break;
                    default:
                        ssleep(2);
                        printk(KERN_NOTICE "First passenger in the elevator wants to go to floor %d",list_first_entry(&elev.list, Passenger, list)->destination);
                        
                        if(list_first_entry(&elev.list, Passenger, list)->destination > Elevator.current_floor){
                            Elevator.current_floor++;
                            Elevator.state = "UP";
                        }else if(list_first_entry(&elev.list, Passenger, list)->destination < Elevator.current_floor){
                            Elevator.current_floor--;
                            Elevator.state = "DOWN";
                        }
                        break;
                }
                
                if(Elevator.passengers > 0){
                    list_for_each_safe(rep,mock,&elev.list){
                        if(list_entry(rep, Passenger, list)->destination == Elevator.current_floor){
                            Elevator.state = "LOADING";
                            Elevator.passengers--;
                            Elevator.serviced++;
                            
                            switch (Elevator.passengers){
                                case 0:
                                    Elevator.infected = 0;
                                    break;
                            }

                            list_del(rep);
                            kfree(list_entry(rep, Passenger, list));
                        }
                    }
                }

                switch(Elevator.active){
                    case 1:
                        if(Elevator.wait > 0){
                            ssleep(1);
                            list_for_each_safe(rep,mock,&pfloor.list){
                                int num = Elevator.passengers + 1;
                                
                                switch(num){
                                    case 10:
                                        if(list_entry(rep, Passenger, list)->start == Elevator.current_floor){
                                            if(list_entry(rep, Passenger, list)->type == Elevator.infected){
                                                Elevator.state = "LOADING";

                                                npass = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                if(npass == NULL){
                                                    return -ENOMEM;
                                                }
                                                
                                                npass->start = list_entry(rep, Passenger, list)->start;
                                                npass->type = list_entry(rep, Passenger, list)->type;
                                                npass->destination = list_entry(rep, Passenger, list)->destination;
                                                
                                                list_add_tail(&npass->list,&elev.list);
                                                
                                                Elevator.passengers++;
                                                Elevator.wait--;

                                                if(list_entry(rep, Passenger, list)->type){
                                                    switch (Elevator.infected){
                                                        case 0:
                                                            Elevator.infected = list_entry(rep, Passenger, list)->type;
                                                            break;
                                                    }
                                                }
                                                
                                                list_del(rep);
                                                kfree(list_entry(rep, Passenger, list));
                                            } else if(list_entry(rep, Passenger, list)->type){
                                                Elevator.state = "LOADING";

                                                npass = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                if(npass == NULL){
                                                    return -ENOMEM;
                                                }
                                                
                                                npass->start = list_entry(rep, Passenger, list)->start;
                                                npass->type = list_entry(rep, Passenger, list)->type;
                                                npass->destination = list_entry(rep, Passenger, list)->destination;
                                                
                                                list_add_tail(&npass->list,&elev.list);
                                                
                                                Elevator.passengers++;
                                                Elevator.wait--;

                                                if(list_entry(rep, Passenger, list)->type){
                                                    switch (Elevator.infected){
                                                        case 0:
                                                            Elevator.infected = list_entry(rep, Passenger, list)->type;
                                                            break;
                                                    }
                                                }

                                                list_del(rep);
                                                kfree(list_entry(rep, Passenger, list));
                                            } 
                                        }
                                        break;
                                    default: 
                                        if(list_entry(rep, Passenger, list)->start == Elevator.current_floor){
                                            if(num < 10){
                                                if(list_entry(rep, Passenger, list)->type == Elevator.infected){
                                                    Elevator.state = "LOADING";

                                                    npass = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                    if(npass == NULL){
                                                        return -ENOMEM;
                                                    }
                                                    
                                                    npass->start = list_entry(rep, Passenger, list)->start;
                                                    npass->type = list_entry(rep, Passenger, list)->type;
                                                    npass->destination = list_entry(rep, Passenger, list)->destination;
                                                    
                                                    list_add_tail(&npass->list,&elev.list);
                                                    
                                                    Elevator.passengers++;
                                                    Elevator.wait--;

                                                    if(list_entry(rep, Passenger, list)->type){
                                                        switch (Elevator.infected){
                                                            case 0:
                                                                Elevator.infected = list_entry(rep, Passenger, list)->type;
                                                                break;
                                                        }
                                                    }

                                                    list_del(rep);
                                                    kfree(list_entry(rep, Passenger, list));
                                                } else if(list_entry(rep, Passenger, list)->type){
                                                    Elevator.state = "LOADING";

                                                    npass = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                    if(npass == NULL){
                                                        return -ENOMEM;
                                                    }
                                                    
                                                    npass->start = list_entry(rep, Passenger, list)->start;
                                                    npass->type = list_entry(rep, Passenger, list)->type;
                                                    npass->destination = list_entry(rep, Passenger, list)->destination;
                                                    
                                                    list_add_tail(&npass->list,&elev.list);
                                                    
                                                    Elevator.passengers++;
                                                    Elevator.wait--;

                                                    if(list_entry(rep, Passenger, list)->type){
                                                        switch (Elevator.infected){
                                                            case 0:
                                                                Elevator.infected = list_entry(rep, Passenger, list)->type;
                                                                break;
                                                        }
                                                    }

                                                    list_del(rep);
                                                    kfree(list_entry(rep, Passenger, list));
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
                            switch (Elevator.wait){
                                case 0:
                                    Elevator.state = "IDLE";
                                    break;
                                default:
                                    switch(Elevator.active){
                                        case 0:
                                            Elevator.state = "OFFLINE";
                                            break;
                                    }
                                    break;
                            }
                        } else {
                            if(Elevator.active == 0){
                                Elevator.state = "OFFLINE";
                            }
                        }
                        break;
                }
            }
            
        }
        mutex_unlock(&tp->mutex);
    }

    return 0;
}

long start_elevator(void){
    int same = strcmp(Elevator.state,"OFFLINE");

    switch(same){
        case 0:
            Elevator.state = "IDLE";
            Elevator.current_floor = 1;
            Elevator.passengers = 0;
            Elevator.passengers = 0;
            Elevator.wait = 0;
            Elevator.serviced = 0;
            Elevator.active = 1;
            Elevator.infected = 0;
            printk(KERN_NOTICE "Elevator initialized. Status: %s\n", Elevator.state);
            return 0;
            break;
        default:
            if(Elevator.state == NULL){
                Elevator.state = "IDLE";
                Elevator.current_floor = 1;
                Elevator.passengers = 0;
                Elevator.passengers = 0;
                Elevator.wait = 0;
                Elevator.serviced = 0;
                Elevator.active = 1;
                Elevator.infected = 0;
                printk(KERN_NOTICE "Elevator initialized. Status: %s\n", Elevator.state);
                return 0;
            } else {
                return 1;
            }
            break;
    }
}

long stop_elevator(void){
    int same = strcmp("OFFLINE", Elevator.state);

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

long issue_request(int start_floor, int dest_floor, int type){
    Passenger* rep = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

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
    
    if(rep != NULL){
        rep->type = type;
        printk("Loaded a %d passenger.\n",rep->type);

        rep->start = start_floor;
        rep->destination = dest_floor;
        Elevator.wait += 1;
        
        list_add_tail(&rep->list, &pfloor.list);
    } else {
        return -ENOMEM;
    }

    return 0;
}

static int read;
struct thread_parameters t;
static char* file;
int elevator_proc_open(struct inode *sp_inode, struct file *sp_file) {
    char* representation = kmalloc(sizeof(char) * 1000, __GFP_FS | __GFP_RECLAIM | __GFP_IO);
    file = kmalloc(sizeof(char) * 1000, __GFP_IO | __GFP_RECLAIM | __GFP_FS);

    if(representation == ""){
        return -ENOMEM;
    } else if(representation == NULL){
        return -ENOMEM;
        } else {
            if(file == NULL){
                return -ENOMEM;
            } else {

                sprintf(file, "Elevator state: %s\n", Elevator.state);

                char* result = kmalloc(sizeof(char) * 1000, __GFP_IO | __GFP_RECLAIM | __GFP_FS);

                strcat(file, "Elevator status: ");
                switch(Elevator.infected){
                    case 1:
                        strcat(file, "Infected\n");
                        break;
                    case 0:
                        strcat(file, "Not infected\n");
                        break;
                }
                
                struct list_head* rep;
                int j = 0;
                int i = 0;
                int length = 0;

                strcat(file, "Current floor: ");
                sprintf(result, "%d", Elevator.current_floor);
                strcat(file, result);
                strcat(file, "\n");
                strcat(file, "Number of passengers: ");
                sprintf(result, "%d", Elevator.passengers);
                strcat(file, result);
                strcat(file, "\n");
                strcat(file, "Number of passengers waiting: ");
                sprintf(result, "%d", Elevator.wait);
                strcat(file, result);
                strcat(file, "\n");
                strcat(file, "Number of passengers serviced: ");
                sprintf(result, "%d", Elevator.serviced);
                strcat(file, result);
                strcat(file, "\n\n");

                int floor = 0;
                while(i < 10){
                    j=0;
                    floor = 10 - i;
                    memset(representation, 0, strlen(representation));

                    if(Elevator.current_floor == floor){
                        strcat(file, "[ * ] ");

                        strcat(file, "Floor ");
                        sprintf(result, "%d: ", floor);
                        strcat(file, result);

                        list_for_each(rep, &pfloor.list){
                            if(list_entry(rep, Passenger, list)->start == floor){
                                j++;
                                if(list_entry(rep, Passenger, list)->type){
                                    strcat(representation, "X ");
                                } else {
                                    strcat(representation, "| ");
                                }
                            }
                        }

                        sprintf(result, "%d ", j);
                        strcat(file,result);

                        if(strlen(representation) > 0){
                            strcat(file,representation);
                        }

                        strcat(file,"\n");
                    } else {
                        strcat(file, "[   ] ");

                        strcat(file, "Floor ");
                        sprintf(result, "%d: ", floor);
                        strcat(file, result);

                        list_for_each(rep, &pfloor.list){
                            if(list_entry(rep, Passenger, list)->start == floor){
                                j++;
                                if(list_entry(rep, Passenger, list)->type){
                                    strcat(representation, "X ");
                                } else {
                                    strcat(representation, "| ");
                                }
                            }
                        }

                        sprintf(result, "%d ", j);
                        strcat(file,result);

                        if(strlen(representation) > 0){
                            strcat(file,representation);
                        }

                        strcat(file,"\n");
                    }
                    i++;
                }

                mutex_unlock(&t.mutex);            
                kfree(representation);
                read = 1;
                strcat(file, "\n(\"|\" for human, \"X\" for zombie)\n\n");
            }
    }

    return 0;
}

ssize_t elevator_proc_read(struct file *sp_file, char __user *result, size_t size, loff_t *offset) {   
    read = !read;

    switch(read){
        case 1:
            return 0;
            break;
        case 0:
            copy_to_user(result, file, strlen(file));
            return strlen(file);
        break;
    }
}

int elevator_proc_release(struct inode *sp_inode, struct file *sp_file) {
    kfree(file);
    return 0;
}

void thread_init_parameter(struct thread_parameters *tp) {
    mutex_init(&tp->mutex);
    tp->kthread = kthread_run(thread_run, tp, "Elevator thread");
}

extern long (*STUB_start_elevator)(void);
extern long (*STUB_stop_elevator)(void);
extern long (*STUB_issue_request)(int,int,int);
static struct file_operations procfile_handler;
static int elevator_init(void) {
    procfile_handler.open = elevator_proc_open;
    procfile_handler.read = elevator_proc_read;
    procfile_handler.release = elevator_proc_release;

    STUB_start_elevator = start_elevator;
    STUB_issue_request = issue_request;
    STUB_stop_elevator = stop_elevator;
    
    if(proc_create("elevator",0644,NULL,&procfile_handler)){
        thread_init_parameter(&t);

        if (IS_ERR(t.kthread)) {
            remove_proc_entry("elevator", NULL);
            printk(KERN_WARNING "Could not initialize thread");
            return PTR_ERR(t.kthread);
        } else {
            printk(KERN_NOTICE "Elevator module started \n");
            INIT_LIST_HEAD(&pfloor.list);
            INIT_LIST_HEAD(&elev.list);
        }
    } else {
        remove_proc_entry("elevator", NULL);
        printk(KERN_WARNING,"No enough memory for PROC creation.\n");
        return -ENOMEM;
    }

    return 0;
}
module_init(elevator_init);

static void elevator_exit(void) {
    struct list_head* mock;
    struct list_head* rep;
    Passenger* p;

    STUB_start_elevator = NULL;
    STUB_issue_request = NULL;
    STUB_stop_elevator = NULL;
    
    kthread_stop(t.kthread);
    mutex_destroy(&t.mutex);

    remove_proc_entry("elevator", NULL);

    list_for_each_safe(rep,mock,&elev.list){
        list_del(rep);
        kfree(list_entry(rep,Passenger,list));
    }

    list_for_each_safe(rep,mock,&pfloor.list){
        list_del(rep);
        kfree(list_entry(rep,Passenger,list));
    }

    printk(KERN_NOTICE "Removed /proc/elevator module\n");
}
module_exit(elevator_exit);

MODULE_LICENSE("Dual BSD/GPL");