#pragma once

#include <cstdint>

namespace temp {
namespace core {

struct Color {
  float red;
  float green;
  float blue;
  float alpha;

  static const Color kWhite;
  static const Color kBlack;
  static const Color kRed;
  static const Color kGreen;
  static const Color kBlue;
  static const Color kYellow;
  static const Color kCyan;
  static const Color kMagenta;
};

struct Color32Bit {
  std::uint8_t red;
  std::uint8_t green;
  std::uint8_t blue;
  std::uint8_t alpha;

  static const Color32Bit kWhite;
  static const Color32Bit kBlack;
  static const Color32Bit kRed;
  static const Color32Bit kGreen;
  static const Color32Bit kBlue;
  static const Color32Bit kYellow;
  static const Color32Bit kCyan;
  static const Color32Bit kMagenta;
};

Color Color32BitToColor(const Color32Bit& color32);
Color32Bit ColorToColor32Bit(const Color& color);

}
}