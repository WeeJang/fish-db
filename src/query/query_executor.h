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
private:
	std::shared_ptr<fishdb::FishDBImpl> p_fish_db_;
	std::shared_ptr<SharedQueryData>    p_shared_query_data_;
	std::set<std::shared_ptr<TripleQuery>>		    triple_query_set_;
	bool is_finished_;
};//class QueryExector


}//query


#endif //QUERY_QUERY_EXECUTOR_H_
