#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

typedef struct{
	int x;
	int y;
} Pos;

typedef struct{
	//m is rows, n is columns
	int m;
	int n;
	int** board;
	Pos* p;
	int steps;
	
} Board;




int max_squares = 0;

Board** dead_end_boards;
int dead_end_max_length = 2;
int dead_end_pointer = 0;
int lower_bound = 0;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



//check int function to check the input
//this function is from my OS project 1
int checkInt(char* input){
	for(int i = 0; i < strlen(input); i++){
		if(isdigit(input[i]) == 0){
			//fprintf(stderr, "the error char is [%c]\n", input[i]);
			return -1;
		}
	}
	return atoi(input);
	
}

int addBoard(Board*** array, int* length, int* pointer, Board* info){
	//check is there enough space
	if( (*pointer) == (*length) ){
		//if no space
		(*length) *= 2;
		
		Board** temp = realloc(*array, (*length)*sizeof(Board*));
		if(temp == NULL){
			return -1;
		}else{
			*array = temp;
		}
		//add info
		*((*array)+(*pointer)) = info;
		*pointer += 1;
		return 1;
		
	}else if ((*pointer) < (*length)){
		*((*array)+(*pointer)) = info;
		*pointer += 1;
		return 1;
	}
	
	return -1;
}

void printBoard(Board* b, long int thread){
	for(int i = 0; i < b->m; i++){
		if( i == 0){
			printf("THREAD %ld: > ", thread);
		}else{
			printf("THREAD %ld:   ",thread);
		}
		for(int j = 0; j < b->n; j++){
			if(b->board[i][j] == 0){
				printf(".");
			}else{
				printf("S");
			}	
		}
		printf("\n");
	}
}

void boardViewer(Board* b){
	printf("the m of board is [%d]\n", b->m);
	printf("the n of board is [%d]\n", b->n);
	printf("the pos_x of board is [%d]\n", b->p->x);
	printf("the pos_y of board is [%d]\n", b->p->y);
	printf("the steps of board is [%d]\n", b->steps);
	printBoard(b, 0);
}

void freeBoard(Board* b){
	for(int i = 0; i < b->m; i++){
		free(b->board[i]);
	}
	free(b->board);
	free(b->p);
	free(b);
}

int checkInBound(int m, int n, int pos_c, int pos_r){
	if( pos_r >= 0 && pos_r < m){
		if(pos_c >= 0 && pos_c < n){
			return 1;
		}
	}
	return 0;
}


Pos** isValid(Board* b, int* returnLength){
	//this function will check the avalible
	//possibilities and put the number of 
	//possibility in an array of int
	//1 represents the c-2 r-1 condition
	//8 represents the r-2 c+1 condition
	//in clockwise
	
	Pos** result = calloc(8, sizeof(Pos*));
	int pointer = 0;
	
	//check left
	if(checkInBound(b->m, b->n, b->p->x-2, b->p->y) == 1){
		//if two steps move valid
		if(checkInBound(b->m, b->n, b->p->x-2, b->p->y-1) == 1){
			//check left
			//printf("7\n");
			if(b->board[b->p->y-1][b->p->x-2] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x-2;
				temp->y = b->p->y-1;
				result[pointer] = temp;
				pointer+=1;
			}
		}
		
	}
	//check up
	if(checkInBound(b->m, b->n, b->p->x, b->p->y-2) == 1){
		//if two steps move valid
		if(checkInBound(b->m, b->n, b->p->x-1, b->p->y-2) == 1){
			//printf("1\n");
			//check left
			if(b->board[b->p->y-2][b->p->x-1] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x-1;
				temp->y = b->p->y-2;
				result[pointer] = temp;
				pointer+=1;
			}
		}
		if(checkInBound(b->m, b->n, b->p->x+1, b->p->y-2) == 1){
			//check left
			//printf("2\n");
			if(b->board[b->p->y-2][b->p->x+1] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x+1;
				temp->y = b->p->y-2;
				result[pointer] = temp;
				pointer+=1;
			}
		}
	}
	//check Right
	if(checkInBound(b->m, b->n, b->p->x+2, b->p->y) == 1){
		//if two steps move valid
		if(checkInBound(b->m, b->n, b->p->x+2, b->p->y-1) == 1){
			//check left
			//printf("3\n");
			if(b->board[b->p->y-1][b->p->x+2] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x+2;
				temp->y = b->p->y-1;
				result[pointer] = temp;
				pointer+=1;
			}
		}
		if(checkInBound(b->m, b->n, b->p->x+2, b->p->y+1) == 1){
			//check left
			//printf("4\n");
			if(b->board[b->p->y+1][b->p->x+2] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x+2;
				temp->y = b->p->y+1;
				result[pointer] = temp;
				pointer+=1;
			}
		}
	}
	
	//check down
	if(checkInBound(b->m, b->n, b->p->x, b->p->y+2) == 1){
		//if two steps move valid
		
		if(checkInBound(b->m, b->n, b->p->x+1, b->p->y+2) == 1){
			//check left
			//printf("6\n");
			
			if(b->board[b->p->y+2][b->p->x+1] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x+1;
				temp->y = b->p->y+2;
				result[pointer] = temp;
				pointer+=1;
			}
		}
		if(checkInBound(b->m, b->n, b->p->x-1, b->p->y+2) == 1){
			//check left
			//printf("5\n");
			if(b->board[b->p->y+2][b->p->x-1] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x-1;
				temp->y = b->p->y+2;
				result[pointer] = temp;
				pointer+=1;
			}
		}
	}
	
	
	if(checkInBound(b->m, b->n, b->p->x-2, b->p->y) == 1){
		if(checkInBound(b->m, b->n, b->p->x-2, b->p->y+1) == 1){
			//check left
			//printf("8\n");
			if(b->board[b->p->y+1][b->p->x-2] == 0){
				//check has the grid been reached
				Pos* temp = calloc(1, sizeof(Pos));
				temp->x = b->p->x-2;
				temp->y = b->p->y+1;
				result[pointer] = temp;
				pointer+=1;
			}
		}
	}
	
	
	if(pointer == 0){
		*returnLength = 0;
		free(result);
		return NULL;
	}
	*returnLength = pointer;
	return result;
}





void * findPath( void * arg);

int main(int argc, char* argv[]){
	setvbuf( stdout, NULL, _IONBF, 0 );
	
	int m = 0;
	int n = 0;
	int x = -1;
	
	//check the input number
	if( argc != 3 && argc != 4){
		fprintf(stderr, "ERROR: Invalid argument(s)\n");
		fprintf(stderr, "USAGE: a.out <m> <n> [<x>]\n");
		return EXIT_FAILURE;
	}else if(argc == 3){
		//if x is not specified
		m = checkInt(argv[1]);
		n = checkInt(argv[2]);
		if(m <= 2 || n <= 2){
			fprintf(stderr, "ERROR: Invalid argument(s)\n");
			fprintf(stderr, "USAGE: a.out <m> <n> [<x>]\n");
			return EXIT_FAILURE;
		}
	}else if(argc == 4){
		m = checkInt(argv[1]);
		n = checkInt(argv[2]);
		x = checkInt(argv[3]);
		if(m <= 2 || n <= 2 || x > (m*n) || x == -1){
			fprintf(stderr, "ERROR: Invalid argument(s)\n");
			fprintf(stderr, "USAGE: a.out <m> <n> [<x>]\n");
			return EXIT_FAILURE;
		}
		lower_bound = x;
	}
	
	dead_end_boards = calloc(dead_end_max_length, sizeof(Board*));
	Board* initial_board = calloc(1, sizeof(Board));
	initial_board->board = calloc(m,sizeof(int*));
	for(int i = 0; i < m; i++){
		initial_board->board[i] = calloc(n, sizeof(int));
	}
	initial_board->p = calloc(1, sizeof(Pos));
	initial_board->m = m;
	initial_board->n = n;
	initial_board->steps = 1;
	initial_board->board[0][0] = initial_board->steps;
	
	/*
	Pos** pArray;
	int pLength = 0;
	
	pArray = isValid(initial_board, &pLength);
	for(int i = 0; i < pLength; i++){
		printf("at i = %d: x = %d y = %d\n", i,pArray[i]->x,pArray[i]->y );
		initial_board->steps += 1;
		initial_board->board[pArray[i]->y][pArray[i]->x] = initial_board->steps;
	}
	*/
	
	//boardViewer(initial_board);
	
	
	//start to do the algo
	
	pthread_t tid;
	int rc;
	rc = pthread_create(&tid, NULL, findPath, initial_board);
	if(rc != 0){
		fprintf( stderr, "ERROR: Could not create thread (%d)\n", rc );
	}
	
	int* temp;
	rc = pthread_join(tid, (void**) &temp);
	if(rc != 0){
		fprintf( stderr, "ERROR: Could not join thread (%d)\n", rc );
	}
	free(temp);
	
	freeBoard(initial_board);
	for(int i = 0; i < dead_end_pointer; i++){
		freeBoard(dead_end_boards[i]);
	}
	free(dead_end_boards);
	
	
	return EXIT_SUCCESS;
}

void * findPath(void * arg){
	//printf("i am working here\n");
	Board* b = calloc(1, sizeof(Board));
	//copy the passed in board
	Board* passed = (Board*)arg;
	b->m = passed->m;
	b->n = passed->n;
	b->board = calloc(b->m, sizeof(int*));
	for(int i = 0; i < b->m; i++){
		b->board[i] = calloc(b->n,sizeof(int));
	}
	for(int i = 0; i < b->m; i++){
		for(int  j = 0; j < b->n; j++){
			b->board[i][j] = passed->board[i][j];
		}
	}
	
	b->p = calloc(1, sizeof(Pos));
	b->p->x = passed->p->x;
	b->p->y = passed->p->y;
	b->steps = passed->steps;
	
	//boardViewer(b);
	//boardViewer(passed);
	
	if(b->steps == 1){
		printf("THREAD %ld: Solving Sonny's knight's tour problem for a %dx%d board\n", pthread_self(),b->m, b->n);
	}
	//check the futher steps:
	//boardViewer(b);
	Pos** pArray;
	int pLength = 1;
	int* prv = NULL;
	while(pLength == 1){
		//printBoard(b, pthread_self());
		//printf("\n");
		pArray = isValid(b, &pLength);
		if(pLength == 1){
			//note down the result
			b->steps += 1;
			b->p->x = pArray[0]->x;
			b->p->y = pArray[0]->y;
			b->board[b->p->y][b->p->x] = b->steps;
			

			//free the memory
			free(pArray[0]);
			free(pArray);
			
		}
		else if(pLength == 0){
			// if dead end
			//check is every grid been visited
			if(b->steps == (b->m * b->n)){
				//if fully visited
				printf("THREAD %ld: Sonny found a full knight's tour!\n", pthread_self());
				pthread_mutex_lock(&mutex);
				if(max_squares < b->steps){
					max_squares = b->steps;
				}
				pthread_mutex_unlock(&mutex);
				
				freeBoard(b);
			}else{
				printf("THREAD %ld: Dead end after move #%d\n", pthread_self(), b->steps);
				//if not fully visited
				pthread_mutex_lock(&mutex);
				if(max_squares < b->steps){
					max_squares = b->steps;
				}
				
				//add this board to the dead_end_boards
				addBoard(&dead_end_boards, &dead_end_max_length, &dead_end_pointer, b);
				pthread_mutex_unlock(&mutex);
			}
			//return here
			int *r = calloc(1, sizeof(int));
			*r = b->steps;
			pthread_exit(r);
		}else{
			printf("THREAD %ld: %d moves possible after move #%d; creating threads...\n",pthread_self(),pLength,b->steps);
			//if there are more than one options avalible
			//create multiple thread to check
			
			#ifdef NO_PARALLEL
			for(int i = 0; i < pLength; i++){
				//printf("the %i possible is %d %d\n",i, pArray[i]->x, pArray[i]->y);
				//change the board
				b->steps += 1;
				b->p->x = pArray[i]->x;
				b->p->y = pArray[i]->y;
				b->board[b->p->y][b->p->x] = b->steps;
				
				
				//create thread
				pthread_t tid;
				int rc = pthread_create(&tid, NULL, findPath, b);
				if(rc != 0){
					fprintf(stderr, "ERROR: FindPath failed to create child thread\n");
				}
				
				//get the return value of process
				
				int* returnValue;
				rc = pthread_join(tid, (void**) &returnValue);
				if(rc == 0){
					printf("THREAD %ld: Thread [%ld] joined (returned %d)\n",pthread_self(),tid,*returnValue);
				}
				if(prv == NULL){
					prv = returnValue;
				}else if(*prv < *returnValue){
					free(prv);
					prv = returnValue;
				}else{
					free(returnValue);
				}

				//reset b's data
				b->board[b->p->y][b->p->x] = 0;
				b->p->x = passed->p->x;
				b->p->y = passed->p->y;
				b->steps -= 1;
				
				
				free(pArray[i]);
			}
			//every thing is finished, free the memory
			free(pArray);
			#endif
			
			#ifndef NO_PARALLEL
			pthread_t tid[pLength];
			Board** boardArray = calloc(pLength, sizeof(Board*));
			for(int i = 0; i < pLength; i++){
				//printf("the %i possible is %d %d\n",i, pArray[i]->x, pArray[i]->y);
				//change the board
				Board* temp = calloc(1, sizeof(Board));
				temp->m = b->m;
				temp->n = b->n;
				temp->steps = b->steps;
				temp->steps += 1;
				temp->p = calloc(1, sizeof(Pos));
				temp->p->x = pArray[i]->x;
				temp->p->y = pArray[i]->y;
				temp->board = calloc(temp->m, sizeof(int*));
				for(int i = 0; i < b->m; i++){
					temp->board[i] = calloc(temp->n,sizeof(int));
				}
				for(int i = 0; i < temp->m; i++){
					for(int  j = 0; j < temp->n; j++){
						temp->board[i][j] = b->board[i][j];
					}
				}
				
				temp->board[temp->p->y][temp->p->x] = temp->steps;
				//create thread
				//boardViewer(temp);
				int rc = pthread_create(&tid[i], NULL, findPath, temp);
				if(rc != 0){
					fprintf(stderr, "ERROR: FindPath failed to create child thread\n");
				}
				//reset b's data
				boardArray[i] = temp;
				
			}

			for(int i = 0; i < pLength; i++){
				//get the return value of process
				
				int* returnValue;
				int rc = pthread_join(tid[i], (void**) &returnValue);
				if(rc == 0){
					printf("THREAD %ld: Thread [%ld] joined (returned %d)\n",pthread_self(),tid[i],*returnValue);
				}else{
					fprintf(stderr, "ERROR: cannot join the thread\n");
				}
				if(prv == NULL){
					prv = returnValue;
				}else if(*prv < *returnValue){
					free(prv);
					prv = returnValue;
				}else{
					free(returnValue);
				}
				
			}
			for(int i = 0; i < pLength; i++){
				freeBoard(boardArray[i]);
				free(pArray[i]);
			}
			free(boardArray);
			//every thing is finished, free the memory
			free(pArray);
			
			#endif
		}
		
	}
	
	//if we exited the loop
	//we need to free the memory
	if(b->steps == 1){
		printf("THREAD %ld: Best solution(s) found visit %d squares (out of %d)\n", pthread_self(),max_squares,b->m*b->n);
		printf("THREAD %ld: Dead end boards:\n", pthread_self());
		for(int i = 0; i < dead_end_pointer; i++){
			if(dead_end_boards[i]->steps >= lower_bound){
				printBoard(dead_end_boards[i], pthread_self());
			}
		}
	}
	freeBoard(b);
	return prv;
}