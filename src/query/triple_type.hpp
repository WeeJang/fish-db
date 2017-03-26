//提供sparql-parser runtime 类型转换

#include "../sparql/sparqlparser.h"
#include "triple_query.h"

SPARQLParserType_T make_typed_value(
    sparql::SPARQLParser::TriplePatternElem& triple_elem,
    SPARQLParserTypeTag& tag) {
  SPARQLParserType_T ret;
  if (triple_elem.type_ ==
      sparql::SPARQLParser::TriplePatternElem::Type::Variable) {
    tag = SPARQLParserTypeTag::VAR;
    ret = query::Variable(triple_elem.value_);
  } else if (triple_elem.type_ ==
             sparql::SPARQLParser::TriplePatternElem::Type::IRI) {
    std::string format_iri;
    int err = Utils::IRI2inner_format(triple_it->subj_.value_, &format_iri);
    if (err != 0) {
      fprintf(stderr, "make_type_value error \n");
      exit(-1);
    }
    if (core::IRIType::is_hashvalue(format_iri.c_str(), format_iri.size())) {
      tag = SPARQLParserTypeTag::HV;
      ret = query::HashValue(::strtol(triple_elem.value_.c_str(), nullptr, 16));
    } else {
      tag = SPARQLParserTypeTag::SS;
      ret = query::ShortString(triple_elem.value_.c_str());
    }
  } else if (triple_elem.type_ ==
             sparql::SPARQLParser::TriplePatternElem::Type::Literal) {
    tag = SPARQLParserTypeTag::SS;
    ret = query::ShortString(triple_elem.value_.c_str());
  }
  return ret;
}
