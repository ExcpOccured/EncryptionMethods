#include "pch.h"

BOOL APIENTRY dll_main(HMODULE h_module,
	const DWORD  ul_reason_for_call,
	LPVOID lp_reserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	default: ;
	}
	return TRUE;
}

