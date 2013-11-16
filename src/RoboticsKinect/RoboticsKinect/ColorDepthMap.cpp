//------------------------------------------------------------------------------
// <copyright file="DepthWithColor-D3D.cpp" company="Portland State University">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "ColorDepthMap.h"


/// <summary>
/// Constructor
/// </summary>
ColorDepthMap::ColorDepthMap(INuiSensor *pNuiSensor, bool nearMode) :
    KinectField(pNuiSensor, nearMode),
    m_colorField(ColorField(pNuiSensor, nearMode)),
    m_depthField(DepthField(pNuiSensor, nearMode))
{
}

/// <summary>
/// Destructor
/// </summary>
ColorDepthMap::~ColorDepthMap()
{}

void
ColorDepthMap::SetNearMode(bool nearMode)
{
    (this->*&KinectField::SetNearMode)(nearMode);
    m_depthField.SetNearMode(nearMode);
    m_colorField.SetNearMode(nearMode);
}


/// <summary>
/// Refresh the depth field and the color field
/// </summary>
void
ColorDepthMap::RefreshField()
{
    m_depthField.RefreshField();
    m_colorField.RefreshField();
}

HANDLE
ColorDepthMap::GetNextColorFrameEvent()
{
    return m_colorField.GetNextFrameEvent();
}

HANDLE
ColorDepthMap::GetColorStreamHandle()
{
    return m_colorField.GetStreamHandle();
}

HANDLE
ColorDepthMap::GetNextDepthFrameEvent()
{
    return m_depthField.GetNextFrameEvent();
}

HANDLE
ColorDepthMap::GetDepthStreamHandle()
{
    return m_depthField.GetStreamHandle();
}

