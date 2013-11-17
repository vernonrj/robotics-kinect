//------------------------------------------------------------------------------
// <copyright file="DepthWithColor-D3D.h" company="Portland State University">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"
#include <strsafe.h>
#include "Field.h"
#include "Depth.h"
#include "Color.h"


class ColorDepthMap: public KinectField
{
public:
    /// <summary>
    /// Constructor
    /// </summary>
    ColorDepthMap(INuiSensor *pNuiSensor, bool nearMode);

    /// <summary>
    /// Denstructor
    /// </summary>
    virtual ~ColorDepthMap();

    /// <summary>
    /// Refreshes the stored depth field
    /// </summary>
    virtual void RefreshField();

    virtual void SetNearMode(bool nearMode);

    HANDLE GetNextColorFrameEvent();
    HANDLE GetColorStreamHandle();

    HANDLE GetNextDepthFrameEvent();
    HANDLE GetDepthStreamHandle();

    const ColorField& GetColorFieldRef() const { return m_colorField; };
    const DepthField& GetDepthFieldRef() const { return m_depthField; };

private:
    // Color field
    ColorField m_colorField;

    // Depth field
    DepthField m_depthField;
};

