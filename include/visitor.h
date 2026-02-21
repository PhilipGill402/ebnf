#ifndef INCLUDE_VISITOR_H_
#define INCLUDE_VISITOR_H_

#include "ast.h"

struct Visitor {
    Visitor() = default; 
    virtual ~Visitor() = default;
    
    virtual void visit(Syntax &n) = 0;
    virtual void visit(Rule &n) = 0;
    virtual void visit(Expr &n) = 0;
    virtual void visit(Sequence &n) = 0;
    virtual void visit(Terminal& n) = 0;
    virtual void visit(Nonterminal& n) = 0;
    virtual void visit(Optional& n) = 0;
    virtual void visit(Repeated& n) = 0;
    virtual void visit(Grouped& n) = 0;
    virtual void visit(Empty& n) = 0;
};

#endif // !INCLUDE_VISITOR_H_
