#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/proc_fs.h> 
#include <linux/uaccess.h> 
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/mutex.h>

#define MAX_CAPACITY 100
#define PROCFS_NAME "partb_21CS10064"

struct Set_TYPE {
    int *data;
    int capacity;
    int size;
    uint32_t pid;
    struct Set_TYPE *next;
};

typedef struct Set_TYPE Set;

static DEFINE_MUTEX(sets_lock);
static Set * head = NULL;

//Function to insert a new set
static int insert_set(uint32_t pid) {
    Set *new_set = kmalloc(sizeof(Set), GFP_KERNEL);
    if (!new_set) {
        return -ENOMEM;
    }
    new_set->data = NULL;
    new_set->capacity = 0;
    new_set->size = 0;
    new_set->pid = pid;
    new_set->next = NULL;

    mutex_lock(&sets_lock);
    if (!head) {
        head = new_set;
    } else {
        Set *cur = head;
        while (cur->next) {
            cur = cur->next;
        }
        cur->next = new_set;
    }
    mutex_unlock(&sets_lock);

    pr_info("Set with pid %u inserted\n", pid);
    return 0;
}

//Fuction to find a set, returns NULL if set not found
static Set * find_set(uint32_t pid) {
    Set *cur;
    mutex_lock(&sets_lock);
    cur = head;
    while (cur) {
        if (cur->pid == pid) {
            mutex_unlock(&sets_lock);
            return cur;
        }
        cur = cur->next;
    }
    mutex_unlock(&sets_lock);
    return NULL;
}

//Function to remove a set
static int remove_set(uint32_t pid) {
    Set *cur, *prev;
    mutex_lock(&sets_lock);
    cur = head;
    prev = NULL;
    while (cur) {
        if (cur->pid == pid) {
            if (prev) {
                prev->next = cur->next;
            } else {
                head = cur->next;
            }
            if (cur->data) {
                kfree(cur->data);
            }
            kfree(cur);
            mutex_unlock(&sets_lock);
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    mutex_unlock(&sets_lock);
    return -1;
}

static int partB_open(struct inode *inode, struct file *file) {
    //Getting pid to be used to find the set
    unsigned int set_id = current->pid;

    pr_info("OPEN: Set with pid %u opened\n", set_id);

    Set *cur_set;
    cur_set = find_set(set_id);
    if (!cur_set) { //If set not found, insert a new set
        pr_info("OPEN: Set with pid %u not found, inserting new set\n", set_id);
        insert_set(set_id);
        cur_set = find_set(set_id);
    } else { //If set is found, reset the set
        pr_info("OPEN: Set with pid %u found, resetting the set\n", set_id);
        kfree(cur_set->data);
        cur_set->data = NULL;
        cur_set->capacity = 0;
        cur_set->size = 0;
    }

    file->private_data = cur_set;
    return 0;
}

static int partB_release(struct inode *inode, struct file *file) {
    //Getting pid to be used to free the set
    unsigned int set_id = current->pid;

    int removed_set = remove_set(set_id);
    if (removed_set == 0) {
        pr_info("RELEASE: Set with pid %u released\n", set_id);
    } else {
        pr_info("RELEASE: Set with pid %u not found\n", set_id);
    }

    return 0;
}

static struct proc_ops partb_fops = {
    .proc_read = NULL,
    .proc_write = NULL,
    .proc_open = partB_open,
    .proc_release = partB_release
};

static int __init partb_init(void) {
    struct proc_dir_entry *proc_entry;
    proc_entry = proc_create(PROCFS_NAME, 0666, NULL, &partb_fops);
    if (!proc_entry) {
        return -ENOMEM;
    }

    pr_info("Loadable Kernel Module partb loaded\n");
    return 0;
}

static void __exit partb_exit(void) {
    //Freeing all the sets
    Set *cur = head;
    while (cur) {
        Set *temp = cur;
        cur = cur->next;
        if (temp->data) {
            kfree(temp->data);
        }
        kfree(temp);
    }
    remove_proc_entry(PROCFS_NAME, NULL);
    pr_info("Loadable Kernel Module partb unloaded\n");
}

module_init(partb_init);
module_exit(partb_exit);

MODULE_LICENSE("GPL");