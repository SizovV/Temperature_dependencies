#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <math.h>
#include <unistd.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB1"
#define _POSIX_SOURCE 1 /* POSIX-совместимый источник */
#define FALSE 0
#define TRUE 1
#define N 512
#define READ_TEMP "#010\r"

FILE *fdf;
int fd, i, j, c, res, n, strl;
char ch[] = " ";
char cmd_temp[256], filename[256];
struct termios oldtio, newtio;
char* buf;
double vmin, vmax, vstep, imax, temp, vtemp = 0, itemp = 0, time = 0;





// Читаем <bsize> байт в GLOBAL<buf>
int readans(int bsize)
{
	if((buf = malloc(bsize)) == NULL) return -2;

	for(i = 0; i < bsize; i++)
	{
		n = read(fd, &ch, 1);
		if(n < 0) return n;
		if((n > 0) && (ch[0] != '\n') && (ch[0] != '\r'))
		{
			buf[i] = ch[0];
		}
		else
		{
			buf[i] = 0;
			return i;
		}
	}
	return (bsize + 1);
}





// Записываем команду <cmd>
int writecmd(char* cmd)
{
	printf("\nIN = %s\n", cmd);

	strl = strlen(cmd);
	j = write(fd, cmd, strl);

	if (j <= 0) return -1;
	else
	{
		for(i = 0; i < strl; i++)
		{
			if(cmd[i] == '?') return 0;
		}
		return 0; //return j;
	}
}



int ret(char* cmd)
{
	j = 0;
	n = writecmd(cmd);
	//printf("TEMP %d\n", n);
	if(n < 0)
	{
		printf("ERROR = Can't send the command\n");
		j = 1;
	}
	else if(n == 0)
	{
		//читаем N символов
		//printf("TEMP\n");
		n = readans(N);
		if(n < 0)
		{
			//ошибка чтения ответа
			printf("ERROR = Can't read answer");
			free(buf);
			j = 3;
		}
		else if(n <= N)
		{
			//полностью прочитали ответ
			printf("OUT = %s", buf);
			j = 2;
		}
		else
		{
			//прочитали только часть ответа
			printf("ERROR = N is very small (<buf> = %s)", buf);
			free(buf);
			j = 3;
		}
		printf("\n");
	}
	return j;
}



int main()
{	char cmd[256];

	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
	if(fd < 0)
	{
		perror(MODEMDEVICE);
		return fd;
	}
	tcgetattr(fd, &oldtio);		/* сохранение текущих установок порта */
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag		= BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag		= IGNPAR;
	newtio.c_oflag		= 0;
	newtio.c_lflag		= 0;	/* set input mode (non-canonical, no echo,...) */
	newtio.c_cc[VTIME]	= 0;	/* посимвольный таймер не используется */
	newtio.c_cc[VMIN]	= 1;	/* блокировка чтения до тех пор пока не будет принят 1 символ */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	printf("DATA filename = ");
	scanf("%s", filename);
	while((fdf = fopen(filename, "w+")) == NULL)
	{
		printf("ERROR = cannot open or create file %s", filename);
		printf("INPUT filename = ");
		scanf("%s", filename);
	}

	fdf = fopen(filename, "a");
	for(int counter = 0; counter < 1000; counter++)
	{
		ret(READ_TEMP);
		if (j == 2) {
			fprintf(fdf, "%s\n", buf+1);
			fflush(fdf);
		}
		sleep(1);
	}

	/*
	printf("INPUT filename = ");
	scanf("%s", filename);
	while((fdf = fopen(filename, "w+")) == NULL)
	{
		printf("ERROR = cannot open or create file %s", filename);
		printf("INPUT filename = ");
		scanf("%s", filename);
	}
	*/

	tcsetattr(fd, TCSANOW, &oldtio);
	fclose(fdf);
	close(fd);

	return 0;
}
