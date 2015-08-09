/* Changes:  */

/* 1. Character constants removed */
/* 2. Changed INTCONSTANT to INT_CONSTANT */
/* 3. Changed the production for constant_expression to include FLOAT_CONSTANT */
/* 4. Added examples of FLOAT_CONSTANTS */
/* 5. Added the description of STRING_LITERAL */
/* 6. Changed primary_expression and FOR */
/* 7. The grammar permits a empty statement. This should be  */
/*    explicitly represented in the AST. */
/* 8. To avoid local decl inside blocks, a rule for statement  */
/*    has been changed. */

/* ----------------------------------------------------------------------- */

/* start symbol is translation_unit */

/* ---------------------------------------------------- */
/*
Assuming we have a global symbol table initialised before already */

%scanner Scanner.h
%scanner-token-function d_scanner.lex()


%token VOID INT FLOAT FLOAT_CONSTANT INT_CONSTANT AND_OP OR_OP EQ_OP NE_OP LE_OP GE_OP STRING_LITERAL IF ELSE WHILE FOR RETURN IDENTIFIER INC_OP

%polymorphic ast:abstract_astnode*; stringTag:std::string; listTag:std::list<abstract_astnode*>

%type <ast> expression l_expression primary_expression postfix_expression unary_expression multiplicative_expression additive_expression 
iteration_statement selection_statement statement function_definition translation_unit logical_and_expression
equality_expression relational_expression assignment_statement compound_statement constant_expression 

%type <listTag> expression_list statement_list

%type <stringTag> unary_operator IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL INC_OP type_specifier fun_declarator


%%

translation_unit
    : function_definition 
    {
        //($1)->abstract_astnode::print();
    }
    | translation_unit function_definition 
    {
        //globalTable->print();

    }
    ;

function_definition
    : 
    type_specifier fun_declarator compound_statement 
    {
        /*
        bool ret;
        ret = globalTable->insert(funcName,currentTable);
        if(!ret) {
            cout << "function name " << funcName << " already defined at line " << lineno << endl;
            exit(1);
        }
        */
        bool espPushing = (currentTable->offsetVar)!=0;
        if(espPushing) outFile<<indent<< "addi(" << currentTable->offsetVar << ", esp);\n";
        
        ($3)->genCode();

        // printing symbol table to cout
        cout << endl;
        cout << $2 << endl;
        cout << "----------------------" << endl;
        currentTable->print();
        cout << endl;
        
        /*
        ret = globalTable->insertFunctionNames(funcName, parameterTypes, $1);
        cout<<funcName<<" ";
        for(int i=0; i<parameterTypes.size(); i++){
            cout<<parameterTypes[i]<<" ";
        }
        cout<<"\n";
        */
        ($3)->print();
        outFile<<"e:\n";
        if($2 != "main") {//restoring dynamic link
            if(espPushing) 
                outFile<<indent<< "addi(" << -currentTable->offsetVar << ", esp);\n";
            outFile<<indent<<"loadi(ind(ebp), ebp);\n";
            outFile<<indent<<"popi(1);\n";
        }
        else {
            if(espPushing) 
                outFile<<indent<< "addi(" << -currentTable->offsetVar << ", esp);\n";
        }
        outFile<<indent<<"return;\n";
        outFile << "}\n\n";


        currentTable = new lst();        
        indent = "";
        labelCount = 0;
        firstType = true;
        parameterTypes.clear();
            //currentTable->clear();
        //}
    }
    ;

type_specifier
    : VOID 
    {
        $$ = "void";
        type = "void";
        typeAll = "void";
        width = 0;
        widthAll = 0;
        if(firstType){
            returnType = type;
            firstType = false;
        }
    }
    | INT 
    {
        $$ = "int";
        type = "int";
        typeAll = "int";
        width = 4;
        widthAll = 4;
        if(firstType){
            returnType = type;
            firstType = false;
        }
    }
    | FLOAT 
    {
        $$ = "float";
        type = "float";
        typeAll = "float";
        width = 4;
        widthAll = 4;
        if(firstType){
            returnType = type;
            firstType = false;
        }
    }
    ;

fun_declarator
    : IDENTIFIER '(' parameter_list ')' 
    {
        funcName = $1;
        bool ret;
        ret = globalTable->insert(funcName,currentTable);
        if(!ret) {
            cout << "function name " << funcName << " already defined at line " << lineno << endl;
            exit(1);
        }

        globalTable->insertFunctionNames(funcName, parameterTypes);
        $$ = $1;

        globalTable->functionReturnVal[$1] = returnType;

        outFile << indent << "void " << funcName << "()\n{\n";
        indent += "\t";
        if(funcName != "main") {//setting dynamic link
            outFile<<indent<<"pushi(ebp);\n";
            outFile<<indent<<"move(esp,ebp);\n";
        }
    }
    | IDENTIFIER '(' ')'
    {
        funcName = $1;

        bool ret;
        ret = globalTable->insert(funcName,currentTable);
        if(!ret) {
            cout << "function name " << funcName << " already defined at line " << lineno << endl;
            exit(1);
        }

        globalTable->insertFunctionNames(funcName, parameterTypes);
        $$ = $1;
        globalTable->functionReturnVal[$1] = returnType;
        outFile << indent << "void " << funcName << "()\n{\n";
        indent += "\t";  
        if(funcName != "main") {//setting dynamic link
            outFile<<indent<<"pushi(ebp);\n";
            outFile<<indent<<"move(esp,ebp);\n";
        }
    } 
    ;

parameter_list //No need ST
    : parameter_declaration 
    | parameter_list ',' parameter_declaration 
    ;

parameter_declaration 
    : type_specifier declarator
    {
        lstEntry* row = new lstEntry(name, type, true, width, currentTable->offsetParam);
        currentTable->offsetParam = currentTable->offsetParam + width;
        bool ret = currentTable->insert(name,row);
        if(!ret) {
            cout << "Redefinition of paramater " << name << " not allowed at line " << lineno << endl;
            exit(1);
        }
        if(type == "void") {
            cout << "Void declaration not allowed at line " << lineno << endl;
            exit(1);
        }

        parameterTypes.push_back(type); 
    }
    ;

declarator
    : IDENTIFIER // No need ST 
    {
        name = $1;
        type = typeAll;
        width = widthAll;
    }
    | declarator '[' constant_expression ']' //Should write for arrays 
    {
        
        if($3->astType!="int"){
            printf("Error in line %i: Only int can be used in array declaration \n", lineno);
            exit(1);
        }  
        
        width = width*intConst;
        type = "array("+ to_string(intConst) + "," + type + ")";
    }
    ;

constant_expression 
        : INT_CONSTANT 
        {
            intConst = stoi($1);
            $$ =  new IntConstAst($1);
        }
        | FLOAT_CONSTANT 
        {
            $$ = new FloatConstAst($1);
        }
        ;

compound_statement
    : '{' '}' 
    {
        $$ = new BlockAst(std::list<abstract_astnode*>());
    }
    | '{' statement_list '}' 
    {
        $$ = new BlockAst($2);
    }
    | '{' declaration_list statement_list '}' 
    {
        // TODO make space in stack after declaration list
        $$ = new BlockAst($3);// doubt
    }
    ;

statement_list
    : statement
    {
        $$ = std::list<abstract_astnode*>(1,$1);
    }
        | statement_list statement  
        {
            $$ = $1;
            ($$).push_back($2);
        }
    ;

statement
        : '{' statement_list '}'  //a solution to the local decl problem
        {
            $$ = new BlockAst($2);
        }
        | selection_statement   
        {
            $$ = $1;
        }
        | iteration_statement
        {
            $$ = $1;
        }   
        | assignment_statement  
        {
            $$ = $1;
        }
        | RETURN expression ';'
        {
            if(returnType != $2->astType) {
                
                if(returnType=="int" && $2->astType=="float"){
                    //cout<<"100"<<endl;
                    UnopAst* temp = new UnopAst("TO_INT", $2);
                    temp->astType = "int";
                    $$ = new ReturnAst(temp);        
                }

                else if(returnType=="float" && $2->astType=="int"){
                    //cout<<"200"<<endl;
                    UnopAst* temp = new UnopAst("TO_FLOAT", $2);
                    temp->astType = "float";
                    $$ = new ReturnAst(temp);        
                }

                else
                {
                    cout << "Return type should be " << returnType << " given " ;
                    cout << $2->astType << " at line " << lineno << endl;
                    exit(1);
                }
            }
            else
            {
                $$ = new ReturnAst($2);
            }
            
        }
        | IDENTIFIER '(' expression_list ')' ';'
        {
            bool ret = currentTable->lookup($1);
            if(ret) {
                cout << "Variable " << $1 << " has same name as function";
                cout << " at line " << lineno << endl;
                exit(1);
            }

            if($1 == "printf") {
                auto callParamList = $3;
                auto argType = (*callParamList.begin())->astType;
                    $$ = new FuncCallAst($1, $3);
                    $$->astType = "int";
            }
            else {
                auto mapFunc = globalTable->functionTables;
                if(mapFunc.find($1) == mapFunc.end()) {
                    cout << "Function " << $1 << " not declared at line " << lineno << endl;
                    exit(1);
                }
                //cout << globalTable->functionReturnVal[$1] << endl;
                auto funcParamList = globalTable->functionNames[$1];
                //auto callParamList = ($3);
                if(funcParamList.size() != ($3).size()) {
                    cout << "Number of parameters don't match in function call " << $1 ;
                    cout << " at line " << lineno << endl;
                    exit(1);
                }
                int countTemp = 0;
                for(auto it = ($3).begin(); it!= ($3).end(); it++) {
                    if((*it)->astType != funcParamList[countTemp]) {
                        if((*it)->astType=="int" && funcParamList[countTemp]=="float"){
                            UnopAst* temp = new UnopAst("TO_FLOAT", *it);
                            temp->astType = "float";
                            *it = temp;
                            //$$ = new FuncCallAst($1, temp);
                        }
                        else if((*it)->astType=="float" && funcParamList[countTemp]=="int"){
                            abstract_astnode* temp = new UnopAst("TO_INT", *it);
                            temp->astType = "int";
                            *it = temp;   
                            //$$ = new FuncCallAst($1, temp);
                        }
                        else
                        {                            
                            cout << "Type mismatch " << (*it)->astType << " and " << funcParamList[countTemp];
                            cout << " in parameter " << countTemp+1 << " of function call at line ";
                            cout << lineno << endl; 
                            exit(1);
                        }
                    }
                    countTemp++;
                }
                $$ = new FuncCallAst($1, $3);
                $$->astType = globalTable->functionReturnVal[$1];                
            }
        }
        ;

assignment_statement
    : ';'
    {
        $$ = new EmptyAst();
    }                               
    |  l_expression '=' expression ';' 
    {
        //cout << "check " << $3->astType << endl;
        //cout << endl;
        bool ret = currentTable->lookup(lexp);
        assert(ret);
        string ltype = $1->astType;
        string rtype = $3->astType;
        /*
        $1->print();
        cout << endl;
        $3->print();
        cout << endl;
        cout << ltype << " " << rtype << endl; 
        */
        // string lName = $1->getString();
        
        //cout<<"Name is "<<lName<<endl;
        
        if(ltype == "int" && rtype == "float") {
            UnopAst* temp = new UnopAst(std::string("TO_INT"), $3);
            temp->astType = "int";
            $$ = new OpAst(std::string("ASSIGNMENT"), $1, temp);
        }
        else if (ltype == "float" && rtype == "int") {
            UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
            temp->astType = "float";
            $$ = new OpAst(std::string("ASSIGNMENT"), $1, temp);
        } 
        else if(ltype != rtype) {
            cout << "Types mismatch - " << ltype << " and " << rtype << " at line " << lineno << endl;
            exit(1);
        }
        else  {
            $$ = new OpAst(std::string("ASSIGNMENT"), $1, $3);
        }
        //cout<<"Label is "<< $3->label <<endl;
        //cout << endl;
        // int offset = currentTable->getOffset(lName);
        // outFile<<indent<< "storei(eax, (ind(ebp,"<<offset<<"))); \n";
    }
    ;


expression
        : logical_and_expression 
        {
            $$ = $1;
        }
        | expression OR_OP logical_and_expression
        {
            $$ = new OpAst("OR",$1,$3);
            $$->astType = "int";
        }
    ;

logical_and_expression
        : equality_expression
        {
            $$ = $1;
        }
        | logical_and_expression AND_OP equality_expression 
        {
            $$ = new OpAst(std::string("AND"),$1,$3);
            $$->astType = "int";
        }
    ;

equality_expression
    : relational_expression
    {
        $$ =$1;
    } 
    | equality_expression EQ_OP relational_expression
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {
            
            if($1->astType=="int" && $3->astType=="int")
            {
                $$ = new OpAst(std::string("EQ_OP_INT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="float")
            {
                $$ = new OpAst(std::string("EQ_OP_FLOAT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("EQ_OP_FLOAT"),$1, temp);     
                $$->astType = "int";
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("EQ_OP_FLOAT"), temp, $3);    
                $$->astType = "int";
            }

        }

        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        }
    }   
    | equality_expression NE_OP relational_expression
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {
            
            if($1->astType=="int" && $3->astType=="int")
            {
                $$ = new OpAst(std::string("NE_OP_INT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="float")
            {
                $$ = new OpAst(std::string("NE_OP_FLOAT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("NE_OP_FLOAT"),$1, temp);     
                $$->astType = "int";
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("NE_OP_FLOAT"), temp, $3);    
                $$->astType = "int";
            }

        }

        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        }
    }   
    ;


relational_expression
    : additive_expression
    {
        $$ = $1;
    }
    | relational_expression '<' additive_expression
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {
            
            if($1->astType=="int" && $3->astType=="int")
            {
                $$ = new OpAst(std::string("LT_INT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="float")
            {
                $$ = new OpAst(std::string("LT_FLOAT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("LT_FLOAT"),$1, temp);     
                $$->astType = "int";
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("LT_FLOAT"), temp, $3);    
                $$->astType = "int";
            }

        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        }
    } 
    | relational_expression '>' additive_expression 
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {

            if($1->astType=="int" && $3->astType=="int")
            {
                $$ = new OpAst(std::string("GT_INT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="float")
            {
                $$ = new OpAst(std::string("GT_FLOAT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("GT_FLOAT"),$1, temp);     
                $$->astType = "int";
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("GT_FLOAT"), temp, $3);    
                $$->astType = "int";
            }

        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        }
    } 
    | relational_expression LE_OP additive_expression 
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {
            
            if($1->astType=="int" && $3->astType=="int")
            {
                $$ = new OpAst(std::string("LE_OP_INT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="float")
            {
                $$ = new OpAst(std::string("LE_OP_FLOAT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("LE_OP_FLOAT"),$1, temp);     
                $$->astType = "int";
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("LE_OP_FLOAT"), temp, $3);    
                $$->astType = "int";
            }
        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        }
    } 
    | relational_expression GE_OP additive_expression 
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {
            
            if($1->astType=="int" && $3->astType=="int")
            {
                $$ = new OpAst(std::string("GE_OP_INT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="float")
            {
                $$ = new OpAst(std::string("GE_OP_FLOAT"),$1,$3);
                $$->astType = "int";
            }
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("GE_OP_FLOAT"),$1, temp);     
                $$->astType = "int";
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("GE_OP_FLOAT"), temp, $3);    
                $$->astType = "int";
            }
        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        }
    } 
    ;

additive_expression 
    : multiplicative_expression
    {
        $$ = $1;
    }
    | additive_expression '+' multiplicative_expression
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {

            if($1->astType=="int" && $3->astType=="int")
                $$ = new OpAst(std::string("PLUS_INT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="float")
                $$ = new OpAst(std::string("PLUS_FLOAT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("PLUS_FLOAT"),$1, temp);     
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("PLUS_FLOAT"), temp, $3);    
            }

        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        } 
    } 
    | additive_expression '-' multiplicative_expression
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {
            
            if($1->astType=="int" && $3->astType=="int")
                $$ = new OpAst(std::string("SUB_INT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="float")
                $$ = new OpAst(std::string("SUB_FLOAT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("SUB_FLOAT"),$1, temp);     
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("SUB_FLOAT"), temp, $3);    
            }

        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        } 
    } 
    ;

multiplicative_expression
    : unary_expression
    {
        $$ = $1;
    }
    | multiplicative_expression '*' unary_expression 
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {
            
            if($1->astType=="int" && $3->astType=="int")
                $$ = new OpAst(std::string("MULT_INT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="float")
                $$ = new OpAst(std::string("MULT_FLOAT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("MULT_FLOAT"),$1, temp);     
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("MULT_FLOAT"), temp, $3);    
            }

        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        } 
    }
    | multiplicative_expression '/' unary_expression 
    {
        if(($1->astType == "int" || $1->astType == "float") 
            && ($3->astType == "int" || $3->astType == "float")) {

            if($1->astType=="int" && $3->astType=="int")
                $$ = new OpAst(std::string("DIV_INT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="float")
                $$ = new OpAst(std::string("DIV_FLOAT"),$1,$3);
            
            else if($1->astType=="float" && $3->astType=="int") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $3);
                temp->astType = "float";
                $$ = new OpAst(std::string("DIV_FLOAT"),$1, temp);     
            }
            
            else if ($1->astType=="int" && $3->astType=="float") 
            {
                UnopAst* temp = new UnopAst(std::string("TO_FLOAT"), $1);
                temp->astType = "float";
                $$ = new OpAst(std::string("DIV_FLOAT"), temp, $3);    
            }

        }
        else {
            cout << "Invalid comparision between " << $1->astType << " and " << $3->astType;
            cout << " at line " << lineno << endl;
            exit(1);
        } 
    }
    ;

unary_expression
    : postfix_expression
    {
        $$ = $1;
    }   
    | unary_operator postfix_expression
    {
        if($2->astType == "int" || $2->astType == "float") {
            $$ = new UnopAst($1,$2);
            if($1 == "NOT") {
                $$->astType = "int";
            }
            //$$->astType = $2->astType;
        }
        else {
            cout << "Invalid operand " << $2->astType << " for unary operator at line ";
            cout << lineno << endl;
            exit(1);
        }
    } 
    ;

postfix_expression
    : primary_expression
    {
        $$ = $1;
    }
    | IDENTIFIER '(' ')'
    {
        bool ret = currentTable->lookup($1);
        if(ret) {
            cout << "Variable " << $1 << " has same name as function";
            cout << " at line " << lineno << endl;
            exit(1);
        }

        auto mapFunc = globalTable->functionTables;
        if($1 == "printf") {
            cout << $1 << " has too few arguments at line " << lineno << endl;
            exit(1);            
        }
        if(mapFunc.find($1) == mapFunc.end()) {
            cout << "Function " << $1 << " not declared at line " << lineno << endl;
            exit(1);
        }
        //cout << globalTable->functionReturnVal[$1] << endl;
        auto funcParamList = globalTable->functionNames[$1];
        //auto callParamList = $3;
        if(funcParamList.size() != 0) {
            cout << "Number of parameters don't match in function call " << $1 ;
            cout << " at line " << lineno << endl;
            exit(1);
        }

        $$ = new FuncCallAst($1,list<abstract_astnode*>());
        $$->astType = globalTable->functionReturnVal[$1];
    }
    | IDENTIFIER '(' expression_list ')' 
    {
        bool ret = currentTable->lookup($1);
        if(ret) {
            cout << "Variable " << $1 << " has same name as function";
            cout << " at line " << lineno << endl;
            exit(1);
        }


        if($1 == "printf") {
            auto callParamList = $3;
            auto argType = (*callParamList.begin())->astType;
            if(argType != "string") {
                cout << "First argument of printf not a string at line " << lineno << endl;
                exit(1);
            }
            else {
                $$ = new FuncCallAst($1, $3);
                $$->astType = "int";
            }
        }
        else {
            auto mapFunc = globalTable->functionTables;
            if(mapFunc.find($1) == mapFunc.end()) {
                cout << "Function " << $1 << " not declared at line " << lineno << endl;
                exit(1);
            }
            //cout << globalTable->functionReturnVal[$1] << endl;
            auto funcParamList = globalTable->functionNames[$1];
            //auto callParamList = ($3);
            if(funcParamList.size() != ($3).size()) {
                cout << "Number of parameters don't match in function call " << $1 ;
                cout << " at line " << lineno << endl;
                exit(1);
            }
            int countTemp = 0;
            for(auto it = ($3).begin(); it!= ($3).end(); it++) {
                
                if((*it)->astType != funcParamList[countTemp]) {

                    if((*it)->astType=="int" && funcParamList[countTemp]=="float"){
                        UnopAst* temp = new UnopAst("TO_FLOAT", *it);
                        temp->astType = "float";
                        *it = temp;
                        //$$ = new FuncCallAst($1, temp);
                    }

                    else if((*it)->astType=="float" && funcParamList[countTemp]=="int"){
                        abstract_astnode* temp = new UnopAst("TO_INT", *it);
                        temp->astType = "int";
                        *it = temp;
                        
                        //$$ = new FuncCallAst($1, temp);
                    }

                    else
                    {
                        
                        cout << "Type mismatch " << (*it)->astType << " and " << funcParamList[countTemp];
                        cout << " in parameter " << countTemp+1 << " of function call at line ";
                        cout << lineno << endl; 
                        exit(1);
                    }
                }
                countTemp++;
            }
            $$ = new FuncCallAst($1, $3);
            $$->astType = globalTable->functionReturnVal[$1];
        }
    }
    | l_expression INC_OP
    {
        if($1->astType == "int" || $1->astType == "float") {
            $$ = new UnopAst("PP",$1);
            //$$->astType = $1->astType;
        }
        else {
            cout << "Invalid operand " << $1->astType <<  " for increment increment at line ";
            cout << lineno << endl;
            exit(1);
        }
    }
    ;

primary_expression
    : l_expression
    {
        $$ = $1;
    }
    | l_expression '=' expression // added this production
    {
        $$ = new AssAst($1,$3);
    }
    | INT_CONSTANT
    {
        $$ = new IntConstAst($1);
    }
    | FLOAT_CONSTANT
    {
        $$ = new FloatConstAst($1);
    }
    | STRING_LITERAL
    {
        $$ = new StringConstAst($1);
    }
    | '(' expression ')'
    {
        $$ = $2;
    }
    ;

l_expression
        : IDENTIFIER
        {
            lexp = $1;
            //cout << "Lexp is " <<lexp<< endl;
            bool ret = currentTable->lookup($1);
            if(!ret) {
                cout << "Variable " << $1 << " not declared in the function " << funcName;
                cout << " at line " << lineno << endl;
                exit(1);
            }
            $$ = new IdentifierAst($1);
        }
        | l_expression '[' expression ']'
        {
            // type checking to be done.
            //lexp = lexp + "|" + $3->getString();
            //cout<<"Final is "<<lexp<<endl;
            $$ = new IndexAst($1, $3);
            if($3->astType != "int") {
                printf("Array indices must be of type int in line %i \n", lineno);
                exit(1);    
            }

        }
        ;

expression_list
        : expression
        {
            $$ = std::list<abstract_astnode*>(1,$1);
        }
        | expression_list ',' expression
        {
            $$ = $1;
            ($$).push_back($3);
        }
;

unary_operator
        : '-'
        {
            $$ = "UMINUS";
        }   
        | '!'
        {
            $$ = "NOT";
        }
        ;

selection_statement
        : IF '(' expression ')' statement ELSE statement 
        {
            $$ = new IfAst($3,$5,$7);
        }
    ;

iteration_statement
    : WHILE '(' expression ')' statement
    {
        $$ = new WhileAst($3,$5);
    }   
        | FOR '(' expression ';' expression ';' expression ')' statement  //modified this production
        {
            $$= new ForAst($3, $5, $7, $9);         
        }
        ;

declaration_list
        : declaration                   
        | declaration_list declaration
    ;

declaration
    : type_specifier declarator_list';'
    {
        if(type == "void") {
            cout << "Void declaration not allowed at line " << lineno << endl;
            exit(1);
        } 
    }
    ;

declarator_list
    : declarator
    {
        //<New>
        string s=type;
        vector<string> a;
        int pos = find(s.begin(),s.end(),'(')-s.begin();
        while(pos!=s.length()){
            s.erase(0,pos+1);
            a.push_back(s.substr(0,find(s.begin(),s.end(),',')-s.begin()));
            pos = find(s.begin(),s.end(),'(')-s.begin();
        }
        pos = find(s.begin(), s.end(),')') - s.begin();
        int posComma = find(s.begin(), s.end(),',') - s.begin();
        if(pos!=s.size() || posComma!= s.size()){
            type = s.substr(posComma+1,pos-posComma-1);
        }
        for(int i=0;i<a.size();i++){
            type="array("+a[i]+","+type+")";
        }
        //</New>
        currentTable->offsetVar = currentTable->offsetVar - width;
        lstEntry* row = new lstEntry(name, type, false, width, currentTable->offsetVar);// variable declaration
        bool ret = currentTable->insert(name,row);
        if(!ret) {
            cout << "Redeclaration of " << name << " not allowed at line " << lineno << endl;
            exit(1);
        }
    }
    | declarator_list ',' declarator 
    {
        //<New>
        //cout<<"Type is "<<type<<endl;
        string s=type;
        vector<string> a;
        int pos = find(s.begin(),s.end(),'(')-s.begin();
        while(pos!=s.length()){
            s.erase(0,pos+1);
            a.push_back(s.substr(0,find(s.begin(),s.end(),',')-s.begin()));
            pos = find(s.begin(),s.end(),'(')-s.begin();
        }
        pos = find(s.begin(), s.end(),')') - s.begin();
        int posComma = find(s.begin(), s.end(),',') - s.begin();
        if(pos!=s.size() || posComma!= s.size()){
            type = s.substr(posComma+1,pos-posComma-1);
        }
        for(int i=0;i<a.size();i++){
            type="array("+a[i]+","+type+")";
        }
        //</New>
        currentTable->offsetVar = currentTable->offsetVar - width;
        lstEntry* row = new lstEntry(name, type, false, width, currentTable->offsetVar);// variable declaration
        bool ret = currentTable->insert(name,row);
        if(!ret) {
            cout << "Redeclaration of " << name << " not allowed at line " << lineno << endl;
            exit(1);
        }
    }
    ;


/* A description of integer and float constants. Not part of the grammar.   */

/* Numeric constants are defined as:  */

/* C-constant: */
/*   C-integer-constant */
/*   floating-point-constant */
 
/* C-integer-constant: */
/*   [1-9][0-9]* */
/*   0[bB][01]* */
/*   0[0-7]* */
/*   0[xX][0-9a-fA-F]* */
 
/* floating-point-constant: */
/*   integer-part.[fractional-part ][exponent-part ] */

/* integer-part: */
/*   [0-9]* */
 
/* fractional-part: */
/*   [0-9]* */
 
/* exponent-part: */
/*   [eE][+-][0-9]* */
/*   [eE][0-9]* */

/* The rule given above is not entirely accurate. Correct it on the basis of the following examples: */

/* 1. */
/* 23.1 */
/* 01.456 */
/* 12.e45 */
/* 12.45e12 */
/* 12.45e-12 */
/* 12.45e+12 */

/* The following are not examples of FLOAT_CONSTANTs: */

/* 234 */
/* . */

/* We have not yet defined STRING_LITERALs. For our purpose, these are */
/* sequence of characters enclosed within a pair of ". If the enclosed */
/* sequence contains \ and ", they must be preceded with a \. Apart from */
/* \and ", the only other character that can follow a \ within a string */
/* are t and n.  */
