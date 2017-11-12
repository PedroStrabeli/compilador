typedef struct _tokenArr {
    Token tokens[4096];
    int size;
    int pos;
} TokenArr;

void compile(FILE *sourceFile);
int getTokens(TokenArr *tokenArr, InputStr *str);
int checkSyntax(TokenArr *tokenArr);
int acceptProgram(TokenArr *tokenArr);
int recognizeDeclaration(TokenArr *tokenArr);
int recognizeDeclareFunction(TokenArr *tokenArr);
int recognizeDeclareVariable(TokenArr *tokenArr);
int recognizeCommands(TokenArr *tokenArr);
int recognizeCommand(TokenArr *tokenArr);
int recognizeExpression(TokenArr *tokenArr);
int recognizeBoolExpression(TokenArr *tokenArr) { //TO;
int recognizeArithExpression(TokenArr *tokenArr) { //TO;
int recognizeAttribution (TokenArr *tokenArr);
int recognizeCondicional (TokenArr *tokenArr);
int recognizeLoops (TokenArr *tokenArr);
int recognizeFunctionCall (TokenArr *tokenArr);
int recognizeInput (TokenArr *tokenArr);
int recognizePrint (TokenArr *tokenArr);
int recognizeAdditionalCondition(TokenArr *tokenArr);
int recognizeArgument(TokenArray *tokenArr);
