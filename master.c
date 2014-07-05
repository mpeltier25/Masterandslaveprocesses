#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

volatile sig_atomic_t keep_going = 1;

void catch_alarm (int sig)
{
       keep_going = 0;
       printf("Alarm Triggered\n");
       signal (sig, catch_alarm);
}
          
int main(int argc, char *argv[])
{
	int pid;
	signal (SIGALRM, catch_alarm);
     	alarm (3);
	int i;
	int j;
	int summation=0;
	int seed;
	int key=0;
	char string1[argc];
	char string2[argc];
	int argcount = argc-1;
	
	int temp[argc];
	char  *procpath = "./slave";
	char  *procname = "slave";

while (keep_going)
{
 				
	for (i=1;i<argc;i++) //fill new array with int values from argv
		{
		temp[i-1] = atoi(argv[i]);
		}
		
	if (argcount&1) //deal with uneven arguments.
		{
		temp[argcount]=0;
		argcount++;
		seed=(argcount)/2;
		}
	  else
		{
		seed = argcount/2;
		}
		
	while (seed>0)//manage layers of addition
	{
		
	for (i=0;i<argcount;i+=2)
		{
		
		pid = fork();
		
   		if (pid == 0)
			{
			int temp1 = temp[i];
			int temp2 = temp[i+1];
			
			sprintf(string1,"%d",temp1);
			sprintf(string2,"%d",temp2);
			
			execl(procpath, procname,string1,string2,(char*)0);
			
			perror("execl failed to run slave program");
			exit(1);
			}
			
    		else if (pid < 0 )
     			{
    			printf("call to fork failed, no child\n");
  			exit(-1);
    			}
    			 
    		else
  			{
			//catch exit status with wait
         		int status;
			pid_t pidd;
			pidd = wait(&status);
			int returnvalue = WEXITSTATUS(status);
 			printf("Parent: Child with PID %ld returned partial sum %d.\n", (long)pidd, returnvalue );
 			
			temp[key]=returnvalue;
			summation=returnvalue;
			if (argcount>2)//prevents infinite loop
				key++;
			seed--; //tracks level of addition
			
			if (seed == 0)
				{
				argcount = key;
				if (argcount&1) {argcount++;}
						
				for (j=key;j<argc;j++)
				{
					temp[j]=0;
				}
				
				
				seed = argcount/2;
				key = 0;
				}
			
			}
		}
	}
	printf("Sum: %d\n",summation);
	keep_going=0;
	
}
		
	return 0;
}
