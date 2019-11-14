#ifndef TINY_SAT_PATH_H
#define TINY_SAT_PATH_H

namespace tiny_sat {

template <typename T>
class Path {
 public:
  Path(size_t capacity = 1) : capacity_(capacity), size_(0) {
    entries_ = new T[capacity_]();
  }

  size_t size() const {
    return this->size_;
  }

  void push_back(const T &value) {
    entries_[size_++] = value;
    if (size_ == capacity_) {
      reserve(capacity_ * 2);
    }
  }

  T pop_back() {
    return entries_[--size_];
  }

  T get(size_t index) const {
    return entries_[index];
  }

  T &operator[](size_t index) {
    return entries_[index];
  }

  const T &operator[](size_t index) const {
    return entries_[index];
  }

  T &back() {
    return entries_[size_ - 1];
  }

  const T &back() const {
    return entries_[size_ - 1];
  }

  void reset() {
    this->size_ = 0;
  }

  void remove(size_t index) {
    if (index < size_) {
      entries_[index] = entries_[size_ - 1]; 
      --size_;
    }
  }

  void resize(size_t size) {
    size_ = size;
    if (size_ >= capacity_) {
      reserve(std::max(capacity_ * 2, size_));
    }
  }

  void fill(T &value) {
    std::fill(entries_, entries_ + size_, value);
  }

  void fill(T value) {
    std::fill(entries_, entries_ + size_, value);
  }

  void reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
      T *new_entries = new T[new_capacity]();
      std::copy(entries_, entries_ + capacity_, new_entries);
      delete [] entries_;
      entries_ = new_entries;
      capacity_ = new_capacity;
    }
  }

  ~Path() {
    delete [] entries_;
  }

 private:
  T *entries_;
  size_t size_;
  size_t capacity_;
};

}  // namespace tiny_sat

#endif
