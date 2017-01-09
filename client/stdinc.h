#pragma once
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
#include <MessageIDs.h>
#include <BuildVersion.h>
#include <MessageHandler.h>
#include <SyncStructs/OnFootSync.h>
#include <SyncStructs/OnConnectionStruct.h>

#include "Game/Player.h"
#include "Game/LocalPlayer.h"
#include "Game/Game.h"
#include "Network/Network.h"

#include "Network/Handlers/ClientSyncHandler.h"
#include "Network/Handlers/ClientConnectionHandler.h"

#include "Core.h"