#include "sparqlplaner.h"

namespace sparql{

using ValueFilter = Index::Bigpita::ValueFilter;

//--------------------------------------------------------
std::unique_ptr<ValueFilter> SPARQLPlaner::make_filter(const SPARQLParser::Filter& filter)
{
	using FilterType = SPARQLParser::Filter::Type;
	using FilterArgsType = SPARQLParser::TriplePatternElem::LiteralType;	
	
	//过滤器怎么着也得存在一个参数。要么是函数名，要么是函数参数.
	if(filter.filter_args_.size() == 0)
	{
		return nullptr;
	}

	SPARQLParser::TriplePatternElem filter_arg_0 = filter.filter_args_[0];
	switch(filter.filter_type_)
	{
		// like =
		case FilterType::Equal:
			//like: ?year_old = 28
			if(filter_arg_0.literal_type_ == FilterArgsType::Double)
			{
				return sparql::make_unique<AmountFilter>(AmountFilter::Type::Equal,\
						atof(filter_arg_0.value_.c_str()));	
			}
			//like: ?comp_name = "IBM"
			else if(filter_arg_0.literal_type_ == FilterArgsType::String)
			{
				std::set<std::string> datum;
				datum.insert(filter_arg_0.value_);
				return sparql::make_unique<StringFilter>(datum);
			}
			break;
		// like != 
		// NOTE:当前仅支持数值型(double)
		case FilterType::NotEqual:
			if(filter_arg_0.literal_type_ == FilterArgsType::Double)
			{
				return sparql::make_unique<AmountFilter>(AmountFilter::Type::NotEqual,\
						atof(filter_arg_0.value_.c_str()));	
			}
			break;
		// like <
		// NOTE:当前仅支持数值型(double)
		case FilterType::Less:
			if(filter_arg_0.literal_type_ == FilterArgsType::Double)
			{
				return sparql::make_unique<AmountFilter>(AmountFilter::Type::Less,\
						atof(filter_arg_0.value_.c_str()));
			}
			break;
		// like <=
		// NOTE:当前仅支持数值型(double)
		case FilterType::LessOrEq:
			if(filter_arg_0.literal_type_ == FilterArgsType::Double)
			{
				return sparql::make_unique<AmountFilter>(AmountFilter::Type::LessOrEq,\
						atof(filter_arg_0.value_.c_str()));	
			}
			break;
		// like >
		// NOTE:当前仅支持数值型(double)
		case FilterType::Greater:
			if(filter_arg_0.literal_type_ == FilterArgsType::Double)
			{
				return sparql::make_unique<AmountFilter>(AmountFilter::Type::Greater,\
						atof(filter_arg_0.value_.c_str()));
			}
			break;
		// like >=
		// NOTE:当前仅支持数值型(double)
		case FilterType::GreaterOrEq:
			if(filter_arg_0.literal_type_ == FilterArgsType::Double)
			{
				return sparql::make_unique<AmountFilter>(AmountFilter::Type::GreaterOrEq,\
						atof(filter_arg_0.value_.c_str()));	
			}
			break;
		// like : get_range (比如有个叫get_range的函数)
		// NOTE:当前仅支持数值型(double)
		// 待实现
		case FilterType::Function:
			break;
	}
	return nullptr;
}


//--------------------------------------------------------
std::unique_ptr<ValueFilter> SPARQLPlaner::make_filter(const SPARQLParser::TriplePatternElem& triple_elem)
{
	using TripleElemType = SPARQLParser::TriplePatternElem::Type;
	using TripleElemLiteralType = SPARQLParser::TriplePatternElem::LiteralType;
	switch(triple_elem.type_)
	{
		//IRI 可能出现在sub or obj
		case TripleElemType::IRI:
			{
				std::string formated_str;
				int err = Utils::IRI2inner_format(triple_elem.value_,&formated_str);
				if(err != 0)
				{
					return nullptr;
				}
				std::set<std::string> datum;
				datum.insert(formated_str);
				return sparql::make_unique<StringFilter>(datum);
			}
		//Literal 只能出现在 obj	
		case TripleElemType::Literal:
			if(triple_elem.literal_type_ == TripleElemLiteralType::Double)
			{
				return sparql::make_unique<AmountFilter>(AmountFilter::Type::Equal,\
						atof(triple_elem.value_.c_str()));
			}
			else if(triple_elem.literal_type_ == TripleElemLiteralType::String)
			{
				std::set<std::string> datum;
				datum.insert(triple_elem.value_);
				return sparql::make_unique<StringFilter>(datum);
			}else
			{
				return nullptr;
			}
		defalut:
			return nullptr;
	}
	return nullptr;
}

//--------------------------------------------------------
query::TripleSetQuery SPARQLPlaner::handle(const std::string sparql_query)
{
	SPARQLLexer lexer(sparql_query);
	SPARQLParser parser(lexer);

	try
	{
		parser.parse();
	}
	catch(SPARQLParser::ParserException& parse_excep)
	{
		LOG("parse failed.Info : %s",parse_excep.to_string().c_str());
		return {};
	}
	
	query::TripleSetQuery triple_set;
	std::string template_var("var_");
	size_t      current_var_count = 0; 
	std::vector<std::string> variables = parser.project_variables();
	std::set<std::string>    current_variables(variables.begin(),variables.end());	

	auto triple_patterns = parser.triple_patterns();
	auto filters = parser.filters();

	std::cout << "=============filter=================" << std::endl;
	for(auto it = filters.begin(); it != filters.end(); it++)
	{
		std::cout << it->first <<"|" << SPARQLParser::to_string(it->second);
	}
	std::cout << std::endl;	
	std::cout << "=============filter=================" << std::endl;

	auto get_next_temporary_var_name = [&template_var,\
									    &current_var_count,\
										&current_variables]()\
		{
			while(true)
			{
				std::stringstream sstream;
				sstream << current_var_count ++;
				std::string new_var = template_var + sstream.str();
				if(current_variables.find(new_var) == current_variables.end())
				{
					current_variables.insert(new_var);
					return new_var;	
				}
			}
		};


	for(auto triple_it = triple_patterns.begin() ; triple_it != triple_patterns.end();\
			triple_it ++ )
	{
		switch(triple_it->subj_.type_)
		{
			//subj 为 IRI。需要转化成 blv.
			case SPARQLParser::TriplePatternElem::Type::IRI:
				{
					std::string format_iri;
					int err = Utils::IRI2inner_format(triple_it->subj_.value_,&format_iri);
					if(err != 0)
						return {};

					}
					//fuck here
					core::IRIType::is_hashvalue(format_iri.c_str(),format_iri.size());		
				}
				break;
			//subj 为 Variable.暂时忽略有针对主语的过滤器。此时主要考虑对obj的情况。
			case SPARQLParser::TriplePatternElem::Type::Variable:
				//如果obj为 Variable,需要看filters里面有没有对应的过滤器
				if(triple_it->obj_.type_ == SPARQLParser::TriplePatternElem::Type::Variable)
				{
					std::string obj_str = triple_it->obj_.value_;
					auto obj_filter = filters.find(obj_str);
											
					if(obj_filter != filters.end())
					{
						std::unique_ptr<ValueFilter> p_valuefilter = make_filter(obj_filter->second);
						if(p_valuefilter == nullptr)
							return {};
						if(triple_it->is_option_)
						{
							triple_set.add_opt_vlf(triple_it->subj_.value_,triple_it->pred_.value_,\
								triple_it->obj_.value_,p_valuefilter.release());
						}
						else
						{
							triple_set.add_vlf(triple_it->subj_.value_,triple_it->pred_.value_,\
								triple_it->obj_.value_,p_valuefilter.release());
						}
						continue;
					}
					else
					{
						if(triple_it->is_option_)
						{
							triple_set.add_opt_vlv(triple_it->subj_.value_,triple_it->pred_.value_,\
								triple_it->obj_.value_);
						}
						else
						{
							triple_set.add_vlv(triple_it->subj_.value_,triple_it->pred_.value_,\
								triple_it->obj_.value_);
						}
					}
				}
				//obj为literal 则根据其数值类型，建立对应过滤器(即等号过滤器).在山哥kg内部IRI与str是一样的.
				else if(triple_it->obj_.type_ == SPARQLParser::TriplePatternElem::Type::Literal)
				{
					std::unique_ptr<ValueFilter> p_valuefilter = make_filter(triple_it->obj_);
					if(p_valuefilter == nullptr)
						return {};
					std::string new_var = get_next_temporary_var_name();
					if(triple_it->is_option_)
					{
						triple_set.add_opt_vlf(triple_it->subj_.value_,triple_it->pred_.value_,\
								new_var,p_valuefilter.release());
					}		
					else
					{
						triple_set.add_vlf(triple_it->subj_.value_,triple_it->pred_.value_,\
								new_var,p_valuefilter.release());
					}

				}
				else
				{
					LOG("Un-support triple_obj_type : %s",triple_it->obj_.type_);
					return {};		
				}
				break;
			
			defalut:
				LOG("subject of triple pattern must be IRI or Variable,but now is : %s" ,\
						SPARQLParser::to_string(*triple_it).c_str());
				return {};
		}
	}
	return triple_set;
}




}//sparql

