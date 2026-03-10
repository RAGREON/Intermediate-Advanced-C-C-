#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <utility>

namespace fi {
  template <class T, class Allocator = std::allocator<T>>
  class vec {
  private:
    T* data_;
    size_t size_;
    size_t capacity_;
    Allocator alloc_;

  public:
    vec() : size_(0), capacity_(0), data_(nullptr) {}

    vec(size_t n) : size_(0), capacity_(n) {
      data_ = alloc_.allocate(n);
    }

    vec(size_t n, T value) : size_(n) {
      capacity_ = static_cast<int>(n * 1.5);
      data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));

      for (size_t i = 0; i < n; i++) 
        ::new (data_ + i) T(value);

      size_ = n;
    }

    vec(const std::initializer_list<T>& elements) {
      size_ = elements.size();
      capacity_ = (capacity_ == 0) ? 1 : static_cast<size_t>(size_ * 1.5);
      data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));

      const T* ptr = elements.begin();

      for (size_t i = 0; i < elements.size(); i++)
        ::new (data_ + i) T(ptr[i]);
    }

    vec(const vec& other) : size_(other.size_), capacity_(other.capacity_) {
      data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
      for (size_t i = 0; i < size_; i++)
        ::new (data_ + i) T(other.data_[i]);
    }

    void push_back(T element) {
      if ((size_ + 1) >= capacity_) {
        capacity_ = static_cast<size_t>(capacity_ * 1.5);

        T *newdata_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));

        for (size_t i = 0; i < size_; i++) {
          ::new (newdata_ + i) T(std::move(data_[i]));
        }

        for (size_t i = 0; i < size_; i++) {
          data_[i].~T();
        }
        ::operator delete(data_);

        data_ = newdata_;
      }

      ::new (data_ + size_++) T(std::move(element));
    }

    T& at(const size_t index) {
      if (index >= size_) {
        std::cerr << "invalid index" << std::endl;
        exit(1);
      }
      
      return data_[index];
    }

    T& at(const size_t index) const {
      if (index >= size_) {
        std::cerr << "invalid index" << std::endl;
        exit(1);
      }

      return data_[index];
    }
  
    size_t size() const {
      return size_;
    }

    size_t capacity() const {
      return capacity_;
    }

#ifdef LIB_TEST
    T* getData() {
      return this->data_;
    }
#endif

    vec& operator=(vec other) {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(data_, other.data_);
      return *this; 
    }

    T& operator[](size_t index) const {
      return at(index); 
    }

    T& operator[](size_t index) {
      return at(index); 
    }

  ~vec() {
    for (size_t i = 0; i < size_; i++) {
      data_[i].~T();
    }
    ::operator delete(data_);
  }

  };
}