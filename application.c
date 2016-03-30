#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#define DEVICE "/dev/dummydev"

int main()
{
int fp,x;
char buff[100];
char rbuff[100];
char temp[50];
printf("Attempting to obtain File Access Rights\n");
fp = open(DEVICE , O_RDWR);
if(fp == -1)
{
	printf("%s cann't be accessed right now try after sometime\n",DEVICE);
	exit(-1);
}
printf("File Access Rights Obtained\n");
printf("\nEnter your Choice:\n1.Read\n2.Write\n");
fgets(temp,50,stdin);
sscanf(temp,"%d",&x);
switch(x)
{
	case 1	:	read(fp,rbuff,sizeof(rbuff));
			printf("Recieved from Device: %s\n",rbuff);
			break;
	case 2	:	printf("Enter any Character String to transmit to Device:");
			fgets(buff,100,stdin);
			write(fp,buff,sizeof(buff));
			printf("\nWritten: %s\n",buff);
			break;
	default	:	printf("Invalid Selection... Exiting.");
}
close(fp);
return 0;
}
