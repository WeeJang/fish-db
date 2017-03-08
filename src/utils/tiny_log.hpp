#ifndef TINY_LOG_H_
#define TINY_LOG_H_

#include <cstdio>

#define LOG(...) \
	fprintf(stderr," %s Line: %d \t " , __FILE__, __LINE__); \
	fprintf(stderr,__VA_ARGS__); \
	fprintf(stderr,"\n");

#endif
