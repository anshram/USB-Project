#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdint.h>
	char a[20];
typedef struct devData
{
        uint32_t a;

        uint32_t b;
}devData;

int main(int argv, char *argc[])
{
	char hel[20];
	devData *dev;
	int d = open("/dev/USBDev250",O_RDWR);
	if(!d)
		exit(1);

		

	write(d,argc[1],sizeof(argc[1])+1);
//	write(d,"x",1);
//while(	!read(d,a,20));
//	printf("%s",a);i
	read(d,hel , 1);
	dev = (devData *)hel;
//	printf("%d , %d",dev->a,dev->b);
	printf("%d\n",hel[0]);
	return 0;
}
