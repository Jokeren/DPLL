#ifndef TINY_SAT_PROPOSITION_H
#define TINY_SAT_PROPOSITION_H

namespace tiny_sat {

class Proposition {
 public:
  explicit Proposition(size_t id) : id_(id) {}

  size_t id() : {
    return this->id_;
  }
   
 private:
  size_t id_;
};

}  // namespace tiny_sat

#endif
