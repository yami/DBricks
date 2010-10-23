#ifndef TEXTSTRING_HXX
#define TEXTSTRING_HXX

#include <string>
#include <list>

#include "Font.hxx"

namespace DBricks {

enum Alignment {
    I_Do_Not_Know_Yet
};

struct TextAttributes {
    int       start;
    int       end;
    FontDesc  font;
    Color     outline_color;
    Color     fill_color;
    Alignment alignment;
};

struct TextString {
    std::string               text;
    std::list<TextAttributes> attrs;
};


} // namespace DBricks


#endif // TEXTSTRING_HXX
