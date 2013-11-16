//------------------------------------------------------------------------------
// <copyright file="Field.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include "resource.h"
#include "NuiApi.h"
#include <stdint.h>

struct Point
{
    uint32_t x;
    uint32_t y;
};


static const int cFieldWidth  = 640;
static const int cFieldHeight = 480;
static const int cBytesPerPixel = 4;


class KinectField
{

public:
    /// <summary>
    /// Constructor
    /// </summary>
	KinectField(INuiSensor * pNuiSensor, bool nearMode) :
		m_pNuiSensor(pNuiSensor),
        m_streamHandle(INVALID_HANDLE_VALUE),
        m_nextFrameEvent(INVALID_HANDLE_VALUE),
        m_nearMode(nearMode)
	{};

    /// <summary>
    /// Destructor
    /// </summary>
	virtual ~KinectField()
    {
        if (m_nextFrameEvent != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_nextFrameEvent);
            m_nextFrameEvent = INVALID_HANDLE_VALUE;
        }
    }

	uint32_t GetFieldWidth() const { return cFieldWidth; };
	uint32_t GetFieldHeight() const { return cFieldHeight; };
    uint32_t GetBytesPerPixel() const { return cBytesPerPixel; };
	uint32_t GetFieldArea() const { return GetFieldWidth() * GetFieldHeight(); };
    virtual void SetNearMode(bool newNearMode) { m_nearMode = newNearMode; };
    bool IsNearMode() const { return m_nearMode; };
    HANDLE GetNextFrameEvent() { return m_nextFrameEvent; };
    HANDLE GetStreamHandle() { return m_streamHandle; };

    /// <summary>
    /// Refreshes the stored depth field
    /// </summary>
    virtual void RefreshField() = 0;

protected:
    // Current Kinect
    INuiSensor *m_pNuiSensor;

    HANDLE m_streamHandle;
    HANDLE m_nextFrameEvent;

private:

    bool m_nearMode;
};

