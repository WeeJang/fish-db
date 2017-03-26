#include "sparqllexer.h"

namespace sparql {

// helper function
std::string SPARQLLexer::to_string(Token_T token_t) {
  switch (token_t) {
    case Token_T::None:
      return std::string("Token_T::None");
    case Token_T::Error:
      return std::string("Token_T::Error");
    case Token_T::Eof:
      return std::string("Token_T::Eof");
    case Token_T::Variable:
      return std::string("Token_T::Variable");
    case Token_T::IRI:
      return std::string("Token_T::IRI");
    case Token_T::Identifier:
      return std::string("Token_T::Identifier");
    case Token_T::String:
      return std::string("Token_T::String");
    case Token_T::Dot:
      return std::string("Token_T::Dot");
    case Token_T::LCurly:
      return std::string("Token_T::LCurly");
    case Token_T::RCurly:
      return std::string("Token_T::RCurly");
    case Token_T::Equal:
      return std::string("Token_T::Equal");
    case Token_T::NotEqual:
      return std::string("Token_T::NotEqual");
    case Token_T::Less:
      return std::string("Token_T::Less");
    case Token_T::LessOrEq:
      return std::string("Token_T::LessOrEq");
    case Token_T::Greater:
      return std::string("Token_T::Greater");
    case Token_T::GreaterOrEq:
      return std::string("Token_T::GreaterOrEq");
    default:
      return std::string("no-defined");
  }
}

SPARQLLexer::SPARQLLexer(const std::string& input_string)
    : input_string_(input_string),
      current_pos_(input_string_.begin()),
      token_start_pos_(current_pos_),
      token_stop_pos_(current_pos_),
      put_back_(Token_T::None),
      is_token_finish_(false) {}

SPARQLLexer::~SPARQLLexer() {}

bool SPARQLLexer::is_keywords(const char* words) {
  bool is_matched = true;
  std::string::const_iterator current_token_iter = token_start_pos_;
  char* ch = const_cast<char*>(words);
  for (; current_token_iter != token_stop_pos_;) {
    if (*ch == '\0') break;
    if (*(current_token_iter++) != *(ch++)) {
      is_matched = false;
      break;
    }
  }
  if (current_token_iter != token_stop_pos_ || *ch != '\0') is_matched = false;
  return is_matched;
}

inline bool SPARQLLexer::is_legal_identifier_character(const char& input_char) {
  // LOG("is iden_char : %c",input_char);
  if (((input_char >= '0') && (input_char <= '9')) ||
      ((input_char >= 'a') && (input_char <= 'z')) ||
      ((input_char >= 'A') && (input_char <= 'Z')) || ((input_char == '_'))) {
    return true;
  } else {
    // LOG("false");
    return false;
  }
}

inline bool SPARQLLexer::is_legal_numerical_character(const char& input_char) {
  if ((input_char >= '0') && (input_char <= '9')) {
    return true;
  } else {
    return false;
  }
}

inline bool SPARQLLexer::is_legal_variable_character(const char& input_char) {
  if (((input_char >= '0') && (input_char <= '9')) ||
      ((input_char >= 'a') && (input_char <= 'z')) ||
      ((input_char >= 'A') && (input_char <= 'Z')) || ((input_char == '_'))) {
    return true;
  } else {
    return false;
  }
}

inline bool SPARQLLexer::is_legal_IRI_inner_character(const char& input_char) {
  if ((input_char == '\t') || (input_char == ' ') || (input_char == '\n') ||
      (input_char == '\r')) {
    return false;
  } else {
    return true;
  }
}

std::string SPARQLLexer::get_current_token_value() {
  return std::string(token_start_pos_, token_stop_pos_);
}

SPARQLLexer::Token_T SPARQLLexer::get_next_token_type() {
  if (is_token_finish_) return Token_T::None;

  if (put_back_ != Token_T::None) {
    Token_T ret_value = put_back_;
    put_back_ = Token_T::None;
    return ret_value;
  }

  while (current_pos_ != input_string_.end()) {
    is_token_finish_ = false;
    token_start_pos_ = current_pos_;
    switch (*(current_pos_++)) {
      case ' ':
      case '\t':
        continue;
      case '{':
        token_stop_pos_ = current_pos_;
        return Token_T::LCurly;
      case '}':
        token_stop_pos_ = current_pos_;
        return Token_T::RCurly;
      case '(':
        token_stop_pos_ = current_pos_;
        return Token_T::LRound;
      case ')':
        token_stop_pos_ = current_pos_;
        return Token_T::RRound;
      case '>':
        if (*current_pos_ == '=') {
          current_pos_++;
          token_stop_pos_ = current_pos_;
          return Token_T::GreaterOrEq;
        }
        token_stop_pos_ = current_pos_;
        return Token_T::Greater;
      case '.':
        token_stop_pos_ = current_pos_;
        return Token_T::Dot;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        while (current_pos_ != input_string_.end() &&
               is_legal_numerical_character(*current_pos_)) {
          current_pos_++;
        }
        if (*current_pos_ == '.') {
          current_pos_++;
          while (current_pos_ != input_string_.end() &&
                 is_legal_numerical_character(*current_pos_)) {
            current_pos_++;
          }
        }
        token_stop_pos_ = current_pos_;
        return Token_T::Number;
      case '"':
        while (current_pos_ != input_string_.end()) {
          if (*(current_pos_++) == '"') {
            token_stop_pos_ = current_pos_;
            return Token_T::String;
          }
        }
        is_token_finish_ = true;
        return Token_T::Error;
      case '?':
        while (current_pos_ != input_string_.end() &&
               is_legal_variable_character(*current_pos_)) {
          current_pos_++;
        }
        token_stop_pos_ = current_pos_;
        return Token_T::Variable;
      case '=':
        token_stop_pos_ = current_pos_;
        return Token_T::Equal;

      case '!':
        if (*current_pos_ == '=') {
          token_stop_pos_ = current_pos_;
          return Token_T::NotEqual;
        }
        is_token_finish_ = true;
        return Token_T::Error;
      case '<':
        if (*current_pos_ == ' ') {
          token_stop_pos_ = current_pos_;
          return Token_T::Less;
        } else if (*current_pos_ == '=') {
          current_pos_++;
          token_stop_pos_ = current_pos_;
          return Token_T::LessOrEq;
        }

        while (current_pos_ != input_string_.end() &&
               is_legal_IRI_inner_character(*current_pos_)) {
          if (*(current_pos_++) == '>') {
            token_stop_pos_ = current_pos_;
            return Token_T::IRI;
          }
        }
        is_token_finish_ = true;
        return Token_T::Error;
        break;

      default:
        while (current_pos_ != input_string_.end() &&
               is_legal_identifier_character(*current_pos_)) {
          current_pos_++;
        }
        token_stop_pos_ = current_pos_;
        // LOG("stop : %c",*token_stop_pos_);
        return Token_T::Identifier;
    }
  }

  is_token_finish_ = true;
  return Token_T::Eof;
}

}  // namespace sparql
