
#include "Stopwatch.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

// constexpr size_t elements = 1'000'000;
constexpr size_t elements = 200'000;
constexpr size_t components = 1000;

using TimesliceComponent = std::vector<uint64_t>;
using Timeslice = std::array<TimesliceComponent, components>;
using Digis = std::vector<uint64_t>;

Timeslice create_timeslice() {
  Timeslice ts;
  for (auto &c : ts) {
    c.reserve(elements);
    for (size_t i = 0; i < elements; ++i) {
      c.emplace_back(i);
    }
  }
  return ts;
}

Digis reader(const TimesliceComponent &c) {
  Digis d;
  d.reserve(c.size());
  for (size_t i = 0; i < c.size(); ++i) {
    d.emplace_back(c[i] + 1);
  }
  return Digis(d);
}

void reader(const TimesliceComponent &c, Digis *d) {
  for (size_t i = 0; i < c.size(); ++i) {
    d->emplace_back(c[i] + 1);
  }
}

uint64_t analysis(const Digis &digis) {
  return std::accumulate(digis.cbegin(), digis.cend(), 0);
}

void run(int mode) {
  std::cout << "- receive (invent) a new timeslice\n";
  Stopwatch stopwatch;
  Timeslice ts = create_timeslice();
  stopwatch.Print("m");

  std::cout << "- reserve global digi vector\n";
  stopwatch.Start();
  Digis d;
  d.reserve(components * elements);
  stopwatch.Print("m");

  if (mode == 0) {
    std::cout << "- run the reader (create vector, then append)\n";
    Stopwatch reader_only;
    Stopwatch vector_append;
    stopwatch.Start();
    for (auto &c : ts) {
      // run the reader on every timeslice component
      reader_only.Start(false);
      Digis d_temp = reader(c);
      reader_only.Stop();
      // add the digis to a global vector
      vector_append.Start(false);
      d.insert(d.end(), d_temp.begin(), d_temp.end());
      vector_append.Stop();
    }
    stopwatch.Print("m");
    reader_only.Print("m");
    vector_append.Print("m");
  } else if (mode == 1) {
    std::cout << "- run the reader (append directly)\n";
    stopwatch.Start();
    for (auto &c : ts) {
      // run the reader on every timeslice component
      reader(c, &d);
    }
    stopwatch.Print("m");
  }

  std::cout << "- sort the digis\n";
  stopwatch.Start();
  std::sort(d.begin(), d.end());
  stopwatch.Print("m");

  std::cout << "- forward the digis to the analysis\n";
  stopwatch.Start();
  auto result = analysis(d);
  stopwatch.Print("m");
  std::cout << "- analysis result: " << result << "\n";
}

int main() {
  std::cout << "total size in bytes: "
            << (elements * components * sizeof(uint64_t)) << "\n";

  for (int mode = 0; mode < 2; ++mode) {
    std::cout << "=== RUN MODE " << mode << " ===\n";
    run(mode);
  }

  return 0;
}
