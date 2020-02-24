#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


FILE *fdf;

int main(int argc, char **argv)
{
    int sd;
    int port;
    int rval;
    int reply_length;
    struct hostent *hostaddr;
    struct sockaddr_in servaddr;

    port = 4001;

    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sd == -1)
    {
        perror("Socket()\n");
        return (errno);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    hostaddr = gethostbyname("192.168.6.2");

    memcpy(&servaddr.sin_addr, hostaddr->h_addr, hostaddr->h_length);

    rval = connect(sd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(rval == -1)
    {
        printf("Port %d is closed\n", port);
        //close(sd);
    }
    else printf("Port %d g open\n", port);

	char *strk = "";
	char *msg = "#010\r";
    char servaddr_reply[20];
    char part1[] = "";
    char part2[] = "";
    fdf = fopen("kekich.txt", "a");

    while(1)
    {
    	printf("HUY");
		if(send(sd, msg, strlen(msg), 0) < 0)
    	{
    		printf("Send failed%s \n", strk);
    	return 1;
    	}

    // server reply
    	do
    	{
			reply_length = recv(sd, servaddr_reply, 4, 0);
    		if(reply_length < 0)
    		{
    			printf("Recv failed %s \n", strk);
    			break;
    		}
    		/*for(int i = 0; i < 4; i++)
  			{
  				char tmp = servaddr_reply[i];
  				strcat(temp, &tmp);
  			}*/
    		printf("Reply length: %d\n", reply_length);
    		printf("%c%c%c%c\n", servaddr_reply[0], servaddr_reply[1], servaddr_reply[2], servaddr_reply[3]);
    		printf("%s\n", servaddr_reply);
    		if (reply_length == 4)
    		{
    			for (int i = 0; i < 4; i++)
    			{
    				if (servaddr_reply[i] != '>')
    				{
    					fprintf(fdf, "%c", servaddr_reply[i]);
						fflush(fdf);
					}
				}
			}

    	} while((reply_length != 1) && (servaddr_reply[0] != '\x0D'));
    	//printf("Temperature: %s\n", temp);
    	sleep(1);
    	fprintf(fdf, "\n");
    	system("/home/student/1.h");
    }
    fclose(fdf);
    close(sd);

    return 0;
}
