#pragma once
#include "temp/base/define.h"
#ifdef TEMP_GFX_API_D3D11
#include <d3d11.h>
#include <wrl/client.h>

namespace temp {
namespace gfx {
namespace d3d11 {
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
}
}  // namespace gfx
}  // namespace temp
#endif