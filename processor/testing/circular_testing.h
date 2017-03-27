bool print_tabledatalist_test_next_down(TABLEDATALIST * tabledatalist)
{
	/*printing of the content of the tabledatalist*/
	TABLEDATALIST * temp1, * temp2;
	if (tabledatalist == NULL)
	{
		cout << "TABLE EMPTY";
		return false;
	}
		
	temp2 = temp1 = tabledatalist;
	
	do{
		if(temp1->up != tabledatalist)
		{
			//cout << "New Row\n";
			do
			{
				temp1 = temp1->next;	
			}while(temp1!=temp2);
			
		}
		temp2 = temp2->down;
		temp1 = temp2;		
	}while(temp2 != tabledatalist);
	
	return true;
}

bool print_tabledatalist_test_next_up(TABLEDATALIST * tabledatalist)
{
	/*printing of the content of the tabledatalist*/
	TABLEDATALIST * temp1, * temp2;
	if (tabledatalist == NULL)
	{
		cout << "TABLE EMPTY";
		return false;
	}
		
	temp2 = temp1 = tabledatalist;

	do{
		if(temp1->up != tabledatalist)
		{
			do
			{
				temp1 = temp1->next;	
			}while(temp1!=temp2);
			
		}
		temp2 = temp2->up;
		temp1 = temp2;		
	}while(temp2 != tabledatalist);
	
	return true;
}


bool print_tabledatalist_test_prev_down(TABLEDATALIST * tabledatalist)
{
	int i = 0;
	/*printing of the content of the tabledatalist*/
	TABLEDATALIST * temp1, * temp2;
	if (tabledatalist == NULL)
	{
		cout << "TABLE EMPTY";
		return false;
	}
	
	temp2 = temp1 = tabledatalist;
	
	do{
		if(temp1->up != tabledatalist)
		{	
			//cout << "New Row\n";
			do
			{
				//cout << "New Column\n";
				temp1 = temp1->prev;	
			}while(temp1!=temp2);
			//cout << "End of New Row\n";
		}
		temp2 = temp2->down;
		temp1 = temp2;		
	}while(temp2 != tabledatalist);
	
	return true;
}

bool print_tabledatalist_test_prev_up(TABLEDATALIST * tabledatalist)
{
	/*printing of the content of the tabledatalist*/
	TABLEDATALIST * temp1, * temp2;
	if (tabledatalist == NULL)
	{
		cout << "TABLE EMPTY";
		return false;
	}
	
	temp2 = temp1 = tabledatalist;
	
	do{
		if(temp1->up != tabledatalist)
		{
			do
			{
				temp1 = temp1->prev;	
			}while(temp1!=temp2);
		}
		temp2 = temp2->up;
		temp1 = temp2;		
	}while(temp2 != tabledatalist);
	
	return true;
}


bool circular_test(TABLEDATALIST ** tabledatalist)
{
	cout << "\nTesting begins...\n";
	TABLEDATALIST * temp = tabledatalist[0];	

	//print_data(tabledatalist[0]->down->down->down->down->down->down->prev);
	
	do
	{
		//cout << "New Column\n";
		print_tabledatalist_test_next_down(temp);
		temp = temp->next;
	}while(temp != tabledatalist[0]);
	
	cout << "First pass ok \n";
	temp = tabledatalist[0];
	
	do
	{
		print_tabledatalist_test_next_up(temp);
		temp = temp->next;
	}while(temp != tabledatalist[0]);
	
	cout << "Second pass ok \n";
	temp = tabledatalist[0];
	
	do
	{
		//cout << "New Column\n";
		print_tabledatalist_test_prev_down(temp);
		temp = temp->next;
	}while(temp != tabledatalist[0]);
	
	cout << "Third pass ok \n";
	temp = tabledatalist[0];
	
	do
	{
		print_tabledatalist_test_prev_up(temp);
		temp = temp->next;
	}while(temp != tabledatalist[0]);
	
	cout << "Fourth pass ok \n";
	
	return true;
	
}