#pragma once
#include <cstdint>
#include <memory>

#include "temp/base/define.h"
#ifdef TEMP_PLATFORM_MAC
#include "temp/app/application_base.h"

namespace temp {
namespace app {
namespace mac {

class MacApplication : public ApplicationBase<MacApplication> {
 public:
  MacApplication();
  ~MacApplication();

 public:
  std::int32_t Run();
  void Exit();

 private:
  void MainLoop();

 private:
  struct Properties;
  std::unique_ptr<Properties> properties_;
};
}  // namespace mac
using Application = mac::MacApplication;

}  // namespace app
}  // namespace temp
#endif
