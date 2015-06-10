#include "CNetwork.h"
#include "CLocalPlayer.h"
#include "../shared/structures.h"
#include "../shared/CBitArray.h"
#include "CCore.h"
#include "RakNetTime.h"
#include <fstream>
#include "TCPInterface.h"
#include "../shared/version.h"

#include "FileListTransfer.h"
#include "CFileTransfer.h"
	  
extern CCore *g_CCore;

CNetworkManager::CNetworkManager()
{
	isConnected = false;
	peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(1,&sd, 1);
	GetConnectInfo();
	ValidateIP();

	std::fstream nickname;
	
	/*nickname.open("lhmp/nickname.txt",std::ios::in);
	if(nickname.is_open())
	{
		nickname	>> NickName;
		nickname.close();
	}
	if(strlen(NickName) == 0)
		sprintf(NickName,"Unknown");
	*/
	// Loads values from Windows Registry system (e.g. nicknamestring)
	LoadRegistryConfig();
	//g_CCore->GetLocalPlayer()->SetNickname(NickName);


	transferPlugin = NULL;

}
CNetworkManager::~CNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

RakNet::RakPeerInterface* CNetworkManager::GetPeer()
{
	return peer;
}

bool CNetworkManager::ConnectServer()
{
	if (strlen(CONNECT_PASSWORD) > 0)
		peer->Connect(CONNECT_IP, CONNECT_PORT, CONNECT_PASSWORD, strlen(CONNECT_PASSWORD));
	else
		peer->Connect(CONNECT_IP,CONNECT_PORT,0,0);
	char buffer[255];
	sprintf(buffer,"Connecting %s:%d",CONNECT_IP,CONNECT_PORT);
	g_CCore->GetChat()->AddMessage(buffer);	
	return true;
}

void CNetworkManager::OnConnectionAccepted(RakNet::Packet* packet)
{
	g_CCore->GetChat()->AddMessage("#00d717Connection accepted.");
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_INITLHMP);
	//bsOut.Write(NickName);
	//bsOut.Write(g_CCore->GetGame()->CameraGetFov());
	bsOut.Write(LHMP_VERSION_TEST_HASH);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void	CNetworkManager::OnConnectionIsAboutFinish()
{
	// send him CONNECTION FINAL
	isConnected = true;

	//g_CCore->GetChat()->AddMessage("#ff8600Almost there ...");

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID) ID_CONNECTION_FINISHED);
	bsOut.Write(NickName);
	bsOut.Write(g_CCore->GetGame()->CameraGetFov());
	peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void CNetworkManager::Pulse()
{
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		RakNet::TimeMS TS = NULL;
		int offset = 0;
		if (packet->data[0] == ID_TIMESTAMP)
		{
			// this packet has a timesteam before raw data
			RakNet::BitStream timestamp(packet->data+1, sizeof(RakNet::TimeMS)+1, false);
			timestamp.Read(TS);
			offset = 1 + sizeof(RakNet::TimeMS);
			
		}
		switch (packet->data[offset])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				this->OnConnectionAccepted(packet);
			}
			break;
		case ID_CONNECTION_FINISHED:
		{
				g_CCore->GetChat()->ClearChat();
				g_CCore->GetChat()->AddMessage("#ff8600Lost Heaven Multiplayer started.");
				char version[255];
#if LHMP_VERSION_TYPE == 1
				sprintf(version, "#fec606Version %d.%d", LHMP_VERSION_MAJOR,LHMP_VERSION_MINOR);
				g_CCore->GetChat()->AddMessage(version);
#else

				sprintf(version, "#ff8600Build hash: #e3e3e3%s", LHMP_VERSION_TEST_HASH);
				g_CCore->GetChat()->AddMessage(version);
				sprintf(version, "#ff8600Build time:  #e3e3e3%s (%s)", __DATE__, __TIME__);
				g_CCore->GetChat()->AddMessage(version);
#endif
				//g_CCore->GetChat()->AddMessage("#71ba51Connected to the server.");
				
				RakNet::BitStream bsIn(packet->data+offset, packet->length-offset, false);
				
				_Server svr;
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(svr);
				sprintf(this->m_pServerName, "%s", svr.server_name);
				g_CCore->GetLocalPlayer()->SetID(svr.playerid);

				char mapName[255];
				bsIn.Read(mapName);

				// TODO - change map if it differs

				g_CCore->GetEngineStack()->AddMessage(ES_CAMERAUNLOCK, 0);

				int len = strlen(mapName);
				char* vstup = new char[len + 1];
				strcpy(vstup, mapName);

				g_CCore->GetEngineStack()->AddMessage(ES_CHANGEMAP, (DWORD)vstup);

				// send him CONNECTION FINAL

				/*RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID) ID_CONNECTION_FINISHED);
				bsOut.Write(NickName);
				bsOut.Write(g_CCore->GetGame()->CameraGetFov());
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);*/

			}
			break;
		case ID_GAME_BAD_VERSION:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection aborted, the server is running a different version.");
				break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection aborted, the server is full.");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			g_CCore->GetChat()->AddMessage("#f31d2fDisconnected from the server.");
			break;
		case ID_CONNECTION_LOST:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection with the server lost.");
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection failed, trying to reconnect.");
			ConnectServer();
			break;
		case ID_INVALID_PASSWORD:
			g_CCore->GetChat()->AddMessage("#ff0000Password rejected.");
			break;
		case ID_CONNECTION_BANNED:
			g_CCore->GetChat()->AddMessage("#ff0000Connection refused - we are banned.");
			break;
		case ID_GAME_ALIVE:
		case ID_GAME_SYNC:
			break;
		case ID_GAME_LHMP_PACKET:
			{
				if (this->IsConnected())
					ProceedLHMP(packet,TS);
			}
			break;
		case ID_FILETRANSFER:
		{
			/*char buff[200];
			char foo[3];
			for (int i = 0; i < packet->length; i++)
			{
				sprintf(foo, "%2X", packet->data[i]);
				if (foo[1] == '0')
					foo[0] = '0';
				buff[i * 2] = foo[0];
				buff[1+i * 2] = foo[1];
			}
			buff[packet->length * 2] = 0x0;

			g_CCore->GetChat()->AddMessage(buff);
			*/

								if (packet->length > 2)
								{
									RakNet::BitStream bsIn(packet->data + 1, packet->length - 1, false);
									g_CCore->GetFileTransfer()->HandlePacket(&bsIn);
								}
								else {
									g_CCore->GetChat()->AddMessage("Fuck");
								}
		}
			break;

		case ID_SERVERRELOAD:
		{
			// replay connecting process
			this->isConnected = false;
			g_CCore->GetEngineStack()->AddMessage(ES_SERVERRELOAD, NULL);
			// now just wait what will come next
			g_CCore->GetFileSystem()->Reset();

			g_CCore->GetFileTransfer()->Reset();
			// Delete all current client scripts
			g_CCore->GetSquirrel()->DeleteScripts();
		}
			break;
		case ID_SCRIPSTLIST:
			RakNet::BitStream bsIn(packet->data + offset, packet->length - offset, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			unsigned short count = 0;
			char scriptname[256];
			bsIn.Read(count);
			for (int i = 0; i < count; i++)
			{
				bsIn.Read(scriptname);

				// try to start script
				//g_CCore->GetChat()->AddMessage(scriptname);
				g_CCore->GetSquirrel()->LoadClientScript(scriptname);
				//g_CCore->GetEngineStack()->AddMessage(ES_SCRIPT_RUN,NULL);
			}
			break;
		
		}
	}
}

void CNetworkManager::ProceedLHMP(RakNet::Packet* packet, RakNet::TimeMS timestamp)
{
	//g_CCore->GetChat()->AddMessage("LHMP PACKED arrived");
	//int start = 1;
	//if(packet->data[0] == ID_TIMESTAMP)
	//	start = sizeof(RakNet::Time)+2;
	int offset = 1;
	if (timestamp != NULL)
	{
		//offset += sizeof(RakNet::MessageID);
		offset += sizeof(RakNet::MessageID) + sizeof(RakNet::TimeMS);
	}
	switch(packet->data[offset])
	{
	case LHMP_PLAYER_CHAT_MESSAGE:
		{
			char buff[500];
			RakNet::BitStream bsIn(packet->data+offset+1, packet->length-offset-1, false);
			bsIn.Read(buff);
			g_CCore->GetChat()->AddMessage(buff);
		}
		break;
		case LHMP_PLAYER_SENT_SYNC_ON_FOOT:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			SYNC::ON_FOOT_SYNC syncData;
			bsIn.Read(syncData);
			if (syncData.ID >= 0 && syncData.ID <= 100)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(syncData.ID);
				if(ped == NULL) break;

				if (timestamp - ped->GetTimeStamp() > 0)
				{
					//char buffer[255];
					//sprintf(buffer,"%i",timestamp);
					//g_CCore->GetLog()->AddLog(buffer);

					Vector3D rot = Tools::ComputeOffsetDegrees(syncData.rotation*360/MAX_USHORT);
					ped->SetRotation(rot);
					ped->SetState(syncData.animStatus);
					ped->SetHealth((float)syncData.health);
					
					CBitArray states(syncData.states);

					ped->SetDucking(states.GetBit(ONFOOT_ISDUCKING));
					ped->SetAiming(states.GetBit(ONFOOT_ISAIMING));
					ped->SetCarAnim(states.GetBit(ONFOOT_ISCARANIM));
					ped->SetTimeStamp(timestamp);
					ped->SetPosition(syncData.position);
					ped->SetUpInterpolation();
					ped->UpdateGameObject();
				}
			}
		}
		break;
		case LHMP_PLAYER_SENT_CAR_UPDATE:
		{
			//MessageBox(NULL,"1","omg",MB_OK);
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			SYNC::IN_CAR syncData;
			bsIn.Read(syncData);
			if (syncData.ID >= 0 && syncData.ID <= MAX_VEHICLES)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(syncData.ID);
				if (ped == NULL) break;

				ped->SetHealth(syncData.health);
				ped->SetAiming(syncData.isAiming);
				if (ped->GetEntity() != NULL)
				{
					*(float*)(ped->GetEntity() + 0x5F4) = syncData.aim;
					*(float*)(ped->GetEntity() + 0x644) = syncData.health;

					if (ped->GetCurrentWeapon()!= 0)
						*(byte*)(ped->GetEntity() + 0x1E5) = syncData.isAiming;
					else
						*(byte*)(ped->GetEntity() + 0x1E5) = 0;
				}

			}
			//MessageBox(NULL,"2","omg",MB_OK);
		}
			break;
		case LHMP_PLAYER_CONNECT:
		{
			
			char buff[255];
			int PlayerId,skinId;
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);

			bsIn.Read(PlayerId);
			bsIn.Read(buff); // nickname
			bsIn.Read(skinId); // nickname
			g_CCore->GetPedPool()->New(PlayerId);
			g_CCore->GetLog()->AddLog("LHMP_CONNECTORCREATEPED");
			CPed* ped = g_CCore->GetPedPool()->Return(PlayerId);
			if(ped != 0)
			{
				ped->SetName(buff);
				ped->SetSkinId(skinId);
			}
		}
		break;
		case LHMP_PLAYER_CHANGESKIN:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID,skinID;
			bsIn.Read(ID);
			bsIn.Read(skinID);
			if(ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				g_CCore->GetLocalPlayer()->ourSkin = skinID;
				g_CCore->GetEngineStack()->AddMessage(ES_CHANGESKIN,ID);
			} else 
			{
				CPed* ped = g_CCore->GetPedPool()->Return(ID);
				if(ped == 0) break;
				ped->SetSkinId(skinID);
				g_CCore->GetEngineStack()->AddMessage(ES_CHANGESKIN,ID);
			}	
		}
		break;
		case LHMP_PLAYER_DISCONNECT:
		{
			int ID;
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			bsIn.Read(ID);
			CPed* ped = g_CCore->GetPedPool()->Return(ID);
			if(ped != 0)
			{
				if (ped->GetEntity() != NULL)
				{
					g_CCore->GetEngineStack()->AddMessage(ES_DELETEPLAYER, ped->GetEntity());
				}
				g_CCore->GetPedPool()->Delete(ID);
			}

		}
		break;
		case LHMP_PLAYER_PLAYANIM:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID,animID;
			bsIn.Read(ID);
			bsIn.Read(animID);
			if(ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				g_CCore->GetLocalPlayer()->ourAnim = animID;
				g_CCore->GetEngineStack()->AddMessage(ES_PLAYANIM,ID);
			} else 
			{
				CPed* ped = g_CCore->GetPedPool()->Return(ID);
				if(ped == 0) break;
				ped->SetAnim(animID);
				g_CCore->GetEngineStack()->AddMessage(ES_PLAYANIM,ID);
			}	
		}
		case LHMP_PLAYER_PLAYANIM_STRING:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			char animString[255];
			bsIn.Read(ID);
			bsIn.Read(animString);

			ENGINE_STACK::PLAYER_PLAYANIM* pw = new ENGINE_STACK::PLAYER_PLAYANIM[1];
			pw->ID = ID;
			sprintf(pw->name, "%s", animString);
			g_CCore->GetEngineStack()->AddMessage(ES_PLAYANIM_STRING, (DWORD)pw);
		}
			break;
		case LHMP_PLAYER_PLAYSOUND_STRING:
			{
				RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
				char sound[255];
				bsIn.Read(sound);

				ENGINE_STACK::PLAYER_PLAYSOUND* pw = new ENGINE_STACK::PLAYER_PLAYSOUND[1];
				sprintf(pw->name, "%s", sound);
				g_CCore->GetEngineStack()->AddMessage(ES_PLAYSOUND_STRING, (DWORD)pw);
			}
				break;
		case LHMP_PLAYER_RESPAWN:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			bsIn.Read(ID);
			CPed* ped = g_CCore->GetPedPool()->Return(ID);
			if (ped)
			{
				if (ped->InCar != -1)
				{
					CVehicle* veh = g_CCore->GetVehiclePool()->Return(ped->InCar);
					if (veh != NULL)
					{
						for (int i = 0; i < 4; i++)
						{
							if (veh->GetSeat(i) == ID)
								veh->SetSeat(i, 0);
						}
					}
					ped->InCar = -1;
					veh->PlayerExit(ID);
					ped->SetIsOnFoot(true);
				}
				ped->ClearWeapons();
				g_CCore->GetEngineStack()->AddMessage(ES_CREATEPLAYER, ID);
			}

		}
		break;
		case LHMP_PLAYER_PING:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID,ping;
			bsIn.Read(ID);
			bsIn.Read(ping);
			if(ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				g_CCore->GetLocalPlayer()->SetPing(ping);
			} else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(ID);
				if(ped != NULL)
				{
					ped->SetPing(ping);
				}
			}
		}
		break;
		case LHMP_PLAYER_ADDWEAPON:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			//int ID;
			ENGINE_STACK::PLAYER_ADDWEAPON* pw = new ENGINE_STACK::PLAYER_ADDWEAPON[1];
			bsIn.Read(pw->ID);
			bsIn.Read(pw->wepID);
			bsIn.Read(pw->wepLoaded);
			bsIn.Read(pw->wepHidden);
			if(g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
				if (ped != NULL)
				{
					ped->AddWeapon(pw->wepID, pw->wepLoaded, pw->wepHidden);
					if (ped->GetEntity() == NULL)
					{
						return;
					}
				}
				
			}
			g_CCore->GetEngineStack()->AddMessage(ES_ADDWEAPON,(DWORD)pw);		
			char buff[255];
			sprintf(buff,"[NM]AddWep: %i %i %i",pw->wepID,pw->wepLoaded,pw->wepHidden);
			g_CCore->GetLog()->AddLog(buff);
		}
		break;
		case LHMP_PLAYER_DELETEWEAPON:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			//int ID;
			ENGINE_STACK::PLAYER_DELETEWEAPON* pw = new ENGINE_STACK::PLAYER_DELETEWEAPON[1];
			bsIn.Read(pw->ID);
			bsIn.Read(pw->wepID);
			if(g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
				if(ped != NULL)
					ped->DeleteWeapon(pw->wepID);
			}
			g_CCore->GetEngineStack()->AddMessage(ES_DELETEWEAPON,(DWORD)pw);
			g_CCore->GetLog()->AddLog("[NM] delwep");
		}
		case LHMP_PLAYER_SWITCHWEAPON:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			//int ID;
			ENGINE_STACK::PLAYER_SWITCHWEAPON* pw = new ENGINE_STACK::PLAYER_SWITCHWEAPON[1];
			bsIn.Read(pw->ID);
			bsIn.Read(pw->wepID);
			if(g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
				if(ped != NULL)
					ped->SwitchWeapon(pw->wepID);
			}
			else {
				g_CCore->GetEngineStack()->AddMessage(ES_SWITCHWEAPON, (DWORD)pw);
				g_CCore->GetLog()->AddLog("[NM]switchwep");
			}
		}
		break;
		case LHMP_PLAYER_SHOOT:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			//int ID;
			//Vector3D pos;
			//int ID;
			ENGINE_STACK::PLAYER_SHOOT* pw = new ENGINE_STACK::PLAYER_SHOOT[1];
			bsIn.Read(pw->ID);
			bsIn.Read(pw->pos.x);
			bsIn.Read(pw->pos.y);
			bsIn.Read(pw->pos.z);
			int currentID;
			bsIn.Read(currentID);
			char buff[255];
			sprintf(buff,"[NM] Shot %i %f %f %f %d",pw->ID,pw->pos.x,pw->pos.y, pw->pos.z,currentID);
			if(g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
				if (ped != NULL)
				{
					ped->SetCurrentWeapon(currentID);
					ped->OnShoot();
				}
			}
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetEngineStack()->AddMessage(ES_SHOOT,(DWORD)pw);
			
		}
		break;
		case LHMP_PLAYER_THROWGRANADE:
		{
			Vector3D position;
			int ID;
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			bsIn.Read(ID);
			bsIn.Read(position);

			ENGINE_STACK::PLAYER_SHOOT* pw = new ENGINE_STACK::PLAYER_SHOOT[1];
			pw->ID = ID;
			pw->pos = position;
			g_CCore->GetEngineStack()->AddMessage(ES_THROWGRANADE, (DWORD)pw);

		} 
		break;
		case LHMP_PLAYER_DEATH:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID,reason,part;
			bsIn.Read(ID);
			bsIn.Read(reason);
			bsIn.Read(part);
			char buff[255];
			sprintf(buff, "[NM] PlayerDeath %i", ID);
			g_CCore->GetLog()->AddLog(buff);
			//g_CCore->GetEngineStack()->AddMessage(ES_PLAYERDEATH, (DWORD)ID);

			ENGINE_STACK::KILL_PED_EX* pw = new ENGINE_STACK::KILL_PED_EX[1];
			pw->ID = ID;
			pw->part = part;
			pw->reason = reason;
			g_CCore->GetEngineStack()->AddMessage(ES_PLAYERDEATHEX, (DWORD)pw);

		}
		break;
		case LHMP_PLAYER_DEATH_END:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;// reason, part;
			bsIn.Read(ID);
			char buff[500];
			sprintf(buff, "[NM] PlayerDeathEND %i", ID);
			g_CCore->GetLog()->AddLog(buff);

			g_CCore->GetEngineStack()->AddMessage(ES_PLAYERDEATH_END, ID);

		}
			break;
		case LHMP_PLAYER_PUT_TO_VEHICLE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID,carID,seatID;
			bsIn.Read(ID);
			bsIn.Read(carID);
			bsIn.Read(seatID);
			char buff[255];
			sprintf(buff, "[Nm] PUT TO VEH %d %d %d", ID, carID, seatID);
			g_CCore->GetLog()->AddLog(buff);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(carID);
			if (veh)
			{
				if (ID == g_CCore->GetLocalPlayer()->GetOurID())
				{
					g_CCore->GetLocalPlayer()->IDinCar = carID;
					g_CCore->GetLocalPlayer()->SetIsOnFoot(false);
					veh->PlayerEnter(ID, seatID);
				}
				else
				{
					CPed* ped = g_CCore->GetPedPool()->Return(ID);
					ped->InCar = carID;
					ped->SetIsOnFoot(false);
					veh->PlayerEnter(ID, seatID);
				}
				ENGINE_STACK::PLAYER_ENTER_VEH* data = new ENGINE_STACK::PLAYER_ENTER_VEH[1];
				data->pID = ID;
				data->seatID = seatID;
				data->vehID = carID;
				g_CCore->GetEngineStack()->AddMessage(ES_PLAYER_PUT_TO_VEH, (DWORD)data);
			}

		}
		break;
		case LHMP_PLAYER_SET_HEALTH:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			float health;
			bsIn.Read(ID);
			bsIn.Read(health);
			char buff[255];
			sprintf(buff, "[Nm] SET HEALTH %d %f", ID, health);
			g_CCore->GetLog()->AddLog(buff);

			if (ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				g_CCore->GetLocalPlayer()->SetHealth(health);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(ID);
				ped->SetHealth(health);
			}

		}
		break;
		case LHMP_PLAYER_SET_MONEY:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);

			int money;
			bsIn.Read(money);

			char buff[255];
			sprintf(buff, "[Nm] SET MONEY %d", money);
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetLocalPlayer()->SetMoney(money);
		}
			break;
		case LHMP_PLAYER_ENABLE_MONEY:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);

			int enable;
			bsIn.Read(enable);

			char buff[255];
			sprintf(buff, "[Nm] ENABLE MONEY %d", enable);
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetLocalPlayer()->EnableMoney(enable);
		}
		break;
		case LHMP_PLAYER_SET_NAMETAG:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			bool status;
			bsIn.Read(status);
			char buff[255];
			sprintf(buff, "[Nm] SET NAMETAG %d", status);
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetGraphics()->SetShowNameTags(status);
		}
		break;
		case LHMP_PLAYER_SET_POSITION:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			Vector3D pos;
			bsIn.Read(ID);
			bsIn.Read(pos);
			char buff[255];
			sprintf(buff, "[Nm] SET POSITION %d", ID);
			g_CCore->GetLog()->AddLog(buff);

			if (ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				g_CCore->GetLocalPlayer()->SetLocalPos(pos);
				g_CCore->GetGame()->SetPlayerPosition(g_CCore->GetLocalPlayer()->GetEntity(), pos);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(ID);
				if (ped != NULL)
				{
					ped->SetPosition(pos);
					g_CCore->GetGame()->SetPlayerPosition(ped->GetEntity(), pos);
				}
				
			}

		}
		break;
		case LHMP_PLAYER_SET_ROTATION:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			Vector3D rot;
			bsIn.Read(ID);
			bsIn.Read(rot);
			char buff[255];
			sprintf(buff, "[Nm] SET ROTATION %d", ID);
			g_CCore->GetLog()->AddLog(buff);

			if (ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				g_CCore->GetLocalPlayer()->SetLocalRot(rot);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(ID);
				ped->SetRotation(rot);
			}

		}
		break;
		case LHMP_PLAYER_SET_CAMERA:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			ENGINE_STACK::CAMERA_SET* data = new ENGINE_STACK::CAMERA_SET[1];
			bsIn.Read(data->pos);
			bsIn.Read(data->rot);
			g_CCore->GetEngineStack()->AddMessage(ES_CAMERASETPOS, (DWORD)data);
		}
		break;
		case LHMP_PLAYER_SET_CAMERA_DEFAULT:
		{
			g_CCore->GetEngineStack()->AddMessage(ES_CAMERAUNLOCK,0);
		}
		break;
		case LHMP_PLAYER_SET_CAMERA_SWING:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int state;
			float force;

			bsIn.Read(state);
			bsIn.Read(force);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER SET CAMERA SWING %d %f", state, force);
			g_CCore->GetLog()->AddLog(buff);

			g_CCore->GetGame()->CameraSetSwing((byte)state, force);
			
		}
			break;

		case LHMP_PLAYER_SET_CAMERA_FOV:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			float FOV;

			bsIn.Read(FOV);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER SET CAMERA FOV %f", FOV);
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetGame()->CameraSetFov(FOV);
		}
			break;
		case LHMP_PLAYER_TOGGLE_CITY_MUSIC:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int state;
			bsIn.Read(state);

			char buff[255];
			sprintf(buff, "[Nm] TOGGLE CITY MUSIC %d", state);
			g_CCore->GetLog()->AddLog(buff);

			g_CCore->GetGame()->ToggleCityMusic(state);
	
		}
		break;
		case LHMP_PLAYER_TIMER_ON:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int remain, seconds, minutes, hours;

			bsIn.Read(remain);
			bsIn.Read(seconds);
			bsIn.Read(minutes);
			bsIn.Read(hours);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER TIMER ON %d %d %d %d", remain, seconds, minutes, hours);
			g_CCore->GetLog()->AddLog(buff);

			g_CCore->GetGame()->TimerOn(remain, seconds, minutes, hours);
			
		}
			break;

		case LHMP_PLAYER_TIMER_OFF:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			bsIn.Read(ID);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER TIMER OFF");
			g_CCore->GetLog()->AddLog(buff);

			g_CCore->GetGame()->TimerOff();
			
		}
			break;

		case LHMP_PLAYER_SET_WEATHER_PARAM:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int  param_val;
			char parameter[300];
			bsIn.Read(parameter);
			bsIn.Read(param_val);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER SET WEATHER PARAM %s, %d", parameter, param_val);
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetGame()->WeatherSetParam(parameter, param_val);

		}
			break;

		case LHMP_PLAYER_SET_OBJECTIVE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			char text[900];

			bsIn.Read(text);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER SET  OBJECTIVE %s", text);
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetGame()->SetObjective(text);
		}
			break;

		case LHMP_PLAYER_CLEAR_OBJECTIVE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;

			bsIn.Read(ID);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER CLEAR OBJECTIVE");
			g_CCore->GetLog()->AddLog(buff);
			g_CCore->GetGame()->ClearObjective();
		}
			break;

		case LHMP_PLAYER_ADD_CONSOLE_TEXT:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			char color[50];
			char text[900];

			bsIn.Read(color);
			bsIn.Read(text);

			DWORD color_ex = Tools::GetARGBFromString(color);

			char buff[255];
			sprintf(buff, "[Nm] PLAYER ADD CONSOLE TEXT %X, %s", color_ex, text);
			g_CCore->GetLog()->AddLog(buff);

			g_CCore->GetGame()->ConsoleAddText(text, color_ex);

		}
			break;
		case LHMP_PLAYER_LOCK_CONTROLS:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			unsigned int status;
			bsIn.Read(status);

			g_CCore->GetGame()->SetLockControls(status == 1);
			//g_CCore->GetGame()->UpdateControls();

		}
			break;
		case LHMP_VEHICLE_CREATE:
		{
			//g_CCore->GetChat()->AddMessage("VEHICLE Arrived");
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			VEH::CREATE vehicle;
			bsIn.Read(vehicle);
			g_CCore->GetVehiclePool()->New(vehicle.ID, vehicle.skinID, vehicle.position,vehicle.rotation, vehicle.isSpawned);
			char buff[255];            
			sprintf(buff, "LHMP_VEHICLE_CREATE %d %d %d %d %d %d", vehicle.ID, vehicle.seat[0], vehicle.seat[1], vehicle.seat[2], vehicle.seat[3], vehicle.siren);

			g_CCore->GetLog()->AddLog(buff);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehicle.ID);
			if (veh == NULL)
			{
				g_CCore->GetLog()->AddLog("daco sa dokurvilo, vozidlo neexistuje");
			}
			else
			{
				veh->SetActive(vehicle.isSpawned);
				veh->SetPosition(vehicle.position);
				veh->SetRotation(vehicle.rotation);
				veh->SetTimeStamp(timestamp);
				veh->SetUpInterpolation();
				veh->SetDamage(vehicle.damage);
				veh->ToggleRoof(vehicle.roofState);
				veh->SetSirenState(vehicle.siren);
				for (int i = 0; i < 4; i++)
					veh->SetSeat(i, vehicle.seat[i]);
				g_CCore->GetLog()->AddLog("LHMP_VEHICLE_CREATE");
			}
		}
		break;
		case LHMP_VEHICLE_TOGGLE_ENGINE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			BYTE state;

			bsIn.Read(ID);
			bsIn.Read(state);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);

			if (veh != NULL && g_CCore->GetLocalPlayer()->GetOurID() != veh->GetSeat(0))
			{
				veh->ToggleEngine(state);
				char buff[255];
				sprintf(buff, "[Nm] TOGGLE ENGINE %d STATE: %d", ID, state);
				g_CCore->GetLog()->AddLog(buff);
			}
		}
			break;

		case LHMP_VEHICLE_TOGGLE_ROOF:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			int state;
			bsIn.Read(ID);
			bsIn.Read(state);

			char buff[255];
			sprintf(buff, "[Nm] TOGGLE ROOF %d STATE: %d", ID, state);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				veh->ToggleRoof((byte)state);
			}
		}
		break;

		case LHMP_VEHICLE_TOGGLE_SIREN:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			int state;
			bsIn.Read(ID);
			bsIn.Read(state);

			char buff[255];
			sprintf(buff, "[Nm] TOGGLE SIREN %d STATE: %d", ID, state);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				veh->SetSirenState((state == 1));
			}
		}
			break;

		case LHMP_VEHICLE_SET_FUEL:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			float fuel;
			bsIn.Read(ID);
			bsIn.Read(fuel);

			char buff[255];
			sprintf(buff, "[Nm] SET FUEL %f", fuel);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				veh->SetFuel(fuel);
			}
		}
			break;
		case LHMP_PLAYER_ENTERED_VEHICLE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int Id, vehId, seatId;
			bsIn.Read(Id);
			bsIn.Read(vehId);
			bsIn.Read(seatId);
			char buff[255];
			sprintf(buff, "ENTER VEHICLE %d %d %d", Id, vehId, seatId);
			g_CCore->GetLog()->AddLog(buff);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
			if (veh != NULL)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(Id);
				if (ped != NULL)
				{
					ped->InCar = vehId;
					veh->PlayerEnter(Id, seatId);
					ENGINE_STACK::PLAYER_ENTER_VEH* data = new ENGINE_STACK::PLAYER_ENTER_VEH[1];
					data->pID = Id;
					data->seatID = seatId;
					data->vehID = vehId;
					g_CCore->GetEngineStack()->AddMessage(ES_PLAYER_ENTER_VEH, (DWORD)data);
					ped->SetIsOnFoot(false);
				}
			}

		}
		break;
		case LHMP_PLAYER_EXIT_VEHICLE:
		{
			//g_CCore->GetChat()->AddMessage("[NM] Exit veh");
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int Id, vehId;
			bsIn.Read(Id);
			bsIn.Read(vehId);
			char buff[255];
			sprintf(buff, "EXIT VEHICLE %d %d", Id, vehId);
			g_CCore->GetLog()->AddLog(buff);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
			CPed* ped = g_CCore->GetPedPool()->Return(Id);
			if (veh && ped)
			{
				ped->InCar = -1;
				veh->PlayerExit(Id);
				ENGINE_STACK::PLAYER_ENTER_VEH* data = new ENGINE_STACK::PLAYER_ENTER_VEH[1];
				data->pID = Id;
				data->vehID = vehId;
				g_CCore->GetEngineStack()->AddMessage(ES_PLAYER_EXIT_VEH, (DWORD)data);
				ped->SetIsOnFoot(true);
			}

		}
		break;
		case LHMP_PLAYER_EXIT_VEHICLE_FINISH:
		{
			//g_CCore->GetChat()->AddMessage("[NM] Exit veh");
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int Id;
			bsIn.Read(Id);
			CPed* ped = g_CCore->GetPedPool()->Return(Id);
			if (ped != NULL)
			{
				if (ped->GetEntity() != NULL)
				{
					*(byte*)(ped->GetEntity() + 0x75) = 0;
				}
				if (ped->InCar != -1)
				{
					CVehicle* veh = g_CCore->GetVehiclePool()->Return(ped->InCar);
					if (veh != NULL)
					{
						veh->PlayerExit(Id);
						ped->InCar = -1;
						ped->SetIsOnFoot(true);
					}
				}
			}
		}
			break;
		case LHMP_VEHICLE_SYNC:
		{
								 
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			VEH::SYNC syncData;
			bsIn.Read(syncData);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(syncData.ID);
			if (veh != NULL)
			{
				if (timestamp - veh->GetTimeStamp() > 0)
				{
					
					//veh->SetVehiclePosition(syncData.position);
					veh->SetRotation(syncData.rotation);
					veh->SetWheelsAngle(syncData.wheels);
					veh->SetSpeed(syncData.speed);
					veh->SetHornState(syncData.horn);
					veh->SetTimeStamp(timestamp);
					veh->SetSecondRot(syncData.secondRot);
					veh->SetPosition(syncData.position);
					veh->SetIsOnGas(syncData.gasOn);
					veh->SetUpInterpolation();
				}
			}


		}
			break;
		case LHMP_VEHICLE_JACK:
		{
			//g_CCore->GetChat()->AddMessage("[NM] Exit veh");
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int Id, vehId, seatId;
			bsIn.Read(Id);
			bsIn.Read(vehId);
			bsIn.Read(seatId);

			char buff[255];
			sprintf(buff, "CAR JACK %d %d %d", Id, vehId, seatId);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
			if (veh)
			{
				int jackedID = veh->GetSeat(seatId);
				veh->PlayerExit(jackedID);
				if (jackedID != g_CCore->GetLocalPlayer()->GetOurID())
				{
					CPed* pedJacked = g_CCore->GetPedPool()->Return(jackedID);
					if (pedJacked != NULL)
					{
						pedJacked->InCar = -1;
						pedJacked->SetIsOnFoot(true);
					}
				}
				else
				{
					g_CCore->GetLocalPlayer()->IDinCar = -1;
					g_CCore->GetLocalPlayer()->SetIsOnFoot(true);

				}
				veh->PlayerExit(veh->GetSeat(seatId));
				ENGINE_STACK::VEH_JACK* data = new ENGINE_STACK::VEH_JACK[1];
				data->pID = Id;
				data->vehID = vehId;
				data->seatID = seatId;
				g_CCore->GetEngineStack()->AddMessage(ES_VEHICLE_JACK, (DWORD)data);
			}
		}
			break;
		case LHMP_VEHICLE_DELETE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int Id;
			bsIn.Read(Id);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(Id);
			if (veh != NULL)
			{
				ENGINE_STACK::VEH_DELETEVEH* data = new ENGINE_STACK::VEH_DELETEVEH[1];
				data->vehID = Id;
				data->base = veh->GetEntity();
				//data->
				g_CCore->GetEngineStack()->AddMessage(ES_DELETECAR, (DWORD)data);
				g_CCore->GetVehiclePool()->Delete(Id);
			}

		}
			break;
		case LHMP_VEHICLE_DAMAGE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int vehId;
			float damage;
			bsIn.Read(vehId);
			bsIn.Read(damage);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
			if (veh != NULL)
			{
				veh->SetDamage(damage);
			}
		}
			break;
		case LHMP_VEHICLE_SHOTDAMAGE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int vehId;
			byte damage;
			bsIn.Read(vehId);
			bsIn.Read(damage);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
			if (veh != NULL)
			{
				veh->SetShotDamage(damage);
			}
		}
			break;
		case LHMP_VEHICLE_SET_POSITION:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			Vector3D pos;
			bsIn.Read(ID);
			bsIn.Read(pos);
			char buff[255];
			sprintf(buff, "[Nm] SET POSITION %d", ID);
			g_CCore->GetLog()->AddLog(buff);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				if (veh->GetSeat(0) == g_CCore->GetLocalPlayer()->GetOurID())
				{
					veh->SetPosition(pos);
					*(float*)(veh->GetEntity() + 0x40C) = pos.x;
					*(float*)(veh->GetEntity() + 0x410) = pos.y;
					*(float*)(veh->GetEntity() + 0x414) = pos.z;
				}
			}

		}
			break;
		case LHMP_VEHICLE_SET_ROTATION:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			Vector3D rot;
			bsIn.Read(ID);
			bsIn.Read(rot);
			char buff[255];
			sprintf(buff, "[Nm] SET ROTATION %d", ID);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				if (veh->GetSeat(0) == g_CCore->GetLocalPlayer()->GetOurID())
				{
					veh->SetRotation(rot);
					*(float*)(veh->GetEntity() + 0xD28) = rot.x;
					*(float*)(veh->GetEntity() + 0xD2C) = rot.y;
					*(float*)(veh->GetEntity() + 0xD30) = rot.z;
				}
			}
		}
			break;
		case LHMP_VEHICLE_SET_SPEED:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			Vector3D speed;
			bsIn.Read(ID);
			bsIn.Read(speed);
			char buff[255];
			sprintf(buff, "[Nm] SET SPEED %d", ID);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				if (veh->GetSeat(0) == g_CCore->GetLocalPlayer()->GetOurID())
				{
					veh->SetSpeed(speed);
				}
			}
		}
			break;
		case LHMP_VEHICLE_ON_EXPLODED:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			bsIn.Read(ID);
			char buff[255];
			sprintf(buff, "[Nm] On Exploded %d", ID);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				g_CCore->GetEngineStack()->AddMessage(ES_CAREXPLODE, ID);
				veh->SetActive(false);
			}
		}
			break;
		case LHMP_VEHICLE_RESPAWN:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			bsIn.Read(ID);
			Vector3D pos, rot;
			bsIn.Read(pos);
			bsIn.Read(rot);


			char buff[255];
			sprintf(buff, "[Nm] On Car Respawn %d", ID);
			g_CCore->GetLog()->AddLog(buff);

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh != NULL)
			{
				veh->SetPosition(pos);
				veh->SetRotation(rot);

				veh->SetDamage(100.0f);
				veh->SetShotDamage(10);
				veh->SetActive(true);
				g_CCore->GetEngineStack()->AddMessage(ES_CARRESPAWN, ID);
			}
		}
			break;

		case LHMP_DOOR_SET_STATE:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			char name[200];
			//Vector3D speed;
			bool state,facing;
			bsIn.Read(name);
			bsIn.Read(state);
			bsIn.Read(facing);

			char buff[250];
			sprintf(buff, "[Nm] SET DOOR '%s'[%d] STATE %d %d",name,strlen(name),state,facing);
			g_CCore->GetLog()->AddLog(buff);
			if (strlen(name) > 0)
			{

				ENGINE_STACK::DOOR_SET_STATE* data = new ENGINE_STACK::DOOR_SET_STATE[1];
				sprintf(data->buff, name);
				data->state = state;
				data->facing = facing;
				g_CCore->GetEngineStack()->AddMessage(ES_DOOR_SET_STATE, (DWORD)data);
			}
			
		}
			break;
		case LHMP_SCRIPT_CHANGE_MAP:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			char name[200];
			bsIn.Read(name);

			sprintf(g_CCore->GetGame()->mapName, name);
			char buff[250];
			sprintf(buff, "[Nm] SET map %s", name);
			g_CCore->GetLog()->AddLog(buff);
			//g_CCore->GetChat()->A
			//g_CCore->GetEngineStack()->AddMessage(ES_CHANGEMAP, (DWORD)name);

		}
		break;
		case LHMP_FILE_SEND:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			FileList* listFile = new FileList();
			listFile->Deserialize(&bsIn);
			g_CCore->GetChat()->AddMessage("FileList arrived");
			char buff[200];
			sprintf(buff,"List size: %u",listFile->fileList.Size());
			g_CCore->GetChat()->AddMessage(buff);
			listFile->WriteDataToDisk("lhmp/files/");

		}
			break;

		case LHMP_SCRIPT_CALLFUNC:
		{
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			char script_name[500];
			char func_name[500];

			bsIn.Read(script_name);
			bsIn.Read(func_name);
			g_CCore->GetSquirrel()->callClientFunc(script_name, func_name, &bsIn);
		}
			break;

		case LHMP_PICKUP_CREATE:
		{

								   //g_CCore->GetChat()->AddMessage("pickup lol");
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			char model[250];
			bool isVisible;
			Vector3D position;
			float size;
			bsIn.Read(ID);
			bsIn.Read(model);
			bsIn.Read(position);
			bsIn.Read(size);
			bsIn.Read(isVisible);
			g_CCore->GetPickupPool()->New(ID, model, position, size, isVisible);
			g_CCore->GetEngineStack()->AddMessage(ES_CREATEPICKUP, (DWORD)ID);
			

		}
			break;
		case LHMP_PICKUP_DELETE:
		{
			//g_CCore->GetChat()->AddMessage("pickup lol");
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			bsIn.Read(ID);
			CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
			if (pickup)
			{
				g_CCore->GetEngineStack()->AddMessage(ES_DELETEPICKUP, (DWORD)ID);
			}
		}
			break;
		case LHMP_PICKUP_SETVISIBLE:
		{
			//g_CCore->GetChat()->AddMessage("pickup lol");
			//g_CCore->GetChat()->AddMessage("pickup visible network");
			RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
			int ID;
			bool shouldBeVisible;
			bsIn.Read(ID);
			bsIn.Read(shouldBeVisible);
			CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
			if (pickup)
			{
				pickup->SetVisible(shouldBeVisible);
				g_CCore->GetEngineStack()->AddMessage(ES_SETPICKUPVISIBLE, (DWORD)ID);
				
			}
		}
			break;
	}
}

void CNetworkManager::SendServerMessage(RakNet::BitStream* bsOut,PacketPriority PP = MEDIUM_PRIORITY,PacketReliability PR = RELIABLE_ORDERED)
{
	peer->Send(bsOut,PP,PR,0,RakNet::UNASSIGNED_RAKNET_GUID,true);
}

void CNetworkManager::GetConnectInfo()
{
	//char commandLine[255] = "\"C:\\nejaky string\" 50.50.50.50 127";
	//MessageBoxA(NULL,commandLine,"-1",MB_OK);
	char* commandLine	= GetCommandLine();
	LPCSTR pch;
	pch=strrchr(commandLine,'"');
	if(pch == NULL)
	{
		pch = strtok(commandLine," ");
		pch = strtok(NULL," ");
	} else {
		char buffer[255];
		pch = strtok(commandLine,"\"");
		//MessageBoxA(NULL,pch,"1",MB_OK);
		pch = strtok(NULL,"\"");
		//MessageBoxA(NULL,pch,"2",MB_OK);
		sprintf(buffer,"%s",pch);
		pch = strtok(buffer," ");
		//MessageBoxA(NULL,pch,"3",MB_OK);
	}

	// is there any IP as argument
	if(pch != NULL)
	{
		sprintf(CONNECT_IP,"%s",pch);
		pch = strtok(NULL," ");
		// is there any PORT as argument
		if(pch != NULL)
		{
			CONNECT_PORT = atoi(pch);
			pch = strtok(NULL, " ");
			if (pch != NULL)
			{
				// password argument
				sprintf(CONNECT_PASSWORD, "%s", pch);
			}
			else {
				sprintf(CONNECT_PASSWORD, "%s", "");
			}
		} else
		{
			CONNECT_PORT = 27015;
		}
	} else
	{
		// if there aren't any arguments, use default ones
		sprintf(CONNECT_IP,"%s","127.0.0.1");
		CONNECT_PORT = 27015;
	}
}

void CNetworkManager::ValidateIP()
{
	char buffer[255];
	sprintf(buffer,"%s",CONNECT_IP);
	if(strlen(buffer) < 7)
	{
		sprintf(CONNECT_IP,"%s","127.0.0.1");
	} else 
	{
		int countNum = 0;
		int countDot = 0;
		for(int i = 0; i < (int) strlen(buffer);i++)
		{
			if(buffer[i] == '.')
			{
				if(countNum == 0)
				{
					sprintf(CONNECT_IP,"%s","127.0.0.1");
					break;
				} else
				{
					countNum = 0;
					countDot++;
				}
			}
			else if(isdigit(buffer[i]))
			{
				countNum++;
			}
			else
			{
				sprintf(CONNECT_IP,"%s","127.0.0.1");
				break;
			}
		}
	}
}


char* CNetworkManager::GetNick()
{
	return NickName;
}

RakNet::RakPeerInterface* CNetworkManager::ReturnPeer()
{
	return peer;
}

bool CNetworkManager::IsConnected()
{
	return this->isConnected;
}

char*	CNetworkManager::GetServerName()
{
	return this->m_pServerName;
}
char* GetData(unsigned char* input, int size)
{
	char* newpole = new char[size + 1];
	memcpy(newpole, input, size);
	newpole[size] = 0x0;

	char* pointer = strstr(newpole, "\r\n\r\n");

	char* output = NULL;
	if (pointer != NULL)
	{
		//g_CCore->GetChat()->AddMessage("OK");
		pointer += 4;
		int len = size-(pointer - newpole);
		char* output = new char[len+1];
		memcpy(output, pointer, len);
		output[len] = 0x0;

		/*char ale[100];
		sprintf(ale, "%d %x", len, output);
		g_CCore->GetChat()->AddMessage(ale);*/
		return output;
	}
	else{
		//g_CCore->GetChat()->AddMessage("BAD");
		output = NULL;
	}
	//delete [] newpole;
	return output;

}
void httpRequestSlave(LPVOID pArgs)
{
	sHttpRequest* request = (sHttpRequest*)pArgs;
	RakNet::TCPInterface TCP;
	if (TCP.Start(0, 0) != false)
	{
		RakNet::SystemAddress server;
		char hostname[100];
		char rest[300];
		Tools::AnalyseUrl(request->buff, hostname, rest);
		server = TCP.Connect(hostname, 80, true);
		if (server != RakNet::UNASSIGNED_SYSTEM_ADDRESS)			// if we are connected
		{
			char postRequest[2048] = "";
			char data[1024] = "";
			//sprintf_s(data, "name=%s&pt=%d&ms=%d&mod=%s", svrname.c_str(), port, maxplayers, "");
			sprintf_s(postRequest,
				"GET %s HTTP/1.0\r\n"
				"Content-Type: application/x-www-form-urlencoded\r\n"
				"Content-Length: %d\r\n"
				//"Accept-Encoding: identity\r\n"
				"Host: %s\r\n\r\n", rest, strlen(data), hostname);
			//"%s\r\n", strlen(data), data);
			TCP.Send(postRequest, strlen(postRequest), server, false);
			RakNet::TimeMS StartTime = RakNet::GetTimeMS();
			while (1)
			{
				RakNet::Packet *packet = TCP.Receive();
				if (packet)
				{
				
					char* output = GetData(packet->data, packet->length);
					/*if (output == NULL)
					{
					} else {
						callback CB = (callback)request->callback;
						CB(output);
					}*/
					callback CB = (callback)request->callback;
					CB(output);
		
					break;
				}
				if (RakNet::GetTimeMS() - StartTime > 5000)
				{
					//g_CCore->GetChat()->AddMessage("Request failed");
					break;
				}
				Sleep(100);
			}
		}
		else
		{
			//g_CCore->GetChat()->AddMessage("Connection failed");
		}
	}
}
void CNetworkManager::httpRequest(int type, char* url, void* callback)
{
	sHttpRequest* request = new sHttpRequest();
	request->type = type;
	strcpy(request->buff, url);
	request->callback = callback;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&httpRequestSlave,request, 0, 0);
}


void CNetworkManager::LoadRegistryConfig()
{
	// load nickname
	HKEY key;
	HRESULT res = RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Lost Heaven Multiplayer\\Launcher"), &key);
	if (res != ERROR_SUCCESS)
	{
		strcpy(this->NickName, "UnknownPlayer");
	}
	else {
		DWORD size = 256;
		char nickname[256];
		DWORD type = REG_SZ;
		if (RegQueryValueEx(key, "nickname", NULL, &type, (LPBYTE)&nickname, &size) == ERROR_SUCCESS)
		{
			strcpy(this->NickName, nickname);
		}
		else {
			strcpy(this->NickName, "UnknownPlayer");
		}
		RegCloseKey(key);
	}

}