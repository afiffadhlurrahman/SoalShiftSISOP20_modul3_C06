#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

pthread_t tid[1000];

char* nowDir, *tempDir;

void *moveFile();
void *moveFile2();
void *moveFileBintang();

int main(int argc, char **argv){
    char tes[1000];
    nowDir = getcwd(tes,sizeof(tes));
    printf("dir : %s\n", nowDir);
    if(!strcmp(argv[1],"-f")){
        printf("1\n");
        for(int i=0;i<argc-2;i++){
            pthread_create(&tid[i],NULL,moveFile,(void *)argv[i+2]);
        }
        for(int i=0;i<argc-2;i++){
            pthread_join(tid[i],NULL);
        }
    }
    else if(!strcmp(argv[1],"-d")){
        printf("2\n");
        int index=0;
        chdir(argv[2]); //ganti directory
        tempDir = getcwd(tes,sizeof(tes));
        struct dirent* dp;
        struct stat file;
        DIR* fd = opendir(".");
        if(fd){
            while((dp = readdir(fd)) != NULL){
                if (stat(dp->d_name, &file) < 0);
                else if (!S_ISDIR(file.st_mode))
                {
                    pthread_create(&tid[index],NULL,moveFile2,(void *)dp->d_name);
                }
                index++;
            }
            for(int j=0;j<(index-2);j++){
                pthread_join(tid[j],NULL);
            }
        }

    }
    else if(!strcmp(argv[1],"*")){
        printf("3\n");
        int index=0;
        struct dirent* dp;
        struct stat file;
        DIR* fd = opendir(".");
        if(fd){
            while((dp = readdir(fd)) != NULL){
                if (stat(dp->d_name, &file) < 0);
                else if (!S_ISDIR(file.st_mode))
                {
                    pthread_create(&tid[index],NULL,moveFileBintang,(void *)dp->d_name);
                }
                index++;
            }
            for(int j=0;j<(index-2);j++){
                pthread_join(tid[j],NULL);
            }
        }
    }

    return 0;
}

char* getFileName(char name[]){
    char* fileName, *final;
    fileName = strchr(name,'/');
    if(fileName == NULL)return name;
    else{
        while (fileName != NULL) {
            final = fileName+1;
            fileName = strchr(fileName+1,'/');
        }
    }
    return final;
}

char *getExtentionFile(char name[]){
    char *ex = strchr(getFileName(name),'.'); 
    if(ex==NULL) return NULL;
    else{
        return ex+1;
    }
}

void moveFileNow(char s[],char d[]){ //mindahin file
    int c;
    FILE *f1, *f2;
    f1 = fopen(s,"r");
    f2 = fopen(d,"w");
    if (!f1) {
        printf("Unable to open source file to read!\n");
        fclose(f2);
        return ;
    }
    if (!f2) {
        printf("Unable to open target file to write!\n");
        return ;
    }
    while ((c = fgetc(f1)) != EOF)fputc(c, f2);
    fclose(f1);
    fclose(f2);
    remove(s);
}

void *moveFile(void *arg){
    char* extention ;
    extention= getExtentionFile((char *)arg);
    char destinationFolder[1000];
    if(extention==NULL) strcpy(destinationFolder,"Unknown");
    else{
        strcpy(destinationFolder,extention);
        //typecast huruf besar ke huruf kecil
        for(int i=0;i<strlen(destinationFolder);i++){
            if(destinationFolder[i]>=65 && destinationFolder[i]<=98){
                destinationFolder[i]+=32;
            }
        }
    }
    mkdir(destinationFolder,0777);
    char destinationPath[1000];
    snprintf(destinationPath, sizeof(destinationPath), "%s/%s/%s", nowDir,destinationFolder,getFileName((char*)arg));
    moveFileNow((char *)arg, destinationPath);
}

void *moveFile2(void *arg){
    char *extention;
    extention = getExtentionFile((char *)arg);
    char destinationFolder[1000];
    if(extention==NULL) strcpy(destinationFolder,"Unknown");
    else{
        strcpy(destinationFolder, extention);
        //typecast huruf besar ke huruf kecil
        for(int i=0;i<strlen(destinationFolder);i++){
            if(destinationFolder[i]>=65 && destinationFolder[i]<=98){
                destinationFolder[i]+=32;
            }
        }
    }
    mkdir(destinationFolder,0777);
    char destinationPath[1000],sourcePath[1000];
    snprintf(sourcePath, sizeof(sourcePath), "%s/%s", tempDir, (char *)arg);
    snprintf(destinationPath, sizeof(destinationPath), "%s/%s/%s", nowDir, destinationFolder, getFileName((char *)arg));
    moveFileNow(sourcePath, destinationPath);
}

void *moveFileBintang(void *arg){
    char *extention ;
    extention = getExtentionFile((char *)arg);
    char destinationFolder[1000];
    if(extention==NULL) strcpy(destinationFolder,"Unknown");
    else{
        strcpy(destinationFolder, extention);
        //typecast huruf besar ke huruf kecil
        for(int i=0;i<strlen(destinationFolder);i++){
            if(destinationFolder[i]>=65 && destinationFolder[i]<=98){
                destinationFolder[i]+=32;
            }
        }
    }
    mkdir(destinationFolder,0777);
    char destinationPath[1000],sourcePath[1000];
    snprintf(sourcePath, sizeof(sourcePath), "%s/%s", nowDir, (char *)arg);
    snprintf(destinationPath, sizeof(destinationPath), "%s/%s/%s", nowDir, destinationFolder, getFileName((char *)arg));
    moveFileNow(sourcePath, destinationPath);

}

