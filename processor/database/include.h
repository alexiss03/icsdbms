//list of structures 
#include<iostream>
#include<string>
#include<locale>
#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<vector>
#include <sstream>
#include <fstream>
using namespace std;

namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

//structures
#include"processor/structures/queryparse.h"
#include"processor/structures/struct.h"
#include"processor/functions/functions_headers.h"

//global variables
TABLE ** table_pool;
BPT ** bpt_pool;
TABLEDATALIST ** last_accessed;
clock_t init, final;
float time_processed;

//testing
#include"processor/testing/testing.h"

//functions
//physical operators
#include"processor/functions.h"
#include"processor/functions/selectors/selector.h"
#include"processor/functions/selectors/condlist_selector.h"
#include"processor/functions/physical_operators.h"

#include"processor/functions/physical_operators/insert_physical_operator.h"
#include"processor/functions/physical_operators/update_physical_operator.h"
#include"processor/functions/physical_operators/delete_physical_operator.h"
#include"processor/functions/physical_operators/select_physical_operator.h"

#include"processor/functions/tables/table.h"
#include"processor/functions/convert_query_parse_tree_to_relational_algebra.h"
#include"processor/functions/statistical_data.h"

#include"processor/testing/printingfunctions.h"
#include"processor/testing/circular_testing.h"
#include"processor/functions/data/data_conversion.h"

#include"processor/functions/character_functions.h"
#include"processor/functions/mathematical_functions.h"

//optimization
#include"processor/functions/optimization/pattern_matching.h"
#include"processor/functions/optimization/patterns.h"
#include"processor/functions/optimization/selection.h"

//bpt
#include"processor/functions/bpt/bpt.h"
#include"processor/functions/bpt/integrated_bpt.h"

#include"processor/functions/physical_operators/join_operator.h"
#include"processor/functions/physical_operators/group_by_operator.h"

//aggregates
#include"processor/functions/physical_operators/aggregate_operators/sum_aggregate_operator.h"
#include"processor/functions/physical_operators/aggregate_operators/count_aggregate_operator.h"
#include"processor/functions/physical_operators/aggregate_operators/avg_aggregate_operator.h"
#include"processor/functions/physical_operators/aggregate_operators/first_aggregate_operator.h"
#include"processor/functions/physical_operators/aggregate_operators/last_aggregate_operator.h"
#include"processor/functions/physical_operators/aggregate_operators/max_aggregate_operator.h"
#include"processor/functions/physical_operators/aggregate_operators/min_aggregate_operator.h"

#include"processor/functions/physical_operators/set_operators/union_set_operator.h"
#include"processor/functions/physical_operators/set_operators/difference_set_operator.h"

//commit
#include"processor/functions/commit/commit.h"

//writing and reading
#include"database/rw_struct/concat_data.h"
#include"database/rw_struct/write_file.h"
#include"database/rw_struct/read_file.h"
#include"database/functions/include_functions.h"

