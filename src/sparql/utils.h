#ifndef _SPARQL_UTILS_H_
#define _SPARQL_UTILS_H_

#include <iostream>
#include <memory>
#include <regex>
#include <type_traits>

namespace sparql {

class Utils {
 public:
  // trans "<http://rdf.dingfu.com/ns/xxx>" => "df:xxx"
  // if failed(no match)
  //    then the std_iri == return value
  static int IRI2inner_format(const std::string& std_iri,
                              std::string* format_result);
};

//---------------------make_unique-----------------------
template <typename T, typename... ARGS>
inline
    typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T> >::type
    make_unique(ARGS&&... args) {
  return std::unique_ptr<T>(new T(std::forward<ARGS>(args)...));
}

template <typename T>
inline typename std::enable_if<std::is_array<T>::value &&
                                   std::extent<T>::value == 0,
                               std::unique_ptr<T> >::type
make_unique(size_t size) {
  using _T = typename std::remove_extent<T>::type;
  return std::unique_ptr<T>(new _T[size]());
}

template <typename T, typename... ARGS>
inline typename std::enable_if<std::extent<T>::value != 0, void>::type
make_unique(ARGS&&...) = delete;
//---------------------make_unique-----------------------

}  // namespace sparql

#endif  // utils.h
