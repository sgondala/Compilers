#include <iostream>
#include <list>
#include <stdio.h>
using namespace std;

class abstract_astnode
{
public:
    virtual void print ()=0;
};


class ExpAst : public abstract_astnode {
public:
    void print ();
};


class StmtAst : public abstract_astnode {
public:
    void print ();
};


class ForAst: public StmtAst {
private:
    abstract_astnode* exp1;
    abstract_astnode* exp2;
    abstract_astnode* exp3;
    abstract_astnode* stmt;
public:
    void print ();
    ForAst(abstract_astnode *a, abstract_astnode *b, abstract_astnode* c, abstract_astnode* d);
};

class WhileAst: public StmtAst
{
private:
    abstract_astnode* exp;
    abstract_astnode* stmt;

public:
    void print();
    WhileAst(abstract_astnode *a, abstract_astnode *b);
};

class IfAst: public StmtAst 
{
private:
    abstract_astnode* exp;
    abstract_astnode* stmt1;
    abstract_astnode* stmt2;

public:
    void print();
    IfAst(abstract_astnode *a, abstract_astnode *b, abstract_astnode *c);
};

class ReturnAst: public StmtAst
{
private:
    abstract_astnode* exp;

public:
    void print();
    ReturnAst(abstract_astnode* a);
};

class AssAst: public StmtAst
{
private:
    abstract_astnode* exp1;
    abstract_astnode* exp2;

public:
    void print();
    AssAst(abstract_astnode* a, abstract_astnode* b);
};

class EmptyAst: public StmtAst
{
public:
    void print();
    EmptyAst();
};


class BlockAst: public abstract_astnode 
{
private:
    list<abstract_astnode*> block;

public:
    void print();
    BlockAst(list<abstract_astnode*> a);///////////////changed
    void add(abstract_astnode* a);
};


/////////////////////////////////////////////// Valar Doherias


class OpAst: public ExpAst{
    //std::string rank[12] = { "OR", "AND", "EQ_OP", "NE_OP", "LT", "GT", "LE_OP", "GE_OP", "PLUS", "MINUS", "MULT", "ASSIGN" };
    abstract_astnode* exp1;
    abstract_astnode* exp2;
    string s;  

public:
    void print();
    OpAst(std::string a, abstract_astnode* b, abstract_astnode* c);
};

class UnopAst: public ExpAst{
    /*
    enum OPERATOR {
        UMINUS=1, NOT, PP
    };
    std::string rank[3] = { "UMINUS", "NOT", "PP"};
    */
    string s;
    abstract_astnode* exp1;
public:
    UnopAst(string a, abstract_astnode* b);
    void print();
};

class FuncCallAst: public ExpAst {
    string s;
    list<abstract_astnode*> exp;

public:
    FuncCallAst(string a, list<abstract_astnode*> l);
    void print();
};

class FloatConstAst: public ExpAst{
    string s;
public:
    FloatConstAst(string a);
    void print();
};

class IntConstAst: public ExpAst{
    string s;
public:
    IntConstAst(string a);
    void print();
};

class StringConstAst: public ExpAst{
    std::string s;
public:
    StringConstAst(std::string a);
    void print();
};

class IdentifierAst: public ExpAst{
    std::string s;
public:
    void print();
    IdentifierAst(std::string a);
};

    
class ArrayRefAst:public ExpAst{
public:
    void print();
};


class IndexAst: public ArrayRefAst{
    abstract_astnode* exp1;
    abstract_astnode* exp2;
public:
    void print();
    IndexAst(abstract_astnode* a, abstract_astnode* b);
};
