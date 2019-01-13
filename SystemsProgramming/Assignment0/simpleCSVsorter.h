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
    char dataType[20];
};

int columnIndex; //this stores the column index of the column the movies will be sorted on
char buffer[1024];//this is what each line of the CSV is read into

int GetDataType(char input[]);
void GetMovie(char buffer[], char movie[28][200]); //parses a line of a CSV and stores it into the movie array
char* DeleteSpace(char * field, int length); //deletes leading/trailing spaces of a string
void Add(struct movieNode** headptr, char  movie[28][200]); //pushes a Node to the linked list

struct movieNode * MergeSort (struct movieNode * start, char type[]); //sorts the linked list
struct movieNode * Merge(struct movieNode * first, struct movieNode * second, char type[]);
int Compare(char value1[], char value2[], char datatype[]); //compares two values based on their data type
