#ifndef QUERY_TRIPLE_QUERY_H_
#define QUERY_TRIPLE_QUERY_H_ 
#include "../core/iri_index.hpp"
#include "../core/iri_type.h"
#include "../core/triple.h"
#include "../sparql/sparqlparser.h"
#include "../core/fish_db_impl.h"

#include <set>
#include <boost/variant.hpp>

namespace query{
using BitMap_T    = core::IRIIndex<core::IRIType::Value>::BitMap_T;
using HashValue   = core::IRIType::HashValue;
using ShortString = core::IRIType::ShortString;
using Variable	  = std::string;
using SPARQLParserType_T   = boost::variant<HashValue,ShortString,Variable>;
enum class SPARQLParserTypeTag{
	HV = 0,
	SS,
	VAR
}; 

using HV_T        = core::IRITypeTrait<HashValue>::value_type;
using SS_T        = core::IRITypeTrait<ShortString>::value_type;
using IRITypeUnion_T = boost::variant<HV_T,SS_T>;

enum class IRITypeUnionTag{
	UD = 0, //un-decide
	HV, //HashValue
	SS  //shortstring
}; 

class SharedQueryData{
	friend class TripleQuery;
	friend class QueryExecutor;
public:
	SharedQueryData(std::shared_ptr<fishdb::FishDBImpl> p_fish_db):p_fish_db_(p_fish_db) {}
	int make_cartesian_product_by_filter_vector_linked(std::string another_var_name,std::string var_name,std::vector<std::string> (&filter_var_val_vector)[2]);
	//debug
	void printf_var_val_type();
	void printf_hv_bound_vals();
	void printf_ss_bound_vals();
	void printf_intermediate_result();
	void printf_intermediate_result_col_name();
private:
	std::shared_ptr<fishdb::FishDBImpl> p_fish_db_;	
	std::unordered_map<std::string,IRITypeUnionTag> var_val_type_;
	std::unordered_map<std::string,std::set<HV_T>>  hv_bound_vals_; //var_1 -> set(val_1,val_2,...)
	std::unordered_map<std::string,std::set<SS_T>>  ss_bound_vals_; //same 
	//var->val1->val2*val3
	std::unordered_map<std::string,std::unordered_map<std::string,std::vector<std::string>>> intermediate_result_;
	std::unordered_map<std::string,std::vector<std::string>> intermediate_result_col_name_;
	//std::vector<std::string> intermediate_result_col_name_;	
};

class TripleQuery{
	friend class QueryExecutor;
public:
	TripleQuery(HashValue sub, ShortString  pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(HashValue sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(HashValue sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(HashValue sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared);
	
	TripleQuery(ShortString sub, ShortString  pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(ShortString sub, Variable pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(ShortString sub, Variable pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(ShortString sub, Variable pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared);
	
	TripleQuery(Variable sub, ShortString  pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(Variable sub, ShortString  pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(Variable sub, ShortString  pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(Variable sub, Variable  pre,HashValue obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(Variable sub, Variable  pre,ShortString obj,std::shared_ptr<SharedQueryData> p_shared);
	TripleQuery(Variable sub, Variable  pre,Variable obj,std::shared_ptr<SharedQueryData> p_shared);

public:
	const bool is_valid() const { return is_valid_; } 
	const std::vector<std::string> var_vec() const { return var_vec_; }
	bool contain_vars(std::string var_name){
		return var_pos_.find(var_name) != var_pos_.end();
	}

	void select();
	void union_update_shared_data(std::string var_name);
	void join_update_shared_data(std::string var_name);
	std::string update_cartesian_product_in_shared_data(std::string var_name);
	
	void shrink_cur_valid_row_bm();
private:
	void init();

	int get_iri_index(IRITypeUnion_T iri_variant,IRITypeUnionTag typetag,core::TripleElemPos pos,BitMap_T& ret_bitmap);

	void improve_spo_vec_iri_type_tag(std::shared_ptr<core::TripleSpec> p_triple);		
	void select_new_triple(std::shared_ptr<core::TripleSpec> p_triple);

	//for debug
	void printf_select_spo_vec();

private:
	std::vector<IRITypeUnion_T>     iri_vec_; //non-var: HV/SS
	std::vector<core::TripleElemPos> iri_pos_; //non-var-pos [0,2]
	std::vector<std::string>         var_vec_;
	std::unordered_map<std::string,core::TripleElemPos> var_pos_; //var-pos ( var_name-> pos )
	IRITypeUnionTag    spo_vec_iri_type_tag_[3];   // s-p-o iri_type [ note var's val has type ! ] //其实这里与type-system是冲突的(当pre不同时）
	std::vector<IRITypeUnion_T> select_spo_vec_[3]; // s-p-o selected
	BitMap_T cur_valid_row_bm_index_;    // current valid row bitmap index;
	uint64_t cur_valid_row_bm_index_cardinality_;
	std::shared_ptr<SharedQueryData> p_shared_;
	bool is_valid_; //no elem 
};//class TripleQuery


//helper function
SPARQLParserType_T make_typed_value(sparql::SPARQLParser::TriplePatternElem& triple_elem,SPARQLParserTypeTag& tag);
std::set<std::shared_ptr<TripleQuery>> make_triple_query_set(std::string sparql_query_str,std::shared_ptr<SharedQueryData> p_shared_data);

}//namespace query



#endif //QUERY_TRIPLE_QUERY_H_
