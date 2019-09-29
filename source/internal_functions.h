#ifndef INTERNAL_FUNCTIONS_H
#define INTERNAL_FUNCTIONS_H

void echo(const char *msg);

void pwd();

void cd(const char *path);

void make_dir(const char *param);

void rmtree(const char path[]);

void whoami();

void history();

void readcmd(const char *readline);

void declare();

void unset();

int check_internal_alias();

#endif