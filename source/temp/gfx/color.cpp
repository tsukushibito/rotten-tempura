#include "temp/gfx/color.h"
#include "temp/base/clamp.h"

namespace temp {
namespace gfx {

const Color Color::kWhite = Color{1.0f, 1.0f, 1.0f, 1.0f};
const Color Color::kBlack = Color{0.0f, 0.0f, 0.0f, 1.0f};
const Color Color::kRed = Color{1.0f, 0.0f, 0.0f, 1.0f};
const Color Color::kGreen = Color{0.0f, 1.0f, 0.0f, 1.0f};
const Color Color::kBlue = Color{0.0f, 0.0f, 1.0f, 1.0f};
const Color Color::kYellow = Color{1.0f, 1.0f, 0.0f, 1.0f};
const Color Color::kCyan = Color{0.0f, 1.0f, 1.0f, 1.0f};
const Color Color::kMagenta = Color{1.0f, 0.0f, 1.0f, 1.0f};

const Color32Bit Color32Bit::kWhite = Color32Bit{0xff, 0xff, 0xff, 0xff};
const Color32Bit Color32Bit::kBlack = Color32Bit{0, 0, 0, 0xff};
const Color32Bit Color32Bit::kRed = Color32Bit{0xff, 0, 0, 0xff};
const Color32Bit Color32Bit::kGreen = Color32Bit{0, 0xff, 0, 0xff};
const Color32Bit Color32Bit::kBlue = Color32Bit{0, 0, 0xff, 0xff};
const Color32Bit Color32Bit::kYellow = Color32Bit{0xff, 0xff, 0, 0xff};
const Color32Bit Color32Bit::kCyan = Color32Bit{0, 0xff, 0xff, 0xff};
const Color32Bit Color32Bit::kMagenta = Color32Bit{0xff, 0, 0xff, 0xff};

Color Color32BitToColor(const Color32Bit& color32) {
  return Color{(float)color32.red / 255.0f, (float)color32.green / 255.0f,
               (float)color32.blue / 255.0f, (float)color32.alpha / 255.0f};
}

Color32Bit ColorToColor32Bit(const Color& color) {
  return Color32Bit{
      static_cast<std::uint8_t>(clamp(color.red, 0.0f, 1.0f) * 0xff),
      static_cast<std::uint8_t>(clamp(color.green, 0.0f, 1.0f) * 0xff),
      static_cast<std::uint8_t>(clamp(color.blue, 0.0f, 1.0f) * 0xff),
      static_cast<std::uint8_t>(clamp(color.alpha, 0.0f, 1.0f) * 0xff)};
}
}  // namespace base
}  // namespace temp
