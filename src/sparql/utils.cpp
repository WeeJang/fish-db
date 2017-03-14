#include "utils.h"

namespace sparql{

int Utils::IRI2inner_format(const std::string& std_iri,std::string* format_result)
{
	std::regex prefix("^<http://rdf\\.dingfu.com/ns/(.*)>$");
	*format_result = static_cast<std::string>(std::regex_replace(std_iri, prefix, "df:$1"));
	if(*format_result == std_iri)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


}//sparql
