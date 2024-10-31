#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <glob.h>

#define PATH_MAX 100

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int nprocs=5;

struct data{
    int ID;
    char* currDIR;
    int fdFileType;
    int chkFolderDepth;
    char* fdFileOwner;
    char* fdFileName;
};

typedef struct arguments{
    char* root;
    char* type;
    char* fname;
    char* user;
    char* maxdepth;
}arguments;

int globme(const char *root,  const char *fpattern, const char *fname);


// Get type of file and return it as int
// regfile=1, directory=2, symboliclink=3, chardevice=4, block=5
int printType(char* file, char* directoryChange){
    pthread_mutex_lock(&lock);
    struct stat buffer;
    int fileType=0;
    if (chdir(directoryChange) != 0){
        printf("failed to change directory (printType function)\n");
        pthread_mutex_unlock(&lock);
        return fileType;
    }
    // failed to open file
    if (stat(file, &buffer) == -1){
        printf("failed to open file (printType function)\n");
        pthread_mutex_unlock(&lock);
        return fileType;
    }
    // switch (buffer.st_mode & S_IFMT) {
    //     case S_IFBLK:  fileType=5; break;
    //     case S_IFCHR:  fileType=4; break;
    //     case S_IFDIR:  fileType=2; break;
    //     case S_IFIFO:              break;
    //     case S_IFLNK:  fileType=3; break;
    //     case S_IFREG:  fileType=1; break;
    //     case S_IFSOCK:             break;
    //     default:                   break;
    // }

    // printf("%s\n", file);
    stat(file, &buffer);

	if(S_ISLNK(buffer.st_mode) != 0){
		fileType = 3;
	}
	else if(S_ISREG(buffer.st_mode) != 0){
		fileType = 1;
	}
	else if(S_ISDIR(buffer.st_mode) != 0){
		fileType = 2;
	}
	else if(S_ISCHR(buffer.st_mode) != 0){
		fileType = 4;
	}
	else if(S_ISBLK(buffer.st_mode) != 0){
		fileType = 5;
	}
	else if(S_ISFIFO(buffer.st_mode) != 0){
		fileType = 0;
	}
	else{
		fileType = 0;
	}


    pthread_mutex_unlock(&lock);
    return fileType;
}

// convert users inputted filetype to int, easier to work with
// regfile=1, directory=2, symboliclink=3, chardevice=4, block=5
int convertUserFileType(char* fdFileType){
    int fileType;
    if (strcmp(fdFileType, "f") == 0){
        fileType=1;
    }
    else if (strcmp(fdFileType, "d") == 0){
        fileType=2;
    }
    else if (strcmp(fdFileType, "s") == 0){
        fileType=3;
    }
    else if (strcmp(fdFileType, "c") == 0){
        fileType=4;
    }
    else if (strcmp(fdFileType, "b") == 0){
        fileType=5;
    }
    else{
        fileType=0;
    }
    return fileType;
}

// check if we have a match for file owner, no match=0, match=1, error=2
//
int checkOwnerID(char* file, char* matchUser){
    struct stat buffer;
    struct passwd *pw;
    int match=0; // default is not a match
    // error
    if (stat(file, &buffer) == -1){
        match=2;
        return match;
    }

    // get file ownername, then compare
    pw = getpwuid(buffer.st_uid);
    if (!strcmp(pw->pw_name, matchUser)){
        match=1;
    }

    // printf("checkOwnerID - File name: %s\n", file);
    // printf("checkOwnerID - matchUser: %s\n", matchUser);
    // printf("checkOwnerID - pw       : %s\n", pw->pw_name);
    return match;

}

void* threadFolder(void* arg){
    struct data* d = (struct data*) arg;

    // max depth hit, don't check this level
    if (d->chkFolderDepth < 0){
        return (NULL);
    }
    // we have current folderpath           d->currDIR              (string)
    // file type to search                  d->fdFileType           (int)
    // file ownership to search             d->fdFileOwner          (string)
    // file name to search                  d->fdFileName           (string)
    // max folder depth level to search     d->chkFolderDepth       (int)
    // printf("thread ID: %d, ", d->ID);
    // printf("current DIR: %s\n", d->currDIR);
    // printf("find filetype: %d, ", d->fdFileType);
    // printf("check filedepth: %d, ", d->chkFolderDepth);
    // printf("find fileowner: %s, ", d->fdFileOwner);
    // printf("find filename: %s\n", d->fdFileName);

    int usernameMatch, currFileType, threadID=0, strLength;
    char downOneLevel[210], filePathwayFound[210];
    pthread_t tid[nprocs];
    struct data* idata = (struct data*)malloc(nprocs * sizeof(struct data));
    struct dirent* filesInFolder;

    if (idata == NULL){
        printf("memory allocation failed\n");
        return (NULL);
    }

    // check if opening directory is NULL
    DIR* fldrptr=opendir(d->currDIR);
    if (fldrptr == NULL){
        printf("Error : Failed to open input directory\n");
        return (NULL);
    }

    // iterates through the folder, reads files one by one
    while ((filesInFolder = readdir(fldrptr))){
        currFileType = printType(filesInFolder->d_name, d->currDIR);
        // ensures we just skip the '.' & '..' & symbolic links
        if (!strcmp(filesInFolder->d_name, ".") || !strcmp(filesInFolder->d_name, "..") ){      //|| currFileType == 3
            continue;
        }
        else{
            //printf("secondary, filename pathway: %s, filetype: %d\n", filesInFolder->d_name, currFileType);
            if (currFileType == 2){
                while (1){
                    // ensures a thread gets made, else it'll wait for threads to finish and reset
                    if (threadID < nprocs){
                        sprintf(downOneLevel, "%s/%.100s", d->currDIR, filesInFolder->d_name);
                        idata[threadID].ID = threadID;
                        strLength = strlen(downOneLevel);
                        idata[threadID].currDIR = malloc(sizeof(char) * strLength + 1);
                        strcpy(idata[threadID].currDIR, downOneLevel);
                        idata[threadID].fdFileType = d->fdFileType;
                        idata[threadID].chkFolderDepth = d->chkFolderDepth-1;
                        strLength = strlen(downOneLevel);
                        idata[threadID].fdFileOwner = malloc(sizeof(char) * strLength + 1);
                        strcpy(idata[threadID].fdFileOwner, d->fdFileOwner);
                        strLength = strlen(downOneLevel);
                        idata[threadID].fdFileName = malloc(sizeof(char) * strLength + 1);
                        strcpy(idata[threadID].fdFileName, d->fdFileName);
                        pthread_create(&tid[threadID], NULL, threadFolder,(void *)&idata[threadID]);
                        threadID++;
                        
                        break;
                    }
                    else{
                        // wait for all threads to complete
                        for (int i=0; i < threadID; i++){
                            pthread_join(tid[i], NULL);
                            free(idata[i].currDIR);
                            free(idata[i].fdFileOwner);
                            free(idata[i].fdFileName);
                        }
                        threadID=0;
                        // free(idata);
                        // idata = (struct data*)malloc(nprocs * sizeof(struct data));
                    }
                }
            }
            char testname[50], testowner[50];
            strcpy(testname, filesInFolder->d_name);
            strcpy(testowner, d->fdFileOwner);

            // check if file name matches glob pattern
            int glob_matches = globme(d->currDIR, d->fdFileName, filesInFolder->d_name);

            // if pattern does not match, continue to the next file in the directory
            if(glob_matches == 0 ){
                continue;
            }
            // if pattern does match, we now check all the other perameters: username, file type, etc
            else{
                usernameMatch=checkOwnerID(testname, testowner);
                if (currFileType==d->fdFileType && usernameMatch==1){
                    // get currentpath, append the filename that matchs
                    sprintf(filePathwayFound, "%.100s/%.100s", d->currDIR, filesInFolder->d_name);
                    printf("secondary, match found, pathway: %s\n", filePathwayFound);
                }
            }
        }
    }
    for (int i=0; i < threadID; i++){
        pthread_join(tid[i], NULL);
        free(idata[i].currDIR);
        free(idata[i].fdFileOwner);
        free(idata[i].fdFileName);
    }
    free(idata);

    closedir(fldrptr);


    return (NULL);
}

// initial set up to gather necessary info for folder transverse
// only one thread, this is the main thread, handles all files/folders
// splits directories to threads to handle
void findme(char* fdDIR, char* fdType, char* fdDepth, char* fdUser, char* fdFile){
    int fileType, findType, usernameMatch, threadID=0, strLength;
    char currentFilePath[100], downOneLevel[201], filePathwayFound[201];
    pthread_t tid[nprocs];
    struct data* idata = (struct data*)malloc(nprocs * sizeof(struct data));
    struct dirent* filesInFolder;
    getcwd(currentFilePath, sizeof(currentFilePath));

    if (idata == NULL){
        printf("memory allocation failed\n");
        return;
    }

    // check if opening directory is NULL
    DIR* fldrptr=opendir(fdDIR);
    if (fldrptr == NULL){
        printf("Error : Failed to open input directory\n");
        return;
    }

    // converts users fileType to int, easier to work with imo
    findType = convertUserFileType(fdType);

    // iterates through the folder, reads files one by one
    while ((filesInFolder = readdir(fldrptr))){
        fileType = printType(filesInFolder->d_name, currentFilePath);
        //printf("fileType: %d, fileName: %s \n", fileType, filesInFolder->d_name);

        // ensures we just skip the '.' & '..' & symbolic links
        if (!strcmp(filesInFolder->d_name, ".") || !strcmp(filesInFolder->d_name, "..") ){          //|| fileType == 3
            continue;
        }
        else{
            // printf("main, filename loop: %s, filetype: %d\n", filesInFolder->d_name, fileType);
            // printf("fileType: %d, fileName: %s \n", fileType, filesInFolder->d_name);
            if (fileType == 2){
                while (1){
                    // ensures a thread gets made, else it'll wait for threads to finish and reset
                    if (threadID < nprocs){
                        sprintf(downOneLevel, "%s/%.100s", currentFilePath, filesInFolder->d_name);
                        idata[threadID].ID = threadID;
                        strLength = strlen(downOneLevel);
                        idata[threadID].currDIR = malloc(sizeof(char) * strLength + 1);
                        strcpy(idata[threadID].currDIR, downOneLevel);
                        idata[threadID].fdFileType = findType;
                        idata[threadID].chkFolderDepth = atoi(fdDepth)-1;
                        strLength = strlen(downOneLevel);
                        idata[threadID].fdFileOwner = malloc(sizeof(char) * strLength + 1);
                        strcpy(idata[threadID].fdFileOwner, fdUser);
                        strLength = strlen(downOneLevel);
                        idata[threadID].fdFileName = malloc(sizeof(char) * strLength + 1);
                        strcpy(idata[threadID].fdFileName, fdFile);
                        pthread_create(&tid[threadID], NULL, threadFolder,(void *)&idata[threadID]);
                        threadID++;
                        
                        break;
                    }
                    else{
                        // wait for all threads to complete
                        for (int i=0; i < threadID; i++){
                            pthread_join(tid[i], NULL);
                            free(idata[i].currDIR);
                            free(idata[i].fdFileOwner);
                            free(idata[i].fdFileName);
                        }
                        threadID=0;
                    }
                }
            }

            

            // check if file name matches glob pattern
            int glob_matches = globme(currentFilePath, fdFile, filesInFolder->d_name);      

            // if pattern does not match, continue to the next file in the directory
            if(glob_matches == 0 ){
                // printf("globme - filesInFolder->d_name: %s\n", filesInFolder->d_name);
                // printf("globme - fdFile               : %s\n", fdFile);
                continue;
            }
            // if pattern does match, we now check all the other perameters: username, file type, etc
            else{
                // check if we have a match and it fulfills all arguements
                usernameMatch=checkOwnerID(filesInFolder->d_name, fdUser);
                if (fileType==findType && usernameMatch==1){
                    // append filename to filepath
                    sprintf(filePathwayFound, "%s/%.100s", currentFilePath, filesInFolder->d_name);
                    printf("main, found file pathway: %s\n", filePathwayFound);
                }
            }
        }
    }

    for (int i=0; i < threadID; i++){
        pthread_join(tid[i], NULL);
        free(idata[i].currDIR);
        free(idata[i].fdFileOwner);
        free(idata[i].fdFileName);
    }
    free(idata);
    closedir(fldrptr);

    return;
}


int globme(const char *root,  const char *fpattern, const char *fname) {
    glob_t glob_result;         // Create a glob structure
    char pattern[PATH_MAX];     // string to store the patern with the full file path
    char target[PATH_MAX];      // string to store the full file name with the file path

    snprintf(target, PATH_MAX, "%s/%s", root, fname);           //fills target with file path and file name
    snprintf(pattern, PATH_MAX, "%s/%s", root, fpattern);       //fills fpattern with file path and patern for glob
    // printf("\n\npattern: %s\n", pattern);
    int ret = glob(pattern, 0, NULL, &glob_result);     //runs glob command and checks for matching file names

    //if the glob function found match
    if (ret == 0) {     
        // printf("Number of matches: %lu\n", glob_result.gl_pathc);

        //loop through all the file names that match the pattern
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {     
            //if the matching file name matches the file we are checking, this function returns 1   
            if(!strcmp(glob_result.gl_pathv[i], target)){
                // printf("Match %lu: %s\n", i + 1, glob_result.gl_pathv[i]);
                return 1;
            }
        }
        // if the file name wasn't in the glob pattern we return 0
        // this could occur if there is a file in the current directory that matches the pattern
        // but the current file is not that file
        return 0;       
    } else {            // if there was no match for the glob we return 0
        return 0;
    }

    globfree(&glob_result);         // free up the memory used to store the glob structure
}


int main(int argc, char *argv[]){
    printf("\nstarting program\n\n");
    arguments args = {NULL, NULL, NULL, NULL, NULL};
    if (argc < 2){
        printf("No arguments have been inputted\n");
        return 0;
    }
    args.root = argv[1];
    int c = 2;
    while(c < argc){
        if(strcmp(argv[c], "-type") == 0){
            args.type = argv[c+1];
            c++;
        }
        else if(strcmp(argv[c], "-name") == 0){
            args.fname = argv[c+1];
            c++;
        }
        else if(strcmp(argv[c], "-user") == 0){
            args.user = argv[c+1];
            c++;
        }
        else if(strcmp(argv[c], "-maxdepth") == 0){
            args.maxdepth = argv[c+1];
            c++;
        }
        c++;
    }

    int fail_count = 0;
    if(args.type == NULL){
        printf("Missing argment: file type\n");
        fail_count++;
    }
    if(args.fname == NULL){
        printf("Missing argment: file name\n");
        fail_count++;
    }
    if(args.user == NULL){
        printf("Missing argment: username\n");
        fail_count++;
    }
    if(args.maxdepth == NULL){
        printf("Missing argment: max depth\n");
        fail_count++;
    }
    if(fail_count > 0){
        exit(EXIT_FAILURE);
    }
  
    printf("Root: %s\nType: %s\nName: %s\nUser: %s\nMax depth: %s\n\n", 
    args.root,
    args.type,
    args.fname,
    args.user,
    args.maxdepth);

    findme(args.root, args.type, args.maxdepth, args.user, args.fname);

    exit(EXIT_SUCCESS);

    return 0;
}