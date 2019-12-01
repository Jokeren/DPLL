#ifndef TINY_SAT_COMMON_H
#define TINY_SAT_COMMON_H

#include <sys/time.h>

#define TINY_SAT_LOG_INFO(prefix, ...)      \
  do {                                      \
    fprintf(stdout, "INFO: %s-->", prefix); \
    fprintf(stdout, __VA_ARGS__);           \
    fprintf(stdout, "\n");                  \
  } while (0)


#define TINY_SAT_LOG_WARNING(prefix, ...)  \
  do {                                     \
    fprintf(stderr, "ERROR: %s:%d:%s-->",  \
      __FILE__, __LINE__, prefix);         \
    fprintf(stderr, __VA_ARGS__);          \
    fprintf(stderr, "\n");                 \
  } while (0)


#define TINY_SAT_LOG_ERROR(prefix, ...)    \
  do {                                     \
    fprintf(stderr, "ERROR: %s:%d:%s-->",  \
      __FILE__, __LINE__, prefix);         \
    fprintf(stderr, __VA_ARGS__);          \
    fprintf(stderr, "\n");                 \
    exit(1);                               \
  } while (0)


#define TINY_SAT_TIMER_START(elapsed_time, t1)  \
  do {                                          \
    elapsed_time = 0.0;                         \
    gettimeofday(&t1, NULL);                    \
  } while (0)


#define TINY_SAT_TIMER_END(elapsed_time, t1, t2)        \
  do {                                                  \
    gettimeofday(&t2, NULL);                            \
    elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;    \
    elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0; \
    elapsed_time /= 1000.0;                             \
  } while (0)

namespace tiny_sat {

enum Evaluation {
  EVAL_UNSAT = 0,
  EVAL_SAT = 1,
  EVAL_UNDECIDED = 2,
  EVAL_COUNT = 3
};

}  // namespace

#endif
