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

	//select the less triple		
	auto p_triple_query = sorted_triple_vec[0]	
	p_triple_query->select();	

			
		
}

}
