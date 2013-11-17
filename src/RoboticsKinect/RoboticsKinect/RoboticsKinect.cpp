// KinectTest1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <strsafe.h>
#include "ColorDepthMap.h"
#include "KinectNui.h"
#include "resource.h"
#include <iostream>

using namespace std;

/*
int
_tmain(int argc, _TCHAR* argv[])
{
	INuiSensor *pNuiSensor = GetNuiPointer();
	if (NULL == pNuiSensor)
	{
		cout << "failed to init NUI sensor";
		exit(EXIT_FAILURE);
	}
    ColorDepthMap cdmap(pNuiSensor);
    cout << "ping" << endl;
    Direct2DWindow application(&cdmap);
    application.Run(hInstance, nCmdShow);

	ReleaseNui(pNuiSensor);
	return 0;
}
*/

/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	INuiSensor *pNuiSensor = GetNuiPointer();
	if (NULL == pNuiSensor)
	{
		cout << "failed to init NUI sensor";
		exit(EXIT_FAILURE);
	}
    ColorDepthMap cdmap(pNuiSensor, false);
    cout << "ping" << endl;
    Direct2DWindow application(&cdmap);
    application.Run(hInstance, nCmdShow);
}


