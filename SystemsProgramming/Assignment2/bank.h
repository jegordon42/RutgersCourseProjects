enum State{INSERVICE, INACTIVE, SUCCESS, FAIL};

typedef struct bankAccountNode{
	char accountName[256];
	double balance;
	int status;
	struct bankAccountNode* next;
}bankAccountNode;

bankAccountNode* searchAccount(char inAccountName[256]);
void createAccount(char inAccountName[256]);
int withdraw(char inAccountName[256], double amount);
void deposit(char inAccountName[256], double amount);
void printAccounts();
