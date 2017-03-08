#pragma once

class ServerPlayerHandler : MessageHandler
{
public:
	ServerPlayerHandler(std::map<RakNet::RakNetGUID, Client*> *Clients);
	~ServerPlayerHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;
private:
	void OnChatMessage(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	void OnHit(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	void OnShoot(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	void OnRespawn(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	void OnWeaponSwitch(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	void OnWeaponAdd(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	std::map<RakNet::RakNetGUID, Client*> *mClients;
};