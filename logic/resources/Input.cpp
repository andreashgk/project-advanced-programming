#include "Input.h"

namespace resources {

Input::Input(float xAxis, bool jump) : horizontalAxis(xAxis), jump(jump) {}

float Input::getHorizontalAxis() const { return horizontalAxis; }

bool Input::isJumping() const { return jump; }

#ifndef NDEBUG
bool Input::isDebugMode() const { return debug; }
void Input::setDebugMode(bool debug) { this->debug = debug; }
#endif

} // namespace resources
