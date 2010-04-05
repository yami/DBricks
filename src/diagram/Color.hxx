#ifndef COLOR_HXX
#define COLOR_HXX

#define RGB_P(C) C.r, C.g, C.b

namespace DBricks {

struct Color {
    Color(double red, double green, double blue)
        :r(red), g(green), b(blue)
    {
    }

    Color(const Color& other)
        :r(other.r), g(other.g), b(other.b)
    {
    }

    Color()
    {
    }

    double r, g, b;
};

inline
Color make_rgb24(int red, int green, int blue)
{
    return Color((double)red/255, (double)green/255, (double)blue/255);
}

inline
Color make_rgb24(int hex)
{
    int red   = (hex >> 16) & 0xFF;
    int green = (hex >> 8)  & 0xFF;
    int blue  = hex & 0xFF;

    return Color((double)red/255, (double)green/255, (double)blue/255);
}

extern const Color Black;
extern const Color Blue;
extern const Color Green;
extern const Color Magenta;
extern const Color Red;
extern const Color White;
extern const Color Yello;

} // namespace DBricks


#endif // COLOR_HXX
