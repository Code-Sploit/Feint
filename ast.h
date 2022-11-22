#include "parser.h"

void ASTGenerateMachineCode(Scope_T *_Scope, int _debug);

void WriteASMTo(char *_out, char *_start_function_data, char *_declarations, char **_functions, int _F_Count);
