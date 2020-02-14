#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int sd;
    int port;
    int rval;
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

	//printf("Port %d g open\n", port);
	//printf("Enter msg: %s \n", strk);
    char servaddr_reply[20];

    //while(1)
    //{
    //
    //scanf("%s" , msg);

    char *msg = "#010\r";
    //printf("HUY");

	if( send(sd, msg , strlen(msg) , 0) < 0)
    {
    printf("Send failed%s \n", strk);
    //return 1;
    }
    else printf("SEND SUX%s \n", strk);
    // server reply
    if( recv(sd, servaddr_reply , 2000 , 0) < 0)
    {
    printf("Recv failed %s \n", strk);
    //break;
    }
    printf("Echo: %s \n", strk);
    printf("%s \n", servaddr_reply);
    //}
    close (sd);



    return 0;
}
