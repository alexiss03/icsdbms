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
#include<iostream>   // std::cout
#include<string>
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
//#include"structures/queryparse.h"
//#include"parser/queryparse.hpp"
#include"functions/functions_headers.h"

//global variables
TABLE ** table_pool;
BPT ** bpt_pool;
TABLEDATALIST ** last_accessed;
clock_t init, final;
float time_processed;

//testing
#include"testing/testing.h"

//functions
//physical operators
//#include"functions.h"
#include"functions/selectors/selector.h"
#include"functions/selectors/condlist_selector.h"

#include"database/functions/include_functions.h"
#include"database/rw_struct/write_file.h"
#include"database/rw_struct/read_file.h"
#include"database/queryprocessing_db.h"

#include"functions/physical_operators/insert_physical_operator.h"
#include"functions/physical_operators/update_physical_operator.h"
#include"functions/physical_operators/delete_physical_operator.h"
#include"functions/physical_operators/select_physical_operator.h"
#include"functions/physical_operators/alter_physical_operator.h"

#include"functions/tables/table.h"
#include"functions/statistical_data.h"

#include"testing/printingfunctions.h"
#include"testing/circular_testing.h"
#include"functions/data/data_conversion.h"

#include"functions/character_functions.h"
#include"functions/mathematical_functions.h"

//optimization
#include"functions/optimization/pattern_matching.h"
#include"functions/optimization/patterns.h"
#include"functions/optimization/selection.h"

//bpt
#include"functions/bpt/bpt.h"
#include"functions/bpt/integrated_bpt.h"

#include"functions/physical_operators/join_operator.h"
#include"functions/physical_operators/group_by_operator.h"

//aggregates
#include"functions/physical_operators/aggregate_operators/sum_aggregate_operator.h"
#include"functions/physical_operators/aggregate_operators/count_aggregate_operator.h"
#include"functions/physical_operators/aggregate_operators/avg_aggregate_operator.h"
#include"functions/physical_operators/aggregate_operators/first_aggregate_operator.h"
#include"functions/physical_operators/aggregate_operators/last_aggregate_operator.h"
#include"functions/physical_operators/aggregate_operators/max_aggregate_operator.h"
#include"functions/physical_operators/aggregate_operators/min_aggregate_operator.h"

#include"functions/physical_operators/set_operators/union_set_operator.h"
#include"functions/physical_operators/set_operators/difference_set_operator.h"

//commit
#include"functions/commit/commit.h"

//writing
