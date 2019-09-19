#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>

#define MAX_LINE 80
#define MAX_ALIAS 10

/* Usuario */
char USER[254];

/* Comandos internos */
const char *internal[] = {"exit", "echo", "pwd", "cd", "mkdir", "rmdir", "history", "declare", "unset", "whoami"};

/* Caminho base e caminho atual */
char c_path[254];

/* Linha temporaria */
char readline[MAX_LINE];

/* Linha de comando */ 
char cmd[MAX_LINE+1];

/* Lista de parametros */
char params[10][MAX_LINE+1];
int n_params=0;

/* History */

int n_history=0;
char history_com[254][254];

/* Variaveis de ambiente */

int n_env_var=0;
struct env_var{
	char key[256];
	char value[256];
} var[254];

/* Funcoes internas */

struct stat st = {0};

int is_dir(char _path[]){

    if(stat(_path, &st) == -1)
	    return 0;
    else
        return 1;

}

void echo(){
    if(n_params > 0)
        printf("%s\n", params[0]);
}

void pwd(){
    printf("%s\n", c_path);
}

void whoami(){
    printf("%s\n", USER);
}

void cd(){

    if(n_params > 0){
        if(chdir(params[0])){
            printf("Diretorio nao encontrado.\n");
        }
    } else{
        chdir(getenv("HOME"));
    }
            
    getcwd(c_path, sizeof(c_path));   
}



void make_dir(){

    if(n_params > 0){
        char path[254];
        strcpy(path, c_path);
        strcat(path, "/");
        strcat(path, params[0]);
        if(stat(path, &st) == -1){
            mkdir(path, 0700);
        }
    }
}

void remove_dir(){
}

void history(){
    int i;
    
    if(n_params > 0 && !strcmp(params[0], "-c")){
        for(i = 0; i < n_history; i++){
            history_com[i][0] = '\0';
        }
        n_history = 0;        
    } else {
        for(i = 0; i < n_history; i++){
            printf("%s\n", history_com[i]);
        }
    }
}

int check_var_alias(char key[]){
    int i;

    if(n_params < 1)
        return 0;

    for(i = 0; i < n_env_var; i++){
        if(!strcmp(var[i].key, "\0")){
            i--;
            continue;
        }

        if(!strcmp(var[i].key, params[0])){
            return i;
        }
    }

    return -1;
}

void declare(){
    int idx;

    if(n_params < 2){
        printf("Informe a variavel e o valor da variavel.\n");
        return;
    }

    idx = check_var_alias(params[0]);

    if(idx == -1){
        strcpy(var[n_env_var].key, params[0]);
        strcpy(var[n_env_var].value, params[1]);
        n_env_var++;
    } else {
        strcpy(var[idx].value, params[1]);
    }
        
}

void unset(){
    int idx, i;

    if(n_params < 1){
        return;
    }

    idx = check_var_alias(params[0]);

    if(idx == -1){
        printf("Variavel nao encontrada.\n");
    } else {
        var[idx].key[0] = '\0';
        n_env_var--;
    }
    /*bug aqui*/
    for(i = 0; i < n_env_var; i++){
        if(!strcmp(var[i].key, "\0")){
            i--;
            break;
        }
        printf("key: %s value: %s\n", var[i].key, var[i].value);
    }

}

/* ////////////////////// */

void read_cmd(){
    int i, j;
    
    scanf(" %[^\n]s",readline);
    strcpy(history_com[n_history], readline);
    n_history++;

    for(i=0; i < strlen(readline); i++){
        if(readline[i] == ' ')
            break;
        cmd[i] = readline[i];
    }
    
    int cont = 0;
    if(i+1 < strlen(readline)){
        n_params = 1;
        for(i=i+1, j=0; i < strlen(readline); i++){
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
}

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
            echo();
            break;
            
        case 2:
            pwd();
            break;
       
        case 3:
            cd();
            break;
        case 4:
            make_dir();
            break;
            
        case 5:
            remove_dir;
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
    memset(readline, 0, sizeof(readline));
    memset(params, 0, sizeof(params));
    memset(cmd, 0, sizeof(cmd));
    n_params = 0;
}

void debug_print(){
    int i, j;
    
    printf("\ncmd:\n");
    for(i=0; i < strlen(cmd); i++){
        printf("%c", cmd[i]);
    }
    
    printf("\nParams:\n");
    for(i = 0; i < n_params; i++){
        for(j = 0; j < strlen(params[i]); j++){
            printf("%c", params[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}



int main(){
    
    cuserid(USER);

    if(getcwd(c_path, sizeof(c_path)) == NULL){
        perror("Nao foi possivel obter o diretorio atual");
        return 1;
    }
    
    int runner;
    runner = 1; 
    while (runner){
        printf("%s: %s $ ", USER, c_path);
        fflush(stdout);
        read_cmd();
        
        if(!check_internal_alias()){
            printf(" commando nao interno\n");
        }
        
        reset();
    }
    
    return 0;
}

