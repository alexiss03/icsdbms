void createExportFile(std::string table_name, std::string filname);
void parseImportFile(std::string table_name, std::string filname);

void createExportFile(std::string table_name,std::string filname){
	isExport = true;

	std::vector<string> rawTableInfo;
	std::vector< vector<std::string> > tableInfo;
	std::vector< pair< std::string, std::string > > tableValues;
	TABLEDATALIST ** tabledatalist;
	std::vector<string> insertQueries;
	std::string createTableString;
	std::string insertQueryString;
	std::string insertString;

	rawTableInfo = show_table_contents(table_name.c_str());
	tableInfo = print_table_properties2(rawTableInfo);

	tabledatalist = get_table(table_name);
	tableValues = print_tabledatalist(tabledatalist);

	int noCol = no_of_columns(tabledatalist);

	createTableString = "CREATE TABLE " + table_name + "(\n";

	for(int i=0;i<tableInfo[0].size();i++){
		for(int j=0;j<7;j++){
			
			if(j == 0){
				createTableString = createTableString + tableInfo[j][i] + " ";
			}else if(j == 1){
				createTableString = createTableString + tableInfo[j][i] + " ";
			}else if(j == 2){
				if(tableInfo[j][i] == "True")
					createTableString = createTableString + "not null";
				else
					createTableString = createTableString + "null";
			}else if(j == 4){
				if(tableInfo[j][i] == "NULL" && tableInfo[j-1][i] == "True"){
					createTableString = createTableString + " primary key,\n";
				}else if(tableInfo[j][i] != "NULL" && tableInfo[j-1][i] == "True"){
					createTableString = createTableString + " constraint " + tableInfo[j][i] + " primary key,\n";
				}else if(tableInfo[j][i] == "NULL" && tableInfo[j-1][i] == "False"){
					createTableString = createTableString + ",\n";
				}	
			}else if(j == 6){
				if(tableInfo[j][i] == "NULL" && tableInfo[j-1][i] == "True"){
					createTableString = createTableString + "unique("+tableInfo[0][i]+"),\n";
				}else if(tableInfo[j][i] != "NULL" && tableInfo[j-1][i] == "True"){
					createTableString = createTableString + "constraint " + tableInfo[j][i] + " unique("+tableInfo[0][i]+"),\n";
				}/*else if(tableInfo[j][i] == "NULL" && tableInfo[j-1][i] == "False"){
					createTableString = createTableString + ",\n";
				}	*/
			}

			//cout << tableInfo[j][i] << " ";
		}
		//cout << "\n";
	}
	createTableString[(int)createTableString.length()-2] = ')';
	createTableString[(int)createTableString.length()-1] = ';';
	//cout << createTableString << "\n";
	//cout << "\n";

	insertString = "INSERT INTO "+table_name+"(";
	
	for(int i=0;i<noCol;i++){
		
		if(i+1 != noCol)
			insertString = insertString + tableValues[i].second + ", ";
		else
			insertString = insertString + tableValues[i].second + ") VALUES(";
	}
	
	insertQueryString = insertString;
	
	for(int i=noCol;i<(int)tableValues.size();i++){
		//cout << tableValues[i].second << " ";
		
		if(i%noCol != noCol-1){
			if(tableValues[i].first == "string"){
				insertQueryString = insertQueryString + "'" + tableValues[i].second + "', ";
			}else if(tableValues[i].first == "float"){
				float tempValue = atof(tableValues[i].second.c_str());
				insertQueryString = insertQueryString + convertFloatToString(tempValue) + ", ";
			}	
			else{
				insertQueryString = insertQueryString + tableValues[i].second + ", ";
			}
		}else{
			if(tableValues[i].first == "string"){
				insertQueryString = insertQueryString + "'" + tableValues[i].second + "');";
			}else if(tableValues[i].first == "float"){
				float tempValue = atof(tableValues[i].second.c_str());
				insertQueryString = insertQueryString + convertFloatToString(tempValue) + ");";
			}	
			else{
				insertQueryString = insertQueryString + tableValues[i].second + ");";
			}

			//cout << insertQueryString << "\n";
			insertQueries.push_back(insertQueryString);
			insertQueryString = insertString;

		}
			//cout << "\n";
	}
	char fName[300];
	fName[0] = '\0';
	strcat(fName,"export/"); 
	strcat(fName,filname.c_str());
	std::ofstream output_file(fName);
	output_file << createTableString << "\n\n";
	std::ostream_iterator<std::string> output_iterator(output_file, "\n");
	std::copy(insertQueries.begin(), insertQueries.end(), output_iterator);
	output_file.close();
}

void parseImportFile(std::string table_name, std::string filname){
	charPos = 0;
	string query, contQuery;
	
	char fName[300];
	fName[0] = '\0';
	
	vector<pair<pair<int, string>, pair<string, string> > > token_lexeme;
	std::string parsedString;

	strcat(fName,"import/"); 
	strcat(fName,filname.c_str());
	
	ifstream input_file (fName);

	if(input_file.is_open()){
	  	
	  	while(getline(input_file,query) && !error){
	  		
	  		while(*query.rbegin() != ';') //loop until T_SEMICOLON is at the end of the query
			{
				getline(input_file, contQuery);	//accepts the continuation of the query 
				query.append(" ");
				query.append(contQuery);
			}
			
			charPos = 0;
			if(!error){
				//cout << query << "\n";
				DDL_getSymbol(query);
				parseQuery(query);
			}
	  	}
		
		input_file.close();
	}else{
  	error = true;
  	displayError("IMPORTING ERROR: Unable to open file.");
  }

}