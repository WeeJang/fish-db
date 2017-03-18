#ifndef QUERY_QUERY_EXECUTOR_H_
#define QUERY_QUERY_EXECUTOR_H_

#include "triple_query.h"

#include <functional>
#include <memory>
#include <algorithm>
#include <set>


namespace query{

class QueryExecutor{

public:
	QueryExecutor(std::shared_ptr<fishdb::FishDBImpl> p_fish_db):p_fish_db_(p_fish_db){
	p_shared_query_data_ = std::make_shared<SharedQueryData>(p_fish_db_);
}
	void add_triple_query(std::shared_ptr<TripleQuery> p_triple_query){ triple_query_set_.insert(p_triple_query);}
	bool is_finished() const { return is_finished_; };
	void run();
	void make_result();

private:
	int get_min_growth_tree_by_kruskal();
	int shrink_min_growth_tree();

private:
	std::shared_ptr<fishdb::FishDBImpl> p_fish_db_;
	std::shared_ptr<SharedQueryData>    p_shared_query_data_;
	std::set<std::shared_ptr<TripleQuery>>		    triple_query_set_;
	bool is_finished_;
	bool is_empty_result_;
	size_t result_row_num_; // result_num
	size_t result_col_num_; // var_num
	std::vector<std::vector<std::string>> result_matrix_;
};//class QueryExector


}//query


#endif //QUERY_QUERY_EXECUTOR_H_
