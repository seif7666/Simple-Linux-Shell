#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#define SIZE 100 //Max size of input string

void execute_command(char*,char**,int);
int get_number_of_spaces(char*);
void get_args(char*,char**);
char* exit_str = "exit";

int main(){
  char process_name = 1;
  char input[SIZE];
  while(1){
    printf(">>>");
    gets(input);

    int spaces=get_number_of_spaces(input); // Needed to get total number of args.

    char **args=(char**)calloc(spaces+2,sizeof(char*)); // last character is null.
    args[0] = input;
    args[spaces+1] = NULL; //Args array must terminate with a null pointer.
    get_args(input,args); // Now we fill the args array.
    execute_command(input,args,spaces+1);
  }
  return 0;
}
int get_number_of_spaces(char* str ){
  int spaces = 0;
  for (int i = 0; i<SIZE ; i++){
    if (str[i] == '\0'){
      break;
    }
    if (str[i] == ' '){
      spaces++;
    }
  }
  return spaces;
}
void get_args(char* str,char**args){
  char *ptr = str;
  int count = 1;
  char space_found =0;
  while(*ptr != '\0'){
    if(*ptr == ' '){
        *ptr = '\0';//Separate args by null characters.
        space_found =1;
    }
    else if(space_found){
      args[count] = ptr;
      count++;
      space_found =0;
    }
      ptr++;
  }

}
void execute_command(char* input ,char **args,int length ){
  char wait_arg = 1;
  for(int i = 1;i<length;i++)
    if(strchr(args[i],'&') != NULL){//Don't wait for child if found.
      args[i] = NULL;
      printf("Don't Wait!\n");
      wait_arg =0;
    }
  if (strcmp(input,exit_str) == 0){ // Exit the shell
    printf("Exiting ...\n");
    sleep(1.5);
    exit(1);
  }
  int pid=fork();
  if (!pid){
    sleep(2); // Needed to be sure that parent waits.
    int done=execvp(input,args);
    if(done ==-1)
      printf("Error!\n");
  }
  else{
    signal(SIGCHLD,SIG_IGN); // To avoid Zombie processes.
    if(wait_arg){
      printf("Waiting for process ...\n");
      // wait(NULL);
      waitpid(pid,0,WUNTRACED);
  }
  }
}
