#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/select.h> 

//HERE is the global variables for server
char* clients[32];
int socketArray[32];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	int fd;
	int clientNo;
	
} clientInfo;

unsigned short int checkInt(char* input){
	/*
		this is the helper function from HW3
		this function will check that is the input
		a valid unsigned int and is in the range
		of unsigned short
		
		over here, 0 is a invalid port number
	*/
	if(strlen(input) >= 6){
		//all out of bound values will be ignored
		return 0;
	}
	
	for(int i = 0; i < strlen(input); i++){
		//all negative values will be ignored
		if(isdigit(input[i]) == 0){
			//fprintf(stderr, "the error char is [%c]\n", input[i]);
			return 0;
		}
	}
	
	int temp = atoi(input);
	
	if( temp > 65535 ){
		//check for five digits numbers
		return 0;
	}
	
	return ((unsigned short int)temp);
	
}


void * TCPService(void * arg);

int main( int argc, char* argv[]){
	
	//here is the fd set to keep udpport and tcpport
	fd_set readfds;
	
	//check the input
	if( argc != 2 ){
		fprintf( stderr, "Main : ERROR Invalid argument(s)\n" );
		return EXIT_FAILURE;
	}
	
	//get the input
	if( checkInt(argv[1]) == 0 ){
		//check is the input a valid number
		fprintf( stderr, "Main : ERROR Invalid argument(s)\n" );
		return EXIT_FAILURE;
	}
	
	//if valid, we will assign this number 
	//as the port number we will use
	unsigned short int port = checkInt(argv[1]);
	
	//create the TCP socket
	int TCPSock = socket(PF_INET, SOCK_STREAM, 0);
	
	if( TCPSock < 0 ){
		fprintf(stderr, "MAIN: ERROR failed to create socket\n");
		return EXIT_FAILURE;
	}
	
	struct sockaddr_in TCPServer;
	TCPServer.sin_family = PF_INET;
	TCPServer.sin_addr.s_addr = htonl( INADDR_ANY );
	TCPServer.sin_port = htons( port );
	int TCPLen = sizeof( TCPServer );
	
	if( bind( TCPSock, (struct sockaddr *)&TCPServer, TCPLen ) < 0 ){
		fprintf(stderr, "MAIN: ERROR Bind socket to port failed\n");
		return EXIT_FAILURE;
	}
	
	//create the UDP socket
	int UDPSock = socket( AF_INET, SOCK_DGRAM, 0 );
	
	if( UDPSock == -1 ){
		fprintf( stderr, "MAIN: ERROR failed to create socket\n" );
		return EXIT_FAILURE;
	}
	
	struct sockaddr_in UDPServer;
	int UDPLen;
	
	UDPServer.sin_family = AF_INET;
	UDPServer.sin_addr.s_addr = htonl( INADDR_ANY );
	UDPServer.sin_port = htons(0);
	UDPLen = sizeof( UDPServer );
	
	if( bind( UDPSock, (struct sockaddr *) &UDPServer, UDPLen ) < 0 ){
		fprintf( stderr, "MAIN: ERROR Bind socket to port failed\n" );
		return EXIT_FAILURE;
	}
	
	listen( TCPSock, 32 );
	listen( UDPSock, 32 );
	
	struct sockaddr_in client;
	int fromlen = sizeof( client );
	
	int socketPointer = 0;
	
	while(1){
		
		FD_ZERO( &readfds );
		FD_SET( TCPSock, &readfds );
		FD_SET( UDPSock, &readfds );
		
		select( FD_SETSIZE, &readfds, NULL, NULL, NULL );
		
		//check the connection type
		if( FD_ISSET( TCPSock, &readfds ) ){
			
			//if it is a TCP client
			//accept then create a new thread to deal with it
			int newSock = accept( TCPSock, (struct sockaddr *)&client, (socklen_t *)&fromlen );
			printf("accept the client, socket fd is %d\n", newSock);
			
			pthread_t tid;
			int rc;
			rc = pthread_create(&tid, NULL, TCPService, &newSock);
			if( rc != 0 ){
				fprintf(stderr, "MAIN: ERROR failed to create the thread\n");
				return EXIT_FAILURE;
			}
			
		}
		
	}
	
	return EXIT_SUCCESS;
	
}

void* TCPService( void* arg ){
	
	char* buffer = calloc( 1024, sizeof(char) );
	int newSock = *((int*) arg);
	pthread_detach(pthread_self());
	int n = 0;
	
	do{
		n = recv( newSock, buffer, 1023, 0 );
		
		if( n == -1 ){
			fprintf( stderr, "CHILD: ERROR failed to receive message\n" );
		}
		else if ( n == 0 ){
			printf( "SERVER: Rcvd 0 from recv(); closing socket...\n" );
		}
		else{
			buffer[n] = '\0';
			printf( "SERVER: Rcvd message: %s\n", buffer );
			n = send ( newSock, "ACK\n", 4, 0 ); 

		}
		
		
		
	}
	while(n > 0);
	
	close( newSock );
	free(buffer);
	
	pthread_exit( EXIT_SUCCESS );

}





