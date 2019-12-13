#pragma once

#ifdef _WIN64
#ifdef COOKIE_DLL
#define COOKIE_API __declspec(dllexport)
#else
#define COOKIE_API __declspec(dllimport)
#endif
#else
#error Only WIN32 build are supported. Make sure your project has the WIN32 macro defined.
#endif