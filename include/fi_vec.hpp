#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <utility>

namespace fi {
  template <class T, class Allocator = std::allocator<T>>
  class vec {
  public:
    vec() : count(0), containerCapacity(0), container(nullptr) {}

    vec(size_t n) : count(0), containerCapacity(n) {
      container = static_cast<T*>(::operator new(n * sizeof(T)));
    }

    vec(size_t n, T value) : count(n) {
      containerCapacity = static_cast<int>(n * 1.5);
      container = static_cast<T*>(::operator new(containerCapacity * sizeof(T)));

      for (size_t i = 0; i < n; i++) 
        ::new (container + i) T(value);

      count = n;
    }

    vec(const std::initializer_list<T>& elements) {
      count = elements.size();
      containerCapacity = (containerCapacity == 0) ? 1 : static_cast<size_t>(count * 1.5);
      container = static_cast<T*>(::operator new(containerCapacity * sizeof(T)));

      const T* ptr = elements.begin();

      for (size_t i = 0; i < elements.size(); i++)
        ::new (container + i) T(ptr[i]);
    }

    vec(const vec& other) : count(other.count), containerCapacity(other.containerCapacity) {
      container = static_cast<T*>(::operator new(containerCapacity * sizeof(T)));
      for (size_t i = 0; i < count; i++)
        ::new (container + i) T(other.container[i]);
    }

    void push_back(T element) {
      if ((count + 1) >= containerCapacity) {
        containerCapacity = static_cast<size_t>(containerCapacity * 1.5);

        T *newContainer = static_cast<T*>(::operator new(containerCapacity * sizeof(T)));

        for (size_t i = 0; i < count; i++) {
          ::new (newContainer + i) T(std::move(container[i]));
        }

        for (size_t i = 0; i < count; i++) {
          container[i].~T();
        }
        ::operator delete(container);

        container = newContainer;
      }

      ::new (container + count++) T(std::move(element));
    }

    T& at(const size_t index) {
      if (index >= count) {
        std::cerr << "invalid index" << std::endl;
        exit(1);
      }
      
      return container[index];
    }

    T& at(const size_t index) const {
      if (index >= count) {
        std::cerr << "invalid index" << std::endl;
        exit(1);
      }

      return container[index];
    }
  
    size_t size() const {
      return count;
    }

    size_t capacity() const {
      return containerCapacity;
    }

#ifdef LIB_TEST
    T* getContainer() {
      return this->container;
    }
#endif

    vec& operator=(vec other) {
      std::swap(count, other.count);
      std::swap(containerCapacity, other.containerCapacity);
      std::swap(container, other.container);
      return *this; 
    }

    T& operator[](size_t index) const {
      return at(index); 
    }

    T& operator[](size_t index) {
      return at(index); 
    }

  ~vec() {
    for (size_t i = 0; i < count; i++) {
      container[i].~T();
    }
    ::operator delete(container);
  }

  private:
    T *container;
    
    size_t count;
    size_t containerCapacity;
  };
}