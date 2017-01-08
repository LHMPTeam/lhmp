#include "MemoryPatcher.h" 
#include <malloc.h> 

#ifndef WIN32 
#include <sys/mman.h> 
#ifndef PAGESIZE 
#define PAGESIZE 4096 
#endif 
#endif 

ProtectionInfo MemoryPatcher::Unprotect(DWORD dwAddress, int iSize)
{
	ProtectionInfo protectionInfo;
	protectionInfo.dwAddress = dwAddress;
	protectionInfo.iSize = iSize;
#ifdef WIN32 
	VirtualProtect((void *)dwAddress, iSize, PAGE_EXECUTE_READWRITE, &protectionInfo.dwOldProtection);
#else 
	mprotect((void *)((dwAddress / PAGESIZE) * PAGESIZE), PAGESIZE, (PROT_EXEC | PROT_READ | PROT_WRITE));
#endif 
	return protectionInfo;
}

void MemoryPatcher::Reprotect(ProtectionInfo protectionInfo)
{
#ifdef WIN32 
	DWORD dwProtection;
	VirtualProtect((void *)protectionInfo.dwAddress, protectionInfo.iSize, protectionInfo.dwOldProtection, &dwProtection);
#else 
	//get old protection 
#endif 
}

void MemoryPatcher::InstallNopPatch(DWORD dwAddress, int iSize)
{
	DWORD dwAddr = dwAddress;
	ProtectionInfo protectionInfo = Unprotect(dwAddr, iSize);
	memset((void *)dwAddr, X86_NOP, iSize);
	Reprotect(protectionInfo);
}

void * MemoryPatcher::InstallDetourPatchInternal(DWORD dwAddress, DWORD dwDetourAddress, BYTE byteType, int iSize)
{
	BYTE * pbyteTrampoline = (BYTE *)malloc(iSize + 5);
	Unprotect((DWORD)pbyteTrampoline, (iSize + 5));
	ProtectionInfo protectionInfo = Unprotect(dwAddress, (iSize + 5));
	memcpy(pbyteTrampoline, (void *)dwAddress, iSize);
	DWORD dwTrampoline = (DWORD)(pbyteTrampoline + iSize);
	*(BYTE *)dwTrampoline = byteType;
	*(DWORD *)(dwTrampoline + 1) = ((dwAddress + iSize) - dwTrampoline - 5);
	*(BYTE *)dwAddress = byteType;
	*(DWORD *)(dwAddress + 1) = (dwDetourAddress - dwAddress - 5);
	Reprotect(protectionInfo);
	return pbyteTrampoline;
}

void MemoryPatcher::UninstallDetourPatchInternal(DWORD dwAddress, void * pTrampoline, int iSize)
{
	ProtectionInfo protectionInfo = Unprotect(dwAddress, iSize);
	memcpy((void *)dwAddress, pTrampoline, iSize);
	Reprotect(protectionInfo);
	free(pTrampoline);
}

void MemoryPatcher::InstallPatchType(DWORD dwAddress, DWORD dwTypeAddress, BYTE byteType, int iSize)
{
	switch (byteType)
	{
	case X86_JMP:
	{
		InstallDetourPatchInternal(dwAddress, dwTypeAddress, X86_JMP, iSize);
	}
	case X86_CALL:
	{
		InstallDetourPatchInternal(dwAddress, dwTypeAddress, X86_CALL, iSize);
	}
	}
}

void MemoryPatcher::InstallRetnPatch(DWORD dwAddress)
{
	DWORD dwAddr = dwAddress;
	ProtectionInfo protectionInfo = Unprotect(dwAddr, 1);
	*(BYTE *)dwAddr = X86_RETN;
	Reprotect(protectionInfo);
}

void MemoryPatcher::InstallStringPatch(DWORD dwAddress, char * szString, int iSize)
{
	DWORD dwAddr = dwAddress;
	ProtectionInfo protectionInfo = Unprotect(dwAddr, iSize);
	memcpy((void *)dwAddr, szString, iSize);
	Reprotect(protectionInfo);
}

void MemoryPatcher::InstallMethodPatch(DWORD dwHookAddress, DWORD dwFunctionAddress)
{
	DWORD dwHookAddr = dwHookAddress;
	ProtectionInfo protectionInfo = Unprotect(dwHookAddr, 4);
	*(DWORD *)dwHookAddr = (DWORD)dwFunctionAddress;
	Reprotect(protectionInfo);
}

DWORD MemoryPatcher::GetFunctionAddress(LPCWSTR szLibrary, char * szFunction)
{
	return (DWORD)GetProcAddress(LoadLibrary(szLibrary), szFunction);
}

DWORD MemoryPatcher::GetFunctionAddress(LPCWSTR szLibrary, unsigned int uOrdinal)
{
	return GetFunctionAddress(szLibrary, (char *)MAKELONG(uOrdinal, 0));
}

void * MemoryPatcher::InstallDetourPatch(LPCWSTR szLibrary, char * szFunction, DWORD dwFunctionAddress)
{
	return InstallDetourPatchInternal(GetFunctionAddress(szLibrary, szFunction), dwFunctionAddress, X86_JMP, 5);
}

void * MemoryPatcher::InstallDetourPatch(LPCWSTR szLibrary, unsigned int uOrdinal, DWORD dwFunctionAddress)
{
	return InstallDetourPatchInternal(GetFunctionAddress(szLibrary, uOrdinal), dwFunctionAddress, X86_JMP, 5);
}

void MemoryPatcher::UninstallDetourPatch(void * pTrampoline, DWORD dwFunctionAddress)
{
	ProtectionInfo protectionInfo = Unprotect(dwFunctionAddress, 5);
	memcpy((void *)dwFunctionAddress, pTrampoline, 5);
	Reprotect(protectionInfo);
	free(pTrampoline);
}

BYTE MemoryPatcher::InstallDetourPatchWithData(LPCWSTR szLibrary, unsigned int uOrdinal, DWORD dwFunctionAddress)
{
	DWORD dwAddress = GetFunctionAddress(szLibrary, uOrdinal);
	DWORD dwDetourAddress = dwFunctionAddress;
	BYTE byteType = X86_JMP;
	int iSize = 5;
	BYTE * pbyteTrampoline = (BYTE *)malloc(iSize + 5);
	Unprotect((DWORD)pbyteTrampoline, (iSize + 5));
	ProtectionInfo protectionInfo = Unprotect(dwAddress, (iSize + 5));
	memcpy(pbyteTrampoline, (void *)dwAddress, iSize);
	DWORD dwTrampoline = (DWORD)(pbyteTrampoline + iSize);
	*(BYTE *)dwTrampoline = byteType;
	*(DWORD *)(dwTrampoline + 1) = ((dwAddress + iSize) - dwTrampoline - 5);
	*(BYTE *)dwAddress = byteType;
	*(DWORD *)(dwAddress + 1) = (dwDetourAddress - dwAddress - 5);
	Reprotect(protectionInfo);
	return (pbyteTrampoline != NULL);
}

void MemoryPatcher::InstallPushPatch(DWORD dwAddress, DWORD dwFunc)
{
	ProtectionInfo protectionInfo = Unprotect(dwAddress, 5);
	*(BYTE*)(dwAddress) = 0x68;
	*(DWORD*)(dwAddress + 1) = dwFunc;
	Reprotect(protectionInfo);
}

void MemoryPatcher::PatchAddress(DWORD dwAddress, BYTE *bPatch, size_t iSize)
{
	DWORD d, ds;

	VirtualProtect((void*)dwAddress, iSize, PAGE_EXECUTE_READWRITE, &d);
	memcpy((void*)dwAddress, bPatch, iSize);
	VirtualProtect((void*)dwAddress, iSize, d, &ds);
}

void MemoryPatcher::PatchAddress(DWORD dwAddress, DWORD dwPatch)
{
	DWORD d, ds;

	VirtualProtect((void*)dwAddress, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &d);
	*(DWORD *)(dwAddress) = dwPatch;
	VirtualProtect((void*)dwAddress, sizeof(DWORD), d, &ds);
}