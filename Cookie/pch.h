#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <cassert>
#include <cstdint>
#include <tchar.h>

#include <exception>

#define _XM_NO_INTRINSICS_
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>