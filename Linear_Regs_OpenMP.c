/* File:     Linear_Regs_Sequential.c
 *
 * Author:   Mehmet Cagri Aksoy - SW Engineer github.com/mcagriaksoy
 * 
 * License:  GNU General Public License v3.0
 * 
 * Purpose:  A program  try to predict Stock Prices in the next day (Sequentially No Process, No Threading is used).
 * 
 * Compile:
 *    gcc -g -Wall -o Linear_Regs_Sequential Linear_Regs_Sequential.c
 *    timer.h must be available
 *
 * Usage:
 *    ./Linear_Regs_Sequential.c
 *
 * Input:    Number of lines should be covered to predict stock prices.
 * Output:   A message from each process
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

#ifdef _OPENMP
# include <omp.h>
#endif

#define S 9900

int main(int argc, char **argv)
{
int thread_count = strtol(argv[1], NULL, 10);
int n;
// Initializing the file pointer
FILE *fs;
fs = fopen("1.csv", "r");
char ch, buffer[9900];
int i = 0, j = 0;
float float_buffer[9900];
float x[S], y[S], sumX=0, sumX2=0, sumY=0, sumXY=0, a, b;
double start, finish;

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
		float_buffer[j] = atoi(buffer);
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
printf("How many lines of Stock Price will be covered? (Higher numbers checks higher number of stocks Max:1530)\n");
scanf("%d", &n);

for(i=1;i<=n;i++)
{
y[i] = float_buffer[i];
}
GET_TIME(start);
#  pragma omp parallel num_threads(thread_count) \
	reduction(+:sumX)
/* Calculating Required Sum */
for(i=1;i<=n;i++)
{
sumX = sumX + x[i];
}
#  pragma omp parallel num_threads(thread_count) \
	reduction(+:sumX2)
/* Calculating Required Sum */
for(i=1;i<=n;i++)
{
sumX2 = sumX2 + x[i]*x[i];

}
#  pragma omp parallel num_threads(thread_count) \
	reduction(+:sumY)
/* Calculating Required Sum */
for(i=1;i<=n;i++)
{
sumY = sumY + y[i];

}
#  pragma omp parallel num_threads(thread_count) \
	reduction(+:sumXY)
/* Calculating Required Sum */
for(i=1;i<=n;i++)
{
sumXY = sumXY + x[i]*y[i];
}

/* Calculating a and b */
#  pragma omp parallel num_threads(thread_count)

b = (n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX);
a = (sumY - b*sumX)/n;
 GET_TIME(finish);
   printf("Thread 0 > Elapsed time = %e seconds\n", finish - start);

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

return(0);
}
