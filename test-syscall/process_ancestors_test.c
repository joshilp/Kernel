#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "process_ancestors.h"

// Sys-call number:
#define _PROCESS_ANCESTORS_ 342

void printinfo(struct process_info *info, long size) {
	struct process_info process;
	for (int i=0; i<size; i++){
		process = info[i];
		printf("info->pid: %ld\n", process.pid);
		printf("info->name: %s\n", process.name);
		printf("info->state: %ld\n", process.state);
		printf("info->uid: %ld\n", process.uid);
		printf("info->nvcsw: %ld\n", process.nvcsw);
		printf("info->nivcsw: %ld\n", process.nivcsw);
		printf("info->num_children: %ld\n", process.num_children);
		printf("info->num_siblings: %ld\n", process.num_siblings);
		printf("\n");	
	}
}

int main(int argc, char const *argv[])
{
	long size = 5;
	long num;
	struct process_info *info = malloc(sizeof(struct process_info)*size);
	printf("\nDiving to kernel level\n\n");

	int result = syscall(_PROCESS_ANCESTORS_, info,size,&num);

	printinfo(info, num);
	//printf("test num is %ld\n",num);

	printf("\nRising to user level w/ result = %d\n\n", result);

	free (info);

	return 0;
}