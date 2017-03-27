/*
	Pattern:
		If, row_count(lolop->prev) > row_count(lolop->next) and lolop = and
		Then, swap
*/
void swap_and_pattern(TABLEDATALIST ** table, CONDLIST * lolop, CONDLIST * search_condition, CONDLIST ** condlist_pool)
{
	CONDLIST 	** temp = condlist_pool;
	
	int 		count = 0;
	
	
	while(*(temp + count) !=  NULL)
	{
		count++;
	}
	
	if(lolop->op.compare("and") == 0)
		if(cond_stat(table, lolop->prev) >  cond_stat(table, lolop->next))
		{
			switch_cond(lolop->prev, lolop->next);
			*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
		}
}


/*
	Pattern:
		If, row_count(lolop->prev) < row_count(lolop->next) and lolop = or
		Then, swap
*/
void swap_or_pattern(TABLEDATALIST ** table, CONDLIST * lolop, CONDLIST * search_condition, CONDLIST ** condlist_pool)
{
	CONDLIST	** temp = condlist_pool;
	
	int 		count = 0;
	
	while(*(temp + count) !=  NULL)
	{
		count++;
	}
	if(lolop->op.compare("or") == 0)
		if(cond_stat(table, lolop->prev) <  cond_stat(table, lolop->next))
		{
			switch_cond(lolop->prev, lolop->next);
			*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
		}
}

void elim_or_and_pattern(TABLEDATALIST ** table, CONDLIST * lolop, CONDLIST * search_condition, CONDLIST ** condlist_pool)
{
	
	CONDLIST 	** temp = condlist_pool,
				* temp2;
	
	int count = 0;
	
	while(*(temp + count) !=  NULL)
	{
		count++;
	}
	
	if(lolop->prev->column_name.compare(lolop->next->column_name) == 0)
	{
		// > OR > , > AND > 
		if(lolop->prev->op.compare(">") == 0 && lolop->next->op.compare(">") == 0) 
		{
			if(lolop->prev->type.compare("int") == 0)
			{
				if(get_int(lolop->prev->value) < get_int(lolop->next->value))
				{
					if(lolop->op.compare("or") == 0)
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					else if(lolop->op.compare("and") == 0)
						temp2 = elim_cond(lolop, "PREV", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;
					
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
				else if(get_int(lolop->prev->value) > get_int(lolop->next->value))
				{
					if(lolop->op.compare("or") == 0)
						temp2 = elim_cond(lolop, "PREV", search_condition);
					else if(lolop->op.compare("and") == 0)
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
			}
		}
		// > OR = , > AND =
		else if(lolop->prev->op.compare(">") == 0 && lolop->next->op.compare("=") == 0)
		{
			if(lolop->prev->type.compare("int") == 0)
			{
				if(get_int(lolop->prev->value) < get_int(lolop->next->value))
				{
					if(lolop->op.compare("or") == 0)
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					else if(lolop->op.compare("and") == 0)
						temp2 = elim_cond(lolop, "PREV", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
			}
		}
		// = OR > , = AND >
		else if(lolop->prev->op.compare("=") == 0 && lolop->next->op.compare(">") == 0)
		{
			if(lolop->prev->type.compare("int") == 0)
			{
				if(get_int(lolop->prev->value) > get_int(lolop->next->value))
				{
					if(lolop->op.compare("or") == 0)
						temp2 = elim_cond(lolop, "PREV", search_condition);
					else if(lolop->op.compare("and") == 0)
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
			}
		}
		//< OR < , < AND <
		else if(lolop->prev->op.compare("<") == 0 && lolop->next->op.compare("<") == 0) 
		{
			if(lolop->prev->type.compare("int") == 0)
			{
				if(get_int(lolop->prev->value) > get_int(lolop->next->value))
				{
					if(lolop->op.compare("or") == 0)
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					else if(lolop->op.compare("and") == 0)
						temp2 = elim_cond(lolop, "PREV", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
				else if(get_int(lolop->prev->value) < get_int(lolop->next->value))
				{
					
					if(lolop->op.compare("or") == 0)
						temp2 = elim_cond(lolop, "PREV", search_condition);
					else if(lolop->op.compare("and") == 0)
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
			}
		}
		// < OR = , < AND =
		else if(lolop->prev->op.compare("<") == 0 && lolop->next->op.compare("=") == 0)
		{
			if(lolop->prev->type.compare("int") == 0)
			{
				if(get_int(lolop->prev->value) > get_int(lolop->next->value))
				{
					if(lolop->op.compare("or") == 0)	
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					else if(lolop->op.compare("and") == 0)	
						temp2 = elim_cond(lolop, "PREV", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;	
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
			}
		}
		// = OR <
		else if(lolop->prev->op.compare("=") == 0 && lolop->next->op.compare("<") == 0)
		{
			if(lolop->prev->type.compare("int") == 0)
			{
				if(get_int(lolop->prev->value) < get_int(lolop->next->value))
				{
					if(lolop->op.compare("or") == 0)
						temp2 = elim_cond(lolop, "PREV", search_condition);
					else if(lolop->op.compare("and") == 0)
						temp2 = elim_cond(lolop, "NEXT", search_condition);
					if(temp2 != NULL)
						search_condition = temp2;		
					*(condlist_pool + count++) = search_duplicate(search_condition, condlist_pool);
				}
			}
		}
	}
}


/*
	Eliminates condition lolop
*/
CONDLIST * elim_cond(CONDLIST * lolop, string node, CONDLIST * search = NULL)
{
	CONDLIST * temp = lolop,
			 * temp2;
			 
	bool t = false;
	
	if(search == lolop->prev)
		t = true;
		
	if(node.compare("PREV") == 0)
	{
		
		temp->prev->prev->next = temp->next;
		temp->next->prev = temp->prev->prev;
		temp2 = temp->next;
		free(temp->prev);
		free(temp);
		if(t)
			return temp2;
		else
			return search;
	}
	else if(node.compare("NEXT") == 0)
	{
		temp->next->next->prev = temp->prev;
		temp->prev->next = temp->next->next;
		temp2 = temp->prev;
		free(temp->next);
		free(temp);
		return search;
		
	}
}

/*
	Searches if 'search_condition' has a duplicate in 'condlist_pool'
*/
CONDLIST * search_duplicate(CONDLIST * search_condition, CONDLIST ** condlist_pool)
{
	CONDLIST	* temp = search_condition,
				* temp2;
			
	int 		i = 0;
	
	while(*(condlist_pool + i) != NULL)
	{
		temp2 = *(condlist_pool + i++);
		
		do
		{	
			if(!is_equal(temp, temp2))
			{
				temp = NULL;
				temp2 = NULL;
				break;
			}	
			temp2 = temp2->next;
			temp = temp->next;
		}while(temp != search_condition || temp2 != *(condlist_pool + i - 1));
		
		if(temp == search_condition || temp2 == *(condlist_pool + i - 1))
		{
			return NULL;
		}

		temp = search_condition;		
		
	}
	
	return search_condition;
}