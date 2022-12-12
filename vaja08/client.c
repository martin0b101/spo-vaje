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

void main( int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;	// naslov strežnika
	//char buf[BUFSIZE], sendbuf[BUFSIZE];
	int n;

	if ( argc != 2) {
		printf("Uporaba: %s TCP_vrata\n", argv[0]);
		exit(0);
	}
	
	if ( (sockfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)
		err("socket")
		
	bzero( &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));
	if ( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
		err("pton")
	
	if ( connect( sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		err("connect")

	char sendbuf[BUFSIZE];
	char recvbuf[BUFSIZE];

	while ( read( STDIN_FILENO, sendbuf, sizeof sendbuf) > 0 ) {
		if ( write( sockfd, sendbuf, strlen(sendbuf)) != strlen(sendbuf) ) err("write")

		if( read( sockfd, recvbuf, sizeof recvbuf) > 0 ) { printf("%s", recvbuf); }

		// clear buffers
		memset(&sendbuf, 0, sizeof(sendbuf));
		memset(&recvbuf, 0, sizeof(recvbuf));
	}



}