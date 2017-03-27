/*
	SAMPLE DATA: FOR TESTING ONLY
*/

CONDLIST * sample_condlist();
CONDLIST * sample_condlist2();
CONDLIST * sample_condlist3();



SELECTQUERYPARSE build_sample_select3()
{
	SELECTQUERYPARSE query;
	query.table = new DATA("varchar", new string("employees"));
	
	query.columnlist = new DATA("varchar", new string("names"));
	/*query.columnlist->next = new DATA("varchar", new string("age"));
	query.columnlist->next->next = new DATA("varchar", new string("age"));
	query.columnlist->next->next->next = new DATA("varchar", new string("names"));
	query.columnlist->next->next->next->next = new DATA("varchar", new string("salary"));*/
	//query.columnlist->next->down = new DATA("varchar", new string("substr"));
	// query.columnlist->next->down->down = new DATA("int", new int(0));
	// query.columnlist->next->down->down->next = new DATA("int", new int(3));
	// query.columnlist->next->next = new DATA("varchar", new string("age"));
	// query.columnlist->up = new DATA("varchar", new string("Added age"));
	// query.columnlist->next->up = new DATA("varchar", new string("New name"));
	// query.columnlist->next->next->up = new DATA("varchar", new string("New age"));
	
	//query.columnlist->down = new DATA("varchar", new string("sum"));
	//query.columnlist->down->down = new DATA("", NULL);
	
	//query.columnlist->down->down = new DATA("int", new float(100));
	//query.order_grouping = NULL;
	query.group_condition = NULL;
	// query.join_condition = 	new CONDLIST("=","", NULL, "",
	// 						new DATA("varchar", new string("age")),
	// 						new DATA("varchar", new string("children")));
	query.order_grouping = NULL;
	query.search_condition = NULL;
	query.group_grouping = NULL;
	query.display_constraint = NULL;
	//query.display_constraint = new DATA("varchar", new string("distinct"));
	//query.display_constraint = new DATA("varchar", new string("top"), NULL, NULL, NULL, new DATA("int", new int(200), NULL, NULL, NULL, new DATA("varchar", new string("%"))));
	// query.display_constraint = new DATA("varchar", new string("top"), NULL, NULL, NULL, new DATA("int",new int(10000)));
	// query.order_grouping = new DATA("varchar", new string("children"), NULL, NULL, NULL, new DATA("varchar",new string("asc")));
	// query.group_grouping = new DATA("varchar", new string("gender"));
	//query.search_condition = sample_condlist2();
	return query;
}


SELECTQUERYPARSE build_sample_select()
{
	SELECTQUERYPARSE query;
	query.table = new DATA("varchar", new string("employees"));
	
	// query.table->next = new DATA("varchar", new string("full join"));
	// query.table->next->next = new DATA("varchar", new string("emp"));
	// query.table->next->next->next = new DATA("varchar", new string("left join"));
	// query.table->next->next->next->next = new DATA("varchar", new string("employees"));

	//query.search_condition = NULL;
	query.columnlist = NULL;
	query.columnlist = new DATA("varchar", new string("salary"));
	query.columnlist->next = new DATA("varchar", new string("age"));
	query.columnlist->next->next = new DATA("varchar", new string("age"));
	query.columnlist->next->next->next = new DATA("varchar", new string("names"));
	query.columnlist->next->next->next->next = new DATA("varchar", new string("salary"));
	//query.columnlist->next->down = new DATA("varchar", new string("substr"));
	// query.columnlist->next->down->down = new DATA("int", new int(0));
	// query.columnlist->next->down->down->next = new DATA("int", new int(3));
	// query.columnlist->next->next = new DATA("varchar", new string("age"));
	// query.columnlist->up = new DATA("varchar", new string("Added age"));
	// query.columnlist->next->up = new DATA("varchar", new string("New name"));
	// query.columnlist->next->next->up = new DATA("varchar", new string("New age"));
	
	//query.columnlist->down = new DATA("varchar", new string("sum"));
	//query.columnlist->down->down = new DATA("", NULL);
	
	//query.columnlist->down->down = new DATA("int", new float(100));
	//query.order_grouping = NULL;
	query.group_condition = NULL;
	// query.join_condition = 	new CONDLIST("=","", NULL, "",
	// 						new DATA("varchar", new string("age")),
	// 						new DATA("varchar", new string("children")));
	query.order_grouping = NULL;
	query.search_condition = NULL;
	query.group_grouping = NULL;
	query.display_constraint = NULL;
	//query.display_constraint = new DATA("varchar", new string("distinct"));
	//query.display_constraint = new DATA("varchar", new string("top"), NULL, NULL, NULL, new DATA("int", new int(200), NULL, NULL, NULL, new DATA("varchar", new string("%"))));
	// query.display_constraint = new DATA("varchar", new string("top"), NULL, NULL, NULL, new DATA("int",new int(10000)));
	// query.order_grouping = new DATA("varchar", new string("children"), NULL, NULL, NULL, new DATA("varchar",new string("asc")));
	// query.group_grouping = new DATA("varchar", new string("gender"));
	//query.search_condition = sample_condlist2();
	return query;
}

SELECTQUERYPARSE build_sample_select2()
{
	SELECTQUERYPARSE query;
	query.table = new DATA("varchar", new string("employees"));
	query.table->next = new DATA("varchar", new string("inner_join"));
	query.table->next->next = new DATA("varchar", new string("emp"));
	//query.search_condition = NULL;
	query.columnlist = NULL;
	query.columnlist = new DATA("varchar", new string("salary"));
	query.columnlist->next = new DATA("varchar", new string("name"));
	query.columnlist->next->next = new DATA("varchar", new string("age"));
	query.columnlist->up = new DATA("varchar", new string("Added age"));
	query.columnlist->next->up = new DATA("varchar", new string("New name"));
	query.columnlist->next->next->up = new DATA("varchar", new string("New age"));
	query.columnlist->down = new DATA("varchar", new string("+"));
	query.columnlist->down->down = new DATA("int", new float(100));
	//query.order_grouping = NULL;
	query.group_condition = NULL;
	query.join_condition = NULL;
	query.order_grouping = NULL;
	query.search_condition = NULL;
/*	query.order_grouping = new DATA("varchar", new string("age"), NULL, NULL, NULL, new DATA("varchar",new string("asc")));
	query.group_grouping = new DATA("varchar", new string("gender"));
	query.search_condition = sample_condlist2();*/
	return query;
}


TABLEDATALIST ** sample_join5()
{
	TABLEDATALIST ** rows;
	rows = join_op(*table_pool[0], *table_pool[1], 
	new CONDLIST("=","", NULL, "", new DATA("varchar", new string("age")), new DATA("varchar", new string("children"))), "full join");
	print_tabledatalist(rows);
	free_tabledatalist(rows);
}

TABLEDATALIST ** sample_join4()
{
	TABLEDATALIST ** rows;
	rows = join_op(*table_pool[0], *table_pool[1], 
	new CONDLIST("=","", NULL, "", new DATA("varchar", new string("age")), new DATA("varchar", new string("children"))), "right join");
	print_tabledatalist(rows);
	free_tabledatalist(rows);
}

TABLEDATALIST ** sample_join3()
{
	TABLEDATALIST ** rows;
	rows = join_op(*table_pool[0], *table_pool[1], 
	new CONDLIST("=","", NULL, "", new DATA("varchar", new string("age")), new DATA("varchar", new string("children"))), "left join");
	print_tabledatalist(rows);
	free_tabledatalist(rows);
}


TABLEDATALIST ** sample_join2()
{
	TABLEDATALIST ** rows;
	rows = join_op(*table_pool[0], *table_pool[1], 
	new CONDLIST("=","", NULL, "", new DATA("varchar", new string("age")), new DATA("varchar", new string("children"))), "inner_join");
	print_tabledatalist(rows);
	free_tabledatalist(rows);
}

TABLEDATALIST ** sample_join()
{
	TABLEDATALIST ** rows;
	rows = join_op(*table_pool[0], *table_pool[1], new CONDLIST("<","", NULL, "", new DATA("varchar", new string("age"), NULL, NULL, 
	new DATA("varchar", new string("a"))), new DATA("varchar", new string("age"), NULL, NULL, 
	new DATA("varchar", new string("b")))), "inner_join");
	print_tabledatalist(rows);
	free_tabledatalist(rows);
}


CONDLIST * sample_condlist()
{
	return  new CONDLIST(">","age", new int(22), "int");
}

CONDLIST * sample_condlist2()
{
	CONDLIST * search_condition;
	
	search_condition = new CONDLIST(">", "age", new int(20), "int"); 
	search_condition->next = new CONDLIST("or");
	//search_condition->next->next = new CONDLIST("LIKE", "names", new string("%s"), "varchar");
	
	search_condition->next->next  = new CONDLIST(">", "age", new int(22), "int"); 
	//search_condition->next = new CONDLIST("or");
	//search_condition= new CONDLIST("LIKE", "name", new string("%s"), "varchar");
	
	search_condition->next->next->next = new CONDLIST("or");
	search_condition->next->next->next->next = new CONDLIST(">", "age", new int(18), "int");
	search_condition->next->next->next->next->next = new CONDLIST("or");
	search_condition->next->next->next->next->next->next = new CONDLIST("<", "age", new int(0), "int");
	return search_condition;
}

CONDLIST * sample_condlist3()
{
	CONDLIST * search_condition;
	search_condition = new CONDLIST("is not null","gender", NULL, "");
	search_condition->next = new CONDLIST("or");
	// search_condition->next = new CONDLIST("and");
	search_condition->next->next = new CONDLIST("is not null","salary", NULL, "");
	search_condition->next->next->next = new CONDLIST("or");
	search_condition->next->next->next->next = new CONDLIST("is not null","name", NULL, "");

	return search_condition;
}

DATA * build_sample_columnlist()
{
	DATA * columnlist, * temp1, * temp2;
	//columnlist = new DATA("varchar",new string("age"));
	columnlist = new DATA("varchar", new string("gender"));
	temp1 = new DATA("varchar", new string("names"));
	columnlist->link(temp1);
	temp2 = new DATA("varchar", new string("age"));
	temp1->link(temp2);
	//columnlist->link(temp1);
	//temp1->link(temp2);
	return columnlist;
}

UPDATEQUERYPARSE build_sample_update()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	//query.search_condition = new CONDLIST("=","age", new int(18), "int");
	query.search_condition = new CONDLIST("=","age", new int(18), "int");
	//query.search_condition->next = new CONDLIST("OR");
	//query.search_condition->next->next = new CONDLIST("=","age", new int(20), "int");
	//query.search_condition->next->next = new CONDLIST("LIKE", "name", new string("%solis%"), "varchar");
	//query.search_condition->next->next->next = new CONDLIST("AND");
	//query.search_condition->next->next->next->next = new CONDLIST("LIKE", "name", new string("%a%"), "varchar");
	query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("S")));
	return query;

}

UPDATEQUERYPARSE build_sample_update2()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	//query.search_condition = new CONDLIST("=","age", new int(18), "int");
	query.search_condition = new CONDLIST("LIKE", "name", new string("%s"), "varchar");
	query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("S")));
	return query;
}

UPDATEQUERYPARSE build_sample_update3()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	query.search_condition = new CONDLIST("LIKE", "name", new string("%olis%"), "varchar");
	query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("J")));
	return query;
}

UPDATEQUERYPARSE build_sample_update4()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	query.search_condition = new CONDLIST("LIKE", "name", new string("_q%"), "varchar");
	query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("J")));
	return query;
}

UPDATEQUERYPARSE build_sample_update5()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	query.search_condition = new CONDLIST("LIKE", "name", new string("[a]%"), "varchar");
	query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("X")));
	return query;
}

UPDATEQUERYPARSE build_sample_update6()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	query.search_condition = new CONDLIST("<=", "age", new int(17), "int");
	query.search_condition->next = new CONDLIST("AND");
	query.search_condition->next->next = new CONDLIST("=", "name", new string("tan"), "varchar");
	query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("Z")));
	return query;
}

UPDATEQUERYPARSE build_sample_update7()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	query.search_condition = new CONDLIST("<=", "age", new int(17), "int");
	query.search_condition->next = new CONDLIST("OR");
	query.search_condition->next->next = new CONDLIST("=", "age", new int(18), "int");
	query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("Z")));
	return query;
}

UPDATEQUERYPARSE build_sample_update8()
{
	UPDATEQUERYPARSE query;
	query.tablename = "employees";
	query.search_condition = new CONDLIST("ISNULL", "age", NULL, "");
	query.column_value = new DATA("varchar", new string("age"), NULL, NULL, NULL, new DATA("int", new int(12)));
	return query;
}

DELETEQUERYPARSE build_sample_delete2()
{
	DELETEQUERYPARSE query;
	query.tablename = "employees";
	//query.search_condition = neDELETEw CONDLIST(">","age", new int(20), "int");
	query.search_condition = new CONDLIST("LIKE","name", new string("%s"), "varchar");
	//query.search_condition->next = new CONDLIST("OR");
	query.search_condition->next = new CONDLIST("AND");
	//query.search_condition->next->next = new CONDLIST("=","age", new int(18), "int");
	//query.search_condition->next->next->next = new CONDLIST("OR");
	//query.search_condition->next->next->next->next = new CONDLIST("=","age", new int(19), "int");
	query.search_condition->next->next = new CONDLIST("=", "age", new int(20), "int");
	//query.search_condition->next->next->next = new CONDLIST("AND");
	//query.search_condition->next->next->next->next = new CONDLIST("LIKE", "name", new string("%a%"), "varchar");
	//query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("S")));
	return query;

}

DELETEQUERYPARSE build_sample_delete()
{
	DELETEQUERYPARSE query;
	query.tablename = "employees";
	query.search_condition = new CONDLIST(">","age", new int(18), "int");
	//query.search_condition = NULL;

	//query.search_condition = sample_condlist3();
	//query.search_condition = new CONDLIST("is null","age", NULL, "");
	//query.search_condition->next = new CONDLIST("OR");
	query.search_condition->next = new CONDLIST("and");
	query.search_condition->next->next = new CONDLIST(">","age", new int(20), "int");
	//query.search_condition->next->next->next = new CONDLIST("OR");
	//query.search_condition->next->next->next->next = new CONDLIST("=","age", new int(19), "int");
	//query.search_condition->next->next = new CONDLIST("LIKE", "gender", new string("%F%"), "varchar");
	//query.search_condition->next->next->next = new CONDLIST("AND");
	//query.search_condition->next->next->next->next = new CONDLIST("LIKE", "name", new string("%a%"), "varchar");
	//query.column_value = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("S")));
	return query;

}

INSERTQUERYPARSE build_sample_insert_input(string name, int age, string gender)
{
	INSERTQUERYPARSE query;
	query.tablename = "employees";
	query.inserttype = "manual";
	DATA * temp = new DATA("varchar", new string("name"), NULL, NULL, NULL, new DATA("varchar", new string(name)));
	temp->next = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string(gender)));
	temp->next->next = new DATA("varchar", new string("age"), NULL, NULL, NULL, new DATA("varchar", new int(age)));
	query.datalist = temp;
	return query;
}

INSERTQUERYPARSE build_sample_insert()
{
	/*	
		TABLEDATALIST * temp = new TABLEDATALIST(new string("tan"), "varchar");
		temp->next = new TABLEDATALIST(new int(17), "int");
		temp->next->next = new TABLEDATALIST(new string("F"), "varchar");
		temp->next->next->next = temp;
	*/
	INSERTQUERYPARSE query;
	query.tablename = "employees";
	query.inserttype = "manual";
	DATA * temp = new DATA("varchar", new string("age"), NULL, NULL, NULL, new DATA("int", new int(17)));
	//temp->next = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("F")));
	query.datalist = temp;
	return query;
}

INSERTQUERYPARSE build_sample_insert2()
{
	
	INSERTQUERYPARSE query;
	query.tablename = "employees";
	query.inserttype = "manual";
	DATA * temp = new DATA("varchar", new string("name"), NULL, NULL, NULL, new DATA("varchar", new string("aquino")));
	query.datalist = temp;
	return query;
}

INSERTQUERYPARSE build_sample_insert3()
{
	INSERTQUERYPARSE query;
	query.tablename = "employees";
	query.inserttype = "manual";
	DATA * temp = new DATA("varchar", new string("name"), NULL, NULL, NULL, new DATA("varchar", new string("tan")));
	temp->next = new DATA("varchar", new string("age"), NULL, NULL, NULL, new DATA("int", new int(15)));
	temp->next->next = new DATA("varchar", new string("gender"), NULL, NULL, NULL, new DATA("varchar", new string("M")));
	query.datalist = temp;
	return query;
}


TABLE * build_test_table()
{
	TABLE * table =  new TABLE();
	int noOfRows = 7;
	int noOfColumns = 5;
	TABLEDATALIST  ** head, * temp1, * temp2;
	head = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *)*11);
	//col name
	head[0] = new TABLEDATALIST(new string("names"), "varchar");
	head[0]->next = new TABLEDATALIST(new string("age"), "varchar");
	head[0]->next->next = new TABLEDATALIST(new string("gender"), "varchar");
	head[0]->next->next->next = new TABLEDATALIST(new string("salary"), "varchar");
	head[0]->next->next->next->next = new TABLEDATALIST(new string("resigned"), "varchar");
	head[0]->next->prev = head[0];
	head[0]->next->next->prev = head[0]->next;
	head[0]->next->next->next->prev = head[0]->next->next;
	head[0]->next->next->next->next->prev = head[0]->next->next->next;
	head[0]->next->next->next->next->next = head[0];
	head[0]->prev = head[0]->next->next->next->next;
	
	// //cout << "Entered" << "\n";
	// 	//::print_tabledatalist(head[0]);
	// //end of col name

	// //col type
	head[1] = new TABLEDATALIST(new string("varchar"), "varchar");
	//head[1]->next = new TABLEDATALIST(new string("int"), "varchar", "PRIMARY");
	head[1]->next = new TABLEDATALIST(new string("int"), "varchar");
	head[1]->next->next = new TABLEDATALIST(new string("varchar"), "varchar");
	head[1]->next->next->next = new TABLEDATALIST(new string("float"), "varchar");
	head[1]->next->next->next->next = new TABLEDATALIST(new string("bool"), "varchar");
	head[1]->next->prev = head[1];
	head[1]->next->next->prev = head[1]->next;
	head[1]->next->next->next->prev = head[1]->next->next;
	head[1]->next->next->next->next->prev = head[1]->next->next->next;
	head[1]->next->next->next->next->next= head[1];
	head[1]->prev = head[1]->next->next->next->next;
		//::print_tabledatalist(head[1]);
	//end of col data type
	
		
	// //populate data
	for(int i=2; i<noOfRows+2; i++)
	{
		head[i]= new TABLEDATALIST(new string("solisis"+i-2), "varchar");
		head[i]->next = new TABLEDATALIST(new int(i+15), "int");
		 if(i % 3 == 0)
		 	head[i]->next->next = new TABLEDATALIST(new string("M"), "varchar");
		else if(i%3==1)
			head[i]->next->next = new TABLEDATALIST(new string("F"), "varchar");
		else
			head[i]->next->next = new TABLEDATALIST(new string("B"), "varchar");

		head[i]->next->next->next = new TABLEDATALIST(new float(100+i), "float");

		if(i % 2 == 0)
			head[i]->next->next->next->next = new TABLEDATALIST(new bool(true), "bool");
		else
			head[i]->next->next->next->next = new TABLEDATALIST(new bool(false), "bool");
		head[i]->next->prev = head[i];
		head[i]->next->next->prev = head[i]->next;
		head[i]->next->next->next->prev = head[i]->next->next;
		head[i]->next->next->next->next->prev = head[i]->next->next->next;
		head[i]->next->next->next->next->next = head[i];
		head[i]->prev = head[i]->next->next->next->next;
		//print_tabledatalist(head[i]);
	}
	
	for(int i=0; i<noOfRows+2; i++)
	{
		if(i != noOfRows+1)
			head[i]->down=head[i+1];
		if(i != 0)
			head[i]->up=head[i-1];
		if(i == 0)
			head[i]->up = head[noOfRows+1];
		if(i == noOfRows+1)
			head[i] = head[0];
	}

	temp1 = temp2 = head[0];
	/*assigning the ->down to other cells except for the first column*/
	do{
		do{
			temp2->next->down = temp2->down->next;
			temp2 = temp2->down;
		}while(temp2->down != NULL);
		
		if(temp2->down == NULL)
		{
			temp2->down = temp1;
		}
		
		temp1 = temp1->next;
		temp2 = temp1;
		
	}while(temp1 != head[0]);
	
	
	temp1 = temp2 = head[0]->next;
	do{
		do{
			temp2->down->up = temp2;
			temp2 = temp2->down;
		}while(temp2->down != temp1);
		
		
		if(temp2->down == temp1)
		{
			temp1->up = temp2;
			//temp2->down = temp1;
		}
		
		temp1 = temp1->next;
		temp2 = temp1;
	}while(temp1->next!=head[0]->next);
		
		//::print_tabledatalist(head[0]->next->next);
	
	table->table = head;
	table->alias = "a";
	table->tablename = "employees";
	return table;
}

TABLE * build_test_table2()
{
	TABLE * table = new TABLE();
	int noOfRows = 4;
	int noOfColumns = 3;
	TABLEDATALIST  ** head, * temp1, * temp2;
	head = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *)*6);

	//col name
	head[0] = new TABLEDATALIST(new string("name"), "varchar");
	head[0]->next = new TABLEDATALIST(new string("age"), "varchar");
	head[0]->next->next = new TABLEDATALIST(new string("gender"), "varchar");
	head[0]->next->prev = head[0];
	head[0]->next->next->prev = head[0]->next;
	head[0]->next->next->next = head[0];
	head[0]->prev = head[0]->next->next;
	
	
	//cout << "Entered" << "\n";
		//::print_tabledatalist(head[0]);
	//end of col name

	//col type
	head[1] = new TABLEDATALIST(new string("varchar"), "varchar");
	head[1]->next = new TABLEDATALIST(new string("int"), "varchar", "PRIMARY");
	head[1]->next->next = new TABLEDATALIST(new string("varchar"), "varchar");
	head[1]->next->prev = head[1];
	head[1]->next->next->prev = head[1]->next;
	head[1]->next->next->next = head[1];
	head[1]->prev = head[1]->next->next;
		//::print_tabledatalist(head[1]);
	//end of col data type
	
	
	head[0]->down = head[1];
	head[1]->up = head[0];
	head[0]->next->down = head[1]->next;
	head[1]->next->up = head[0]->next;
	head[0]->next->next->down = head[1]->next->next;
	head[1]->next->next->up = head[0]->next->next;
	
	
	head[0]->up = head[1];
	head[1]->down = head[0];
	head[0]->next->up = head[1]->next;
	head[1]->next->down = head[0]->next;
	head[0]->next->next->up = head[1]->next->next;
	head[1]->next->next->down = head[0]->next->next;
	
	table->table = head;
	table->alias = "";
	table->tablename = "employees";
	return table;
}

TABLE * build_test_table3()
{
	TABLE * table =  new TABLE();
	int noOfRows = 4;
	int noOfColumns = 3;
	TABLEDATALIST  ** head, * temp1, * temp2;
	head = (TABLEDATALIST **)malloc(sizeof(TABLEDATALIST *)*8);

	//col name
	head[0] = new TABLEDATALIST(new string("name"), "varchar");
	head[0]->next = new TABLEDATALIST(new string("children"), "varchar");
	head[0]->next->next = new TABLEDATALIST(new string("gender"), "varchar");
	head[0]->next->prev = head[0];
	head[0]->next->next->prev = head[0]->next;
	head[0]->next->next->next = head[0];
	head[0]->prev = head[0]->next->next;
	
	
	//cout << "Entered" << "\n";
		//::print_tabledatalist(head[0]);
	//end of col name

	//col type
	head[1] = new TABLEDATALIST(new string("varchar"), "varchar");
	head[1]->next = new TABLEDATALIST(new string("int"), "varchar", "PRIMARY");
	head[1]->next->next = new TABLEDATALIST(new string("varchar"), "varchar");
	head[1]->next->prev = head[1];
	head[1]->next->next->prev = head[1]->next;
	head[1]->next->next->next = head[1];
	head[1]->prev = head[1]->next->next;
		//::print_tabledatalist(head[1]);
	//end of col data type
		
	//populate data
	for(int i=2; i<noOfRows+2; i++)
	{
		head[i]= new TABLEDATALIST(new string("solisis"+i-2), "varchar");
		head[i]->next = new TABLEDATALIST(new int(i+20), "int");
		if(i%2==0)
			head[i]->next->next = new TABLEDATALIST(new string("B"), "varchar");
		else
			head[i]->next->next = new TABLEDATALIST(new string("F"), "varchar");
		head[i]->next->prev = head[i];
		head[i]->next->next->prev = head[i]->next;
		head[i]->next->next->next = head[i];
		head[i]->prev = head[i]->next->next;
		//print_tabledatalist(head[i]);
	}
	
	for(int i=0; i<noOfRows+2; i++)
	{
		if(i != noOfRows+1)
			head[i]->down=head[i+1];
		if(i != 0)
			head[i]->up=head[i-1];
		if(i == 0)
			head[i]->up = head[noOfRows+1];
		if(i == noOfRows+1)
			head[i] = head[0];
	}

	temp1 = temp2 = head[0];
	/*assigning the ->down to other cells except for the first column*/
	do{
		do{
			temp2->next->down = temp2->down->next;
			temp2 = temp2->down;
		}while(temp2->down != NULL);
		
		if(temp2->down == NULL)
		{
			temp2->down = temp1;
		}
		
		temp1 = temp1->next;
		temp2 = temp1;
		
	}while(temp1 != head[0]);
	
	
	temp1 = temp2 = head[0]->next;
	do{
		do{
			temp2->down->up = temp2;
			temp2 = temp2->down;
		}while(temp2->down != temp1);
		
		
		if(temp2->down == temp1)
		{
			temp1->up = temp2;
			//temp2->down = temp1;
		}
		
		temp1 = temp1->next;
		temp2 = temp1;
	}while(temp1->next!=head[0]->next);
		
		//::print_tabledatalist(head[0]->next->next);
	
	table->table = head;
	table->alias = "b";
	table->tablename = "emp";
	return table;
}

SELECTQUERYPARSE build_test_query()
{
	/*
	SELECTQUERYPARSE * query = new QUERYPARSE();
	query->type.assign("SELECT");
	cout << query->type << "\n";
	
	SELLIST * sellist = new SELLIST();
	ATTRIBUTE attribute;
	attribute.name.assign("name");
	sellist->attribute = attribute;
	sellist->next = NULL;
	sellist->prev = NULL; 
	//cout << attribute.name << "\n";
	query->sellist = sellist;
	
	CONDLIST * condlist = new CONDLIST();
	
	COND cond;
	cond.attribute.assign("age");
	cond.op.assign(">");
	cond.value = 18;
	//cout << cond.attribute << cond.op << "\n";
	
	condlist->cond = cond;
	condlist->prev = NULL;
	condlist->next = NULL;
	
	query->condlist = condlist;
	
	TABLELIST  * tablelist = new TABLELIST();
	
	TABLE table;
	table.name.assign("employees");
	table.metadata = NULL;
	//cout << table.name << "\n";
	
	tablelist->table = table;
	tablelist->prev = NULL;
	tablelist->next = NULL;
	
	query->tablelist = tablelist;
	return *query;	
	*/
}


bool is_primary_key(string table, string column)
{
	if(table.compare("employees") == 0 && column.compare("age") == 0)
		//return true;
		return false;
	return false;
}

bool is_unique_key(string table, string column)
{
	if(table.compare("employees") == 0 && column.compare("age") == 0)
		//return true;
		return false;
	return false;
}

bool is_not_null(string table, string column)
{
	if(table.compare("employees") == 0 && column.compare("salary") == 0)
		//return true;
		return false;
	return false;	
}