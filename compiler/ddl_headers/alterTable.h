void multipleAlteration(std::string query);
void generateAlterAddParsedString();
void generateAlterRenameParsedString();
void generateAlterDropParsedString();

bool isMultipleAlteration = false;

std::vector<std::string> columnToBeDrop;
std::vector<std::string> constraintToBeDrop;
std::vector<std::string> columnToBeRename;
std::vector<std::string> constraintToBeRename;
std::vector<std::string> columnNew;
std::vector<std::string> constraintNew;

void generateAlterAddParsedString(){

	for(int i=0;i<(int)columnNameList.size();i++){
		if(queryActionColumn[i] == "add"){
			int noOfToken = 0;
			std::string parsedString;
			std::size_t found = columnNameDataType[i].find("varchar");
			
			if(found!=std::string::npos){
				noOfToken++;
			}

			if(columnNameNotNull[i] == "Not Null"){
				parsedString = parsedString + "_nn ";
				noOfToken++;
			}

			for(int j=0;j < (int)constraintColumnName.size();j++){
				if(columnNameList[i] == constraintColumnName[j] && (constraintType[j] == "_pk" || constraintType[j] == "_uk")){
					parsedString = parsedString + constraintType[j] + " "; 
					noOfToken++;
					if(constraintName[j] != "NULL"){
						parsedString = parsedString + constraintName[j] + " ";
						noOfToken++;
					}
				}
			}
			noOfToken = noOfToken + 3;
			parsedString = "alter " + tableName + " add "+ convertIntToString(noOfToken) +" " + columnNameList[i] + " " + columnNameDataType[i] + " "+ parsedString +"|";
			//cout << "\nParsed String: " << parsedString << "\n";
			add_column(tableName,columnNameList[i],columnNameDataType[i]);
			queryprocessing(parsedString);
		}
	}

}

void generateAlterDropParsedString(){

	for(int i=0;i<(int)columnToBeDrop.size();i++){
		std::string parsedString;

		std::vector<std::string> values;
		
		values=show_table_contents(tableName.c_str());
		values=show_table_names(values);

		if(values.size() == 1){
			parsedString = "drop table " + tableName;
			//cout << parsedString << "\n";
			queryprocessing(parsedString);
		}else{
			parsedString = "alter " + tableName + " drop " + columnToBeDrop[i];
			//cout << "\nParsed String: " << parsedString << "\n";
			drop_column(tableName,columnToBeDrop[i]);
			queryprocessing(parsedString);
		}
	}
}

void generateAlterRenameParsedString(){

	for(int i=0;i<(int)columnToBeRename.size();i++){
		std::string parsedString = "alter " + tableName + " rename " + columnToBeRename[i] + " " + columnNew[i];
		//cout << "\nParsed String: " << parsedString << "\n";
		queryprocessing(parsedString);
	}
}

void multipleAlteration(std::string query){
	DDL_getSymbol(query);
	if(DDL_sym == T_ADD || DDL_sym == T_DROP || DDL_sym == T_RES_RENAME || DDL_sym == T_MODIFY){
		do{
			if(isMultipleAlteration && !error){
				DDL_getSymbol(query);
			}

			if(DDL_sym == T_ADD){
				isToAdd = true;
				DDL_getSymbol(query);
				columnNameClause(query);
			}else if(DDL_sym == T_DROP){
				DDL_getSymbol(query);
				if(DDL_sym == T_IDENT){
					const char* table_name = tableName.c_str();
					const char* column_name = identName.c_str();
					if(check_column_name(table_name, column_name)){
						if(std::find(columnToBeDrop.begin(), columnToBeDrop.end(), identName) != columnToBeDrop.end()){
							error = true;
							displayError("PARSING ERROR: The column " +identName+ " in table " +tableName+ " is already declared to be drop.");
						}else{
							columnToBeDrop.push_back(identName);
							DDL_getSymbol(query);
						}
					}else{
						error = true;
						displayError("PARSING ERROR: The column " +identName+ " in table " +tableName+ " is not declared.");
					}
					//columnToBeDrop.push_back(identName);
				}else  if(DDL_sym == T_CONSTRAINT){
					DDL_getSymbol(query);
					if(DDL_sym == T_IDENT){
						const char* table_name = tableName.c_str();
						const char* column_name = identName.c_str();
						if(check_constraint_name(table_name,column_name)){
							if(std::find(constraintToBeDrop.begin(), constraintToBeDrop.end(), identName) != constraintToBeDrop.end()){
								error = true;
								displayError("PARSING ERROR: The constraint " +identName+ " in table " +tableName+ " is already declared to be drop.");
							}else{
								constraintToBeDrop.push_back(identName);
								DDL_getSymbol(query);
							}
						}else{
							error = true;
							displayError("PARSING ERROR: The constraint " +identName+ " in table " +tableName+ " is not declared.");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: CONSTRAINT should be followed by a valid column name format.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: DROP should be followed by CONSTRAINT or a a existing column name.");
				}
			}else if(DDL_sym == T_RES_RENAME){
				DDL_getSymbol(query);
				if(DDL_sym == T_IDENT){
					const char* table_name = tableName.c_str();
					const char* column_name = identName.c_str();
					if(check_column_name(table_name, column_name)){
						if(std::find(columnToBeRename.begin(), columnToBeRename.end(), identName) != columnToBeRename.end()){
							error = true;
							displayError("PARSING ERROR: The column " +identName+ " in table " +tableName+ " is already declared to be rename.");
						}else{
							if(std::find(columnToBeDrop.begin(), columnToBeDrop.end(), identName) != columnToBeDrop.end()){
								error = true;
								displayError("PARSING ERROR: The column " +identName+ " in table " +tableName+ " is already declared to be drop.");
							}else{
								columnToBeRename.push_back(identName);
								DDL_getSymbol(query);
								if(DDL_sym == T_TO){
									DDL_getSymbol(query);
									if(DDL_sym == T_IDENT){
										const char* table_name = tableName.c_str();
										const char* column_name = identName.c_str();
										if(check_column_name(table_name, column_name) || (std::find(columnNew.begin(), columnNew.end(), identName) != columnNew.end())){
											error = true;
											displayError("PARSING ERROR: The column " +identName+ " in table " +tableName+ " is already declared.");
										}else{
											columnNew.push_back(identName);
											DDL_getSymbol(query);
										}
									}else{
										error = true;
										displayError("PARSING ERROR: Invalid column name format.");
									}
								}else{
									error = true;
									displayError("PARSING ERROR: TO is expected after the column name to be rename");
								}
							}
						}
					}else{
						error = true;
						displayError("PARSING ERROR: The column " +identName+ " in table " +tableName+ " is not declared.");
					}
					
				}else if(DDL_sym == T_CONSTRAINT){
					DDL_getSymbol(query);
					if(DDL_sym == T_IDENT){
						const char* table_name = tableName.c_str();
						const char* column_name = identName.c_str();
						if(check_constraint_name(table_name,column_name)){
							if(std::find(constraintToBeRename.begin(), constraintToBeRename.end(), identName) != constraintToBeRename.end()){
								error = true;
								displayError("PARSING ERROR: The constraint " +identName+ " in table " +tableName+ " is already declared to be rename.");
							}else{
								if(std::find(columnToBeDrop.begin(), columnToBeDrop.end(), identName) != columnToBeDrop.end()){
									error = true;
									displayError("PARSING ERROR: The column " +identName+ " in table " +tableName+ " is already declared to be drop.");
								}else{
									constraintToBeRename.push_back(identName);
									DDL_getSymbol(query);
									if(DDL_sym == T_TO){
										DDL_getSymbol(query);
										if(DDL_sym == T_IDENT){
											const char* table_name = tableName.c_str();
											const char* column_name = identName.c_str();
											if(check_constraint_name(table_name,column_name) || (std::find(constraintNew.begin(), constraintNew.end(), identName) != constraintNew.end())){
												error = true;
												displayError("PARSING ERROR: The constraint " +identName+ " in table " +tableName+ " is already declared.");
											}else{
												constraintNew.push_back(identName);
												DDL_getSymbol(query);
											}
										}else{
											error = true;
											displayError("PARSING ERROR: Invalid column name format.");
										}
									}else{
										error = true;
										displayError("PARSING ERROR: TO is expected after the column name to be rename");
									}
								}
							}
						}else{
							error = true;
							displayError("PARSING ERROR: The constraint " +identName+ " in table " +tableName+ " is not declared.");
						}
					}else{
						error = true;
						displayError("PARSING ERROR: CONSTRAINT should be followed by a valid column name format.");
					}
				}else{
					error = true;
					displayError("PARSING ERROR: RENAME should be followed by a CONSTRAINT or valid column name format.");
				}
			}else if(DDL_sym == T_MODIFY){
				isToModify = true;
				DDL_getSymbol(query);
				columnNameClause(query);
			}else{
				error = true;
				displayError("PARSING ERROR: ADD, RENAME, DROP or MODIFY command should be followed after every ','");
			}
			
			isMultipleAlteration = true;
			isToModify = false;
			isToAdd = false;
		}while(DDL_sym == T_COMMA && !error);

		if(DDL_sym == T_SEMICOLON && charPos >= query.length() && !error){
			std::cout << "Table Altered!\n";
		}else if(!error){
			error = true;
			displayError("PARSING ERROR: Error near column to be alter.");
		}

	}else{
		error = true;
		displayError("PARSING ERROR: ADD, RENAME, DROP or MODIFY command should be followed after the table name.");
	}

}