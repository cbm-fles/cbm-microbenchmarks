
#include "Stopwatch.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

// Parameters
// constexpr size_t elements = 1000000;
constexpr size_t elements = 200000;
constexpr size_t vectors = 1000;

using vec_t = std::vector<uint64_t>;
using meta_t = std::array<vec_t, vectors>;

volatile uint64_t result;

/******************************************/

vec_t return_value() {
  vec_t v;
  v.reserve(elements);
  for (size_t i = 0; i < elements; ++i) {
    v.emplace_back(i);
  }
  return v;
}

void TEST_return_value() {
  Stopwatch stopwatch;
  meta_t m;
  for (size_t i = 0; i < vectors; ++i) {
    m[i] = return_value();
  }
  std::cout << "return_value:\n";
  stopwatch.Print("m");
  result = m[vectors - 1][elements - 1];
}

/******************************************/

vec_t return_copy() {
  vec_t v;
  v.reserve(elements);
  for (size_t i = 0; i < elements; ++i) {
    v.emplace_back(i);
  }
  vec_t new_v(v); // try to force a vector copy
  result = v[elements - 1];
  return new_v;
}

void TEST_return_copy() {
  Stopwatch stopwatch;
  meta_t m;
  for (size_t i = 0; i < vectors; ++i) {
    m[i] = return_copy();
  }
  std::cout << "return_copy:\n";
  stopwatch.Print("m");
  result = m[vectors - 1][elements - 1];
}

/******************************************/

void fill_reference(vec_t *v) {
  v->reserve(elements);
  for (size_t i = 0; i < elements; ++i) {
    v->emplace_back(i);
  }
}

void TEST_fill_reference() {
  Stopwatch stopwatch;
  meta_t m;
  for (size_t i = 0; i < vectors; ++i) {
    fill_reference(&m[i]);
  }
  std::cout << "fill_reference:\n";
  stopwatch.Print("m");
  result = m[vectors - 1][elements - 1];
}

/******************************************/

void append_to_reference(vec_t *v) {
  for (size_t i = 0; i < elements; ++i) {
    v->emplace_back(i);
  }
}

void TEST_append_to_reference() {
  Stopwatch stopwatch;
  vec_t v;
  v.reserve(elements * vectors);
  for (size_t i = 0; i < vectors; ++i) {
    append_to_reference(&v);
  }
  std::cout << "append_to_reference:\n";
  stopwatch.Print("m");
  result = v[vectors * elements - 1];
}

/******************************************/

void TEST_append_to_reference_noreserve() {
  Stopwatch stopwatch;
  vec_t v;
  // v.reserve(elements * vectors);
  for (size_t i = 0; i < vectors; ++i) {
    append_to_reference(&v);
  }
  std::cout << "append_to_reference_noreserve:\n";
  stopwatch.Print("m");
  result = v[vectors * elements - 1];
}

/******************************************/

vec_t forget_reserve() {
  vec_t v;
  // v.reserve(elements);
  for (size_t i = 0; i < elements; ++i) {
    v.emplace_back(i);
  }
  return v;
}

void TEST_forget_reserve() {
  Stopwatch stopwatch;
  meta_t m;
  for (size_t i = 0; i < vectors; ++i) {
    m[i] = forget_reserve();
  }
  std::cout << "forget_reserve:\n";
  stopwatch.Print("m");
  result = m[vectors - 1][elements - 1];
}

/******************************************/

int main() {
  std::cout << "total size in bytes: "
            << (elements * vectors * sizeof(uint64_t)) << "\n";

  TEST_return_value();
  TEST_return_copy();
  TEST_fill_reference();
  TEST_append_to_reference();
  TEST_forget_reserve();
  TEST_append_to_reference_noreserve();

  return 0;
}
