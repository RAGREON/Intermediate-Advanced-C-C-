#include <cstddef>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define LIB_TEST

#include <doctest.h>
#include <fi_vec.hpp>


TEST_CASE("vector constructor test") {
  fi::vec<int> vec_1;

  REQUIRE(vec_1.size() == 0);
  REQUIRE(vec_1.capacity() == 0);
  CHECK(vec_1.getContainer() == nullptr);

  fi::vec<int> vec_2(10);

  REQUIRE(vec_2.size() == 0);
  REQUIRE(vec_2.capacity() == 10);
  CHECK(vec_2.getContainer() != nullptr);

  fi::vec<int> vec_3(4, 1);

  REQUIRE(vec_3.size() == 4);
  REQUIRE(vec_3.capacity() == 4);
  CHECK(vec_3.getContainer() != nullptr);

  SUBCASE("all elements are same") {
    for (int i = 0; i < 4; i++) {
      CHECK(vec_3[i] == 1);
    }
  }

}

TEST_CASE("push_back test") {
  size_t n = 3;

  fi::vec<int> vec(n);

  vec.push_back(10);

  REQUIRE(vec.size() == 1);
  REQUIRE(vec.capacity() == n);

  SUBCASE("pushing after count > capacity will resize vector") {
    vec.push_back(10);
    vec.push_back(10);

    CHECK(vec.size() == 3);
    CHECK(vec.capacity() >= static_cast<size_t>(n * 1.5));
  }
}
