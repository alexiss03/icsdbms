TABLEDATALIST ** 	get_table 					(	string tablename
												);
vector< pair <string,string> >  print_tabledatalist		(
														TABLEDATALIST ** tabledatalist
														);
void 				print_data 					(	TABLEDATALIST * data
												);
void 				print_data 					(	DATA * data
												);
bool 				circular_test				(	TABLEDATALIST ** tabledatalist
												);
TABLE * 			build_test_table			(
												);
void 				print_row					(	TABLEDATALIST * row
												);
void 				print_column				(	TABLEDATALIST * column)
												;

//functions/character_functions.h
bool				substr						(	TABLEDATALIST * column,
													int pos,
													int len
												);
bool 				replace						(	TABLEDATALIST * column,
													string tosearchfor,
													string replacewiththis 
												);
TABLEDATALIST *		length						(	TABLEDATALIST * column
												);
bool 				initcap						(	TABLEDATALIST * column
												);
TABLEDATALIST * 	concat						(	TABLEDATALIST * column1,
													TABLEDATALIST * column2,
													string newcolumname
												);
bool 				upper						(	TABLEDATALIST * column
												);
bool 				lower						(	TABLEDATALIST * column
												);

//functions/functions_headers.h
bool 				lower						(	TABLEDATALIST * column
												);
bool 				upper						(	TABLEDATALIST * column
												);
//table/table.h
TABLEDATALIST ** 	select_all_distinct_rows	(	TABLEDATALIST ** table,
													TABLEDATALIST * column
												);
TABLEDATALIST ** 	select_top_rows				(	TABLEDATALIST ** table,
													int top_rows
												);
bool 				delete_column				(	TABLEDATALIST ** table,
													 string column_name
												);
void * 				copy_data					( 	void * data,
													string type1,
													string type2
												);
TABLEDATALIST ** 	delete_row					(	TABLEDATALIST * row
												);
TABLEDATALIST ** 	select_all_rows				(	TABLEDATALIST ** table
												);
TABLEDATALIST * 	get_column					(	TABLEDATALIST * table,
													string column_name
												);
bool 				is_row_unique_null			(	TABLEDATALIST ** table, 
													TABLEDATALIST * row,
													TABLEDATALIST * end
												);
int 				no_of_columns				(	TABLEDATALIST ** table
												);
TABLEDATALIST * 	find_row_head				(	TABLEDATALIST ** table,
													TABLEDATALIST * cell
												);
bool 				create_new_cell				(	void * data,
													string type,
													TABLEDATALIST * next,
													TABLEDATALIST * prev,
													TABLEDATALIST * up,
													TABLEDATALIST * down
												);
TABLEDATALIST * 	make_row_data_null			(	TABLEDATALIST ** table,
													string column,
													TABLEDATALIST * end = NULL
												);
void 				connect_two_rows_horizontal(	TABLEDATALIST * row,
													TABLEDATALIST * row2
												);
void 				connect_two_rows_vertical	(	TABLEDATALIST * row,
													TABLEDATALIST * row2
												);
TABLEDATALIST *		 find_row_end				(	TABLEDATALIST * row
												);
TABLEDATALIST * 	copy_row					(	TABLEDATALIST * row
												);
TABLEDATALIST * 	find_column					(	TABLEDATALIST ** table,
													string column,
													TABLEDATALIST * end
												);
 bool 				remove_column				(	TABLEDATALIST ** table,
													DATA * columnlist
												);
 TABLEDATALIST ** 	get_table_header			(	TABLEDATALIST ** table
												);
 TABLEDATALIST ** 	create_new_tabledatalist	(	TABLEDATALIST ** table,
													TABLEDATALIST ** rows,
													DATA * columnlist
												);
 TABLEDATALIST ** 	get_table					(	string tablename
												);
 
//data/data_conversion.h
 
bool 				is_first_of_the_repeated_column	(DATA * columnlist,
												 DATA * current
												);
bool 				delete_first_duplicate		(	DATA * columnlist,
													string column
												);
bool 				is_equal					(	TABLEDATALIST * cell1,
													TABLEDATALIST * cell2
												);
bool 				is_equal					(	CONDLIST * node1,
													CONDLIST * node2
												);
bool 				is_void_equal				(	string type,
													void * data,
													void * data2
												);
string 				get_string					(	void * data
												);
int 				get_int						(	void * data
												);
float 				get_float					(	void * data
												);
int 				get_bool					(	void * data
												);
DATE 				get_date					(	void * data
												);
string 				convert_to_string			(	TABLEDATALIST * temp
												);


//physical_operators/update_physical_operator.h
bool  				update_op 					(	UPDATEQUERYPARSE query
												);
TABLEDATALIST **  	update						(	UPDATEQUERYPARSE query	
												);
bool 				update_physical_operator	(	TABLEDATALIST ** tabledatalist,
													DATA * column_value,
													TABLEDATALIST ** rows,
													string tablename
												);
										
//physical_operators/delete_physical_operator.h
bool 				delete_op					(	DELETEQUERYPARSE query
												);
bool 				delete_physical_operator	(	TABLEDATALIST ** tabledatalist,
													TABLEDATALIST ** rows
												);
												
//physical_operators/select_physical_operator.h	
TABLEDATALIST ** 	select_display_constraint	(	TABLEDATALIST ** table,
													DATA * display_constraint,
													DATA * columnlist
												);
bool 				select_display_op			(	TABLEDATALIST ** tabledatalist,
													DATA * columnlist,
													DATA * group_grouping
												);
bool 				select_display_alteration	(	TABLEDATALIST *column,
													string op, 
													DATA * value,
													DATA * group_grouping,
													TABLEDATALIST ** table
												);
bool 				select_alias				(	TABLEDATALIST ** table,
													DATA * tempColList
												);

												
//selector/condlist_selector.h
TABLEDATALIST ** 	condlist_selector			(	TABLEDATALIST ** table, 
													CONDLIST * search_condition,
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
int 				cond_stat					(	
													TABLEDATALIST ** table, 
													CONDLIST * search_condition,
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
												
//selector/selector.h
TABLEDATALIST ** 	selector					(	TABLEDATALIST ** table,
													string colname, 
													string op,
													TABLEDATALIST ** rows, 
													string lolop,
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
TABLEDATALIST ** 	selector					(	TABLEDATALIST ** table,
													string colname, 
													string op,
													string * value, 
													TABLEDATALIST ** rows, 
													string lolop, 
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
TABLEDATALIST ** 	selector					(	TABLEDATALIST ** table, 
													string colname,
													string type,
													void * lower,
													bool lBound,
													void * upper,
													bool uBound,
													TABLEDATALIST ** rows,
													string lolop,
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
TABLEDATALIST ** 	selector					(	TABLEDATALIST ** table
												);	
												
//physical_operators/insert_physical_operator.h
bool 				insert_op					(	INSERTQUERYPARSE query
												);
bool  				insert_physical_operator	(	TABLEDATALIST ** table,
													TABLEDATALIST * querydata,
													string inserttype,
													string tablename
												);
TABLEDATALIST * 	build_datalist				(	TABLEDATALIST ** header, 
													DATA * datalist,
													string tablename
												);

//physical_operators/alter_physical_operator.h
bool 					drop_column				(	string tablename,
													string column_name
												);
bool 					add_column				(	string tablename,
													string column_name,
													string type
												);

//optimization/pattern_matching.h
void 					de_double_linked_list	(	CONDLIST * search_condition
												);
void 					double_linked_list		(	CONDLIST * search_condition
												);
int  					count_lolop				(	CONDLIST * search_condition
												);
CONDLIST **				get_lolop				(	CONDLIST * search_condition
												);
void 					switch_cond				(	CONDLIST * prev,
													CONDLIST * next
												);
CONDLIST * 				recopy_cond				(	CONDLIST * condlist
												);
CONDLIST * 				locate_current_lolop	(	CONDLIST * lolop,
													CONDLIST * search_condition,
													CONDLIST * new_search_condition
												);
CONDLIST **				match_pattern			(	TABLEDATALIST ** table,
													CONDLIST * search_condition
												);
void 					pattern					(	CONDLIST * search_condition
												);

//optimization/patterns.h
CONDLIST * 				search_duplicate		(	CONDLIST * condlist,
													CONDLIST ** condlist_pool
												);
void 					elim_or_pattern			(	TABLEDATALIST ** table,
													CONDLIST * lolop,
													CONDLIST * condlist,
													CONDLIST ** condlist_pool
												);
void 					swap_or_pattern			(	TABLEDATALIST ** table,
													CONDLIST * lolop,
													CONDLIST * condlist,
													CONDLIST ** condlist_pool
												);
void					swap_and_pattern		(	TABLEDATALIST ** table,
													CONDLIST * lolop,
													CONDLIST * condlist,
													CONDLIST ** condlist_pool
												);
void 					elim_or_and_pattern		(	TABLEDATALIST ** table,
													CONDLIST * lolop,
													CONDLIST * search_condition,
													CONDLIST ** condlist_pool
												);
CONDLIST * 				elim_cond				(	CONDLIST * lolop,
													string node,
													CONDLIST * search
												);

//bpt/integrated_bpt.h
TABLE * 				get_table_from_table_pool(	string table_name
												);
TABLEDATALIST ** 		order_by				(	TABLEDATALIST ** tabledatalist,
													string tablename,
													DATA * order_grouping
												);
TABLEDATALIST 			** sort_desc			(	node * root,
													TABLEDATALIST ** table
												);
TABLEDATALIST 			** sort_asc				(	node * root,
													TABLEDATALIST ** table
												);
node 					* build_bpt_index		(	TABLEDATALIST ** table,
													TABLEDATALIST * column
												);
node 					* bpt_index_exists		(	string tablename, 
													string columname
												);
node 					* insert_to_bpt_pool	(	string table_name,
													string column_name, 
													node * root
												);

//statistical_data.h
void 					* column_min			(	string table_name,
													string column_name													
												);
void 					* column_max			(	string table_name,
													string column_name													
												);
int 					row_count				(	TABLEDATALIST ** table,
													string colname,
													string op,
													string * value,
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
int 					row_count				(	TABLEDATALIST ** table,
													string colname,
													string type,
													void * lower,
													bool lBound,
													void * upper,
													bool uBound,
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
int 					row_count				(	TABLEDATALIST ** table,
													string colname, 
													string op,
													TABLEDATALIST ** rows, 
													string lolop,
													TABLEDATALIST * column_search,
													TABLEDATALIST * row_search
												);
int 					row_count				(	TABLEDATALIST * table
												);
int 					col_count				(	TABLEDATALIST ** table
												);
//physical_operators/join_op.h
TABLEDATALIST ** 		full_join				(	TABLEDATALIST ** join_table,
													TABLEDATALIST * column,
													TABLEDATALIST * column2,
													string cond_op,
													int row_count_table1,
													int row_count_table2, 
													TABLEDATALIST * table2_start
												);
TABLEDATALIST ** 		right_join				(	TABLEDATALIST ** join_table,
													TABLEDATALIST * column,
													TABLEDATALIST * column2,
													string cond_op,
													int row_count_table1, 
													int row_count_table2, 
													TABLEDATALIST * table2_start
												);
TABLEDATALIST ** 		left_join				(	TABLEDATALIST ** join_table,
													TABLEDATALIST * column,
													TABLEDATALIST * column2,
													string cond_op,
													int row_count_table2, 
													TABLEDATALIST * table2_start
												);
TABLEDATALIST ** 		join_op					(	TABLE table,
													TABLE table2,
													CONDLIST * cond,
													string table_op
												);
TABLEDATALIST ** 		inner_join				(	TABLEDATALIST ** join_table,
													TABLEDATALIST * column,
													TABLEDATALIST * column2,
													string cond_op
												);
TABLEDATALIST * 		get_other_table			(	TABLEDATALIST ** table
												);
TABLEDATALIST ** 		join					(	TABLEDATALIST ** table,
													TABLEDATALIST ** table2
												);
						 						
//printing functions
void 					print_cond				(	CONDLIST  * cond
												);
bool					free_tabledatalist		(	TABLEDATALIST ** tabledatalist
												);

//physical_operators/sum_aggregate_operator.h
void * 					sum_aggregate_operator	(	TABLEDATALIST ** table,
													TABLEDATALIST * column,
													TABLEDATALIST * group_grouping
												);
void * 					sum_op					(	TABLEDATALIST ** table,
													TABLEDATALIST * start,
													TABLEDATALIST * end,
													TABLEDATALIST * column
												);

//physical_operators/count_aggregate_operator.h
void * 					count_aggregate_operator(	TABLEDATALIST ** table,
													TABLEDATALIST * column,
													TABLEDATALIST * group_grouping
												);
void * 					count_op				(	TABLEDATALIST ** table,
													TABLEDATALIST * start,
													TABLEDATALIST * end,
													TABLEDATALIST * column
												);
//physical_operators/avg_aggregate_operator.h
void * 					avg_aggregate_operator	(	TABLEDATALIST ** table,
													TABLEDATALIST * column,
													TABLEDATALIST * group_grouping
												);
void * 					avg_op					(	TABLEDATALIST ** table,
													TABLEDATALIST * start,
													TABLEDATALIST * end,
													TABLEDATALIST * column
												);
												
//physical_operators/first_aggregate_operator.h
void * 					first_aggregate_operator(	TABLEDATALIST ** table,
													TABLEDATALIST * column,
													TABLEDATALIST * group_grouping
												);
void * 					first_op				(	TABLEDATALIST ** table, 
													TABLEDATALIST * start, 
													TABLEDATALIST * end, 
													TABLEDATALIST * column
												);

//physical_operators/last_aggregate_operator.h
void * 					last_aggregate_operator	(	TABLEDATALIST ** table,
													TABLEDATALIST * column,
													TABLEDATALIST * group_grouping
												);
void * 					last_op					(	TABLEDATALIST ** table, 
													TABLEDATALIST * start, 
													TABLEDATALIST * end, 
													TABLEDATALIST * column
												);

//physical_operators/max_aggregate_operator.h
void * 					max_aggregate_operator	(	TABLEDATALIST ** table,
													TABLEDATALIST * column,
													TABLEDATALIST * group_grouping
												);
void * 					max_op					(	TABLEDATALIST ** table, 
													TABLEDATALIST * start, 
													TABLEDATALIST * end, 
													TABLEDATALIST * column
												);
												
//physical_operators/min_aggregate_operator.h
void * 					min_aggregate_operator	(	TABLEDATALIST ** table,
													TABLEDATALIST * column,
													TABLEDATALIST * group_grouping
												);
void * 					min_op					(	TABLEDATALIST ** table, 
													TABLEDATALIST * start, 
													TABLEDATALIST * end, 
													TABLEDATALIST * column
												);

//physical_operators/set_aggregate_operator.h
TABLEDATALIST ** 		union_set_operator		(	TABLEDATALIST ** table1,
													TABLEDATALIST ** table2
												);
TABLEDATALIST ** 		union_all_set_operator	(	TABLEDATALIST ** table1,
													TABLEDATALIST ** table2
												);

//optimization/header
CONDLIST * 				best_candidate_selector	(	TABLEDATALIST ** table_data, 
													CONDLIST ** condlist
												);

//functions/physical_operators/group_by_operator.h
TABLEDATALIST ** 		group_by_operator		(	TABLEDATALIST ** table,
													CONDLIST * condition, 
													DATA * column,
													DATA * columnlist
												);
TABLEDATALIST ** 		group_by 				(	TABLEDATALIST ** table,
													DATA * column,
													DATA * columnlist,	
													CONDLIST * condition
												);

//functions/commit/commit
void 					clear_bpt_pool			();
void 					commit					();

//writing
void 					write_tabledatalist		(	TABLEDATALIST ** tabledatalist,
													 const char* table_name
												);
TABLEDATALIST ** 		read_file_start			(	const char* table_name
												);
