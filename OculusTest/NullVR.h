#pragma once

#include "VR.h"

class NullVR: public VR
{
    virtual void recenter() {}
};
