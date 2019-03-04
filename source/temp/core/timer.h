#pragma once

#include <chrono>

namespace temp {
namespace core {

class Timer {
 public:
  Timer() : begin_(std::chrono::system_clock::now()) {}

  float durationMs() const {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now() - begin_).count();
  }

  float durationUs() const {
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now() - begin_).count();
  }

  float durationNs() const {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(system_clock::now() - begin_).count();
  }

 private:
  std::chrono::system_clock::time_point begin_;
};

}  // namespace core
}  // namespace temp