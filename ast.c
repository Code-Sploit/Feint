#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

void AllocateTree(AST_T *tree)
{
    tree->children = malloc(256 * sizeof(AST_T));
}

void AssignNodeToTree(AST_T *tree, AST_T *node)
{
    tree->children[tree->_cc] = node;

    ++tree->_cc;
}

void CopyNode(AST_T *tree, AST_T *node)
{
    tree->_fname = node->_fname;
    tree->_vname = node->_vname;
    tree->_vvalue = node->_vvalue;
    tree->vtype = node->vtype;
    tree->type = node->type;
    tree->ltype = node->ltype;
    tree->_isstable = node->_isstable;
    tree->_branchc = node->_branchc;
    tree->_eob = node->_eob;
    tree->_cc = node->_cc;
}

AST_T *NewAST(int _type, int _eob)
{
    AST_T *ast_node = calloc(1, sizeof(AST_T));

    ast_node->type = _type;
    ast_node->_eob = _eob;
    ast_node->_cc  = 0;

    for (int i = 0; i < 256; i++)
    {
        ast_node->children[i] = calloc(1, sizeof(AST_T));
    }

    return ast_node;
}

AST_T *EmptyAST(void)
{
    AST_T *empty_ast = calloc(1, sizeof(AST_T));

    return empty_ast;
}

AST_T *GenerateFunction(char *_fname)
{
    AST_T *empty_ast = EmptyAST();

    empty_ast->ltype  = LEXER_TOKEN_ID;
    empty_ast->type   = AST_FUNCTION;
    empty_ast->_fname = _fname;

    return empty_ast;
}

AST_T *GenerateVariable(char *_vname, char *_vvalue, int _vtype)
{
    AST_T *empty_ast = EmptyAST();

    empty_ast->ltype  = LEXER_TOKEN_ID;
    empty_ast->type   = AST_DECLARATION;
    
    empty_ast->_vname  = _vname;
    empty_ast->_vvalue = _vvalue;
    empty_ast->vtype   = _vtype;

    return empty_ast;
}

AST_T *GenerateReturnStatement(char *_rvalue)
{
    AST_T *empty_ast = EmptyAST();

    empty_ast->ltype   = LEXER_TOKEN_ID;
    empty_ast->type    = AST_STATEMENT;
    
    empty_ast->_rvalue = _rvalue;

    return empty_ast;
}

AST_T *ASTParseFunction(Lexer_T *lexer)
{
    /* We got a function definition! */
    /* Next token should be the function name! */

    Token_Node *tok = LexerGetNextToken(lexer);

    /* Add a function declaration to the tree */

    AST_T *node = GenerateFunction(tok->_value);

    /* For now we can ignore the next 3 tokens because they are
    1. (
    2. )
    3. {
    */

    if (MatchExpr(tok->_value, "main"))
    {
        printf("Main function!\n");
    }

    /* TODO: Add function arguments */

    return node;
}

AST_T *ASTParseInteger(Lexer_T *lexer, int _isstable)
{
    /* Integer definition! */
    /* Next tokens should be (variable name), (operator), (value)! */

    Token_Node *tok_vname    = LexerGetNextToken(lexer);
    Token_Node *tok_operator = LexerGetNextToken(lexer);
    Token_Node *tok_vvalue   = LexerGetNextToken(lexer);

    AST_T *node = GenerateVariable(tok_vname->_value, tok_vvalue->_value, VARIABLE_TYPE_INT);

    /* Set _isstable flag for constants */

    node->_isstable = _isstable;

    /* Set global 'tok' to last next token */

    printf("Variable type: (%s) oper: (%s) stable: (%d) value: (%s)\n", tok_vname->_value, tok_operator->_value, _isstable, tok_vvalue->_value);

    return node;
}

AST_T *ASTParseString(Lexer_T *lexer, int _isstable)
{
    /* Integer definition! */
    /* Next tokens should be (variable name), (operator), (value)! */

    Token_Node *tok_vname    = LexerGetNextToken(lexer);
    Token_Node *tok_operator = LexerGetNextToken(lexer);
    Token_Node *tok_vvalue   = LexerGetNextToken(lexer);

    AST_T *node = GenerateVariable(tok_vname->_value, tok_vvalue->_value, VARIABLE_TYPE_INT);
    
    /* Set _isstable flag for constants */

    node->_isstable = _isstable;

    /* Set global 'tok' to last next token */

    printf("Variable type: (%s) oper: (%s) stable: (%d) value: (%s)\n", tok_vname->_value, tok_operator->_value, _isstable, tok_vvalue->_value);

    return node;
}

AST_T *ASTParseReturnStatement(Lexer_T *lexer)
{
    /* Return statement! */
    /* Next token should be return value! */

    Token_Node *tok_retval = LexerGetNextToken(lexer);

    AST_T *node = GenerateReturnStatement(tok_retval->_value);

    printf("Return statement returns: (%s)\n", tok_retval->_value);

    return node;
}

AST_T *GenerateASTTree(char *fdata)
{
    Lexer_T *lexer  = InitializeLexer(fdata);
    Token_Node *tok = LexerGetNextToken(lexer);

    AST_T *tree = EmptyAST();

    AllocateTree(tree);

    while (tok->Type != TOKEN_EOF)
    {
        /* Start parsing the tokens */
        AST_T *node = EmptyAST();
        
        /* Is the token of type ID? */
        if (tok->Type == TOKEN_ID)
        {
            if (MatchExpr(tok->_value, SYNTAX_FUNCTION_DEFINITION))
            {
                AST_T *node = ASTParseFunction(lexer);

                tok = LexerSkipToken(lexer, 3);

                /* Assign to main tree node */

                CopyNode(tree, node);
            }

            /* Variable declaration? */
            if (MatchExpr(tok->_value, SYNTAX_INT_DEFINITION) || MatchExpr(tok->_value, SYNTAX_STR_DEFINITION))
            {
                /* We got a variable definition! */
                /* Let's find out if it's integer/string! */

                if (MatchExpr(tok->_value, SYNTAX_INT_DEFINITION))
                {
                    AST_T *node = ASTParseInteger(lexer, 0);

                    tok = LexerSkipToken(lexer, 1);

                    AssignNodeToTree(tree, node);
                }
                else
                {
                    AST_T *node = ASTParseString(lexer, 0);

                    tok = LexerSkipToken(lexer, 1);

                    AssignNodeToTree(tree, node);
                }
            }

            if (MatchExpr(tok->_value, SYNTAX_STABLE_DEFINITION))
            {
                /* Skip the 'stable' keyword */

                tok = LexerSkipToken(lexer, 1);

                /* We can now check the type of the variable */

                if (MatchExpr(tok->_value, SYNTAX_INT_DEFINITION))
                {
                    AST_T *node = ASTParseInteger(lexer, 1);

                    tok = LexerSkipToken(lexer, 1);

                    AssignNodeToTree(tree, node);
                }
                else
                {
                    AST_T *node = ASTParseString(lexer, 1);

                    tok = LexerSkipToken(lexer, 1);

                    AssignNodeToTree(tree, node);
                }
            }

            if (MatchExpr(tok->_value, SYNTAX_RETURN_STATEMENT))
            {
                AST_T *node = ASTParseReturnStatement(lexer);

                tok = LexerSkipToken(lexer, 1);

                AssignNodeToTree(tree, node);
            }

            /* Continue the loop and start from top */
            
            tok = LexerGetNextToken(lexer);

            continue;
        }

        tok = LexerGetNextToken(lexer);
    }

    return tree;
}
