#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main( int argc, char *argv[])
{
	//Convert strings to ints
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int c = a+b ;
	pid_t pid=getpid();
	printf("Slave: My PID is %ld and args are %d, and %d\n",(long)pid,a,b);
	exit(c);	
}

