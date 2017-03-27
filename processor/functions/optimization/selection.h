/*
	Computes each candidate's selectivity
	Selects the lowest selectibity as the best candidate
*/
CONDLIST * best_candidate_selector(TABLEDATALIST ** table_data, CONDLIST ** condlist)
{
	CONDLIST		* cond,
					* temp,
					* temp2;
			
	int				count = 0;
	
	int 			r_count,
					int_temp,
					index_min;
					
	float *			selectivity,
					minimum;

	if(condlist == NULL)
		return NULL;
	
	for(int i=0; *(condlist + i) != NULL; i++)
		count++;
	
	selectivity = (float *)malloc(sizeof(float) * count);
	
	for(int i=0; i<count; i++)	
		selectivity[i] = 0.000;
	
	for(int i=0; *(condlist + i) != NULL; i++)
	{
		cond = *(condlist + i);
		
		temp = cond;
		
		do
		{	
			if(temp->value != NULL)
			{
				int_temp = cond_stat(table_data, temp);
				if(int_temp != 0)
					selectivity[i] += (float)1 / (float)cond_stat(table_data, temp);
			}
			else if(temp->op.compare("and") == 0)
				 selectivity[i]  += (float)1/(float)20;
			else if(temp->op.compare("or") == 0)
				selectivity[i] += (float)1/(float)10;

			temp = temp->next;
		}while(temp != cond);
		
		if(i == 0)
		{
			minimum = selectivity[i];
			index_min = i;
		}
		else if(selectivity[i] < minimum)
		{
			minimum = selectivity[i];
			index_min = i;
		}
		
	}

	de_double_linked_list(condlist[index_min]);
	return condlist[index_min];
}