using namespace std;
void table_data(const char* name, int num, vector<string> &words, int type){	//inserts the credentials of the table to file .stdat
	char filename[1000];	
	filename[0]='\0';
	
	
	if(type==1){	
	strcat(filename,"processor/database/data/main/");	
	strcat(filename,name);
	strcat(filename,".stdat");
	
	ofstream ofs;
	ofs.open(filename,ios::binary);
		
	for(int i=2;i<words.size();i++){
	long length =strlen(words[i].c_str());	
	length=(length/8)+1;
	ofs.write(words[i].c_str(),sizeof(string)*2*(length));
	}	
	ofs.close();
	}

	else if(type==2){
	strcat(filename,"processor/database/data/tmp/");	
	strcat(filename,name);
	strcat(filename,".stdat");
		
	
	ofstream ofs;
	ofs.open(filename,ios::binary);
		
	for(int i=0;i<words.size();i++){
	long length =strlen(words[i].c_str());	
	length=(length/8)+1;
	ofs.write(words[i].c_str(),sizeof(string)*2*(length));
	}	
	ofs.close();
}
}
