#include "valuefilter_impl.h"

namespace sparql {

std::string ValueFilterImpl::to_string() const {
  return std::string("default value filter");
}

bool AmountFilter::operator()(const Index::Bigpita::ReturnValue& val) {
  if (val.is_real_nan()) {
    return false;
  }
  double cur = val.val.real;

  switch (type_) {
    case Type::Equal:
      return cur == datum_;
    case Type::NotEqual:
      return !(cur == datum_);
    case Type::Less:
      return cur < datum_;
    case Type::LessOrEq:
      return cur <= datum_;
    case Type::Greater:
      return cur > datum_;
    case Type::GreaterOrEq:
      return cur >= datum_;
    defult:
      return true;
  }
  return true;
}

std::string AmountFilter::to_string() const {
  std::stringstream sstream;
  sstream << "AmountFilter("
          << "type:" << type_ << "datum_:" << datum_ << ")";
  return sstream.str();
}

bool StringFilter::operator()(const Index::Bigpita::ReturnValue& val) {
  return datum_set_.find(val.val.str) != datum_set_.end();
}

std::string StringFilter::to_string() const {
  std::stringstream sstream;
  sstream << "StringFilter("
          << "datum_set_:[";

  for (const auto& elem : datum_set_) {
    sstream << elem << ",";
  }
  sstream << "]";
  sstream << ")";
  return sstream.str();
}

std::string DateFilter::to_string() const {
  std::stringstream sstream;
  sstream << "DateFilter("
          << "datum_set_:[";

  for (const auto& elem : datum_set_) {
    sstream << elem << ",";
  }
  sstream << "]";
  sstream << ")";
  return sstream.str();
}

}  // namespace sparql
