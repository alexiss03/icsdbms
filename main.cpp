//#include "processor/database/functions/include_functions.h"
//#include "processor/include.h"
//#include "processor/database/queryprocessing_db.h"
//#include "sqlCompiler.hpp"
//#include "lexical_analyzer/symbol.hpp"
//#include "compiler/include.hpp"


#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <utility>
#include <fstream>

bool isExport = false;
bool isImport = false;

void parseQuery(std::string query);
void uninitialize();

#include "compiler/semantics_analyzer/queryparse.hpp"
#include "processor/include.h"
#include "compiler/include.hpp"
#include "compiler/ddl_headers/ddl_include_functions.h"

using namespace std;

void uninitialize(){
	isMultipleColumn = isConstraintTable = isWithArithGroup = isMultipleCheckCondition = false;
	isImport = isExport = isToAdd = isToModify = isMultipleAlteration = error = multipleTablesUsed = isTable = false;
	isMultipleColumnNameView = isMultipleUpdateColumn = isMultipleProcedureParameter = isMultipleInsertColumn = isMultipleInsertParameter = false;
	
	prevSymbol= insertParameter = insertColumn = charPos = openParen = prevCharPos = 0;
	
	tableName = identName = numberValue = realIdentName = arithSymbol = "";
	procedureName = whereCond = procedureTableName = tempString = checkCond = checkColumnName = "";
	viewTableName = viewName = selectWhereCond = "";

	columnNameList.clear();
	columnNameDataType.clear();
	columnNameNotNull.clear();
	
	constraintColumnName.clear(); 
	constraintName.clear();
	constraintType.clear();
	referenceTable.clear();
	referenceColumn.clear();
	checkCondition.clear();

	columnToBeDrop.clear();
	constraintToBeDrop.clear();
	columnToBeRename.clear();
	constraintToBeRename.clear();
	columnNew.clear();
	constraintNew.clear();
	queryActionColumn.clear();
	queryActionConstraint.clear();

	paramenterName.clear();
	parameterDataType.clear();
	valuesName.clear();

}

int main() {
	//start_logs();
	//declarations	
	//std::string line;	
	cout.precision(10);
	string query;
	string contQuery;

	bpt_pool = (BPT **) malloc(sizeof(BPT *) * 100);
	table_pool = (TABLE **) malloc(sizeof(TABLE *) * 50);
		
	for(int i = 0; i < 100; i++)
		*(bpt_pool + i) = NULL;
	for(int i = 0; i < 50; i++)
		*(table_pool + i) = NULL;
	
	//convert_query_parse_tree_to_relational_algebra(build_test_query());	
	TABLEDATALIST ** table, **temp;
	int i = 0, choice;
	string strTemp;
	char charTemp[100], cur_input;
	//table_pool[0]= build_test_table();
	//table_pool[1]= build_test_table3();
	//print_tabledatalist(table_pool[0]->table);
	//read_file_start("kat");
	//return 1;
	init_keyword();
	DDL_initResword();
	system("clear");
	showmsg();

	while(true){
	
		query = contQuery = "";
		uninitialize();  
		printf("\nICSDBMS> ");  
		getline(cin, query);	//accepts the input query

		while(*query.rbegin() != ';') //loop until T_SEMICOLON is at the end of the query
		{
			cout << "       > ";
			getline(cin, contQuery);	//accepts the continuation of the query 
			query.append(" ");
			query.append(contQuery);
		}
   		
   		DDL_getSymbol(query);
   		parseQuery(query);
		
	}//end of while loop  	
}//end of main

void parseQuery(std::string query){

	std:: string parsedString = "";
	vector<pair<pair<int, string>, pair<string, string> > > token_lexeme;

	if(DDL_sym == T_CREATE){
		   DDL_getSymbol(query);
		   if(DDL_sym == T_TABLE){
			isTable = true;
			createTableClause(query);
			if(!error){
				/*cout << "Table Name: " << tableName << "\n";
				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "Columns: " << columnNameList[i] << " " << columnNameDataType[i] << " " << columnNameNotNull[i] << "\n";*/
			 
				/*cout << "\nCount ConstraintColumnName: " << (int)constraintColumnName.size();
				cout << "\nCount ConstraintName: " << (int)constraintName.size();
				cout << "\nCount ConstraintType: " << (int)constraintType.size();
				cout << "\nCount referenceTable: " << (int)referenceTable.size();
				cout << "\nCount referenceColumn: " << (int)referenceColumn.size();
				cout << "\nCount checkCondition: " << (int)checkCondition.size();*/

				/*for(int i=0;i < (int)constraintColumnName.size();i++)
					cout << "Constraints: " << constraintColumnName[i] << "|" << constraintName[i] << "|" << constraintType[i] << "|" << referenceTable[i] << "|" << referenceColumn[i] << "|" << checkCondition[i] << "\n";*/

				parsedString = generateCreateParsedString();
				queryprocessing(parsedString);
				//cout << "\nParsed String: " << parsedString << "\n";
			}
		}else if(DDL_sym == T_PROCEDURE){
			DDL_getSymbol(query);
			createProcedureClause(query);

			/*if(!error){
				cout << "Procedure Name: " << procedureName << "\n";
				for(int i=0;i < (int)paramenterName.size();i++)
					cout << "Parameter: " << paramenterName[i] << " | " << parameterDataType[i] << "\n";

				cout << "Table Name: " << procedureTableName << "\n";
				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "Column: " << columnNameList[i] << "\n";

				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "Values: " << valuesName[i] << "\n";

				cout << "Where Condition: " << whereCond << "\n";
			}*/	

		}else if(DDL_sym == T_VIEW){
			DDL_getSymbol(query);
			createViewClause(query);

			/*if(!error){
				cout << "View Name: " << viewName << "\n";
				cout << "Table Name: " << viewTableName << "\n";
				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "Column: " << columnNameList[i] << "\n";

				cout << "Where Condition: " << selectWhereCond << "\n";
			}*/

		}else{
			error = true;
		   	displayError("PARSING ERROR: TABLE, PROCEDURE, or VIEW should be followed after CREATE.");
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
					
					if(!error){
					/*cout << "Table Name: " << tableName << "\n";
						for(int i=0;i < (int)columnNameList.size();i++)
							cout << "Columns: " << queryActionColumn[i] << "|" << columnNameList[i] << "|" << columnNameDataType[i] << "|" << columnNameNotNull[i] << "\n";

						for(int i=0;i < (int)constraintColumnName.size();i++)
							cout << "Constraints: " << queryActionConstraint[i] << "|" << constraintColumnName[i] << "|" << constraintName[i] << "|" << constraintType[i] << "|" << referenceTable[i] << "|" << referenceColumn[i] << "|" << checkCondition[i] << "\n";

						for(int i=0;i<(int)columnToBeDrop.size();i++)
							cout << "Drop Column: " << columnToBeDrop[i] << "\n";

						for(int i=0;i<(int)constraintToBeDrop.size();i++)
							cout << "Drop Constraint: " << constraintToBeDrop[i] << "\n";

						for(int i=0;i<(int)columnToBeRename.size();i++)
							cout << "Rename Column: " << columnToBeRename[i] << " to " << columnNew[i] << "\n";

						for(int i=0;i<(int)constraintToBeRename.size();i++)
							cout << "Rename Constraint: " << constraintToBeRename[i] << " to " << constraintNew[i] << "\n";*/

						generateAlterAddParsedString();
						generateAlterDropParsedString();
						generateAlterRenameParsedString();
						commit();
						success_msg(104);
					}

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
			isTable = true;
			DDL_getSymbol(query);
			if(DDL_sym == T_TABLE_NAME || DDL_sym == T_DATABASE_TABLE_NAME || DDL_sym == T_USER_DATABASE_TABLE_NAME){
				const char* table_name = tableName.c_str();
				if(check_exist(table_name)){
					DDL_getSymbol(query);
					if(DDL_sym == T_SEMICOLON && charPos >= query.length() ){
						parsedString = "drop table " + tableName;
						commit();
						queryprocessing(parsedString);
						cout << "Table " << tableName << " is dropped successfully!\n";
					}else{
						error = true;
						displayError("PARSING ERROR: A ';' should be followed after the table name and no other extra character should be after ';'.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: The table "+ tableName +" is not yet created.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid table name should be followed after TABLE");
			}
		}else if(DDL_sym == T_PROCEDURE){
			DDL_getSymbol(query);
			if(DDL_sym == T_IDENT){
				DDL_getSymbol(query);
				if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
					cout << "Procedure " << identName << " is dropped successfully!\n";
				}else{
					error = true;
					displayError("PARSING ERROR: A ';' should be followed after the procedure name and no other extra character should be after ';'.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid procedure name should be followed after PROCEDURE");
			}
		}else if(DDL_sym == T_VIEW){
			DDL_getSymbol(query);
			if(DDL_sym == T_IDENT){
				DDL_getSymbol(query);
				if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
					cout << "View " << identName << " is dropped successfully!\n";
				}else{
					error = true;
					displayError("PARSING ERROR: A ';' should be followed after the view name and no other extra character should be after ';'.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid view name should be followed after VIEW");
			}
		}else if(DDL_sym == T_ALL){
			DDL_getSymbol(query);
			if(DDL_sym == T_TABLE){
				DDL_getSymbol(query);
				if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
					if(check_main_files_exist()){
						queryprocessing("drop all");
						cout << "All tables are dropped successfully!\n";
					}else{
						error = true;
						displayError("PARSING ERROR: There are no table exist.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: A ';' should be followed after the view name and no other extra character should be after ';'.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: TABLE is expected after ALL.");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: TABLE, PROCEDURE or VIEW is expected after DROP.");
		}

	}else if(DDL_sym == T_REPLACE){
		DDL_getSymbol(query);
	    if(DDL_sym == T_PROCEDURE){
			DDL_getSymbol(query);
			createProcedureClause(query);

			/*if(!error){
				cout << "Procedure Name: " << procedureName << "\n";
				for(int i=0;i < (int)paramenterName.size();i++)
					cout << "Parameter: " << paramenterName[i] << " | " << parameterDataType[i] << "\n";

				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "Column: " << columnNameList[i] << "\n";

				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "Values: " << valuesName[i] << "\n";

				cout << "Where Condition: " << whereCond << "\n";
			}	*/

		}else if(DDL_sym == T_VIEW){
			DDL_getSymbol(query);
			createViewClause(query);

			/*if(!error){
				cout << "View Name: " << viewName << "\n";
				cout << "Table Name: " << viewTableName << "\n";
				for(int i=0;i < (int)columnNameList.size();i++)
					cout << "Column: " << columnNameList[i] << "\n";

				cout << "Where Condition: " << selectWhereCond << "\n";
			}*/

		}else{
			error = true;
	    	displayError("PARSING ERROR: PROCEDURE, or VIEW should be followed after REPLACE.");
	    }
	}else if(DDL_sym == T_DESC){
		DDL_getSymbol(query);
		if(DDL_sym == T_IDENT){
			const char* table_name = identName.c_str();
			if(check_exist(table_name)){
				DDL_getSymbol(query);
				if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
					parsedString = "desc " + identName;
					queryprocessing(parsedString);
				}else{
					error = true;
					displayError("PARSING ERROR: A ';' should be followed after the table name and no other extra character should be after ';'.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: The table "+ identName +" is not yet created.");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: A valid table name should be followed after DESC");
		}
	}else if(DDL_sym == T_EXPORT){
		DDL_getSymbol(query);
		if(DDL_sym == T_IDENT){
			const char* table_name = identName.c_str();
			if(check_exist(table_name)){
				DDL_getFileName(query);
				if(DDL_sym == T_FILENAME){
					DDL_getSymbol(query);
					if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
						createExportFile(identName,tempString);
					}else{
						error = true;
						displayError("PARSING ERROR: A ';' should be followed after the table name and no other extra character should be after ';'.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: Invalid filename.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: The table "+ identName +" is not yet created.");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: A valid table name should be followed after EXPORT");
		}
	}else if(DDL_sym == T_IMPORT){
		DDL_getFileName(query);
		if(DDL_sym == T_FILENAME){
			DDL_getSymbol(query);
			if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
				isImport = true;
				parseImportFile(identName,tempString);
				cout << "\nImport successful!\n";
			}else{
				error = true;
				displayError("PARSING ERROR: A ';' should be followed after the table name and no other extra character should be after ';'.");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: Invalid filename.");
		}
	}else if(DDL_sym == T_QUIT){
		DDL_getSymbol(query);
		if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
			commit();
			free_tables();
			queryprocessing("quit");
		}else{
			error = true;
			displayError("PARSING ERROR: ';' is expected after QUIT and no other extra character should be after ';'");
		}
	}else if(DDL_sym == T_SHOWDBS){
		DDL_getSymbol(query);
		if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
			queryprocessing("showdbs");
		}else{
			error = true;
			displayError("PARSING ERROR: ';' is expected after SHOWDBS and no other extra character should be after ';'");
		}
	}else if(DDL_sym == T_COMMIT){
		cout << "Commit successful" << endl;
		commit();
	}else if(DDL_sym == T_ROLLBACK){
		cout << "Rollback successful" << endl;
		rollback();
	}else if(DDL_sym == T_SELECT || DDL_sym == T_UPDATE || DDL_sym == T_DELETE || DDL_sym == T_INSERT){

		string tokens = get_tokens(query, token_lexeme);

		if (tokens != "T_ERROR")
		{
			//for (int a = 0; a < token_lexeme.size(); a++)
			//	cout << token_lexeme[a].first.first << " : " << token_lexeme[a].first.second << " : " << token_lexeme[a].second.first << " : " << token_lexeme[a].second.second << endl << endl;

			if (token_lexeme[0].first.first == T_SELECT)
				checkSelectSyntax(token_lexeme);
			else if (token_lexeme[0].first.first == T_INSERT)
				checkInsertSyntax(token_lexeme);
			else if (token_lexeme[0].first.first == T_DELETE)
				checkDeleteSyntax(token_lexeme);
			else if (token_lexeme[0].first.first == T_UPDATE)
				checkUpdateSyntax(token_lexeme);
			else
				cout << token_lexeme[0].first.second << ": " << "syntax error: expected 'select', 'insert', 'update' or 'delete' at the start of query" << endl;
		}

		tokens = "";
		token_lexeme.clear();		
		//cout << "Count: " << token_lexeme.size() << "\n";

	}else{
		error = true;
		displayError("PARSING ERROR: Invalid start of a query statement.");	

		/*if(DDL_sym == T_NUMBER || DDL_sym == T_DECIMAL){
			isWithArithGroup = false;
			cout << "Answer: " << parseArithmeticExpression(query,"int") << "\n";
		}else if(DDL_sym == T_LPAREN){
			//openParen++;
			//cout << "\nOpenParenSize: " << openParen << "\n";
			isWithArithGroup = true;
			DDL_getSymbol(query);
			cout << "Answer: " << parseArithmeticExpression(query,"int") << "\n";
			//DDL_getSymbol(query);
			if(DDL_sym == T_RPAREN){
				cout << "Answer: " << "\n";
				//openParen--;
				//cout << "\nOpenParenSize: " << openParen << "\n";
			}
		}*/
	}

}
