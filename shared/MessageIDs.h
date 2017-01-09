#pragma once

#include "MessageIdentifiers.h"

namespace MessageIDs
{
	enum MessageIDs
	{
		ID_CONNECTION_INIT_LHMP = ID_USER_PACKET_ENUM + 1,
		ID_CONNECTION_REFUSED_LHMP,
		ID_CONNECTION_ACCEPTED_LHMP,
		
		ID_PACKET_LHMP,
		ID_SYNC_LHMP,
		ID_CREATE_PLAYER
	};

	enum LHMPSyncIDs
	{
		LHMP_PLAYER_ONFOOTSYNC

	};

	enum LHMPMessageIDs
	{
		LHMP_PLAYER_SPAWN,
	};

	enum RefusedMessageIDs
	{
		REFUSED_CLIENT_VERSION
	};
}