#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include "temp/gfx/tvk/tvk_context.h"
#include "temp/gfx/tvk/tvk_swap_chain.h"

namespace temp {
namespace gfx {
namespace tvk {

TvkSwapChain::TvkSwapChain(std::shared_ptr<Context>& context)
    : context_(context) {}

void TvkSwapChain::Present() const {}
}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif