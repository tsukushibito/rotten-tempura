#include "temp/base/define.h"
#ifdef TEMP_GFX_API_D3D11

#include "temp/gfx/d3d11/d3d11_swap_chain.h"

namespace temp {
namespace gfx {
namespace d3d11 {
D3d11SwapChain::D3d11SwapChain(const D3d11Device& device, const void* window,
                               std::uint32_t width, std::uint32_t height) {}

D3d11SwapChain::D3d11SwapChain(const ComPtr<IDXGISwapChain>& swap_chain) {
  swap_chain_ = swap_chain;
  DXGI_SWAP_CHAIN_DESC desc;
  swap_chain_->GetDesc(&desc);
  width_ = desc.BufferDesc.Width;
  height_ = desc.BufferDesc.Height;
}

void D3d11SwapChain::Present(const Device* device) {}

void D3d11SwapChain::Resize(const Device* device, std::uint32_t width,
                            std::uint32_t height) {}

std::uint32_t D3d11SwapChain::width() const { return width_; }

std::uint32_t D3d11SwapChain::height() const { return height_; }

}  // namespace d3d11
}  // namespace gfx
}  // namespace temp

#endif