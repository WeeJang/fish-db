#ifndef _SPARQLLERER_H_
#define _SPARQLLERER_H_

#include <iostream>
#include <string>

#include "log.h"

namespace sparql {

class SPARQLLexer {
 public:
  // Token_T
  //	Variable   : "?id"
  //	IRI        : "<http://../.../>"
  //	Identifier : "select","where"
  //  String     : "\"招商银行\""
  //  Number	   : 113.4
  // 	Dot        : "."
  // 	LCurly	   : "{"
  // 	RCurly     : "}"
  // 	LRound     : "("
  // 	RRound     : ")"
  // 	Equal      : "="
  // 	NotEqual   : "!="
  // 	Less	   : "<"
  // 	LessOrEq   : "<="
  // 	Greater    : ">"
  // 	GreaterOrEq: ">="

  enum Token_T {
    None,
    Error,
    Eof,
    Variable,
    IRI,
    Identifier,
    String,
    Number,
    Dot,
    LCurly,
    RCurly,
    LRound,
    RRound,
    Equal,
    NotEqual,
    Less,
    LessOrEq,
    Greater,
    GreaterOrEq
  };

  // helper function
  static std::string to_string(Token_T token_t);

 public:
  SPARQLLexer(const std::string& input_string);
  ~SPARQLLexer();

  // has next
  bool has_next();
  // get next token
  Token_T get_next_token_type();
  // put back the token
  void unget(Token_T value) { put_back_ = value; }
  // get current value in string format
  std::string get_current_token_value();
  //
  bool is_keywords(const char* words);

 private:
  inline bool is_legal_numerical_character(const char& input_char);
  inline bool is_legal_identifier_character(const char& input_char);
  inline bool is_legal_variable_character(const char& input_char);
  inline bool is_legal_IRI_inner_character(const char& input_char);

 private:
  std::string input_string_;
  std::string::const_iterator current_pos_;
  std::string::const_iterator token_start_pos_;
  std::string::const_iterator token_stop_pos_;
  Token_T put_back_;
  bool is_token_finish_;
};

}  // namespace sparql

#endif  // SPARQLLexer.h
