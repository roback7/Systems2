#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h> 

#define MAX_NAME_LENGTH 256
#define HIJACKED_SYSCALL __NR_tuxcall

int sizeProc[1];

int files = 0;
unsigned short procs;
typedef struct process{
	char *name;
	int pid;
} process;

char* get_name(int pid);

int main(int argc, char* argv[]) {
	
	//Known virus signatures
	char  *signatures[13] = {"DD7A0BA8", "1AA1E86F", "9A6231DC", "A5FB507D", "3A76C92", "2523B959", 
				"CC4F1D8D", "8000D3AD", "C8B4593", "6CF483F9", "B6CB4EAF", "F11602A1", "36118429"};

	//User selects function
	int function = 0;
	printf ("Select a function to execute\n");
	printf ("1) Scan Processes\n");
	printf ("2) Scan Files\n");
	scanf ("%d", &function);

	//Scan Processes
	if(function == 1){

		//Obtain number of running processes
		struct sysinfo info;
		sysinfo (&info);

		procs = info.procs;		
		process *all_procs = malloc((unsigned long)procs*sizeof(process));

		for (int i = 0; i < 200; i++){
			all_procs[i].name = malloc(40 * sizeof(char));
		}
		
		printf("Scanning processes\n");



		//Display all or search for input
		int pselect = 0;
		printf("Select an option\n");
		printf("1) Display all processes\n");
		printf("2) Search for a specific process\n");
		scanf("%d", &pselect);

		char name[MAX_NAME_LENGTH];

		//Syscall to scan processes

		int pid[(int)procs];	
		int error = syscall(HIJACKED_SYSCALL, (int)procs, all_procs);
		if (error == 0){
			printf("Error performing system call\n");
			return 0;
		}

		// Find process name for given pid
		int test_pid = all_procs[0].pid;
		int count = 0;
		while (test_pid != NULL) {
			char *searchForName = get_name(all_procs[count].pid);
			char *findSpace;
			int index;

			findSpace = strchr(searchForName, ' ');
			index = (int)(findSpace - searchForName);
			char *pName = malloc(40);
			for (int i = 0; i < index; i++) {
				pName[i] = searchForName[i];
			}
			all_procs[count].name = pName;
			test_pid = all_procs[count+1].pid;
			count++;
	}
		

					
		

		//Print Processes
		if (pselect == 1){
	
			for (int i = 0; i < count; i++){
				printf("%s [%d]\n", all_procs[i].name, all_procs[i].pid);
			}
		}
		//Search through processes
		else if (pselect == 2){
			
			printf("Enter name to search for\n");
			scanf("%s", name);
			for (int i = 0; i < count; i++){

				if (strcmp(all_procs[i].name, name) == 0){

					//Ask to kill process if found
					printf("%s [%d]\n",all_procs[i].name, all_procs[i].pid);
					printf("Process found\n");
					printf("Kill process? (y/n) (%d Processes left to be searched)\n", count-i);
					
					char kselect = 'y';
					scanf(" %c", kselect);

					if (kselect == 'y' || kselect == 'Y'){
						pid_t killP = (pid_t)all_procs[i].pid;

						//Kill Process(es)
						int check = all_procs[i].pid;
						if (check == NULL || check <= 0){
							printf("PID not valid\n");
							return 0;
						}
						error = kill(killP, SIGUSR1);
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
			return 0;

		}
		//free(buffer);

	}

	else if(function == 2){
		
		//Scan memory for virus patterns
		//Open file, parse file
		//User prompt to rename files

		char *directory;
		char *find;
		
		directory = malloc(100*sizeof(char));
		find = malloc(100*sizeof(char));
		int choice = 1;
		printf("Enter the directory to search:\n");
		scanf("%s", directory);

		printf("1) Scan %s for known virus signatures\n", directory);
		printf("2) Input file name or signature to scan for in %s\n", directory);
		scanf("%d", &choice);

		if (choice == 1) {
			for (int i = 0; i < 13; i++) {
				find = signatures[i];
				listdir(directory, find);

			}
		}
		if (choice == 2) {
			printf("Enter the file to search for:\n");
			scanf("%s", find);
			listdir(directory, find);
		}
		if (files == 0){
				printf("File/Signature not found in directory!\n");
			}

return 0;


	}
}

char* get_name(int pid)
{
    char* name = (char*)calloc(1024,sizeof(char));
    if(name){
        sprintf(name, "/proc/%d/sched", pid);
        FILE* f = fopen(name,"r");
        if(f){
            size_t size;
            size = fread(name, sizeof(char), 1024, f);
            if(size>0){
                if('\n'==name[size-1])
                    name[size-1]='\0';
            }
            fclose(f);
        }
    }
    return name;
}

void listdir(char *name, char *find)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			if (strcmp(entry->d_name, find) == 0) {
				 printf("%s found in directory!\n", entry->d_name);
				 files++;
			}
		
            listdir(path, find);
        } else {
			            if (strcmp(entry->d_name, find) == 0) {
				 		printf("%s found in directory!\n", entry->d_name);
						 files++;
			}

		}
    }
    closedir(dir);
}

