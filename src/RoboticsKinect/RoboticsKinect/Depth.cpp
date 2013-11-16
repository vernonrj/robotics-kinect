//------------------------------------------------------------------------------
// <copyright file="DepthBasics.cpp" company="Portland State University">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "Depth.h"
#include <iostream>
#include "resource.h"

static int GetMinNuiDepth(BOOL nearMode);
static int GetMaxNuiDepth(BOOL nearMode);
static Depth WrapDepth(USHORT depth, BOOL nearMode);

/// <summary>
/// Constructor
/// </summary>
DepthField::DepthField(INuiSensor * pNuiSensor, bool nearMode) :
	KinectField(pNuiSensor, nearMode),
    m_depthField(NULL)
{
    // create heap storage for depth pixel data in RGBX format
    m_depthField = new Depth[GetFieldArea()];
    if (NULL == m_depthField)
    {
        exit(EXIT_FAILURE);
    }

    // Create an event that will be signaled when depth data is available
    m_nextFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Open a depth image stream to receive depth frames
    HRESULT hr = m_pNuiSensor->NuiImageStreamOpen(
                 NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
                 cDepthResolution,
                 0,
                 2,
                 m_nextFrameEvent,
                 &m_streamHandle);
    if (FAILED(hr))
    {
        throw "failed to open depth image stream";
    }
}

/// <summary>
/// Destructor
/// </summary>
DepthField::~DepthField()
{
    if (m_depthField)
    {
        delete [] m_depthField;
        m_depthField = NULL;
    }
}



/// <summary>
/// Handle new depth data
/// </summary>
void
DepthField::RefreshField()
{
    HRESULT hr;
    NUI_IMAGE_FRAME imageFrame;
    BOOL nearMode = IsNearMode();

    if (NULL == m_pNuiSensor)
    {
        throw "Cannot refresh depth field, no Nui sensor available";
    }
    if ( WAIT_OBJECT_0 != WaitForSingleObject(m_nextFrameEvent, 0) )
    {
        return;
    }

    // Attempt to get the depth frame
    hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_streamHandle, 0, &imageFrame);
    if (FAILED(hr))
    {
        return;
    }

    INuiFrameTexture* pTexture;

    // Get the depth image pixel texture
    hr = m_pNuiSensor->NuiImageFrameGetDepthImagePixelFrameTexture(
        m_streamHandle, &imageFrame, &nearMode, &pTexture);
    if (FAILED(hr))
    {
        goto ReleaseFrame;
    }

    NUI_LOCKED_RECT LockedRect;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    pTexture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {
        // Get the min and max reliable depth for the current frame
        const int minDepth = GetMinNuiDepth(nearMode);
        const int maxDepth = GetMaxNuiDepth(nearMode);

        Depth * newdepth = m_depthField;
        const NUI_DEPTH_IMAGE_PIXEL * pBufferRun =
				reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL*>(LockedRect.pBits);

        // end pixel is start + width*height - 1
		const NUI_DEPTH_IMAGE_PIXEL * pBufferEnd = pBufferRun + GetFieldArea();

        while ( pBufferRun < pBufferEnd )
        {
            // discard the portion of the depth that contains only the player index
            USHORT depth = pBufferRun->depth;

            // To convert to a byte, we're discarding the most-significant
            // rather than least-significant bits.
            // We're preserving detail, although the intensity will "wrap."
            // Values outside the reliable depth range are mapped to 0 (black).

            // Note: Using conditionals in this loop could degrade performance.
            // Consider using a lookup table instead when writing production code.
			const Depth intensity = WrapDepth(depth, nearMode);

            *(newdepth++) = intensity;

            // Increment our index into the Kinect's depth buffer
            ++pBufferRun;
        }
    }

    // We're done with the texture so unlock it
    pTexture->UnlockRect(0);

    pTexture->Release();

ReleaseFrame:
    // Release the frame
    m_pNuiSensor->NuiImageStreamReleaseFrame(m_streamHandle, &imageFrame);
}



Depth
DepthField::GetDepthAt(const Point& p)
{
    return GetDepthAt(p.x, p.y);
}


Depth
DepthField::GetDepthAt(uint32_t x, uint32_t y)
{
    uint32_t depth = y * GetFieldWidth();
    uint32_t index = x + depth;
    if (index < 0 || index > (GetFieldArea()))
    {
        throw "index out of range";
    }
    else return m_depthField[index];
}


static int
GetMinNuiDepth(BOOL nearMode)
{
    int minDepth = nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM;
    return minDepth >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
}


static int
GetMaxNuiDepth(BOOL nearMode)
{
    int maxDepth = nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM;
    return maxDepth >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
}


static Depth
WrapDepth(USHORT depth, BOOL nearMode)
{
	int minDepth = GetMinNuiDepth(nearMode);
	int maxDepth = GetMaxNuiDepth(nearMode);
	if (depth < minDepth || depth > maxDepth)
		return 0;
	else
	{
		return static_cast<Depth>(depth % 256);
	}
}
