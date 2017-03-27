typedef struct node {
	void ** pointers;
	int * keys;
	struct node * parent;
	bool is_leaf;
	int num_keys;
	struct node * next; // Used for queue.
} node;

class BPT
{
	public:
		BPT(string newtable_name, string newcolumn_name, node * newroot)
		{
			root = newroot;
			table_name = newtable_name;
			column_name = newcolumn_name;
		}
		
	node * root;
	string table_name;
	string column_name;
};

class DATE
{
	public:
		DATE(DATE * newdate)
		{
			month = newdate->month;
			day = newdate->day;
			year = newdate->year;
		}
		
		DATE(int newmonth, int newday, int newyear)
		{
			month = newmonth;
			day = newday;
			year = newyear;
		}
		
		int month;
		int day;
		int year;
};

class DATA
{
	public:
	
		DATA(DATA * old)
		{
			if(old != NULL)
			{	
				type = old->type;
				if(old->type.compare("int") == 0)
					data = new int(*((int *)(old->data)));
				else if(old->type.compare("varchar") == 0)
					data = new string(*((string *)(old->data)));
				else if(old->type.compare("bool") == 0)
					data = new bool(*((bool *)(old->data)));
				else if(old->type.compare("float") == 0)
					data = new float(*((float *)(old->data)));
				else if(old->type.compare("date") == 0)
					data = new DATE(*((DATE *)(old->data)));
			}
		}
	
		DATA(string newType, void * newData, DATA * newnext=NULL, DATA * newprev = NULL, DATA * newup = NULL, DATA * newdown = NULL)
		{
			data = newData;
			type = newType;
			
			if(newnext!= NULL)
				next = newnext;
			else
				next = NULL;
				
			if(newprev!= NULL)
				prev = newprev;
			else
				prev = NULL;
				
			if(newup!= NULL)
				up = newup;
			else
				next = NULL;
				
			if(newdown!= NULL)
				down = newdown;
			else
				down = NULL;
		}
		
		void link(DATA * second)
		{
			next = second;
			second->prev = this;
		}
		
		void * data;
		void * data2; // new string();
		string type;
		DATA * next;
		DATA * prev;	
		DATA * up;
		DATA * down;

};

class TABLEDATALIST
{
	public:
		TABLEDATALIST(	void * newdata,
						string newtype, 
						string newconstraint = "", 
						TABLEDATALIST * newnext = NULL, 
						TABLEDATALIST * newprev = NULL,
						TABLEDATALIST * newup = NULL,
						TABLEDATALIST * newdown = NULL
		)
		{
			next = newnext;
			prev = newprev;
			up = newup;
			down = newdown;
			data = newdata;
			type = newtype;	
			constraint = newconstraint;
		};
		
		TABLEDATALIST(TABLEDATALIST * newtbl)
		{
			if(newtbl->type.compare("varchar") == 0)
			{
				type = "varchar";
				if(newtbl->data != NULL)
					data = new string(*(static_cast<string *>(newtbl->data)));
				else
					data = NULL;
					
				constraint = newtbl->constraint;
			}
			else if(newtbl->type.compare("int") == 0)
			{
				type = "int";
				if(newtbl->data != NULL)
					data = new int(*(static_cast<int *>(newtbl->data)));
				else
					data = NULL;
				constraint = newtbl->constraint;
			}
			else if(newtbl->type.compare("float") == 0)
			{
				type = "float";
				if(newtbl->data != NULL)
					data = new float(*(static_cast<float *>(newtbl->data)));
				else
					data = NULL;
				constraint = newtbl->constraint;
			}
			else if(newtbl->type.compare("bool") == 0)
			{
				type = "bool";
				if(newtbl->data != NULL)
					data = new int(*(static_cast<bool *>(newtbl->data)));
				else
					data = NULL;
				constraint = newtbl->constraint;
			}
			else if(newtbl->type.compare("date") == 0)
			{
				type = "date";
				if(newtbl->data != NULL)
					data = new DATE(*(static_cast<DATE *>(newtbl->data)));
				else
					data = NULL;
				constraint = newtbl->constraint;
			}
			else if(newtbl->type.compare("CHAR") == 0)
			{
				type = "CHAR";
				if(newtbl->data != NULL)
					data = new int(*(static_cast<char *>(newtbl->data)));
				else
					data = NULL;
				constraint = newtbl->constraint;
			}
			else if(newtbl->type.compare("") == 0)
			{
				type = "";
				data = NULL;
				constraint = newtbl->constraint;
			}
			next = NULL;
			prev = NULL;
			up = NULL;
			down = NULL;
		};
		
		void update(void * newdata, string newtype= "", string newconstraint = "")
		{
			data = newdata;
			type = newtype;	
			constraint = newconstraint;
		}
		
		void * data;
		string type;
		string constraint;	
		TABLEDATALIST * prev;
		TABLEDATALIST * next;	
		TABLEDATALIST * up;
		TABLEDATALIST * down;		
};
/*
class METADATA
{
	void * data;
}METADATA;

class STATSDATA
{
	int no_of_rows;
	
}STATSDATA;

class COLLIST
{
	string colname;
	COLLIST * next;
	COLLIST * prev;
};

class TABLELIST
{
	string tablename;
	TABLELIST * next;
	TABLELIST * prev;
};*/

class CONDLIST
{
	public:
		CONDLIST()
		{
		}
	
		CONDLIST(string newop, string newcolumn_name = "",  void * newvalue = NULL , string newtype = "", DATA * newcolumn1 = NULL, DATA * newcolumn2 = NULL)
		{
			column_name = newcolumn_name;
			op = newop;
			value = newvalue;
			type = newtype;
			next = NULL;
			prev = NULL;
			column1 = newcolumn1;
			column2 = newcolumn2;
		}
		
		CONDLIST(CONDLIST * old)
		{
			column_name = old->column_name;
			op = old->op;
			type = old->type;
			
			if(old->type.compare("int") == 0)
				value = new int(*((int *)(old->value)));
			else if(old->type.compare("varchar") == 0)
				value = new string(*((string *)(old->value)));
			else if(old->type.compare("bool") == 0)
				value = new bool(*((bool *)(old->value)));
			else if(old->type.compare("float") == 0)
				value = new float(*((float *)(old->value)));
			else if(old->type.compare("date") == 0)
				value = new DATE(*((DATE *)(old->value)));
			
			column1 = new DATA(old->column1);
			column2 = new DATA(old->column2);
			
			next = NULL;
			prev = NULL;
			
		}
		
		void link(CONDLIST * second)
		{
			next = second;
			second->prev = this;
		}
		
		
		void swap(CONDLIST * next)
		{
			string temp;
			void * void_temp;
			
			temp = next->column_name;
			next->column_name = this->column_name;
			this->column_name = temp;
			
			temp = next->op;
			next->op = this->op;
			this->op = temp;
			
			temp = next->type;
			next->type = this->type;
			this->type = temp;
			
			void_temp = next->value;
			next->value = this->value;
			this->value = void_temp;
			
		}
	string column_name;
	string op;
	void * value; //commonly is of type DATA
	string type;
	CONDLIST * next;
	CONDLIST * prev;
	DATA * column1;
	DATA * column2;
};

class SELECTQUERYPARSE
{
	public:
		DATA * table;
		DATA * columnlist;
		DATA * display_constraint;
		CONDLIST * search_condition;
		CONDLIST * join_condition;
		CONDLIST * group_condition;	
		DATA * group_grouping;
		DATA * order_grouping;
};
/*
	possible inserttype values =  manual | subquery
	if(inserttype == MANUAL)	void * = TABLEDATALIST
	else if(inserttype == SUBQUERY)	void * = SELECTQUERYPARSE
	
*/
class INSERTQUERYPARSE
{
	public:
		string tablename;
		DATA * datalist;
		string inserttype;
		
};

class UPDATEQUERYPARSE
{
	public:
		string tablename;
		DATA * column_value;
		CONDLIST * search_condition;
};

class DELETEQUERYPARSE
{
	public:
		string tablename;
		CONDLIST * search_condition;
};


class TABLE
{
	public:
		TABLE()
		{

		}
		
		TABLE(TABLEDATALIST ** newtable, string newtablename="", string newalias="")
		{
			table = newtable;
			tablename = newtablename;
			alias = newalias;
		}

		string tablename;
		TABLEDATALIST ** table;
		string alias;
};

typedef struct record {
	int value;
	TABLEDATALIST ** data;
} record;

class CREATEQUERYPARSE
{

	void empty()
	{
		columnNameList.empty();
		columnNameDataType.empty();
		columnNameNotNull.empty();
		constraintColumnName.empty();
		constraintName.empty();
		constraintType.empty();
		referenceTable.empty();
		referenceColumn.empty();
		checkCondition.empty();
	}
	string table_name;
	vector<string> columnNameList;
	vector<string> columnNameDataType;
	vector<string> columnNameNotNull;
	vector<string> constraintColumnName; 
	vector<string> constraintName;
	vector<string> constraintType;
	vector<string> referenceTable;
	vector<string> referenceColumn;
	vector<string> checkCondition;
};