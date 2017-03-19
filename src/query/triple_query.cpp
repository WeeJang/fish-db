#include "triple_query.h"

namespace query{

TripleQuery::TripleQuery(HashValue sub, ShortString pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(pre.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = UD;
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
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
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
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_vec_.push_back(pre);
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}



TripleQuery::TripleQuery(HashValue sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = UD;
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
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = UD;
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
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
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
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_vec_.push_back(pre);
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}


TripleQuery::TripleQuery(ShortString sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = UD;
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
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = HV;
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
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = SS;
	var_vec_.push_back(sub);
	var_pos_[sub] = core::TripleElemPos::sub;
	p_shared_ = p_shared;
	init();
}

TripleQuery::TripleQuery(Variable sub, ShortString pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(pre.value());
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = UD;
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
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
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
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_vec_.push_back(sub);
	var_vec_.push_back(pre);
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
	init();
}

TripleQuery::TripleQuery(Variable sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
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
		auto iri_value = std::get<HV_T>(iri_variant);	
		auto p_iri_index = p_shared_->p_fish_db_->hv_meta_iri_index_.get_IRI_index(iri_value);
		if(!p_iri_index){
			return -1;
		}else{
			ret_bitmap = p_iri_index->at(pos);
			return 0;	
		}
	}else if(IRITypeUnionTag::SS == typetag){
		auto iri_value = std::get<SS_T>(iri_variant);	
		auto p_iri_index = p_shared_->p_fish_db_->hv_meta_iri_index_.get_IRI_index(iri_value);
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
		return;
	}
	BitMap_T bit_map;	
	for(size_t i = 0 ; i < iri_vec_.size() ; i ++){
		auto iri_variant = iri_vec_[i];
		auto iri_elem_pos = iri_pos_[i];
		auto iri_type_tag = spo_vec_iri_type_tag_[iri_elem_pos];
		if(-1 == get_iri_index(iri_variant,iri_type_tag,iri_elem_pos,bit_map)){
			is_valid_ = false;
			return;
		}
		if(0 == i){
			cur_valid_row_bm_index_ = bit_map;
		}else{
			cur_valid_row_bm_index_ &= bit_map;
		}
	}
	cur_valid_row_bm_index_ = cur_valid_row_bm_index_.cardinality();
	is_valid_ = true;
}


void TripleQuery::improve_spo_vec_iri_type_tag(std::shared_ptr<core::TripleSpec> p_triple){
	for(size_t i = 0 ; i < spo_vec_iri_type_tag_.size() ; i ++){
		if(IRITypeUnionTag::UD == spo_vec_iri_type_tag_[i]){
			if(IRIType::is_hashvalue(p_triple->at(i).c_str(),p_triple->at(i).size())){
				spo_vec_iri_type_tag_ = IRITypeUnionTag::HV;	
			}else{
				spo_vec_iri_type_tag_ = IRITypeUnionTag::SS;
			}
		}
	}
}

void select_new_triple(std::shared_ptr<core::TripleSpec> p_triple_spec){
	for(auto iter = var_pos_.begin(); iter != var_pos_.end() ; iter++){
		auto var_name = iter->first;
		auto var_pos  = iter->second;
		auto var_tag  = spo_vec_iri_type_tag_[var_pos];
		if(IRITypeUnionTag::HV == var_tag){
			HV_T hv_value = ::strtol(p_triple_spec->at(var_pos).c_str(),nullptr,16);
			select_spo_vec_[var_pos].push_back(hv_value);
		}else if(IRITypeUnionTag::SS == var_tag){
			select_pos_vec_[var_pos].push_back(p_triple_spec->at(var_pos));
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
	auto var_tag = spo_vec_iri_type_tag_[var_pos];
	auto another_tag = spo_vec_iri_type_tag_[another_pos];

	p_shared_->intermediate_result_col_name_[another_var_name].push_back(var_name);
	
	std::unordered_map<std::string,std::vector<std::string>> another2var_map;
	//check if another_var_name in shared_data.intermidiate_result	
	auto got_another_var_intermediate_result = p_shared_->intermediate_result_.find(another_var_name);
	if(got_another_var_intermediate_result == p_shared_->intermediate_result_.end()){
		if(another_tag == IRITypeUnionTag::HV && var_tag == IRITypeUnionTag::HV){
			for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
				auto another_val = std::get<HV_T>(select_spo_vec_[another_pos][i]);
				auto var_val     = std::get<HV_T>(select_spo_vec_[var_pos][i]);
				if(p_shared_->hv_bound_vals[another_var_name].find(another_val) \
					== p_shared_->hv_bound_vals[another_var_name].end()){ continue;
				}
				if(p_shared_->hv_bound_vals[var_name].find(var_val) \
					== p_shared_->hv_bound_vals[var_name].end()){
					continue;
				}
				std::string another_str = std::to_string(another_val);
				std::string var_str     = std::to_string(var_val);
				another2var_map[another_str].push_back(var_str);
			}	
		}else if(another_tag == IRTTypeUnionTag::HV && var_tag == IRITypeUnionTag::SS){
			for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
				auto another_val = std::get<HV_T>(select_spo_vec_[another_pos][i]);
				auto var_val     = std::get<SS_T>(select_spo_vec_[var_pos][i]);
				if(p_shared_->hv_bound_vals[another_var_name].find(another_val) \
					== p_shared_->hv_bound_vals[another_var_name].end()){
					continue;
				}
				if(p_shared_->ss_bound_vals[var_name].find(var_val) \
					== p_shared_->ss_bound_vals[var_name].end()){
					continue;
				}
				std::string another_str = std::to_string(another_val);
				another2var_map[another_str].push_back(var_val);
			}	
		}
		}else if(another_tag == IRTTypeUnionTag::SS && var_tag == IRITypeUnionTag::SS){
			for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
				auto another_val = std::get<SS_T>(select_spo_vec_[another_pos][i]);
				auto var_val     = std::get<SS_T>(select_spo_vec_[var_pos][i]);
				if(p_shared_->ss_bound_vals[another_var_name].find(another_val) \
					== p_shared_->ss_bound_vals[another_var_name].end()){
					continue;
				}
				if(p_shared_->ss_bound_vals[var_name].find(var_val) \
					== p_shared_->ss_bound_vals[var_name].end()){
					continue;
				}
				another2var_map[another_val].push_back(var_val);

			}	
		}
		}else if(another_tag == IRTTypeUnionTag::SS && var_tag == IRITypeUnionTag::HV){
			for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
				auto another_val = std::get<SS_T>(select_spo_vec_[another_pos][i]);
				auto var_val     = std::get<HV_T>(select_spo_vec_[var_pos][i]);
				if(p_shared_->ss_bound_vals[another_var_name].find(another_val) \
					== p_shared_->ss_bound_vals[another_var_name].end()){
					continue;
				}
				if(p_shared_->hv_bound_vals[var_name].find(var_val) \
					== p_shared_->hv_bound_vals[var_name].end()){
					continue;
				}
				std::string var_str     = std::to_string(var_val);
				another2var_map[another_val].push_back(var_str);
			}	
		}
		}else{
			fprintf(stderr,"update cartesian product error\n");
			error(-1);
		}
}else{
	auto interm_result = got_another_var_intermediate_result->second;
	p_shared_->intermediate_result_col_name_[another_var_name] = var_name;
	if(another_tag == IRITypeUnionTag::HV && var_tag == IRITypeUnionTag::HV){
		for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
			auto another_val = std::get<HV_T>(select_spo_vec_[another_pos][i]);
			auto var_val     = std::get<HV_T>(select_spo_vec_[var_pos][i]);
			std::string another_str = std::to_string(another_val);
			std::string var_str     = std::to_string(var_val);
			auto got_another_var_vec = interm_result.find(another_str)
			if(got_another_var_vec == interm_result.end()){
				continue;	
			}	
			if(p_shared_->ss_bound_vals[var_name].find(var_val) \
				== p_shared_->ss_bound_vals[var_name].end()){
				continue;
			}

			std::vector<std::string> another_var_vec = *got_another_var_vec;
			for(auto str : another_var_vec){
				str.append("\n").append(var_str);
				another2var_map[another_str].push_back(str);
			}	
		}	
	}else if(another_tag == IRTTypeUnionTag::HV && var_tag == IRITypeUnionTag::SS){
		for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
			auto another_val = std::get<HV_T>(select_spo_vec_[another_pos][i]);
			auto var_val     = std::get<SS_T>(select_spo_vec_[var_pos][i]);
			std::string another_str = std::to_string(another_val);
			auto got_another_var_vec = interm_result.find(another_val)
			if(got_another_var_vec == interm_result.end()){
				continue;	
			}	
			if(p_shared_->ss_bound_vals[var_name].find(var_val) \
				== p_shared_->ss_bound_vals[var_name].end()){
				continue;
			}

			std::vector<std::string> another_var_vec = *got_another_var_vec;
			for(auto str : another_var_vec){
				str.append("\n").append(var_str);
				another2var_map[another_val].push_back(str);
			}	
		}	
	}
	}else if(another_tag == IRTTypeUnionTag::SS && var_tag == IRITypeUnionTag::SS){
		for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
			auto another_val = std::get<SS_T>(select_spo_vec_[another_pos][i]);
			auto var_val     = std::get<SS_T>(select_spo_vec_[var_pos][i]);
			auto got_another_var_vec = interm_result.find(another_val)
			if(got_another_var_vec == interm_result.end()){
				continue;	
			}	
			
			if(p_shared_->ss_bound_vals[var_name].find(var_val) \
				== p_shared_->ss_bound_vals[var_name].end()){
				continue;
			}
			std::vector<std::string> another_var_vec = *got_another_var_vec;
			for(auto str : another_var_vec){
				str.append("\n").append(var_val);
				another2var_map[another_val].push_back(str);
			}	
		}	
	}
	}else if(another_tag == IRTTypeUnionTag::SS && var_tag == IRITypeUnionTag::HV){
		for(size_t i = 0 ; i < select_spo_vec_[var_pos].size() ; i++){
			auto another_val = std::get<SS_T>(select_spo_vec_[another_pos][i]);
			auto var_val     = std::get<HV_T>(select_spo_vec_[var_pos][i]);
			std::string var_str     = std::to_string(var_val);
			
			auto got_another_var_vec = interm_result.find(another_val)
			if(got_another_var_vec == interm_result.end()){
				continue;	
			}	
			
			if(p_shared_->hv_bound_vals[var_name].find(var_val) \
				== p_shared_->hv_bound_vals[var_name].end()){
				continue;
			}
			std::vector<std::string> another_var_vec = *got_another_var_vec;
			for(auto str : another_var_vec){
				str.append("\n").append(var_str);
				another2var_map[another_val].push_back(str);
			}	
		}	
	}
	}else{
		fprintf(stderr,"update cartesian product error\n");
		error(-1);
	}			
}//else
		p_shared_->intermediate_result_[another_var_name] = another2var_map;
			


}




void TripleQuery::union_update_shared_data(std::string var_name){
	auto var_pos = var_pos_[var_name];
	auto var_tag = spo_vec_iri_type_tag_[var_pos];
	auto val_vec = select_spo_vec_[var_pos];
	
	p_shared_->var_val_type_[var_name] = var_tag;	
	if(IRITypeUnionTag::HV == var_tag){
		for(auto& elem : val_vec){
			p_shared_->hv_bound_vals_[var_name].insert(std::get<HV_T>(elem));
		}
	}else if(IRITypeUnionTag::SS == var_tag){
		for(auto& elem : val_vec){
			p_shared_->ss_bound_vals_[var_name].insert(std::get<SS_T>(elem));
		}
	}else{
		fprintf(stderr,"union update shared data error!\n");
		exit(-1);
	}
}

void TripleQuery::join_update_shared_data(std::string var_name){
	auto var_pos = var_pos_[var_name];
	auto var_tag = spo_vec_iri_type_tag_[var_pos];
	auto val_vec = select_spo_vec_[var_pos];
	
	if(IRITypeUnionTag::HV == var_tag){
		auto hv_var_vals_set = p_shared_->hv_bound_vals_[var_name];
		p_shared_->hv_bound_vals_[var_name].clear();
		for(auto& elem : val_vec){
			HV_T hv_elem = std::get<HV_T>(elem);
			if(hv_var_vals_set.find(hv_elem) != hv_var_vals_set.end()){
				p_shared_->hv_bound_vals_[var_name].insert(hv_elem);
			}
		}
	}else if(IRITypeUnionTag::SS == var_tag){
		auto ss_var_vals_set = p_shared_->ss_bound_vals_[var_name];
		p_shared_->ss_bound_vals_[var_name].clear();
		for(auto& elem : val_vec){
			SS_T ss_elem = std::get<SS_T>(elem);
			if(ss_var_vals_set.find(hv_elem) != ss_var_vals_set.end()){
				p_shared_->ss_bound_vals_[var_name].insert(ss_elem);
			}
		}
	}else{
		fprintf(stderr,"join update shared data error!\n");
		exit(-1);
	}

}


//根据现有信息，取数据
void TripleQuery::select(){
	shrink_cur_valid_row_bm();
	
	auto p_db = p_shared_->p_fish_db_;
	auto p_triple_spec = std::make_shared<core::TripleSpec>(); 
	auto rowindex_bm = new uint64_t[cur_valid_row_bm_index_cardinality];
	cur_valiad_row_bm_index_.toUnit64Array(rowindex_bm);	
	for(size_t i = 0 ; i < cur_valid_row_bm_index_cardinality_ ; i++){
		p_fish_db_->get_triple_by_row_index(rowindex_bm[i],p_triple_spec);
		if(0 == i){
			improve_spo_vec_iri_type_tag(p_triple_spec);
		}
		select_new_triple(p_triple_spec);				
	}
	delete [] bitmap_array;

	for(auto var_name : var_vec_){
		if(p_shared_->var_val_type_.find(var_name) == p_shared_->var_val_type_.end()){
			union_update_shared_data(var_name);
		}else{
			join_update_shared_data(var_name); 
		}	
	}
}

void TripleQuery::shrink_cur_valid_row_bm(){
	auto p_fish_db = p_shared_->p_fish_db_;
	for(auto var_name : var_vec_){
		if(p_shared_->var_val_type_.find(var_name) == p_shared_->var_val_type_.end()){
			continue;
		}
		auto var_pos = var_pos_[var_name];
		BitMap_T union_bm;
		auto var_tag = spo_vec_iri_type_tag_[var_pos];
		if(var_tag == IRITypeUnionTag::HV){
			auto var_set = p_shared_->hv_bound_vals_[var_name];
			for(auto var_set_elem : var_set){
				union_bm |= p_fish_db->hv_meta_iri_index_.get_IRI_index(var_set_lem).at(var_pos);
			}
		}else if(var_tag == IRITypeUnionTag::SS){
			auto var_set = p_shared_->ss_bound_vals_[var_name];
			for(auto var_set_elem : var_set){
				union_bm |= p_fish_db->ss_meta_iri_index_.get_IRI_index(var_set_lem).at(var_pos);
			}
		}else{
			fprintf(stderr,"shink_cur_valid_row_bm error!\n");
			exit(-1);
		}
		cur_valid_row_bm_index_ &= union_bm;
	}			
}

}//namespace query
