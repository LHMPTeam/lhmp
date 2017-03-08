#include <stdinc.h>

ClientPlayerHandler::ClientPlayerHandler(std::map<RakNet::RakNetGUID, Player*>* players)
	: mPlayers(players)
{

}


ClientPlayerHandler::~ClientPlayerHandler()
{
}

void ClientPlayerHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch (packet->data[1])
	{
	case MessageIDs::LHMPID_PLAYER_CREATE:
	{
		CreatePlayer(network, packet);
	}
	break;
	case MessageIDs::LHMPID_PLAYER_CHATMSG:
	{
		OnChatMessage(network->GetPeer(), packet);
	}
	break;

	case MessageIDs::LHMPID_PLAYER_RESPAWN:
	{
		OnRespawn(network->GetPeer(), packet);
	}
	break;

	case MessageIDs::LHMPID_PLAYER_ONSHOOT:
	{
		OnShoot(network->GetPeer(), packet);
	}
	break;

	case MessageIDs::LHMPID_PLAYER_WEAPON_SWITCH:
	{
		OnWeaponSwitch(network->GetPeer(), packet);
	}
	break;

	case MessageIDs::LHMPID_PLAYER_WEAPON_ADD:
	{
		OnWeaponAdd(network->GetPeer(), packet);
	}
	break;
	}
}

void ClientPlayerHandler::CreatePlayer(Network * network, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID playerGuid;
	bitStream.Read(playerGuid);

	size_t nickNameLenght;
	bitStream.Read(nickNameLenght);
	wchar_t* allocatedNickName = new wchar_t[nickNameLenght];
	bitStream.Read(allocatedNickName);

	RakNet::RakString modelName;
	bitStream.Read(modelName);
	Player* newPlayer = new Player(modelName.C_String());
	newPlayer->Spawn();
	newPlayer->SetNickName(allocatedNickName);


	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"{FFCC2002}[LHMP] {FFf9f8f7}<" + std::wstring(allocatedNickName) + L"> Connected.");
	mPlayers->insert(std::make_pair(playerGuid, newPlayer));
}

void ClientPlayerHandler::OnChatMessage(RakNet::RakPeerInterface * peer, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	
	RakNet::RakNetGUID playerGUID;
	bitStream.Read(playerGUID);

	auto player = mPlayers->at(playerGUID);

	if (player != nullptr)
	{
		size_t playerMessageSize;
		bitStream.Read(playerMessageSize);
		wchar_t* playerMessageBuff = new wchar_t[playerMessageSize];
		bitStream.Read(playerMessageBuff);

		Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"<" + player->GetNickName() + L"> " + std::wstring(playerMessageBuff));
	}
}

void ClientPlayerHandler::OnHit(RakNet::RakPeerInterface * peer, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakNetGUID playerGUID;
	bitStream.Read(playerGUID);

	auto player = mPlayers->at(playerGUID);

	if (player != nullptr)
	{
		int hitType;
		const Vector3D unk1, unk2, unk3;
		float damage;
		RakNet::RakNetGUID atackerGUID;
		unsigned long hittedPart;

		bitStream.Read(hitType);
		bitStream.Read(unk1);
		bitStream.Read(unk2);
		bitStream.Read(unk3);
		bitStream.Read(damage);
		bitStream.Read(atackerGUID);
		bitStream.Read(hittedPart);

		MafiaSDK::C_Human* getPointer = NULL;

		for (auto player : Core::GetCore()->GetNetwork()->GetPlayers())
		{
			if (player.first == atackerGUID)
				getPointer = player.second->GetActor();
		}

		player->GetActor()->Hit(hitType, unk1, unk2, unk3, damage, getPointer, hittedPart, NULL);
	}

}


void ClientPlayerHandler::OnShoot(RakNet::RakPeerInterface * peer, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakNetGUID playerGUID;
	bitStream.Read(playerGUID);
	Vector3D vPos;
	bitStream.Read(vPos);

	auto player = mPlayers->at(playerGUID);

	if (player != nullptr)
	{
		player->GetActor()->Do_Shoot(true, &vPos);
		player->GetActor()->Do_Shoot(false, NULL);
	}
}

void ClientPlayerHandler::OnWeaponSwitch(RakNet::RakPeerInterface * peer, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakNetGUID playerGUID;
	bitStream.Read(playerGUID);

	auto player = mPlayers->at(playerGUID);

	if (player != nullptr)
	{
		byte weaponIdToChange;
		bitStream.Read(weaponIdToChange);

		MafiaSDK::C_Actor* playerActor = player->GetActor();
		*(byte*)((unsigned long)playerActor + 0x4A0) = weaponIdToChange;
		player->GetActor()->Do_ChangeWeapon(0, 0);
	}
}

void ClientPlayerHandler::OnWeaponAdd(RakNet::RakPeerInterface * peer, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakNetGUID playerGUID;
	bitStream.Read(playerGUID);

	auto player = mPlayers->at(playerGUID);

	if (player != nullptr)
	{
		byte weaponId;
		int ammoInClip, ammoHidden;

		bitStream.Read(weaponId);
		bitStream.Read(ammoInClip);
		bitStream.Read(ammoHidden);

		player->GetActor()->G_Inventory_AddWeapon((int)weaponId, ammoInClip, ammoHidden);
	}
}

void ClientPlayerHandler::OnRespawn(RakNet::RakPeerInterface * peer, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakNetGUID playerGUID;
	bitStream.Read(playerGUID);

	auto player = mPlayers->at(playerGUID);

	if (player != nullptr)
	{
		player->Respawn();
	}
}