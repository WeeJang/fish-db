#ifndef UTILS_UNIONFIND_H_
#define UTILS_UNIONFIND_H_

#include <vector>
#include <unordered_map>

namespace utils{

template<typename T>
class UnionFind{
  public:
    UnionFind(const std::vector<T>& vec){ 
      uf_size_ = vec.size();
      group_id_vec_.resize(vec.size());
      init(vec);
    }

   int find(const T& elem,size_t& ret_group_id){
	auto got = pos_map_.find(elem);
        if(got == pos_map_.end()){
        	return -1; 
	}
	ret_group_id = group_id_vec_[got->second];
        return 0;
   }

   int union_set(const T& elem_1,const T& elem_2){
   	size_t group_id_of_elem_1 = 0 ;
	size_t group_id_of_elem_2 = 0 ;
	if(-1 == find(elem_1,group_id_of_elem_1) || -1 == find(elem_2,group_id_of_elem_2)){
        	return -1;
        }
	for(auto& elem : group_id_vec_){
		if(elem == group_id_of_elem_2){
			elem = group_id_of_elem_1;
		}
	}	
	return 0;
   }
 
   bool connected(const T& elem_1,const T& elem_2){
	size_t group_id_of_elem_1 = 0;
	size_t group_id_of_elem_2 = 0;
	if(-1 == find(elem_1,group_id_of_elem_1) || \
		-1 == find(elem_2,group_id_of_elem_2)){
		return false;
	}
	return (group_id_of_elem_1 == group_id_of_elem_2);
   }

   const size_t count() const { return  uf_size_; }

  private:
    void init(const std::vector<T>& vec){
      for(size_t i = 0 ; i < uf_size_ ; i++){
	pos_map_[vec[i]] = i ;	
        group_id_vec_[i] = i ;
      }
    }

 private:
  size_t uf_size_;
  std::unordered_map<T,size_t> pos_map_;
  std::vector<size_t> group_id_vec_;

};//class UnionFind
}//namespace utils

#endif
