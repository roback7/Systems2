#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define MAX_NAME_LENGTH 256
#define HIJACKED_SYSCALL __NR_tuxcall
#define HIJACKED_SYSCALL2 __NR_security

unsigned short procs;
typedef struct process{
	char *name;
	int pid;
} process;

//TODO Error handling

int main(int argc, char* argv[]) {
	
	//User selects function
	int input = 0;
	printf ("Select a function to execute\n");
	printf ("1) Scan Processes\n");
	printf ("2) Scan files\n");
	scanf ("%d", input);

	//Scan Processes
	if(input == 1){

		//Obtain number of running processes
		struct sysinfo info;
		sysinfo (&info);
		procs = info.procs;
		process *all_procs = malloc(procs*sizeof(process));

		printf("Scanning processes");

		//Display all or search for input
		int pselect = 0;
		fprintf("Select an option\n");
		fprintf("1) Display all processes");
		fprintf("2) Search for specific process");
		scanf("%d", pselect);

		char name[MAX_NAME_LENGTH];

		//Syscall to scan processes
		int error = syscall(HIJACKED_SYSCALL, procs, pselect-1, all_procs);
		if (error){
			printf("Error performing system call");
		}

		//Print Processes
		if (pselect == 1){
	
			for (int i = 0; i < size; i++){
				printf("%s\n", all_procs[i].name);
			}
		}
		//Search through processes
		else if (pselect == 2){
			
			fprintf("Enter name to search for\n");
			scanf("%s", name);

			for (int i = 0; i < size; i++){

				if (strcmp(all_procs[i].name, name) == 0){

					//Ask to kill process if found
					printf("%s\n %d",all_procs[i].name, all_procs[i].pid);
					printf("Process found\n");
					printf("Kill process? (y/n) (%d Processes left to be 							searched)\n", size-i);
					char kselect;
					scanf("%c", kselect);
					if (kselect == 'y' || kselect == 'Y'){

						//Kill Process(es)
						int check = all_procs[id].pid;
						if (check == NULL || check <= 0){
							printf("PID not valid\n");
							return 0;
						}
						error = kill(all_procs[i].pid, SIGTERM);
						if(error){
							printf("Error killing process\n");
							return 0;
						}
						printf("Process with PID %d killed\n", check);
					}
					//Resume loop without killing
					else if (kselect == 'n'|| kselect == 'N'){
						printf("Process not killed\n");
					}
					else {
						printf("Invalid Input");
						return 0;
					}
				}
			}

		}
		else {
			printf("Invalid Input");
			return 0

		}
		free(buffer);

	}

	else if(input == 2){
		
		//Scan memory for virus patterns
		//Open file, parse file
		//User prompt to rename files


	}
	else {
		printf("Invalid Input");
		return 0
	}
	
return 0;


}

