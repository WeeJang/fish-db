#ifndef _SPARQLPARSER_H_
#define _SPARQLPARSER_H_

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "log.h"
#include "sparqllexer.h"
#include "utils.h"

namespace sparql {

class SPARQLParser {
 public:
  // A parseing exception
  struct ParserException {
    std::string message_;
    ParserException(std::string message) : message_(message) {
      LOG("%s", message_.c_str());
    }
    ParserException(const char* message) : message_(message) {
      LOG("%s", message);
    }
    std::string to_string() const { return message_; }
  };

  // A basic elememt of triple parttern
  struct TriplePatternElem {
    enum Type { Variable, IRI, Literal, Blank };
    enum LiteralType { Integer, Double, String, Function, None };

    TriplePatternElem()
        : type_(Type::Blank), literal_type_(LiteralType::None), value_("") {}
    TriplePatternElem(Type type, LiteralType literal_type, std::string value)
        : type_(type), literal_type_(literal_type), value_(value) {}

    Type type_;
    LiteralType literal_type_;
    std::string value_;
  };

  using TPElem = TriplePatternElem;

  // triple pattern
  struct TriplePattern {
    TriplePattern(TPElem subj, TPElem pred, TPElem obj, bool is_option)
        : subj_(subj), pred_(pred), obj_(obj), is_option_(is_option) {}

    TriplePattern(TPElem subj, TPElem pred, TPElem obj)
        : subj_(subj), pred_(pred), obj_(obj), is_option_(false) {}

    TPElem subj_;
    TPElem pred_;
    TPElem obj_;
    bool is_option_;
  };

  // Filter
  // TODO : 考虑的不够全
  struct Filter {
    enum Type {
      Equal,
      NotEqual,
      Less,
      LessOrEq,
      Greater,
      GreaterOrEq,
      Function
    };
    Type filter_type_;
    std::string variable_str_;
    // if Type == Function then filter_args[0] is functions_register_name
    std::vector<TriplePatternElem> filter_args_;
  };

 public:
  std::vector<std::string> project_variables() const {
    return project_variables_;
  }
  std::vector<TriplePattern> triple_patterns() const {
    return triple_patterns_;
  }
  std::map<std::string, Filter> filters() const { return filters_; }

  // helper function
  TPElem make_variable(std::string variable);
  TPElem make_IRI(std::string IRI);
  TPElem make_notype_literal(std::string literal_value);
  TPElem make_integer_literal(std::string literal_value);
  TPElem make_double_literal(std::string literal_value);
  TPElem make_string_literal(std::string literal_value);
  TPElem make_function_literal(std::string literal_value);
  // TPElem make_blank();

  static std::string to_string(const TriplePatternElem&);
  static std::string to_string(const TriplePattern&);
  static std::string to_string(const Filter&);
  std::string to_string();

 public:
  SPARQLParser(const SPARQLLexer& sparql_lexer) : sparql_lexer_(sparql_lexer) {}
  ~SPARQLParser() {}

  void parse();

 private:
  // parse projection : "select ?data ?id"
  void parseProjection();
  // parse where : "where"
  void parseWhere();
  // parser filter
  void parseFilter();
  // parse basic graph pattern
  void parseBasicGraphPattern(bool is_option);
  // parse group graph pattern
  void parseGroupGraphPattern();

 private:
  SPARQLLexer sparql_lexer_;
  //所有要输出的变量
  std::vector<std::string> project_variables_;
  //所有三元组模式
  std::vector<TriplePattern> triple_patterns_;
  // filters 限制的肯定variable,所以用string
  std::map<std::string, Filter> filters_;
};

}  // namespace sparql

#endif
