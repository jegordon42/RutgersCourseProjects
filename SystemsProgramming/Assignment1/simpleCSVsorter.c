#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include "simpleCSVsorter.h"
#include "mergesort.c"


// FFRREEEEE, test errors, 

int main(int argc, char **argv)
{
	if(argc != 3 && argc != 5 && argc != 7)
	{
		fprintf(stderr, "Invalid quantity of parameters.");
		return 0;
	}
	if(strcmp(argv[1], "-c") != 0)
	{ 
		fprintf(stderr, "Invalid parameter: specify what you want to sort(missing '-c').");
		return 0;
	}
	
	searchColumnType = GetDataType(argv[2]);
	if(searchColumnType == NOTFOUND_TYPE)
	{
		fprintf(stderr, "Invalid column to be searched on.");
		return 0;
	}

	char *curDirPath;
	char *outputDirPath;
	if(argc > 3 && strcmp(argv[3], "-d") == 0)
	{
		curDirPath = malloc(strlen(argv[4]) * sizeof(char *));
		strcpy(curDirPath, argv[4]);
		if(argc > 5 && strcmp(argv[5],"-o") == 0)
		{
			outputDirPath = malloc(sizeof(char*) * strlen(argv[6]));
			strcpy(outputDirPath, argv[6]);
			hasOutputPath = 1;
		}
		else
		{
			outputDirPath = malloc(sizeof(char*) * 2000);
			getcwd(outputDirPath, sizeof(outputDirPath));
			hasOutputPath = 0;
		}
	}
	else if(argc > 3 && strcmp(argv[3], "-o") == 0)
	{
		outputDirPath = malloc(strlen(argv[4]) * sizeof(char *));
		curDirPath = malloc(sizeof(char*) * 2000);
		getcwd(curDirPath, sizeof(curDirPath));
		strcpy(outputDirPath, argv[4]);
		hasOutputPath = 1;
	}
	else
	{
		curDirPath = malloc(sizeof(char*) * 2000);
		outputDirPath = malloc(sizeof(char*) * 2000);
		getcwd(curDirPath, sizeof(curDirPath));
		getcwd(outputDirPath, sizeof(outputDirPath));
		hasOutputPath = 0;
	}
	int pathType = getPathType(outputDirPath, argv[2]);
	if(pathType == PERMISSION_DENIED)
	{
		fprintf(stderr, "Output directory does not have valid permissions.");
		return 0;
	}
	else if(pathType == DIR_NOT_FOUND)
	{
		fprintf(stderr, "Output directory does not exist");
		return 0;
	}
	else if(pathType == CSV_TYPE || pathType == OTHER_FILE_TYPE || pathType == FILE_NOT_FOUND )
	{
		fprintf(stderr, "Output specified is not a directory.");
		return 0;
	}
	else if(pathType != DIR_TYPE)
	{
		fprintf(stderr, "Cannot access output directory");
		return 0;
	}
	pathType = getPathType(curDirPath, argv[2]);
	if(pathType == PERMISSION_DENIED)
	{
		fprintf(stderr, "Input directory does not have valid permissions.");
		return 0;
	}
	else if(pathType == DIR_NOT_FOUND)
	{
		fprintf(stderr, "Input directory does not exist");
		return 0;
	}
	else if(pathType == CSV_TYPE || pathType == OTHER_FILE_TYPE || pathType == FILE_NOT_FOUND )
	{
		fprintf(stderr, "Input specified is not a directory.");
		return 0;
	}
	else if(pathType != DIR_TYPE)
	{
		fprintf(stderr, "Cannot access input directory");
		return 0;
	}
	fprintf(stdout, "Initial PID: %d\n", getpid());
	fprintf(stdout, "PIDS of all child processes: ");
	fflush(stdout);
	
	EvaluateDir(curDirPath, outputDirPath, argv[2]);
	
	fprintf(stdout, "\n");
	fprintf(stdout, "Total number of processes: %d \n", numProcesses(curDirPath, argv[2]));
	free(curDirPath);
	free(outputDirPath);
	return 0;
}

void EvaluateDir(char * curDirPath, char *outputDirPath, char searchColumn[])
{
	DIR *curDir = opendir(curDirPath);
	char *curPath;
	char *newFilePath;
	int pid1;
	int pid2;
	int pathType;			
	struct dirent *dent;
	
	while((dent = readdir(curDir)) != NULL)
	{
		if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
			continue;
		curPath = malloc((strlen(curDirPath) + 1 + strlen(dent->d_name)) * sizeof(char *));
		strcpy(curPath, curDirPath);
		strcat(curPath, "/");
		strcat(curPath, dent->d_name);
		pathType = getPathType(curPath, searchColumn);
		if(pathType == DIR_TYPE)
		{
				pid1 = fork();
				if(pid1 == 0)
				{
					//printf("%d, %s\n ", getpid(),dent->d_name);
					fprintf(stdout, "%d, ", getpid());
					fflush(stdout);
					curDirPath = malloc(strlen(curPath) * sizeof(char *));
					strcpy(curDirPath, curPath);
					EvaluateDir(curDirPath, outputDirPath, searchColumn);
					exit(0);
				}
				else
					wait(0);

		}
		else if(pathType == CSV_TYPE || pathType == OTHER_FILE_TYPE || PERMISSION_DENIED)
		{
			pid2 = fork();
			if (pid2 == 0)
			{
				//printf("%d, %s\n ", getpid(),dent->d_name);
				fprintf(stdout, "%d, ", getpid());
				fflush(stdout);
				if(pathType == CSV_TYPE)
				{
					newFilePath = malloc((strlen(outputDirPath) + 1 + strlen(dent->d_name)) * sizeof(char *));
					strcpy(newFilePath, outputDirPath);
					strcat(newFilePath, "/");
					strcat(newFilePath, dent->d_name);
					CreateCSV(curPath, newFilePath, searchColumn);
				}
				else if(pathType == PERMISSION_DENIED)
				{
					fprintf(stderr, "File: %s permissions not met\n", dent->d_name);
				}
				else if(pathType == OTHER_FILE_TYPE)
				{
					fprintf(stderr, "File: %s is not in the correct CSV format\n", dent->d_name);
				}
				else if(pathType == FILE_NOT_FOUND)
				{
					fprintf(stderr, "File: %s was not found\n", dent->d_name);
				}
				
				exit(0);
			}
			else
				wait(0);
		}	
	}
}

void CreateCSV(char * inputPath, char * outputPath, char searchColumn[])
{
	char buffer[1024];
	FILE *fptr = fopen(inputPath, "r");
	fgets(buffer, sizeof(buffer), fptr); //get title column
	char titleRow[28][200];
	int columnCount = GetMovie(buffer, titleRow); 
	struct movieNode* titleMovie = NULL;
	Add(&titleMovie, titleRow, columnCount);
	int searchColumnIndex = GetColumnIndex(titleRow, searchColumn, columnCount);
	if(searchColumnIndex == -1)
	{
		fprintf(stderr, "Column to be sorted not found at location : %s.\n", inputPath);
	}
		return;
	int i;
	struct movieNode* curMovie = NULL;
  	for (i = 0; fgets(buffer, sizeof(buffer), fptr); i++){  //loop through every line of CSV
    	char movie[28][200];
		GetMovie(buffer, movie); //convert each line into an array of String
		Add(&curMovie, movie, columnCount); // add the movie to the linked list
  	}
	
	curMovie = MergeSort(curMovie, searchColumnIndex); //sort the linked list
	
	titleMovie->next = curMovie; //put the title row in front of list
	curMovie = titleMovie;

	char newFileName[256];
	if(hasOutputPath == 0)
		strcpy(newFileName,inputPath);
	else
		strcpy(newFileName,outputPath);

	newFileName[strlen(newFileName) - 4] = '\0';
	strcat(newFileName, "-sorted-");
	strcat(newFileName, searchColumn);
	strcat(newFileName, ".csv");

	FILE *newFile = fopen(newFileName, "w");

	struct movieNode* prev;
	while(curMovie != NULL)
	{
		int j;
		for(j = 0; j<columnCount-1;j++){
			fprintf(newFile,"%s,", curMovie->movie[j]);
		}
		fprintf(newFile,"%s\n", curMovie->movie[columnCount-1]);

		prev = curMovie;
		curMovie = curMovie->next;
		free(prev);
	}
  	fclose(newFile);
}

int numProcesses(char *path, char searchColumn[]){
	int processCount = 1;
	DIR *curDir = opendir(path);
	char curPath[256];

	struct dirent *dent;
	while((dent = readdir(curDir)) != NULL)
	{
		if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
			continue;

		strcpy(curPath, path);
		strcat(curPath, "/");
		strcat(curPath, dent->d_name);
		int pathType = getPathType(curPath, searchColumn);
		if(pathType == DIR_TYPE)
			processCount = processCount + numProcesses(curPath, searchColumn);
		else if(pathType == CSV_TYPE || pathType == OTHER_FILE_TYPE || PERMISSION_DENIED)
			processCount++;
	}
	return processCount;
}
int GetColumnIndex(char titleRow[28][200],  char searchColumn[], int columnCount){
	int index;
	for (index = 0;index<columnCount; index++)
	{
		if(strcmp(titleRow[index], searchColumn) == 0)
			return index;
	}
	return -1;
}

int GetDataType(char input[]){
	struct Column columns[28] =
	{
		{"color", STRING_TYPE},
		{"director_name", STRING_TYPE},
		{"num_critic_for_reviews", NUMERIC_TYPE},
		{"duration", DATETIME_TYPE},
		{"director_facebook_likes", NUMERIC_TYPE},
		{"actor_3_facebook_likes", NUMERIC_TYPE},
		{"actor_2_name", STRING_TYPE},
		{"actor_1_facebook_ltymikes", NUMERIC_TYPE},
		{"gross", NUMERIC_TYPE},
		{"genres", STRING_TYPE},
		{"actor_1_name", STRING_TYPE},
		{"movie_title", STRING_TYPE},
		{"num_voted_users", NUMERIC_TYPE},
		{"cast_total_facebook_likes", NUMERIC_TYPE},
		{"actor_3_name", STRING_TYPE},
		{"facenumber_in_poster", NUMERIC_TYPE},
		{"plot_keywords", STRING_TYPE},
		{"movie_imdb_link", STRING_TYPE},
		{"num_user_for_reviews", NUMERIC_TYPE},
		{"language", STRING_TYPE},
		{"country", STRING_TYPE},
		{"content_rating", STRING_TYPE},
		{"budget", NUMERIC_TYPE},
		{"title_year", NUMERIC_TYPE},
		{"actor_2_facebook_likes", NUMERIC_TYPE},
		{"imdb_score", NUMERIC_TYPE},
		{"aspect_ratio", NUMERIC_TYPE},
		{"movie_facebook_likes", NUMERIC_TYPE}
	};

	int i;
	for (i = 0;i<28; i++)
	{
		if(strcmp(columns[i].name, input) == 0)
			return columns[i].dataType;
	}

	return NOTFOUND_TYPE;
}


void Add(struct movieNode **headptr, char movie[28][200], int columnCount) {
	struct movieNode* newMovieNode = (struct movieNode*) malloc(sizeof(struct movieNode));

	int i;
	int j;
	for(i = 0; i < columnCount; i++)
	{
		for(j = 0; j < 200; j++)
		{
			newMovieNode->movie[i][j] = movie[i][j];
		}
	}
	newMovieNode->next = (*headptr);
	(*headptr) = newMovieNode;
}

int GetMovie(char buffer[], char movie[28][200]){
	int columnCount = -1;

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
			columnCount = movieIndex;
		}
		else
		{
			field[j] = buffer[i];
			j++;
		}
    }
	return columnCount;
}

int getPathType(char * path, char searchColumn[]){
	struct stat s;
	if(stat(path, &s) == 0)
	{
		if(s.st_mode & S_IFDIR)
		{
			DIR *dir = opendir(path);
			if(dir)
			{
				closedir(dir);
				
				return DIR_TYPE;
			}
			
			else if(ENOENT == errno)
				return DIR_NOT_FOUND;
			else if(EACCES == errno)
				return PERMISSION_DENIED;
			else
				return INV_TYPE;
		}
		else if(s.st_mode & S_IFREG)
		{
			FILE *fptr = fopen(path, "r");
			if(fptr)
			{
				fclose(fptr);
				char alreadySorted[256];
				strcpy(alreadySorted, "-sorted-");
				strcat(alreadySorted, searchColumn);
				if(path[strlen(path) - 4] == '.' && path[strlen(path) - 3] == 'c' && path[strlen(path) - 2] == 's'  && path[strlen(path) - 1] == 'v' && (strstr(path, alreadySorted) == NULL)) 
					return CSV_TYPE;
				else
					return OTHER_FILE_TYPE;
			}
			else if(ENOENT == errno)
			{
				return FILE_NOT_FOUND;
			}
			else if(EACCES == errno)
				return PERMISSION_DENIED;
			else 
				return INV_TYPE;
		}
		else
			return INV_TYPE;
		
	}
	return UNFOUND_TYPE;
}
