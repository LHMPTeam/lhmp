/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose server-side Sqlite support
@author Davo
******************************************************************************/

#include "../sdks/sqlite/sqlite3.h"
#include "squirrelheads.h"

SQInteger sq_sqliteOpen(SQVM *vm)
{
	const SQChar*		dbname;
	sq_getstring(vm, -1, &dbname);

	int rc;
	sqlite3* db = NULL;
	rc = sqlite3_open(dbname, &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	sq_pushuserpointer(vm, db);
	return 1;
}

static int callback(void *squirrel, int argc, char **argv, char **azColName){
	if (argc > 0)
	{
		SQVM* vm = (SQVM*)squirrel;
		int i;
		for (i = 0; i < argc; i++){
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
	}
	return 0;
}

SQInteger sq_sqliteExec(SQVM *vm)
{
	int rc;
	const SQChar*		query;
	//sq_getinteger(vm, -2, &ID);
	//sq_getinteger(vm, -1, &skinID);
	sqlite3* db = NULL;
	char *zErrMsg = NULL;
	sq_getuserpointer(vm, -2, (SQUserPointer*)db);
	sq_getstring(vm, -1, &query);
	sq_pushroottable(vm);
	rc = sqlite3_exec(db, query, callback, vm, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	return 1;
}

SQInteger sq_sqliteClose(SQVM *vm)
{
	SQInteger	ID, skinID;
	//sq_getinteger(vm, -2, &ID);
	//sq_getinteger(vm, -1, &skinID);
	sqlite3* db = NULL;
	sq_getuserpointer(vm, -1, (SQUserPointer*)db);
	sqlite3_close(db);
	return 1;
}
