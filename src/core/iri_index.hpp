
#include <sdsl/bit_vectors.hpp>

namespace core{

template<typename IRI_Type>
class IRI_Index{
	constexpr static int NUM_LIMIT = 1024 ; //TODO: extend !

public:
	explicit IRI_Index(IRI_Type value):value_(value),\
				sub_index_(NUM_LIMIT,0),\
				pre_index_(NUM_LIMIT,0),\
				obj_index_(NUM_LIMIT,0){
	}
	//non-copyable
	IRI_Index(const IRI_Index&) = delete ;
	IRI_Index(IRI_Index&&) = delete;
	IRI_Index& operator = (const IRI_Index&)  = delete;
	IRI_Index& operator = (IRI_Index&&) = delete;
	
	const sdsl::bit_vector& sub_index() const {
		return sub_index_; 
	}
	const sdsl::bit_vector& pre_index() const {
		return pre_index_;
	}
	const sdsl::bit_vector& obj_index() const {
		return obj_index_;
	}		
	
private:
	IRI_Type value_;
	sdsl::bit_vector sub_index_;	
	sdsl::bit_vector pre_index_;
	sdsl::bit_vector obj_index_;

};//IRI_Index




}//namespace core



