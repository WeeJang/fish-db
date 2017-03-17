#include "query_executor.h"

namespace query{

void QueryExecutor::run(){
	if(is_finished_){
		return;
	}
	if(triple_query_set_.size() == 0){
		is_finished_ = true;
		return;
	}
	for(auto p_triple : triple_query_set_){
		if(!p_triple->is_valid()){
			is_finished_ = true;
			return;
		}	
	}
	//sorted	
	std::vector<std::shared_ptr<TripleQuery>> sorted_triple_vec(triple_query_set_.begin(),triple_query_set_.end());	
	std::sort(sorted_triple_vec.begin(),sorted_triple_vec.end(),\
		[](std::shared_ptr<TripleQuery> p_a,std::shared_ptr<TripleQuery> p_b){
			return p_a->cur_valid_row_bm_index_cardinality_  \
				< p_b->cur_valid_row_bm_index_cardinality_;
});
	std::set<size_t> visited_query_index_set;
	std::vector<std::string> visited_var_vec;
	//select the less triple		
	auto p_triple_query = sorted_triple_vec[0]	
	p_triple_query->select();	
	for(auto& var_name : p_triple_query->var_vec()){
		p_triple_query->union_update_shared_data(var_name);
		visited_var_vec.push_back(var_name);
	}
	visited_index_query_set.insert(0);
		
	auto pick_next_query = [&sorted_triple_vec,\
				&visited_index_query_set,\
				&visited_var_vec](){
			for(size_t i = 1 ; i < sorted_triple_vec.size() ; i ++){
				if(visited_index_query_set.find(i) != visited_index_query_set.end()){
					continue;
				}
				for(auto& visited_var : visited_var_vec){
					if(sorted_triple_vec[i].contain_vars(visited_var)){
						return i;
					}
				}
			}
			return -1;
		};	
	
	do{
		size_t query_index = pick_next_query();
		sorted_triple_vec[i]->select();
		visited_index_query_set.insert(query_index);
	}while(true);								
				
}

}
