#ifndef FISHDB_H_
#define FISHDB_H_


namespace fishdb{


class DB{

public:
	DB() { }
	virtual DB();

	virtual int open_db(const std::string& db_name);
	virtual int close_db();
	
	virtual int load_data(const std::string& triple_file_path);

	/**
	*	get_triple("df:96666","df:type.object.name","?",query_result);
	*	get_triple("?","df:type.object.name","?",query_result);
	*/	
	virtual int get_triple(const std::string& sub_str,\
				const std::string& pre_str,\
				const std::string& obj_str,\
				std::shared_ptr<std::vector<core::TripleSpec>> query_result);

};//class DB

}//namespace fishdb

#endif // FISH_DB_H
