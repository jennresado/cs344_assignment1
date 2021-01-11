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
	char* title;
	int year;
	char* languages[5][21]; // 5 languages, 20 chars max + null
	double ratingValue;
	struct movie* next;
};

/* struct for highest rated movie for each year*/
struct rating {
	int year;
	double ratingValue;
	char* title;
	struct rating* next;
};

/*
	Parse the current line which is comma delimited and create a
	movie struct with the data in this line.
*/
struct movie* createMovie(char* currLine) {
	struct movie* currMovie = malloc(sizeof(struct movie));

	// For use with strtok_r
	char* saveptr1;
	char* saveptr2;

	// The first token is the title
	char* token = strtok_r(currLine, ",", &saveptr1);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->title, token);

	// The next token is year
	token = strtok_r(NULL, ",", &saveptr1);
	currMovie->year = atoi(token);

	// The next token is list of languages
	token = strtok_r(NULL, ",", &saveptr1);
	char* tokenLang;
	int numLanguage = 0;
	for (tokenLang = strtok_r(token, "[;]", &saveptr2); tokenLang; tokenLang = strtok_r(NULL, "[;]", &saveptr2)) {
		strcpy(currMovie->languages[numLanguage], tokenLang);
		numLanguage++;
	}

	// The last token is rating value
	token = strtok_r(NULL, "\n", &saveptr1);
	char* ptr;
	currMovie->ratingValue = strtod(token, ptr);

	// Set the next node to NULL in the newly created student entry
	currMovie->next = NULL;

	return currMovie;
}

/*
	Create a rating struct with current movie data.
*/
struct rating *createRating(struct movie* aMovie) {
	struct rating *currRating = malloc(sizeof(struct rating));

	// Set the year
	currRating->year = aMovie->year;

	// Highest rating for movie of year
	currRating->ratingValue = aMovie->ratingValue;

	// Title of movie of year
	currRating->title = calloc(strlen(aMovie->title) + 1, sizeof(char));
	strcpy(currRating->title, aMovie->title);

	// Set the next node to NULL in newly created rating entry
	currRating->next = NULL;

	return currRating;
}

/*
	Return a linked list of movies by parsing data from
	each line of the specified file.
*/
struct movie *processFile(char *filePath) {
	int movies = 0;

    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

	// Skip header
	nread = getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1) {
        // Get a new student node corresponding to the current line
        struct movie *newNode = createMovie(currLine);
		movies++;

        // Is this the first node in the linked list?
        if (head == NULL) {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        } else {
            // This is not the first node.
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
void printMoviesReleased(struct movie* list) {
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
		printf("No data about movies released in the year %d\n", year);
	}
}

/*
	Print highest rated movies for each year
*/
void printHighestRatedMoviesYr(struct movie* list) {
	// Find highest rated movie per year
	struct rating* rList = processRatings(list);

	while (rList != NULL) {
		printf("%d %.1f %s\n", rList->year, rList->ratingValue, rList->title);
		rList = rList->next;
	}
}

int processRatings(struct movie* list) {
	// The head of the linked list
	struct rating* head = NULL;
	// The tail of the linked list
	struct rating* tail = NULL;

	// Find the highest rated movie per year
	while (list != NULL) {
		int exists;

		// First node in linked list is empty
		if (head == NULL) {
			// Get a new rating node corresponding to the current movie
			struct rating* newNode = createRating(list);

			head = newNode;
			tail = newNode;
		}
		else {
			// This is not the first node
			// Determine if current movie ranks highest for its year
			// Updates rating for year, if it outranks another movie
			exists = checkRatingYear(head, list);

			// Year of current movie does not exist or rating is not higher
			if (exists == 0) {
				// Get a new rating node corresponding to the current movie
				struct rating* newNode = createRating(list);

				// Add this node to the list and advance the tail
				tail->next = newNode;
				tail = newNode;
			}
		}
		list = list->next;
	}
	return head;
}

/*
	Determine if current movie ranks highest for its year
*/
int checkRatingYear(struct rating* list, struct movie* aMovie) {
	while (list != NULL) {
		// Year of current movie exists 
		if (list->year == aMovie->year) {
			if (list->ratingValue < aMovie->ratingValue) {
				// Update the rating for year if current movie outranks
				// previous highest rated movie
				list->ratingValue = aMovie->ratingValue;
				strcpy(list->title, aMovie->title);
			}
			return 1;
		}
		list = list->next;
	}
	return 0;
}

/*
	Print movies and their year of release for a specified language
*/
void printMoviesSpecLang(struct movie* list) {
	char language[21];
	int count = 0;

	printf("Enter the language for which you want to see movies: ");
	scanf("%s", language);

	// Print the year of release and movie title for movies released
	// for specified language
	while (list != NULL) {
		for (int i = 0; i < 5; i++) {
			if (strcmp(language, list->languages[i]) == 0) {
				printf("%d %s\n", list->year, list->title);
				count++;
			}
		}

		list = list->next;
	}

	// No movies were released for specified language
	if (count == 0) {
		printf("No data about movies released in %s\n", language);
	}
}

/*
	Print menu of interactive choices to user and process
	the selected choice.
*/
void interactions(struct movie* list) {
	int choice;
	char *choices = "\n"
		"1. Show movies released in the specified year\n"
		"2. Show highest rated movie for each year\n"
		"3. Show the title and year of release of all movies in a specific language\n"
		"4. Exit from the program\n"
		"\n";

	// Receive input from user and process based on value
	printf("%s", choices);
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
			printf("You entered an incorrect choice. Try again.\n");
			break;
		}

		// Receive input from user and process based on value
		printf("%s", choices);
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
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_sample_1.txt\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
    interactions(list);
    return EXIT_SUCCESS;
}
