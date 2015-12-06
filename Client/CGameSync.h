#pragma once



#include "../shared/structures.h"
#include "CPedPool.h"
#include "CVehiclePool.h"
#include "CNetwork.h"

// CGameSync keep all data synced properly with respect to server-side
class CGameSync
{
public:
	// Handle all events sent over the network (server-side) 
	void HandlePacket(RakNet::Packet* packet, RakNet::TimeMS timestamp);

	// When local player picks up a weapon from the ground
	void Engine_onPlayerTakeWeapon(DWORD wepId, DWORD wepLoaded, DWORD wepHidden);
	// When local player swap his weapon 
	void Engine_onChangeWeapon(DWORD wepId);
	// When local player shoot at @position with @weaponID 
	void Engine_onShoot(Vector3D position, DWORD weaponID);
	// When local player is shot by another player
	void Engine_onPlayerGetHit(CPed* attacker);
	// When local player is killed by enemy
	void Engine_onPlayerDie(CPed* enemy, unsigned char hitbox);
	// When local player enters a car
	void Engine_onPlayerEnterVehicle(CVehicle* vehicle, DWORD seatID);
	// When local player exits a car
	void Engine_onPlayerExitVehicle(CVehicle* vehicle);
	// When local player jacks someone from a car
	void Engine_onPlayerCarjack(CVehicle* vehicle, int seatId);
	// When local player throws a granade/molotov
	void Engine_onPlayerThrowGranade(Vector3D position);
	// When local player throws a granade/molotov
	void Engine_onDoorStateChange(char* doorName, int state,bool facing);
	// When a car explodes
	void Engine_onExplodeCar(CVehicle* vehicle);
	// When player died and the game is about to reload
	void Engine_onMapRespawn();
	// When vehicle's engine changes
	void Engine_onVehicleEngineStateChange(CVehicle* vehicle, BYTE state);

	/* Server side functions*/
private:
	// when anyone send a text message
	void onTextMessage(RakNet::BitStream* bitInput);

	// regular position etc refreshment when player is on foot (not in a car)
	void onPlayerUpdateOnFoot(RakNet::BitStream* bitInput, RakNet::TimeMS timestamp);

	// regular position etc refreshment when player is in the CAR
	void onPlayerUpdateInCar(RakNet::BitStream* bitInput);

	// when player connects, new instance must be created & filled
	void onPlayerConnect(RakNet::BitStream* bitInput);

	// when player disconnect
	void onPlayerDisconnect(RakNet::BitStream* bitInput);

	// when player respawns (after death)
	void onPlayerRespawn(RakNet::BitStream* bitInput);

	// when player changes skin
	void onPlayerChangeSkin(RakNet::BitStream* bitInput);

	// when player starts a custom animation
	void onPlayerPlayAnimation(RakNet::BitStream* bitInput);

	// custom sound track from Mafia will be played
	void playSound(RakNet::BitStream* bitInput);
	
	// used for scoretable
	void onPlayerPingArrives(RakNet::BitStream* bitInput);

	// when player is given a weapon
	void onPlayerAddWeapon(RakNet::BitStream* bitInput);

	// when player is taken the weapon
	void onPlayerDeleteWeapon(RakNet::BitStream* bitInput);

	// when player changes his weapon (he hide it and take another one)
	void onPlayerChangeWeapon(RakNet::BitStream* bitInput);

	// when player changes his weapon (he hide it and take another one)
	void onPlayerShoot(RakNet::BitStream* bitInput);

	// when player throws granade/molotov
	void onPlayerThrowGranade(RakNet::BitStream* bitInput);

	// when player dies
	void onPlayerDie(RakNet::BitStream* bitInput);

	// when player dies (end)
	void onPlayerDieEnd(RakNet::BitStream* bitInput);

	// when player is "inserted" into car (in no time)
	void onPlayerIsPutToCar(RakNet::BitStream* bitInput);

	// when player is "kicked" from car (in no time)
	void onPlayerIsKickedFromCar(RakNet::BitStream* bitInput);

	// when GUI's money changes
	void onGUIMoneyChange(RakNet::BitStream* bitInput);

	// when GUI's money are enabled or disabled
	void onGUIEnableChange(RakNet::BitStream* bitInput);

	// when nametags are enabled or disabled
	void onGUIEnableNametags(RakNet::BitStream* bitInput);

	// when player position is forced by server (teleport,  etc)
	void onPlayerPositionChange(RakNet::BitStream* bitInput);
	// when player rotation is forced by server (teleport,  etc)
	void onPlayerRotationChange(RakNet::BitStream* bitInput);

	///////////////////////////////////////////////////////////////////////////
	// Set of camera functions
	///////////////////////////////////////////////////////////////////////////
	// when detach camera is set somewhere
	void onCameraIsSet(RakNet::BitStream* bitInput);

	// when camera is reattached to localplayer
	void onCameraReturn(RakNet::BitStream* bitInput);

	// when camera side hanging is changed (e.g. ship mission)
	void onCameraSetSwing(RakNet::BitStream* bitInput);

	// when camera field of view is changed
	void onCameraSetFOV(RakNet::BitStream* bitInput);

	///////////////////////////////////////////////////////////////////////////
	// Client stuff
	///////////////////////////////////////////////////////////////////////////
	// when music is toggled 
	void onMusicIsTurnedOff(RakNet::BitStream* bitInput);

	void onWeatherIsChanged(RakNet::BitStream* bitInput);

	// when objectives (which are hidden in anyway - Davo is an idiot) are changed
	void onObjectivesAreSet(RakNet::BitStream* bitInput);
	void onObjectivesAreCleared(RakNet::BitStream* bitInput);

	// on new text is added into ingame console (left-bottom console, e.g. when you pick up a gun)
	void onConsoleTextIsAdded(RakNet::BitStream* bitInput);

	// when player is detached or attached from/to game control
	// NOTE: nowadays it's done at client-side, too
	void onPlayerControlsAreLocked(RakNet::BitStream* bitInput);


	///////////////////////////////////////////////////////////////////////////
	// Vehicle Sync
	///////////////////////////////////////////////////////////////////////////
	void onVehicleIsSpawned(RakNet::BitStream* bitInput, RakNet::TimeMS timestamp);

	// when vehicle is removed
	void onVehicleIsDeleted(RakNet::BitStream* bitInput);

	// on vehicle engine state change -> when engine is started / switched off
	void onVehicleEngineStateChange(RakNet::BitStream* bitInput);

	// when vehicle roof is hidden or shown
	void onVehicleRoofIsChanged(RakNet::BitStream* bitInput);

	// when vehicle's siren is switched on/off
	void onVehicleSirenIsSet(RakNet::BitStream* bitInput);

	// when vehicle's fuel is set
	void onVehicleFuelIsSet(RakNet::BitStream* bitInput);

	// when player enters a car
	void onVehicleIsEntered(RakNet::BitStream* bitInput);

	// when player exits a car
	void onVehicleIsExited(RakNet::BitStream* bitInput);

	// on vehicle refreshment arrives
	void onVehicleSyncArrives(RakNet::BitStream* bitInput, RakNet::TimeMS timestamp);

	// when player is jacked by another one
	void onVehicleIsJacked(RakNet::BitStream* bitInput);

	// when vehicle's damage changes (bumps into wall, collision, etc)
	void onVehicleIsDamaged(RakNet::BitStream* bitInput);

	// when vehicle's damage changes (engine shot by a player)
	void onVehicleIsDamagedByShot(RakNet::BitStream* bitInput);

	// overdrives vehicle position (even if localplayer is the driver)
	void onVehiclePositionIsSet(RakNet::BitStream* bitInput);

	// overdrives vehicle rotation (even if localplayer is the driver)
	void onVehicleRotationIsSet(RakNet::BitStream* bitInput);

	// overdrives vehicle speed (even if localplayer is the driver)
	void onVehicleSpeedIsSet(RakNet::BitStream* bitInput);

	// when vehicle is respawned (after explosion)
	void onVehicleRespawned(RakNet::BitStream* bitInput);

	// when vehicle is respawned (after explosion)
	void onVehicleExploded(RakNet::BitStream* bitInput);

	///////////////////////////////////////////////////////////////////////////
	// Door Sync
	///////////////////////////////////////////////////////////////////////////
	void onDoorStateChange(RakNet::BitStream* bitInput);

	// on map change
	void onMapChange(RakNet::BitStream* bitInput);

	///////////////////////////////////////////////////////////////////////////
	// Pickups Sync
	///////////////////////////////////////////////////////////////////////////

	void onPickupIsCreated(RakNet::BitStream* bitInput);

	void onPickupIsDeleted(RakNet::BitStream* bitInput);

	void onPickupIsHidden(RakNet::BitStream* bitInput);
};