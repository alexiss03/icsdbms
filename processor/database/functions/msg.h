#include <stdio.h>
#include <iostream>
using namespace std;

void showmsg(){
	printf("Welcome to ICS Database Management System Version 1.0\n");
	printf("written in C++\n");	
	//printf("\n");
}

void showexit(){
	printf("Thank You for using ICSDBMS\n");
}

const char* error_msg(int num){
	switch(num){
	
	case 200: printf("ICSDBMS_ERROR200: CREATE_TABLE_NOT_SUCCESSFUL\n");
				return " ICSDBMS_ERROR200:_CREATE_TABLE_NOT_SUCCESSFUL";
	case 201: printf("ICSDBMS_ERROR201: INSERT_DATA_NOT_SUCCESSFUL\n");
				return " ICSDBMS_ERROR201:_INSERT_DATA_NOT_SUCCESSFUL";
	case 202: printf("ICSDBMS_ERROR202: DELETE_DATA_NOT_SUCCESSFUL\n");
				return " ICSDBMS_ERROR202: DELETE_DATA_NOT_SUCCESSFUL";
	case 203: printf("ICSDBMS_ERROR203: UPDATE_DATA_NOT_SUCCESSFUL\n");
				return " ICSDBMS_ERROR202: DELETE_DATA_NOT_SUCCESSFUL";			
	case 204: printf("ICSDBMS_ERROR204: ALTER_TABLE_NOT_SUCCESSFUL\n");
				return " ICSDBMS_ERROR204: ALTER_TABLE_NOT_SUCCESSFUL"; 
	case 205: printf("ICSDBMS_ERROR205: DROP_TABLE_NOT_SUCCESSFUL\n");
				return " ICSDBMS_ERROR205: DROP_TABLE_NOT_SUCCESSFUL";
	case 206: printf("ICSDBMS_ERROR206: DROP_TABLE_DOESN'T_EXIST\n");
				return " ICSDBMS_ERROR206: DROP_TABLE_DOESN'T_EXIST";
	case 207: printf("ICSDBMS_ERROR207: TABLE_DOESN'T_EXIST\n");
				return " ICSDBMS_ERROR207: TABLE_DOESN'T_EXIST";
	case 208: printf("ICSDBMS_ERROR208: COMMAND NOT SUPPORTED\n");
				return " ICSDBMS_ERROR208: COMMAND NOT SUPPORTED";
	}
}

const char* success_msg(int num){
	switch(num){
	
	case 100: printf("ICSDBMS_MSG100: CREATE_TABLE_SUCCESSFUL\n");
				return " ICSDBMS_MSG100: CREATE_TABLE_SUCCESSFUL";
	case 101: printf("ICSDBMS_MSG101: INSERT_DATA_SUCCESSFUL\n");
				return " ICSDBMS_MSG101: INSERT_DATA_SUCCESSFUL";
	case 102: printf("ICSDBMS_MSG102: DELETE_DATA_SUCCESSFUL\n");
				return " ICSDBMS_MSG102: DELETE_DATA_SUCCESSFUL";
	case 103: printf("ICSDBMS_MSG103: UPDATE_DATA_SUCCESSFUL\n");
				return " ICSDBMS_MSG103: UPDATE_DATA_SUCCESSFUL";
	case 104: printf("ICSDBMS_MSG104: ALTER_TABLE_SUCCESSFUL\n");
				return " ICSDBMS_MSG104: ALTER_TABLE_SUCCESSFUL";
	case 105: printf("ICSDBMS_MSG105: DROP_TABLE_SUCCESSFUL\n");
				return " ICSDBMS_MSG105: DROP_TABLE_SUCCESSFUL";
	case 106: printf("ICSDBMS_MSG106: ALTER_TABLE_RENAME_SUCCESSFUL\n");
				return " ICSDBMS_MSG106: ALTER_TABLE_RENAME_SUCCESSFUL";
	}
}

