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
#define HIJACKED_SYSCALL2 __NR_security

int files = 0;
unsigned short procs;
typedef struct process{
	char *name;
	int pid;
} process;



//TODO Error handling

int main(int argc, char* argv[]) {
	
	//Known virus signatures
	char * signatures[] = {"DD7A0BA8", "1AA1E86F", "9A6231DC", "A5FB507D", "3A76C92", "2523B959", 
				"CC4F1D8D", "8000D3AD", "C8B4593", "6CF483F9", "B6CB4EAF", "F11602A1", "36118429"};

	//User selects function
	int function = 0;
	printf ("Select a function to execute\n");
	printf ("1) Scan Processes\n");
	printf ("2) Scan files\n");
	scanf ("%d", &function);

	//Scan Processes
	if(function == 1){

		//Obtain number of running processes
		struct sysinfo info;
		sysinfo (&info);
		procs = info.procs;
		process *all_procs = malloc(procs*sizeof(process));
		

		printf("Scanning processes");
		int size = sizeof(all_procs)/sizeof(all_procs[0]);


		//Display all or search for input
		int pselect = 0;
		printf("Select an option\n");
		printf("1) Display all processes");
		printf("2) Search for specific process");
		scanf("%d", &pselect);

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
			
			printf("Enter name to search for\n");
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
						int check = all_procs[i].pid;
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
			return 0;

		}
		//free(buffer);

	}

	else if(function == 2){
		
		//Scan memory for virus patterns
		//Open file, parse file
		//User prompt to rename files

		//struct dirent *de;  // Pointer for directory entry
		char *directory;
		char *find;
		
		directory = malloc(100*sizeof(char));
		find = malloc(100*sizeof(char));
		int choice;
		printf("Enter the directory to search:\n");
		scanf("%s", directory);

		printf("1) Scan %s for known virus signatures\n", directory);
		printf("2) Input file name or signature to scan for in %s\n", directory);
		scanf("%d", choice);

		if (choice == 1) {
			for (int i = 0; i < 13; i++) {
				find = signatures[i];
				listdir(directory, find);

			}
		}
		else if (choice == 2) {
			printf("Enter the file to search for:\n");
			scanf("%s", find);
			listdir(directory, find);
		}
		if (files == 0){
				printf("File/Signature not found in directory!");
			}

		

    // opendir() returns a pointer of DIR type.  
    /*DIR *dr = opendir(directory); 

  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 

	int i = 0;
    while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
    closedir(dr);     
    return 0; 
	}
	else {
		printf("Invalid Input");
		return 0;
	} */
	
return 0;


	}
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

