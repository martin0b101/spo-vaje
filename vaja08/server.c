#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#define err(mess) { fprintf(stderr,"Napaka: %s.\n", mess); exit(1); }

#define BUFSIZE 1024
#define LISTENQ 1024


void main( int argc, char *argv[])
{
	int sockfd, connfd;	// sockfd je listening socket
	struct sockaddr_in servaddr;	 // naslov strežnika
	//char buf[BUFSIZE], sendbuf[2048];
	int n;

	if ( argc != 2) {
		printf("Uporaba: %s TCP_vrata\n", argv[0]);
		exit(0);
	}
	
	if ( (sockfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)		// isto kot pri odjemalcu
		err("socket")
		
	bzero( &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));	
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ( bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		err("bind")

	if ( listen(sockfd, LISTENQ) < 0)
		err("listen")

	while(1)
	{
		if ( (connfd = accept( sockfd, (struct sockaddr *) NULL, NULL)) < 0) // blokira, dokler se ne poveže nek odjemalec
			err("accept")
			

		// send, reply
		char sendbuf[BUFSIZE], recvbuf[BUFSIZE];


		while ( (n = read(connfd, recvbuf, BUFSIZE)) > 0)
		{
			
			float num1, num2, result = 0;
			char operation[BUFSIZE];
			
			sscanf( recvbuf, "%f %s %f", &num1, operation, &num2 );
			if (operation[0] == '+') { result = num1 + num2; }
			if (operation[0] == '-') { result = num1 - num2; }
			if (operation[0] == '*') { result = num1 * num2; }
			if (operation[0] == '/') { result = num1 / num2; }
		
			
			sprintf( sendbuf, "= %.2f\n", result);
			if ( write( connfd, sendbuf, strlen(sendbuf)) != strlen(sendbuf))
				err("write")

			// clear buffers
			memset(&sendbuf, 0, sizeof(sendbuf));
			memset(&recvbuf, 0, sizeof(recvbuf));
		}

		//close conn
		if(close(connfd))
			err("close")
	}

	if (close(sockfd))
		err("close socket")
}