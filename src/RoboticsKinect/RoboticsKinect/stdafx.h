// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <Windows.h>
//#include "targetver.h"

// Direct2D Header Files
#include <d2d1.h>
#pragma comment ( lib, "d2d1.lib" )

//#include <stdio.h>
//#include <tchar.h>



// TODO: reference additional headers your program requires here
// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}