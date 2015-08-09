#include <iostream>
#include <bits/stdc++.h>
#include <list>
#include <stdio.h>
#include <map>
using namespace std;

extern int lineno;
extern ofstream outFile;

class lstEntry
{
public:
    string name;
    string type;
    bool isParam; //1 for parameter, 0 for localVar
    int size;
    int offset;
    lstEntry(string a, string t, bool b, int c, int d);
    lstEntry();    
};

class lst
{
//private:
public:
    map<string, lstEntry*> entryMap;
    //gst* parentLst;
    int offsetParam;
    int offsetVar;
    lst();
    bool lookup(string s);// True if already exists. check if variable with same string name exists
    bool insert(string s, lstEntry* row);// returns false if elt. already present
    void clear();
    void print();
    //void insertLst(lst* a){
    int getOffset(string s);    
};

class gst
{
public:
    std::map<string, lst*> functionTables;
    std::map<string, vector<string> > functionNames;
    std::map<string, string> functionReturnVal;

    gst();
    bool insert(string s, lst* row);
    bool insertFunctionNames(string s, vector<string> v); 
    void print();
};


//////////////////////////// Lab 3 code -- generating AST's

class abstract_astnode
{
public:
    virtual void print ()=0;
    virtual void genCode()=0;
    virtual string getString()=0;
    string astType;
    int label;
};


class ExpAst : public abstract_astnode {
public:
    void genCode();    
    void print ();
    string getString(){};
};


class StmtAst : public abstract_astnode {
public:
    void genCode();    
    void print ();
    string getString(){};
};


class ForAst: public StmtAst {
private:
    abstract_astnode* exp1;
    abstract_astnode* exp2;
    abstract_astnode* exp3;
    abstract_astnode* stmt;
public:
    void print ();
    void genCode();
    ForAst(abstract_astnode *a, abstract_astnode *b, abstract_astnode* c, abstract_astnode* d);
    
};

class WhileAst: public StmtAst
{
private:
    abstract_astnode* exp;
    abstract_astnode* stmt;

public:
    void print();
    void genCode();
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
    void genCode();
    IfAst(abstract_astnode *a, abstract_astnode *b, abstract_astnode *c);
};

class ReturnAst: public StmtAst
{
private:
    abstract_astnode* exp;

public:
    void print();
    void genCode();
    ReturnAst(abstract_astnode* a);
};

class AssAst: public StmtAst
{
private:
    abstract_astnode* exp1;
    abstract_astnode* exp2;

public:
    void print();
    void genCode();
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
    void genCode();
    string getString(){};
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
    void genCode();
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
    void genCode();
    string getString();
};

class FuncCallAst: public ExpAst {
    string s;
    list<abstract_astnode*> exp;

public:
    FuncCallAst(string a, list<abstract_astnode*> l);
    void genCode();
    void print();
};

class FloatConstAst: public ExpAst{
    string s;
public:
    FloatConstAst(string a);
    void print();
    float getFloat();
    void genCode();
    string getString();
};

class IntConstAst: public ExpAst{
    string s;
public:
    int getInt();
    string getString();
    IntConstAst(string a);
    void genCode();
    void print();
};

class StringConstAst: public ExpAst{
    std::string s;
public:
    StringConstAst(std::string a);
    void print();
    string getString();
};

class IdentifierAst: public ExpAst{
    std::string s;
public:
    void print();
    void genCode();
    string getString();
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
    string getString();
    void genCode();
    IndexAst(abstract_astnode* a, abstract_astnode* b);
};
