#include "functions/include_functions.h"
#include "queryprocessing_db.h"
#include "tokenizer.h"
#include "queryparse.h"
#include "createTable.h"
#include "alterTable.h"

using namespace std;

int main(){
	string query;
	string contQuery, createParsedString;

	cout << "q> ";		
	getline(cin, query);	//accepts the input query

	while(*query.rbegin() != ';') //loop until T_SEMICOLON is at the end of the query
	{
		cout << " > ";
		getline(cin, contQuery);	//accepts the continuation of the query 
		query.append(" ");
		query.append(contQuery);
		
	}
	
	DDL_initResword();
	DDL_getSymbol(query);

	//printf("%d", DDL_sym);
	///*
	if(DDL_sym == resSelect){		//select
		//selectClause(query);
	}else if(DDL_sym == T_CREATE){
	    DDL_getSymbol(query);
	    if(DDL_sym == T_TABLE){
			isTable = true;
			createTableClause(query);
			if(!error){
				cout << "\nTable Name: " << tableName;
				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "\nColumns: " << columnNameList[i] << " " << columnNameDataType[i] << " " << columnNameNotNull[i];
			
				/*cout << "\nCount ConstraintColumnName: " << (int)constraintColumnName.size();
				cout << "\nCount ConstraintName: " << (int)constraintName.size();
				cout << "\nCount ConstraintType: " << (int)constraintType.size();
				cout << "\nCount referenceTable: " << (int)referenceTable.size();
				cout << "\nCount referenceColumn: " << (int)referenceColumn.size();*/
			
				for(int i=0;i < (int)constraintColumnName.size();i++)
					cout << "\nConstraints: " << constraintColumnName[i] << "|" << constraintName[i] << "|" << constraintType[i] << "|" << referenceTable[i] << "|" << referenceColumn[i] << "|" << checkCondition[i];
			
				//writeToFile();
				createParsedString = generateCreateParsedString();
				cout << "\n" << createParsedString << "\n";
				columnNameList.clear();
				columnNameDataType.clear();
				columnNameNotNull.clear();
				
				constraintColumnName.clear(); 
				constraintName.clear();
				constraintType.clear();
				referenceTable.clear();
				referenceColumn.clear();
				checkCondition.clear();
				checkColumnNameList.clear();
			}
		}else{
			error = true;
	    	displayError("PARSING ERROR: TABLE should be followed after CREATE.");
	    }
	}else if(DDL_sym == T_ALTER){
		DDL_getSymbol(query);
		if(DDL_sym == T_TABLE){
			isTable = true;
			DDL_getSymbol(query);
			if(DDL_sym == T_TABLE_NAME || DDL_sym == T_DATABASE_TABLE_NAME || DDL_sym == T_USER_DATABASE_TABLE_NAME){
				const char* table_name = tableName.c_str();
				if(check_exist(table_name)){
					multipleAlteration(query);
				}else{
					displayError("PARSING ERROR: Table " +tableName+ " is not yet created.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid table name should be followed after TABLE.");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: TABLE should be followed after CREATE.");
		}

	}else if(DDL_sym == T_DROP){
		DDL_getSymbol(query);
		if(DDL_sym == T_TABLE){
			DDL_getSymbol(query);
			if(DDL_sym == T_IDENT){
				const char* table_name = tableName.c_str();
				if(check_exist(table_name) && prevSym == T_TABLE){
					if(DDL_sym == T_SEMICOLON){
						cout << "Table " << identName << " is dropped successfully!";
					}else{
						error = true;
						displayError("PARSING ERROR: A ';' should be followed after the procedure name.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: The table "+ identName +" is not yet created.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid table name should be followed after TABLE");
			}
		}else if(DDL_sym == T_PROCEDURE){
			DDL_getSymbol(query);
			if(DDL_sym == T_IDENT){
				DDL_getSymbol(query);
				if(DDL_sym == T_SEMICOLON){
					cout << "Procedure " << identName << " is dropped successfully!";
				}else{
					error = true;
					displayError("PARSING ERROR: A ';' should be followed after the procedure name.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid procedure name should be followed after PROCEDURE and VIEW");
			}
		}else if(DDL_sym == T_VIEW){
			DDL_getSymbol(query);
			if(DDL_sym == T_IDENT){
				DDL_getSymbol(query);
				if(DDL_sym == T_SEMICOLON){
					cout << "View " << identName << " is dropped successfully!";
				}else{
					error = true;
					displayError("PARSING ERROR: A ';' should be followed after the view name.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid view name should be followed after PROCEDURE and VIEW");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: TABLE, PROCEDURE or VIEW is expected after DROP.");
		}

	}else{
		error = true;
		displayError("PARSING ERROR: Invalid start of a query statement.");	
		
		/*if(DDL_sym == T_NUMBER || DDL_sym == T_DECIMAL){
			parseArithmeticExpression(query);
		}else if(DDL_sym == T_LPAREN){
			//openParen++;
			//cout << "\nOpenParenSize: " << openParen << "\n";
			DDL_getSymbol(query);
			parseArithmeticExpression(query);
			//DDL_getSymbol(query);
			if(DDL_sym == T_RPAREN){
				//openParen--;
				//cout << "\nOpenParenSize: " << openParen << "\n";
			}
		}*/
	}
	return 0;
}

