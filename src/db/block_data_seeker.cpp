
#include "block_data_seeker.h"


namespace db{


constexpr static size_t BlockDataSeeker::start_offset_of_block_ = offsetof(Block,block_data_)  ;

constexpr static size_t BlockDataSeeker::end_offset_of_block_ = offsetof(Block,row_data_offset_);

BlockDataSeeker::BlockDataSeeker(const std::string& block_file_name)::fd_(-1),is_file_open_(false){	
	int fd_ = ::open(block_file_name.c_str(),O_RDONLY);
	if(-1 == fd_){
		fprintf(stderr,"BlockDataSeeker open file : %s failed !", block_file_name.c_str());
		exit(-1);	
	}
	is_file_open_ = true;		
}


BlockDataSeeker::~BlockDataSeeker(){
	if(is_file_open){
		close(fd_);
		is_file_open = false;
	}
}

int BlockDataSeeker::get_triple_by_index(uint64_t row_index,std::shared_ptr<core::TripleSpec> p_triple_spec){
	uint64_t seek_pos = row_index + start_offset_of_block_;
	assert(seek_pos <  end_offset_of_block_);
	
	if(lseek(fd_,seek_pos,SEEK_SET) != seek_pos){
		fprintf(stderr,"BlockDataSeeker seek_pos failed !");
		exit(-1);			
	}
	char triple_set_pos = 0;	
	std::string elem_buffer;
	while(seek_pos < end_offset_of_block_){
		char read_c;
		if(-1 == ::read(fd_,&read_c,1)){
			fprintf(stderr,"BlockDataSeeker read failed !");
			exit(-1);
		}
	
		if('\t' == read_c){
			if(0 == triple_set_pos){
				p_triple_spec->sub(elem_buffer);
			}else if(1 == triple_set_pos){
				p_triple_spec->prd(elem_buffer);
			}else{
				p_triple_spec->obj(elem_buffer);
			}		
			elem_buffer.clear()	
		}else if('\n' ==  read_c){
			break;	
		}else{
			elem_buffer.push_back(read_c);
		}
		seek_pos ++;
	}
	return 0;
}


}//namespace db
