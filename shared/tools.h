//#ifndef SHARED_TOOLS
//#define SHARED_TOOLS
#pragma once
#include <stdio.h>
#include <iostream>
#ifdef _WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
typedef unsigned char byte;
typedef unsigned long DWORD;
#endif
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <math.h> 


namespace Tools
{
	#ifdef _WIN32
	static void WriteBytes(DWORD address,byte* bytes,size_t size)
	{
		DWORD lpflOldProtect;
		VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		memcpy((void*)address, (void*)bytes, size);
		VirtualProtect((void*)address, size, lpflOldProtect, &lpflOldProtect);
	}
	static void SetString(DWORD address, char* string)
	{
		DWORD oldp;
		VirtualProtect((PVOID)address,2,PAGE_EXECUTE_READWRITE,&oldp);
		strcpy((PCHAR)address,string);
	}

	static void SetValue(DWORD address, int value) 
	{
		DWORD oldp;
		VirtualProtect((PVOID)address,2,PAGE_EXECUTE_READWRITE,&oldp);
		(address) = value;
	}
	static void SetValueFloat(DWORD address, float value) 
	{
		DWORD oldp;
		VirtualProtect((PVOID)address,2,PAGE_EXECUTE_READWRITE,&oldp);
		(address) = (DWORD) value;
	}
	static void SetValueNew(HANDLE hPro, LPVOID address, int value) 
	{
		WriteProcessMemory(hPro, address,(LPCVOID) value,sizeof(value),NULL);
	}
	static void Nop(DWORD address,int howmany)
	{
		for(int i = 0; i < howmany;i++)
		{
			DWORD oldp;
			VirtualProtect((PVOID)address,howmany,PAGE_EXECUTE_READWRITE,&oldp);
			*(BYTE*)(address+i) = 0x90;
		}
	}

	static DWORD GetARGBFromString(char buff[]) 
	{
		if(strlen(buff) < 6)
			return 0xFFFFFFFF;
		DWORD result = 0xff000000;
		unsigned long multiply;
		//unsigned long plus;
		for(short i=0; i < 3; i++)
		{
			if(buff[i*2] >= '0' && buff[i*2] <= '9')
			{
				multiply = (buff[i*2]-'0')*0x10;
			} else { 
				if(buff[i*2] >= 'a' && buff[i*2] <= 'f')
					multiply = (buff[i*2]-'a'+10)*0x10;
				else if(buff[i*2] >= 'A' && buff[i*2] <= 'F')
					multiply = (buff[i*2]-'A'+10)*0x10;
				else
					multiply = 15*16;
			}

			if(buff[i*2+1] >= '0' && buff[i*2+1] <= '9')
			{
				multiply += buff[i*2+1]-'0';
			} else { 
				if(buff[i*2+1] >= 'a' && buff[i*2+1] <= 'f')
					multiply += buff[i*2+1]-'a'+10;
				else if(buff[i*2+1] >= 'A' && buff[i*2+1] <= 'F')
					multiply += buff[i*2+1]-'A'+10;
				else
					multiply += 15;
			}
			for(int e=2;e > i;e--)
			{
				multiply = multiply*16*16;
			}
			result += multiply;
			//memcpy((void*)((result)+0x2+i),(const void*) multiply,1);
			//*(unsigned char*) (result+2+i) = multiply;
		}
		return result;
	}
	static Vector3D ComputeOffsetDegrees(float degree)
	{
		Vector3D ang;
		ang.x = (float) (cos(degree*3.14/180)-sin(degree*3.14/180));
		ang.z = (float) (sin(degree*3.14/180)+cos(degree*3.14/180));
		//x' = x*cos b - y*sin b
		//y' = x*sin b + y*cos b
		return ang;
	}
	static void InstallCallHook(DWORD address, DWORD function)
	{
		DWORD lpflOldProtect;
		VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		*(BYTE*)(address) = 0xE8;
		*(DWORD*)(address + 1) = (unsigned long)function - (address+5);
		VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
	}	
	static void InstallJmpHook(DWORD address, DWORD function)
	{
		DWORD lpflOldProtect;
		VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		*(BYTE*)(address) = 0xE9;
		*(DWORD*)(address + 1) = (unsigned long)function - (address + 5);
		VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
	}
	#endif
	static float GetMetricUnitNum(float num)
	{
		while ((num / 1000.0f) >= 1.0f)
		{
			num = num / 1000.0f;
		}
		return num;
	}
	static int GetMetricUnitIndex(float num)
	{
		int i = 0;
		while ((num / 1000.0f) >= 1)
		{
			i++;
			num = (num / 1000.0f);
		}
		return i;
	}
	static char MetricUnits[][10] =
	{
		"","K", "M", "G", "T", "P"
	};

	static float GetDistanceBetween3DPoints(Vector3D pos1, Vector3D pos2)
	{
		return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) + pow(pos1.z - pos2.z, 2));
	}
	static bool isFloatNan(float f){ return (f != f); };
	static Vector3D NullVect()
	{
		Vector3D vect;
		vect.x = 0.0f;
		vect.y = 0.0f;
		vect.z = 0.0f;
		return vect;
	}

	template <typename It> static It Clamp(It input, It min, It max)
	{
		if (input < min)
			return min;
		if (input > max)
			return max;
		return input;
	}
	static bool fileExists(char filename[255])
	{
		FILE* file = fopen(filename, "r");
		if (file != NULL)
		{
			fclose(file);
			return 1;
		}
		return 0;
	}

	static bool isStringNumeric(char* string)
	{
		int len = strlen(string);
		for (int i = 0; i < len; i++)
		{
			if (string[i] < '0' || string[i] > '9')
			{
				return false;
			}
		}
		return true;
	}

	// game wep
	static bool isShootingArm(int ID)
	{
		return !(ID > -1 && ID < 6) || (ID > 14 && ID < 33);
	}
	static void GetProcessNameFromPath(char* path, char* output,int maxlen)
	{
		int len = strlen(path);
		for (int i = len - 1; i >= 0; i--)
		{
			if (path[i] == '\\' || path[i] == '/')
			{
				strncpy(output, &path[i+1],maxlen);
				return;
			}
		}
		sprintf(output, "null");
	}
	static void AnalyseUrl(char* url, char* host,char* rest)
	{
		int len = strlen(url);
		for (int i = 0; i < len; i++)
		{
			if (url[i] == '/')
			{
				memset(host, NULL, i + 1);
				strncpy(host, url, i);
				strcpy(rest, url+i);
				return;
			}
		}
		strcpy(host, url);
		sprintf(rest, "");
	}
	static bool IsEmptyString(char* string)
	{
		return (string[0] == 0x0); // if C-string string has first char \0, it's empty
	}

	static bool  hexChars[] = {	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
							1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
							0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
							1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
							1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	static bool isHexChar(char hChar)
	{
		return hexChars[hChar];
	}

	static bool isHEXStamp(char* pch)
	{
		for (short i = 1; i < 7; i++)
		{
			//if(!(((pch[i] >= 'a') && (pch[i] <= 'f')) || ((pch[i] >= 'A') && (pch[i] <= 'F')) || ((pch[i] >= '0') && (pch[i] <= '9'))))
			if (Tools::isHexChar(pch[i]) == false)
			{
				return false;
			}

		}
		return true;
	}
}