#ifndef TINY_SAT_BUFFER_H
#define TINY_SAT_BUFFER_H

namespace tiny_sat {

template <typename T>
class Buffer {
 public:
  Buffer(size_t capacity = 1) : capacity_(capacity), size_(0) {
    entries_ = new T[capacity_]();
  }

  size_t size() const {
    return this->size_;
  }

  T get(size_t index) const {
    return entries_[index];
  }

  void add(T &value) {
    entries_[size_++] = value;
    if (size_ == capacity_) {
      reserve(capacity_ * 2);
    }
  }

  void fill(size_t index, T &value) {
    if (index < size_) {
      entries_[index] = value;
    }
  }

  void remove(size_t index) {
    if (index < size_) {
      entries_[index] = entries_[size_ - 1]; 
      --size_;
    }
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

  ~Buffer() {
    delete [] entries_;
  }

 private:
  T *entries_;
  size_t size_;
  size_t capacity_;
};

}  // namespace tiny_sat

#endif
