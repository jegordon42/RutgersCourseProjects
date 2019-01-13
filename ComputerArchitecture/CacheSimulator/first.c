#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "first.h"

int main(int argc, char** argv) {
	if (argc!= 6) {
		fprintf(stderr,"error\n");
		exit(1);
	}

	initializeCache(argv[1], argv[2], argv[3], argv[4], argv[5]);
	cacheBlock* 	allBlocks = malloc(sizeof(cacheBlock) * totalBlockCount);
	cacheSet* theCache = malloc(sizeof(cacheSet) * totalSetCount);	

	resetCounts();
	initializeCacheData(allBlocks, theCache);
	runSimulation(theCache, FALSE);
	printf("no-prefetch\n");
	printCounts();

	resetCounts();
	initializeCacheData(allBlocks, theCache);
	runSimulation(theCache, TRUE);
	printf("with-prefetch\n");
	printCounts();

	free(allBlocks);
	free(theCache);
	return 0;
}

void resetCounts() {
	hitCount = 0;
	missCount = 0;
	readCount = 0;
	writeCount = 0;
}

void printCounts() {
	printf("Memory reads: %d\n", readCount);
	printf("Memory writes: %d\n", writeCount);
	printf("Cache hits: %d\n", hitCount);
	printf("Cache misses: %d\n", 	missCount);
}

int logTwo(int input) {
	int index = 0;
	for (index = 0; index < 31; index++) 
		if ((input >> index) == 1)	
			break;
	return index;
}

int isValidNum(char* input)
{
	int i;
	for(i = 0; i < strlen(input); i++)
	{
		if(!isdigit(input[i]))
			return FALSE;
	}

	int numInput = atoi(input);
	int num = 1;
	while(num <= numInput)
	{
		if(num == numInput)
			return TRUE;
		num = num * 2;
	}
	return FALSE;
}

unsigned long hexToLong(char* input) {
	unsigned long output = 0;
	int index = 0;

	if (*(input + index) == '0' && (*(input + index + 1) == 'x' || *(input + index + 1) == 'X') ) 
		index = index + 2;

	while (*(input + index) != 0) 
	{
		char oneChar = *(input + index);
		output = output * 16;

		if (oneChar > '0' && oneChar <= '9') 
			output = output + (oneChar - '0');
		else if (oneChar >= 'a' && oneChar <= 'f') 
			output = output + (oneChar - 'a') + 10;
		else if (oneChar >= 'A' && oneChar <= 'F')
			output = output + (oneChar - 'A') + 10;

		index++;
	}
	return output;
}

int getLRUIndex(cacheSet* curSet, int justUsedIndex){
	int i;	
	for(i = 0; i < blocksPerSet; i++)
	{
		if(i == justUsedIndex)
			*(curSet->LRUtracker + i)  = 0;
		else if(*(curSet->LRUtracker + i) == -1)
			return i;
		else
			*(curSet->LRUtracker + i) = *(curSet->LRUtracker + i) + 1;	
	}

	int LRUindex = 0;
	int LRUvalue = 0;
	for(i = 0; i < blocksPerSet; i++)
	{
		if(*(curSet->LRUtracker + i) > LRUvalue)
		{
				LRUindex = i;
				LRUvalue = *(curSet->LRUtracker + i);
		}
	}
	return LRUindex;
}

void runSimulation(cacheSet* theCache, int prefetch) {
    FILE *file = fopen(traceFileName, "r");
	if (file == NULL)
	{
		fprintf(stderr, "error\n");
		exit(1);
	}
	else 
	{
		char line[128];
		char programCounter[32];
		char readWrite;
		char address[32];
		
		while (fgets(line, 128, file) != NULL) 
		{
			int colCount = sscanf(line, "%s %c %s", programCounter, &readWrite, address);
			if (colCount == 3) 
			{
				if (readWrite == 'R' || readWrite == 'r') 
					readOrWrite(READ, theCache, hexToLong(address), prefetch, FALSE);
				else if (readWrite == 'W' || readWrite == 'w') 
					readOrWrite(WRITE, theCache, hexToLong(address), prefetch, FALSE);
			}
			else if (colCount == 1) 
			{
				if (strcmp(programCounter, "#eof")==0) 
					break;
			}
		}
 	}
	if (file) {
		fclose(file);
	}
}

void initializeCacheData(cacheBlock* allBlocks, cacheSet* theCache) {
	int i;
	for (i=0; i< totalBlockCount; i++) 
	{
		cacheBlock* curBlock = allBlocks + i;
		curBlock->tag	= 0;
		curBlock->valid	= FALSE;
	}

	for (i=0; i < totalSetCount; i++) 
	{
		cacheSet* curSet = theCache + i;
		curSet->blocksInSet	= allBlocks + i * blocksPerSet;
	
		curSet->LRUtracker 	= malloc(sizeof(int) * blocksPerSet);
		int j;		
		for(j = 0; j <  blocksPerSet; j++)
			*(curSet->LRUtracker + j) = -1;

		curSet->indexToOverwrite	= 0;
	}
}

unsigned long getPartFromLong(unsigned long input, int start, int length) {
	unsigned long output;

	if (length == 0) 
		output = 0;
	else 
	{
		output  = input << (64  - length - start);
		output  = output >> (64  - length);
	}

	return output;
}

void readOrWrite(enum ReadWrite readWrite,cacheSet* theCache, unsigned long address, int prefetch, int adjacent) {
	unsigned long tag = getPartFromLong(address, tagStart, tagLength);
	int index 	=  (int)getPartFromLong(address, indexStart, indexLength);
	cacheSet* curSet = theCache + index;

	int hit = FALSE;
	int i;
	for (i=0; i < blocksPerSet; i++) {
		cacheBlock* curBlock = (curSet->blocksInSet + i);
		if (curBlock->tag == tag && curBlock->valid == TRUE) 
		{
			if(adjacent == FALSE)
			{
				hitCount++;
				if (readWrite == WRITE)
					writeCount++;

				if(theCachePolicy == LRU)
					curSet->indexToOverwrite = getLRUIndex(curSet, i);
			}
			hit = TRUE;
			break;
		}
	}

	if (hit == FALSE) 
	{
		cacheBlock* curBlock = (curSet->blocksInSet + curSet->indexToOverwrite);
		
		if(theCachePolicy == LRU)
			curSet->indexToOverwrite = getLRUIndex(curSet, curSet->indexToOverwrite);
		else// FIFO
		{
			curSet->indexToOverwrite++;
			curSet->indexToOverwrite = curSet->indexToOverwrite % blocksPerSet;
		}		

		readCount++;

		curBlock->tag = tag;
		curBlock->valid = TRUE;
		if(adjacent == FALSE)
		{
			missCount++;
			if (readWrite == WRITE) 
				writeCount++;
		}

		if(prefetch)
			readOrWrite(readWrite, theCache, address + bytesPerBlock, FALSE, TRUE);
	}
}

void initializeCache(char* inCacheSize, char* inAssociativity, char* inCachePolicy, char* inBytesPerBlock, char* inTraceFileName) {	
	if(!isValidNum(inCacheSize) || !isValidNum(inBytesPerBlock))
	{
		fprintf(stderr,"error\n");
		exit(1);
	}

	traceFileName = inTraceFileName;
	totalBytesInCache = atoi(inCacheSize);
	bytesPerBlock	= atoi(inBytesPerBlock);
	totalBlockCount = totalBytesInCache / bytesPerBlock;

	if (strcmp(inAssociativity,"direct")==0)
		blocksPerSet = 1;
	else if (strcmp(inAssociativity, "assoc")==0) 
		blocksPerSet = totalBlockCount;
	else 
	{
		if (strstr (inAssociativity, "assoc:") != NULL) 
		{
			if(!isValidNum(inAssociativity+strlen("assoc:")))
			{
				fprintf(stderr,"error\n");
				exit(1);
			}
			blocksPerSet = atoi(inAssociativity+strlen("assoc:") );
		}
		else 
		{
			fprintf(stderr,"error\n");
			exit(1);
		}
	}

	if(strcmp(inCachePolicy,"fifo")==0)
		theCachePolicy = FIFO;
	else if(strcmp(inCachePolicy,"lru")==0)
		theCachePolicy = LRU;
	else
	{
		fprintf(stderr,"error\n");
		exit(1);
	}

	totalSetCount = totalBlockCount / blocksPerSet;		
	tagStart = logTwo(bytesPerBlock) + logTwo(totalSetCount);		
	tagLength = 48 - logTwo(bytesPerBlock) - logTwo(totalSetCount);
	indexStart = logTwo(bytesPerBlock);
	indexLength = logTwo(totalSetCount);

	if (totalBytesInCache <= 0 || bytesPerBlock <= 0 || blocksPerSet <=0 || totalBlockCount <= 0 || totalSetCount <= 0 
		|| totalBytesInCache != bytesPerBlock * totalBlockCount || totalBytesInCache != bytesPerBlock * blocksPerSet * totalSetCount) 
	{
		fprintf(stderr,"error\n");
				exit(1);
	}
}
