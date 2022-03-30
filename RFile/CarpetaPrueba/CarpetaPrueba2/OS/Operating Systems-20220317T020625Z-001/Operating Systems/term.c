#include<stdio.h> 
#include<unistd.h> 
#include<sys/wait.h> 
#include<sys/types.h> 
  
void func(int signum) { 
    wait(NULL); 
} 
  
int main() { 
    int i; 
    int pid = fork(); 
    if (pid == 0) {
        for (i=0; i<5; i++) 
        printf("I am a child\n"); 
    } else { 
        signal(SIGCHLD, func); 
        printf("I am a parent\n"); 
        while(1); 
    } 
} 