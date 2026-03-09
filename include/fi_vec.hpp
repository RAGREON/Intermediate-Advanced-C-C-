#pragma once

#include <initializer_list>
#include <iostream>
#include <utility>

namespace fi {
  template <class T>
  class vec {
  public:
    vec() : count(0), containerCapacity(0), container(nullptr) {}

    vec(size_t n) : count(0), containerCapacity(n) {
      container = new T[n];
    }

    vec(size_t n, T value) : containerCapacity(n) {
      container = new T[n];

      for (size_t i = 0; i < n; i++) 
        container[i] = value;

      count = n;
    }

    vec(const std::initializer_list<T>& elements) {
      count = elements.size();
      containerCapacity = (containerCapacity == 0) ? 1 : static_cast<size_t>(count * 1.5);
      container = new T[containerCapacity];

      const T* ptr = elements.begin();

      for (size_t i = 0; i < elements.size(); i++)
        container[i] = ptr[i];
    }

    vec(const vec& other) : count(other.count), containerCapacity(other.containerCapacity) {
      container = new T[containerCapacity];
      for (size_t i = 0; i < count; i++)
        container[i] = other.container[i];
    }

    void push_back(T element) {
      if ((count + 1) >= containerCapacity) {
        containerCapacity = static_cast<size_t>(containerCapacity * 1.5);
        T *newContainer = new T[containerCapacity];

        for (size_t i = 0; i < count; i++) {
          newContainer[i] = container[i];
        }

        delete[] container;
        container = newContainer;
      }

      container[count++] = element;
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
    delete[] container;
  }

  private:
    T *container;
    
    size_t count;
    size_t containerCapacity;
  };
}