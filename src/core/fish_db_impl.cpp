#include "fish_db_impl.h"

namespace fishdb{


void FishDBImpl::init(){
	db_dir_path_ =std::string("/Users/jiangwei/Workspace/cayley-backend/data/");
	db_dir_path_ += db_name_;
	
	db_block_dir_path_ = db_dir_path_;	
	db_iri_dir_path_ = db_dir_path_;	
	db_roottable_path_ = db_dir_path_;
	
	db_block_dir_path_ += std::string("/block/");
	db_iri_dir_path_   += std::string("/iri_index/");	
	db_roottable_path_ += std::string("/root");
}


int FishDBImpl::open_db(const std::string& db_name){
	//初始化root_table
	//载入iri 索引
	//初始化block_index索引
	db_name_ = db_name;
	init();
	root_table_.fast_init_by_dump_file(db_roottable_path_);
	hv_meta_iri_index_.load_from_files(db_iri_dir_path_);	
	ss_meta_iri_index_.load_from_files(db_iri_dir_path_);	
	
	std::vector<std::string> block_file_list = utils::get_file_list(db_block_dir_path_);
	root_table_.resize_offset_list(block_file_list.size());	
	for(auto& file_name : block_file_list){ 
		std::string block_file_path(db_block_dir_path_);
		block_file_path += file_name;
		auto p_block_index = std::make_shared<db::BlockIndex>(block_file_path);
		root_table_.modify_block_index_at(p_block_index->block_id(),p_block_index->row_start_index(),p_block_index);
	}	
	return 0;
}


int FishDBImpl::create_db(const std::string& db_name){
	db_name_ = db_name;
	init();
	if(-1 == utils::mkdir(db_dir_path_)){
		fprintf(stderr,"mkdir %s failed! Maybe exist alreday .",db_dir_path_.c_str());	
		exit(-1);
	}
	if(-1 == utils::mkdir(db_block_dir_path_)){
		fprintf(stderr,"mkdir %s failed! Maybe exist alreday .",db_block_dir_path_.c_str());	
		exit(-1);
	}
	if(-1 == utils::mkdir(db_iri_dir_path_)){
		fprintf(stderr,"mkdir %s failed! Maybe exist alreday .",db_iri_dir_path_.c_str());	
		exit(-1);
	}

	root_table_ = db::RootTable(db_roottable_path_);
	return 0;
}

int FishDBImpl::close_db(){
	return 0;
}
	
int FishDBImpl::load_data(const std::string& triple_file_path){
	int fd = ::open(triple_file_path.c_str(),O_RDONLY);
	if(-1 == fd){
		fprintf(stderr,"FishDBImpl open file %s failed !",triple_file_path.c_str());
		exit(-1);				
	}

	uint64_t row_offset_in_global = 0; //row-count in global
	uint64_t last_block_start_offset = 0; //last block start_offset
	uint64_t block_id_counter = -1; 

	size_t block_size = db::BlockData::BLOCK_DATA_SIZE_LIMIT;
	char* buffer = (char*)malloc(sizeof(char) * block_size);
	size_t has_read_size = -1;
	std::stringstream  sstream;
	char triple_elem_pos = 0;

	//not used ringbuffer
	while((has_read_size = ::read(fd,buffer,block_size)) != -1){
		LOG("has read size : %zu",has_read_size);
		if( (1 == has_read_size) && ('\n' == *buffer) ){
			LOG("read finish !");
			break;	
		}

		block_id_counter ++;		
		//find last '\n'	
		size_t row_block_size = has_read_size;	
		for(; row_block_size > 0 ; row_block_size--){
			if(*(buffer + row_block_size) == '\n'){
				break;
			}	
		}	
		//lseek	
		if(-1 == ::lseek(fd,row_block_size - has_read_size,SEEK_CUR)){
			fprintf(stderr,"FishDBImpl lseek failed !\n");
			exit(-1);
		}
	
		for(size_t i = 0; i < row_block_size ; i++){
			if(triple_elem_pos == 3){
				triple_elem_pos = 0;
				row_offset_in_global ++;
			}
			if(*(buffer + i) == '\n'){
				if(triple_elem_pos != 2){
					fprintf(stderr,"FishDBImpl Error,meet illegal newline flag\n");
					exit(-1);
				}	
			}else if(*(buffer + i) != '\t'){
				sstream << *(buffer+i);
				continue;
			}else{
			}

			auto elem_str = sstream.str();
			sstream.str("");
			LOG("elem str : %s",elem_str.c_str());
			//only sub/obj will be md5	
			if( 0 == triple_elem_pos || 2 == triple_elem_pos){
				if(core::IRIType::is_hashvalue(elem_str.c_str(),elem_str.size())){
					LOG("elem is hv");
					auto elem_int = ::strtol(elem_str.c_str(),nullptr,16);
					auto iri_index =  hv_meta_iri_index_[elem_int];
					iri_index->add_index(triple_elem_pos++,row_offset_in_global);
					continue;
				}	
			}
			LOG("elem is ss");
			auto iri_index =  ss_meta_iri_index_[elem_str];
			if(iri_index.get() == nullptr){
				LOG("nullptr !");
			}
			iri_index->add_index(triple_elem_pos++,row_offset_in_global);		
			LOG("add result triple_pos : %hhd, row_offset_in_global : %llu",triple_elem_pos,row_offset_in_global);
						
		}

		//ugly	
		auto p_block = db::create_block_by_raw_data(buffer,row_block_size);
		p_block->block_id(block_id_counter);
		p_block->row_start_index(last_block_start_offset); //row 	
		auto p_block_index = std::make_shared<db::BlockIndex>(p_block.get());		
		root_table_.append_block_index(last_block_start_offset,p_block_index);				
		std::string block_path(db_block_dir_path_);
		block_path += std::to_string(block_id_counter);
		p_block->dump(block_path);		
		//update	
		last_block_start_offset = row_offset_in_global;
	}
	free(buffer);			
	//dump iri_index
	hv_meta_iri_index_.save_to_files(db_iri_dir_path_);	
	ss_meta_iri_index_.save_to_files(db_iri_dir_path_);
	//root_table dump
	root_table_.dump(db_roottable_path_);
	return 0;
}

std::shared_ptr<db::BlockDataSeeker> FishDBImpl::get_block_data_seeker(size_t block_id){
	auto got = block_data_seeker_map_.find(block_id);
	if(got == block_data_seeker_map_.end()){
		std::string block_path(db_block_dir_path_);
		block_path.append("/").append(std::to_string(block_id));
		auto p_seeker =  std::make_shared<db::BlockDataSeeker>(block_path);
		block_data_seeker_map_[block_id] = p_seeker;
		return p_seeker;
	}else{
		return got->second;		
	}
}

int FishDBImpl::get_triple_by_row_index(uint64_t row_index,std::shared_ptr<core::TripleSpec> p_triple_spec){
	uint32_t block_id;
	uint64_t block_offset;
				
	LOG("row index : %d",row_index);	
	if(-1 == root_table_.get_seek_pos_by_row_index(row_index,block_id,block_offset)){
		fprintf(stderr,"fish_db_imple get_seek_pos failed\n");
		return -1;	
	}

	LOG("block_id : %d, block_offset : %d",block_id,block_offset);	
	auto p_block_seeker = get_block_data_seeker(block_id);
	
	if(-1 == p_block_seeker->get_triple_by_index(block_offset,p_triple_spec)){
		fprintf(stderr,"fish_db_imple get_triple_by_index failed\n");
		return -1;
}	
	return 0;		
}


/**
*	get_triple("df:96666","df:type.object.name","?",query_result);
*	get_triple("?","df:type.object.name","?",query_result);
*/	
int FishDBImpl::get_triple(const std::string& sub_str,\
		const std::string& pre_str,\
		const std::string& obj_str,\
		std::shared_ptr<std::vector<core::TripleSpec>> query_result){
	return 0;				
}

}//namespace fishdb

