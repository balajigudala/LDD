#include<stdio_ext.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>

#define EXIT 0
#define BASIC_MAGIC 'a'
#define RD_VALUE _IOR(BASIC_MAGIC, 1, char*)
#define WR_VALUE _IOW(BASIC_MAGIC, 2, char*)

int main()
{
	int fd, number, retval;
	char buff[20];

	fd = open("/dev/mydev_file", O_RDWR);
	if (0 > fd) {
		printf("Cannot open device file...\n");
		return -1;
	}

	while (1) {
		printf("\nEnter the Value to send \n 0 - exit \n"
			       	"1 - write \n 2 - read  \n");
		__fpurge(stdin);
		scanf("%d",&number);

		if (EXIT == number) {
			break;
		}

		switch (number) {
			case 1 :
				printf("\nWrite msg to the driver :\n");
				__fpurge(stdin);
				scanf("%[^\n]", buff);
				retval = ioctl(fd, WR_VALUE, (char *)buff);
				break;	

			case 2:
				printf("\nReading Value from Driver\n");
				retval = ioctl(fd, RD_VALUE, (char *)buff);

				printf("%s\n", buff);

				break;

			default :
				printf("select valid option\n ");
		}
	}
	close(fd);
}
