#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define HIJACKED_SYSCALL __NR_tuxcall
#define HIJACKED_SYSCALL2 __NR_security

unsigned short procs;
process *virus_proc;
struct process{
	char *name;
	int pid;
}

int main(int argc, char* argv[]) {

	//Obtain number of running processes
	struct sysinfo info;
	sysinfo (&info);
	procs = info.procs;
	process *buffer = malloc(procs*sizeof(process));
	
	//User selects function

	if(input == 1){
	
		//Syscall to scan process
		printf("Scanning processes");
		syscall(HIJACKED_SYSCALL, buffer, procs);
	
		//Parse buffer, place matches to virus file in virus_proc

		//User prompt to rename files

		//rename files

	}

	if(input == 2){
		
		//Scan memory for virus patterns

	}

