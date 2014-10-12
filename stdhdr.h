/**************************************************************************
	created		:	2011-06-02
	filename	: 	stdhdr.h
	author		:	xebeth`
	copyright	:	North Edge (2011)
	purpose		:	Standard windows platform inclusions
**************************************************************************/
#ifndef __STDHDR_H__
#define __STDHDR_H__

#define WINDOWS_LEAN_AND_MEAN
#define VC_EXTRALEAN

#ifndef STRICT
	#define STRICT
#endif

#include <sdkddkver.h>

// disable analysis warning in shlobj.h
#pragma warning (disable : 6387)

#if defined _AFXDLL || defined _AFXEXT
	#include <afxwin.h>
	#include <afxcmn.h>
#else
	#include <windows.h>	
#endif // _AFXEXT

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
// wide char version of __FILE__
#define _WIDEN(x) L ## x
#define WIDEN(x) _WIDEN(x)
#define __WFILE__ WIDEN(__FILE__)
// pragma message with file & line output
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "
#define warn(x) message(__FILE__LINE__ x "\n")

#if defined _DEBUG && defined __USE_VLD
	#include <vld.h>
#endif // _DEBUG

#endif//__STDHDR_H__