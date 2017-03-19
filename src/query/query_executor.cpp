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
	
	auto p_triple_query = sorted_triple_vec[0]	
	p_triple_query->select();	
	if(!p_triple_query->is_valid()){
		is_empty_result_ = true;
		return ;
	}
	visited_index_query_set.insert(0);
	do{
		size_t query_index = pick_next_query();
		sorted_triple_vec[i]->select();
		if(!sorted_triple_vec[i]->is_valid()){
			is_empty_result_ = true;
			return ;
		}
		visited_index_query_set.insert(query_index);
	}while(true);								
}

void QueryExecutor::get_min_growth_tree_by_kruskal(){
	std::vector<std::shared_ptr<TripleQuery>> sorted_triple_vec;
	for(auto p_triple_query : triple_query_set_){
		if(p_triple_query->var_vec_.size() < 2){
			continue;	
		}
		filter_query_vec.push_back(p_triple_query);	
	}
	//sorted	
	std::sort(sorted_triple_vec.begin(),sorted_triple_vec.end(),\
		[](std::shared_ptr<TripleQuery> p_a,std::shared_ptr<TripleQuery> p_b){
			return p_a->cur_valid_row_bm_index_cardinality_  \
				< p_b->cur_valid_row_bm_index_cardinality_;
});
	std::set<std::string> visited_var_node;
	for(auto p_triple_query : sorted_triple_vec){
		size_t find_count = 0;
		for(auto& var_name : p_triple_query->var_vec_){
			if(visited_var_node.find(var_name) != visited_var_node.end()){
				find_count ++;
				visited_var_node.insert(var_name);//trick
			}
		}
		if(find_count < 2){
			min_growth_tree_.push_back(p_triple_query);			
		}	
	}		
}

void QueryExecutor::shrink_min_growth_tree(){
	std::unorder_map<std::string,size_t> node_degree;
	for(auto p_triple_query : min_growth_tree_){
		for(auto& var_name : p_triple_query->var_vec_){
			node_degree[var_name] ++;
		}	
	}
	
	std::set<size_t> visited_triple_query;
	auto shrink_step = [&min_growth_tree_,&node_degree,&visited_triple_query](){
		for(size_t i = 0 ; i < min_growth_tree_.size() ; i++){
			if(visited_triple_query.find(i) != visited_triple_query.end()){
				continue;
			}
			auto p_triple_query = visited_triple_query[i];
			for(auto& var_name : p_triple_query->var_vec_){
				if(node_degree[var_name] == 1){
					std::string merged_var_name = p_triple_query->update_cartesian_product_in_shared_data(var_name);
					node_degree[var_name] --;
					node_degree[merged_var_name] --;		
					visited_triple_query.insert(i);
					break;
				}	
			}
		}};

	while(visited_triple_query.size() != min_growth_tree.size()){
		shrink_step();
	}
}

void QueryExecutor::make_result(){	
	size_t result_col_num_ = var_val_type_.size();
	auto make_empty_result = [&result_matrix_,\
				  result_col_num_](){
		std::vector<std::string> null_result;
		for(size_t i = 0 ; i < result_col_num_ ; i++){
			null_result.push_back("null");
		}
		result_matrix_.push_back(null_result);
	};

	if(is_empty_result_){
		make_empty_result();
		return;
	}
	
	if(-1 == get_min_growth_tree_by_kruskal()){
		make_empty_result();
		return;	
	}
	
	if(-1 == shrink_min_growth_tree()){
		make_empty_result();
		return;
	}
}
