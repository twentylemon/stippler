#include "FinalizeParams.h"


FinalizeParams::FinalizeParams() :
        radiusFunc(RADIUS_FUNC_LINEAR),
    radiusScale(2.0f),
    radiusMode(RADIUS_MODE_SCALAR),
    colorMode(false)
{
}


FinalizeParams::FinalizeParams(int radiusFunc, float radiusScale, int radiusMode, bool colorMode) :
    radiusFunc(radiusFunc),
    radiusScale(radiusScale),
    radiusMode(radiusMode),
    colorMode(colorMode)
{
}

