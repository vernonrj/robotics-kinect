//------------------------------------------------------------------------------
// <copyright file="DepthBasics.h" company="Portland State University">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"
#include <strsafe.h>
#include "Field.h"
#include <stdint.h>

typedef uint8_t Depth;

class DepthField: public KinectField
{
    static const NUI_IMAGE_RESOLUTION   cDepthResolution = NUI_IMAGE_RESOLUTION_640x480;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    DepthField(INuiSensor * pNuiSensor, bool nearMode);

    /// <summary>
    /// Destructor
    /// </summary>
    virtual ~DepthField();

    /// <summary>
    /// Refreshes the stored depth field
    /// </summary>
    virtual void RefreshField();

    /// <summary>
    /// returns the depth at cartesion coordinates x, y
    /// </summary>
    Depth GetDepthAt(const Point& p);

    Depth GetDepthAt(uint32_t x, uint32_t y);

private:
    // stored depth field
    Depth *m_depthField;

};
