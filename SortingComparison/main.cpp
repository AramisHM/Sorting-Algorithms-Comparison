/*
Bubble sort
by Aramis Hornung Moraes

Trying my best to bubble sort as efficient as it  can be in C
August 29th 2017

Licensed under MIT Open Source License
*/

#include <stdio.h> // printf
#include <stdlib.h> //malloc file management
#include <string.h>
#include <time.h>
// prints the array
void print_array(int *arr, unsigned int array_size) {
	int i = 0;
	for (i = 0; i < array_size; ++i)
		printf("%d, ", arr[i]);
}

// reads a file into a memory array
char *data_From_file(char *file_name)
{

	FILE *fp;
	long lSize;
	char *buffer;

	//------------- lets get the file data
	fp = fopen(file_name, "rb");
	if (!fp) perror(file_name), exit(1);

	// get file size
	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	/* allocate memory for entire content */
	buffer = (char*)calloc(1, lSize + 1);
	if (!buffer) fclose(fp), fputs("memory allocation failed", stderr), exit(1);

	/* copy the file into the buffer */
	if (1 != fread(buffer, lSize, 1, fp))
		fclose(fp), free(buffer), fputs("file read has failed", stderr), exit(1);
	buffer[lSize] = 0; // null termination
					   /* do your work here, buffer is a string contains the whole text */

	fclose(fp);

	return buffer;
}

// Loads the array from a file, containing positive integer numbers separated by sspaces
int *array_from_file(char *file_name, unsigned int *file_size)
{
	char *buffer = data_From_file(file_name);

	//------------- now lets parse the array

	int i = 0;
	char *p = strtok(buffer, " "); // we will check the numbers separated by spaces

	int *int_array;
	unsigned int array_size = 0;
	// first lets count the number of elements
	while (p != NULL)
	{
		++array_size;
		p = strtok(NULL, " ");
	}

	
	*file_size = array_size;
	int_array = (int*)calloc(array_size, sizeof(int));
	
	// restart strtok
	free(buffer); // the previous string input has been modified by strtok, we need to reaload it.
	buffer = data_From_file(file_name);
	p = strtok(buffer, " ");
	while (p != NULL)
	{
		int_array[i++] = atoi(p);
		p = strtok(NULL, " ");
	}

	// just a lil debug to show the array we just loaded
	if(array_size < 1000)
	{
		print_array(int_array, array_size);
	}
	free(buffer); // free the file buffer, we wont neet it anymore since we loaded the array

	return int_array;
}

// bubblesort sorting algorithm
void bubble_sort(int *input_array, unsigned int array_size)
{
	int i;
	int swapped = 0;
	int indexOfLastInsortedElement = array_size - 1;

	do
	{
		swapped = 0;
		for (i = 0; i < indexOfLastInsortedElement; ++i) {
			if (input_array[i] > input_array[i + 1])
			{
				int swap = input_array[i];
				input_array[i] = input_array[i + 1];
				input_array[i + 1] = swap;
				swapped = 1;
			}
		}
		--indexOfLastInsortedElement;
	} while (swapped == 1);
}

int main()
{
	clock_t start_t, end_t, total_t; // used to measure time elapsed during sorting

	unsigned int arry_sz = 0;
	int *arry;
	arry =  array_from_file("100k.txt", &arry_sz);
	
	start_t = clock();
	bubble_sort(arry, arry_sz);
	end_t = clock();

	total_t = (end_t - start_t);

	print_array(arry, arry_sz);
	printf("\n\nBubbleSort took: %f seconds to finish\n\n", (float)total_t/1000.0f);

	free(arry);
	fflush(stdin);
	getchar();
}