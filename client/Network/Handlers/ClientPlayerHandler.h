#pragma once

class ClientPlayerHandler : MessageHandler
{
public:
	ClientPlayerHandler(std::map<RakNet::RakNetGUID, Player*>* players);
	~ClientPlayerHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;
private:
	void CreatePlayer(Network* network, RakNet::Packet* packet);
	void OnChatMessage(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	void OnHit(RakNet::RakPeerInterface * peer, RakNet::Packet * packet);
	void OnShoot(RakNet::RakPeerInterface * peer, RakNet::Packet * packet);
	void OnWeaponSwitch(RakNet::RakPeerInterface * peer, RakNet::Packet * packet);
	void OnWeaponAdd(RakNet::RakPeerInterface * peer, RakNet::Packet * packet);
	void OnRespawn(RakNet::RakPeerInterface * peer, RakNet::Packet * packet);
	std::map<RakNet::RakNetGUID, Player*>* mPlayers;
};