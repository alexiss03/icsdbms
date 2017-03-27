void multipleTables(std::string query){
	while(sym == comma && error == false){
		getSymbol(query);		//comma separater column names
		if(sym == table_column){
			getSymbol(query);
		}
		else{
			displayError("Error near ','.");
			error = true;
		}
	}
}

void singeTable(std::string query){
	while(sym == comma && error == false){
		getSymbol(query);		//comma separater column names
		if(sym == ident){
			getSymbol(query);
		}
		else{
			displayError("Error near ','.");
			error = true;
		}
	}
}

void orderByClause(std::string query){
	getSymbol(query);
	if(multipleTablesUsed == true){		//if columns are in multiple table access format
		if(sym == table_column){
			getSymbol(query);
			multipleTables(query);
			if(sym == semicolon && error == false){
				std::cout << "Query accepted!\n";
			}
			else if(sym == asc){
				getSymbol(query);
				if(sym == semicolon){
					std::cout << "Query accepted!\n";
				}
				else{
					displayError("Error near column name.");
				}
			}
			else if(sym == desc){
				getSymbol(query);
				if(sym == semicolon){
					std::cout << "Query accepted!\n";
				}
				else{
					displayError("Error near column name.");
				}
			}
			else{
				displayError("Error near 'column name'.");
			}
		}
		else if(sym == ident){		//if columns are in single table access format
			displayError("Error near column name.");
		}
		else{		//if single table is manipulated
			displayError("Error near 'by'.");
		}
	}
	else{
		if(sym == ident){
			getSymbol(query);
			singeTable(query);
			if(sym == semicolon && error == false){
				std::cout << "Query accepted!\n";
			}
			else if(sym == asc){
				getSymbol(query);
				if(sym == semicolon){
					std::cout << "Query accepted!\n";
				}
				else{
					displayError("Error near column name.");
				}
			}
			else if(sym == desc){
				getSymbol(query);
				if(sym == semicolon){
					std::cout << "Query accepted!\n";
				}
				else{
					displayError("Error near column name.");
				}
			}
			else{
				displayError("Error near 'column name'.");
			}
		}
		else if(sym == table_column){		//if columns are in single table access format
			displayError("Error near column name.");
		}
		else{		//if single table is manipulated
			displayError("Error near 'by'.");
		}
	}

}

void groupByClause(std::string query){
	getSymbol(query);
	if(multipleTablesUsed == true){		//if columns are in multiple table access format
		if(sym == table_column){
			getSymbol(query);
			multipleTables(query);
			if(sym == semicolon && error == false){
				std::cout << "Query accepted!\n";
			}
			else if(sym == order){
				getSymbol(query);
				if(sym == by){
					orderByClause(query);
				}
				else{
					displayError("Error near 'order'.");
				}
			}
			else{
				displayError("Error near 'column name'.");
			}
		}
		else if(sym == ident){		//if columns are in single table access format
			displayError("Error near column name.");
		}
		else{		//if single table is manipulated
			displayError("Error near 'by'.");
		}
	}
	else{
		if(sym == ident){
			getSymbol(query);
			singeTable(query);
			if(sym == semicolon && error == false){
				std::cout << "Query accepted!\n";
			}
			else if(sym == order){
				getSymbol(query);
				if(sym == by){
					orderByClause(query);
				}
				else{
					displayError("Error near 'order'.");
				}
			}
			else{
				displayError("Error near 'column name'.");
			}
		}
		else if(sym == table_column){		//if columns are in single table access format
			displayError("Error near column name.");
		}
		else{		//if single table is manipulated
			displayError("Error near 'by'.");
		}
	}
}

void fromClause(std::string query){
	getSymbol(query);
	if(sym == ident){		//select [distinct | all] [column_name {, column_name} | *] from table_name
		getSymbol(query);
		if(sym == ident){			//select [distinct | all] [column_name {, column_name} | *] from table_name as
			getSymbol(query);
			if(sym == semicolon){		//select [distinct | all] [column_name {, column_name} | *] from table_name [as alias];
				std::cout << "Query accepted!\n";
			}
			else if(sym == resLeft || sym == resRight || sym == full){
				getSymbol(query);
				if(sym == outer){
					getSymbol(query);
				}

				if(sym == join){
					getSymbol(query);
					if(sym == ident){
						getSymbol(query);
						if(sym == ident){
							getSymbol(query);
							printf("to be continued...");
						}
						else{
							displayError("Error near table name.");
						}
					}
					else{
						displayError("Error near 'join'.");
					}
				}
				else{
					displayError("Error near join type.");
				}
			}
			else if(sym == group){
				getSymbol(query);
				if(sym == by){
					groupByClause(query);
				}
				else{
					displayError("Error near 'group'.");
				}
			}
			else if(sym == order){
				getSymbol(query);
				if(sym == by){
					orderByClause(query);
				}
				else{
					displayError("Error near 'order'.");
				}
			}			
			else{
				displayError("Error near table alias.");	
			}						
		}
		else if(sym == semicolon){		//select [distinct | all] [column_name {, column_name} | *] from table_name [as alias];
			std::cout << "Query accepted!\n";
		}
		else if(sym == group){
			getSymbol(query);
			if(sym == by){
				groupByClause(query);
			}
			else{
				displayError("Error near 'group'.");
			}
		}
		else if(sym == order){
			getSymbol(query);
			if(sym == by){
				orderByClause(query);
			}
			else{
				displayError("Error near 'order'.");
			}
		}		
		else{
			displayError("Error near table name.");	
		}
	}
	else{
		displayError("Error near 'from'.");	
	}
}

void multipleTableColumns(std::string query){
	while(sym == comma && error == false){		//select [distinct | all] column_name {, column_name}
		getSymbol(query);
		if(sym == table_column){
			getSymbol(query);
			if(sym == as){
				getSymbol(query);
				if(sym == ident){
					getSymbol(query);
				}
				else{
					error = true;
					displayError("Error near 'as'.");
				}
			}
		}
		else if(sym == table_times){
			getSymbol(query);
		}
		else{
			displayError("Error near ','.");		
		}
	}

	if(sym == from && error == false){		//select [distinct | all] column_name {, column_name} from
		multipleTablesUsed = true;
		fromClause(query);
	}
	else{
		if(sym != from){
			displayError("Error near column name.");
		}
	}
}

void multipleColumns(std::string query){
	while(sym == comma && error == false){		//select [distinct | all] column_name {, column_name}
		getSymbol(query);
		if(sym == ident){
			getSymbol(query);
			if(sym == as){
				getSymbol(query);
				if(sym == ident){
					getSymbol(query);
				}
				else{
					error = true;
					displayError("Error near 'as'.");
				}
			}
		}
		else{
		        error = true;
			displayError("Error near ','.");		
		}
	}

	if(sym == from && error == false){		//select [distinct | all] column_name {, column_name} from
		fromClause(query);
	}
	else{
		if(sym != from){
			displayError("Error near column name.");
		}
	}
}

void selectClause(std::string query){
	getSymbol(query);
	/*
	if(sym == distinct){	//select distinct
		getSymbol(query);
	}
	else if(sym == all){	//select all
		getSymbol(query);
	}
	else;		//select
	*/
	if(sym == times){		//select [distinct | all] *
		getSymbol(query);
		if(sym == from){		//select [distinct | all] * from
			fromClause(query);
		}
		else{
			displayError("Error near '*'.");
		}
	}
	else if(sym == ident){		//select [distinct | all] column_name
		getSymbol(query);
		if(sym == as){
			getSymbol(query);
			if(sym == ident){
				getSymbol(query);
				multipleColumns(query);
			}
			else{
				displayError("Error near 'as'.");
			}
		}
		else{
			multipleColumns(query);
		}
	}
	else if(sym == table_times){
		getSymbol(query);
		multipleTablesUsed = true;
		multipleTableColumns(query);
	}
	else if(sym == table_column){
		getSymbol(query);
		if(sym == as){
			getSymbol(query);
			if(sym == ident){
				getSymbol(query);
				multipleTableColumns(query);
			}
			else{
				displayError("Error near 'as'.");
			}
		}
		else{
			multipleTableColumns(query);
		}
	}
	else{
		displayError("Error near 'select'");
	}
}
