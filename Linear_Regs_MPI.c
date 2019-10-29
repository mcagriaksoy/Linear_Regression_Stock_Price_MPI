#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define S 9900
char ch, buffer[S];
int i = 0, j = 0;
float float_buffer[S];
float x[S], y[S], sumX=0, sumX2=0, sumY=0, sumXY=0, a, b,local_b;
int local_result,sumx_local_result=0;
float x_local[S], y_local[S];
int source, local_int, n,my_rank, comm_sz, local_n_left = 1, local_n_right,local_n;
int Regression(int sumX, int sumY, int local_b, int n);
float dummy;
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
/* Input */
	for(i=1;i<=n;i++)
	{
	y[i] = float_buffer[i];
	}
}

int main(int argc, char **argv)
{
n=500;

/* Let the system do what it needs to start up MPI */
   MPI_Init(NULL, NULL);

   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);


Read_File();


/* Calculating Required Sum */

for(i=1;i<=local_n;i++)
{
sumX = sumX + x[i];
sumY = sumY + y[i];
sumX2 = sumX2 + x[i]*x[i];
sumXY = sumXY + x[i]*y[i];
}
dummy = sumY /n ;
b = (n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX);
local_b = b/comm_sz;

local_result = Regression(sumX, sumY,local_b,n);
 /* Add up the integrals calculated by each process */
   MPI_Reduce(&local_result, & sumx_local_result, 1, MPI_INT, MPI_SUM, 0,
         MPI_COMM_WORLD);

printf("A::%d", &sumx_local_result);
a = dummy - sumx_local_result;
/* Displaying value of a and b */
printf("Values are: a=%0.2f and b = %0.2f",a,b);
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

 
   /* Shut down MPI */
   MPI_Finalize();

return(0);
}

int Regression(int sumX, int sumY, int local_b, int n)
{
a = (local_b*sumX)/n;

return a;
}
