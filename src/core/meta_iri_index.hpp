#include "../utils/file_util.h"
#include "../utils/tiny_log.hpp"
#include "iri_type.h"

#include <iostream>  //TODO: remove
#include <memory>
#include <sstream>
#include <fstream>
#include <unordered_map>

namespace core {

template <typename IRIType>
class IRIIndex;  // forward-declartion

template <typename T>
class MetaIRIIndex {
 public:
  using MAP_KEY_T = typename IRITypeTrait<T>::value_type;
  using MAP_VALUE_T = typename std::shared_ptr<IRIIndex<T>>;
  using Map_Type = typename std::unordered_map<MAP_KEY_T, MAP_VALUE_T>;

 public:
  MetaIRIIndex() { LOG("MetaIRIIndex init"); }

  MAP_VALUE_T get_IRI_index(MAP_KEY_T iri_value) {
    auto got = meta_iri_map_.find(iri_value);
    if (got == meta_iri_map_.end()) {
      LOG("get iri error");
      return {};
    } else {
      return got->second;
    }
  }
  // get
  MAP_VALUE_T get_IRI_index(T iri_value) {
    return get_IRI_index(iri_value.value());
  }

  void printf() { LOG("meta iri map size %d", meta_iri_map_.size()); }

  MAP_VALUE_T operator[](const MAP_KEY_T& key) {
    std::cout << "find " << key << std::endl;
    auto got = meta_iri_map_.find(key);
    if (got == meta_iri_map_.end()) {
      std::cout << "new !!! " << key << std::endl;
      auto new_elem = std::make_shared<IRIIndex<T>>(T(key));
      meta_iri_map_[key] = new_elem;
      return new_elem;
    } else {
      std::cout << "exit !!! " << key << std::endl;
      return got->second;
    }
  }

  MAP_VALUE_T operator[](const T& key) { return operator[](key.value()); }
  // insert
  void insert_IRI_index(T iri_value, MAP_VALUE_T shp_iri_index) {
    meta_iri_map_.insert(std::make_pair<MAP_KEY_T, MAP_VALUE_T>(
        iri_value.value(), shp_iri_index));
  }

  // save
  void save_to_files(const std::string& meta_iri_path) {
    save_to_files_<T>(meta_iri_path);
  }

  void load_from_files(const std::string& meta_iri_path) {
    load_from_files_<T>(meta_iri_path);
  }

 private:
  // load <HashValue>
  template <typename U>
  typename std::enable_if<std::is_same<U, IRIType::HashValue>::value,
                          void>::type
  load_from_files_(const std::string& meta_iri_path) {
    // TODO: ugly !
    std::string hv_iri_path(meta_iri_path);
    hv_iri_path.append("/HV");
    LOG("hash value load from file : %s",hv_iri_path.c_str());
    std::fstream ifs;
    ifs.open(hv_iri_path, std::ios::binary | std::ios::in);
    if(!ifs.is_open()){
	fprintf(stderr,"ifs open failed\n");
	exit(-1);
    }
    size_t r_c = 0;
    char try_get;
    ifs.get(try_get);
    while (!ifs.eof()) {
      ifs.unget();
      //read key
      MAP_KEY_T iri_key; 
      ifs.read(reinterpret_cast<char*>(&iri_key),sizeof(MAP_KEY_T));
      LOG("load iri_key size:  %zu",sizeof(MAP_KEY_T));
      LOG("load iri_key %zu",iri_key);
      r_c += sizeof(MAP_KEY_T);
      auto iri_index = std::make_shared<IRIIndex<IRIType::HashValue>>(
          IRIType::HashValue(iri_key));
      //read value
      size_t iri_value_size = 0;
      ifs.read(reinterpret_cast<char*>(&iri_value_size),sizeof(size_t));
      r_c += sizeof(size_t);
      char* p_iri_value = new char[iri_value_size];
      ifs.read(p_iri_value, iri_value_size);
      size_t offset = iri_index->deserialize(p_iri_value, iri_value_size);
      if (offset != iri_value_size) {
        fprintf(stderr, "file bad\n");
        exit(-1);
      }
      r_c += iri_value_size;
      LOG("load iri_value_size %zu",iri_value_size);
      meta_iri_map_[iri_key] = iri_index;
      delete [] p_iri_value;
      LOG("meta_iri_map_ load key : %llu", iri_key);
      LOG("!!!FUCK Read :%zu",r_c);
      ifs.get(try_get);
    }
  ifs.close();
  LOG("load HV key : finish");
  }

  // load <ShortString>
  template <typename U>
  typename std::enable_if<std::is_same<U, IRIType::ShortString>::value,
                          void>::type
  load_from_files_(const std::string& meta_iri_path) {
    // TODO: ugly !
    std::string ss_iri_path(meta_iri_path);
    ss_iri_path.append("/SS");
    LOG("short string load from file : %s",ss_iri_path.c_str());
    std::fstream ifs;
    ifs.open(ss_iri_path, std::ios::binary | std::ios::in);
    char try_get;
    ifs.get(try_get);
    while (!ifs.eof()) {
      //read key
      ifs.unget();
      size_t iri_key_size = 0;
      ifs.read(reinterpret_cast<char*>(&iri_key_size),sizeof(size_t));
      char* p_iri_key = new char[iri_key_size];
      ifs.read(p_iri_key, iri_key_size);
      std::string ss_index_key(p_iri_key,iri_key_size); 
      auto iri_index = std::make_shared<IRIIndex<IRIType::ShortString>>(
          IRIType::ShortString(ss_index_key));
      delete [] p_iri_key;

      //read value
      size_t iri_value_size = 0;
      ifs.read(reinterpret_cast<char*>(&iri_value_size),sizeof(size_t));
      char* p_iri_value = new char[iri_value_size];
      ifs.read(p_iri_value, iri_value_size);
      size_t offset = iri_index->deserialize(p_iri_value, iri_value_size);
      if (offset != iri_value_size) {
        fprintf(stderr, "file bad\n");
        exit(-1);
      }
      meta_iri_map_[ss_index_key] = iri_index;
      delete [] p_iri_value;
      LOG("meta_iri_map_ load key : %s", ss_index_key.c_str());
      ifs.get(try_get);
    }
    ifs.close();
    LOG("load SS key finish");
  }

  // save <HashValue>
  template <typename U>
  typename std::enable_if<std::is_same<U, IRIType::HashValue>::value,
                          void>::type
  save_to_files_(const std::string& meta_iri_path) {
    std::stringstream path_ss;
    path_ss << meta_iri_path << "/" << U::TYPE_STR;
    std::ofstream ofs;
    ofs.open(path_ss.str(), std::ios::out | std::ios::trunc | std::ios::binary);
    size_t w_c = 0;
    for (auto it = meta_iri_map_.begin(); it != meta_iri_map_.end(); ++it) {
      MAP_KEY_T key_value = it->first;
      size_t iri_key_size = sizeof(MAP_KEY_T);
      ofs.write(reinterpret_cast<char*>(&key_value),iri_key_size);
      w_c += iri_key_size;
      LOG("save write iri_key_size %zu", iri_key_size);
      LOG("save write iri_key  %zu", key_value);
      
      std::string value_buf;
      // write value
      if (0 != it->second->serialize(&value_buf)) {
        fprintf(stderr, "MetaIRIIndex save to %s failed !\n",
                path_ss.str().c_str());
        exit(-1);
      }
      size_t iri_value_size = value_buf.length();
      ofs.write(reinterpret_cast<char*>(&iri_value_size),sizeof(size_t));
      ofs.write(value_buf.c_str(),iri_value_size);
      w_c += sizeof(size_t);
      w_c += iri_value_size;
      LOG("save write iri_value_size %zu", iri_value_size);
      LOG("save write iri_value %s", value_buf.c_str());
    }
    ofs.close();
    LOG("!!!!FUCK HV Write %zu ", w_c);
  }

  // save <ShortString>
  template <typename U>
  typename std::enable_if<std::is_same<U, IRIType::ShortString>::value,
                          void>::type
  save_to_files_(const std::string& meta_iri_path) {
    std::stringstream path_ss;
    path_ss << meta_iri_path << "/" << T::TYPE_STR;
    std::ofstream ofs;
    ofs.open(path_ss.str(), std::ios::out | std::ios::trunc | std::ios::binary);

    for (auto it = meta_iri_map_.begin(); it != meta_iri_map_.end(); ++it) {
      const std::string& key_str = it->first;
      // write key
      size_t iri_key_size = key_str.length();
      ofs.write(reinterpret_cast<char*>(&iri_key_size),sizeof(size_t));
      ofs.write(key_str.c_str(),iri_key_size);
      LOG("save write iri_key_size %zu", iri_key_size);
      LOG("save write iri_key %s", key_str.c_str());
	
      // write value
      std::string value_buf;
      if (0 != it->second->serialize(&value_buf)) {
        fprintf(stderr, "MetaIRIIndex save to %s failed !\n",
                path_ss.str().c_str());
        exit(-1);
      }
      size_t iri_value_size = value_buf.length();
      ofs.write(reinterpret_cast<char*>(&iri_value_size),sizeof(size_t));
      ofs.write(value_buf.c_str(),iri_value_size);
      LOG("save write iri_value_size %zu", iri_value_size);
      LOG("save write iri_value %s", value_buf.c_str());
    }
    ofs.close();
  }


 private:
  Map_Type meta_iri_map_;
};

}  // namespace core
