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
//the client name and the fd num of that client should have same index
char* clients[32];
int socketArray[32];
int socketPointer = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	int fd;
	int clientNo;
	
} clientInfo;

typedef struct{
	char** argv;
	int argc;
} argument;

//helper functions
void printClients(){
	pthread_mutex_lock(&mutex);
	for(int i = 0; i < 32; i++){
		if(clients[i] != NULL){
			printf("[%s] ", clients[i]);
		}
	}
	printf("\n");
	pthread_mutex_unlock(&mutex);
}

void printSockets(){
	pthread_mutex_lock(&mutex);
	for(int i = 0; i < 32; i++){
		printf("[%d] ", socketArray[i]);
	}
	printf("\n");
	pthread_mutex_unlock(&mutex);
}

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

int checkPos( int fd ){
	//NOTE: this function should be run under mutex lock
	int pos = -1;
	for(int i = 0; i < 32; i++){
		if(socketArray[i] == fd){
			pos = i;
			break;
		}
	}
	
	return pos;	
}

int checkNameExist( char* name ){
	int result = 0;
	//NOTE: this function should be run under mutex lock
	for(int i = 0; i < 32; i++){
		if(clients[i] != NULL){
			if(strcmp(name, clients[i]) == 0){
				result = 1;
				break;
			}
		}
		
	}
	
	return result;
}

int checkNameValid( char* name ){
	//we will check the name here
	if(strlen(name) < 4 || strlen(name) > 16){
		printf("the invalid name is [%s]\n", name);
		return -1;
	}
	for(int i = 0; i < strlen(name); i++){
		if(isalnum( name[i] ) == 0){
			//failed the alnum test
			printf("the invalid name is [%s]\n", name);
			return -1;
		}
	}
	
	return 0;
}


argument* readCommand( char* input ){
	/*
		this function will be used to read the 
		first line of the command
	*/
	char* token;
	
	//create the result
	argument* result = calloc(1, sizeof(argument));
	result -> argv = calloc(10, sizeof(char*));
	result -> argc = 0;
	
	token = strtok(input, " ");
	//if read nothing
	if(token == NULL){
		free(result->argv);
		free(result);
		return NULL;
	}
	
	while( token != NULL ){
		char* temp = calloc( 64, sizeof(char) );
		temp = strcpy(temp, token);
		result->argv[result->argc] = temp;
		result->argc += 1;
		token = strtok( NULL, " " );
	}
	
	return result;
}

int commandHandeler( argument* arg ){
	//make sure the input is valid
	if(arg == NULL){
		//invalid argument, there is no infomation
		return -1;
	}
	
	//check the command
	if(strcmp(arg->argv[0], "LOGIN") == 0){
		return 1;
	}
	
	//this should be UNKNOWN Command
	return 0;
}



int login( char* name, int fd ){
	//this function will do the login
	int pos;
	
	pthread_mutex_lock(&mutex);
	//IMPORTANT: over here, cause the client will change
	//so we will get the index by fd number every time 
	//we want to access name
	
	pos = checkPos(fd);
	
	if(pos == -1){
		pthread_mutex_unlock(&mutex);
		//return 0 means fd is not found in fd array
		return -1;
	}
	
	if( checkNameValid(name) == -1 ){
		pthread_mutex_unlock(&mutex);
		//return 1 means name is not valid
		return 1;
	}
	
	if( checkNameExist(name) == 1 ){
		pthread_mutex_unlock(&mutex);
		//return 2 means name is occupied
		return 2;
	}
	
	//if passed all the test
	//add the name to the pos, with the same index
	//of the fd num that client has
	char* temp = calloc(17, sizeof(char));
	temp = strcpy(temp, name);
	clients[pos] = temp;
	
	pthread_mutex_unlock(&mutex);
	
	//this means the 
	return 0;
	
}




void freeArgument( argument* arg ){
	for(int i = 0; i < 10; i++){
		free(arg->argv[i]);
	}
	free(arg->argv);
	free(arg);
}

void * TCPService(void * arg);

int main( int argc, char* argv[]){
	printf("MAIN: Started server\n");
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
	
	printf("MAIN: Listening for TCP connection on port: %d\n", port);
	printf("MAIN: Listening for UDP connection on port: %d\n", port);
	
	struct sockaddr_in client;
	int fromlen = sizeof( client );
	
	
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
			printf("MAIN: Rcvd incoming TCP connection from: %s\n",
			inet_ntoa( client.sin_addr ));
			
			clientInfo ci;
			ci.fd = newSock;
			ci.clientNo = socketPointer;
			
			pthread_t tid;
			int rc;
			
			pthread_mutex_lock(&mutex);
			socketArray[socketPointer] = newSock;
			socketPointer += 1;
			pthread_mutex_unlock(&mutex);
			
			rc = pthread_create(&tid, NULL, TCPService, &ci);
			if( rc != 0 ){
				//it is safe to add fd table first, because if this 
				//chunk of code is run, the whole program will be 
				//terminated and we don't need to worry about the
				//not working fd 
				//
				//also, we should add the fd first, or there maybe 
				//a possibility that, the child thread will access
				//to the socketArray without the target fd table in
				//that and cause some problem
				fprintf(stderr, "MAIN: ERROR failed to create the thread\n");
				return EXIT_FAILURE;
			}
			
		}
		
	}
	
	return EXIT_SUCCESS;
	
}

void* TCPService( void* arg ){
	
	char* buffer = calloc( 1024, sizeof(char) );
	clientInfo ci = *((clientInfo*) arg);
	int newSock = ci.fd;
	
	pthread_detach(pthread_self());
	int n = 0;
	
	do{
		//read the instruction
		n = recv( newSock, buffer, 1023, 0 );
		
		if( n == -1 ){
			fprintf( stderr, "TCP Client #%d: ERROR failed to receive message\n", 
			ci.clientNo+1 );
		}
		else if ( n == 0 ){
			printf( "CHILD %d: Rcvd 0 from recv(); closing socket...\n", 
			ci.clientNo+1 );
		}
		else{
			//over here, we will move n up a bit 
			//so we will overwrite that \n
			buffer[n-1] = '\0';
			argument* arg = readCommand( buffer );
			int result = commandHandeler(arg);
			printf("the handeler result is [%d]\n", result);
			
			if( result == 1 ){
				//if log in request
				printf("CHILD %d: Rcvd LOGIN request for userid %s\n",
				ci.clientNo + 1, arg->argv[1] );
				int lr = login(arg->argv[1], ci.fd );
				
				//check the login result 
				if(lr == 2 ){
					//name is uesd
					printf("CHILD %d: ERROR Already connected\n", ci.clientNo+1);
				}else if(lr == 1){
					//name is not valid
					printf("CHILD %d: ERROR Invalid userid\n", ci.clientNo+1);
				}else{
					//success
					n = send(newSock, "OK!\n", 4, 0);
				}
			}
			else if(result == 0){
				printf("CHILD %d: ERROR Invalid argument\n", ci.clientNo+1);
			}
			
			//after reading, free the argument
			if(result != -1){
				freeArgument(arg);
			}
			

		}
		
		/*
		printf("now the clents are: \n");
		printClients();
		printf("now the Sockets are: \n");
		printSockets();
		*/
		
	}
	while(n > 0);
	
	//need to do more specific memory clean here
	
	pthread_mutex_lock(&mutex);
	//over here, we do the decreament to the
	//two global arrays
	int tp = checkPos( newSock );
	
	close(newSock);
	free(clients[tp]);
	
	for(int i = tp; i < (socketPointer-1); i++){
		clients[i] = clients[i+1];
		socketArray[i] = socketArray[i+1];
		clients[socketPointer] = 0;
		socketArray[socketPointer] = 0;
		socketPointer -= 1;
	}
	
	pthread_mutex_unlock(&mutex);
	
	free( buffer );
	
	pthread_exit( EXIT_SUCCESS );

}





