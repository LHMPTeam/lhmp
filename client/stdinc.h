#pragma once
#pragma warning(disable : 4996)
#define _WINSOCKAPI_
#define CLIENT
#include <string>
#include <thread>
#include <map>
#include <vector>
#include <queue>
#include <mutex>
#include <Windows.h>
#include <wchar.h>

#include <d3d8.h>
#include <d3dx8.h>
#include <detours.h>

#include <MafiaSDK/MafiaSDK.h> 
#include <Singleton.hpp>

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
#include "Graphics/Chat.h"
#include "Graphics/LoadingScreen.h"
#include "Graphics/Graphics.h"
#include "Network/TickManager.h"

#include "Game/Player.h"
#include "Game/LocalPlayer.h"
#include "Game/Game.h"
#include "Network/Network.h"
#include "Testing/Sandbox.h"

#include "Network/Handlers/ClientSyncHandler.h"
#include "Network/Handlers/ClientConnectionHandler.h"
#include "Network/Handlers/ClientRakNetHandler.h"
#include "Network/Handlers/ClientPlayerHandler.h"

#include "Core.h"
#include "RawInput.h"
#include "Graphics/Direct3DDevice8Proxy.h"