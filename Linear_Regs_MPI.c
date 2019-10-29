#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


#define SIZE 4000
int n = 1400;
char ch, buffer[SIZE];
int i = 0, j = 0;
float float_buffer[SIZE];
float x[SIZE], y[SIZE], sumX=0, sumX2=0, sumY=0, sumXY=0, a, b,local_b,local_sumX,T_sumXsumY;
float local_result=0;
float b_local_result=0;
int source, local_int,my_rank, comm_sz;
float  Regression(int sumX, int sumY, int local_b, int n);
float dummy;
double start, finish, elapsed;
void Print_Result(void);
float f_sumXY(float local_sumX, float local_sumY);
void Read_File(void)
{
// Initializing the file pointer
FILE *fs;
fs = fopen("1.txt", "r");
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
	for(i=1;i<=n;i++)
	{
		y[i] = float_buffer[i];
	}
}

int main(int argc, char **argv)
{
/* Let the system do what it needs to start up MPI */
   MPI_Init(NULL, NULL);

   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);


Read_File();


//local_sumY = sumY/comm_sz;


/* Calculating Required Sum */
GET_TIME(start);
for(i=1;i<=n;i++)
{
sumX = sumX + x[i];
sumY = sumY + y[i];
sumX2 = sumX2 + x[i]*x[i];
sumXY = sumXY + x[i]*y[i];
}
local_sumX = sumX/comm_sz;
float local_sumXsumY = f_sumXY(local_sumX,sumY);

MPI_Reduce(&local_sumXsumY, & T_sumXsumY, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);


b = (n*sumXY-T_sumXsumY)/(n*sumX2-sumX*sumX);
local_b = b/comm_sz;
local_result = Regression(sumX, sumY,local_b,n);
 /* Add up the integrals calculated by each process */
MPI_Reduce(&local_result, & b_local_result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

 if (my_rank == 0) {
     	Print_Result();
	 GET_TIME(finish);
	 elapsed = finish - start;
	printf("The code to be timed took %e seconds\n", elapsed);
   }
 /* Shut down MPI */
   MPI_Finalize();
return(0);
}
float f_sumXY(float local_sumX,float local_sumY)
{
	return (local_sumX*local_sumY);
}
float Regression(int sumX, int sumY, int local_b, int n)
{
	return (local_b*sumX)/n;
}

void Print_Result(void){

a = (sumY /n) - b_local_result;
/* Displaying value of a and b */
printf("Values are: a=%0.2f and b = %0.2f",a,b);
printf("\nEquation of best fit is: y = %0.2f + %0.2fx",a,b);
float res = a + (b * (n+1));
if(res < y[n] )
{
printf("\n NEXT DAY, THE STOCK PRICE WILL BE: DECREASED!!\n");
}
else{
printf("\n NEXT DAY, THE STOCK PRICE WILL BE: INCREASED!!\n");
}
printf("\n Next Stock Price Prediction is: %0.2f " , res);
}
