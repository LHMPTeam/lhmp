#pragma once
#pragma warning(disable : 4996)
#define _WINSOCKAPI_
#define CLIENT
#include <string>
#include <thread>
#include <map>
#include <Windows.h>

#include <MafiaSDK/MafiaSDK.h> 

#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <GetTime.h>
#include <MessageIDs.h>
#include <BuildVersion.h>
#include <MessageHandler.h>
#include <PacketStructs/OnFootSyncStruct.h>
#include <PacketStructs/OnConnectionStruct.h>

#include "Interpolator.h"
#include "Network/TickManager.h"
#include "Game/Player.h"
#include "Game/LocalPlayer.h"
#include "Game/Game.h"
#include "Network/Network.h"

#include "Network/Handlers/ClientSyncHandler.h"
#include "Network/Handlers/ClientConnectionHandler.h"
#include "Network/Handlers/ClientRakNetHandler.h"
#include "Network/Handlers/ClientPlayerHandler.h"

#include "Core.h"