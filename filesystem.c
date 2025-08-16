#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

//files structure
typedef struct File {
    char fname[MAX];
} File;

//directories structure
typedef struct Directories {
    char name[MAX];
    struct Directories *directories;
    File *files;
    int fsize;
    int dsize;
} Directories;

// Function to create a new directory node with 1 parameter {Dirname}
Directories *createNode(const char *name) {
    Directories *newDir = (Directories *)malloc(sizeof(Directories));
    strncpy(newDir->name, name, MAX);
    newDir->directories = NULL;
    newDir->files = NULL;
    newDir->fsize = 0;
    newDir->dsize = 0;
    return newDir;
}

//function to create a new file node with 1 parameter{fileName}
File *createFileNode(const char *name){
	File *newFile = (File*)malloc(sizeof(File));
	strncpy(newFile->fname,name,MAX);
	return newFile;
}

// Function to find a directory by name with 2 parameters {root dir address}{fileName}
Directories *findDirectory(Directories *root, const char *name) {
    int i;
	if (strcmp(root->name, name) == 0) 
        return root;
    for (i = 0; i < root->dsize; ++i) {
        Directories *result = findDirectory(&root->directories[i], name);
        if (result != NULL)
            return result;
    }
    return NULL;
}

// Function to create a directory with 3 parameters {fileSystem Root file address}{parentDir name}{newdir name}
void createDirectory(Directories *root, const char *parentName, const char *dirName) {
    Directories *parentDir = findDirectory(root, parentName);
    if (parentDir == NULL) {
        printf("Parent directory not found.\n");
        return;
    }
    parentDir->directories = (Directories *)realloc(parentDir->directories, (parentDir->dsize + 1) * sizeof(Directories));
    parentDir->directories[parentDir->dsize++] = *createNode(dirName);
    printf("\n \t Directory created sucessfully\n");
}

// Function to print the directory tree with 2 parameters {root address}{level for space}
void printTree(const Directories *dir, int level) {
    int i,j;
	for (i = 0; i < level; ++i) {
        printf("  ");
    }
    printf("D: %s\n", dir->name);
    for (i = 0; i < dir->fsize; ++i) {
        for (j = 0; j < level + 1; ++j) {
            printf("   ");
        }
        printf("F: %s\n", dir->files[i].fname);
    }
    for (i = 0; i < dir->dsize; ++i) {
        printTree(&dir->directories[i], level + 1);
    }
}

//Function to creating file with 3 parameters {root address}{dirName}{fileNmae}
void createFile(Directories *root,char *dir,char *f_name){
	Directories *parentDir = findDirectory(root,dir);
	if(parentDir==NULL){
		printf("Parent directory not found .\n");
		return;
	}
	parentDir->files=(File*)realloc(parentDir->files,(parentDir->fsize+1)*sizeof(File));
	parentDir->files[parentDir->fsize++]=*createFileNode(f_name);
	printf("\n\n\t File created sucessfully \n");
}

//Function to delete file with 2 parameters {dirName}{fileName}
int deleteFile(Directories *dir, const char *fname) {
    int index = -1,i,flag=0;
    for (i = 0; i < dir->fsize; ++i) {
        if (strcmp(dir->files[i].fname, fname) == 0) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        // Shift files to fill the gap
        for (i = index; i < dir->fsize - 1; ++i) {
            dir->files[i] = dir->files[i + 1];
        }
        dir->fsize--;
        dir->files = (File *)realloc(dir->files, dir->fsize * sizeof(File));
        flag=1;
		return flag;
		
    } 
    return flag;
}

//Function to delete directorie with 2 parameters {root address}{dirName}
int deleteDirectory(Directories *root, const char *dirName) {
    Directories *parentDir = NULL;
    int index = -1,i,flag=0;
    for (i = 0; i < root->dsize; ++i) {
        if (strcmp(root->directories[i].name, dirName) == 0) {
            parentDir = root;
            index = i;
            break;
        } else {
            deleteDirectory(&root->directories[i], dirName);
        }
    }
    if (parentDir != NULL && index != -1) {
        // Free memory allocated for the directory and its contents
        free(parentDir->directories[index].files);
        free(parentDir->directories[index].directories);
        
        // Shift directories to fill the gap
        for (i = index; i < parentDir->dsize - 1; ++i) {
            parentDir->directories[i] = parentDir->directories[i + 1];
        }
        parentDir->dsize--;
        parentDir->directories = (Directories *)realloc(parentDir->directories, parentDir->dsize * sizeof(Directories));
		flag = 1;
        return flag;
    } 
}

//main function 
int main() {
    char name[MAX], dirname[MAX];
    int ch;
    Directories *root = createNode("root");
    do {
    	printf("\n **--FILE SYSTEM--**\n");
        printf("1. Create directory\n");
        printf("2. Create File\n");
        printf("3. Print Tree\n");
        printf("4. Delete Directory\n");
        printf("5. Delete File\n");
        printf("6. Exit \n");
        printf("  Enter your choice: \n");
        scanf("%d", &ch);
        switch(ch) 
		{
            case 1:
                printf("Enter the directory name: \n");
                scanf("%s", name);
                printf("Enter the parent name: \n");
                scanf("%s", dirname);
                createDirectory(root, dirname, name);
                break;
            case 2:
            	printf("enter the file name \n");
            	scanf("%s",name);
            	printf("enter the directori name \n");
            	scanf("%s",dirname);
            	createFile(root,dirname,name);
                break;
            case 3:
                printTree(root, 0);
                break;
            case 4:
            	printf("\n enter the directory name \n");
            	scanf("%s",dirname);
            	if(deleteDirectory(root,dirname))
            		printf("%s is deleted sucessfully deleted \n",dirname);
            	else
            		printf("%s is not found \n",dirname);
            	break;
            case 5:
            	printf("\n enter the directory name \n");
            	scanf("%s",dirname);
            	Directories *dir=findDirectory(root,dirname);
         		if(dir)
         		{
         			printf("\n enter the file name to be delete \n");
					scanf("%s",name);
					if(deleteFile(dir,name))
						printf("\n %s is deleted sucessfully \n ",name);
					else
						printf("\n No such file found with name %s \n ",name);
				}
				else
					printf("\n no such directory with name %s \n",dirname);
            	break;
            case 6:
                printf("Exiting...\n");
            	break;
			default:
                printf("Invalid choice\n");
                break;
        }
        getch();
    } while(ch != 6);
    return 0;
}

