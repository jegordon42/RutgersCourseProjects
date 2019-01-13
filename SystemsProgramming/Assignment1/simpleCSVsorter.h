//These are the nodes to store the linked list of all of the movies
struct movieNode
{
    char movie[28][200]; //this stores an array of movie properties
    struct movieNode* next;
};
//This is used to store the column names and datatypes of the CSV
struct Column
{
    char name[50];
    int dataType;
};

char buffer[1024];//this is what each line of the CSV is read into
int numProcesses(char *path, char searchColumn[]);
int searchColumnType;
int hasOutputPath;
enum pathType{DIR_TYPE, DIR_NOT_FOUND, PERMISSION_DENIED, OTHER_FILE_TYPE, FILE_NOT_FOUND, CSV_TYPE, INV_TYPE, UNFOUND_TYPE};
enum columnType{STRING_TYPE, NUMERIC_TYPE, DATETIME_TYPE, NOTFOUND_TYPE};

void CreateCSV(char * inputPath, char * outputPath, char searchColumn[]);
int GetDataType(char input[]);
int GetColumnIndex(char titleRow[28][200],  char searchColumn[], int columnCount);
int GetMovie(char buffer[], char movie[28][200]); //parses a line of a CSV and stores it into the movie array
void Add(struct movieNode** headptr, char  movie[28][200], int columnCount); //pushes a Node to the linked list
int getPathType(char * path, char searchColumn[]);
void EvaluateDir(char * curDirPath, char *outputDirPath, char searchColumn[]);


struct movieNode * MergeSort (struct movieNode * start, int sortColumnIndex); //sorts the linked list
struct movieNode * Merge(struct movieNode * first, struct movieNode * second, int sortColumnIndex);
int Compare(char value1[], char value2[]); //compares two values based on their data type
char* DeleteSpace(char * field, int length); //deletes leading/trailing spaces of a string
