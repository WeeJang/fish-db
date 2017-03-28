#include <string>

namespace test{

namespace test_query{
std::string sparql_str =
    "select ?id ?name ?parent ?child where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.isin> cne100001zg7 .  \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.parent> ?parent .  \
 		   ?parent <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.child> ?child .  \
        }";

}
}
