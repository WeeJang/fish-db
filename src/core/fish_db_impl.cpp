#ifndef FISHDB_H_
#define FISHDB_H_

namespace fishdb{

int FishDBImpl::open_db(const std::string& db_name){
	//初始化root_table
	//载入iri 索引
	//初始化block_index索引	
}


int FishDBImpl::create_db(const std::string& db_name){
	db_name_ = db_name;
	init();
	if(-1 == utils::mkdir(db_dir_path_)){
		fprintf(stderr,"mkdir %s failed! Maybe exist alreday .",db_data_dir_path.c_str());	
		exit(-1);
	}
	root_table_ = db::RootTable(db_roottable_path_);
}

int FishDBImpl::close_db() 
	
int FishDBImpl::load_data(const std::string& triple_file_path){
	//buffer,size= ringbuffer 读入 < (1<<27)的行数据
	//block = create_block_by_row_data(buffer,size)
	//构建iri 索引
	//构建block_index索引
	//添加到root_table
	//block.dump
	//release block
	int fd = ::open(triple_file_path.c_str(),O_RDONLY);
	if(-1 == fd){
		fprintf(stderr,"FishDBImpl open file %s failed !",triple_file_path.c_str());
		exit(-1);				
	}

	std::string 	
	uint64_t row_offset_in_global = 0; //row-count in global
	uint64_t last_block_start_offset = 0; //last block start_offset
	uint64_t block_id_counter = -1; 

	size_t block_size = db::BlockData::BLOCK_DATA_SIZE_LIMIT;
	char* buffer = (char*)malloc(sizeof(char) * block_size);
	size_t has_read_size = -1;
	std::stringstream  sstream;
	char triple_elem_pos = 0;

	//not used ringbuffer
	while((has_read_size = ::read(fd,buffer,block_size) != -1){
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
			if(*(buffer + i) == '\n'){
				if(triple_elem_pos != 3){
					fprintf(stderr,"FishDBImpl Error,meet illegal newline flag\n");
					exit(-1);
				}	
				row_offset_in_global ++;
				triple_elem_pos = 0;
			}

			if(*(buffer + i) != '\t'){
				sstream << *(buffer+i);
				continue
			}
			auto elem_str = sstream.str();
			sstream.str("");
			//only sub/obj will be md5	
			if( 0 == triple_elem_pos || 2 == triple_elem_pos){
				if(IRIType::is_hashvalue(elem_str.c_str(),elem_str.size())){
					auto elem_int = ::strtol(elem_str.c_str(),nullptr,16);
					auto& iri_index =  hv_meta_iri_index_[elem_int];
					iri_index.add_index(triple_elem_pos++,row_offset_in_global);
					continue;
				}	
			}
			auto& iri_index =  ss_meta_iri_index_[elem_str];
			iri_index.add_index(triple_elem_pos++,row_offset_in_global);		
		}

		auto p_block = db::create_block_by_row_data(buffer,row_block_size);
		p_block->block_id(block_id_counter);
		p_block->row_start_index(last_block_start_offset); //row 	
		auto p_block_index = std::make_shared<db::BlockIndex>(p_block);		
		root_table_.append(last_block_start_offset,p_block_index);				
		std::string block_path(db_block_dir_path_);
		block_path += std::to_string(block_id_counter);
		p_block->dump(block_path);		
		//update	
		last_block_start_offset = row_offset_in_global;
	}
	free(buffer);			
	//dump iri_index
	hv_meta_iri_index_->save_to_files(db_iri_dir_path_);	
	ss_meta_iri_index_->save_to_files(db_iri_dir_path_);	
	
}

/**
*	get_triple("df:96666","df:type.object.name","?",query_result);
*	get_triple("?","df:type.object.name","?",query_result);
*/	
int FishDbImpl::get_triple(const std::string& sub_str,\
		const std::string& pre_str,\
		const std::string& obj_str,\
		std::shared_ptr<std::vector<core::TripleSpec>> query_result);

};//class DB

}//namespace fishdb

#endif // FISH_DB_H
