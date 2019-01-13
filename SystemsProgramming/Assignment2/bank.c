#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank.h"
#include "bankingServer.h"

bankAccountNode* allBankAccounts = NULL;

bankAccountNode* searchAccount(char inAccountName[256]){
	bankAccountNode* curAccount = allBankAccounts;
	while(curAccount != NULL)
	{
		if(strcmp(curAccount->accountName, inAccountName) == 0)
			return curAccount;
		curAccount = curAccount->next;
	}
	return NULL;
}

void createAccount(char inAccountName[256]){
	bankAccountNode* newAccount	= malloc(sizeof(bankAccountNode));
	strcpy(newAccount->accountName, inAccountName);
	newAccount->balance = 0;
	newAccount->status = INACTIVE;
	newAccount->next = allBankAccounts;
	allBankAccounts = newAccount;
}

int withdraw(char inAccountName[256], double amount){
    bankAccountNode* curAccount = searchAccount(inAccountName);
    
    if(curAccount->balance < amount)
		return FAIL;

    curAccount->balance -= amount;
    return SUCCESS;
}

void deposit(char inAccountName[256], double amount){
    bankAccountNode* curAccount = searchAccount(inAccountName);
    curAccount->balance += amount;
}

void printAccounts(){
	printf("Server: Current balances:\n");
	bankAccountNode* curAccount = allBankAccounts;
	if(curAccount == NULL)
		printf("No accounts in bank:\n");
	while(curAccount != NULL)
	{
		printf("%s\t$%0.2lf", curAccount->accountName, curAccount->balance);
		if(curAccount->status == INSERVICE)
			printf("\tIN SERVICE");
		printf("\n");
		
		curAccount = curAccount->next;
	}
}
