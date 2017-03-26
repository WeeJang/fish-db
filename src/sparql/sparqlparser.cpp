#include "sparqlparser.h"

namespace sparql {

//---------------------------------
SPARQLParser::TPElem SPARQLParser::make_variable(std::string variable) {
  return TriplePatternElem(TPElem::Type::Variable, TPElem::LiteralType::None,
                           variable);
}

SPARQLParser::TPElem SPARQLParser::make_IRI(std::string IRI) {
  return TriplePatternElem(TPElem::Type::IRI, TPElem::LiteralType::None, IRI);
}

SPARQLParser::TPElem SPARQLParser::make_notype_literal(std::string literal) {
  return TriplePatternElem(TPElem::Type::Literal, TPElem::LiteralType::None,
                           literal);
}

SPARQLParser::TPElem SPARQLParser::make_integer_literal(std::string literal) {
  return TriplePatternElem(TPElem::Type::Literal, TPElem::LiteralType::Integer,
                           literal);
}

SPARQLParser::TPElem SPARQLParser::make_double_literal(std::string literal) {
  return TriplePatternElem(TPElem::Type::Literal, TPElem::LiteralType::Double,
                           literal);
}

SPARQLParser::TPElem SPARQLParser::make_string_literal(std::string literal) {
  // strip
  size_t literal_len = literal.size();
  std::string cleaned_literal("");
  if (literal_len > 2) {
    cleaned_literal = literal.substr(1, literal_len - 2);
  }
  return TriplePatternElem(TPElem::Type::Literal, TPElem::LiteralType::String,
                           cleaned_literal);
}

SPARQLParser::TPElem SPARQLParser::make_function_literal(std::string literal) {
  return TriplePatternElem(TPElem::Type::Literal, TPElem::LiteralType::Function,
                           literal);
}

//---------------------------------

std::string SPARQLParser::to_string(
    const SPARQLParser::TPElem& triple_pattern_elem) {
  std::stringstream sstream;
  switch (triple_pattern_elem.type_) {
    case SPARQLParser::TPElem::Type::Variable:
      sstream << "Variable";
      break;
    case SPARQLParser::TPElem::Type::IRI:
      sstream << "IRI";
      break;
    case SPARQLParser::TPElem::Type::Literal:
      sstream << "Literal";
      break;
    case SPARQLParser::TPElem::Type::Blank:
      sstream << "Blank";
      break;
    default:
      sstream << "Error";
      break;
  }
  sstream << ":" << triple_pattern_elem.value_;
  return sstream.str();
}

std::string SPARQLParser::to_string(
    const SPARQLParser::TriplePattern& triple_pattern) {
  std::stringstream sstream;
  if (triple_pattern.is_option_) sstream << "option";
  sstream << "(" << SPARQLParser::to_string(triple_pattern.subj_) << ","
          << SPARQLParser::to_string(triple_pattern.pred_) << ","
          << SPARQLParser::to_string(triple_pattern.obj_) << ")";
  return sstream.str();
}

std::string SPARQLParser::to_string(const SPARQLParser::Filter& filter) {
  std::stringstream sstream;
  sstream << "filter";
  sstream << "(" << filter.variable_str_ << " " << filter.filter_type_;
  for (auto it = filter.filter_args_.begin(); it != filter.filter_args_.end();
       it++) {
    // sstream << " " << *it ;
    sstream << " " << SPARQLParser::to_string(*it);
  }
  sstream << ")";
  return sstream.str();
}

std::string SPARQLParser::to_string() {
  std::stringstream sstream;
  sstream << "===============================" << std::endl;
  sstream << "project_variables:[";
  for (auto it = project_variables_.begin(); it != project_variables_.end();
       it++) {
    sstream << *it << ",";
  }
  sstream << "]" << std::endl;

  sstream << "triples:" << std::endl;
  for (auto it = triple_patterns_.begin(); it != triple_patterns_.end(); it++) {
    sstream << "\t" << SPARQLParser::to_string(*it) << std::endl;
    ;
  }

  sstream << "filters:" << std::endl;
  for (auto it = filters_.begin(); it != filters_.end(); it++) {
    sstream << "\t" << SPARQLParser::to_string(it->second) << std::endl;
  }
  sstream << "===============================" << std::endl;
  return sstream.str();
}

//-----------------------------------
void SPARQLParser::parseProjection() {
  auto token_t = sparql_lexer_.get_next_token_type();

  if (token_t != SPARQLLexer::Token_T::Identifier ||
      !sparql_lexer_.is_keywords(std::string("select").c_str())) {
    throw ParserException("Except : 'select'");
  }

  project_variables_.clear();
  do {
    token_t = sparql_lexer_.get_next_token_type();
    if (token_t != SPARQLLexer::Token_T::Variable) {
      sparql_lexer_.unget(token_t);
      break;
    }
    project_variables_.push_back(sparql_lexer_.get_current_token_value());
  } while (true);

  if (project_variables_.size() == 0)
    throw ParserException("projection variables is empty");
}

void SPARQLParser::parseWhere() {
  if (sparql_lexer_.get_next_token_type() != SPARQLLexer::Token_T::Identifier ||
      !sparql_lexer_.is_keywords(std::string("where").c_str())) {
    throw ParserException("Except : 'where'");
  }
}

void SPARQLParser::parseFilter() {
  LOG("parse filter");

  if (sparql_lexer_.get_next_token_type() != SPARQLLexer::Token_T::LRound) {
    throw ParserException("Expect : (");
  }

  if (sparql_lexer_.get_next_token_type() != SPARQLLexer::Token_T::Variable) {
    throw ParserException("Expect: Variable");
  }
  std::string variable = sparql_lexer_.get_current_token_value();
  Filter filter;
  filter.variable_str_ = variable;
  auto token = sparql_lexer_.get_next_token_type();
  std::cout << "filter : " << token << std::endl;
  std::cout << "filter str : " << sparql_lexer_.get_current_token_value()
            << std::endl;
  switch (token) {
    case SPARQLLexer::Token_T::Equal:
      filter.filter_type_ = Filter::Type::Equal;
      break;
    case SPARQLLexer::Token_T::NotEqual:
      filter.filter_type_ = Filter::Type::NotEqual;
      break;
    case SPARQLLexer::Token_T::Less:
      filter.filter_type_ = Filter::Type::Less;
      break;
    case SPARQLLexer::Token_T::LessOrEq:
      filter.filter_type_ = Filter::Type::LessOrEq;
      break;
    case SPARQLLexer::Token_T::Greater:
      filter.filter_type_ = Filter::Type::Greater;
      break;
    case SPARQLLexer::Token_T::GreaterOrEq:
      filter.filter_type_ = Filter::Type::GreaterOrEq;
      break;
    default:
      filter.filter_type_ = Filter::Type::Function;
      std::string function_name = sparql_lexer_.get_current_token_value();
      filter.filter_args_.push_back(make_function_literal(function_name));
      break;
  }

  bool is_finish = false;
  while (!is_finish) {
    auto token = sparql_lexer_.get_next_token_type();
    switch (token) {
      case SPARQLLexer::Token_T::RRound:
        is_finish = true;
        break;
      case SPARQLLexer::Token_T::Eof:
        throw ParserException("Unexpect EOF in parse 'filter(...'");
      case SPARQLLexer::Token_T::String: {
        std::string value = sparql_lexer_.get_current_token_value();
        auto string_elem = make_string_literal(value);
        filter.filter_args_.push_back(string_elem);
      } break;
      case SPARQLLexer::Token_T::Number: {
        std::string value = sparql_lexer_.get_current_token_value();
        auto double_elem = make_double_literal(value);
        filter.filter_args_.push_back(double_elem);
      } break;
      default:
        throw ParserException(std::string("Parse filter failed when meet :") +
                              sparql_lexer_.get_current_token_value());
    }
  }
  filters_[filter.variable_str_] = filter;
}

void SPARQLParser::parseBasicGraphPattern(bool is_option) {
  TriplePatternElem pattern_elem[3];
  for (int i = 0; i < 3; i++) {
    auto token_t = sparql_lexer_.get_next_token_type();
    std::string token_value = sparql_lexer_.get_current_token_value();
    LOG("token_value : %s", token_value.c_str());
    TriplePatternElem elem;
    switch (token_t) {
      case SPARQLLexer::Token_T::Variable:
        elem = make_variable(token_value);
        break;
      case SPARQLLexer::Token_T::IRI:
        elem = make_IRI(token_value);
        break;
      case SPARQLLexer::Token_T::String:
        elem = make_string_literal(token_value);
        break;
      case SPARQLLexer::Token_T::Number:
        elem = make_double_literal(token_value);
        break;
      case SPARQLLexer::Token_T::Identifier:
        elem = make_notype_literal(token_value);
        break;
      // case SPARQLLexer::Token_T::Blank:
      //	break;
      default:
        throw ParserException("Except varialbe or IRI or Literal or Blank");
    }
    pattern_elem[i] = elem;
  }
  // maybe '.' exist
  auto token_t = sparql_lexer_.get_next_token_type();
  if (token_t != SPARQLLexer::Token_T::Dot) {
    sparql_lexer_.unget(token_t);
  }
  TriplePattern pattern(pattern_elem[0], pattern_elem[1], pattern_elem[2],
                        is_option);
  triple_patterns_.push_back(pattern);
}

// Group Graph Pattern:
//  such like '',
void SPARQLParser::parseGroupGraphPattern() {
  if (sparql_lexer_.get_next_token_type() != SPARQLLexer::Token_T::LCurly) {
    throw ParserException("Except : '{' ");
  }

  while (true) {
    auto token_t = sparql_lexer_.get_next_token_type();
    if (token_t == SPARQLLexer::Token_T::LCurly) {
      sparql_lexer_.unget(token_t);
      parseGroupGraphPattern();
    } else if (token_t == SPARQLLexer::Token_T::Identifier &&
               sparql_lexer_.is_keywords(std::string("optional").c_str())) {
      if (sparql_lexer_.get_next_token_type() != SPARQLLexer::Token_T::LCurly)
        throw ParserException("Except : '{' ");
      parseBasicGraphPattern(true);
      if (sparql_lexer_.get_next_token_type() != SPARQLLexer::Token_T::RCurly)
        throw ParserException("Except : '}' ");
    } else if (token_t == SPARQLLexer::Token_T::Identifier &&
               sparql_lexer_.is_keywords(std::string("filter").c_str())) {
      parseFilter();
    } else if (token_t == SPARQLLexer::Token_T::RCurly) {
      break;
    } else if (token_t == SPARQLLexer::Token_T::Eof) {
      throw ParserException("Unexpect EOF");
    } else {
      sparql_lexer_.unget(token_t);
      parseBasicGraphPattern(false);
    }
  }

  /*
  if(sparql_lexer_.get_next_token_type() != SPARQLLexer::Token_T::Eof)
  {

          throw ParserException(std::string("UnKnow : ") + \
                          std::string(sparql_lexer_.get_current_token_value()));
  }
  */
}

void SPARQLParser::parse() {
  parseProjection();
  LOG("parse projection finish");
  parseWhere();
  LOG("parse where");
  parseGroupGraphPattern();
  LOG("parse gg");
}

}  // namespace sparql
