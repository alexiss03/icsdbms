using namespace std;
std::vector<string> listFiles( const char* path, vector<string> files ){	//gets the list of files in the main folder
	DIR* dirFile = opendir( path );
	string str3;
	std::string str;
	std::vector <string> names;

   const char* str2 = str.c_str();
	if ( dirFile ){
      struct dirent* hFile;
      while (( hFile = readdir( dirFile )) != NULL ){
		
         if ( !strcmp( hFile->d_name, "."  )) continue;
         if ( !strcmp( hFile->d_name, ".." )) continue;
		 if ( strstr( hFile->d_name, ".dat" )){
			str3 = hFile->d_name;
			int length=strlen(str3.c_str());			
			str3.erase (length-4, length);			
			files.push_back(str3);
            }
      } 
      closedir( dirFile );
   }
	
	return files;
}

void print_table_names(vector<string> &files){						//prints all the table names 
	int j=0;
	
	
	std::sort(files.begin(),files.end());
	
	cout << "Table Names" <<endl;	
	for(int i=0;i<files.size();i++){
		cout << files[i] << endl;
	}	
		cout << "========================================" << endl;
		if(files.size()==1){
		cout << (files.size()) << " Table" << endl;
		}
		else cout << (files.size()) << " Tables" << endl; 
}//end of print_table_names


