#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include <dirent.h>

#define MAX_LINE 80
#define MAX_ALIAS 10


/* Comandos internos */


const char *internal[] = {"exit", "echo", "pwd", "cd", "mkdir", "rmdir", "history", "declare", "unset", "whoami"};

int n_params=0;

char params[25][256];
char cmd[256];
char readline[256];

struct stat st = {0};

void echo(const char *msg){
    printf("%s\n", msg);
}

void pwd(){
    char path[256];
    getcwd(path, sizeof(path));
    
    if(path == NULL){
        printf("Nao foi possivel obter o diretorio atual\n");
        return;
    }

    printf("%s\n", path);
}

void cd(const char *path){

    if(path != NULL){
        if(chdir(path)){
            printf("Diretorio nao encontrado\n");
        }
    } else {
        chdir(getenv("HOME"));
    }
}

void make_dir(const char *param){

    if(n_params > 0){
        char path[256];
        getcwd(path, sizeof(path));;
        strcat(path, "/");
        strcat(path, param);
        if(stat(path, &st) == -1){
            mkdir(path, 0700);
        }
    }
}

void rmtree(const char path[]){
    size_t path_len;
    char *full_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    stat(path, &stat_path);

    if (S_ISDIR(stat_path.st_mode) == 0) {
        fprintf(stderr, "%s: %s\n", "Is not directory", path);
        exit(-1);
    }

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
        exit(-1);
    }

    path_len = strlen(path);

    while ((entry = readdir(dir)) != NULL) {

        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);

        stat(full_path, &stat_entry);

        if (S_ISDIR(stat_entry.st_mode) != 0) {
            rmtree(full_path);
            continue;
        }

        if (unlink(full_path) == 0)
            printf("Removed a file: %s\n", full_path);
        else
            printf("Can`t remove a file: %s\n", full_path);
    }

    if (rmdir(path) == 0)
        printf("Removed a directory: %s\n", path);
    else
        printf("Can`t remove a directory: %s\n", path);

    closedir(dir);
}

void whoami(){
    char *p = getenv("USER");
    
    if(p == NULL){
        printf("Erro ao tentar identificar o usuario.\n");
        return;
    }
    
    printf("%s\n", p);
}

void history();

void declare();

void unset();

int check_internal_alias(){
    int i, idx=-1;
    for(i=0; i < MAX_ALIAS; i++){
        if(!strcmp(cmd, internal[i])){
            idx = i;
            break;
        }
    }
    
    switch(idx){
        case 0:
            exit(1);
            break;
            
        case 1:
            echo(params[0]);
            break;
            
        case 2:
            pwd();
            break;
       
        case 3:
            cd(params[0]);
            break;
        case 4:
            make_dir(params[0]);
            break;
            
        case 5:
            rmtree(params[0]);
            break;
            
        case 6:
            history();
            break;
            
        case 7:
            declare();
            break;
            
        case 8:
            unset();
            break;
        
        case 9:
            whoami();
            break;
            
        case -1:
            return 0;
            break;
            
        default:
            return 0;
            break;
    }
    
    return 1;
}

void reset(){
    memset(params, 0, sizeof(params));
    memset(cmd, 0, sizeof(cmd));
    memset(readline, 0, sizeof(readline));
}

void read_cmd(){
    int i, j, str_len;

    scanf(" %[^\n]s",readline);
    str_len = strlen(readline);

    for(i=0; i < str_len; i++){
        if(readline[i] == ' '){
            break;
        }
        cmd[i] = readline[i];
    }

    /* copia os parametros para params */
    int cont = 0;
    if(i+1 < str_len){
        n_params = 1;
        for(i=i+1, j=0; i < str_len; i++){
            if(readline[i] == ' '){
                n_params++;
                j++;
                cont=0;
                continue;
            }
            
            params[j][cont] = readline[i];
            cont++;
        }        
    }

    if(!check_internal_alias()){
        printf("Comando nao interno..\n");
    } 

    reset();
}