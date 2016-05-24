#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/sysmacros.h>

int main (int argc, char* argv[]) 
{
	int x,min;
	dev_t dt;
	char filename[100],name[50],path[100],user[50],temp[50];
	FILE *fp;
	min = 0;
	strcpy(path,"/home/");
	getlogin_r(user,50);
	strcat(path,user);
	strcat(path,"/cryptodev/modentry.dat");
	fp = fopen(path , "a+");
	while(fscanf(fp,"%s\n",temp)!=EOF)
	{
		min++;
	}
	fclose(fp);
	dt = makedev(250,min);
	strcpy(filename,"/home/cryptomod/");
	if(argc>1)
	{
		strcpy(name,argv[1]);
		strcat(filename,name);
		x = mknod(filename,S_IFCHR | 0644,dt);
	}
	/*strcpy(path,"/home/");
	getlogin_r(user,50);
	strcat(path,user);
	strcat(path,"/modentry.dat");
	fp = fopen(path , "a+");
	while(fscanf(fp,"%s\n",temp)!=EOF)
	{
		strcpy(filename,"/dev/");
		strcat(filename,temp);
		x = mknod(filename,S_IFCHR | 0644,dt);
	}
	fclose(fp);*/
	uid_t real = getuid();
	seteuid(real);
	return 0;
}
