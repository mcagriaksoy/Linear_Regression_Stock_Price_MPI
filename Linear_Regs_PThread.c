/* File:     Linear_Regs_PThread.c
 *
 * Author:   Mehmet Cagri Aksoy - SW Engineer github.com/mcagriaksoy
 * 
 * License:  GNU General Public License v3.0
 * 
 * Purpose:  A program in which multiple PThread processes try to predict Stock Prices in the next day.
 * 
 * Compile:
 *    gcc -g -Wall -o  Linear_Regs_PThread Linear_Regs_PThread.c -lpthread
 *    timer.h must be available
 *
 * Usage:
 *    ./Linear_Regs_PThread <thread_count>
 *
 * Input:    Number of lines should be covered to predict stock prices.
 * Output:   A message from each process
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#define S 2000
/* Global variables */
int n,m;
const int MAX_THREADS = 128;
int arr[128];
char ch, buffer[S];
int i = 0, j = 0;
float float_buffer[S];
float x[S], y[S], sumX=0, sumX2=0, sumY=0, sumXY=0, a, b;
int     thread_count;
double start, finish;

/* Parallel function */
void *Pth_mat_vect(void* rank);
void Usage(char* prog_name);

int main(int argc, char **argv)
{
	long       thread;
	pthread_t* thread_handles;
	FILE *fs;
	fs = fopen("TurkishAirlinesDATASET.csv", "r");
	// Read the file unless the file encounters an EOF
	while(1){
		// Reads the character where the seeker is currently
		ch = fgetc(fs);

		// If EOF is encountered then break out of the while loop
		if(ch == EOF){
			break;
		}

		// If the delimiter is encounterd(which can be
		// anything according to your wish) then skip the character
		// and store the last read array of characters in
		// an integer array
		else if(ch == '\n'){
	// Increamenting the array position
			j++;
			// Converting the content of the buffer into
			// an array position
			y[j] = atoi(buffer);
			//printf("%0.2f",float_buffer[j]);		
			x[j]=x[j-1]+1;
			// Clearing the buffer, this function takes two
			// arguments, one is a character pointer and 
			// the other one is the size of the character array
			bzero(buffer, 32);

			// clearing the counter which counts the number
			// of character in each number used for reading
			// into the buffer.
			i = 0;

			// then continue
			continue;
		}
		else{

			// reads the current character in the buffer
			buffer[i] = ch;
			//printf("%c",ch);
			// increamenting the counter so that the next
			// character is read in the next position in 
			// the array of buffer
			i++;
		}
	}
	/* Input */
	printf("How many lines of Stock Price will be covered? (Higher numbers checks higher number of stocks Max:1503 )\n");
	scanf("%d", &n);

	   /* Get number of threads from command line */
	   if (argc != 2) Usage(argv[0]);
	   thread_count = strtol(argv[1], NULL, 10);  
	   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

	   thread_handles = malloc (thread_count*sizeof(pthread_t)); 

	   for (thread = 0; thread < thread_count; thread++)
	      pthread_create(&thread_handles[thread], NULL,
		 Pth_mat_vect, (void*) thread);
	   for (thread = 0; thread < thread_count; thread++)
	      pthread_join(thread_handles[thread], NULL);

	b = (n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX);
	a = (sumY - b*sumX)/n;	
	//printf("Values are: a=%0.2f and b = %0.2f",a,b);
	printf("\nEquation of best fit is: y = %0.2f + %0.2fx",a,b);
	float res = a + (b * (n+1));
	if(res < y[n] )
	{
		printf("NEXT DAY, THE STOCK PRICE WILL BE: DECREASED!!\n");
	}
	else{
		printf("NEXT DAY, THE STOCK PRICE WILL BE: INCREASED!!\n");
		}
		printf("Next Stock Price Prediction is: %0.2f " , res);
	int resultime =0;
	for (int i = 0; i <= thread; i++){
		resultime += arr[i];
	}
	 printf("Thread > Elapsed time = %e seconds\n", finish - start);
	 free(thread_handles);
	return(0);
}

/*------------------------------------------------------------------
 * Function:       Pth_mat_vect
 * Purpose:        Calculate result of regression to create formula.
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i;
   int local_n = n/thread_count; 
   int my_first_row = my_rank*local_n;
   int my_last_row = my_first_row + local_n;
 
   GET_TIME(start);
   for (i = my_first_row; i < my_last_row; i++) {
      	sumX = sumX + x[i];
	sumX2 = sumX2 + x[i]*x[i];
	sumY = sumY + y[i];
	sumXY = sumXY + x[i]*y[i];
   }
   GET_TIME(finish);
  arr[my_rank] = finish - start;
   return NULL;
} 

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */
