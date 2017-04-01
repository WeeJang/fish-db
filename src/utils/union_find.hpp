#ifndef UTILS_UNIONFIND_H_
#define UTILS_UNIONFIND_H_

#include <vector>
#include <unordered_map>
#include <set>

namespace utils{

template<typename T>
class UnionFind{
  public:
    UnionFind(const std::set<T>& vertex_set){ 
      vertex_size_ = vertex_set.size();
      group_id_vec_.resize(vertex_size_);
      init(vertex_set);
    }

   int find(const T& elem,size_t& ret_group_id){
	auto got = pos_map_.find(elem);
        if(got == pos_map_.end()){
        	return -1; 
	}
	ret_group_id = group_id_vec_[got->second];
        return 0;
   }

   int union_set(const T& vertex_1,const T& vertex_2){
   	size_t group_id_of_vertex_1 = 0 ;
	size_t group_id_of_vertex_2 = 0 ;
	if(-1 == find(vertex_1,group_id_of_vertex_1) || -1 == find(vertex_2,group_id_of_vertex_2)){
        	return -1;
        }
	for(auto& elem : group_id_vec_){
		if(elem == group_id_of_vertex_2){
			elem = group_id_of_vertex_1;
		}
	}	
	return 0;
   }
 
   bool connected(const T& vertex_1,const T& vertex_2){
	size_t group_id_of_vertex_1 = 0;
	size_t group_id_of_vertex_2 = 0;
	if(-1 == find(vertex_1,group_id_of_vertex_1) || \
		-1 == find(vertex_2,group_id_of_vertex_2)){
		return false;
	}
	return (group_id_of_vertex_1 == group_id_of_vertex_2);
   }

   const size_t count() const { return  vertex_size_; }

  private:
    void init(const std::set<T>& vertex_set){
      size_t count = 0;
      for(auto& v : vertex_set){
        pos_map_[v] = count;
	group_id_vec_[count] = count;
	count ++;
      }
    }

 private:
  size_t vertex_size_;
  std::unordered_map<T,size_t> pos_map_;
  std::vector<size_t> group_id_vec_;

};//class UnionFind
}//namespace utils

#endif
