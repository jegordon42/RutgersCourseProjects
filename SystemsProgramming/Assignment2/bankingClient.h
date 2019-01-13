enum state{FALSE, TRUE};

char receiving[1024];
char sending[1024];
int serverFD = 0;

int hasMessage(char *string);
void* outputThread(void* fd);
void* inputThread(void* fd);
void clientQuitSignal(int sig);
