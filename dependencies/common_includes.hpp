#pragma once

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>

#include "utilities/singleton.hpp"
#include "utilities/fnv.hpp"
#include "utilities/utilities.hpp"
#include "utilities/render.hpp"
#include "utilities/hook.hpp"
#include "interfaces/interfaces.hpp"
#include "utilities/netvars.hpp"
#include "../core/features/visuals/visuals.hpp"
#include "../source-sdk/sdk.hpp"
#include "../core/hooks/hooks.hpp"
#include "../dependencies/utilities/singleton.hpp"
#include "../dependencies/math/math.hpp"
#include "../dependencies/interfaces/ivtraceray.hpp"

template< typename Function > Function call_vfunc(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}