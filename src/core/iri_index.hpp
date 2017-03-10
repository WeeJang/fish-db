
#include <sdsl/bit_vectors.hpp>

namespace core{

template<typename IRIType>
class IRIIndex{
	constexpr static int NUM_LIMIT = 1024 ; //TODO: extend !

public:
	explicit IRIIndex(IRIType value):value_(value),\
				sub_index_(NUM_LIMIT,0),\
				pre_index_(NUM_LIMIT,0),\
				obj_index_(NUM_LIMIT,0){
	}
	//non-copyable
	IRIIndex(const IRIIndex&) = delete ;
	IRIIndex(IRIIndex&&) = delete;
	IRIIndex& operator = (const IRIIndex&)  = delete;
	IRIIndex& operator = (IRIIndex&&) = delete;
	
	const sdsl::bit_vector& sub_index() const {
		return sub_index_; 
	}
	const sdsl::bit_vector& pre_index() const {
		return pre_index_;
	}
	const sdsl::bit_vector& obj_index() const {
		return obj_index_;
	}		

	bool save_to_files(const std::string dict_path){
		return (sdsl::save_to_file(sub_index_,dict_path + std::string("/sub.index"))) && \
			(sdsl::save_to_file(pre_index_,dict_path + std::string("/pre.index")))&& \
			(sdsl::save_to_file(obj_index_,dict_path + std::string("/obj.index")));
	}
	
	bool load_from_files(const std::string dict_path){
		return (sdsl::load_from_file(sub_index_,dict_path + std::string("/sub.index"))) && \
			(sdsl::load_from_file(pre_index_,dict_path + std::string("/pre.index"))) && \
			(sdsl::load_from_file(obj_index_,dict_path + std::string("/obj.index")));			
	}



private:
	IRIType value_;
	sdsl::bit_vector sub_index_;	
	sdsl::bit_vector pre_index_;
	sdsl::bit_vector obj_index_;

};//IRIIndex




}//namespace core



