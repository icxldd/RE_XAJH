#pragma once

namespace AnyCall
{
	template <typename T, typename... Args>
	T invokeStdcall(void* functionAddress, Args... args) noexcept
	{
		typedef T(__stdcall* stdFuncDef)(Args...);
		stdFuncDef f = (stdFuncDef)functionAddress;
		return f(args...);
	}

	template <typename T, typename... Args>
	T invokeCdecl(void* functionAddress, Args... args) noexcept
	{
		typedef T(__cdecl* cDeclFuncDef)(Args...);
		cDeclFuncDef f = (cDeclFuncDef)functionAddress;
		return f(args...);
	}

	template<typename R, typename... Args>
	R invokeThiscall(void* ecx, void* address, Args...args) noexcept
	{
		typedef R(__thiscall* thisFuncDef)(void*, Args...);
		thisFuncDef function = (thisFuncDef)address;
		return function(ecx, args...);
	}

	template<typename R, typename... Args>
	R invokeFastcall(void* ecx, void* edx, void* address, Args...args) noexcept
	{
		typedef R(__fastcall* fastFuncDef)(void*, void*, Args...);
		fastFuncDef function = (fastFuncDef)address;
		return function(ecx, edx, args...);
	}
}