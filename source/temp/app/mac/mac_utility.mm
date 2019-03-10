#import <Cocoa/Cocoa.h>

#include "temp/app/utility.h"
#include "temp/core/logger.h"

namespace temp {
namespace app {

WindowViewSize GetWindowViewSize(const void* window) {
    NSView* view = (__bridge NSView*)(window);
    auto&& size = view.frame.size;
    std::uint32_t w = size.width;
    std::uint32_t h = size.height;
    return WindowViewSize { w, h };
}

}
}
