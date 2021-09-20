#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/ioctl.h>
#include<errno.h>

#define MY_MAGIC 'a'
#define WR_VALUE _IOW(MY_MAGIC,'a',int32_t*)
#define RD_VALUE _IOR(MY_MAGIC,'b',int32_t*)
#define MY_IOCTL_MAX_CMD 2

int main()
{
	int fd;
    int32_t value, number;	
	
	printf("[%d] - Opening device my_cdrv\n", getpid() );
	
	fd = open( "/dev/my_Ioctl_driver", O_RDWR );
	if( fd < 0 ) {
		perror("open\n");
		return 1;
	}
	printf("Device opened with ID [%d]\n", fd);
	
	   
	printf("Enter the Value to send\n");
	scanf("%d",&number);
	printf("Writing Value to Driver\n");
	ioctl((fd, WR_VALUE, (int32_t*) &number)<0) {
  perror("ioctl");
}

	printf("Reading Value from Driver\n");
	ioctl((fd, RD_VALUE, (int32_t*) &value)<0))
{ 
	perror("ioctl");

}
	printf("Value is %d\n", value);

	printf("Closing Driver\n");
	if(close(fd)<0){
			perror("close");
 			}
	
	
	
	exit(0);
}



