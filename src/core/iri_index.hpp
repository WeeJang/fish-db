#ifndef IRI_INDEX_H_
#define IRI_INDEX_H_

#include "../utils/roaring.hh"
//#include "../utils/roaring.c"
#include "../utils/file_util.h"
#include "../utils/tiny_log.hpp"
#include "triple.h"

#include <fstream>

namespace core {

template <typename IRIType>
class IRIIndex {
 public:
  constexpr static int NUM_LIMIT = 65536;  // TODO: extend !
  using BitMap_T = Roaring64Map;
  // using BitMap_T = Roaring;
 public:
  IRIIndex() { LOG("iri_index init ..."); }
  explicit IRIIndex(IRIType value) : value_(value) {}
  // non-copyable
  IRIIndex(const IRIIndex&) = delete;
  IRIIndex(IRIIndex&&) = delete;
  IRIIndex& operator=(const IRIIndex&) = delete;
  IRIIndex& operator=(IRIIndex&&) = delete;

  void value(IRIType value) { value_ = value; }

  const BitMap_T& sub_index() const { return sub_index_; }
  const BitMap_T& pre_index() const { return pre_index_; }
  const BitMap_T& obj_index() const { return obj_index_; }

  const BitMap_T& at(core::TripleElemPos pos) const {
    if (core::TripleElemPos::sub == pos) {
      return sub_index_;
    } else if (core::TripleElemPos::pre == pos) {
      return pre_index_;
    } else if (core::TripleElemPos::obj == pos) {
      return obj_index_;
    } else {
      fprintf(stderr, "iri_index.at error!\n");
      exit(-1);
    }
  }

  void add_index(const char triple_elem_pos, uint64_t pos) {
    LOG("tirple elem pos : %d, pos : %llu pos", triple_elem_pos, pos);
    switch (triple_elem_pos) {
      case 0:
        sub_index_.add(pos);
        break;
      case 1:
        pre_index_.add(pos);
        break;
      case 2:
        obj_index_.add(pos);
        break;
      default:
        fprintf(stderr, "IRIIndex add_index : %d error\n", triple_elem_pos);
        exit(-1);
    }
  }

  bool run_optimize() {
    return (sub_index_.runOptimize() && pre_index_.runOptimize() &&
            obj_index_.runOptimize());
  }

  bool dump_to_files(const std::string dict_path) {
    // run_optimize();
    return (dump_to_file(sub_index_, dict_path + std::string("/sub.index"))) &&
           (dump_to_file(pre_index_, dict_path + std::string("/pre.index"))) &&
           (dump_to_file(obj_index_, dict_path + std::string("/obj.index")));
  }

  bool load_from_files(const std::string dict_path) {
    return (load_from_file(sub_index_,
                           dict_path + std::string("/sub.index"))) &&
           (load_from_file(pre_index_,
                           dict_path + std::string("/pre.index"))) &&
           (load_from_file(obj_index_, dict_path + std::string("/obj.index")));
  }

 private:
  bool dump_to_file(BitMap_T& bit_map, std::string dump_file_path) {
    auto expected_size = bit_map.getSizeInBytes() + 4;
    char* serialized_bytes = new char[expected_size];
    auto write_ret = bit_map.write(serialized_bytes);
    LOG("expected_size : %llu, write_re : %llu", expected_size, write_ret);
    bool result = false;
    if (!utils::write_to_file(dump_file_path, serialized_bytes,
                              expected_size)) {
      fprintf(stderr, "dump bit map to file :%s failed !\n",
              dump_file_path.c_str());
      exit(-1);
    } else {
      result = true;
    }
    delete[] serialized_bytes;
    return result;
  }

  bool load_from_file(BitMap_T& bit_map, std::string load_file_path) {
    std::ifstream is(load_file_path, std::ifstream::binary);
    if (is) {
      LOG("load from file : %s", load_file_path.c_str());
      is.seekg(0, is.end);
      auto length = is.tellg();
      is.seekg(0, is.beg);
      char* serialized_bytes = new char[length];
      is.read(serialized_bytes, length);
      bit_map = BitMap_T::read(serialized_bytes);
      delete[] serialized_bytes;
      is.close();
      LOG("load from file : %s success !", load_file_path.c_str());
      return true;
    } else {
      LOG("open load file %s failed !", load_file_path.c_str());
      return false;
    }
  }

 private:
  IRIType value_;
  BitMap_T sub_index_;
  BitMap_T pre_index_;
  BitMap_T obj_index_;

};  // IRIIndex

}  // namespace core

#endif  // IRI_INDEX_H_
