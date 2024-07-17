//Ben Morley - Lab 3



#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdint.h>
#include<sys/sysmacros.h>
#include<string.h>

int fileSystemTests(char*);
int magicNumberTests(char*);
int asciiOrBinary(char*);




int main(int argc, char* argv[]){
	//printf("%s\n", argv[1]); 			//prints file name for troubleshooting

	int fst = fileSystemTests(argv[1]); 		//gets file system type
	//printf("fst: %d\n", fst);

	if(fst == 0){
		int aob = asciiOrBinary(argv[1]); 		//checks for ascii or binary
		//printf("aob: %d\n", aob);
		
		if(aob == 1){
			int mnt = magicNumberTests(argv[1]); 		//reads elf header
			//printf("mnt: %d\n", mnt);
			
			switch(mnt){
				case 0:
					printf("ELF Relocatable\n");
					break;
				case 1:
					printf("ELF Executable, x86-64\n");
					break;
				case 2:
					printf("ELF Shared Object\n");
					break;
				case 3:
					printf("ELF Core\n");
					break;
				case 4:
					printf("BASH Script\n");
					break;
				case 5:
					printf("Python Script\n");
					break;
				default:
					printf("regular file; BINARY format\n");
			}
		}
		else{
			printf("regular file; ASCII format\n");
		}

	}
	else{
		switch(fst){
			case 1:
				printf("directory\n");
				break;
			case 2:
				printf("character special file\n");
				break;
			case 3:
				printf("block special file\n");
				break;
			case 4:
				printf("pipe\n");
				break;
			case 5:
				printf("symbolic link\n");
				break;
			case 6:
				printf("socket\n");
				break;

		}
	}


}


int fileSystemTests(char * filename){
	struct stat buffer;

	stat(filename, &buffer);

	if(S_ISREG(buffer.st_mode)){
		return(0);
	}
	else if(S_ISDIR(buffer.st_mode)){
		return(1);
	}
	else if(S_ISCHR(buffer.st_mode)){
		return(2);
	}
	else if(S_ISBLK(buffer.st_mode)){
		return(3);
	}
	else if(S_ISFIFO(buffer.st_mode)){
		return(4);
	}
	else if(S_ISLNK(buffer.st_mode)){
		return(5);
	}
	else{
		return(6);
	}

}

int magicNumberTests(char* filename){
	FILE *fp = fopen(filename, "rb");

	if(fp == NULL){
		exit(EXIT_FAILURE);
	}

	unsigned char header[32];

	if(fread(header, 1, 32, fp) < 4){
		exit(EXIT_FAILURE);
	}

	fclose(fp);

	/* 		Prints the ELF header in hex so I could read it
	for(int c = 0; c < 16; c++){
		printf("%02x ", header[c]);
	}
	printf("\n");
	for(int c = 16; c < 32; c++){
		printf("%02x ", header[c]);
	}
	*/


	if( (header[1] == 'E') && (header[2] == 'L') && (header[3] == 'F') ) {
		unsigned char type = header[16];
		//printf("type: %u\n", type);

		if((type > 0) && (type <= 4)){
			return(type-1);
		}
	}

	if (strncmp((char*) header, "#!/bin/bash", 11) == 0){
		return(4);
	}
	else if (strncmp((char*) header, "#!/bin/python", 17) == 0){
		return(5);
	}
	else{
		return(-1);
	}

}

int asciiOrBinary(char* filename){
	FILE *fp = fopen(filename, "rb");

	if(fp == NULL){
		exit(EXIT_FAILURE);
	}

	int bit = fgetc(fp);
	

	while(bit != EOF){
		if(bit > 0x7f){
			fclose(fp);
			return(1);
		}
		bit = fgetc(fp);
	}

	fclose(fp);
	return(0);


}

