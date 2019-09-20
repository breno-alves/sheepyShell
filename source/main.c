#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>

#include "internal_functions.h"

#define MAX_LINE 80
#define MAX_ALIAS 10

/* Usuario */
char USER[254];
char c_path[256];

int main(){
    int runner;

    cuserid(USER);

    if(getcwd(c_path, sizeof(c_path)) == NULL){
        perror("Nao foi possivel obter o diretorio atual");
        return 1;
    }
    
    runner = 1; 
    while (runner){

        getcwd(c_path, sizeof(c_path));
        printf("%s: %s $ ", USER, c_path);
        fflush(stdout);
        read_cmd();
    }
    
    return 0;
}

