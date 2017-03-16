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
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
}
	
TripleQuery::TripleQuery(HashValue sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}
	


TripleQuery::TripleQuery(HashValue sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}



TripleQuery::TripleQuery(HashValue sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;


}
	
TripleQuery::TripleQuery(ShortString sub, ShortString pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(pre.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
}


TripleQuery::TripleQuery(ShortString sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}




TripleQuery::TripleQuery(ShortString sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;


}
TripleQuery::TripleQuery(ShortString sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub.value());
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;

}
	
TripleQuery::TripleQuery(Variable sub, ShortString pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(pre.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::pre);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = HV;
	var_pos_[sub] = core::TripleElemPos::sub;
	p_shared_ = p_shared;
}


TripleQuery::TripleQuery(Variable sub, ShortString pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(pre.value());
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::pre);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = SS;
	var_pos_[sub] = core::TripleElemPos::sub;
	p_shared_ = p_shared;

}
TripleQuery::TripleQuery(Variable sub, ShortString pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(pre.value());
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
}
TripleQuery::TripleQuery(Variable sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}

TripleQuery::TripleQuery(Variable sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(obj.value());
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}

TripleQuery::TripleQuery(Variable sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared){
	spo_vec_iri_type_tag_[0] = UD;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
	var_pos_[sub] = core::TripleElemPos::sub;
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
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

}














}//namespace query
