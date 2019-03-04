#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "temp/core/logger.h"
#include "temp/core/sleep.h"
#include "temp/core/thread_pool.h"
#include "temp/core/timer.h"

using namespace temp::core;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(core)
BOOST_AUTO_TEST_CASE(logger) {
  Logger::setLogLevel(Logger::LogLevel::kInfo);
  TEMP_LOG_TRACE("trace");
  TEMP_LOG_DEBUG("debug");
  TEMP_LOG_INFO("info");
  TEMP_LOG_WARNING("warning");
  TEMP_LOG_ERROR("error");
}

BOOST_AUTO_TEST_CASE(threadpool) {
  {
    Timer timer;
    ThreadPool threadPool(8);
    for (int i = 0; i < 1000; ++i) {
      threadPool.enqueue([]() { sleep(10); });
    }
    threadPool.waitForTasks();
    TEMP_LOG_TRACE("total: ", timer.durationMs(), "ms");
  }

  {
    Timer timer;
    for (int i = 0; i < 1000; ++i) {
      sleep(10);
    }
    TEMP_LOG_TRACE("total: ", timer.durationMs(), "ms");
  }
}
BOOST_AUTO_TEST_SUITE_END()
