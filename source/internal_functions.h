#ifndef INTERNAL_FUNCTIONS_H
#define INTERNAL_FUNCTIONS_H

void echo();

void pwd();

void cd();

void make_dir();

void rmtree(const char path[]);

void whoami();

void history();

void readcmd(const char *readline);

int check_internal_alias();

#endif