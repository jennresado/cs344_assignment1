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
	char *saveptr2;

	// The first token is the title
	char *token = strtok_r(currLine, ",", &saveptr1);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->title, token);

	// The next token is year
	token = strtok_r(NULL, ",", &saveptr1);
	currMovie->year = atoi(token);

	// The next token is list of languages
	token = strtok_r(NULL, ",", &saveptr1);
	char *tokenLanguage = strtok_r(token, "[", &saveptr2);
	tokenLanguage = strtok_r(NULL, ";", &saveptr2);
	int numLanguage = 0;
	while (tokenLanguage != NULL) {
		strcpy(currMovie->languages[numLanguage], tokenLanguage);
		numLanguage++;
		tokenLanguage = strtok_r(NULL, ';]', &saveptr2);
	}

	// The last token is rating value
	token = strtok_r(NULL, "\n", &saveptr1);
	char *ptr;
	currMovie->ratingValue = strtod(token, ptr);

	// Set the next node to NULL in the newly created student entry
	currMovie->next = NULL;

	return currMovie;
}

/*
	Return a linked list of movies by parsing data from
	each line of the specified file.
*/
struct movie *processFile(char* filePath) {
	int movies = 0;

	// Open the specified file for reading onlu
	FILE *movieFile = fopen(filePath, "r");

	char *currLine = NULL;
	size_t len = 0;
	ssize_t nread;
	char *token;

	// The head of the linked list
	struct movie *head = NULL;
	struct movie *tail = NULL;

	// Read the file line by line
	while ((nread = getLine(&currLine, &len, movieFile)) != -1) {
		// Get a new movie node corresponding to the current line
		struct movie *newNode = createMovie(currLine);
		movies++;

		// Is this the first node in the linked list?
		if (head == NULL) {
			// This is the first node in the linked list
			// Set the head and the tail to this node
			head = newNode;
			tail = newNode;
		}
		else {
			// This is not the first node
			// Add this node to the list and advance the tail
			tail->next = newNode;
			tail = newNode;
		}
	}
	free(currLine);
	fclose(movieFile);

	printf("Processed file %s and parsed data for %d movies\n", filePath, movies);

	return head;
}

/*
	Print movies released in a specified year
*/
void printMoviesReleased(struct movie *list) {
	int year;
	int count = 0;

	// Ask user for a year
	printf("Enter the year for which you want to see movies: ");
	scanf("%d", &year);

	// Print the names of all the movies released in specified year
	while (list != NULL) {
		if (list->year == year) {
			printf("%s\n", list->title);
			count++;
		}
		list = list->next;
	}

	// No movies were released in specified year
	if (count == 0) {
		printf("No data about movies released in the year %d", year);
	}
}

/*
	Print highest rated movies for each year
*/
void printHighestRatedMoviesYr(struct movie *list) {

}

/*
	Print movies and their year of release for a specified language
*/
void printMoviesSpecLang(struct movie *list) {
	char language[21];
	int count = 0;

	printf("Enter the language for which you want to see movies: ");
	scanf("%s", language);

	// Print the year of release and movie title for movies released
	// for specified language
	while (list != NULL) {
		for (int i = 0; i < 5; i++) {
			if (strcmp(language, list->languages[i])) {
				printf("%d %s\n", list->year, list->languages[i]);
				count++;
			}
		}
		list = list->next;
	}

	// No movies were released for specified language
	if (count == 0) {
		printf("No data about movies released in %s", language);
	}
}

/*
	Print menu of interactive choices to user and process
	the selected choice.
*/
void interactions(struct movie *list) {
	int choice;

	printf("1. Show movies released in the specified year\n");
	printf("2. Show highest rated movie for each year\n");
	printf("3. Show the title and year of release of all movies in a specific language\n");
	printf("4. Exit from the program\n");
	
	// Receive input from user and process based on value
	printf("Enter a choice from 1 to 4: ");
	scanf("%d", &choice);

	while (choice != 4) {
		switch (choice) {
		case 1:
			printMoviesReleased(list);
			break;
		case 2:
			printHighestRatedMoviesYr(list);
			break;
		case 3:
			printMoviesSpecLang(list);
			break;
		default:
			printf("You entere an incorrect choice. Try again.\n");
			break;
		}

		printf("Enter a choice from 1 to 4: ");
		scanf("%d", &choice);
	}
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
	interactions(list);
	return EXIT_SUCCESS;
}