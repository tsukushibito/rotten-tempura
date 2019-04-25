#pragma once

#include <cstdint>

#include <chrono>

namespace temp {

class Timer {
 public:
  Timer() : begin_(std::chrono::system_clock::now()) {}

  std::int64_t durationMs() const {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now() - begin_).count();
  }

  std::int64_t durationUs() const {
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now() - begin_).count();
  }

  std::int64_t durationNs() const {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(system_clock::now() - begin_).count();
  }

 private:
  std::chrono::system_clock::time_point begin_;
};

}  // namespace temp