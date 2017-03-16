#include "triple_query.h"


namespace query{

TripleQuery::TripleQuery(HV_T sub, SS_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_vec_.push_back(pre);
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
}
	
TripleQuery::TripleQuery(HV_T sub, VAR_T pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_vec_.push_back(obj);
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}
	


TripleQuery::TripleQuery(HV_T sub, VAR_T pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_vec_.push_back(obj);
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}



TripleQuery::TripleQuery(HV_T sub, VAR_T pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = HV;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;


}
	
TripleQuery::TripleQuery(SS_T sub, SS_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_vec_.push_back(pre);
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::pre);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = SS;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;
}


TripleQuery::TripleQuery(SS_T sub, VAR_T pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_vec_.push_back(obj);
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = HV;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;
}




TripleQuery::TripleQuery(SS_T sub, VAR_T pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_vec_.push_back(obj);
	iri_pos_.push_back(core::TripleElemPos::sub);
	iri_pos_.push_back(core::TripleElemPos::obj);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = SS;
	var_pos_[pre] = core::TripleElemPos::pre;
	p_shared_ = p_shared;


}
TripleQuery::TripleQuery(SS_T sub, VAR_T pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared){
	iri_vec_.push_back(sub);
	iri_pos_.push_back(core::TripleElemPos::sub);
	spo_vec_iri_type_tag_[0] = SS;
	spo_vec_iri_type_tag_[1] = UD;
	spo_vec_iri_type_tag_[2] = UD;
	var_pos_[pre] = core::TripleElemPos::pre;
	var_pos_[obj] = core::TripleElemPos::obj;
	p_shared_ = p_shared;

}
	
	TripleQuery(VAR_T sub, SS_T  pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, SS_T  pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, SS_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, VAR_T  pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, VAR_T  pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, VAR_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);














}//namespace query
