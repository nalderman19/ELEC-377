/*+
 * lab1.c - Print process information
-*/

// Import all the correct packages
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

int isProcessDir(const struct dirent*d){
    // This function takes in a dirent type struct
    // and checks and returns true if its name only
    // digits else it will return false
	char *temp = d->d_name;
    while(*temp != '\0'){
		if (!(isdigit(*temp))){
			return 0;
		}
		temp++;
	}	
	return 1;
}

void main(){
    // Initializing all the variables
	struct dirent ** namelist;
	int n,i;
	FILE *fp;
	char buffer [50], fileBuffer [50], name [50], status [50], number [50], user [50], group [50];
	char *ptr;
	// This gets the number of files in the /proc directory 
	// It also stores the process dirents in namelist
	n = scandir("/proc", &namelist, isProcessDir, NULL);
	// Print the description
	printf("The currently running processes are:\n");
	// Print the header
	printf("#\tName\t\tStatus\t\tUser\tGroup\n");
    // Iterate through each process
    for (i = 0; i < n; i++) {
        // This creates a file name string based off the process
        sprintf(buffer, "/proc/%s/status",namelist[i]->d_name);
        // This reads a file
        fp = fopen (buffer, "r");
        // This iterates through each line of the file
        while(fgets(fileBuffer, 50, fp)) {
            // We are looking for lines that give us
            // the information we are looking for
            if (strstr(fileBuffer, "Name:")){
                // This would copy the current line into a 
                // string buffer with the correct offset
                // to get the actual value from the line
                strcpy(name, fileBuffer+6);
                // This if statement gets a ptr to a point 
                // where we want to cut the string
                if ((ptr=strchr(name, '\n')) != NULL)
                    // We replace cutting point with
                    // the correct number of tabs depending 
                    // on the string length and then append the
                    // null charater to complete the string
                    if (strlen(name) >= 8){
                        *ptr++ = '\t';
                        *ptr = '\0';
                    }else{
                        *ptr++ = '\t';
                        *ptr++ = '\t';
                        *ptr = '\0';
                    }
            }else if (strstr(fileBuffer, "State:")){
                strcpy(status, fileBuffer+7);
                if ((ptr=strchr(status, '\n')) != NULL)
                    *ptr = '\0';
            }else if (strstr(fileBuffer, "Pid:") && !strstr(fileBuffer, "PPid:") && !strstr(fileBuffer, "TracerPid:")){
                strcpy(number, fileBuffer+5);
                if ((ptr=strchr(number, '\n')) != NULL)
                    *ptr = '\0';
            }else if (strstr(fileBuffer, "Uid:")){
                strcpy(user, fileBuffer+5);
                if ((ptr=strchr(user, '\t')) != NULL)
                    *ptr = '\0';
            }else if (strstr(fileBuffer, "Gid:")){
                strcpy(group, fileBuffer+5);
                if ((ptr=strchr(group, '\t')) != NULL)
                    *ptr = '\0';
            }
        }
        // This prints the string with the info on each of the process
        printf("%s\t%s%s\t%s\t%s\n",number, name, status, user, group);
	}
}
