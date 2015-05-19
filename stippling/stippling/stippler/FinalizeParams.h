

#pragma once
#include <functional>
#include "Stipple.h"

class FinalizeParams {
public:
    FinalizeParams();
    FinalizeParams(int radiusFunc, float radiusScale, int radiusMode, bool colorMode = false);

    static const int RADIUS_FUNC_LINEAR = 0x01;
    static const int RADIUS_FUNC_TRIG   = RADIUS_FUNC_LINEAR << 1;
    static const int RADIUS_MODE_SCALAR   = RADIUS_FUNC_TRIG << 1;
    static const int RADIUS_MODE_RELATIVE = RADIUS_MODE_SCALAR << 1;
    static const int RADIUS_MODE_MASS     = RADIUS_MODE_RELATIVE << 1;

    int radiusFunc;
    float radiusScale;
    int radiusMode;
    bool colorMode;
};
