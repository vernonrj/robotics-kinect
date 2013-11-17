//------------------------------------------------------------------------------
// <copyright file="KinectNui.cpp" company="Portland State University">
//     Copyright (c) Portland State University.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "KinectNui.h"
#include "stdafx.h"

INuiSensor*
GetNuiPointer()
{
    int sensorCount = 0;
    INuiSensor *nuiSensor = NULL;
    HRESULT hr = NuiGetSensorCount(&sensorCount);

	if (FAILED(hr))
	{
		throw "Failed to get NUI sensor count";
	}
	

    // Look at each Kinect sensor
    for (int i=0; i<sensorCount; i++)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &nuiSensor);
        if (FAILED(hr)) continue;

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = nuiSensor->NuiStatus();
        if (S_OK == hr) break;

        // This sensor wasn't OK, so release it since we're not using it
        nuiSensor->Release();
    }

    // If we couldn't get an instance, return failure
	if (NULL == nuiSensor)
	{
		throw "Failed to get NUI Sensor instance";
	}

    // Initialize the Kinect and specify that we'll be using depth
    hr = nuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR
            | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX);

    if (FAILED(hr)) return NULL;
    else return nuiSensor;
}

void
ReleaseNui(INuiSensor *nuiSensor)
{
    if (NULL != nuiSensor)
    {
        nuiSensor->NuiShutdown();
        nuiSensor->Release();
    }
    SafeRelease(nuiSensor);
    return;
}




/// <summary>
/// Constructor
/// </summary>
Direct2DWindow::Direct2DWindow(ColorDepthMap *cdmap) :
    m_cdmap(cdmap),
    m_pDrawDepth(NULL),
    m_pD2DFactory(NULL)
{
}

/// <summary>
/// Destructor
/// </summary>
Direct2DWindow::~Direct2DWindow()
{
    // clean up Direct2D renderer
    delete m_pDrawDepth;
    m_pDrawDepth = NULL;

    // clean up Direct2D
    SafeRelease(m_pD2DFactory);

}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int Direct2DWindow::Run(HINSTANCE hInstance, int nCmdShow)
{
    MSG       msg = {0};
    WNDCLASS  wc;

    // Dialog custom window class
    ZeroMemory(&wc, sizeof(wc));
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra    = DLGWINDOWEXTRA;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
    wc.lpfnWndProc   = DefDlgProcW;
    wc.lpszClassName = L"DepthBasicsAppDlgWndClass";

    if (!RegisterClassW(&wc))
    {
        return 0;
    }

    // Create main application window
    HWND hWndApp = CreateDialogParamW(
        hInstance,
        MAKEINTRESOURCE(IDD_APP),
        NULL,
        (DLGPROC)Direct2DWindow::MessageRouter, 
        reinterpret_cast<LPARAM>(this));

    // Show window
    ShowWindow(hWndApp, nCmdShow);

    const int eventCount = 2;
    HANDLE hEvents[eventCount];

    // Main message loop
    while (WM_QUIT != msg.message)
    {
        hEvents[0] = m_cdmap->GetNextDepthFrameEvent();
        hEvents[1] = m_cdmap->GetNextColorFrameEvent();

        // Check to see if we have either a message (by passing in QS_ALLINPUT)
        // Or a Kinect event (hEvents)
        // Update() will check for Kinect events individually, in case more than one are signalled
        MsgWaitForMultipleObjects(eventCount, hEvents, FALSE, INFINITE, QS_ALLINPUT);

        // Explicitly check the Kinect frame event since MsgWaitForMultipleObjects
        // can return for other reasons even though it is signaled.
        Update();

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // If a dialog message will be taken care of by the dialog proc
            if ((hWndApp != NULL) && IsDialogMessageW(hWndApp, &msg))
            {
                continue;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

/// <summary>
/// Main processing function
/// </summary>
void Direct2DWindow::Update()
{
    m_cdmap->RefreshField();
    const ColorField color_ref(m_cdmap->GetColorFieldRef());
	BYTE *bytearray = const_cast<BYTE*>(color_ref.GetColorArray());
    m_pDrawDepth->Draw(bytearray,
                       color_ref.GetFieldArea() * color_ref.GetBytesPerPixel());
}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK Direct2DWindow::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Direct2DWindow* pThis = NULL;
    
    if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<Direct2DWindow*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<Direct2DWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK Direct2DWindow::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            // Bind application window handle
            m_hWnd = hWnd;

            // Init Direct2D
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

            // Create and initialize a new Direct2D image renderer (take a look at ImageRenderer.h)
            // We'll use this to draw the data we receive from the Kinect to the screen
            m_pDrawDepth = new ImageRenderer();
            HRESULT hr = m_pDrawDepth->Initialize(GetDlgItem(m_hWnd, IDC_VIDEOVIEW),
                                                  m_pD2DFactory,
                                                  m_cdmap->GetFieldWidth(),
                                                  m_cdmap->GetFieldHeight(),
                                                  m_cdmap->GetFieldWidth() * sizeof(long));
            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device.");
            }
        }
        break;

        // If the titlebar X is clicked, destroy app
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            // Quit the main message pump
            PostQuitMessage(0);
            break;

        // Handle button press
        case WM_COMMAND:
            // If it was for the near mode control and a clicked event, change near mode
            if (IDC_CHECK_NEARMODE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
            {
                // Toggle out internal state for near mode
                //m_bNearMode = !m_bNearMode;

                //if (NULL != m_pNuiSensor)
                //{
                //    // Set near mode based on our internal state
                //    m_pNuiSensor->NuiImageStreamSetImageFrameFlags(m_pDepthStreamHandle, m_bNearMode ? NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE : 0);
                //}
            }
            break;
    }

    return FALSE;
}

/// <summary>
/// Set the status bar message
/// </summary>
/// <param name="szMessage">message to display</param>
void Direct2DWindow::SetStatusMessage(WCHAR * szMessage)
{
    SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, (LPARAM)szMessage);
}

