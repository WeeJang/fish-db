#include "../src/core/fish_db_impl.h"
#include "../src/query/query_executor.h"
#include "../src/query/triple_query.h"

#include "../src/utils/tiny_log.hpp"

#include <iostream>

std::string sparql_str_1 = "\
	select ?id ?name ?val where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
           ?id <http://rdf.dingfu.com/ns/business.business_operation.accounts_receivable> ?val \
        }";

std::string sparql_str_2 = "\
	select ?id ?name  where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.isin> cne100001zg7 .  \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
        }";

//6f1c42c2d2b919

int main(int argc,char** argv){
	LOG("begin");
	auto p_fishdb = std::make_shared<fishdb::FishDBImpl>();
	p_fishdb->open_db("test_db");
	LOG("open finish");
	query::QueryExecutor query_exec(p_fishdb);
	LOG("query exec init");
	//auto p_shared_data = std::make_shared<query::SharedQueryData>(p_fishdb);
	auto p_shared_data = query_exec.get_shared_query_data_ptr();
	LOG("p_shared_data finished");
	auto query_set = query::make_triple_query_set(sparql_str_2,p_shared_data);
	LOG("query set");
	for(auto p_query : query_set){
		query_exec.add_triple_query(p_query);
	}	
	query_exec.run();
	LOG("query query exec");
	query_exec.make_result();
	LOG("query make_result");
	p_fishdb->close_db();
	return 0;
}

