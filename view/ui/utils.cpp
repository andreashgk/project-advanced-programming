#include "utils.h"

float scale(unsigned int windowHeight, float scale) {
    float size = (static_cast<float>(windowHeight) / 16.5f) * scale;
    return size;
}
