g++ -v -std=c++11 -march=native -o query_server query_server.cpp ../query/triple_query.cpp ../query/query_executor.cpp ../sparql/sparqllexer.cpp  ../sparql/sparqlparser.cpp  ../sparql/utils.cpp ../sparql/valuefilter_impl.cpp  ../db/block.cpp ../db/block_index.cpp ../db/root_table.cpp ../db/block_data_seeker.cpp ../core/triple.cpp ../core/fish_db_impl.cpp ../core/iri_type.cpp ../utils/file_util.cpp ../utils/roaring.c -lboost_system

#g++ -v -g -std=c++11 -DDEBUG -lboost_system -o query_client query_client.cpp
#g++ -v -g -std=c++11 -fsanitize=address -fno-omit-frame-pointer  -DDEBUG -lboost_system -o query_server query_server.cpp
