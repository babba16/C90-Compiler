#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_expression.hpp"
#include "ast/ast_primitives.hpp"
#include "ast/ast_operators.hpp"
#include "ast/ast_node.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_parameter.hpp"
#include "ast/ast_context2.hpp"
#include "ast/ast_program.hpp"
#include "ast/ast_unary.hpp"

extern const Ast_Node *parseAST();

#endif
