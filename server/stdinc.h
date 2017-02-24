#pragma once
#pragma warning(disable : 4996)

#include <thread>
#include <chrono>
#include <string>
#include <map>

#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <BitStream.h>

#include <Utils.h>
#include <ini.hpp>
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
#include "Network/Handlers/ServerSyncHandler.h"
#include "Network/Handlers/ServerRakNetHandler.h"

#include "Core.h"