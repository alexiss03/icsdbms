/*
	Converts the pattern into circular linked list
*/

void pattern(CONDLIST * search_condition)
{
	
	CONDLIST		** lolop = NULL;
	
	double_linked_list(search_condition);
	lolop = get_lolop(search_condition);
	
	int i = 0;	
}


/*
	Matches the available patterns to 'search_condition'
*/
CONDLIST **  match_pattern(TABLEDATALIST ** table, CONDLIST * search_condition)
{
	CONDLIST 	** lolop,
				** cond_pool,
				* new_search_condition,
				** temp  = cond_pool;
	int 		i,
				count = 0;
				
	
	double_linked_list(search_condition);
	
	
	
	cond_pool = (CONDLIST **)malloc(sizeof(CONDLIST *)* 1000);

	for(i = 0; i < 100; i++)
		*(cond_pool + i) = NULL;
		
	*(cond_pool + count) = search_condition;	
	do
	{
		lolop = get_lolop(*(cond_pool + count));
		i=0;		
		while(*(lolop + i) != NULL)
		{
			
			//swaps the conditions if the second has a smaller number of row count	
			new_search_condition = recopy_cond(*(cond_pool + count));
			swap_and_pattern(table, locate_current_lolop(*(lolop + i), *(cond_pool + count), new_search_condition), new_search_condition, cond_pool);
			
			//swaps the conditions if the second has a larger number of row count
			new_search_condition = recopy_cond(*(cond_pool + count));
			swap_or_pattern(table, locate_current_lolop(*(lolop + i), *(cond_pool + count), new_search_condition), new_search_condition, cond_pool);
			
			if(i==0)
			{
				//eliminates the conditions if it can be simplified
				new_search_condition = recopy_cond(*(cond_pool + count));
				elim_or_and_pattern(table, locate_current_lolop(*(lolop+i), *(cond_pool + count), new_search_condition), new_search_condition, cond_pool);
			}
			i++;
		}

		count++;
		
	}while(*(cond_pool + count) != NULL);

	
	for(count = 0; *(cond_pool + count) != NULL; count++)
		print_cond(*(cond_pool + count));
	
	
	return cond_pool;
}


/*
	Locates the current logical operator
*/
CONDLIST * locate_current_lolop(CONDLIST * lolop, CONDLIST * search_condition, CONDLIST * new_search_condition)
{
	CONDLIST 	* temp = search_condition,
				* temp2 = new_search_condition;
				
	do
	{
		if( temp == lolop)
			return temp2;
		temp = temp->next;
		temp2 = temp2->next;
	
	}while(temp != search_condition);
	return NULL;
}


/*
	Copies 'condlist'
*/
CONDLIST * recopy_cond(CONDLIST * condlist)
{
	CONDLIST 	* newcond = NULL,
				* temp = condlist,
				* prev = NULL;
	
	do{
		if(prev == NULL)
		{
			newcond = new CONDLIST(temp);
			prev = newcond;
			
		}
		else
		{
			prev->next = new CONDLIST(temp);
			prev->link(prev->next);
			prev = prev->next;
		}
		temp = temp->next;
	}
	while(temp != condlist); 
	
	prev->link(newcond);
	
	return newcond;
}

/*
	Switches cond 'prev' and 'next'
*/
void switch_cond(CONDLIST * prev, CONDLIST * next)
{
	prev->swap(next);
}


/*
	Converts 'search_condition' into array of CONDLIST
*/
CONDLIST ** get_lolop(CONDLIST * search_condition)
{
	CONDLIST 	* temp = search_condition,
				** lolop;
	
	lolop = (CONDLIST **)malloc(sizeof(CONDLIST *) * count_lolop(search_condition) + 1);
	
	int 		i = 0;
	
	while(temp->next != search_condition)
	{
		lolop[i++] = temp->next;
		temp = temp->next->next;
	}
	
	lolop[i] = NULL;
	
	if(lolop == NULL)
		return NULL;
	
	return lolop;
}


/*
	Counts the number of logical operators in 'search_condition'
*/
int count_lolop(CONDLIST * search_condition)
{
	CONDLIST * temp = search_condition;
	
	int count = 0;
	while(temp->next != search_condition)
	{
		count++;
		temp = temp->next->next;
	}
	return count;
}

/*
	Transforms 'search_condition' into a doubly circular linked list
*/
void double_linked_list(CONDLIST * search_condition)
{
	CONDLIST * temp = search_condition;
	
	while(temp->next != NULL)
	{
		temp->next->prev = temp;
		temp = temp->next;
	}
	
	temp->next = search_condition;
	search_condition->prev = temp;
}

/*
	Transforms back 'search_condition' into a singly non circular linked list
*/
void de_double_linked_list(CONDLIST * search_condition)
{
	search_condition->prev->next = NULL;
	search_condition->prev = NULL;
}