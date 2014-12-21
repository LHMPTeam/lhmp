#pragma once
#include "sq_utils.h"
//#include "sq_utils.h"
#include <iostream>
#include <stdio.h>

SQInteger sq_dopice(SQVM* pVM);
SQInteger sq_sendPlayerMessage(SQVM *vm);
SQInteger sq_sendAllMessage(SQVM *vm); 
SQInteger sq_playerChangeSkin(SQVM *vm);
SQInteger sq_playerIsConnected(SQVM *vm);
SQInteger sq_playerGetSkinID(SQVM *vm); 
SQInteger sq_playerGetHealth(SQVM *vm);
SQInteger sq_playerGetPosition(SQVM *vm);
SQInteger sq_playerGetRotation(SQVM *vm);
SQInteger sq_playerInVehicleID(SQVM *vm);
SQInteger sq_playerSetPosition(SQVM *vm);
SQInteger sq_playerSetHealth(SQVM *vm);
SQInteger sq_playerGetMoney(SQVM *vm);
SQInteger sq_playerSetRotation(SQVM *vm);
SQInteger sq_playerSetMoney(SQVM *vm);
SQInteger sq_playerEnableMoney(SQVM *vm);
SQInteger sq_playerGetIP(SQVM *vm);
SQInteger sq_playerPutToVehicle(SQVM *vm);
SQInteger sq_playerKick(SQVM *vm);
SQInteger sq_playerGetPing(SQVM *vm);
SQInteger sq_playerGetName(SQVM *vm);
SQInteger sq_playerAddWeapon(SQVM *vm);
SQInteger sq_playerDeleteWeapon(SQVM *vm);
SQInteger sq_playerPlayAnim(SQVM *vm);
SQInteger sq_playerPlaySound(SQVM *vm);
SQInteger sq_playerToggleCityMusic(SQVM *vm);
SQInteger sq_playerSetWeatherParam(SQVM *vm);
SQInteger sq_playerSetCameraSwing(SQVM *vm);
SQInteger sq_playerSetCameraFov(SQVM *vm);
SQInteger sq_playerGetCameraFov(SQVM *vm);
SQInteger sq_playerTimerOn(SQVM *vm);
SQInteger sq_playerTimerOff(SQVM *vm);
SQInteger sq_playerSetTimerInterval(SQVM *vm);
SQInteger sq_playerGetTimerInterval(SQVM *vm);
SQInteger sq_playerSetObjective(SQVM *vm);
SQInteger sq_playerClearObjective(SQVM *vm);
SQInteger sq_playerConsoleAddText(SQVM *vm);
SQInteger sq_playerLockControls(SQVM *vm);
SQInteger sq_playerIsLocked(SQVM *vm);
SQInteger sq_allPlaySound(SQVM *vm);
SQInteger sq_guiToggleNametag(SQVM *vm);

SQInteger sq_playerSetCameraPos(SQVM *vm);
SQInteger sq_playerSetCameraToDefault(SQVM *vm);

SQInteger sq_vehicleSpawn(SQVM *vm);
SQInteger sq_vehicleDelete(SQVM *vm);
SQInteger sq_vehicleGetDriverID(SQVM *vm);
SQInteger sq_vehicleGetPlayerIDFromSeat(SQVM *vm);
SQInteger sq_vehicleGetPosition(SQVM *vm);
SQInteger sq_vehicleGetDamage(SQVM *vm);
SQInteger sq_vehicleGetRotation(SQVM *vm);
SQInteger sq_vehicleSetPosition(SQVM *vm);
SQInteger sq_vehicleSetSpeed(SQVM *vm);
SQInteger sq_vehicleSetDamage(SQVM *vm);
SQInteger sq_vehicleSetRotation(SQVM *vm);
SQInteger sq_vehicleToggleRoof(SQVM *vm);
SQInteger sq_vehicleGetRoofState(SQVM *vm);
SQInteger sq_vehicleToggleSiren(SQVM *vm);
SQInteger sq_vehicleGetSirenState(SQVM *vm);
SQInteger sq_vehicleExplode(SQVM *vm);

SQInteger sq_isAnyPlayerInVehicle(SQVM *vm);

SQInteger sq_serverGetName(SQVM *vm);
SQInteger sq_serverGetGamemodeName(SQVM *vm);
SQInteger sq_serverSetGamemodeName(SQVM *vm);
SQInteger sq_serverGetOnlinePlayers(SQVM *vm);
SQInteger sq_serverGetMaxPlayers(SQVM *vm);
SQInteger sq_serverSetDefaultMap(SQVM *vm);


SQInteger sq_timerCreate(SQVM *vm);
SQInteger sq_timerDelete(SQVM *vm);


SQInteger sq_pickupCreate(SQVM *vm);
SQInteger sq_pickupDelete(SQVM *vm);

SQInteger sq_iniGetParam(SQVM *vm);
SQInteger sq_iniSetParam(SQVM *vm);
SQInteger sq_iniRemoveFile(SQVM *vm);
SQInteger sq_iniCreateFile(SQVM *vm);