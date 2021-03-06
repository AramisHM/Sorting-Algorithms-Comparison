/*
Sorting comparison
by Aramis Hornung Moraes

Trying my best to implement some sorting algorithms as efficient as it can be in C
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
int *array_from_file(char *file_name, unsigned int *file_size) {
	char *buffer = data_From_file(file_name);

	//------------- lets parse the array
	int i = 0;
	char *p = strtok(buffer, " "); // we will check the numbers separated by spaces

	int *int_array;
	unsigned int array_size = 0;
	// first lets count the number of elements
	while (p != NULL) {
		++array_size;
		p = strtok(NULL, " ");
	}

	*file_size = array_size;
	int_array = (int*)calloc(array_size, sizeof(int));
	
	// restart strtok
	free(buffer); // the previous string input has been modified by strtok, we need to reaload it.
	buffer = data_From_file(file_name);
	p = strtok(buffer, " ");
	while (p != NULL) {
		int_array[i++] = atoi(p);
		p = strtok(NULL, " ");
	}

	// just a lil debug to show the array we just loaded
	if(array_size < 100) {
		print_array(int_array, array_size);
	}
	free(buffer); // free the file buffer, we wont neet it anymore since we loaded the array

	return int_array;
}

// bubblesort sorting algorithm
void bubble_sort(int *input_array, unsigned int array_size) {
	int swapped = 0;
	unsigned int i;
	unsigned int indexOfLastInsortedElement = array_size - 1;

	do {
		swapped = 0;
		for (i = 0; i < indexOfLastInsortedElement; ++i) {
			if (input_array[i] > input_array[i + 1]) {
				int swap = input_array[i];
				input_array[i] = input_array[i + 1];
				input_array[i + 1] = swap;
				swapped = 1;
			}
		}
		--indexOfLastInsortedElement;
	} while (swapped == 1);
}

// Insertionsort sorting algorithm
void insertion_sort(int *input_array, unsigned int array_size) {
	for (unsigned int i = 1; i < array_size; ++i){
		for (unsigned int j = i; j >= 1; --j) {
			if (input_array[j - 1] > input_array[j]) { // swap
				int aux = input_array[j - 1];
				input_array[j - 1] = input_array[j];
				input_array[j] = aux;
			}
			else break; // spare us unecessary computation!
		}
	}
}

// Bucketsort sorting algorithm
void bucket_sort(int *input_array, unsigned int array_size)
{
	unsigned int max_val = 0;
	int *buckets;
	
	for (unsigned int i = 0; i < array_size; ++i)
		max_val = (max_val < input_array[i] ? input_array[i] : max_val);

	buckets = (int*)calloc(max_val+1, sizeof(int)); // interesting note

	for (unsigned int i = 0; i < array_size; ++i)
		++buckets[input_array[i]];

	unsigned int pos = 0;
	for (unsigned int i = 0; i <= max_val; ++i) // array elements, interesting note
	{
		if(buckets[i]){
			for (unsigned int j = 0; j < buckets[i]; ++j) // same element ocurrences
			{ 
				input_array[pos] = i;
				++pos;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	clock_t start_t, end_t, total_t; // used to measure time elapsed during sorting

	unsigned int arry_sz = 0;
	int *arry;
	
	if (argc < 2)
		printf("No input file given, please, drag and drop the array file.");
	else
	{
		arry = array_from_file(argv[1], &arry_sz);
		if (!arry_sz)
		{
			printf("The file has no valid input.");
		}
		else
		{
			printf("\nALGRTHM\t\t\tF_SIZE\t\t\tTIME", "BUBB SOR", arry_sz, (float)total_t / 1000.0f);

			printf("\n\n");
			//arry = array_from_file("100k.txt", &arry_sz);
			start_t = clock();
			bubble_sort(arry, arry_sz);
			end_t = clock();
			total_t = (end_t - start_t);
			printf("\n%s\t\t\t%d\t\t\t%f milliseconds\n", "BUB SOR", arry_sz, (float)total_t / 1000.0f);
			if (arry_sz < 100)
				print_array(arry, arry_sz); // debug only
			free(arry);
			

			printf("\n\n");
			arry = array_from_file(argv[1], &arry_sz);
			start_t = clock();
			insertion_sort(arry, arry_sz);
			end_t = clock();
			total_t = (end_t - start_t);
			printf("\n%s\t\t\t%d\t\t\t%f milliseconds\n", "INS SOR", arry_sz, (float)total_t / 1000.0f);
			if (arry_sz < 100)
				print_array(arry, arry_sz); // debug only
			free(arry);
			

			printf("\n\n");
			arry = array_from_file(argv[1], &arry_sz);
			start_t = clock();
			bucket_sort(arry, arry_sz);
			end_t = clock();
			total_t = (end_t - start_t);
			printf("\n%s\t\t\t%d\t\t\t%f milliseconds\n", "BUK SOR", arry_sz, (float)total_t / 1000.0f);
			if (arry_sz < 100)
				print_array(arry, arry_sz); // debug only
			free(arry);
			
		}
	}
	fflush(stdin);
	getchar();
}