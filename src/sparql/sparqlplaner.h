#ifndef _SPARQL_PLANER_H_
#define _SPARQL_PLANER_H_

#include <memory>
#include <sstream>
#include <string>

#include "log.h"
#include "sparqlparser.h"
#include "valuefilter_impl.h"

#include "../core/fish_db_impl.h"
#include "../core/iri_type.h"

// TODO:集成后改成 forward-declara

namespace Index {
class Bigpita;
}

namespace query {
class TripleSetQuery {
 public:
  void add_vlv(const std::string &subj, const std::string &pred,
               const std::string &obj) {
    LOG("add_vlv : %s,%s,%s", subj.c_str(), pred.c_str(), obj.c_str());
  }

  void add_blv(const std::string &subj,
               const std::vector<std::string> &bound_vals,
               const std::string &pred, const std::string &obj) {
    LOG("add_blv : %s,%s,%s", subj.c_str(), pred.c_str(), obj.c_str());
    std::stringstream sstream;
    for (const auto &elem : bound_vals) {
      sstream << elem << ",";
    }
    LOG("(__inside__:bound_vals(%s))", sstream.str().c_str());
  }
  //这里暴露出一个问题：像这样的接口（客户端call）能否建议山哥 使用
  //shared_ptr<Index::Bigpita::ValueFilter>. 不然对生命周期的管理不太方便
  void add_vlf(const std::string &subj, const std::string &pred,
               const std::string &obj, Index::Bigpita::ValueFilter *fil) {
    LOG("add_vlf : %s,%s,%s,%s", subj.c_str(), pred.c_str(), obj.c_str(),
        fil->to_string().c_str());
  }

  void add_opt_vlv(const std::string &subj, const std::string &pred,
                   const std::string &obj) {
    LOG("add_opt_vlv : %s,%s,%s", subj.c_str(), pred.c_str(), obj.c_str());
  }
  void add_opt_vlf(const std::string &subj, const std::string &pred,
                   const std::string &obj, Index::Bigpita::ValueFilter *fil) {
    LOG("add_opt_vlf : %s,%s,%s,%s", subj.c_str(), pred.c_str(), obj.c_str(),
        fil->to_string().c_str());
  }
  void add_opt_blv(const std::string &subj,
                   const std::vector<std::string> &bound_vals,
                   const std::string &pred, const std::string &obj) {
    LOG("add_opt_blv : %s,%s,%s", subj.c_str(), pred.c_str(), obj.c_str());
    std::stringstream sstream;
    for (const auto &elem : bound_vals) {
      sstream << elem << ",";
    }
    LOG("(__inside__:bound_vals(%s))", sstream.str().c_str());
  }
};
}  // namespace query

namespace sparql {

class SPARQLPlaner {
  using ValueFilter = Index::Bigpita::ValueFilter;

 private:
  std::unique_ptr<ValueFilter> make_filter(const SPARQLParser::Filter &);
  std::unique_ptr<ValueFilter> make_filter(
      const SPARQLParser::TriplePatternElem &);

 public:
  query::TripleSetQuery handle(const std::string sparql_query);
};
}  // namespace sparql

#endif  // sparqlplaner.h
