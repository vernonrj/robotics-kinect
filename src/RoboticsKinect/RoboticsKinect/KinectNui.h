//------------------------------------------------------------------------------
// <copyright file="KinectNui.h" company="PSU">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include "resource.h"
#include "NuiApi.h"
#include "ImageRenderer.h"
#include "ColorDepthMap.h"

/// <summary>
/// Initializes a NUI sensor pointer and returns it
/// </summary>
INuiSensor *GetNuiPointer();

/// <summary>
/// releases a NUI sensor pointer
/// </summary>
void ReleaseNui(INuiSensor *nuiSensor);




class Direct2DWindow
{
    static const int        cBytesPerPixel = 4;

    static const int        cStatusMessageMaxLen = MAX_PATH*2;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    Direct2DWindow(ColorDepthMap *cdmap);

    /// <summary>
    /// Destructor
    /// </summary>
    ~Direct2DWindow();

    /// <summary>
    /// Handles window messages, passes most to the class instance to handle
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Handle windows messages for a class instance
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    LRESULT CALLBACK        DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Creates the main window and begins processing
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="nCmdShow"></param>
    int                     Run(HINSTANCE hInstance, int nCmdShow);

private:
    ColorDepthMap           *m_cdmap;

    HWND                    m_hWnd;

    // Direct2D
    ImageRenderer*          m_pDrawDepth;
    ID2D1Factory*           m_pD2DFactory;
    
    /// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();

    /// <summary>
    /// Set the status bar message
    /// </summary>
    /// <param name="szMessage">message to display</param>
    void                    SetStatusMessage(WCHAR* szMessage);
};

