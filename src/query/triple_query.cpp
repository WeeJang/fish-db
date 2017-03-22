#include "triple_query.h"

namespace query{


void SharedQueryData::printf_var_val_type(){
	LOG("printf var val type {{{=========");
	for(auto iter = var_val_type_.begin(); iter != var_val_type_.end(); iter++){
		std::cout << iter->first << "\t"  ;
		std::cout << static_cast<int>(iter->second) << std::endl;
	}	
	LOG("printf var val type =========}}}");
}
void SharedQueryData::printf_hv_bound_vals(){
	LOG("printf hv bound vals {{{=========");
	for(auto iter = hv_bound_vals_.begin(); iter != hv_bound_vals_.end(); iter++){
		std::cout << iter->first << ": (" ;
		for(auto& elem : iter->second){
			std::cout << elem << ",";
		}
		std::cout << ")" << std::endl;
	}	
	LOG("printf hv bound vals =========}}}");

}
void SharedQueryData::printf_ss_bound_vals(){
	LOG("printf ss bound vals {{{=========");
	for(auto iter = ss_bound_vals_.begin(); iter != ss_bound_vals_.end(); iter++){
		std::cout << iter->first << ": (" ;
		for(auto& elem : iter->second){
			std::cout << elem << ",";
		}
		std::cout << ")" << std::endl;
	}	
	LOG("printf ss bound vals =========}}}");

}
void SharedQueryData::printf_intermediate_result(){
	LOG("printf itermediate result  {{{=========");
	for(auto iter = intermediate_result_.begin(); iter != intermediate_result_.end(); iter++){
		std::cout << "var: " << iter->first << std::endl;
		auto var_val_map = iter->second;
		for(auto val_ter = var_val_map.begin(); val_ter != var_val_map.end(); val_ter ++){
			std::cout << "\t val:[" << val_ter->first << "]" << "--->[";
			for(auto& elem : val_ter->second){
				std::cout << elem << ",";
			}
			std::cout << "]" << std::endl;
			}

	}	
	LOG("printf intermediate result  =========}}}");

}
void SharedQueryData::printf_intermediate_result_col_name(){
	LOG("printf intermedaite result col name {{{=========");
	for(auto iter = intermediate_result_col_name_.begin(); iter != intermediate_result_col_name_.end(); iter++){
		std::cout << "var: " << iter->first << " ->[";
		for(auto& elem : iter->second){
			std::cout  << elem << ",";
		}
		std::cout << "]" << std::endl;
	}	
	LOG("printf intermediate result col name =========}}}");

}


int SharedQueryData::make_cartesian_product_by_filter_vector_linked(std::string another_var_name,std::string var_name,\
							std::vector<std::string> (&filter_var_val_vector)[2]){

	auto got_another_var_intermediate_result = intermediate_result_.find(another_var_name);
	auto got_var_intermediate_result = intermediate_result_.find(var_name);
	std::unordered_map<std::string,std::vector<std::string>> another_val_2_var_val_map;
	if(got_another_var_intermediate_result == intermediate_result_.end() && \
	   got_var_intermediate_result == intermediate_result_.end()){
		for(size_t i = 0 ; i < filter_var_val_vector[0].size(); i++){
			another_val_2_var_val_map[filter_var_val_vector[0][i]].push_back(filter_var_val_vector[1][i]);
		} intermediate_result_col_name_[another_var_name].push_back(var_name);	
	}else if(got_another_var_intermediate_result == intermediate_result_.end() && \
		 got_var_intermediate_result != intermediate_result_.end()){
		auto var_map = got_var_intermediate_result->second;
		for(size_t i = 0 ; i < filter_var_val_vector[0].size(); i++){
			std::string& another_var_val = filter_var_val_vector[0][i];
			std::string& var_val = filter_var_val_vector[1][i];
			auto var_val_2_vec = var_map.find(var_val);
			if(var_val_2_vec == var_map.end()){
				continue;
			}else{
				std::vector<std::string> val_2_vec = var_val_2_vec->second;
				for(auto& val : val_2_vec){
					another_val_2_var_val_map[another_var_val].push_back(std::string(var_val).append("\t").append(val));		
				}
			}	
		}
		intermediate_result_col_name_[another_var_name].push_back(var_name);	
		for(auto& col_name : intermediate_result_col_name_[var_name]){
			intermediate_result_col_name_[another_var_name].push_back(col_name);	
		}
	}else if(got_another_var_intermediate_result != intermediate_result_.end() && \
		 got_var_intermediate_result == intermediate_result_.end()){
		auto another_var_map = got_another_var_intermediate_result->second;
		for(size_t i = 0 ; i < filter_var_val_vector[0].size(); i++){
			std::string&  another_var_val = filter_var_val_vector[0][i];
			std::string& var_val = filter_var_val_vector[1][i];
			auto another_var_val_2_vec = another_var_map.find(another_var_val);
			if(another_var_val_2_vec == another_var_map.end()){
				continue;
			}else{
				std::vector<std::string> val_2_vec = another_var_val_2_vec->second;
				for(auto val : val_2_vec){
					another_val_2_var_val_map[another_var_val].push_back(std::string(val).append("\t").append(var_val));;
				}
			}
					
		}
		intermediate_result_col_name_[another_var_name].push_back(var_name);	
	}else if(got_another_var_intermediate_result != intermediate_result_.end() && \
		 got_var_intermediate_result != intermediate_result_.end()){
		auto another_var_map = got_another_var_intermediate_result->second;
		auto var_map = got_var_intermediate_result->second;
		for(size_t i = 0 ; i < filter_var_val_vector[0].size(); i++){
			std::string& another_var_val = filter_var_val_vector[0][i];
			std::string& var_val = filter_var_val_vector[1][i];
			auto another_var_val_2_vec = another_var_map.find(another_var_val);
			auto var_val_2_vec = var_map.find(var_val);
			if(var_val_2_vec == var_map.end() || another_var_val_2_vec == another_var_map.end()){
				continue;
			}else{
				std::vector<std::string> val_vec = var_val_2_vec->second;
				std::vector<std::string> another_val_vec = another_var_val_2_vec->second;
				for(auto& another_val : another_val_vec){
					for(auto& val : val_vec){
						another_val_2_var_val_map[another_var_val]\
							.push_back(std::string(another_val).append("\t")\
									.append(var_val).append("\t").append(val));
					}
				}

			}
					
		}
		intermediate_result_col_name_[another_var_name].push_back(var_name);	
		for(auto& col_name : intermediate_result_col_name_[var_name]){
			intermediate_result_col_name_[another_var_name].push_back(col_name);	
		}
	}
	intermediate_result_[another_var_name] = another_val_2_var_val_map;
	return 0;
}


TripleQuery::TripleQuery(HashValue sub, ShortString pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(pre.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::HV;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::UD;
	var_vec_.push_back(obj);
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
	init();
}
	
TripleQuery::TripleQuery(HashValue sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::HV;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::HV;
	var_vec_.push_back(pre);
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}
	


TripleQuery::TripleQuery(HashValue sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::HV;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::SS;
	var_vec_.push_back(pre);
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}



TripleQuery::TripleQuery(HashValue sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::HV;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::UD;
	var_vec_.push_back(pre);
	var_vec_.push_back(obj);
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
	init();
}
	
TripleQuery::TripleQuery(ShortString sub, ShortString pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(pre.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::UD;
	var_vec_.push_back(obj);
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
	init();
}


TripleQuery::TripleQuery(ShortString sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::HV;
	var_vec_.push_back(pre);
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}




TripleQuery::TripleQuery(ShortString sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::SS;
	var_vec_.push_back(pre);
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}


TripleQuery::TripleQuery(ShortString sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::UD;
	var_vec_.push_back(pre);
	var_vec_.push_back(obj);
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
	init();
}
	
TripleQuery::TripleQuery(Variable sub, ShortString pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(pre.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::pre);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::HV;
	var_vec_.push_back(sub);
	var_pos_[sub] = core::TripleElemPos::sub;
	p_shared_ = p_shared;
	init();
}


TripleQuery::TripleQuery(Variable sub, ShortString pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(pre.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::pre);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::SS;
	var_vec_.push_back(sub);
	var_pos_[sub] = core::TripleElemPos::sub;
	p_shared_ = p_shared;
	init();
}

TripleQuery::TripleQuery(Variable sub, ShortString pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(pre.value());
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::SS;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::UD;
	var_vec_.push_back(sub);
	var_vec_.push_back(obj);
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
	init();
}

TripleQuery::TripleQuery(Variable sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::HV;
	var_vec_.push_back(sub);
	var_vec_.push_back(pre);
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}

TripleQuery::TripleQuery(Variable sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::SS;
	var_vec_.push_back(sub);
	var_vec_.push_back(pre);
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}

TripleQuery::TripleQuery(Variable sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	spo_vec_iri_type_tag_[0] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[1] = IRITypeUnionTag::UD;
	spo_vec_iri_type_tag_[2] = IRITypeUnionTag::HV;
	var_vec_.push_back(sub);
	var_vec_.push_back(pre);
	var_vec_.push_back(obj);
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
	init();
}

int TripleQuery::get_iri_index(IRITypeUnion_T iri_variant,IRITypeUnionTag typetag,core::TripleElemPos pos,BitMap_T& ret_bitmap){
	if(IRITypeUnionTag::HV == typetag){
		auto iri_value = boost::get<HV_T>(iri_variant);	
		auto p_iri_index = p_shared_->p_fish_db_->hv_meta_iri_index_.get_IRI_index(iri_value);
		if(!p_iri_index){
			return -1;
		}else{
			ret_bitmap = p_iri_index->at(pos);
			return 0;	
		}
	}else if(IRITypeUnionTag::SS == typetag){
		auto iri_value = boost::get<SS_T>(iri_variant);	
		LOG("ss value %s",iri_value.c_str());
		auto p_iri_index = p_shared_->p_fish_db_->ss_meta_iri_index_.get_IRI_index(iri_value);
		if(!p_iri_index){
			return -1;
		}else{
			ret_bitmap = p_iri_index->at(pos);
			return 0;	
		}
	}else{
		fprintf(stderr,"get_iri_index error\n");
		exit(-1);
	}	
}


void TripleQuery::init(){
	if(iri_vec_.size() == 0){
		LOG("size == 0");
		return;
	}
	BitMap_T bit_map;	
	for(size_t i = 0 ; i < iri_vec_.size() ; i ++){
		LOG("init() : %d",i);
		auto iri_variant = iri_vec_[i];
		auto iri_elem_pos = iri_pos_[i];
		auto iri_type_tag = spo_vec_iri_type_tag_[static_cast<int>(iri_elem_pos)];
		if(-1 == get_iri_index(iri_variant,iri_type_tag,iri_elem_pos,bit_map)){
			LOG("get iri index == -1");
			is_valid_ = false;
			return;
		}
		LOG("bit map:{{{=========");
		bit_map.printf();
		LOG("=========}}}");
		if(0 == i){
			cur_valid_row_bm_index_ = bit_map;
		}else{
			cur_valid_row_bm_index_ &= bit_map;
		}
		cur_valid_row_bm_index_.printf();
	}
	cur_valid_row_bm_index_cardinality_ = cur_valid_row_bm_index_.cardinality();
	LOG("cur_valid_row_bm_index_cardinality : %d ",cur_valid_row_bm_index_cardinality_);
	is_valid_ = true;
}


//完善select 出来的vec的type tag
void TripleQuery::improve_spo_vec_iri_type_tag(std::shared_ptr<core::TripleSpec> p_triple){
	for(size_t i = 0 ; i < 3 ; i ++){
		if(IRITypeUnionTag::UD == spo_vec_iri_type_tag_[i]){
			if(core::IRIType::is_hashvalue(p_triple->at(static_cast<core::TripleElemPos>(i)).c_str(),p_triple->at(static_cast<core::TripleElemPos>(i)).size())){
				spo_vec_iri_type_tag_[i] = IRITypeUnionTag::HV;	
			}else{
				spo_vec_iri_type_tag_[i] = IRITypeUnionTag::SS;
			}
		}
	}
}

void TripleQuery::select_new_triple(std::shared_ptr<core::TripleSpec> p_triple_spec){
	LOG("select new triple :%s",p_triple_spec->to_string().c_str());
	for(auto iter = var_pos_.begin(); iter != var_pos_.end() ; iter++){
		auto var_name = iter->first;
		auto var_pos  = iter->second;
		auto var_tag  = spo_vec_iri_type_tag_[static_cast<int>(var_pos)];
		if(IRITypeUnionTag::HV == var_tag){
			HV_T hv_value = ::strtol(p_triple_spec->at(var_pos).c_str(),nullptr,16);
			select_spo_vec_[static_cast<int>(var_pos)].push_back(hv_value);
		}else if(IRITypeUnionTag::SS == var_tag){
			select_spo_vec_[static_cast<int>(var_pos)].push_back(p_triple_spec->at(var_pos));
		}else{
			fprintf(stderr,"select new triple error!");
			exit(-1);
		}
	}
}

std::string TripleQuery::update_cartesian_product_in_shared_data(std::string var_name){
	//进入这一步的肯定有且只有两个var
	assert(var_vec_.size() == 2);
	std::string another_var_name;
	for(auto& var : var_vec_){
		if(0 != var.compare(var_name)){
			another_var_name = var;
			break;
		}	
	}
	auto var_pos = var_pos_[var_name];
	auto another_pos = var_pos_[another_var_name];
	auto var_tag = spo_vec_iri_type_tag_[static_cast<int>(var_pos)];
	auto another_tag = spo_vec_iri_type_tag_[static_cast<int>(another_pos)];

	p_shared_->intermediate_result_col_name_[another_var_name].push_back(var_name);
	
	std::unordered_map<std::string,std::vector<std::string>> another2var_map;

	std::vector<std::string> filter_var_val_vector[2];//0 : anther_var, 1: var
	//check if another_var_name in shared_data.intermidiate_result	
	if(another_tag == IRITypeUnionTag::HV && var_tag == IRITypeUnionTag::HV){
		for(size_t i = 0 ; i < select_spo_vec_[static_cast<int>(var_pos)].size() ; i++){
			auto another_val = boost::get<HV_T>(select_spo_vec_[static_cast<int>(another_pos)][i]);
			auto var_val     = boost::get<HV_T>(select_spo_vec_[static_cast<int>(var_pos)][i]);
			if(p_shared_->hv_bound_vals_[another_var_name].find(another_val) \
				== p_shared_->hv_bound_vals_[another_var_name].end()){ 
				continue;
			}
			if(p_shared_->hv_bound_vals_[var_name].find(var_val) \
				== p_shared_->hv_bound_vals_[var_name].end()){
				continue;
			}
			std::string another_str = std::to_string(another_val);
			std::string var_str     = std::to_string(var_val);
			filter_var_val_vector[0].push_back(another_str);
			filter_var_val_vector[1].push_back(var_str);

		}	
	}else if(another_tag == IRITypeUnionTag::HV && var_tag == IRITypeUnionTag::SS){
		for(size_t i = 0 ; i < select_spo_vec_[static_cast<int>(var_pos)].size() ; i++){
			auto another_val = boost::get<HV_T>(select_spo_vec_[static_cast<int>(another_pos)][i]);
			auto var_val     = boost::get<SS_T>(select_spo_vec_[static_cast<int>(var_pos)][i]);
			if(p_shared_->hv_bound_vals_[another_var_name].find(another_val) \
				== p_shared_->hv_bound_vals_[another_var_name].end()){
				continue;
			}
			if(p_shared_->ss_bound_vals_[var_name].find(var_val) \
				== p_shared_->ss_bound_vals_[var_name].end()){
				continue;
			}
			std::string another_str = std::to_string(another_val);
			filter_var_val_vector[0].push_back(another_str);
			filter_var_val_vector[1].push_back(var_val);
		}	
	}else if(another_tag == IRITypeUnionTag::SS && var_tag == IRITypeUnionTag::SS){
		for(size_t i = 0 ; i < select_spo_vec_[static_cast<int>(var_pos)].size() ; i++){
			auto another_val = boost::get<SS_T>(select_spo_vec_[static_cast<int>(another_pos)][i]);
			auto var_val     = boost::get<SS_T>(select_spo_vec_[static_cast<int>(var_pos)][i]);
			if(p_shared_->ss_bound_vals_[another_var_name].find(another_val) \
				== p_shared_->ss_bound_vals_[another_var_name].end()){
				continue;
			}
			if(p_shared_->ss_bound_vals_[var_name].find(var_val) \
				== p_shared_->ss_bound_vals_[var_name].end()){
				continue;
			}
			filter_var_val_vector[0].push_back(another_val);
			filter_var_val_vector[1].push_back(var_val);
		}	
	}else if(another_tag == IRITypeUnionTag::SS && var_tag == IRITypeUnionTag::HV){
		for(size_t i = 0 ; i < select_spo_vec_[static_cast<int>(var_pos)].size() ; i++){
			auto another_val = boost::get<SS_T>(select_spo_vec_[static_cast<int>(another_pos)][i]);
			auto var_val     = boost::get<HV_T>(select_spo_vec_[static_cast<int>(var_pos)][i]);
			if(p_shared_->ss_bound_vals_[another_var_name].find(another_val) \
				== p_shared_->ss_bound_vals_[another_var_name].end()){
				continue;
			}
			if(p_shared_->hv_bound_vals_[var_name].find(var_val) \
				== p_shared_->hv_bound_vals_[var_name].end()){
				continue;
			}
			std::string var_str     = std::to_string(var_val);
			filter_var_val_vector[0].push_back(another_val);
			filter_var_val_vector[1].push_back(var_str);
		}	
	}else{
		fprintf(stderr,"update cartesian product error\n");
		exit(-1);
	}

	p_shared_->make_cartesian_product_by_filter_vector_linked(another_var_name,var_name,filter_var_val_vector);
	return another_var_name;	
}




void TripleQuery::union_update_shared_data(std::string var_name){
	auto var_pos = var_pos_[var_name];
	auto var_tag = spo_vec_iri_type_tag_[static_cast<int>(var_pos)];
	auto val_vec = select_spo_vec_[static_cast<int>(var_pos)];
	
	p_shared_->var_val_type_[var_name] = var_tag;	
	if(IRITypeUnionTag::HV == var_tag){
		for(auto& elem : val_vec){
			p_shared_->hv_bound_vals_[var_name].insert(boost::get<HV_T>(elem));
		}
	}else if(IRITypeUnionTag::SS == var_tag){
		for(auto& elem : val_vec){
			p_shared_->ss_bound_vals_[var_name].insert(boost::get<SS_T>(elem));
		}
	}else{
		fprintf(stderr,"union update shared data error!\n");
		exit(-1);
	}
}

void TripleQuery::join_update_shared_data(std::string var_name){
	auto var_pos = var_pos_[var_name];
	auto var_tag = spo_vec_iri_type_tag_[static_cast<int>(var_pos)];
	auto val_vec = select_spo_vec_[static_cast<int>(var_pos)];
	
	if(IRITypeUnionTag::HV == var_tag){
		auto hv_var_vals_set = p_shared_->hv_bound_vals_[var_name];
		p_shared_->hv_bound_vals_[var_name].clear();
		for(auto& elem : val_vec){
			HV_T hv_elem = boost::get<HV_T>(elem);
			if(hv_var_vals_set.find(hv_elem) != hv_var_vals_set.end()){
				p_shared_->hv_bound_vals_[var_name].insert(hv_elem);
			}
		}
	}else if(IRITypeUnionTag::SS == var_tag){
		auto ss_var_vals_set = p_shared_->ss_bound_vals_[var_name];
		p_shared_->ss_bound_vals_[var_name].clear();
		for(auto& elem : val_vec){
			SS_T ss_elem = boost::get<SS_T>(elem);
			if(ss_var_vals_set.find(ss_elem) != ss_var_vals_set.end()){
				p_shared_->ss_bound_vals_[var_name].insert(ss_elem);
			}
		}
	}else{
		fprintf(stderr,"join update shared data error!\n");
		exit(-1);
	}

}


//根据现有信息，从db 中取数据
void TripleQuery::select(){
	shrink_cur_valid_row_bm();
	
	auto p_db = p_shared_->p_fish_db_;
	auto p_triple_spec = std::make_shared<core::TripleSpec>(); 
	auto rowindex_bm = new uint64_t[cur_valid_row_bm_index_cardinality_];
	cur_valid_row_bm_index_.toUint64Array(rowindex_bm);	
	for(size_t i = 0 ; i < cur_valid_row_bm_index_cardinality_ ; i++){
		p_db->get_triple_by_row_index(rowindex_bm[i],p_triple_spec);
		if(0 == i){
			improve_spo_vec_iri_type_tag(p_triple_spec);
		}
		select_new_triple(p_triple_spec);				
	}
	delete [] rowindex_bm;
	printf_select_spo_vec();

	for(auto var_name : var_vec_){
		if(p_shared_->var_val_type_.find(var_name) == p_shared_->var_val_type_.end()){
			union_update_shared_data(var_name);
		}else{
			join_update_shared_data(var_name); 
		}	
	}
	
	//for debug
	p_shared_->printf_var_val_type();
	p_shared_->printf_hv_bound_vals();
	p_shared_->printf_ss_bound_vals();
	p_shared_->printf_intermediate_result();
	p_shared_->printf_intermediate_result_col_name();
}

void TripleQuery::printf_select_spo_vec(){
	LOG("select result {{{====================");		
	for(size_t i = 0 ; i < 3 ; i ++){
		std::cout << i << " | " << "\t";
		for(size_t j = 0 ; j < select_spo_vec_[i].size() ; j++){
			std::cout << select_spo_vec_[i][j] << "\t" ;
		}
		std::cout << std::endl;
	}
	LOG("select result ====================}}}");	
}


void TripleQuery::shrink_cur_valid_row_bm(){
	auto p_fish_db = p_shared_->p_fish_db_;
	for(auto var_name : var_vec_){
		if(p_shared_->var_val_type_.find(var_name) == p_shared_->var_val_type_.end()){
			continue;
		}
		auto var_pos = var_pos_[var_name];
		BitMap_T union_bm;
		auto var_tag = spo_vec_iri_type_tag_[static_cast<int>(var_pos)];
		if(var_tag == IRITypeUnionTag::HV){
			auto var_set = p_shared_->hv_bound_vals_[var_name];
			for(auto var_set_elem : var_set){
				union_bm |= p_fish_db->hv_meta_iri_index_.get_IRI_index(var_set_elem)->at(var_pos);
			}
		}else if(var_tag == IRITypeUnionTag::SS){
			auto var_set = p_shared_->ss_bound_vals_[var_name];
			for(auto var_set_elem : var_set){
				union_bm |= p_fish_db->ss_meta_iri_index_.get_IRI_index(var_set_elem)->at(var_pos);
			}
		}else{
			fprintf(stderr,"shink_cur_valid_row_bm error!\n");
			exit(-1);
		}
		cur_valid_row_bm_index_ &= union_bm;
	}			
}

//helper function
SPARQLParserType_T make_typed_value(sparql::SPARQLParser::TriplePatternElem& triple_elem,SPARQLParserTypeTag& tag){
	SPARQLParserType_T ret;	
	if(triple_elem.type_ == sparql::SPARQLParser::TriplePatternElem::Type::Variable){
		tag = SPARQLParserTypeTag::VAR;	
		ret = query::Variable(triple_elem.value_);	
	}else if(triple_elem.type_ == sparql::SPARQLParser::TriplePatternElem::Type::IRI){
		std::string format_iri;
		int err = sparql::Utils::IRI2inner_format(triple_elem.value_,&format_iri);
		LOG("make typed value ss : %s",format_iri.c_str());
		if(err != 0){
			fprintf(stderr,"make_type_value error \n");
			exit(-1);				
		}
		if(core::IRIType::is_hashvalue(format_iri.c_str(),format_iri.size())){
			tag = SPARQLParserTypeTag::HV;	
			ret = query::HashValue(::strtol(format_iri.c_str(),nullptr,16));	
		}else{
			tag = SPARQLParserTypeTag::SS;
			ret = query::ShortString(format_iri.c_str());
		}	
	}else if(triple_elem.type_ == sparql::SPARQLParser::TriplePatternElem::Type::Literal){
		tag = SPARQLParserTypeTag::SS;
		ret = query::ShortString(triple_elem.value_.c_str());
	}
	return ret;
}


std::set<std::shared_ptr<TripleQuery>> make_triple_query_set(std::string sparql_query_str,std::shared_ptr<SharedQueryData> p_shared_data){
	sparql::SPARQLLexer lexer(sparql_query_str);
	sparql::SPARQLParser parser(lexer);
	try
	{
		parser.parse();
	}
	catch(sparql::SPARQLParser::ParserException& parse_excep)
	{
		LOG("parse failed.Info : %s",parse_excep.to_string().c_str());
		return {};
	}	
	std::set<std::shared_ptr<TripleQuery>> ret_query_set ;

	std::vector<std::string> variables = parser.project_variables();
	auto triple_patterns = parser.triple_patterns();
	//auto filters = parser.filters();
	
	SPARQLParserType_T triple[3];
	SPARQLParserTypeTag triple_tag[3];

	for(auto triple_it = triple_patterns.begin() ; triple_it != triple_patterns.end();\
			triple_it ++ ){
		triple[0] = make_typed_value(triple_it->subj_,triple_tag[0]);
		triple[1] = make_typed_value(triple_it->pred_,triple_tag[1]);
		triple[2] = make_typed_value(triple_it->obj_,triple_tag[2]);
		LOG("triple tag %d %d %d ", static_cast<int>(triple_tag[0]),static_cast<int>(triple_tag[1]),static_cast<int>(triple_tag[2]));


		if(triple_tag[0] == SPARQLParserTypeTag::HV){
			if(triple_tag[1] == SPARQLParserTypeTag::SS){
				if(triple_tag[2] == SPARQLParserTypeTag::VAR){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<HashValue>(triple[0]),
						boost::get<ShortString>(triple[1]),			
						boost::get<Variable>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);		
				}
			}else if(triple_tag[1] == SPARQLParserTypeTag::VAR){
				if(triple_tag[2] == SPARQLParserTypeTag::HV){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<HashValue>(triple[0]),
						boost::get<Variable>(triple[1]),
						boost::get<HashValue>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);		
				}else if(triple_tag[2] == SPARQLParserTypeTag::SS){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<HashValue>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<ShortString>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);		
				}else if(triple_tag[2] == SPARQLParserTypeTag::VAR){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<HashValue>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<Variable>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);		
				}
			}
		}else if (triple_tag[0] == SPARQLParserTypeTag::SS){
			if(triple_tag[1] == SPARQLParserTypeTag::SS){
				if(triple_tag[2] == SPARQLParserTypeTag::VAR){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<ShortString>(triple[0]),
						boost::get<ShortString>(triple[1]),			
						boost::get<Variable>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				}			
			}else if(triple_tag[1] == SPARQLParserTypeTag::VAR){
				if(triple_tag[2] == SPARQLParserTypeTag::HV){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<ShortString>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<HashValue>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
						
				}else if(triple_tag[2] == SPARQLParserTypeTag::SS){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<ShortString>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<ShortString>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				
				}else if(triple_tag[2] == SPARQLParserTypeTag::VAR){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<ShortString>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<Variable>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				}
			}
		}else if (triple_tag[0] == SPARQLParserTypeTag::VAR){
			if(triple_tag[1] == SPARQLParserTypeTag::SS){
				if(triple_tag[2] == SPARQLParserTypeTag::HV){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<Variable>(triple[0]),
						boost::get<ShortString>(triple[1]),			
						boost::get<HashValue>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				}else if(triple_tag[2] == SPARQLParserTypeTag::SS){
					LOG("should be here");
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<Variable>(triple[0]),
						boost::get<ShortString>(triple[1]),			
						boost::get<ShortString>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				}else if(triple_tag[2] == SPARQLParserTypeTag::VAR){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<Variable>(triple[0]),
						boost::get<ShortString>(triple[1]),			
						boost::get<Variable>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				}
			}else if(triple_tag[1] == SPARQLParserTypeTag::VAR){
				if(triple_tag[2] == SPARQLParserTypeTag::HV){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<Variable>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<HashValue>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				}else if(triple_tag[2] == SPARQLParserTypeTag::SS){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<Variable>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<ShortString>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				
				}else if(triple_tag[2] == SPARQLParserTypeTag::VAR){
					auto triple_query = std::make_shared<TripleQuery>(\
						boost::get<Variable>(triple[0]),
						boost::get<Variable>(triple[1]),			
						boost::get<Variable>(triple[2]),
						p_shared_data);
					ret_query_set.insert(triple_query);						
				}
			}
		}
	}	

	return ret_query_set ;
}


}//namespace query
