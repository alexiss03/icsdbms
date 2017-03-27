#include "functions/include_functions.h"
#include "queryprocessing_db.h"
using namespace std;
int main() {
	bool choice;
	choice=check_log_file();
	cout << "status: " << choice;	
		
	start_logs();
	//declarations	
	std::string line;	
	while(true){
	printf("ICSDBMS>");    
	std::getline(std::cin, line); 		
    queryprocessing(line);
	
	
	}//end of while loop  
	
}//end of main

