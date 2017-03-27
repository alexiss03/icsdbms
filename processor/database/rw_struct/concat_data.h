#include <string.h>
#include <string>
vector<string> clean_data(vector<string> data){	
	vector<string> data_contents;
	char buffer[1000];
	buffer[0] = '\0';
	for(int i=0;i<data.size();i++){
		//cout << data[i] << endl;
		for(int j=i;j<data.size();j++){
		if(strcmp(data[j].c_str(),"|")==0){
			i=j;
			data_contents.push_back(buffer);	
			buffer[0] = '\0';			
			break;
			}
		else{
			
			strcat(buffer,data[j].c_str());	
				
		  }
		}
		
	}
 	
	
	return data_contents;
}//end of clean_data


