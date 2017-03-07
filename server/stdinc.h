#pragma once
#pragma warning(disable : 4996)

#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <ini.hpp>

#include <Utils.h>
#include <Singleton.hpp>
#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "MessageHandler.h"
#include <PacketStructs/OnConnectionStruct.h>
#include <PacketStructs/OnFootSyncStruct.h>

#include <MessageIDs.h>
#include <GetTime.h>
#include <BuildVersion.h>

#include "Network/Client.h"
#include "Network/Network.h"
#include "Network/TickManager.h"

#include "Network/Handlers/ServerConnectionHandler.h"
#include "Network/Handlers/ServerPlayerHandler.h"
#include "Network/Handlers/ServerSyncHandler.h"
#include "Network/Handlers/ServerRakNetHandler.h"

#include "Core.h"