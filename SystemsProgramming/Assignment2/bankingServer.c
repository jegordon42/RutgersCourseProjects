#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include "bank.h"
#include "bankingServer.h"

sem_t bankMutex;
FDnode* head = NULL;

int main(int argc, char **argv){
	if(argc != 2){
		printf("Error: Invalid number of arguments.\n");
		exit(1);
	}	

	signal(SIGINT, serverQuitSignal);

	sem_init(&bankMutex, 0, 1);

	int port = atoi(argv[1]);
	if(port <= 8192 || port > 65535)
	{
		printf("Error: Please enter a port between 8192 and 65535\n");
		return 1;
	}
	pthread_t clientListeningThread;
	if(pthread_create(&clientListeningThread, 0, listenForClients, &port) != 0){
		printf("Error: Server was unable to start.\n");
		exit(EXIT_FAILURE);
	}

	pthread_t printingThread;
	pthread_create(&printingThread, 0, printBank, 0);

	pthread_join(clientListeningThread, NULL);

	sem_destroy(&bankMutex);

	printf("Server: Server is ending.\n");

	return 0;
}

void* listenForClients(void* arg){	
	int port = *(int *) arg;
	char sendBuff[1024];
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '\0', sizeof(sendBuff));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	int listeningFD;
	listeningFD = socket(AF_INET, SOCK_STREAM, 0);
	bind(listeningFD, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	while(1)
	{
		if(listen(listeningFD, 69) == -1)
		{
			printf("Error: Listening error.\n");
			return NULL;
		}
		int clientFD = accept(listeningFD, (struct sockaddr*) NULL, NULL);
		strcpy(sendBuff, "Server: Sucessfully connected to server.\n");
		printf("Server: Successfully connected to client.\n");
		send(clientFD, sendBuff, strlen(sendBuff), 0);
		
		pthread_t clientThread;
		if(pthread_create(&clientThread, 0, clientConnection, &clientFD) != 0)
		{
			printf("Error: Failed to launch client service session.\n");
			sleep(1);
		}		
	}
	return NULL;
}

void* clientConnection(void* arg){
	int clientFD = *(int *)arg;	

	addFD(clientFD);
	
	bankAccountNode* curAccount = NULL;
	while(1)
	{
		char message[1024] = {'\0'};
		int num = recv(clientFD, message, 1024,0);
		if (num == -1 || num == 0) 
		{
			printf("Server: Connection was terminated with client.\n");
			break;
		}
		message[num] = '\0';

		printf("Server: Action received from client: %s.\n", message);
		curAccount = executeAction(curAccount, message);

		if (send(clientFD, message, strlen(message),0) == -1) 
		{
			printf("Error: Failed  to send message to client.\n");
            break;
		}
	}
	printf("Server: Closing client service session.\n");	
	deleteFD(clientFD);
	close(clientFD);	
	return NULL;
}

void* printBank(void* arg){
	while(1)
	{
		sem_wait(&bankMutex);
		printAccounts();
		sem_post(&bankMutex);
		sleep(15);
	}
}

bankAccountNode*  executeAction(bankAccountNode* curAccount, char message[]){
	char action[50];
	char argument[1024] = {'\0'};
	sscanf(message, "%s %s", action, argument);	
	memset(message, '\0', strlen(message));
	
	if(strcmp(action, "create") == 0)
	{
		if(isValidAccountName(argument) == FAIL){
			sprintf(message, "Error: Please enter a valid name for the account.");
			return curAccount;
		}
		if(strlen(argument) > 255){
			sprintf(message, "Error: account name cannot be longer than 255 characters.");
			return curAccount;	
		}
		if(curAccount != NULL){
			sprintf(message, "Error: Cannot open while in account session.");
			return curAccount;	
		}
		if(searchAccount(argument) != NULL){
			sprintf(message, "Error: That account already exists.");
			return curAccount;			
		}
		sem_wait(&bankMutex);
		createAccount(argument);
		sprintf(message, "Server: Created account: %s", argument);
		sem_post(&bankMutex);
	} 
	else if(strcmp(action, "serve") == 0)
	{	
		if(curAccount != NULL){
			sprintf(message, "Error: Currently in an account session already.");
			return curAccount;			
		}
		if(isValidAccountName(argument) == FAIL){
			sprintf(message, "Error: Please enter a valid name for the account.");
			return curAccount;
		}
		curAccount = searchAccount(argument);
		if(curAccount == NULL)
		{
			sprintf(message, "Error: Account does not exist.");
			return curAccount;			
		}
		sem_wait(&bankMutex);
		if(curAccount->status == INSERVICE){
			sprintf(message, "Error: Chosen account is already in service.");
			curAccount = NULL;
			sem_post(&bankMutex);
			return curAccount;	
		}
		curAccount->status = INSERVICE;
		sem_post(&bankMutex);
		sprintf(message, "Server: Serving account: %s", curAccount->accountName);
	} 
	else if(strcmp(action, "deposit") == 0)
	{
		if(curAccount == NULL){
			sprintf(message, "Error: Must be in service to deposit.");
			return curAccount;			
		}
		if(isValidAmount(argument) == FAIL){
			sprintf(message, "Error: Please enter a valid amount.");
			return curAccount;
		}
		sem_wait(&bankMutex);
		deposit(curAccount->accountName, atof(argument));
		sem_post(&bankMutex);
		sprintf(message, "Server: Deposit successful.");
	} 
	else if(strcmp(action, "withdraw") == 0)
	{
		if(curAccount == NULL){
			sprintf(message, "Error: Must be in service to withdraw.");
			return curAccount;
		}
		if(isValidAmount(argument) == FAIL){
			sprintf(message, "Error: Please enter a valid amount.");
			return curAccount;
		}
		sem_wait(&bankMutex);
		if(withdraw(curAccount->accountName, atof(argument)) == FAIL){
			sprintf(message, "Error: Invalid withdraw amount.");
			sem_post(&bankMutex);
			return curAccount;
		}
		sem_post(&bankMutex);
		sprintf(message, "Server: Withdrawl successful.");
	} 
	else if(strcmp(action, "query") == 0)
	{
		if(curAccount == NULL){
			sprintf(message, "Error: Must be in service to query.");
			return curAccount;
		}
		sprintf(message, "Account: %s has a balance of $%0.2lf", curAccount->accountName, curAccount->balance);
	} 
	else if(strcmp(action, "end") == 0)
	{		
		if(curAccount == NULL){
			sprintf(message, "Error: Must be in service to end.");
			return curAccount;
		}
		curAccount->status = INACTIVE;
		curAccount = NULL;
		sprintf(message, "Server: Account session ended.");	
	} 
	else if(strcmp(action, "quit") == 0)
	{
		if(curAccount != NULL)
			curAccount->status = INACTIVE;
		sprintf(message, "quit");		
	} 
	else 
	{
		sprintf(message, "Error: Action not found.");
		return curAccount;
	}
	return curAccount;
}

void serverQuitSignal(int sig){
	printf("\nServer: Server is shutting down and closing all of it's clients.\n");
	FDnode* curFD = head;
	while(curFD != NULL)
	{
		int fd = curFD->fd;
		if ((send(fd, "quit", 4, 0))== -1)
		 {
			printf("Error: Could not shut down a client.\n");
			close(fd);
            break;
		}
		curFD = curFD->next;
	}
	exit(0);
}

void addFD(int fd){
	FDnode* newFDnode = malloc(sizeof(FDnode));
	newFDnode->fd = fd;
	newFDnode->next = head;
	head = newFDnode;
}

void deleteFD(int fd){
    FDnode* temp = head; 
    if (temp != NULL && temp->fd == fd) 
    { 
        head = temp->next;
        free(temp);
        return; 
    } 
	FDnode* prev;
    while (temp != NULL && temp->fd != fd) 
    { 
        prev = temp; 
        temp = temp->next; 
    } 

    if (temp == NULL) 
		return; 

    prev->next = temp->next; 
    free(temp);  
}

int isValidAccountName(const char *s) {
  	while (*s != '\0') 
	{
    	if (!isspace(*s))
			return SUCCESS;
    	s++;
  	}
  return FAIL;
}

int isValidAmount(const char *s) {
	if (*s == '\0')
		return FAIL; 
  	while (*s != '\0') 
	{
    	if (!isdigit(*s))
			return FAIL;
    	s++;
  	}
  return SUCCESS;
}
