/*
#include <iostream>
#include <list>
#include <stdio.h>
using namespace std;

class abstract_astnode
{
public:
    virtual void print () = 0;
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
    BlockAst(abstract_astnode* a);
    void add(abstract_astnode* a);
};


/////////////////////////////////////////////// Valar Doherias


class OpAst: public ExpAst{
    enum OPERATOR {
        OR = 1, AND, EQ_OP, NE_OP, LT, GT, LE_OP, GE_OP, PLUS, MINUS, MULT, ASSIGN 
    };
    std::string rank[12] = { "OR", "AND", "EQ_OP", "NE_OP", "LT", "GT", "LE_OP", "GE_OP", "PLUS", "MINUS", "MULT", "ASSIGN" };
    abstract_astnode* exp1;
    abstract_astnode* exp2;
    OPERATOR op;  

public:
    void print();
    OpAst(OPERATOR a, abstract_astnode* b, abstract_astnode* c);
};

class UnopAst: public ExpAst{
    enum OPERATOR {
        UMINUS=1, NOT, PP
    };
    std::string rank[3] = { "UMINUS", "NOT", "PP"};
    OPERATOR op;
    abstract_astnode* exp1;
public:
    UnopAst(OPERATOR a, abstract_astnode* b);
    void print();
};
    
class FloatConstAst: public ExpAst{
    float f;
public:
    FloatConstAst(float a);
    void print();
};

class IntConstAst: public ExpAst{
    int i;
public:
    IntConstAst(int a);
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

*/


///////////////////////////////// definitions

void ExpAst::print(){}
void StmtAst::print(){}

void ForAst::print () {
    std::cout << "(For ";
    exp1->print();
    std::cout << std::endl;
    exp2->print();
    std::cout << std::endl;
    exp3->print();
    std::cout << std::endl;
    stmt->print();
    std::cout << ")";
}

ForAst::ForAst(abstract_astnode *a, abstract_astnode *b, abstract_astnode* c, abstract_astnode* d) :
    exp1(a), exp2(b), exp3(c), stmt(d) {}

void WhileAst::print() {
    std::cout << "(While ";
    exp->print();
    std::cout << std::endl;
    stmt->print();
    std::cout << ")";
}

WhileAst::WhileAst(abstract_astnode* a, abstract_astnode* b) {
    exp = a;
    stmt = b;
}

void IfAst::print() {
    std::cout << "(If ";
    exp->print();
    std::cout << std::endl;
    stmt1->print();
    std::cout << std::endl;
    stmt2->print();
    std::cout << ")";
}   

IfAst::IfAst(abstract_astnode* a, abstract_astnode* b, abstract_astnode* c) {
    exp = a;
    stmt1 = b;
    stmt2 = c;
}

void ReturnAst::print() {
    std::cout << "(Return ";
    exp->print();
    //std::cout << std::endl;
    std::cout << ")";
}

ReturnAst::ReturnAst(abstract_astnode* a) {
    exp = a;
}

void AssAst::print() {
    std::cout << "(Assignment ";
    exp1->print();
    std::cout << " "; // std::endl;
    exp2->print();
    std::cout << ")";
}

AssAst::AssAst(abstract_astnode* a, abstract_astnode* b) {
    exp1 = a;
    exp2 = b;
}

void EmptyAst::print() {
    std::cout << "(Empty)";
}

EmptyAst::EmptyAst(){}

void BlockAst::print() {
    std::cout << "(Block [" ;
    list<abstract_astnode*>::iterator it;
    for(it = block.begin(); it!= block.end(); it++) {
        (*it)->print();
        //if(it != block.end()-1) std::cout << std::endl;
    }
    std::cout << "])" << std::endl;
}

BlockAst::BlockAst(abstract_astnode* a) {
    block.push_back(a);
}

void BlockAst::add(abstract_astnode* a) {
    block.push_back(a);
}




//////////////////////////////////////// Valar Morghulis!

void OpAst::print(){
        std::cout<<"( "<<rank[op-1]<<" ";
        exp1->print();
        std::cout<<" ";
        exp2->print();
        std::cout<<" ) ";
}

OpAst::OpAst(OPERATOR a, abstract_astnode* b, abstract_astnode* c){
    op=a;
    exp1=b;
    exp2=c;
}

UnopAst::UnopAst(OPERATOR a, abstract_astnode* b){
    op = a;
    exp1 = b;
}
             
void UnopAst::print(){
    std::cout<<"( "<<rank[op-1]<<" ";
    exp1->print();
    std::cout<<" ) \n";                 
}             

FloatConstAst::FloatConstAst(float a){
    f=a;
}

void FloatConstAst::print(){
    printf("(FLOATCONST %f)",f);
}

IntConstAst::IntConstAst(int a){
    i=a;
}

void IntConstAst::print(){
    printf("(INTCONST %i)",i);
}

StringConstAst::StringConstAst(std::string a){
    s=a;
}

void StringConstAst::print(){
    printf("(STRINGCONST %s)",s.c_str());
}

IdentifierAst::IdentifierAst(std::string a){
    s=a;
}

void IdentifierAst::print(){
    printf("(IDENTIFIER %s)",s.c_str());
}



//// end



int main() {}
#include <iostream>
#include <list>
#include <stdio.h>
using namespace std;

class abstract_astnode
{
public:
    virtual void print () = 0;
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
    BlockAst(abstract_astnode* a);
    void add(abstract_astnode* a);
};


/////////////////////////////////////////////// Valar Doherias


class OpAst: public ExpAst{
    enum OPERATOR {
        OR = 1, AND, EQ_OP, NE_OP, LT, GT, LE_OP, GE_OP, PLUS, MINUS, MULT, ASSIGN 
    };
    std::string rank[12] = { "OR", "AND", "EQ_OP", "NE_OP", "LT", "GT", "LE_OP", "GE_OP", "PLUS", "MINUS", "MULT", "ASSIGN" };
    abstract_astnode* exp1;
    abstract_astnode* exp2;
    OPERATOR op;  

public:
    void print();
    OpAst(OPERATOR a, abstract_astnode* b, abstract_astnode* c);
};

class UnopAst: public ExpAst{
    enum OPERATOR {
        UMINUS=1, NOT, PP
    };
    std::string rank[3] = { "UMINUS", "NOT", "PP"};
    OPERATOR op;
    abstract_astnode* exp1;
public:
    UnopAst(OPERATOR a, abstract_astnode* b);
    void print();
};
    
class FloatConstAst: public ExpAst{
    float f;
public:
    FloatConstAst(float a);
    void print();
};

class IntConstAst: public ExpAst{
    int i;
public:
    IntConstAst(int a);
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


///////////////////////////////// definitions

void ExpAst::print(){}
void StmtAst::print(){}

void ForAst::print () {
    std::cout << "(For ";
    exp1->print();
    std::cout << std::endl;
    exp2->print();
    std::cout << std::endl;
    exp3->print();
    std::cout << std::endl;
    stmt->print();
    std::cout << ")";
}

ForAst::ForAst(abstract_astnode *a, abstract_astnode *b, abstract_astnode* c, abstract_astnode* d) :
    exp1(a), exp2(b), exp3(c), stmt(d) {}

void WhileAst::print() {
    std::cout << "(While ";
    exp->print();
    std::cout << std::endl;
    stmt->print();
    std::cout << ")";
}

WhileAst::WhileAst(abstract_astnode* a, abstract_astnode* b) {
    exp = a;
    stmt = b;
}

void IfAst::print() {
    std::cout << "(If ";
    exp->print();
    std::cout << std::endl;
    stmt1->print();
    std::cout << std::endl;
    stmt2->print();
    std::cout << ")";
}   

IfAst::IfAst(abstract_astnode* a, abstract_astnode* b, abstract_astnode* c) {
    exp = a;
    stmt1 = b;
    stmt2 = c;
}

void ReturnAst::print() {
    std::cout << "(Return ";
    exp->print();
    //std::cout << std::endl;
    std::cout << ")";
}

ReturnAst::ReturnAst(abstract_astnode* a) {
    exp = a;
}

void AssAst::print() {
    std::cout << "(Assignment ";
    exp1->print();
    std::cout << " "; // std::endl;
    exp2->print();
    std::cout << ")";
}

AssAst::AssAst(abstract_astnode* a, abstract_astnode* b) {
    exp1 = a;
    exp2 = b;
}

void EmptyAst::print() {
    std::cout << "(Empty)";
}

EmptyAst::EmptyAst(){}

void BlockAst::print() {
    std::cout << "(Block [" ;
    list<abstract_astnode*>::iterator it;
    for(it = block.begin(); it!= block.end(); it++) {
        (*it)->print();
        //if(it != block.end()-1) std::cout << std::endl;
    }
    std::cout << "])" << std::endl;
}

BlockAst::BlockAst(abstract_astnode* a) {
    block.push_back(a);
}

void BlockAst::add(abstract_astnode* a) {
    block.push_back(a);
}




//////////////////////////////////////// Valar Morghulis!

void OpAst::print(){
        std::cout<<"( "<<rank[op-1]<<" ";
        exp1->print();
        std::cout<<" ";
        exp2->print();
        std::cout<<" ) ";
}

OpAst::OpAst(OPERATOR a, abstract_astnode* b, abstract_astnode* c){
    op=a;
    exp1=b;
    exp2=c;
}

UnopAst::UnopAst(OPERATOR a, abstract_astnode* b){
    op = a;
    exp1 = b;
}
             
void UnopAst::print(){
    std::cout<<"( "<<rank[op-1]<<" ";
    exp1->print();
    std::cout<<" ) \n";                 
}             

FloatConstAst::FloatConstAst(float a){
    f=a;
}

void FloatConstAst::print(){
    printf("(FLOATCONST %f)",f);
}

IntConstAst::IntConstAst(int a){
    i=a;
}

void IntConstAst::print(){
    printf("(INTCONST %i)",i);
}

StringConstAst::StringConstAst(std::string a){
    s=a;
}

void StringConstAst::print(){
    printf("(STRINGCONST %s)",s.c_str());
}

IdentifierAst::IdentifierAst(std::string a){
    s=a;
}

void IdentifierAst::print(){
    printf("(IDENTIFIER %s)",s.c_str());
}



//// end
