/*
Compare
./myls.out /dev
ls -la /dev
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LEN 256

int main(int argc, char *argv[]) {
	char fullpath[LEN + 1];
	fullpath[LEN] = '\0';
	char linkpath[LEN + 1];
	linkpath[LEN] = '\0';
	int len;
	DIR *dir;
	if(argc == 1) {
		dir = opendir(".");
	} else {
		dir = opendir(argv[1]);
	}
	if(dir == NULL) {
		printf("Cant open dir\n");
		exit(-1);
	}
	struct dirent *file;
	struct stat buf;
	while((file = readdir(dir)) != NULL) {
		printf("%s ", file->d_name);
		if(argc == 1) {
			sprintf(fullpath, "./%s", file->d_name);
		} else {
			sprintf(fullpath, "%s/%s", argv[1], file->d_name);
		}
		if(lstat(fullpath, &buf) < 0) {
			printf("Cant get file status\n");
			exit(-1);
		}
		if(S_ISREG(buf.st_mode)) {
			printf("Regular ");
		} else if(S_ISDIR(buf.st_mode)) {
			printf("Directory ");
		} else if(S_ISCHR(buf.st_mode)) {
			printf("Character device ");
		} else if(S_ISBLK(buf.st_mode)) {
			printf("Block device ");
		} else if(S_ISFIFO(buf.st_mode)) {
			printf("FIFO ");
		} else if(S_ISLNK(buf.st_mode)) {
			printf("Link -> ");
			if((len = readlink(fullpath, linkpath, LEN)) < 0) {
				printf("Cant read link\n");
				exit(-1);
			}
			linkpath[len] = '\0';
			printf("%s ", linkpath);
		} else if(S_ISSOCK(buf.st_mode)) {
			printf("Socket ");
		} else {
			printf("Unknown ");
		}
		printf("\n");
	}
	if(closedir(dir) < 0) {
		printf("Cant close dir\n");
		exit(-1);
	}
	return 0;
}
