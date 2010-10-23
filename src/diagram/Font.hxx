#ifndef FONT_HXX
#define FONT_HXX

#include <stdint.h>

namespace DBricks {

// I do not understand the details of font yet.
// The following code reassembles Pango's PangoFontDescription

enum FontStyle {
    FontStyle_Normal,                  // the font is upright
    FontStyle_Oblique,                 // the font is slanted in a roman style
    FontStyle_Italic,                  // the font is slanted in an italic style
};

enum FontVariant {
    FontVariant_Normal,                  // a normal font
    FontVariant_SmallCaps,               // a font with the lower case replaced by a smaller variants of the capital characters
};

static const int FontWeight_Thin        = 100;
static const int FontWeight_Ultra_Light = 200;
static const int FontWeight_Light       = 300;
static const int FontWeight_Book        = 380;
static const int FontWeight_Normal      = 400;
static const int FontWeight_Medium      = 500;
static const int FontWeight_Semi_Bold   = 600;
static const int FontWeight_Bold        = 700;
static const int FontWeight_Ultra_Bold  = 800;
static const int FontWeight_Heavy       = 900;
static const int FontWeight_Ultra_Heavy = 1000;

enum FontStretch {
    FontStretch_Ultra_Condensed,
    FontStretch_Extra_Condensed,
    FontStretch_Condensed,
    FontStretch_Semi_Condensed,
    FontStretch_Normal,
    FontStretch_Semi_Expanded,
    FontStretch_Expanded,
    FontStretch_Extra_Expanded,
    FontStretch_Ultra_Expanded,
};

static const int FontMask_Family  = 1 << 0;
static const int FontMask_Style   = 1 << 1;
static const int FontMask_Variant = 1 << 2;
static const int FontMask_Weight  = 1 << 3;
static const int FontMask_Stretch = 1 << 4;
static const int FontMask_Size    = 1 << 5;

struct FontDesc {
    char*       family;
    FontStyle   style;
    FontVariant variant;
    int         weight;         // [100, 1000]
    FontStretch stretch;
    int         size;

    uint16_t    mask;
};

} // namespace DBricks


#endif // FONT_HXX
