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
	int year;
	char *languages[5][21]; // 5 languages, 20 chars max + null
	double ratingValue;
	struct movie* next;
};

/*
	Parse the current line which is comma delimited and create a
	movie struct with the data in this line.
*/
struct movie *createMovie(char *currLine) {
	struct movie *currMovie = malloc(sizeof(struct movie));

	// For use with strtok_r
	char *saveptr1;
	char* saveptr2;

	// The first token is the title
	char *token = strtok_r(currLine, ",", &saveptr1);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->title, token);

	// The next token is year
	token = strtok_r(NULL, ",", &saveptr1);
	currMovie->year = atoi(token);

	// The next token is list of languages
	token = strtok_r(NULL, ",", &saveptr1);
	char *tokenLanguage = strtok_r(token, "[;]", &saveptr2);
	strcpy(currMovie->languages[0], tokenLanguage);


}

/*
	Process the file provided as an argument to the program to
	create a linked list of movie structs and print out information
	based on user inputs.
	Compile the program as follows:
		gcc --std=gnu99 -o movies main.c
*/
int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("You must prove the name of the file to process\n");
		printf("Example usage: ./movies movie_sample_1.csv\n");
		return EXIT_FAILURE;
	}
	struct movie* list = processFile(argv[1]);
	printMovieList(list);
	return EXIT_SUCCESS;
}