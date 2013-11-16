//------------------------------------------------------------------------------
// <copyright file="ColorBasics.cpp" company="Portland State University">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "Color.h"
#include <iostream>
#include "resource.h"

/// <summary>
/// Constructor
/// </summary>
ColorField::ColorField(INuiSensor * pNuiSensor, bool nearMode) :
    KinectField(pNuiSensor, nearMode),
    m_colorField(NULL)
{
    m_colorField = new ColorRGB[GetFieldArea()*GetBytesPerPixel()];
    if (NULL == m_colorField)
    {
        throw "ColorField:: Failed to allocate m_colorField";
    }
    // Create an event that will be signaled when color data is available
    m_nextFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Open a color image stream to receive color frames
    HRESULT hr = m_pNuiSensor->NuiImageStreamOpen(
                 NUI_IMAGE_TYPE_COLOR,
                 cColorResolution,
                 0,
                 2,
                 m_nextFrameEvent,
                 &m_streamHandle );
    if (FAILED(hr))
    {
        throw "failed to open color image";
    }
}



/// <summary>
/// Destructor
/// </summary>
ColorField::~ColorField()
{
	if (m_colorField)
	{
		delete [] m_colorField;
		m_colorField = NULL;
	}
}



/// <summary>
/// Handle new color data
/// </summary>
void
ColorField::RefreshField()
{
    HRESULT hr;
    NUI_IMAGE_FRAME imageFrame;

    if (NULL == m_pNuiSensor)
    {
        throw "Cannot refresh color field, no Nui sensor available";
    }

    if ( WAIT_OBJECT_0 != WaitForSingleObject(m_nextFrameEvent, 0) )
    {
		return;
    }
    // Attempt to get the color frame
    hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_streamHandle, 0, &imageFrame);
    if (FAILED(hr))
    {
        return;
    }

    INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
    NUI_LOCKED_RECT LockedRect;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    pTexture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {
        ColorRGB *newcolor = m_colorField;
		BYTE *pBufferRun = static_cast<BYTE*>(LockedRect.pBits);
		const INT size = LockedRect.size;
		for (INT i=0; i<size; i+=4)
		{
			newcolor[i].alpha = pBufferRun[i];
            newcolor[i].red = pBufferRun[i+1];
            newcolor[i].green = pBufferRun[i+2];
            newcolor[i].blue = pBufferRun[i+3];
		}
        //m_pDrawColor->Draw(static_cast<BYTE *>(LockedRect.pBits), LockedRect.size);
    }

    // We're done with the texture so unlock it
    pTexture->UnlockRect(0);

    // Release the frame
    m_pNuiSensor->NuiImageStreamReleaseFrame(m_streamHandle, &imageFrame);
}


ColorRGB
ColorField::GetColorAt(const Point& P)
{
    uint32_t index = GetFieldWidth()*P.y + P.x;
    if (index < 0 || index >= GetFieldArea())
    {
        throw "ColorField::GetColorAt: index out of bounds";
    }
    else return ColorRGB(m_colorField[index]);
}

Point
ColorField::GetCenterOfColorAt(const Point& P)
{
    throw "ColorField::GetCenterOfColorAt: not implemented";
}


Point
ColorField::FindColor(const ColorRGB& rgb)
{
    // Find an instance of color and return the point
    throw "ColorField::GetCenterOfColorAt: not implemented";
}


