#include <cstdio>

#ifdef DEBUG
#define LOG(...)                                          \
  {                                                       \
    fprintf(stderr, "[%s: Line:%d]", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                         \
    fprintf(stderr, "\n");                                \
  }
#else
#define LOG(...)
#endif
