/*
	Port Scanner in C: 
	http://www.binarytides.com/tcp-connect-port-scanner-c-code-linux-sockets/
	*Changes needed to be made as the code from the site did not compile.
*/
#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char **argv)
{
	struct hostent *host;
	int err, i, sock, start, end;
	char hostname[100];
	struct sockaddr_in sa;

	//Get the hostname.  needs to be an argument
	printf("Enter hostname or IP : ");
	gets(hostname); //hostname needs to be an argument

	//Get start port number
	printf("\nEnter start port number: ");
	scanf("%d", &start);

	//Get end port number
	printf("\nEnter end port number: ");
	scanf("%d", &end);

	//Initialize socket
	strncpy((char*)&sa , "", sizeof sa);
	sa.sin_family = AF_INET;

	//direct ip address, use it
	if(isdigit(hostname[0]))  
	{
		printf("Doing inet_addr...");
		sa.sin_addr.s_addr = inet_addr(hostname);
		printf("Done\n");
	}
	//Resolve hostname to IP address
	else if ( (host = gethostbyname(hostname)) !=0)
	{
		printf("Doing gethostbyname...");
		strncpy((char*)&sa.sin_addr , (char*)host->h_addr , sizeof sa.sin_addr);
		printf("Done\n");
	} 
	else
	{
		herror(hostname);
		exit(2);
	}

	//Start the port scan loop.  Maybe do an array here instead
	printf("Starting the portscan loop : \n");
	for( i = start ; i <= end ; i++)
	{
		//Fill in the port number
		sa.sin_port = htons(i);
		//Create a socket of type internet
		sock = socket(AF_INET, SOCK_STREAM , 0);
		//Check whether socket created fine or not
		if(sock < 0)
		{
			perror("\nSocket");
			exit(1);
		}
		//Connect using that socket and sockaddr strucuture
		err = connect(sock , (struct sockaddr*)&sa , sizeof sa);

		//not connected
		if(err < 0)
		{
			//pritnf("%s %-5d %s\r" , hostname , i, strerror(errno));
			fflush(stdout);
		}
		//connected
		else
		{
			printf("%-5d open\n", i);
		}
		close(sock);
	}
	printf("\r");
	fflush(stdout);
	return(0);
}