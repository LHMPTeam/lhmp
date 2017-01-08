#pragma once

namespace MessageIDs
{
	enum MessageIDs
	{
		ID_CONNECTION_INIT_LHMP = ID_USER_PACKET_ENUM + 1,
		ID_CONNECTION_REFUSED_LHMP,
		ID_CONNECTION_ACCEPTED_LHMP,
		
		ID_PACKET_LHMP,
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