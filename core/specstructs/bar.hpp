#ifndef BAR_HPP
#define BAR_HPP

#include <cstdint>
#include "./fontstruct.hpp"

typedef struct{
    Window win;
    bool hidden;
    FontStruct font;
    int32_t DESKTOP_LABEL_x;
    bool init;
} Bar;

#endif