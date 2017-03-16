#ifndef QUERY_TRIPLE_QUERY_H_
#define QUERY_TRIPLE_QUERY_H_

#include "../core/iri_index.hpp"
#include "../core/triple.h"

#include <set>
#include <variant>

namespace query{

using BitMap_T =  IRIIndex<IRIType::Value>::BitMap_T;
using HV_T = core::IRITypeTrait<core::IRIType::HashValue>::value_type;
using SS_T = core::IRITypeTrait<core::IRIType::ShortString>::value_type;
using VAR_T = std::string;
using IRITypeUnion_T = std::variant<HV_T,SS_T>;

enum class IRITypeUnionTag{
	UD, //un-decide
	HV, //HashValue
	SS  //shortstring
}; 

class SharedQueryData{
		
	std::unordered_map<std::string,IRITypeUnionTag>& var_val_type_;
	std::unordered_map<std::string,std::set<HV_T>>&  hv_bound_vals_; //var_1 -> set(val_1,val_2,...)
	std::unordered_map<std::string,std::set<SS_T>>&  ss_bound_vals_; //same 

};


class TripleQuery{
	
public:
	TripleQuery(HV_T sub, SS_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(HV_T sub, VAR_T pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(HV_T sub, VAR_T pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(HV_T sub, VAR_T pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);
	
	TripleQuery(SS_T sub, SS_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(SS_T sub, VAR_T pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(SS_T sub, VAR_T pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(SS_T sub, VAR_T pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);
	
	TripleQuery(VAR_T sub, SS_T  pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, SS_T  pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, SS_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, VAR_T  pre,HV_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, VAR_T  pre,SS_T obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(VAR_T sub, VAR_T  pre,VAR_T obj,std::shared_ptr<SharedQueryData> p_shared);

	
	void set_shared_query_data(std::shared_ptr<SharedQueryData> p_shared_data);
	
	update(std::string var_name)

private:
	std::vector<IRITypeUnion_T>     iri_vec_; //non-var: HV/SS
	std::vector<core::TripleElemPos> iri_pos_; //non-var-pos [0,2]
	std::unordered_map<std::string,core::TripleElemPos> var_pos_; //var-pos ( var_name-> pos )

	IRITypeUnionTag    spo_vec_iri_type_tag_[3];   // s-p-o iri_type [ note var's val has type ! ]
	std::vector<IRITypeUnion_T> selet_spo_vec_[3]; // s-p-o selected

	BitMap_T cur_valid_row_bm_index_;      // current valid row bitmap index;
	
	std::shared_ptr<SharedQueryData> p_shared_;
	

};//class TripleQuery


}//namespace query



#endif //QUERY_TRIPLE_QUERY_H_
