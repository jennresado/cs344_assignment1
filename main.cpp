/* 
Jennifer Do
Assignment 1: Movies
Reads a CSV file with movie data that is provided to the program
as an argument.
Processes the data in the file to create struct to hold data for
each movie.
Creates a linked list of all these structs.
Gives user choices to ask questions about the movies in the data.
Prints out the data about the movies per user choice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for movie information */
struct movie {
	char *title;
	int *year;
	char *languages; // TODO: this is an array
	double *ratingValue;
	struct movie* next;
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("You must prove the name of the file to process\n");
		printf("Example usage: ./movies movie_sample_1.csv\n");
		return EXIT_FAILURE;
	}
	//struct movie* list = processFile(argv[1]);
	//printStudentList(list);
	return EXIT_SUCCESS;
}