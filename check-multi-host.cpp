#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string>
#include<iostream>

using namespace std;

int main()
{
    pid_t process_id;
    int return_val = 1;
    int state;
    process_id = fork();
    if(process_id == -1)
    {   
        printf("can't fork, error occured\n");
        exit(0);
    }
    else if (process_id == 0) 
    { 
        int id = getpid();
        char tmp = char(id) + 'a';
        char name_host[2];
        name_host[0] =  tmp;
        name_host[1] = '\0';
        cout << id << endl;
        char * argv_list[] = {"./Src/Host.out", name_host, "temp", NULL};
        execv("./Host.out",argv_list); // the execv() only return if error occured.
        exit(0);
    } 
    else 
    {
        for(int i = 0; i < 20; i++)
        {
            fork();
        }
        for(int i  = 0; i < 20; i++)
        {
            wait(NULL);
        }
    }
}