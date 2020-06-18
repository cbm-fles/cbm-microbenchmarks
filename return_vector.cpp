
#include "Stopwatch.h"

// 1 MB

int main() {
  Stopwatch stopwatch;
  int j= 0;
  for (int i = 0; i < 100000000; ++i) j+=1;
  stopwatch.Print("m");

  return j & 1;
}
