#pragma once
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HRCheck
#define HRCheck(x)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			LPSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, "Error", MB_OK);		\
			return false;		\
		}								\
	}
#endif
#else
#ifndef HRCheck
#define HRCheck(x) (x)
#endif
#endif 