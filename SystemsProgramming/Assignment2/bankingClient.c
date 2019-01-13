#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "bankingClient.h"

int main(int argc, char** argv)
{
	int i = 0;
	int k;	
	struct sockaddr_in serv_addr;
	k = sizeof(receiving);

	if(argc!=3){
		printf("Error: Incorrect number of args\n");
		exit(1);
	}
	memset(receiving, '\0', k);
	
	serverFD = socket(AF_INET, SOCK_STREAM, 0);

	if(serverFD  < 0)
	{
		printf("Error: Socket could not be created\n");
		return 1;
	}
	
	struct hostent* hostEntry;
	hostEntry = gethostbyname(argv[1]);
	char* ipaddr = inet_ntoa(*((struct in_addr*) hostEntry->h_addr_list[0]));
	int port = atoi(argv[2]);
	if(port <= 8192 || port > 65535)
	{
		printf("Error: Please enter a port between 8192 and 65535\n");
		return 1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
	
	while(connect(serverFD, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
		printf("Currently connecting with the server. If not connected within 10 s, Close terminal and try again.\n");	
		sleep(3);	
	}	
	
	k = k - 1;
	i = read(serverFD, receiving, k);
	if(i < 0)
		printf("Error: Did not read correctly.\n");
	else if(i > 0)
	{		
		receiving[i] = 0;
		if(fputs(receiving, stdout) == EOF)
		{
			printf("Error. Fputs did not execute correctly.\n");
		}		
	}
	
	pthread_t output;
	pthread_t input;
	
	signal(SIGINT, clientQuitSignal);

	if(pthread_create(&output, 0, outputThread, NULL) != 0){
		printf("Error: output thread did not fire.\n");
		exit(1);
	}

	if(pthread_create(&input, 0, inputThread, NULL) != 0){
		printf("Error: input thread did not fire.\n");
		exit(1);
	}
	
	pthread_join(input, NULL);
	printf("Client closed.\n");
	return 0;
		
}

void clientQuitSignal(int sig){
	memset(sending, '\0', strlen(sending));
	strcpy(sending, "quit");
	
	if ((send(serverFD,sending, strlen(sending),0))== -1) {
		printf("Error: Could not close client account session on exit.\n");
		close(serverFD);
		exit(1);
    }
	printf("Client will close. Thank You.\n");
	exit(0);
}

int hasMessage(char *string){
  for( ;*string != '\0'; string++) {
    if (!isspace(*string))
      return TRUE;
  }
  return FALSE;
}

void* outputThread(void* fd){
	int i = strlen(receiving);
	while(1){
		memset(receiving, '\0', i);
		int recievedMessage = recv(serverFD, receiving, sizeof(receiving), 0);
		if(recievedMessage <= 0){
			printf("The Connection has been terminated.\n");
			exit(0);
		}
		
		if(strcmp(receiving, "quit") == 0){
			printf("Client will close. Thank You.\n");
			exit(0);
		}

		if(hasMessage(receiving) == TRUE){
			printf("%s\n", receiving);
		}
	}
	return NULL;
}


void* inputThread(void* fd){
	int i = strlen(sending);
	int k;
	while(1){
		printf("What action would you like to take?\n");
		
		char command[1024];
		memset(command, '\0', 1024);
		memset(sending, '\0', i);
		
		char firstArg[1024];
		char secondArg[1024];
		char inv[50];
		fgets(command, 1024, stdin);
		k = strlen(command);
		if ((k>0) && (command[k - 1] == '\n')){
			command[k - 1] = '\0';
		}
		int numArgs = sscanf(command, "%s %s %s", firstArg, secondArg, inv);
		if(numArgs > 2)
		{
			printf("Error: Command was not valid\n");
			continue;
		}
		else if(numArgs == 2)
		{
			strcat(firstArg, " ");
			strcat(firstArg, secondArg);
		}
		strcpy(sending, firstArg);
		//printf("%s \n", sending);
		if((send(serverFD,sending, strlen(sending),0))== -1) {
                printf("Error: message was not sent.\n");
                close(serverFD);
                exit(1);
        }

        sleep(2);
	}
	return NULL;
}
