//------------------------------------------------------------------------------
// <copyright file="ColorBasics.h" company="Portland State University">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "Field.h"
#include <stdint.h>
#include "resource.h"
#include "NuiApi.h"

struct ColorRGB
{
    ColorRGB
    (
        int alphaparam,
        int redparam,
        int greenparam,
        int blueparam
    ) :
        alpha(alphaparam),
        red(redparam),
        green(greenparam),
        blue(blueparam)
    {};
    ColorRGB() :
        alpha(0),
        red(0),
        green(0),
        blue(0)
    {};
    int alpha;
    int red;
    int green;
    int blue;
};

class ColorField: public KinectField
{
    static const NUI_IMAGE_RESOLUTION   cColorResolution = NUI_IMAGE_RESOLUTION_640x480;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    ColorField(INuiSensor * pNuiSensor, bool nearMode);

    /// <summary>
    /// Destructor
    /// </summary>
    virtual ~ColorField();

    /// <summary>
    /// Refreshes the stored color field
    /// </summary>
    virtual void RefreshField();

    /// <summary>
    /// returns the detected color at cartesian coordinates
    /// </summary>
    ColorRGB GetColorAt(const Point& p);

    /// <summary>
    /// centers on color at Point p
    /// </summary>
    Point GetCenterOfColorAt(const Point& p);

    /// <summary>
    /// Finds coordinate for color given
    /// </summary>
    Point FindColor(const ColorRGB& rgb);

    const BYTE *GetColorArray() const { return reinterpret_cast<BYTE*>(m_colorField); };


private:
    // stored color field
    ColorRGB                *m_colorField;


    /// <summary>
    /// Handle new color data
    /// </summary>
    void                    ProcessColor();
};

