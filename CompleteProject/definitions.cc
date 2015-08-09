#include <iostream>
#include <list>
#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

string name;
gst* globalTable = new gst(); 
lst* currentTable = new lst();
string type, lexp, typeAll;
int width, widthAll;
string funcName, functionReturnVal, indent="";
int intConst;

vector<string> parameterTypes;
string returnType;
bool firstType = true;
int maxRegs = 6;

list<string> rstack = {"eax", "ebx", "ecx", "edx", "edi", "esi"};
list<string> rstackOrig = {"eax", "ebx", "ecx", "edx", "edi", "esi"};
int labelCount = 0;

string trueLabel = "true";
string falseLabel = "false";


string genNewLabel() {
    return "l"+to_string(labelCount++);
}


lstEntry::lstEntry(string a, string t, bool b, int c, int d) {
    this->name = a;
    this->isParam = b;
    this->size = c;
    this->offset = d;
    type = t;
}

lstEntry::lstEntry(){}

lst::lst(){
    //parentLst = globalTable;
    offsetParam = 4;
    offsetVar = 0;
}

bool lst::lookup(string s){
    if(entryMap.find(s)==entryMap.end()) return false;
    else return true;
}

int lst::getOffset(string s){ //Already assumes that it is present/ error is generated somewhere
    if(find(s.begin(),s.end(),'|')!=s.end()){
        string name; 
        int returnVal = 0; 
        int pos = find(s.begin(), s.end(), '|') - s.begin();
        name = s.substr(0,pos); //name - a
        s = s.substr(pos+1); // s - 1|2
        int offsetBase = entryMap[name]->offset;
        string type = entryMap[name]->type; // type - array(2,array(3,int))
        pos = find(type.begin(), type.end(), ',') - type.begin();
        type = type.substr(pos+1); //type = array(3,int);

        while(find(s.begin(), s.end(), '|')!=s.end()){
            pos = find(s.begin(), s.end(), '|') - s.begin();
            string temp = s.substr(0,pos);
            returnVal += stoi(temp);
            s = s.substr(pos+1);
            type = type.substr(6); //array(4)
            pos = find(type.begin(), type.end(), ',') - type.begin();
            returnVal *=  stoi(type.substr(0,pos));
            type = type.substr(pos+1);
        }

            
        if(type.substr(0,3)=="int"){ returnVal+=stoi(s); 
            returnVal*=4; 
            // cout<<"Offset return val = "<<returnVal<<endl; 
            return offsetBase + returnVal;
        }
        if(type.substr(0,5)=="float"){ returnVal+=stoi(s); returnVal*=4; return offsetBase + returnVal;}
    }

    else return entryMap[s]->offset;
}

bool lst::insert(string s, lstEntry* row){
    if(entryMap.find(s)==entryMap.end()) {entryMap[s] = row; return true;}
    else return false;
}

void lst::clear() {
    entryMap.clear();
    offsetParam = 0;
    offsetVar = 0;
    //parentLst  = NULL;
}

void lst::print() {
    auto it1 = entryMap.begin();
    for(;it1!=entryMap.end();it1++){
        auto it = it1->second;
        cout<< (it)->name <<" "<<(it)->type<< " "<<(it)->size<< " "<<(it)->offset<< " "<<endl;
    }
}

gst::gst(){}

bool gst::insert(string s, lst* row){
    if(functionTables.find(s)==functionTables.end()) {functionTables[s] = row; return true;}
    else return false;
}

void gst::print() {
    cout << "All symbol table till now " << endl;
    auto it = functionTables.begin();
    for(;it!=functionTables.end();it++) {
        cout << it->first << endl;
        it->second->print();
    }
}

bool gst::insertFunctionNames(string s, vector<string> v){ //Allowing nly 1 
    if(functionNames.find(s)==functionNames.end()) 
        {
            functionNames[s] = v;
            return true;
        }
    else return false;
}
//////////////////////////// Lab 3 code -- generating AST's


void abstract_astnode::print(){}
void ExpAst::print(){}
void ExpAst::genCode(){}
void StmtAst::print(){}
void StmtAst::genCode(){}

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

void ForAst::genCode() { // TODO: test it
    outFile<<indent<<"//For Code starts \n";
    exp1->genCode();

    string label1 = genNewLabel();
    string label2 = genNewLabel();
    string label3 = genNewLabel();
    outFile<<label1<<":\n";
    trueLabel = label2;
    falseLabel = label3;
    exp2->genCode();

    outFile<<label2<<":\n";
    stmt->genCode();
    exp3->genCode();
    outFile<<indent<<"j("<<label1<<");\n";

    outFile<<label3<<":\n";
}

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

void WhileAst::genCode() {
    outFile<<indent<<"//While Code starts \n";
    
    string label1 = genNewLabel();
    string label2 = genNewLabel();
    string label3 = genNewLabel();
    trueLabel = label2;
    falseLabel = label3;
    
    outFile<<label1<<":\n";
    exp->genCode();

    outFile<<label2<<":\n";
    stmt->genCode();
    outFile<<indent<<"j("<<label1<<");\n";
    
    outFile<<label3<<":\n";
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

void IfAst::genCode() {
    outFile<<indent<<"//If Code starts \n";
    
    string label1 = genNewLabel();
    string label2 = genNewLabel();
    string label3 = genNewLabel();
    trueLabel = label1;
    falseLabel = label2;

    exp->genCode();

    outFile<<label1<<":\n";
    stmt1->genCode();
    outFile<<indent<<"j("<<label3<<");\n";

    outFile<<label2<<":\n";
    stmt2->genCode();

    outFile<<"\n";
    outFile<<label3<<":\n";
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

void ReturnAst::genCode() {
    exp->genCode();

    if(returnType == "int") {
        outFile<<indent<<"storei("<<rstack.front()<<", ind(ebp, "<<currentTable->offsetParam
            << "));\n";
    }
    else if(returnType == "float") {
        outFile<<indent<<"storef("<<rstack.front()<<", ind(ebp, "<<currentTable->offsetParam
            << "));\n";
    }
    // if(funcName != "main")
    outFile<<indent<<"j(e);\n";
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
    astType = a->astType;
    label = max(1, exp2->label);
}

void AssAst::genCode() {
    exp2->genCode();
    string lName = exp1->getString();
    // cout << "Check" << exp1->astType << endl;

    int offset = currentTable->getOffset(lName);
    if(exp1->astType == "int") 
        outFile<<indent<< "storei("<<rstack.front()<<", ind(ebp,"<<offset<<")); \n";
    if(exp1->astType == "float")
        outFile<<indent<< "storef("<<rstack.front()<<", ind(ebp,"<<offset<<")); \n";
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

void BlockAst::genCode() {
    list<abstract_astnode*>::iterator it;
    for(it = block.begin(); it!= block.end(); it++) {
        (*it)->genCode();
    }
}

void OpAst::print(){
    std::cout<<"( "<<s<<" ";
        exp1->print();
        std::cout<<" ";
        exp2->print();
        std::cout<<" ) ";
    //cout<<label<<endl;
}

OpAst::OpAst(string a, abstract_astnode* b, abstract_astnode* c){
    s=a;
    exp1=b;
    exp2=c;

    if(b->astType == "int" && c->astType == "int") {
        astType = "int";
    }
    else if(b->astType == "float" && c->astType == "float") {
        astType = "float";
    }
    else if(b->astType == "string" || c->astType == "string") {
        astType = "error";
    }
    else {
        astType = "mixed";
    }

    if((a=="DIV_FLOAT" || a=="DIV_INT" || a=="LT_INT" || a=="GT_INT" 
        || a=="LT_FLOAT" || a=="GT_FLOAT") 
        && (b->label!=0 && c->label==0)){ //Should ad more
        label = min(2,b->label);
    }

    else if(b->label == c->label){
        label = b->label + 1;
    }
    else {
        label = max(b->label, c->label);
    }
}

void OpAst::genCode() {
    //cout << "in code" << label << "\n";
    std::map<string, string> myMapComp, typeMapComp;
    myMapComp["GT_INT"] = "jg";
    myMapComp["LT_INT"] = "jl";
    myMapComp["GE_OP_INT"] = "jge";
    myMapComp["LE_OP_INT"] = "jle";
    myMapComp["GT_FLOAT"] = "jg";
    myMapComp["LT_FLOAT"] = "jl";
    myMapComp["GE_OP_FLOAT"] = "jge";
    myMapComp["LE_OP_FLOAT"] = "jle";
    myMapComp["EQ_OP_INT"] = "je";
    myMapComp["EQ_OP_FLOAT"] = "je";
    myMapComp["NE_OP_INT"] = "jne";
    myMapComp["NE_OP_FLOAT"] = "jne";

    typeMapComp["GT_INT"] = "cmpi";
    typeMapComp["LT_INT"] = "cmpi";
    typeMapComp["GE_OP_INT"] = "cmpi";
    typeMapComp["LE_OP_INT"] = "cmpi";
    typeMapComp["GT_FLOAT"] = "cmpf";
    typeMapComp["LT_FLOAT"] = "cmpf";
    typeMapComp["GE_OP_FLOAT"] = "cmpf";
    typeMapComp["LE_OP_FLOAT"] = "cmpf";
    typeMapComp["EQ_OP_FLOAT"] = "cmpf";
    typeMapComp["EQ_OP_INT"] = "cmpi";
    typeMapComp["NE_OP_INT"] = "cmpi";
    typeMapComp["NE_OP_FLOAT"] = "cmpf";

    if(s=="ASSIGNMENT") {
        exp2->genCode();
        string lName = exp1->getString();

        int offset = currentTable->getOffset(lName);
        if(exp1->astType == "int") 
            outFile<<indent<< "storei("<<rstack.front()<<", ind(ebp,"<<offset<<")); \n";
        if(exp1->astType == "float")
            outFile<<indent<< "storef("<<rstack.front()<<", ind(ebp,"<<offset<<")); \n";

        return;
    }

    int leftLabel = exp1->label;
    int rightLabel = exp2->label;
    
    if(leftLabel == 0 && rightLabel ==0){ //both are ints / floats
        if(s=="PLUS_INT"){
            outFile<<indent<<"move("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"addi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }    
        if(s=="SUB_INT"){
            outFile<<indent<<"move( -"<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"addi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        if(s=="PLUS_FLOAT"){
            outFile<<indent<<"move("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"addf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        if(s=="SUB_FLOAT"){
            outFile<<indent<<"move( -"<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"addf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        if(s=="MULT_INT"){
            outFile<<indent<<"move("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"muli("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        if(s=="MULT_FLOAT"){
            outFile<<indent<<"move("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"mulf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        if(s=="DIV_INT"){
            outFile<<indent<<"move("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"divi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        if(s=="DIV_FLOAT"){
            outFile<<indent<<"move("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            outFile<<indent<<"divf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }

        else {
            outFile<<indent<<"move("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            
            if(s=="LT_INT"){
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jl("<<trueLabel<<");"<<endl;
            }
            if(s=="LT_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jl("<<trueLabel<<");"<<endl;
            }
            if(s=="GT_INT"){
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jg("<<trueLabel<<");"<<endl;
            }
            if(s=="GT_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jg("<<trueLabel<<");"<<endl;
            }
            if(s=="LE_OP_INT"){
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jle("<<trueLabel<<");"<<endl;
            }
            if(s=="LE_OP_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jle("<<trueLabel<<");"<<endl;
            }
            if(s=="GE_OP_INT"){
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jge("<<trueLabel<<");"<<endl;
            }
            if(s=="GT_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jge("<<trueLabel<<");"<<endl;
            }

            if(s=="EQ_OP_INT" || s=="EQ_OP_FLOAT" || s=="NE_OP_INT" || s=="NE_OP_FLOAT"){
                outFile<<indent<<typeMapComp[s]<<"("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<myMapComp[s]<<"("<<trueLabel<<");"<<endl;   
            } 
            
            outFile<<indent<<"j("<<falseLabel<<");"<<endl;
        }   
    }

    else if(leftLabel!=0 && rightLabel==0){

        if(s=="DIV_FLOAT" || s=="DIV_INT" || s.substr(0,3)=="GT_" || s.substr(0,3)=="LT_" 
            || s.substr(0,3)=="GE_" || s.substr(0,3)=="LE_" || s.substr(0,3)=="EQ_" || s.substr(0,3)=="NE_" ){
            
            string popedReg = rstack.front();
            rstack.pop_front();
            exp1->genCode();

            if(s=="DIV_INT"){
                outFile<<indent<<"move("<<exp2->getString()<<", "<<popedReg<<"); \n";
                outFile<<indent<<"divi("<<rstack.front()<<", "<<popedReg<<"); \n";
            }
            else if(s=="DIV_FLOAT"){
                outFile<<indent<<"move("<<exp2->getString()<<", "<<popedReg<<"); \n";
                outFile<<indent<<"divf("<<rstack.front()<<", "<<popedReg<<"); \n";
            }

            else {
                outFile<<indent<<"move("<<exp2->getString()<<", "<<popedReg<<");\n"; //Second in register
                if(s=="GT_INT"){ //Not taking care of multiple >, < or anything
                    outFile<<indent<<"cmpi("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jg("<<trueLabel<<");"<<endl;
                }
                if(s=="GT_FLOAT"){
                    outFile<<indent<<"cmpf("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jg("<<trueLabel<<");"<<endl;
                }
                if(s=="LT_INT"){ //Not taking care of multiple >, < or anything
                    outFile<<indent<<"cmpi("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jl("<<trueLabel<<");"<<endl;
                }
                if(s=="LT_FLOAT"){
                    outFile<<indent<<"cmpf("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jl("<<trueLabel<<");"<<endl;
                }
                if(s=="GE_OP_INT"){ //Not taking care of multiple >, < or anything
                    outFile<<indent<<"cmpi("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jge("<<trueLabel<<");"<<endl;
                }
                if(s=="GE_OP_FLOAT"){
                    outFile<<indent<<"cmpf("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jge("<<trueLabel<<");"<<endl;
                }
                if(s=="LE_OP_INT"){ //Not taking care of multiple >, < or anything
                    outFile<<indent<<"cmpi("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jle("<<trueLabel<<");"<<endl;
                }
                if(s=="LE_OP_FLOAT"){
                    outFile<<indent<<"cmpf("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<"jle("<<trueLabel<<");"<<endl;
                }
                if(s=="EQ_OP_INT" || s=="EQ_OP_FLOAT" || s=="NE_OP_INT" || s=="NE_OP_FLOAT"){
                    outFile<<indent<<typeMapComp[s]<<"("<<rstack.front()<<", "<<popedReg<<");\n"; //Second in register
                    outFile<<indent<<myMapComp[s]<<"("<<trueLabel<<");"<<endl;   
                } 
                outFile<<indent<<"j("<<falseLabel<<");"<<endl;
            }

            rstack.push_front(popedReg);
        }

        else{
            if(s=="PLUS_INT"){
                exp1->genCode();
                outFile<<indent<<"addi("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            }
            if(s=="PLUS_FLOAT"){
                exp1->genCode();
                outFile<<indent<<"addf("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            }
            if(s=="SUB_INT"){
                exp1->genCode();
                outFile<<indent<<"addi(-"<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            }
            if(s=="SUB_FLOAT"){
                exp1->genCode();
                outFile<<indent<<"addf(-"<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            }
            if(s=="MULT_INT"){
                exp1->genCode();
                outFile<<indent<<"muli("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            }
            if(s=="MULT_FLOAT"){
                exp1->genCode();
                outFile<<indent<<"mulf("<<exp2->getString()<<", "<<rstack.front()<<");\n"; //Second in register
            }    
        }
    }

    else if(leftLabel==0 && rightLabel!=0){
        if(s=="PLUS_INT"){
            exp2->genCode();
            outFile<<indent<<"addi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; 
        }
        else if(s=="PLUS_FLOAT"){
            exp2->genCode();
            outFile<<indent<<"addf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; 
        }
        else if(s=="SUB_INT"){
            exp2->genCode();
            outFile<<indent<<"muli(-1, "<<rstack.front()<<");\n"; 
            outFile<<indent<<"addi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; 
        }
        else if(s=="SUB_FLOAT"){
            exp2->genCode();
            outFile<<indent<<"muli(-1, "<<rstack.front()<<");\n"; 
            outFile<<indent<<"addf(-"<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        else if(s=="MULT_INT"){
            exp2->genCode();
            outFile<<indent<<"muli("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        else if(s=="MULT_FLOAT"){
            exp2->genCode();
            outFile<<indent<<"mulf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        else if(s=="DIV_INT"){
            exp2->genCode();
            outFile<<indent<<"divi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }
        else if(s=="DIV_FLOAT"){
            exp2->genCode();
            outFile<<indent<<"divf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
        }

        else {
            exp2->genCode();
            if(s=="GT_INT"){ //Not taking care of multiple >, < or anything
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jg("<<trueLabel<<");"<<endl;
            }
            if(s=="GT_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jg("<<trueLabel<<");"<<endl;
            }
            if(s=="LT_INT"){ //Not taking care of multiple >, < or anything
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jl("<<trueLabel<<");"<<endl;
            }
            if(s=="LT_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jl("<<trueLabel<<");"<<endl;
            }
            if(s=="GE_OP_INT"){ //Not taking care of multiple >, < or anything
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jge("<<trueLabel<<");"<<endl;
            }
            if(s=="GE_OP_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jge("<<trueLabel<<");"<<endl;
            }
            if(s=="LE_OP_INT"){ //Not taking care of multiple >, < or anything
                outFile<<indent<<"cmpi("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jle("<<trueLabel<<");"<<endl;
            }
            if(s=="LE_OP_FLOAT"){
                outFile<<indent<<"cmpf("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                outFile<<indent<<"jle("<<trueLabel<<");"<<endl;
            }
            if(s=="EQ_OP_INT" || s=="EQ_OP_FLOAT" || s=="NE_OP_INT" || s=="NE_OP_FLOAT"){
                    outFile<<indent<<typeMapComp[s]<<"("<<exp1->getString()<<", "<<rstack.front()<<");\n"; //Second in register
                    outFile<<indent<<myMapComp[s]<<"("<<trueLabel<<");"<<endl;   
            }

            outFile<<indent<<"j("<<falseLabel<<");"<<endl;
        }
    }

    else if(rightLabel < maxRegs && leftLabel >= rightLabel) {
        //outFile << "case 5\n"; 
        // code for left tree
        exp1->genCode();
        string left = rstack.front();
        rstack.pop_front();
        
        // code for right tree (rstack - R.....)
        exp2->genCode();
        string right = rstack.front();
        
        rstack.push_front(left);
        
        // rstack - LR....
        if(s=="SUB_INT") {
            outFile << indent << "muli(-1, " << right << ");\n";
        }
        if(s=="SUB_FLOAT") {
            outFile << indent << "mulf(-1, " << right << ");\n";
        }
        
        if(s=="PLUS_INT" || s=="SUB_INT") {
            outFile << indent << "addi(" << left << ", " << right << ");" << endl;
        }
        else if(s=="MULT_INT") {
            outFile << indent << "muli(" << left << ", " << right << ");" << endl;
        }
        else if(s=="DIV_INT") {
            outFile << indent << "divi(" << left << ", " << right << ");" << endl;
        }
        else if(s=="PLUS_FLOAT" || s=="SUB_FLOAT") {
            outFile << indent << "addf(" << left << ", " << right << ");" << endl;
        }
        else if(s=="MULT_FLOAT") {
            outFile << indent << "mulf(" << left << ", " << right << ");" << endl;
        }
        else if(s=="DIV_FLOAT") {
            outFile << indent << "divf(" << left << ", " << right << ");" << endl;
        }

        if(typeMapComp.count(s)>0) {
            outFile<<indent<<typeMapComp[s]<<"("<<left<<", "<<right<<");\n";
            outFile<<indent<<myMapComp[s]<<"("<<trueLabel<<");"<<endl;
            outFile<<indent<<"j("<<falseLabel<<");"<<endl;
        }
        if(typeMapComp.count(s)==0) {
            outFile << indent << "move(" << right << ", " << left << ");" << endl;
        }

    }
        
    else if(leftLabel < maxRegs && leftLabel < rightLabel) {
        // outFile << indent << "case 4\n"; 
        // code for right tree
        exp2->genCode();
        string right = rstack.front();
        rstack.pop_front();
        
        // code for left tree
        exp1->genCode();
        
        if(s=="SUB_INT") {
            outFile << indent << "muli(-1, " << right << ");\n";
        }
        if(s=="SUB_FLOAT") {
            outFile << indent << "mulf(-1, " << right << ");\n";
        }
        
        if(s=="PLUS_INT" || s=="SUB_INT") {
            outFile << indent << "addi(" << rstack.front() << ", " << right << ");" << endl;
        }
        else if(s=="MULT_INT") {
            outFile << indent << "muli(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="DIV_INT") {
            outFile << indent << "divi(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="PLUS_FLOAT" || s=="SUB_FLOAT") {
            outFile << indent << "addf(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="MULT_FLOAT") {
            outFile << indent << "mulf(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="DIV_FLOAT") {
            outFile << indent << "divf(" << rstack.front() << "," << right << ");" << endl;
        }
        if(typeMapComp.count(s)>0) {
            outFile<<indent<<typeMapComp[s]<<"("<<rstack.front()<<", "<<right<<");\n";
            outFile<<indent<<myMapComp[s]<<"("<<trueLabel<<");"<<endl;
            outFile<<indent<<"j("<<falseLabel<<");"<<endl;        
        }
        rstack.push_front(right);
    }

    else if(leftLabel >= maxRegs && rightLabel >= maxRegs) {
        // outFile << indent << "Case 6\n";
        exp2->genCode();
        
        if(exp2->astType == "int") 
            outFile << indent << "pushi(" << rstack.front() << ");" << endl;
        if(exp2->astType == "float") 
            outFile << indent << "pushf(" << rstack.front() << ");" << endl;
        
        exp1->genCode();
        string right = rstack.front();
        rstack.pop_front();
        if(exp2->astType == "int") 
            outFile << indent << "loadi(ind(esp), " << rstack.front() << ");" << endl;
        if(exp2->astType == "float") 
            outFile << indent << "loadf(ind(esp), " << rstack.front() << ");" << endl;
        
        if(s=="SUB_INT") {
            outFile << indent << "muli(-1, " << right << ");\n";
        }
        if(s=="SUB_FLOAT") {
            outFile << indent << "mulf(-1, " << right << ");\n";
        }
        
        if(s=="PLUS_INT" || s=="SUB_INT") {
            outFile << indent << "addi(" << rstack.front() << ", " << right << ");" << endl;
        }
        else if(s=="MULT_INT") {
            outFile << indent << "muli(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="DIV_INT") {
            outFile << indent << "divi(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="PLUS_FLOAT" || s=="SUB_FLOAT") {
            outFile << indent << "addf(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="MULT_FLOAT") {
            outFile << indent << "mulf(" << rstack.front() << "," << right << ");" << endl;
        }
        else if(s=="DIV_FLOAT") {
            outFile << indent << "divf(" << rstack.front() << "," << right << ");" << endl;
        }
        if(typeMapComp.count(s)>0) {
            outFile<<indent<<typeMapComp[s]<<"("<<rstack.front()<<", "<<right<<");\n";
            outFile<<indent<<myMapComp[s]<<"("<<trueLabel<<");"<<endl;
            outFile<<indent<<"j("<<falseLabel<<");"<<endl;        
        }
        
        rstack.push_front(right);
        if(exp2->astType == "int") 
            outFile << indent << "popi(1);\n";
        if(exp2->astType == "float") 
            outFile << indent << "popf(1);\n";
    }
}

UnopAst::UnopAst(string a, abstract_astnode* b){
    s = a;
    exp1 = b;
    astType = b->astType;
    label = b->label;
}
             
void UnopAst::print(){
    std::cout<<"( "<<s<<" ";
    exp1->print();
    std::cout<<" ) \n";                 
}             

string UnopAst::getString(){
    return exp1->getString();
}

void UnopAst::genCode() {
    //cout << "in unop \n";
    if(s=="PP") {
        int off = currentTable->getOffset(exp1->getString());
        // cout << currentTable->getOffset(exp1->getString()) << endl;
        if(exp1->astType == "int"){
            outFile<<indent<<"loadi(ind(ebp, " << off << "), " << rstack.front() << ");\n";
            outFile<<indent<<"addi(1, " << rstack.front() << ");\n";
            outFile<<indent<<"storei(" << rstack.front() << ", ind(ebp, " << off << "));\n";
        }
        if(exp1->astType == "float"){
            outFile<<indent<<"loadf(ind(ebp, " << off << "), " << rstack.front() << ");\n";
            outFile<<indent<<"addf(1, " << rstack.front() << ");\n";
            outFile<<indent<<"storef(" << rstack.front() << ", ind(ebp, " << off << "));\n";
        }
    }
    else if(s=="NOT") {
        swap(trueLabel, falseLabel);
        exp1->genCode();
    }
    else if(s == "UMINUS") {
        exp1->genCode();
        if(exp1->astType == "int") 
            outFile << indent << "muli(-1, " << rstack.front() << ");\n";

        if(exp1->astType == "float") 
            outFile << indent << "mulf(-1, " << rstack.front() << ");\n";
    }
    else {
        exp1->genCode(); // answer in reg - 
        if(s=="TO_INT") {
            outFile<<indent<<"floatToint("<<rstack.front()<<");\n"; 
        }
        else if(s=="TO_FLOAT") {
            outFile<<indent<<"intTofloat("<<rstack.front()<<");\n"; 
        }
    }
    // TODO: other unary operators -> '-'
}

FuncCallAst::FuncCallAst(string a, list<abstract_astnode*> l) {
    s=a;
    exp = l;
    label = 1;
    for(auto it = l.begin(); it!=l.end(); it++) {
        label = max((*it)->label, label);
    }
}

void FuncCallAst::print() {
    std::cout<<"Function call( " << s << " ";
    for(list<abstract_astnode*>::iterator it = exp.begin(); it!=exp.end(); it++) {
        (*it)->print();
        //std::cout << std::endl;
    }
    std::cout << " )" << std::endl;
}

void FuncCallAst::genCode() {
    
    if(s=="printf"){
        auto it = exp.begin();
        //if(exp.size()==1){ 
        //    outFile<<indent<<"print_string("<< (*it)->getString() <<");"<<endl;
        //}
        
        //else{
            while(it!=exp.end()){
                if((*it)->astType=="string"){outFile<<indent<<"print_string("<< (*it)->getString() <<");";}
                else{
                    (*it)->genCode();
                    if((*it)->astType=="int"){outFile<<indent<<"print_int("<<rstack.front()<<");";}   
                    if((*it)->astType=="float"){outFile<<indent<<"print_float("<<rstack.front()<<");";}   
                }
                ++it;
            }
        //}
    }

    if(s!="printf"){
        outFile << indent << "//function call\n";

        // save using regs
        int numUse = rstackOrig.size() - rstack.size();
        int i = 0;
        for(auto it = rstackOrig.begin(); it != rstackOrig.end(); it++) {
            if(i>=numUse) break;
            outFile<<indent<< "pushi(" <<(*it) << ");\n";
            i++;
        }
        list<string> rstackOld = rstack;
        rstack = rstackOrig;

        // space for return value
        if(globalTable->functionReturnVal[s] == "int") {
            outFile<<indent<<"pushi(0);\n"; 
        }
        else if(globalTable->functionReturnVal[s] == "float") {
            outFile<<indent<<"pushf(0.0);\n"; 
        }

        // pushing arguments into stack
        for(auto it = exp.rbegin(); it!=exp.rend(); it++) {
            (*it)->genCode();
            if((*it)->astType == "int") 
                outFile<<indent<<"pushi("<<rstack.front()<<");\n";
            else if((*it)->astType == "float") 
                outFile<<indent<<"pushf("<<rstack.front()<<");\n";
        }

        outFile<<indent<<s<<"();\n";

        // after function call 

        // pop off the params
        int consecInt = 0;
        int consecFloat = 0;
        for(auto it = exp.begin(); it!=exp.end(); it++) {
            if((*it)->astType == "int") {
                if(consecFloat != 0) {
                    outFile<<indent<<"popf("<<consecFloat<<");\n";
                    consecFloat = 0;
                }
                consecInt++;
            }
            else if((*it)->astType == "float") {
                if(consecInt != 0) {
                    consecInt = 0;
                    outFile<<indent<<"popi("<<consecInt<<");\n";
                }
                consecFloat++;
            }
        }

        if(consecInt != 0) 
            outFile<<indent<<"popi("<<consecInt<<");\n";
        if(consecFloat != 0)
            outFile<<indent<<"popf("<<consecFloat<<");\n";

        // restore register stack
        rstack = rstackOld;

        // pop the return value
        if(globalTable->functionReturnVal[s] == "int") {
            outFile<<indent<<"loadi(ind(esp), " << rstack.front() << ");\n"; 
            outFile<<indent<<"popi(1);\n";
        }
        else if(globalTable->functionReturnVal[s] == "float") {
            outFile<<indent<<"loadf(ind(esp), " << rstack.front() << ");\n"; 
            outFile<<indent<<"popf(1);\n";
        }

        // restore save regs
        i = 0;
        for(auto it = rstackOrig.begin(); it != rstackOrig.end(); it++) {
            if(i>=numUse) break;
            outFile<<indent<< "loadi(ind(esp), " <<(*it) << ");\n";
            outFile<<indent<< "popi(1);\n";
            i++;
        }

        outFile << indent << "//function call end\n";
    }
}

FloatConstAst::FloatConstAst(string a){
    astType = "float";
    s=a;
    label = 0;
}

void FloatConstAst::print(){
    cout<<"(FLOATCONST "<<s<<")";
}

float FloatConstAst::getFloat(){
    return stof(s);
}


string FloatConstAst::getString(){
    return s;
}

void FloatConstAst::genCode(){
    outFile<<indent<<"move("<<s<<","<<rstack.front()<<");\n";
}

IntConstAst::IntConstAst(string a){
    astType = "int";
    s=a;
    label = 0;
}

int IntConstAst::getInt(){
    return stoi(s);
}

string IntConstAst::getString(){
    return s;
}

void IntConstAst::print(){
    cout<<"(INTCONST "<<s<<")";
}

void IntConstAst::genCode(){
    outFile<<indent<<"move("<<s<<","<<rstack.front()<<"); \n";
}

StringConstAst::StringConstAst(std::string a){
    s=a;
    astType = "string";
}

void StringConstAst::print(){
    printf("(STRINGCONST %s)",s.c_str());
}

string StringConstAst::getString(){
    return s;
}

IdentifierAst::IdentifierAst(std::string a){
    bool ret = currentTable->lookup(lexp);
    if(ret) {
        astType = currentTable->entryMap[a]->type;
    }
    s=a;
    label = 1;
}

void IdentifierAst::print(){
    printf("(IDENTIFIER %s)",s.c_str());
    //cout<<label<<endl;
}

void IdentifierAst::genCode(){
    int offset = currentTable->getOffset(s);
    if(astType=="int") outFile<<indent<<"loadi(ind(ebp, "<<offset<<"), "<<rstack.front()<<"); \n";
    if(astType=="float") outFile<<indent<<"loadf(ind(ebp, "<<offset<<"), "<<rstack.front()<<"); \n";
}

string IdentifierAst::getString(){
    return s;
}


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
    string temp = a->astType;
    int commaPos = find(temp.begin(), temp.end(), ',') - temp.begin();
    if(commaPos==temp.length()){
        printf("Error in line %i, Type error \n", lineno);
        exit(1);
    }
    temp.erase(0,commaPos+1);
    temp.pop_back();
    astType = temp;
    label = 1;
}

string IndexAst::getString(){
    return exp1->getString()+"|"+exp2->getString();
}

void IndexAst::genCode(){
    int offset = currentTable->getOffset(getString());
    //cout<<"IndexAstType is "<<astType<<endl;
    if(astType=="int") outFile<<indent<<"loadi(ind(ebp, "<<offset<<"), "<<rstack.front()<<"); \n";
    if(astType=="float") outFile<<indent<<"loadf(ind(ebp ,"<<offset<<"), "<<rstack.front()<<"); \n";
}

