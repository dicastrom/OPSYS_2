// LIZ'S & DIEGO OPSYS PROJ 2 PART 3 CODE
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
	struct list_head list;
	int destination;
} Passenger;

static Passenger floor;
static Passenger elevator;

static struct _elevator{
	char* state;
    int wait;
	int attended;
    int active;
	int passengers;
	int infected;
    int current_floor;
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

	Passenger* item;
	Passenger* newitem;

	while (!kthread_should_stop()) {
		if (mutex_lock_interruptible(&parm->mutex) == 0) { 
			if( (Elevator.active && Elevator.wait > 0) || Elevator.passengers > 0){
                switch(Elevator.passengers){
                    case 0:
                        ssleep(2);
                        if(list_first_entry(&floor.list, Passenger, list)->start > Elevator.current_floor){
                        	Elevator.current_floor++;
                            Elevator.state = "UP";
                        }else if(list_first_entry(&floor.list, Passenger, list)->start < Elevator.current_floor){
                        	Elevator.state = "DOWN";
                        	Elevator.current_floor--;
                        }
                        break;
                    default:
                        ssleep(2);
                        printk(KERN_NOTICE "First passenger in the elevator wants to go to floor %d",list_first_entry(&elevator.list, Passenger, list)->destination);
                        
                        if(list_first_entry(&elevator.list, Passenger, list)->destination > Elevator.current_floor){
                        	Elevator.current_floor++;
                            Elevator.state = "UP";
                        }else if(list_first_entry(&elevator.list, Passenger, list)->destination < Elevator.current_floor){
                            Elevator.current_floor--;
                            Elevator.state = "DOWN";
                        }
                        break;
                }
				
                if(Elevator.passengers > 0){
					list_for_each_safe(temp,dummy,&elevator.list){
						if(list_entry(temp, Passenger, list)->destination == Elevator.current_floor){
							Elevator.state = "LOADING";
							Elevator.passengers--;
							Elevator.attended++;
							
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
                        if(Elevator.wait > 0){
                            ssleep(1);
                            list_for_each_safe(temp,dummy,&floor.list){
                                int num = Elevator.passengers + 1;
                                item = list_entry(temp, Passenger, list);
                                
                                switch(num){
                                    case 10:
                                        if(item->start == Elevator.current_floor){
                                            if(item->type == Elevator.infected){
                                                Elevator.state = "LOADING";

                                                newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                if(newitem == NULL){
                                                    return -ENOMEM;
                                                }
                                                
                                                newitem->start = item->start;
                                                newitem->type = item->type;
                                                newitem->destination = item->destination;
                                                
                                                list_add_tail(&newitem->list,&elevator.list);
                                                
                                                Elevator.passengers++;
                                                Elevator.wait--;

                                                if(item->type){
                                                    switch (Elevator.infected){
                                                        case 0:
                                                            Elevator.infected = item->type;
                                                            break;
                                                    }
                                                }
                                                
                                                list_del(temp);
                                                kfree(item);
                                            } else if(item->type){
                                                Elevator.state = "LOADING";

                                                newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                if(newitem == NULL){
                                                    return -ENOMEM;
                                                }
                                                
                                                newitem->start = item->start;
                                                newitem->type = item->type;
                                                newitem->destination = item->destination;
                                                
                                                list_add_tail(&newitem->list,&elevator.list);
                                                
                                                Elevator.passengers++;
                                                Elevator.wait--;

                                                if(item->type){
                                                    switch (Elevator.infected){
                                                        case 0:
                                                            Elevator.infected = item->type;
                                                            break;
                                                    }
                                                }

                                                list_del(temp);
                                                kfree(item);
                                            } 
                                        }
                                        break;
                                    default: 
                                        if(item->start == Elevator.current_floor){
                                            if(num < 10){
                                                if(item->type == Elevator.infected){
                                                    Elevator.state = "LOADING";

                                                    newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                    if(newitem == NULL){
                                                        return -ENOMEM;
                                                    }
                                                    
                                                    newitem->start = item->start;
                                                    newitem->type = item->type;
                                                    newitem->destination = item->destination;
                                                    
                                                    list_add_tail(&newitem->list,&elevator.list);
                                                    
                                                    Elevator.passengers++;
                                                    Elevator.wait--;

                                                    if(item->type){
                                                        switch (Elevator.infected){
                                                            case 0:
                                                                Elevator.infected = item->type;
                                                                break;
                                                        }
                                                    }

                                                    list_del(temp);
                                                    kfree(item);
                                                } else if(item->type){
                                                    Elevator.state = "LOADING";

                                                    newitem = kmalloc(sizeof(Passenger), __GFP_RECLAIM);

                                                    if(newitem == NULL){
                                                        return -ENOMEM;
                                                    }
                                                    
                                                    newitem->start = item->start;
                                                    newitem->type = item->type;
                                                    newitem->destination = item->destination;
                                                    
                                                    list_add_tail(&newitem->list,&elevator.list);
                                                    
                                                    Elevator.passengers++;
                                                    Elevator.wait--;

                                                    if(item->type){
                                                        switch (Elevator.infected){
                                                            case 0:
                                                                Elevator.infected = item->type;
                                                                break;
                                                        }
                                                    }

                                                    list_del(temp);
                                                    kfree(item);
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
		mutex_unlock(&parm->mutex);
	}

	return 0;
}

// IN PROGRESS - WORKS
long start_elevator(void){
    int same = strcmp(Elevator.state,"OFFLINE");

    switch(same){
        case 0:
            Elevator.state = "IDLE";
            Elevator.current_floor = 1;
            Elevator.passengers = 0;
            Elevator.passengers = 0;
            Elevator.wait = 0;
            Elevator.attended = 0;
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
                Elevator.attended = 0;
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

// DONE - WORKS
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
        Elevator.wait += 1;
        
        list_add_tail(&temp->list, &floor.list);
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
	Passenger* item;

    if(representation == ""){
        return -ENOMEM;
    } else if(representation == NULL){
        return -ENOMEM;
        } else {
            if(filestring == NULL){
                return -ENOMEM;
            } else {

                sprintf(filestring, "Elevator state: %s\n", Elevator.state);

                char* result = kmalloc(sizeof(char) * 1000, __GFP_IO | __GFP_RECLAIM | __GFP_FS);

                strcat(filestring, "Elevator state: ");
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
                sprintf(result, "%d", Elevator.current_floor);
                strcat(filestring, result);
                strcat(filestring, "\n");
                strcat(filestring, "Number of passengers: ");
                sprintf(result, "%d", Elevator.passengers);
                strcat(filestring, result);
                strcat(filestring, "\n");
                strcat(filestring, "Number of passengers wait: ");
                sprintf(result, "%d", Elevator.wait);
                strcat(filestring, result);
                strcat(filestring, "\n");
                strcat(filestring, "Number of passengers attended: ");
                sprintf(result, "%d", Elevator.attended);
                strcat(filestring, result);
                strcat(filestring, "\n\n");

                int floor = 0;
                while(i < 10){
                    j=0;
                    floor = 10 - i;
                    memset(representation, 0, strlen(representation));

                    if(Elevator.current_floor == floor){
                        strcat(filestring, "[ * ] ");

                        strcat(filestring, "Floor ");
                        sprintf(result, "%d: ", floor);
                        strcat(filestring, result);

                        list_for_each(temp, &floor.list){
                            item = list_entry(temp, Passenger, list);
                            if(item->start == floor){
                                j++;
                                if(item->type){
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

                        list_for_each(temp, &floor.list){
                            item = list_entry(temp, Passenger, list);
                            if(item->start == floor){
                                j++;
                                if(item->type){
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
            INIT_LIST_HEAD(&floor.list);
            INIT_LIST_HEAD(&elevator.list);
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

	list_for_each_safe(temp,dummy,&elevator.list){
		p = list_entry(temp,Passenger,list);
		list_del(temp);
		kfree(p);
	}

	list_for_each_safe(temp,dummy,&floor.list){
		p = list_entry(temp,Passenger,list);
		list_del(temp);
		kfree(p);
	}

    printk(KERN_NOTICE "Removed /proc/elevator module\n");
}
module_exit(elevator_exit);

MODULE_LICENSE("Dual BSD/GPL");
