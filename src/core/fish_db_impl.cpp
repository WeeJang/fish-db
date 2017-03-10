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
	std::string db_data_dir_path("/Users/jiangwei/Workspace/cayley-backend/data/");
	db_data_dir_path.append(db_name);
	db_dir_path_ = db_data_dir_path;	
	if(-1 == utils::mkdir(db_data_dir_path)){
		fprintf(stderr,"mkdir %s failed! Maybe exist alreday .",db_data_dir_path.c_str());	
		exit(-1);
	}
	
	std::string root_table_path(db_dir_path_);
	root_table_path.append(std::string("root"));	
	root_table_ = db::RootTable(root_table_path);
}

int FishDBImpl::close_db()  ;
	
int FishDBImpl::load_data(const std::string& triple_file_path){
	//buffer,size= ringbuffer 读入 < (1<<27)的行数据
	//block = create_block_by_row_data(buffer,size)
	//构建iri 索引
	//构建block_index索引
	//添加到root_table
	//block.dump
	//release block

	int fd = ::open(		


}

/**
*	get_triple("df:96666","df:type.object.name","?",query_result);
*	get_triple("?","df:type.object.name","?",query_result);
*/	
int FishDbImplget_triple(const std::string& sub_str,\
		const std::string& pre_str,\
		const std::string& obj_str,\
		std::shared_ptr<std::vector<core::TripleSpec>> query_result);

};//class DB

}//namespace fishdb

#endif // FISH_DB_H
