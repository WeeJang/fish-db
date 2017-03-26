#include <iostream>
#include <string>

//#include "../src/sparql/sparqllexer.h"
//#include "../src/sparql/sparqlparser.h"
#include "../src/sparql/sparqlplaner.h"

using namespace sparql;

std::string sparql_str_1 =
    "\
	select ?id ?name ?val where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
           ?id <http://rdf.dingfu.com/ns/business.business_operation.accounts_receivable> ?val \
        }";

std::string sparql_str_2 =
    "\
	select ?id ?name ?val where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
		   optional { ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.isin> ?isin } \
           ?id <http://rdf.dingfu.com/ns/business.business_operation.accounts_receivable> ?val \
        }";

std::string sparql_str_3 =
    "\
	select ?id ?name ?val where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
           ?id <http://rdf.dingfu.com/ns/business.business_operation.accounts_receivable> ?val \
		   filter (?val <= 000001) \
        }";

std::string sparql_str_4 =
    "\
	select ?id ?name ?val where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
		   optional { ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.isin> ?isin } \
           ?id <http://rdf.dingfu.com/ns/business.business_operation.accounts_receivable> ?val \
           ?id <http://rdf.dingfu.com/ns/business.business_operation.other_property> ?val \
		   filter (?val = \"hello\") \
        }";

std::string sparql_str_5 =
    "\
	select ?id ?name ?val where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
           ?id <http://rdf.dingfu.com/ns/business.business_operation.accounts_receivable> \"hello\" \
        }";

int main(int argc, char** argv) {
  std::vector<std::string> test_set;
  test_set.push_back(sparql_str_1);
  test_set.push_back(sparql_str_2);
  test_set.push_back(sparql_str_3);
  test_set.push_back(sparql_str_4);
  test_set.push_back(sparql_str_5);

  for (auto test_it = test_set.begin(); test_it != test_set.end(); test_it++) {
    std::cout << "=====input=====" << std::endl;
    std::cout << *test_it << std::endl;
    std::cout << "===============" << std::endl;

    // SPARQLLexer sparql_lexer(*test_it);
    /**
    using Token_T = SPARQLLexer::Token_T;
    Token_T current_type;
    do
    {
            current_type = sparql_lexer.get_next_token_type();
            std::cout << SPARQLLexer::to_string(current_type);
            std::cout << "\t:\t" ;
            std::cout << "|" << sparql_lexer.get_current_token_value() << "|" <<
    std::endl; std::cout << sparql_lexer.is_keywords("select") << std::endl;
    }while(current_type != Token_T::Eof);
    **/
    // SPARQLParser sparql_parser(sparql_lexer);
    // sparql_parser.parse();
    // std::string parser_result = sparql_parser.to_string();
    // std::cout << parser_result << std::endl;
    SPARQLPlaner planer;
    auto query_set = planer.handle(*test_it);
  }

  return 0;
}
