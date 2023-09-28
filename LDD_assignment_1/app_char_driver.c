#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define EXIT   2
#define B_SIZE 20

int main()
{
	int fd, retval, option;
	char buff[B_SIZE], str[B_SIZE];

	fd = open("/dev/mydriver_file", O_RDWR);
	/*printf("fd : %d\n",fd);*/
	if (0 > fd) {
		printf("open fialed\n");
		
		return -1;
	}
	while (1) {
		printf("\n0-read \n1-write \n2-exit\n select option : ");
		__fpurge(stdin);
		scanf("%d", &option);
		
		if (EXIT == option)
			break;

		switch (option) {

		case 1:
			 printf("enter the string : ");
			 __fpurge(stdin);
			 scanf("%[^\n]s", str);

			 retval = write(fd, str, strlen(str));
			 if (0 > retval) {
				 printf("write : %d \n", retval);
				 printf("write call failed\n");
				 return -2;
			 }
			/* printf("write : %d \n", retval); */
			 break;

		case 0:
			 retval = read(fd, buff, sizeof(buff)-1);
			 if (0 > retval) {
				 printf("read call failed \n");
				 return -4;

			 }
			/* printf("read : %d \n", retval);*/
			 
			 buff[retval] = '\0';
			 printf("%s\n", buff);
			 break;

		default:
			 printf("select valid option\n");
		}
	}
	close(fd);
	
	return 0;
}
