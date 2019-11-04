#ifndef TINY_SAT_COMMON_H
#define TINY_SAT_COMMON_H

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


namespace tiny_sat {

enum Evaluation {
  EVAL_UNSAT = 0,
  EVAL_SAT = 1,
  EVAL_UNDECIDED = 2,
  EVAL_COUNT = 3
};

}  // namespace

#endif
