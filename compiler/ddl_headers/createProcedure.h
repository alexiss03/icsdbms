void createProcedureClause(std::string query);
void multipleProcedureParameter(std::string query);
void simpleInsertQueryClause(std::string query);
void simpleDeleteQueryClause(std::string query);
void multipleInsertColumn(std::string query);
void multipleInsertParameter(std::string query);
void simpleUpdateQueryClause(std::string query);
void multipleUpdateColumn(std::string query);
void whereConditionClause(std::string query);
std::string retrieveParameterDataType(std::string column);


bool isMultipleProcedureParameter = false;
bool isMultipleInsertColumn = false;
bool isMultipleInsertParameter = false;
bool isMultipleUpdateColumn = false;
std::string procedureTableName = "";
std::string procedureName = "";
std::string whereCond = "";

int insertColumn = 0;
int insertParameter = 0;

std::vector<string> paramenterName;
std::vector<string> parameterDataType;
std::vector<string> valuesName;

std::string retrieveParameterDataType(std::string column){
	for(int i=0;i<(int)paramenterName.size();i++){
		if(column == paramenterName[i])
			return parameterDataType[i];
	}
	return "NULL";
}

void createProcedureClause(std::string query){
	if(DDL_sym == T_IDENT){
		procedureName = identName;
		DDL_getSymbol(query);
		if(DDL_sym == T_LPAREN){
			DDL_getSymbol(query);
			multipleProcedureParameter(query);
		}else{
			error = true;
			displayError("PARSING ERROR: '(' is expected after procedure name.");
		}
	}else{
		error = true;
		displayError("PARSING ERROR: Invalid procedure name format.");
	}
}

void multipleProcedureParameter(std::string query){
	if(DDL_sym == T_IDENT){
		do{
			if(isMultipleProcedureParameter && !error){
				DDL_getSymbol(query);
			}

			if(DDL_sym == T_IDENT){
				if(std::find(paramenterName.begin(), paramenterName.end(), identName) != paramenterName.end()) {
					displayError("PARSING ERROR: Parameter " +identName+ " is declared twice");
					error = true;
				}else{
					paramenterName.push_back(identName);
					DDL_getSymbol(query);
					if((DDL_sym == T_DATE || DDL_sym == T_FLOAT || DDL_sym == T_INT || DDL_sym == T_RES_BOOLEAN || DDL_sym == T_CHARAC) && !error){
						parameterDataType.push_back(getReserveWord(DDL_sym));
						DDL_getSymbol(query);
					}else if(DDL_sym == T_VARCHAR && !error){
						parameterDataType.push_back(getReserveWord(DDL_sym));
						DDL_getSymbol(query);
					}else if(!error){
						error = true;
						displayError("PARSING ERROR: A valid data type should be followed by the paramenter name " +paramenterName.back());
					}
				}
			}else{
				error = true;
				displayError("PARSING ERROR: A valid paramenter name is expected after ','");
			}

			isMultipleProcedureParameter = true;

		}while(DDL_sym == T_COMMA && !error);

		if(DDL_sym == T_RPAREN && !error){
			DDL_getSymbol(query);
			if(DDL_sym == T_BEGIN){
				DDL_getSymbol(query);
				if(DDL_sym == T_INSERT){
					DDL_getSymbol(query);
					simpleInsertQueryClause(query);
				}else if(DDL_sym == T_UPDATE){
					DDL_getSymbol(query);
					simpleUpdateQueryClause(query);
				}else if(DDL_sym == T_DELETE){
					DDL_getSymbol(query);
					simpleDeleteQueryClause(query);
				}else{
					error = true;
					displayError("PARSING ERROR: INSERT, DELETE, UPDATE is expected after the BEGIN.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: BEGIN is expect after the paramenter list.");
			}	
		}else{
			error = true;
			displayError("PARSING ERROR: '(' is expected at the end of the parameter list.");
		}

	}else{
		error = true;
		displayError("PARSING ERROR: A valid parameter name is expected after '('");
	}
}

void simpleDeleteQueryClause(std::string query){
	if(DDL_sym == T_FROM){
		DDL_getSymbol(query);
		if(DDL_sym == T_IDENT){
			if(check_exist(identName.c_str())){
				procedureTableName = identName;
				DDL_getSymbol(query);
				if(DDL_sym == T_WHERE && !error){
					DDL_getSymbol(query);
					whereConditionClause(query);
					if(DDL_sym == T_END && !error){
						DDL_getSymbol(query);
						if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
							cout << "Procedure created!\n";
						}else{
							error = true;
							displayError("PARSING ERROR: A ';' is expected after WHERE condition and no other extra character should be after ';'.");
						}
					}else if(!error){
						error = true;
						displayError("PARSING ERROR: END is expected after the values.");
					}
				}else if(DDL_sym == T_END && !error){
					DDL_getSymbol(query);
					if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
						cout << "Procedure created!\n";
					}else{
						error = true;
						displayError("PARSING ERROR: A ';' is expected after END and no other extra character should be after ';'.");
					}
				}else if(!error){
					error = true;
					displayError("PARSING ERROR: WHERE or END is expected after the values.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: The table "+ identName +" is not yet created.");
			}
		}
	}else{
		error = true;
		displayError("PARSING ERROR: FROM is expected after DELETE");
	}
}

void simpleUpdateQueryClause(std::string query){
	if(DDL_sym == T_IDENT){
		if(check_exist(identName.c_str())){
			procedureTableName = identName;
			DDL_getSymbol(query);
			if(DDL_sym == T_SET){
				DDL_getSymbol(query);
				multipleUpdateColumn(query);
				if(DDL_sym == T_WHERE && !error){
					DDL_getSymbol(query);
					whereConditionClause(query);
					if(DDL_sym == T_END && !error){
						DDL_getSymbol(query);
						if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
							cout << "Procedure created!\n";
						}else{
							error = true;
							displayError("PARSING ERROR: A ';' is expected after WHERE condition and no other extra character should be after ';'.");
						}
					}else if(!error){
						error = true;
						displayError("PARSING ERROR: END is expected after the values.");
					}
				}else if(DDL_sym == T_END && !error){
					DDL_getSymbol(query);
					if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
						cout << "Procedure created!\n";
					}else{
						error = true;
						displayError("PARSING ERROR: A ';' is expected after END and no other extra character should be after ';'.");
					}
				}else if(!error){
					error = true;
					displayError("PARSING ERROR: WHERE or END is expected after the values.");
				}
			}else{
				error = true;
				displayError("PARSING ERROR: SET is expected after the table name " +procedureTableName);
			}
		}else{
			error = true;
			displayError("PARSING ERROR: The table "+ identName +" is not yet created.");
		}

	}
}

void multipleUpdateColumn(std::string query){
	if(DDL_sym == T_IDENT){
		do{
			if(isMultipleUpdateColumn && !error){
				DDL_getSymbol(query);
			}

			if(DDL_sym == T_IDENT){
				if(check_column_name(procedureTableName.c_str(), identName.c_str())){
					if(std::find(columnNameList.begin(), columnNameList.end(), identName) != columnNameList.end()) {
						displayError("PARSING ERROR: Column " +identName+ " is declared twice");
						error = true;
					}else{
						columnNameList.push_back(identName);
						DDL_getSymbol(query);
						if(DDL_sym == T_EQL){
							DDL_getSymbol(query);
							if(DDL_sym == T_IDENT){
								if(std::find(paramenterName.begin(), paramenterName.end(), identName) != paramenterName.end()) {
									if(check_data_type(procedureTableName.c_str(),columnNameList.back(),retrieveParameterDataType(identName))){
										valuesName.push_back(identName);
										DDL_getSymbol(query);
									}else{
										error = true;
										displayError("PARSING ERROR: Column " +columnNameList.back()+ " and column " +identName+ " are not type compatible.");
									}
								}else{
									displayError("PARSING ERROR: Value " +identName+ " is not declared in the parameter list");
									error = true;
								}
							}else{
								error = true;
								displayError("PARSING ERROR: A valid parameter format is expected after '='");
							}
						}else{
							error =true;
							displayError("PARSING ERROR: '=' is expected after the column name " +identName);
						}
					}
				}else{
					error = true;
					displayError("PARSING ERROR: Column " + identName + " is not declared in table " + procedureTableName );
				}
				
			}else{
				error = true;
				displayError("PARSING ERROR: A valid column name format is expected after ','");
			}

			isMultipleUpdateColumn = true;
		}while(DDL_sym == T_COMMA && !error);

	}else{
		error = true;
		displayError("PARSING ERROR: A valid column name format is expected after '('.");
	}
}

void whereConditionClause(std::string query){
	if(DDL_sym == T_IDENT){
		if(check_column_name(procedureTableName.c_str(), identName.c_str())){
			whereCond = identName + " ";
			std::string tempColumnName = identName;
			DDL_getSymbol(query);
			if(DDL_sym == T_NEQ || DDL_sym == T_EQL){
				whereCond = whereCond + getReserveCharacter(DDL_sym) + " ";
				DDL_getSymbol(query);
				if(DDL_sym == T_IDENT){
					if(std::find(paramenterName.begin(), paramenterName.end(), identName) != paramenterName.end()) {
						if(check_data_type(procedureTableName.c_str(),tempColumnName,retrieveParameterDataType(identName))){
							whereCond = whereCond + identName;
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR: Column " +identName+ " and column " +tempColumnName+ " are not type compatible");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: Column " +identName+ " is not declare in the parameter list");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: A valid column name format is expected after the operator.");
				}
			}else{
				error = true;
			displayError("PARSING ERROR: A correct operator should be followed after the " + whereCond);
			}
		}else{
			error = true;
			displayError("PARSING ERROR: Column " + identName + " is not declared in table " + procedureTableName );
		}
	}else{
		error = true;
		displayError("PARSING ERROR: A valid column name format is expected after WHERE.");
	}
}

void simpleInsertQueryClause(std::string query){
	if(DDL_sym == T_INTO){
		DDL_getSymbol(query);
		if(DDL_sym == T_IDENT){
			if(check_exist(identName.c_str())){
				procedureTableName = identName;
				DDL_getSymbol(query);
				if(DDL_sym == T_LPAREN){
					DDL_getSymbol(query);
					multipleInsertColumn(query);
					if(DDL_sym == T_RPAREN && !error){
						DDL_getSymbol(query);
						if(DDL_sym == T_VALUES){
							DDL_getSymbol(query);
							if(DDL_sym == T_LPAREN){
								DDL_getSymbol(query);
								multipleInsertParameter(query);
								if(DDL_sym == T_RPAREN && !error){
									DDL_getSymbol(query);
									if(DDL_sym == T_END && !error){
										DDL_getSymbol(query);
										if(DDL_sym == T_SEMICOLON && charPos >= query.length()){
											cout << "Procedure created!\n";
										}else{
											error = true;
											displayError("PARSING ERROR: ';' is expected after END and no other extra character should be after ';'");
										}
									}else if(!error){
										error = true;
										displayError("PARSING ERROR: END is expected after the WHERE condition.");
									}
								}else if (!error){
									error = true;
									displayError("PARSING ERROR: ')' is expected after the value list" );	
								}
							}else{
								error = true;
								displayError("PARSING ERROR: '(' is expected after VALUES" );
							}
						}else{
							error = true;
							displayError("PARSING ERROR: VALUES is expected after the column name list ");
						}
					}else if(!error){
						error =true;
						displayError("PARSING ERROR: ')' is expected after the column name list" );	
					}
				}else{
					error = true;
					displayError("PARSING ERROR: '(' is expected after the table name" );
				}
			}else{
				error = true;
				displayError("PARSING ERROR: The table "+ identName +" is not yet created.");
			}
		}else{
			error = true;
			displayError("PARSING ERROR: Valid table name format is expected after INTO");
		}
	}else{
		error = true;
		displayError("PARSING ERROR: INTO is expected after INSERT.");
	}
}

void multipleInsertColumn(std::string query){
	if(DDL_sym == T_IDENT){
		do{
			if(isMultipleInsertColumn && !error){
				DDL_getSymbol(query);
			}

			if(DDL_sym == T_IDENT){
				if(check_column_name(procedureTableName.c_str(), identName.c_str())){
					if(std::find(columnNameList.begin(), columnNameList.end(), identName) != columnNameList.end()) {
						displayError("PARSING ERROR: Column " +identName+ " is declared twice");
						error = true;
					}else{
						columnNameList.push_back(identName);
						DDL_getSymbol(query);
					}
				}else{
					error = true;
					displayError("PARSING ERROR: Column " + identName + " is not declared in table " + procedureTableName );
				}
				
			}else{
				error = true;
				displayError("PARSING ERROR: A valid column name is expected after ','");
			}

			isMultipleInsertColumn = true;
			insertColumn++;
		}while(DDL_sym == T_COMMA && !error);

	}else{
		error = true;
		displayError("PARSING ERROR: A valid column name format is expected after '('.");
	}
}

void multipleInsertParameter(std::string query){
	if(DDL_sym == T_IDENT){
		do{
			if(isMultipleInsertParameter && !error){
				DDL_getSymbol(query);
			}

			if(DDL_sym == T_IDENT){
				if(std::find(paramenterName.begin(), paramenterName.end(), identName) != paramenterName.end()) {
					if(insertParameter <= insertColumn){
						if(check_data_type(procedureTableName.c_str(),columnNameList[insertParameter],retrieveParameterDataType(identName))){
							valuesName.push_back(identName);
							DDL_getSymbol(query);
						}else{
							error = true;
							displayError("PARSING ERROR: Column " +columnNameList[insertParameter]+ " and column " +identName+ " are not type compatible.");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: Number of values should not exceed the number of columns.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: Value " +identName+ " is not declare in the parameter list");
				}	
			}else{
				error = true;
				displayError("PARSING ERROR: A valid value is expected after ','");
			}
			isMultipleInsertParameter = true;
			insertParameter++;
			
		}while(DDL_sym == T_COMMA && !error);

		if(insertParameter != insertColumn && !error){
			error = true;
			displayError("PARSING ERROR: Number of values to be insert is not equal to the number of columns.");
		}
		
	}else{
		error = true;
		displayError("PARSING ERROR: A valid valid format is expected after '('.");
	}
}