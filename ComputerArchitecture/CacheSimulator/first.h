#define FALSE 0
#define TRUE 1

int	totalBytesInCache;
int	totalBlockCount;	
int	totalSetCount;
int	bytesPerBlock;
int	blocksPerSet;
int tagStart;								
int tagLength;
int indexStart;
int indexLength;
unsigned int hitCount;
unsigned int	missCount;
unsigned int	readCount;
unsigned int	writeCount;
char* traceFileName;
enum ReadWrite{READ, WRITE};
typedef enum cachePolicy{
	FIFO,
	LRU
} cachePolicy;

cachePolicy theCachePolicy;

typedef struct  cacheBlock {
	unsigned long tag;
	int valid;
} cacheBlock;

typedef struct cacheSet {
	cacheBlock* 	blocksInSet;
	int* LRUtracker;
	int	indexToOverwrite;
} cacheSet;

void resetCounts();
void printCounts();
int logTwo(int input);
int isValidNum(char* input);
unsigned long hexToLong(char* input);
int getLRUIndex(cacheSet* curSet, int justUsedIndex);
void runSimulation(cacheSet* theCache, int prefetch);
void initializeCacheData(cacheBlock* allBlocks, cacheSet* theCache);
unsigned long getPartFromLong(unsigned long input, int start, int length);
void readOrWrite(enum ReadWrite readWrite,cacheSet* theCache, unsigned long address, int prefetch, int adjacent) ;
void initializeCache(char* inCacheSize, char* inAssociativity, char* inCachePolicy, char* inBytesPerBlock, char* inTraceFileName);
