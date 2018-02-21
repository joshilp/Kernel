#include <linux/sched.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/unistd.h>
#include <linux/cred.h>
#include "process_ancestors.h"

// info_array[]: An array of process_info structs that will be written to by the kernel
// as it fills in information from the current process on up through its ancestors.
// size: The number of structs in info_array[]. This is the maximum number of structs
// that the kernel will write into the array (starting with the current process as the first
// entry and working up from there). The size may be larger or smaller than the actual
// number of ancestors of the current process: larger means some entries are left unused
// (see num_filled); smaller means information about some processes not written into the
// array.
// num_filled: A pointer to a long. To this location the kernel will store the number of
// structs (in info_array[]) which are written by the kernel. May be less than size if the
// number of ancestors for the current process is less than size.

asmlinkage long sys_process_ancestors(struct process_info info_array[],long size,long *num_filled){

    struct task_struct *info;  // get info for each array element
    struct process_info process; // each array element
    long children=0; // number of children
    long sibling=0; // number of sibling
    long i=0; // index
    struct list_head *childPointer; // kernel link list for child
    struct list_head *siblingPointer; // kernel link list for sibling
    struct task_struct *child_task; 
    struct task_struct *sibling_task;
    if (size <= 0){
      return -EINVAL;
    }
    if (info_array == NULL){
      return -EINVAL;
    }

    info = current;
    // printk("info->pid: %ld\n", (long) info->pid);
    // printk("info->name: %s\n", info->comm);
    // printk("info->state: %ld\n", info->state);
    // printk("info->uid: %ld\n", (long)info->cred->uid.val);
    // printk("info->nvcsw: %ld\n", info->nvcsw);
    // printk("info->nivcsw: %ld\n", info->nivcsw);


    // childPointer = &info->children;

    // printk("pointer is %p\n", childPointer);

    // list_for_each(childPointer, &info->children) {
    //   //printk("pointer is %p\n",childPointer);
    //   child_task = list_entry(childPointer,struct task_struct,children);
    //   children++;
    //   //printk("child is %ld\n",children);
    // }

    // printk("children is %ld\n", children);

    // list_for_each(siblingPointer, &info->sibling) {
    //   sibling_task = list_entry(siblingPointer,struct task_struct,sibling);
    //   sibling++;
    //   //printk("sibling is %ld\n",sibling);
    // }
    // printk(" sbling is %ld\n", sibling);
    // list_for_each(childPointer, &info->children) {
    //printk("pointer is %p\n",childPointer);
    //   child_task = list_entry(childPointer,struct task_struct,sibling);
    //   children++;
    //   //printk("child is %ld\n",children);
    // }

    while(info->parent != info){

      process.pid = info->pid; // get the process id

      //printk("1.pid is ok\n");
     
      //memset(process.name, '\0', sizeof(process.name));

      strncpy(process.name, info->comm, 16); // copy the process name
      //printk("2.name is ok\n");

      process.state = info->state; // get the process state
      //printk("3.state is ok\n");

      process.uid = info->cred->uid.val; // get the uid
      //printk("4.uid is ok\n");


      process.nvcsw = info->nvcsw;

      process.nivcsw = info->nivcsw;

      //printk("5.nv is ok\n");

      // count the child process
      list_for_each(childPointer, &info->children) {
        //printk("pointer is %p\n",childPointer);
        child_task = list_entry(childPointer,struct task_struct,sibling);
        children++;
        //printk("child is %ld\n",children);
      }

      // count the sibling process
      list_for_each(siblingPointer, &info->sibling) {
        sibling_task = list_entry(siblingPointer,struct task_struct,sibling);
        sibling++;
        //printk("sibling is %ld\n",sibling);
      }

      process.num_children = children;
      process.num_siblings = sibling;

      // printk("children2 is %ld\n",children);
      // printk("sbling2 is %ld\n",sibling);

      //info_array[i] = process; // assign the value back to array

      if (!access_ok(VERIFY_WRITE, &info_array[i], sizeof(process))){
          return -EFAULT;
      }

      if (copy_to_user(&info_array[i], &process, sizeof(process)  ) != 0){
          return -EINVAL;       
      }



      //printk("get here\n");
      i++;
      // printk("info->num_children: %ld\n", children);
      // printk("info->num_siblings: %ld\n", sibling);  

      info = info->parent; // go to the parent process
    } 


    *num_filled = i; // the number of filled is iteration

    //printk("num is %ld\n",*num_filled);

    // printk("after while loop\n");

  return 0;
}