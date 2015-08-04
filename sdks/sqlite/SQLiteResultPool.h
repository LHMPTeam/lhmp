#include <iostream>
#include <vector>
#include "../sqlite/sqlite3.h"

std::vector<sqlite3_stmt*> SQLiteQueryPool;

int SQLiteResultPool_GetResultID(sqlite3_stmt* sqliteresult)
{
	for (size_t i = 0; i < SQLiteQueryPool.size(); i++)
	{
		if (SQLiteQueryPool[i] == sqliteresult)
		{
			return i;
		}
	}
}