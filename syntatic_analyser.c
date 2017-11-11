#include <stdio.h>
#include <string.h>
#include "syntatic_analyser.h"

void compile(FILE *sourceFile) {
    InputStr str;
    TokenArr tokenArr;

    /* Carregar o arquivo */
    char c;
    str->pos = 0;

    do {
        c = fgetc(sourceFile);
        str->value[str->pos] = c;
        str->pos++;
    } while (c != EOF);

    /* Analise sintatica */

    if (getTokens(&tokenArr, &str) == 1 && checkSyntax(&tokenArr) == 1) {
        printf("Sintaxe OK\n");
    } else {
        printf("Erro de sintaxe\n");
    }
}

int getTokens(TokenArr *tokenArr, InputStr *str) {
    tokenArr->size = 0;

    str->pos = 0;

    while (1) {
        Token *t = (&tokenArr->tokens[tokenArr->size]);

        int success = getNextToken(t, str);

        if (success) {
            tokenArr->size++;
            printTokenType(t->type);
            printf("\t %s \n", t->value);
        } else if(str->value[str->pos] == EOF){
            printf("%d tokens extraidos.\n", tokenArr->size);
            return 1;
        } else {
            printf("Caracter invalido '%c' na posicao%d.\n", str->value[str->pos], str->pos);
            return 0;
        }
    }
}

int checkSyntax(TokenArr *tokenArr) {
    tokenArr->pos = 0;
    return acceptProgram(tokenArr);
}

int acceptProgram(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "program") == 0) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (recognizeDeclarations(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (recognizeCommands(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "endprogram") == 0) {
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                printf("Programa reconhecido");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeDeclaration(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (recognizeDeclareFunction(tokenArr) || recognizeDeclareVariable(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (recognizeDeclareFunction(tokenArr) || recognizeDeclareVariable(tokenArr)) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    printf("Declaracao reconhecida");
                    syntaxMatch = 1;
                }
            break;
            case 2:
                printf("Declaracao reconhecida");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeDeclareFunction(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "def") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_IDENTIFIER) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (recognizeIdentificators(tokenArr)) { //CHECK
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (recognizeCommands(tokenArr)) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "return") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 5:
                if (recognizeExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 6:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "enddef") == 0) {
                    tokenArr->pos++;
                    state = 7;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 7:
                if (recognizeDeclareFunction(tokenArr)) {
                    tokenArr->pos++;
                    state = 8;
                } else {
                    printf("Funcao reconhecida");
                    syntaxMatch = 1;
                }
            break;
            case 8:
                printf("Funcao reconhecida");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeDeclareVariable(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "var") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_DATATYPE) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (t.type == TYPE_IDENTIFIER) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (recognizeDeclareVariable(tokenArr)) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    printf("Variavel reconhecida");
                    syntaxMatch = 1;
                }
            break;
            case 4:
                printf("Variavel reconhecida");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeCommands(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (recognizeCommand(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_SEPARATOR &&  strcmp(t.value, ",") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    printf("Comandos reconhecidos");
                    syntaxMatch = 1;
                }
            break;
            case 2:
                if (recognizeCommands(tokenArr)) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                printf("Comandos reconhecidos");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeCommand(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (recognizeDeclareVariable(tokenArr) || recognizeAttribution(tokenArr) || recognizeCondicional(tokenArr) || recognizeLoops(tokenArr) || recognizeFunctionCall(tokenArr) || recognizeInput(tokenArr) || recognizePrint(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                printf("Comando reconhecido");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeExpression(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (recognizeArithExpression(tokenArr) || recognizeBoolExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                printf("Expressao reconhecida");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeBoolExpression(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_DATATYPE && strcmp(t.value, "boolean") == 0) { //identify type here (int, float... -> lexic)
                    tokenArr->pos++;
                    state = 1;
                } else if (recognizeBoolExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 2;
                } else if (recognizeArithExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                printf("Expressao booleana reconhecida");
                syntaxMatch = 1;
            break;
            case 2:
                if (t.type == TYPE_OPERATOR && (strcmp(t.value, "&&") == 0 || strcmp(t.value, "||") == 0)) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (t.type == TYPE_OPERATOR && (strcmp(t.value, ">") == 0 || strcmp(t.value, ">=") == 0 || strcmp(t.value, "<") == 0 || strcmp(t.value, "<=") == 0 || strcmp(t.value, "==") == 0 || strcmp(t.value, "!=") == 0)) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                if (recognizeArithExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 5:
                if (recognizeBoolExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeArithExpression(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_DATATYPE) {
                    tokenArr->pos++;
                    state = 1;
                } else if(recognizeArithExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                printf("Expressao aritmetica reconhecida");
                syntaxMatch = 1;
            break;
            case 2:
                if (t.type == TYPE_OPERATOR) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if(recognizeArithExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int recognizeAttribution (TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == IDENTIFIER) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_OPERATOR && strcmp(t.value, "=") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (t.type == TYPE_DATATYPE || recognizeFunctionCall(tokenArr)) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                printf("Atribuicao reconhecida");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }
}

int recognizeCondicional (TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "if") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (recognizeExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (t.type == TYPE_KEYWORD &&  strcmp(t.value, "then") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (recognizeCommands(tokenArr)) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                if (t.type == TYPE_KEYWORD &&  strcmp(t.value, "end") ==0) {
                    tokenArr->pos++;
                    state = 5;
                } else if(recognizeAdditionalCondition(tokenArr)) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 5:
                if (t.type == TYPE_KEYWORD &&  strcmp(t.value, "if") ==0) {
                    tokenArr->pos++;
                    state = 7;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 6:
                if (t.type == TYPE_KEYWORD &&  strcmp(t.value, "end") ==0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 7:
                printf("Condicional reconhecido");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }
}

int recognizeLoops (TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "for") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else if (t.type == TYPE_KEYWORD && strcmp(t.value, "while") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (recognizeAttribution(tokenArr)) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                if (recognizeExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 5:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 7;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 6:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 8;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 7:
                if (recognizeExpression(tokenArr)) {
                    tokenArr->pos++;
                    state = 9;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 8:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "loop") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 9:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 11;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 10:
                if (recognizeCommands(tokenArr)) {
                    tokenArr->pos++;
                    state = 12;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 11:
                if (recognizeAttribution(tokenArr)) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 12:
                printf("Loop reconhecido");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }
}

int recognizeFunctionCall (TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "invoke") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_IDENTIFIER) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (recognizeArgument(tokenArr)) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                printf("Chamada de funcao reconhecida");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }
}

int recognizeInput (TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "input") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (t.type == TYPE_IDENTIFIER) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                printf("Input reconhecido");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }
    }
}

int recognizePrint (TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "print") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (t.type == TYPE_IDENTIFIER) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else if (t.type == TYPE_SEPARATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                printf("Input reconhecido");
                syntaxMatch = 1;
            break;
            case 5:
                if (recognizeArgument(tokenArr)) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 6:
                if (t.type == TYPE_SEPARATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }
    }
}

int recognizeAdditionalCondition(TokenArray *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "else") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "if") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else if (recognizeCommands(tokenArray)) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 2:
                if (recognizeExpression(tokenArray)) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 3:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "end") == 0 ) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 4:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "then") == 0 ) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 5:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "if") == 0 ) {
                    tokenArr->pos++;
                    state = 7;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 6:
                if (recognizeCommands(tokenArray)) {
                    tokenArr->pos++;
                    state = 8;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 7:
                printf("Condicao Adicional reconhecida");
                syntaxMatch = 1;
            break;
            case 8:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "end") == 0 ) {
                    tokenArr->pos++;
                    state = 5;
                } else if(recognizeAdditionalCondition(tokenArray)){
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }
    }
}

int recognizeArgument(TokenArray *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_IDENTIFIER || t.type == TYPE_DATATYPE) {
                    tokenArr->pos++;
                    state = 1;
                } else if (recognizeArgument(tokenArr)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;
            case 1:
                printf("Argumento reconhecido");
                syntaxMatch = 1;
            break;
            case 2:
                if (recognizeArgument(tokenArray)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;


            default:
                syntaxMatch = 0;
        }
    }
}
