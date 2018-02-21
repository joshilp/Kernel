#include <linux/kernel.h>
#include <linux/uaccess.h>
#include "array_stats.h"

// stats: A pointer to one array_stats structure allocated by the user-space application.
// Structure will be written to by the sys-call to store the minimum, maximum, and sum of all
// values in the array pointed to by data.
// data: An array of long int values passed in by the user-space application.
// size: The number of elements in data. Must be > 0.

asmlinkage long sys_array_stats(struct array_stats *stats,long data[],long size)
{
	long test; // variable for data array elements
	long i; // index
	struct array_stats kernel_stats; // kernel stats structure

	if (size <= 0){
		return -EINVAL;
	}

	for (i=0;i<size;i++){
		//  have problems access stats or data.
		if (copy_from_user(&test, &data[i], sizeof(data[i])) != 0){
			return -EFAULT;
		}
		// initialize kernel stats
		if (i == 0)
		{
			kernel_stats.min = test;
			kernel_stats.max = test;
			kernel_stats.sum = 0;
		}
		// find the min
		if (test < kernel_stats.min){
			kernel_stats.min = test;
		}
		// find the max
		if (test > kernel_stats.max){
			kernel_stats.max = test;
		}
		// sum
		kernel_stats.sum += test;
	}

	// copy back to user
	if (copy_to_user(stats,&kernel_stats,sizeof(kernel_stats)) != 0){
		return -EFAULT;
	}


	// successful, return 0
	return 0;
}

