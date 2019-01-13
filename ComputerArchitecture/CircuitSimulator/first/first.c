#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "first.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr,"error\n");
		exit(1);
	}
	
	initConstVars();

	readCircuit(argv[1]);

	executeCircuit();

	return 0;
}

void executeCircuit(){
	int x;
	int y;
	gateNode* curGate;
	variableNode* curVar;
	for(x = 0; x < toThePower(2, numInputs); x++)
	{
		initInputs(x);
		for(y = 0; y < numGates; y++)
		{
			curGate = getGate(y);
			if(strcmp(curGate->type, "NOT") == 0)
			{
				curVar = getVariable(*(curGate->varIndices));
				int val = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 1));
				if(val == 0)
					curVar->value = 1;
				else
					curVar->value = 0;
			}
			if(strcmp(curGate->type, "AND") == 0)
			{
				curVar = getVariable(*(curGate->varIndices));
				int val1 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 1));
				int val2 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 2));
				if(val1 == 1 && val2 == 1)
					curVar->value = 1;
				else
					curVar->value = 0;
			}
			if(strcmp(curGate->type, "OR") == 0)
			{
				curVar = getVariable(*(curGate->varIndices));
				int val1 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 1));
				int val2 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 2));
				if(val1 == 1 || val2 == 1)
					curVar->value = 1;
				else
					curVar->value = 0;
			}
			if(strcmp(curGate->type, "NAND") == 0)
			{
				curVar = getVariable(*(curGate->varIndices));
				int val1 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 1));
				int val2 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 2));
				if(val1 == 1 && val2 == 1)
					curVar->value = 0;
				else
					curVar->value = 1;
			}	
			if(strcmp(curGate->type, "NOR") == 0)
			{
				curVar = getVariable(*(curGate->varIndices));
				int val1 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 1));
				int val2 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 2));
				if(val1 == 1 || val2 == 1)
					curVar->value = 0;
				else
					curVar->value = 1;
			}
			if(strcmp(curGate->type, "XOR") == 0)
			{
				curVar = getVariable(*(curGate->varIndices));
				int val1 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 1));
				int val2 = curVar->value;
				curVar = getVariable(*(curGate->varIndices + 2));
				if(val1 != val2)
					curVar->value = 1;
				else
					curVar->value = 0;
			}
			if(strcmp(curGate->type, "DECODER") == 0)
			{
				int i;
				int binaryExponent = 0;
				int output = 0;
				for(i = curGate->numInputs - 1; i >= 0; i--)
				{
					curVar = getVariable(*(curGate->varIndices + i));
					if(curVar->value == 1)
						output = output + toThePower(2, binaryExponent);
					binaryExponent++;
				}

				int decimalOutput = 0;
				for(;output;output = output >> 1)
					decimalOutput  ^= output;

				for(i = 0; i < toThePower(2, curGate->numInputs); i++)
				{
						curVar = getVariable(*(curGate->varIndices +curGate-> numInputs + i));
						if(i == decimalOutput)
							curVar->value = 1;
						else
							curVar->value = 0;
				}
			}
			if(strcmp(curGate->type, "MULTIPLEXER") == 0)
			{
				int i;
				int binaryExponent = 0;
				int grayCodeOutput = 0;
				
				for(i = curGate->numInputs + logTwo(curGate->numInputs) - 1; i >= curGate->numInputs; i--)
				{
					curVar = getVariable(*(curGate->varIndices + i));
					if(curVar->value == 1)
						grayCodeOutput = grayCodeOutput + toThePower(2, binaryExponent);
					binaryExponent++;
				}

				int decimalOutput = 0;
				for(;grayCodeOutput;grayCodeOutput = grayCodeOutput >> 1)
					decimalOutput  ^= grayCodeOutput;

				curVar = getVariable(*(curGate->varIndices + decimalOutput));
				variableNode* outputVar = getVariable(*(curGate->varIndices + (curGate->numInputs + logTwo(curGate->numInputs))));
				outputVar->value = curVar->value;
			}
		}
		int i;
		for(i = 0; i < numInputs + numOutputs; i++)
		{
			curVar = getVariable(i);
			if(i == numInputs + numOutputs - 1)
				printf("%d\n", curVar->value);
			else
				printf("%d ", curVar->value);
		}
	}
}

void initInputs(int sequenceNum){
	int grayCode = sequenceNum ^ (sequenceNum >> 1);
	int binaryNum[1000];

	int i = 0;
	for(i = numInputs - 1; i >= 0; i--)
	{
		binaryNum[i] = 0; 
	}

	i = 0;
	while(grayCode > 0)
	{
		binaryNum[i] = grayCode % 2;
		grayCode = grayCode / 2;
		i++;
	}
	
	int inputIndex = 0;
	variableNode* curVar;
	for(i = numInputs - 1; i >= 0; i--)
	{
		curVar = getVariable(inputIndex);
		curVar->value = binaryNum[i];
		inputIndex++;
	}
}

void readCircuit(char* fileName) {
    FILE* file = fopen(fileName, "r");
	if (file == NULL) 
	{
		fprintf(stderr, "error\n");
		exit(1);
	}
	else 
	{
		char line[1000];
		char word[100];
		int lineIndex;
		int wordIndex;
		while (fgets(line, 1000, file) != NULL) 
		{
			for(lineIndex = 0; line[lineIndex] != ' '; lineIndex++)
				word[lineIndex] = line[lineIndex];
			
			word[lineIndex] = '\0';

			if(strcmp(word, "INPUTVAR") == 0 || strcmp(word, "OUTPUTVAR") == 0)
			{
					int isInput = FALSE;
					if(strcmp(word, "INPUTVAR") == 0)
						isInput = TRUE;
					while(line[lineIndex] != '\n')
					{
						wordIndex = 0;
						for(lineIndex++; line[lineIndex] != ' ' && line[lineIndex] != '\n'; lineIndex++)
						{
							word[wordIndex] = line[lineIndex];
							wordIndex++;
						}
						word[wordIndex] = '\0';
						if(word[0] >= 'A' && word[0] <= 'Z')
						{
							variableNode* newVariable = malloc(sizeof(variableNode));
							strcpy(newVariable->name, word);
							newVariable->index = numVariables;
							newVariable->next = variableList;
							variableList = newVariable;
							numVariables++;
						}
						else
						{
							if(isInput == TRUE)
								numInputs = atoi(word);
							else
								numOutputs = atoi(word);
						}
					}
			}
			else
			{
				gateNode* newGate = malloc(sizeof(gateNode) * 100);
				strcpy(newGate->type, word);
				newGate->index = numGates;
				numGates++;
				if(strcmp(word, "DECODER") == 0 || strcmp(word, "MULTIPLEXER") == 0)
				{
					int isDecoder = FALSE;
					if(strcmp(word, "DECODER") == 0 )
						isDecoder = TRUE;
					wordIndex = 0;
					for(lineIndex++; line[lineIndex] != ' ' && line[lineIndex] != '\n'; lineIndex++)
					{
						word[wordIndex] = line[lineIndex];
						wordIndex++;
					}
					word[wordIndex] = '\0';
					newGate->numInputs = atoi(word);
					if(isDecoder == TRUE)
						newGate->varIndices = malloc(sizeof(int *) * (newGate->numInputs + toThePower(2, newGate->numInputs)));
					else
						newGate->varIndices = malloc(sizeof(int *) * (2 + newGate->numInputs + logTwo( newGate->numInputs)));	
				}
				else if(strcmp(word, "NOT") == 0)
				{
					newGate->numInputs = 1;
					newGate->varIndices = malloc(sizeof(int) * 2);
				}
				else
				{
					newGate->numInputs = 2;
					newGate->varIndices = malloc(sizeof(int) * 3);
				}
				int varIndex = 0;
				do
				{
					wordIndex = 0;
					for(lineIndex++; line[lineIndex] != ' ' && line[lineIndex] != '\n'; lineIndex++)
					{
						word[wordIndex] = line[lineIndex];
						wordIndex++;
					}
					word[wordIndex] = '\0';
					if(!isdigit(word[0]))
					{
						variableNode* curVar = getVariableByName(word);
						if(curVar == NULL)
						{
							curVar = malloc(sizeof(variableNode));
							strcpy(curVar->name, word);
							curVar->index = numVariables;
							curVar->next = variableList;
							variableList = curVar;
							numVariables++;
						}
						*(newGate->varIndices + varIndex) = curVar->index;
						varIndex++;
					}
					else
					{
						if(strcmp(word, "1") == 0)
							*(newGate->varIndices + varIndex) = -1; //trueVar index
						else
							*(newGate->varIndices + varIndex) = -2; //falseVar index

						varIndex++;
					}
				}while(line[lineIndex] != '\n');

				newGate->next = gateList;
				gateList = newGate;
			}
		}
 	}
	if (file) {
		fclose(file);
	}
}

variableNode* getVariableByName(char name[100]){
	variableNode* curVar = variableList;
	while(curVar != NULL)
	{
		if(strcmp(curVar->name, name) == 0)
			break;
		curVar = curVar->next;
	}
	return curVar;
}

variableNode* getVariable(int index){
	if(index == -2)
		return falseVar;
	if(index == -1)
		return trueVar;
	variableNode* curVar = variableList;
	while(curVar != NULL)
	{
		if(curVar->index == index)
			break;
		curVar = curVar->next;
	}
	return curVar;
}

gateNode* getGate(int index){
	gateNode* curGate = gateList;
	while(curGate != NULL)
	{
		if(curGate->index == index)
			break;
		curGate = curGate->next;
	}
	return curGate;
}

int logTwo(int input) {
	int index = 0;
	for (index = 0; index < 31; index++) {
		if ((input >> index) == 1)	{
			break;
		}
	}
	return index;
}

int toThePower(int x, int n)  {
    int i;
    int number = 1;
    for (i = 0; i < n; ++i) {
        number *= x;
	}
    return number;
}

void initConstVars(){
	trueVar = malloc(sizeof(variableNode));
	strcpy(trueVar->name, "1");
	trueVar->value = 1;
	trueVar->index = -1;
	falseVar = malloc(sizeof(variableNode));
	strcpy(falseVar->name, "0");
	falseVar->value = 0;
	falseVar->index = -2;
}
