#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#define BUFFER_SIZE 1024


 
int main()
{
  /* create TCP client socket (endpoint) */
  int sd = socket( PF_INET, SOCK_STREAM, 0 );

  if ( sd == -1 )
  {
    perror( "socket() failed" );
    exit( EXIT_FAILURE );
  }

#if 0
  struct hostent * hp = gethostbyname( "localhost" );  /* 127.0.0.1 */
  struct hostent * hp = gethostbyname( "127.0.0.1" );
  struct hostent * hp = gethostbyname( "128.113.126.29" );
#endif

  struct hostent * hp = gethostbyname( "127.0.0.1" );

  if ( hp == NULL )
  {
    fprintf( stderr, "ERROR: gethostbyname() failed\n" );
    return EXIT_FAILURE;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  memcpy( (void *)&server.sin_addr, (void *)hp->h_addr, hp->h_length );
  unsigned short port = 2333;
  server.sin_port = htons( port );

  printf( "server address is %s\n", inet_ntoa( server.sin_addr ) );


  printf( "connecting to server.....\n" );
  if ( connect( sd, (struct sockaddr *)&server, sizeof( server ) ) == -1 )
  {
    perror( "connect() failed" );
    return EXIT_FAILURE;
  }


  char cache[1024];
  char buffer[ BUFFER_SIZE ];
  
  while(1){
	printf("enter the instruction numbers: \n");
	//over here, 1 for log in, 2 for send self, 3 for send other, 4 for who
	int d;
	scanf("%d", &d);
	if(d == 1){
		printf("enter the arguments: \n");
		char* name = calloc(1024, 1);
		char* arg = calloc(1024, 1);
		scanf("%s %s",arg, name);
		sprintf(cache, "%s %s\n",arg, name);
		write(sd, cache, strlen(cache));
		read(sd, buffer, 1024);
		printf("recive from server: [%s]\n", buffer);
		free(name);
		free(arg);
	}
	else if(d == 4){
		write(sd, "WHO\n", 4);
		read(sd, buffer, 1024);
		printf("recive from server: [%s]\n", buffer);
	}
	else if( d == 5){
		write(sd, "LOGOUT\n", 7);
		read(sd, buffer, 1024);
		printf("recive from server: [%s]\n", buffer);
	}
	else{
		printf("enter the arguments: \n");
		char* reciver = calloc(1024, 1);
		char* len = calloc(1024, 1);
		char* arg = calloc(1024, 1);
		char* text = calloc(1024, 1);
		scanf("%s %s %s %s",arg, reciver, len, text);
		sprintf(cache, "%s %s %s\n%s", arg, reciver, len, text);
		write(sd, cache, strlen(cache));
		read(sd, buffer, 1024);
		printf("recive from server: [%s]\n", buffer);
		if(d == 2){
			read(sd, buffer, 1024);
			printf("recive from server: [%s]\n", buffer);
		}
		free(reciver);
		free(len);
		free(arg);
		free(text);
	}
  }
  
  /* The implementation of the application layer protocol is below... */
  
  
  
  /*
  FILE* fp;
  fp = fopen("legend.txt", "r");
  
  if(fp == NULL){
	  fprintf(stderr, "ERROR: open file failed\n");
	  return EXIT_FAILURE;
  }
  
  fseek(fp, 0, SEEK_END);
  long int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  
  char cache[1024];
  char buffer[ BUFFER_SIZE ];
  
  char msg[1024];
  sprintf(msg, "LOGIN LEIJUN\n");
  
  int n = write( sd, msg, strlen( msg ) );    
  read( sd, buffer, BUFFER_SIZE - 1 );
  printf( "Rcvd from server: %s\n", buffer );  
  
  sprintf(msg, "SHARE XIAOMI %ld\n", size);
  printf("%s", msg);
  n = write(sd, msg, strlen(msg));
  read(sd, buffer, BUFFER_SIZE-1);
  printf( "Rcvd from server: %s\n", buffer );


  int count = 0;
  
  do{	
		int tc = 1024;
		for(int i = 0; i < 1024; i++){
			char t = fgetc(fp);
			cache[i] = t;
			if( t == 0){
				tc = i;
				tc += 1;
			}
		}
		
		
		n = write( sd, cache, 1024 );
		count += tc;
		  
		n = read( sd, buffer, BUFFER_SIZE - 1 );   

		if ( n == -1 )
		{
			perror( "read() failed" );
			return EXIT_FAILURE;
		}
		else if ( n == 0 )
		{
			printf( "Rcvd no data; also, server socket was closed\n" );
		}
		else  
		{
			buffer[n] = '\0';    
			printf( "Rcvd from server: %s\n", buffer );
		}
  }
  while(count < size);
 
  */

  close( sd );

  
  return EXIT_SUCCESS;
}
