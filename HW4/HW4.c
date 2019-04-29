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
int clientPointer = 0;
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

int checkMsgLen(char* input){
	
	for(int i = 0; i < strlen(input); i++){
		if( isdigit(input[i]) == 0 ){
			return -1;
		}
		
	}
	int temp = atoi(input);
	return temp;
	
}

long int checkMsgLenLong(char* input){
	for(int i = 0; i < strlen(input); i++){
		if( isdigit(input[i]) == 0 ){
			return -1;
		}
		
	}
	long temp = atol(input);
	return temp;
}

//Viewer functions
void printClients(){
	//NOTE: this function should be run under mutex lock
	for(int i = 0; i < 32; i++){
		if(clients[i] != NULL){
			printf("[%s] ", clients[i]);
		}else{
			printf("[ ] ");
		}
	}
	printf("\n");
}

void printSockets(){
	//NOTE: this function should be run under mutex lock
	for(int i = 0; i < 32; i++){
		printf("[%d] ", socketArray[i]);
	}
	printf("\n");
}


//helper functions
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

int namePos( char* name ){
	//NOTE: this function should be run under mutex lock
	int pos = -1;
	
	for(int i = 0; i < 32; i++){
		if(clients[i] != NULL){
			if( strcmp( clients[i], name ) == 0 ){
				pos = i;
				break;
			}
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
	if(name == NULL){
		return -1;
	}
	if(strlen(name) < 4 || strlen(name) > 16){
		//printf("the invalid name is [%s]\n", name);
		return -1;
	}
	for(int i = 0; i < strlen(name); i++){
		if(isalnum( name[i] ) == 0){
			//failed the alnum test
			//printf("the invalid name is [%s]\n", name);
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
	if(strcmp(arg -> argv[0], "DEBUG" ) == 0){
		return -2;
	}
	
	//check the command
	if(strcmp(arg->argv[0], "LOGIN") == 0){
		return 1;
	}
	
	if(strcmp(arg->argv[0], "WHO") == 0){
		return 2;
	}
	
	if(strcmp(arg->argv[0], "LOGOUT") == 0){
		return 3;
	}
	
	if(strcmp(arg->argv[0], "SEND") == 0){
		return 4;
	}
	
	if(strcmp(arg->argv[0], "BROADCAST") == 0){
		return 5;
	}
	
	if(strcmp(arg->argv[0], "SHARE") == 0){
		return 6;
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
	clientPointer += 1;
	
	pthread_mutex_unlock(&mutex);
	
	//this means add success
	return 0;
	
}

int comparator(const void* a, const void* b){
	char* l = (char*)a;
	char* r = (char*)b;
	return strcmp(l,r);
}

char** getWhoArray(){
	
	pthread_mutex_lock(&mutex);
	
	//if there is no logged in client
	if(clientPointer == 0){
		pthread_mutex_unlock(&mutex);
		return NULL;
	}
	
	char** ta = calloc(clientPointer, sizeof(char*));
	int tp = 0;
	for(int i = 0; i < 32; i++){
		if(clients[i] != NULL){
			char* temp = calloc(17, sizeof(char));
			temp = strcpy(temp, clients[i]);
			ta[tp] = temp;
			tp += 1;
		}
	}
	
	qsort( (void*)ta, tp, sizeof(char*), comparator );
	pthread_mutex_unlock(&mutex);
	
	return ta;
	
}


int sendCheck(char* name, char* length){
	
	if( name == NULL || length == NULL ){
		return -1;
	}
	
	pthread_mutex_lock(&mutex);
	if( namePos(name) == -1 ){
		pthread_mutex_unlock(&mutex);
		//we didn't find that active name
		return -2;
	}
	pthread_mutex_unlock(&mutex);
	
	if( checkMsgLen( length ) == -1 || checkMsgLen( length ) > 990 ||
		checkMsgLen( length ) < 1 ){
		//the length is not valid
		return -3;
	}
	
	//finally, we find the fd we want 
	pthread_mutex_lock(&mutex);
	int pos = namePos(name);
	int fd = socketArray[pos];
	pthread_mutex_unlock(&mutex);
	
	return fd;
		
}

int broadcastCheck( char* length ){
	if( length == NULL ){
		return -1;
	}
	
	if( checkMsgLen( length ) == -1 || checkMsgLen( length ) > 990 ||
		checkMsgLen( length ) < 1 ){
		//the length is not valid
		return -2;
	}
	
	//success
	return 0;
}

int shareCheck(char* name, char* length){
	
	if( name == NULL || length == NULL ){
		return -1;
	}
	
	pthread_mutex_lock(&mutex);
	if( namePos(name) == -1 ){
		pthread_mutex_unlock(&mutex);
		//we didn't find that active name
		return -2;
	}
	pthread_mutex_unlock(&mutex);
	
	if( checkMsgLenLong( length ) == -1 ){
		//the length is not valid
		return -3;
	}
	
	//finally, we find the fd we want 
	pthread_mutex_lock(&mutex);
	int pos = namePos(name);
	int fd = socketArray[pos];
	pthread_mutex_unlock(&mutex);
	
	return fd;
		
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
	int logged = 0;
	char* name = calloc(17, sizeof(char));
	
	do{
		//read the instruction
		n = recv( newSock, buffer, 1023, 0 );
		
		if( n == -1 ){
			fprintf( stderr, "CHILD %d: ERROR failed to receive message\n", 
			ci.clientNo+1 );
		}
		//close socket
		else if ( n == 0 ){
			printf( "CHILD %d: Client disconnected\n", 
			ci.clientNo+1 );
		}
		//receive info
		else{
			//over here, we will move n up a bit 
			//so we will overwrite that \n
			//since every COMMAND will end with \n
			//it is safe to directly move up a bit
			buffer[n-1] = '\0';
			argument* arg = readCommand( buffer );
			int result = commandHandeler(arg);
			
			if( result == 1 ){
				//if log in request
				printf("CHILD %d: Rcvd LOGIN request for userid %s\n",
				ci.clientNo + 1, arg->argv[1] );
				int lr = login(arg->argv[1], ci.fd );
				
				//check the login result 
				if(lr == 2 ){
					//name is uesd
					printf("CHILD %d: Sent Error (Already connected)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Already connected\n", 24, 0 );
				}else if(lr == 1){
					//name is not valid
					printf("CHILD %d: Sent Error (Invalid userid)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Invalid userid\n",21, 0);
				}else{
					//success
					n = send(newSock, "OK!\n", 4, 0);
					//mark this client as logedin
					//this boolean is used to avoid
					//mis-deletion of name if the 
					//client has not logged in
					logged = 1;
					name = strcpy(name, arg->argv[1]);
				}
			}
			else if(result == 2 && logged == 1){
				//if received who
				printf("CHILD %d: Rcvd WHO request\n", ci.clientNo+1);
				char** tempArray = getWhoArray();
				int len = 0;
				if(tempArray != NULL){
					//if there is a valid array
					pthread_mutex_lock(&mutex);
					len = clientPointer;
					pthread_mutex_unlock(&mutex);
					
					send(newSock, "OK!\n", 4, 0);
					for(int i = 0; i < len; i++){
						send( newSock, tempArray[i], sizeof(tempArray[i]), 0 );
						send( newSock, "\n", 1, 0 );
					}
					
				}else{
					//if the array is not valid
					send(newSock, "OK!\n", 4, 0);
				}
				
				//clean up the temp array
				for(int i = 0; i < len; i++){
					free(tempArray[i]);
				}
				free(tempArray);
			}
			else if(result == 3 && logged == 1){
				//log out will only remove the name
				printf("CHILD %d: Rcvd LOGOUT request\n", ci.clientNo+1);
				logged = 0;
				free(name);
				name = calloc(17, sizeof(char));
				pthread_mutex_lock(&mutex);
				
				int tp = checkPos( newSock );
				free(clients[tp]);
				
				for(int i = tp; i < (clientPointer-1); i++){
					clients[i] = clients[i+1];
						
				}
				clientPointer -= 1;
				clients[clientPointer] = NULL;
						
				pthread_mutex_unlock(&mutex);
				send(newSock, "OK!\n", 4, 0);
				
			}
			else if(result == 4 && logged == 1){
				//here is the send command
				int r = sendCheck(arg->argv[1], arg->argv[2]);
				//handel the result
				if( r == -1 ){
					printf("CHILD %d: Sent Error (Invalid SEND format)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Invalid SEND format\n",26,0);
				}
				else if( r == -2 ){
					printf("CHILD %d: Sent Error (Unknown userid)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Unknown userid\n", 21,0);
				}
				else if( r == -3 ){
					printf("CHILD %d: Sent Error (Invalid msglen)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Invalid msglen\n",21,0);
					
				}else{
					//over here, read the info
					//create temp variables
					printf("CHILD %d: Rcvd SEND request to userid %s\n",
					ci.clientNo+1, arg->argv[1]);
					send(newSock, "OK!\n", 4, 0);
					
					int count = 0;
					int total = atoi(arg->argv[2]);
					int read = 0;
					char* tb = calloc(total, sizeof(char));
					
					//create loop to read
					do{
						read = recv(newSock, (tb+count), total, 0 );
						//printf("SERVER: i recived [%s]", tb+count);
						count += read;
					}
					while(count < total);
					//after read
					send(r, "FROM ", 5, 0);
					send(r, name, strlen(name), 0);
					send(r, " ", 1, 0);
					send(r, arg->argv[2], strlen(arg->argv[2]), 0 );
					send(r, " ", 1, 0);
					send(r, tb, total, 0);
					
					//after send, free the memory
					free(tb);
					
				}
				
			}
			//broadcast request
			else if(result == 5 && logged == 1){
				int r = broadcastCheck( arg->argv[1] );
				if( r == -1 ){
					printf("CHILD %d: Sent Error (Invalid Broadcast format)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Invalid BROADCAST format\n",31,0);
				}
				else if( r == -2 ){
					printf("CHILD %d: Sent Error (Invalid msglen)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Invalid msglen\n",21,0);
				}
				else if( r == 0 ){
					//successfully received the request
					printf("CHILD %d: Rcvd BROADCAST request\n", ci.clientNo+1);
					send(newSock, "OK!\n", 4, 0);
					
					//read the info
					int count = 0;
					int total = atoi(arg->argv[1]);
					int read = 0;
					char* tb = calloc(total, sizeof(char));
					
					//create loop to read
					do{
						read = recv(newSock, (tb+count), total, 0 );
						//printf("SERVER: i recived [%s]", tb+count);
						count += read;
					}
					while(count < total);
		
					//after read
					pthread_mutex_lock(&mutex);
					for(int i = 0; i < 32; i++){
						//check active user
						if(clients[i] != NULL){
							//if active user is there
							int tfd = socketArray[i];
							send(tfd, "BROADCAST ", 10, 0);
							send(tfd, name, strlen(name), 0);
							send(tfd, " ", 1, 0);
							send(tfd, arg->argv[1], strlen(arg->argv[1]), 0 );
							send(tfd, " ", 1, 0);
							send(tfd, tb, total, 0);
						}
					}
					pthread_mutex_unlock(&mutex);
					
					
					//after broadcast, free the memory
					free(tb);
					
				}
				
			}
			//share request
			else if(result == 6 && logged == 1){
				int r = shareCheck(arg->argv[1], arg->argv[2]);
				if( r == -1 ){
					printf("CHILD %d: Sent Error (Invalid SHARE format)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Invalid SHARE format\n",27,0);
				}
				else if( r == -2 ){
					printf("CHILD %d: Sent Error (Unknown userid)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Unknown userid\n", 21,0);
				}
				else if( r == -3 ){
					printf("CHILD %d: Sent Error (Invalid msglen)\n",
					ci.clientNo+1);
					send(newSock, "ERROR Invalid msglen\n",21,0);
				}
				else{
					printf("CHILD %d: Rcvd SHARE request to userid %s\n",
					ci.clientNo+1, arg->argv[1]);
					send(newSock, "OK!\n", 4, 0);
					
					send(r, "SHARE ", 6, 0);
					send(r, name, strlen(name), 0);
					send(r, " ", 1, 0);
					send(r, arg->argv[2], strlen(arg->argv[2]), 0 );
					send(r, "\n", 1, 0);
					
					long count = 0;
					long total = atol(arg->argv[2]);
					int read = 0;
					char* tb = calloc(1024, sizeof(char));
					
					//create loop to read
					do{
						read = recv(newSock, tb, 1024, 0 );
						send(newSock, "OK!\n", 4, 0);
						count += read;
						send(r, tb, read, 0);
					}
					while(count < total);
					
					//after send, free the memory
					free(tb);
					
				}
			}
			//unknown input
			else if(result == 0){
				//this is for unknown command
				printf("CHILD %d: Sent ERROR (Invalid argument)\n", ci.clientNo+1);
				send(newSock, "ERROR Invalid argument\n",23,0);
			}
			else if(result == -2){
				//this is for debug
				pthread_mutex_lock(&mutex);
				printf("now the clents are: \n");
				printClients();
				printf("now the Sockets are: \n");
				printSockets();
				pthread_mutex_unlock(&mutex);
				
			}
			//this branch is used to solve the clients
			//which is not connected, except NULL passed in
			else if(result != -1){
				printf("CHILD %d: Sent ERROR (Invalid argument)\n", ci.clientNo+1);
				send(newSock, "ERROR Log in first\n",19,0);
			}
			
			//after reading, free the argument
			if(result != -1){
				freeArgument(arg);
			}
			
			

		}
		
		
		
	}
	while(n > 0);
	
	//need to do more specific memory clean here
	
	pthread_mutex_lock(&mutex);
	//over here, we do the decreament to the
	//two global arrays
	int tp = checkPos( newSock );
	
	close(newSock);
	
	//if this client is logged in
	//we need to delete the name
	if(logged == 1){
		free(clients[tp]);
		for(int i = tp; i < (clientPointer-1); i++){
			clients[i] = clients[i+1];
				
		}
		clientPointer -= 1;
		clients[clientPointer] = NULL;
	}
	
	//delete the fd
	for(int i = tp; i < (socketPointer-1); i++){
		socketArray[i] = socketArray[i+1];
	}
	socketPointer -= 1;
	socketArray[socketPointer] = 0;
	pthread_mutex_unlock(&mutex);
	
	free( buffer );
	free( name );
	
	pthread_exit( EXIT_SUCCESS );

}





