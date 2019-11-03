/* File:     Linear_Regs_PThread.c
 *
 * Author:   Mehmet Cagri Aksoy - SW Engineer github.com/mcagriaksoy
 * 
 * License:  GNU General Public License v3.0
 * 
 * Purpose:  A program in which multiple MPI processes try to predict Stock Prices in the next day.
 *
 * Compile:  mpicc -g -Wall -o Linear_Regs_PThread Linear_Regs_PThread.c
 * Usage:    mpiexec -n<number of processes> ./Linear_Regs_PThread
 * timer.h must be available
 * Input:    Number of lines should be covered to predict stock prices.
 * Output:   A message from each process
 *
 */

#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define SIZE 4000
int n;
char ch, buffer[SIZE];
int i = 0, j = 0, n= 0;
float x[SIZE], y[SIZE], sumX=0, sumX2=0, sumY=0, sumXY=0, a, b;
int source, local_int,my_rank, comm_sz;
int get_input(void);
double process( int x_temp, int y_temp );
double start, finish, start0, finish0;

double Regression(int  my_first_row,int my_last_row, int local_n) ;
double RegressionY(int  my_first_row,int my_last_row, int local_n) ;

void Read_File(void);
int main(int argc, char** argv)
{

 /* Let the system do what it needs to start up MPI */
   MPI_Init(NULL, NULL);

   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   n = get_input();	
   int i, local_int, local_intY, sumx_mpi,sumy_mpi=0;
   int local_n = n/ comm_sz; 
   int my_first_row = my_rank*local_n;
   int my_last_row = my_first_row + local_n;

   GET_TIME(start);

   local_int = Regression(my_first_row, my_last_row, local_n);
   local_intY = Regression(my_first_row, my_last_row, local_n);	
   /* Add up the integrals calculated by each process */
   if (my_rank != 0) { 
	MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, 
	    MPI_COMM_WORLD); 
	MPI_Send(&local_intY, 1, MPI_DOUBLE, 0, 0, 
	    MPI_COMM_WORLD);
   } else {
	sumx_mpi = local_int;
	sumy_mpi = local_intY;
      for (source = 1; source < comm_sz; source++) {
         MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&local_intY, 1, MPI_DOUBLE, source, 0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         sumx_mpi += local_int;
	sumy_mpi += local_intY;

      }
   } 

   /* Print the result */
   if (my_rank == 0) {
	
      for(i=1;i<=n;i++)
	{
		sumX2 = sumX2 + x[i]*x[i];
		sumXY = sumXY + x[i]*y[i];
	}
	b = (n*sumXY-sumx_mpi*sumy_mpi)/(n*sumX2-sumx_mpi*sumx_mpi);
	a = (sumy_mpi - b*sumx_mpi)/n;

	/* Displaying value of a and b */
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
	GET_TIME(finish);
   printf("Thread %ld > Elapsed time = %e seconds\n", 
      my_rank, (finish - start) );
	   }

   /* Shut down MPI */
   MPI_Finalize();
/* Calculating Required Sum */

    return(0);
    
}
double Regression(
      int  my_first_row  /* in */, 
      int    my_last_row  /* in */, 
      int local_n    /* in */) {

   for (int i = my_first_row; i <= my_last_row; i++) 
{
	sumX += x[i];
   }
   return sumX;
} 

double RegressionY(
      int  my_first_row  /* in */, 
      int    my_last_row  /* in */, 
      int local_n    /* in */) {
  
   for (int i = my_first_row; i <= my_last_row; i++) 
{
	sumY += y[i];
   }

   return sumY;
} 


double process( int x_temp, int y_temp )
{
	return x_temp*y_temp ;
}

void Read_File(void)
{
// Initializing the file pointer
FILE *fs;
fs = fopen("1.csv", "r");
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
// Incrementing the array position
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
		// incrementing the counter so that the next
		// character is read in the next position in 
		// the array of buffer
		i++;
	}
}
}

int get_input(void)
{
	if (my_rank == 0){
		Read_File();
		printf("How many lines of Stock Price will be covered? (Higher numbers checks higher number of stocks Max:1530)\n");
		scanf("%d", &n);
	}
return n;
}


