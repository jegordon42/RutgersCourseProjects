void* listenForClients(void* arg);
void* clientConnection(void* arg);
bankAccountNode* executeAction(bankAccountNode* curAccount, char message[]);
void* printBank(void* arg);
int isValidAccountName(const char *s);
int isValidAmount(const char *s);

void serverQuitSignal(int sig);

typedef struct FDnode{
	int fd;
	struct FDnode *next;	
} FDnode;

void deleteFD(int fd);
void addFD(int fd);




