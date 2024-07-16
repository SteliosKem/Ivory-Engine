#pragma once

#ifdef IV_PLATFORM_WINDOWS
	#ifdef IV_BUILD_DLL
		#define	IVORY_API __declspec(dllexport)
	#else
		#define IVORY_API __declspec(dllimport)
	#endif
#endif