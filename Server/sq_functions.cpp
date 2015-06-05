#include "sq_funcdef.h"
#include "sq_functions.h"


int Init(SQVM* vM)
{
	//RegisterFunction(vM, "testfunc", (SQFUNCTION)sq_dopice, 0, "");
	RegisterFunction(vM, "sendPlayerMessage", (SQFUNCTION)sq_sendPlayerMessage, 3, ".is");
	RegisterFunction(vM, "sendAllMessage", (SQFUNCTION)sq_sendAllMessage, 2, ".s");

	// Player natives

	RegisterFunction(vM, "playerChangeSkin", (SQFUNCTION)sq_playerChangeSkin, 3, ".nn");
	RegisterFunction(vM, "playerIsConnected", (SQFUNCTION)sq_playerIsConnected, 2, ".n");
	RegisterFunction(vM, "playerGetSkinID", (SQFUNCTION)sq_playerGetSkinID,2, ".n");
	RegisterFunction(vM, "playerGetHealth", (SQFUNCTION)sq_playerGetHealth, 2, ".n");
	RegisterFunction(vM, "playerGetMoney", (SQFUNCTION)sq_playerGetMoney, 2, ".n");
	RegisterFunction(vM, "playerGetPosition", (SQFUNCTION)sq_playerGetPosition, 2, ".n");
	RegisterFunction(vM, "playerGetRotation", (SQFUNCTION)sq_playerGetRotation, 2, ".n");
	RegisterFunction(vM, "playerGetRotationAsVector", (SQFUNCTION)sq_playerGetRotationAsVector, 2, ".n");
	RegisterFunction(vM, "playerInVehicleID", (SQFUNCTION)sq_playerInVehicleID, 2, ".n");
	RegisterFunction(vM, "playerSetPosition", (SQFUNCTION)sq_playerSetPosition, 5, ".nfff");
	RegisterFunction(vM, "playerSetHealth", (SQFUNCTION)sq_playerSetHealth, 3, ".nf");
	RegisterFunction(vM, "playerSetMoney", (SQFUNCTION)sq_playerSetMoney, 3, ".nn");
	RegisterFunction(vM, "playerEnableMoney", (SQFUNCTION)sq_playerEnableMoney, 3, ".nn");
	RegisterFunction(vM, "playerSetRotation", (SQFUNCTION)sq_playerSetRotation, 5, ".nf");
	RegisterFunction(vM, "playerSetRotationVector", (SQFUNCTION)sq_playerSetRotationVector, 5, ".nfff");
	RegisterFunction(vM, "playerGetIP", (SQFUNCTION)sq_playerGetIP, 2, ".n");
	RegisterFunction(vM, "playerPutToVehicle", (SQFUNCTION)sq_playerPutToVehicle, 4, ".nnn");
	RegisterFunction(vM, "playerKick", (SQFUNCTION)sq_playerKick, 2, ".n");
	RegisterFunction(vM, "playerGetPing", (SQFUNCTION)sq_playerGetPing, 2, ".n");
	RegisterFunction(vM, "playerGetName", (SQFUNCTION)sq_playerGetName, 2, ".n");
	RegisterFunction(vM, "playerAddWeapon", (SQFUNCTION)sq_playerAddWeapon, 5, ".nnnn");
	RegisterFunction(vM, "playerDeleteWeapon", (SQFUNCTION)sq_playerDeleteWeapon, 3, ".nn");
	RegisterFunction(vM, "playerPlayAnim", (SQFUNCTION)sq_playerPlayAnim, 3, ".ns");
	RegisterFunction(vM, "playerPlaySound", (SQFUNCTION)sq_playerPlaySound, 3, ".ns");
	RegisterFunction(vM, "allPlaySound", (SQFUNCTION)sq_allPlaySound, 2, ".s");
	RegisterFunction(vM, "playerToggleCityMusic", (SQFUNCTION)sq_playerToggleCityMusic, 3, ".nn");
	RegisterFunction(vM, "playerLockControls", (SQFUNCTION)sq_playerLockControls, 3, ".nb");
	RegisterFunction(vM, "playerIsLocked", (SQFUNCTION)sq_playerIsLocked, 2, ".n");
	RegisterFunction(vM, "playerToggleCityMusic", (SQFUNCTION)sq_playerToggleCityMusic, 3, ".nn");
	RegisterFunction(vM, "guiToggleNametag", (SQFUNCTION)sq_guiToggleNametag, 3, ".nn");

	RegisterFunction(vM, "playerSetCameraPos", (SQFUNCTION)sq_playerSetCameraPos, 8, ".nffffff");
	RegisterFunction(vM, "playerSetCameraToDefault", (SQFUNCTION)sq_playerSetCameraToDefault, 2, ".n");

	RegisterFunction(vM, "playerSetCameraSwing", (SQFUNCTION)sq_playerSetCameraSwing, 4, ".nnf");
	RegisterFunction(vM, "playerSetCameraFov", (SQFUNCTION)sq_playerSetCameraFov, 3, ".nf");
	RegisterFunction(vM, "playerGetCameraFov", (SQFUNCTION)sq_playerGetCameraFov, 2, ".n");

	RegisterFunction(vM, "playerTimerOn", (SQFUNCTION)sq_playerTimerOn, 6, ".nnnnn");
	RegisterFunction(vM, "playerTimerOff", (SQFUNCTION)sq_playerTimerOff, 2, ".n");
	RegisterFunction(vM, "playerSetTimerInterval", (SQFUNCTION)sq_playerSetTimerInterval, 3, ".nf");
	RegisterFunction(vM, "playerGetTimerInterval", (SQFUNCTION)sq_playerGetTimerInterval, 2, ".n");

	RegisterFunction(vM, "playerSetWeatherParam", (SQFUNCTION)sq_playerSetWeatherParam, 4, ".nsn");
	RegisterFunction(vM, "playerSetObjective", (SQFUNCTION)sq_playerSetObjective, 3, ".ns");
	RegisterFunction(vM, "playerClearObjective", (SQFUNCTION)sq_playerClearObjective, 2, ".n");
	RegisterFunction(vM, "playerAddConsoleText", (SQFUNCTION)sq_playerConsoleAddText, 4, ".nss");

	RegisterFunction(vM, "vehicleSpawn", (SQFUNCTION)sq_vehicleSpawn, 8, ".nffffff");
	RegisterFunction(vM, "vehicleDelete", (SQFUNCTION)sq_vehicleDelete, 2, ".n");
	RegisterFunction(vM, "vehicleGetDriverID", (SQFUNCTION)sq_vehicleGetDriverID, 2, ".n");
	RegisterFunction(vM, "vehicleGetPlayerIDFromSeat", (SQFUNCTION)sq_vehicleGetPlayerIDFromSeat, 2, ".n");
	RegisterFunction(vM, "vehicleGetPosition", (SQFUNCTION)sq_vehicleGetPosition, 2, ".n");
	RegisterFunction(vM, "vehicleGetDamage", (SQFUNCTION)sq_vehicleGetDamage, 2, ".n");
	RegisterFunction(vM, "vehicleGetRotation", (SQFUNCTION)sq_vehicleGetRotation, 2, ".n");
	RegisterFunction(vM, "vehicleSetPosition", (SQFUNCTION)sq_vehicleSetPosition, 5, ".nfff");
	RegisterFunction(vM, "vehicleSetFuel", (SQFUNCTION)sq_vehicleSetFuel, 3, ".nf");
	RegisterFunction(vM, "vehicleGetFuel", (SQFUNCTION)sq_vehicleGetFuel, 2, ".n");
	RegisterFunction(vM, "vehicleSetSpeed", (SQFUNCTION)sq_vehicleSetSpeed, 5, ".nf");
	RegisterFunction(vM, "vehicleSetDamage", (SQFUNCTION)sq_vehicleSetDamage, 3, ".nf");
	RegisterFunction(vM, "vehicleSetRotation", (SQFUNCTION)sq_vehicleSetRotation, 5, ".nfff");
	RegisterFunction(vM, "vehicleToggleRoof", (SQFUNCTION)sq_vehicleToggleRoof, 3, ".nn");
	RegisterFunction(vM, "vehicleToggleSiren", (SQFUNCTION)sq_vehicleToggleSiren, 3, ".nb");
	RegisterFunction(vM, "vehicleGetSirenState", (SQFUNCTION)sq_vehicleGetSirenState, 2, ".n");
	RegisterFunction(vM, "vehicleGetRoofState", (SQFUNCTION)sq_vehicleGetRoofState, 2, ".n");
	RegisterFunction(vM, "vehicleExplode", (SQFUNCTION)sq_vehicleExplode, 2, ".n");
	RegisterFunction(vM, "vehicleExists", (SQFUNCTION)sq_vehicleExists, 2, ".n");

	RegisterFunction(vM, "isAnyPlayerInVehicle", (SQFUNCTION)sq_isAnyPlayerInVehicle, 2, ".n");
	RegisterFunction(vM, "getDistanceBetween3DPoints", (SQFUNCTION)sq_getDistanceBetween3DPoints, 2, ".ffffff");
	RegisterFunction(vM, "getDistanceBetween2DPoints", (SQFUNCTION)sq_getDistanceBetween2DPoints, 2, ".ffff");

	RegisterFunction(vM, "serverGetName", (SQFUNCTION)sq_serverGetName, 1, ".");
	RegisterFunction(vM, "serverGetGamemodeName", (SQFUNCTION)sq_serverGetGamemodeName, 1, ".");
	RegisterFunction(vM, "serverSetGamemodeName", (SQFUNCTION)sq_serverSetGamemodeName, 2, ".s");
	RegisterFunction(vM, "serverGetOnlinePlayers", (SQFUNCTION)sq_serverGetOnlinePlayers, 1, ".");
	RegisterFunction(vM, "serverGetMaxPlayers", (SQFUNCTION)sq_serverGetMaxPlayers, 1, ".");
	RegisterFunction(vM, "serverSetDefaultMap", (SQFUNCTION)sq_serverSetDefaultMap, 2, ".s");

	RegisterFunction(vM, "timerCreate", (SQFUNCTION)sq_timerCreate, 4, ".snn");
	RegisterFunction(vM, "timerDelete", (SQFUNCTION)sq_timerDelete, 2, ".n");

	RegisterFunction(vM, "pickupCreate", (SQFUNCTION)sq_pickupCreate, 7, ".snffff");

	RegisterFunction(vM, "iniGetParam", (SQFUNCTION)sq_iniGetParam, 3, ".ss");
	RegisterFunction(vM, "iniSetParam", (SQFUNCTION)sq_iniSetParam, 4, ".sss");
	RegisterFunction(vM, "iniRemoveFile", (SQFUNCTION)sq_iniRemoveFile, 2, ".s");
	RegisterFunction(vM, "iniCreateFile", (SQFUNCTION)sq_iniCreateFile, 2, ".s");


	RegisterFunction(vM, "callClientFunc", (SQFUNCTION)sq_callClientFunc, 5, ".nss.");
	RegisterFunction(vM, "callFunc", (SQFUNCTION)sq_callFunc, 4, ".ss.");
	
	// part of human body
	RegisterVariable(vM, "PLAYER_RIGHHAND", 1);
	RegisterVariable(vM, "PLAYER_LEFTHAND", 2);
	RegisterVariable(vM, "PLAYER_RIGHTLEG", 3);
	RegisterVariable(vM, "PLAYER_LEFTLEG", 4);
	RegisterVariable(vM, "PLAYER_TORSO", 5);
	RegisterVariable(vM, "PLAYER_HEAD", 6);
	// keyboard const.
	RegisterVariable(vM, "KEY_A", 0); 
	RegisterVariable(vM, "KEY_B", 1);
	RegisterVariable(vM, "KEY_C", 2);
	RegisterVariable(vM, "KEY_D", 3);
	RegisterVariable(vM, "KEY_E", 4); 
	RegisterVariable(vM, "KEY_F", 5);
	RegisterVariable(vM, "KEY_G", 6);
	RegisterVariable(vM, "KEY_H", 7);
	RegisterVariable(vM, "KEY_I", 8);
	RegisterVariable(vM, "KEY_J", 9);
	RegisterVariable(vM, "KEY_K", 10);
	RegisterVariable(vM, "KEY_L", 11);
	RegisterVariable(vM, "KEY_M", 12);
	RegisterVariable(vM, "KEY_N", 13); 
	RegisterVariable(vM, "KEY_O", 14);
	RegisterVariable(vM, "KEY_P", 15);
	RegisterVariable(vM, "KEY_R", 16);
	RegisterVariable(vM, "KEY_Q", 17);
	RegisterVariable(vM, "KEY_S", 18);
	RegisterVariable(vM, "KEY_T", 19);
	RegisterVariable(vM, "KEY_U", 20);
	RegisterVariable(vM, "KEY_V", 21);
	RegisterVariable(vM, "KEY_W", 22);
	RegisterVariable(vM, "KEY_X", 23);
	RegisterVariable(vM, "KEY_Z", 24);

	RegisterVariable(vM, "KEY_NUM1", 25);
	RegisterVariable(vM, "KEY_NUM2", 26);
	RegisterVariable(vM, "KEY_NUM3", 27);
	RegisterVariable(vM, "KEY_NUM4", 28);
	RegisterVariable(vM, "KEY_NUM5", 29);
	RegisterVariable(vM, "KEY_NUM6", 30);
	RegisterVariable(vM, "KEY_NUM7", 31);
	RegisterVariable(vM, "KEY_NUM8", 32);
	RegisterVariable(vM, "KEY_NUM9", 33);
	RegisterVariable(vM, "KEY_NUM0", 34);

	RegisterVariable(vM, "KEY_ARROWUP", 35);
	RegisterVariable(vM, "KEY_ARROWDOWN", 36);
	RegisterVariable(vM, "KEY_ARROWLEFT", 37);
	RegisterVariable(vM, "KEY_ARROWRIGHT", 38);

	return 1; 
}