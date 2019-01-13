enum Bool{FALSE, TRUE};

typedef struct variableNode {
	char name[100];	
	int index;
	int	value;
	struct variableNode* next;
} variableNode;

typedef struct gateNode {
	char type[100];
	int	numInputs;
	int index;
	int* varIndices;
	struct gateNode* next;
} gateNode;

int numInputs;
int numOutputs;
int numVariables = 0;
int numGates = 0;
variableNode* variableList = NULL;
gateNode* gateList = NULL;
variableNode* trueVar;
variableNode* falseVar;

void readCircuit(char* fileName) ;
void initInputs(int sequenceNum);
void executeCircuit();
variableNode* getVariableByName(char name[100]);
variableNode* getVariable(int index);
gateNode* getGate(int index);
int logTwo(int input) ;
int toThePower(int x, int n) ;
void initConstVars();
