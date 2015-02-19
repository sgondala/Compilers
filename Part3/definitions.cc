#include <iostream>
#include <list>
#include <stdio.h>
using namespace std;

void abstract_astnode::print(){}
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
    std::cout << "(Assign_exp ";
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
         std::cout << std::endl;
        //if(it != block.end()-1) std::cout << std::endl;
    }
    std::cout << "])" << std::endl;
}

BlockAst::BlockAst(std::list<abstract_astnode*> a) {////////changed
    block=a;
}

void BlockAst::add(abstract_astnode* a) {
    block.push_back(a);
}




//////////////////////////////////////// Valar Morghulis!

void OpAst::print(){
    std::cout<<"( "<<s<<" ";
        exp1->print();
        std::cout<<" ";
        exp2->print();
        std::cout<<" ) ";
}

OpAst::OpAst(string a, abstract_astnode* b, abstract_astnode* c){
    s=a;
    exp1=b;
    exp2=c;
}

UnopAst::UnopAst(string a, abstract_astnode* b){
    s = a;
    exp1 = b;
}
             
void UnopAst::print(){
    std::cout<<"( "<<s<<" ";
    exp1->print();
    std::cout<<" ) \n";                 
}             

FuncCallAst::FuncCallAst(string a, list<abstract_astnode*> l) {
	s=a;
	exp = l;
}

void FuncCallAst::print() {
	std::cout<<"Function call( " << s << " ";
	for(list<abstract_astnode*>::iterator it = exp.begin(); it!=exp.end(); it++) {
		(*it)->print();
		//std::cout << std::endl;
	}
	std::cout << " )" << std::endl;
}

FloatConstAst::FloatConstAst(string a){
    s=a;
}

void FloatConstAst::print(){
    cout<<"(FLOATCONST "<<s<<")";
}

IntConstAst::IntConstAst(string a){
    s=a;
}

void IntConstAst::print(){
    cout<<"(INTCONST "<<s<<")";
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


//Newly added
void ArrayRefAst::print(){}


void IndexAst::print(){
    cout<<"( Array ( ";
    exp1->print();
    cout<<" ";
    exp2->print();
    cout<<") )\n";
}

IndexAst::IndexAst(abstract_astnode* a, abstract_astnode* b){
    exp1=a;
    exp2=b;
}


//// end



//int main() {}



//L
