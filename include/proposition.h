#ifndef TINY_SAT_PROPOSITION_H
#define TINY_SAT_PROPOSITION_H

namespace tiny_sat {

class Proposition {
 public:
  explicit Proposition(unsigned int id) : id_(id) {}

  unsigned int id() const {
    return this->id_;
  }

  bool operator < (const Proposition &other) const {
    return this->id_ < other.id_;
  }
   
 private:
  unsigned int id_;
};

}  // namespace tiny_sat

#endif
