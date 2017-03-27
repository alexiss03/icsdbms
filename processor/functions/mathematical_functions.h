bool round(TABLEDATALIST * column, int place = 0)
{
	TABLEDATALIST 			* temp;
	
	float					floatTemp;
	
	if(column == NULL)
		return false;
	else
	{
		
		temp = column;
		temp = temp->down->down;
		if(temp->type.compare("int") == 0 || temp->type.compare("float") == 0)
		{
			do
			{
				if(temp->data != NULL)
				{
					floatTemp = convert_to_float(temp);
					round(floatTemp);
					temp->type = "float";
				}
				temp = temp->down;
			}while(temp != column);
		}
	}
	return true;	
}
