#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "simpleCSVsorter.h"
#include "mergesort.c"

int main(int argc, char **argv){
	if(argc != 3)
	{
		fprintf(stderr, "Invalid quantity of parameters.");
		return 0;
	}
	if(strcmp(argv[1], "-c") != 0)
	 {
		fprintf(stderr, "Invalid parameter.");
		return 0;
	}

	char* type;
	int num = GetDataType(argv[2]);
	if(num == 1)
		type = "string";
	if(num == 2)
		type = "numeric";
	if(num == 3)
		type = "dateTime";
	if(num == 4)
		type = "Not Found";

	if(strcmp(type, "Not Found") == 0)
	 {
		printf("Column to be searched was not found.");
		return 0;
	}

	int i;
	struct movieNode* curMovie = NULL;
	char titleColumn[1024];
	fgets(titleColumn, sizeof(buffer), stdin); //get title column
  	for (i = 0; fgets(buffer, sizeof(buffer), stdin); i++){  //loop through every line of CSV
    	char movie[28][200];
		GetMovie(buffer, movie); //convert each line into an array of String
		Add(&curMovie, movie); // add the movie to the linked list
  	}

	curMovie = MergeSort(curMovie, type); //sort the linked list
	//print and free the list
	printf("%s", titleColumn);
	struct movieNode* prev;
	while(curMovie != NULL)
	{
		int j;
		for(j = 0; j<27;j++){
			printf("%s,", curMovie->movie[j]);
		}
		printf("%s\n", curMovie->movie[27]);

		prev = curMovie;
		curMovie = curMovie->next;
		free(prev);
	}
  	return 0;
}

int GetDataType(char input[]){
	struct Column columns[28] =
	{
		{"color", "string"},
		{"director_name", "string"},
		{"num_critic_for_reviews", "numeric"},
		{"duration", "dateTime"},
		{"director_facebook_likes", "numeric"},
		{"actor_3_facebook_likes", "numeric"},
		{"actor_2_name", "string"},
		{"actor_1_facebook_ltymikes", "numeric"},
		{"gross", "numeric"},
		{"genres", "string"},
		{"actor_1_name", "string"},
		{"movie_title", "string"},
		{"num_voted_users", "numericv"},
		{"cast_total_facebook_likes", "numeric"},
		{"actor_3_name", "string"},
		{"facenumber_in_poster", "numeric"},
		{"plot_keywords", "string"},
		{"movie_imdb_link", "string"},
		{"num_user_for_reviews", "numeric"},
		{"language", "string"},
		{"country", "string"},
		{"content_rating", "string"},
		{"budget", "numeric"},
		{"title_year", "numeric"},
		{"actor_2_facebook_likes", "numeric"},
		{"imdb_score", "numeric"},
		{"aspect_ratio", "numeric"},
		{"movie_facebook_likes", "numeric"}
	};

	int index;
	for (index = 0;index<28; index++)
	{
		if(strcmp(columns[index].name, input) == 0)
		{
			columnIndex = index;
			if(strcmp(columns[index].dataType, "string") == 0)
				return 1;
			if(strcmp(columns[index].dataType, "numeric") == 0)
				return 2;
			if(strcmp(columns[index].dataType, "dateTime") == 0)
				return 3;
		}
	}
	return 4;
}

void Add(struct movieNode **headptr, char movie[28][200]) {
	struct movieNode* newMovieNode = (struct movieNode*) malloc(sizeof(struct movieNode));

	int i;
	int j;
	for(i = 0; i < 28; i++)
	{
		for(j = 0; j < 100; j++)
		{
			newMovieNode->movie[i][j] = movie[i][j];
		}
	}
	newMovieNode->next = (*headptr);
	(*headptr) = newMovieNode;
}

void GetMovie(char buffer[], char movie[28][200]){
	int i;
	int j = 0;
	int movieIndex;
	movieIndex = 0;
	char field[200];
	char isQuote = 'N';
    for(i = 0; buffer[i] != '\0'; i++)
    {
		if(buffer[i] == '\"')
		{
			if(isQuote == 'Y')
				isQuote = 'N';
			else
				isQuote = 'Y';
		}
		else if((buffer[i] == ',' || buffer[i] == '\n') && isQuote == 'N')
		{
			field[j] = '\0';
			strcpy(movie[movieIndex], field);
			j = 0;
			movieIndex++;
		}
		else
		{
			field[j] = buffer[i];
			j++;
		}
    }
}
