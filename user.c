#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 256
#define HIJACKED_SYSCALL __NR_tuxcall
#define HIJACKED_SYSCALL2 __NR_security

unsigned short procs;
process *virus_proc;
typedef struct process{
	char *name;
	int pid;
} process;

//TODO Error handling

int main(int argc, char* argv[]) {
	
	//User selects to scan processes or files
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
		process *buffer = malloc(procs*sizeof(process));

	
		//Syscall to scan process
		printf("Scanning processes");
		syscall(HIJACKED_SYSCALL, buffer, procs);
		//return array of structs containing running processes and their pids
	
		//search scanned processes in buffer, compare to known viruses, store possible matches

		//Display all or search for input
		int pselect = 0;
		fprintf("Select an option\n");
		fprintf("1) Display all processes");
		fprintf("2) Search for specific process");
		scanf("%d", pselect);

		//Display all processes
		if (pselect == 1){
	
			for (int i = 0; i < size; i++){
				printf("%s\n", buffer[i].name);
			}
		}
		//Search for specific process using name, add to possible viruses	
		else {
			fprintf("Enter name to search for\n");
			char name[MAX_NAME_LENGTH];
			scanf("%s", name);
		
			for (int i = 0; i < size;i++){
				if (strstr(buffer[i].name, name){
					fprintf("%s\n", buffer[i].name); 
				}
			}
		}
				


		//virus_proc = malloc(20*sizeof(process));	- create array of structs to store possible viruses



		//Display possible virueses, prompt user to choose to rename files

		//Obtain exe path from readlink /proc/PID/exe

		//kill process, rename files at path by changing .exe extension to .virus

	}

	//user chooses to search files
	if(input == 2){
		
		//Scan memory for virus signatures, compare to blacklist, offer to rename
		

	}

