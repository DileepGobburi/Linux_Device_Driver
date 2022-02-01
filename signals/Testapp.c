#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include <signal.h>

#define MAX_SIZE 1024
int8_t write_buf[MAX_SIZE];
int8_t read_buf[MAX_SIZE];

#define WR_VALUE _IOW('a','a',int*) 
#define RD_VALUE _IOR('a','b',int*)
//****************************SIGNALS************************
#if 0
#define SIGETX 44
#endif
#define SIGETX 44
#define REG_CURRENT_TASK _IOW('a','c',int32_t*)
//***********************************************************

static int done = 0;
int check = 0;
 
void ctrl_c_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGINT) {
        printf("\nrecieved ctrl-c\n");
        done = 1;
    }
}
 
void sig_event_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGETX) {
        check = info->si_int;
        printf ("Received signal from kernel : Value =  %u\n", check);
    }
}
int main()
{
	int fd;
	char option;
	int number = 0, value = 0;
	struct sigaction act;
	fd = open("/dev/my_signal_driver",O_RDWR);
	if(fd <0)
	{
		printf("can not open device");
		return 0;
	}
	 
    /* install ctrl-c interrupt handler to cleanup at exit */
    sigemptyset (&act.sa_mask);
    act.sa_flags = (SA_SIGINFO | SA_RESETHAND);
    act.sa_sigaction = ctrl_c_handler;
    sigaction (SIGINT, &act, NULL);
 
    /* install custom signal handler */
    sigemptyset(&act.sa_mask);
    act.sa_flags = (SA_SIGINFO | SA_RESTART);
    act.sa_sigaction = sig_event_handler;
    sigaction(SIGETX, &act, NULL);
 
    printf("Installed signal handler for SIGETX = %d\n", SIGETX);
 
	while(1)
	{
		printf("****Please Enter the Option******\n"); 
		printf("        1. Write               \n");
		printf("        2. Read                 \n");		
		printf("		3. IOCTL				\n");
		printf("		4. Signals				\n");
		printf("        5. Exit                 \n");
		printf("*********************************\n");
		scanf(" %c", &option);
		printf("Your Option = %c\n", option);
		
		switch(option)
		{
			case '1':
				printf("Enter the string to write into driver :"); 
				scanf("  %[^\t\n]s", write_buf);
				printf("Data Writing ...");
				write(fd, write_buf, strlen(write_buf)+1);
				printf("Done!\n");
				break;
			case '2':
			   printf("Data Reading ...");
			   read(fd, read_buf, MAX_SIZE);
			   printf("Done!\n\n");
			   printf("Data = %s\n\n", read_buf);
			   break;
			case '3':	
				 printf("Enter the Value to send\n"); 
				 scanf("%d",&number);
				 printf("Writing Value to Driver\n");
				 ioctl(fd, WR_VALUE, (int32_t*) &number); 
				 printf("Reading Value from Driver\n");
				 ioctl(fd, RD_VALUE, (int32_t*) &value);
				 printf("Value is %d\n", value);
				break;
			case '4':
				 printf("Registering application ...");
				 /* register this task with kernel for signal */
	   			 if (ioctl(fd, REG_CURRENT_TASK,(int32_t*) &number)) {
					printf("Failed\n");
					close(fd);
					exit(1);
				}
	 			 printf("Done!!!\n");


			    while(!done) {
					printf("Waiting for signal...\n");
			 
					//blocking check
					while (!done && !check);
					check = 0;
				}
				break;
		   case '5':
				   close(fd);
				   exit(1);
				   break;
		   default:
				   printf("Enter Valid option = %c\n",option);
				   break;
		}
	
	}
	close(fd);
}
