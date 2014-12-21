#include "CCore.h"
#include "CEngineStack.h"
//#include "structures.h"
#include "../shared/structures.h"
extern CCore *g_CCore;

CEngineStack::CEngineStack()
{
	messageCount = 0;
	start = 0;
	end = 0;
}
CEngineStack::~CEngineStack()
{

}

void CEngineStack::AddMessage(unsigned int messageId,DWORD data)
{
	EngineStackMessage* newMessage = new EngineStackMessage[1];
	newMessage->messageId = messageId;
	newMessage->data = data;
	newMessage->next = 0;
	if(end == 0)
	{
		start = newMessage;
		end = newMessage;
	} else {
		end->next = newMessage;
		end = newMessage;
	}
}

void CEngineStack::DoMessage()
{
	if (start != 0 && g_CCore->m_bIsRespawning == false)
	{
		switch(start->messageId)
		{
		case CLIENT_ENGINESTACK::ES_SERVERRELOAD:
		{
			// delete all cars
			for (unsigned int i = 0; i < MAX_VEHICLES; i++)
			{
				CVehicle* veh = g_CCore->GetVehiclePool()->Return(i);
				if (veh)
				{
					if (veh->GetEntity())
					{
						g_CCore->GetGame()->DeleteCar(veh->GetEntity());
					}
					g_CCore->GetVehiclePool()->Delete(i);
				}
			}

			// delete all peds
			for (unsigned int i = 0; i < MAX_PLAYERS; i++)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(i);
				if (ped)
				{
					if (ped->GetEntity())
					{
						g_CCore->GetGame()->DeletePed(ped->GetEntity());
					}
					g_CCore->GetPedPool()->Delete(i);
				}
			}

			// delete pickups
			for (unsigned int i = 0; i < MAX_PICKUPS; i++)
			{
				CPickup* pi = g_CCore->GetPickupPool()->Return(i);
				if (pi)
				{
					if (pi->GetEntity())
					{
						//g_CCore->GetGame()->frame
					}
					g_CCore->GetPedPool()->Delete(i);
				}
			}
		}
		break;
		case CLIENT_ENGINESTACK::ES_CREATEPLAYER:
			{
				CPed* ped = g_CCore->GetPedPool()->Return(start->data);
				if(ped != NULL)
				{
					if(ped->GetEntity() != NULL)
					{
						
						g_CCore->GetGame()->DeletePed(ped->GetEntity());
						ped->SetEntity(NULL);
					}
					DWORD handle = g_CCore->GetGame()->CreatePED();
					ped->SetEntity(handle);
					
					if (handle != NULL)
					{
						g_CCore->GetGame()->ChangeSkin(ped->GetEntity(),ped->GetSkin());
						for (int i = 0; i < 8; i++)
						{
							SWeapon* wep = ped->GetWeapon(i);
							if (wep->wepID != NULL)
								g_CCore->GetGame()->AddWeapon(ped->GetEntity(), wep->wepID, wep->wepLoaded, wep->wepHidden,0);
						}
					}
					if (ped->InCar != -1)
					{
						CVehicle* veh = g_CCore->GetVehiclePool()->Return(ped->InCar);
						if (veh != NULL)
						{
							g_CCore->GetGame()->GivePlayerToCarFast(ped->GetEntity(), ped->InCar, veh->GetPlayerSeat(start->data));
						}
					}
				}
			}
			break;
		case CLIENT_ENGINESTACK::ES_DELETEPLAYER:
		{
			g_CCore->GetGame()->DeletePed(start->data);
		}
			break;
		case CLIENT_ENGINESTACK::ES_CHANGESKIN:
			{
				if(start->data == g_CCore->GetLocalPlayer()->GetOurID())
				{
					DWORD adr = *(DWORD*) (*(DWORD*)(0x006F9464)+0xE4);
					if (adr != NULL)
					{
						g_CCore->GetGame()->ChangeSkin(adr, g_CCore->GetLocalPlayer()->ourSkin);
						//g_CCore->GetChat()->AddMessage("ChangeSkin reached 3");
					}
				} else
				{	
					CPed* ped = g_CCore->GetPedPool()->Return(start->data);
					if(ped != 0)
					{
						if(ped->GetEntity() != 0)
						{
							/*char buffer[255];
							sprintf(buffer,"SkinID %i",ped->GetSkin());
							g_CCore->GetChat()->AddMessage(buffer);
							*/
							g_CCore->GetGame()->ChangeSkin(ped->GetEntity(), ped->GetSkin());
						}
					}
				}
			}
			break;
		case CLIENT_ENGINESTACK::ES_PLAYANIM:
			{
				if(start->data == g_CCore->GetLocalPlayer()->GetOurID())
				{
					DWORD adr = *(DWORD*) (*(DWORD*)(0x006F9464)+0xE4);
					g_CCore->GetGame()->PlayAnim(adr,g_CCore->GetLocalPlayer()->ourAnim);
				} else
				{	
					CPed* ped = g_CCore->GetPedPool()->Return(start->data);
					if(ped != 0)
					{
						if (ped->GetEntity() != 0)
						{
							g_CCore->GetGame()->PlayAnim(ped->GetEntity(), ped->GetAnim());
						}
					}
				}
			}
			break;
		case CLIENT_ENGINESTACK::ES_PLAYANIM_STRING:
		{
			ENGINE_STACK::PLAYER_PLAYANIM* pw = (ENGINE_STACK::PLAYER_PLAYANIM*) start->data;
			if (pw->ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				DWORD adr = *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
				g_CCore->GetGame()->PlayAnimString(adr, pw->name);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
				if (ped != 0)
				{
					if (ped->GetEntity() != 0)
					{
						g_CCore->GetGame()->PlayAnimString(ped->GetEntity(), pw->name);
					}
				}
			}
		}
		break;
		case CLIENT_ENGINESTACK::ES_PLAYSOUND_STRING:
		{
			ENGINE_STACK::PLAYER_PLAYSOUND* pw = (ENGINE_STACK::PLAYER_PLAYSOUND*) start->data;
			//char test[] = "sounds\\15020060.wav";
			g_CCore->GetGame()->PlayGameSound(pw->name);
		}
		break;
		case CLIENT_ENGINESTACK::ES_ADDWEAPON:
			{
				ENGINE_STACK::PLAYER_ADDWEAPON* pw = (ENGINE_STACK::PLAYER_ADDWEAPON*) start->data;
				if(pw->ID == g_CCore->GetLocalPlayer()->GetOurID())
				{
					DWORD adr = *(DWORD*) (*(DWORD*)(0x006F9464)+0xE4);
					if (g_CCore->GetLocalPlayer()->HasWeapon(pw->wepID))
					{
						if (Tools::isShootingArm(pw->wepID))
						{
							g_CCore->GetLocalPlayer()->SetWeapon(pw->wepID, pw->wepLoaded, pw->wepHidden);
							g_CCore->GetLog()->AddLog("SetWeapon lol");
						}
						else
						{
							g_CCore->GetGame()->AddWeapon(adr, pw->wepID, pw->wepLoaded, pw->wepHidden, 0);
						}
					}
					else {
						g_CCore->GetGame()->AddWeapon(adr, pw->wepID, pw->wepLoaded, pw->wepHidden, 0);
					}
				} else
				{	
					CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
					if(ped != 0)
					{
						if (ped->GetEntity() != 0)
						{
							g_CCore->GetGame()->AddWeapon(ped->GetEntity(), pw->wepID, pw->wepLoaded, pw->wepHidden, 0);
						}
					}
				}
			}
		break;
		case CLIENT_ENGINESTACK::ES_DELETEWEAPON:
			{
				ENGINE_STACK::PLAYER_DELETEWEAPON* pw = (ENGINE_STACK::PLAYER_DELETEWEAPON*) start->data;
				if(pw->ID == g_CCore->GetLocalPlayer()->GetOurID())
				{
					DWORD adr = *(DWORD*) (*(DWORD*)(0x006F9464)+0xE4);
					g_CCore->GetGame()->DeleteWeapon(adr,pw->wepID);
				} else
				{	
					CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
					if(ped != 0)
					{
						if (ped->GetEntity() != 0)
						{
							g_CCore->GetGame()->DeleteWeapon(ped->GetEntity(), pw->wepID);
						}
					}
				}
			}
		break;
		case CLIENT_ENGINESTACK::ES_SWITCHWEAPON:
			{
				ENGINE_STACK::PLAYER_SWITCHWEAPON* pw = (ENGINE_STACK::PLAYER_SWITCHWEAPON*) start->data;
				if(pw->ID == g_CCore->GetLocalPlayer()->GetOurID())
				{
					DWORD adr = *(DWORD*) (*(DWORD*)(0x006F9464)+0xE4);
					g_CCore->GetGame()->SwitchWeapon(adr,pw->wepID);
				} else
				{	
					CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
					if(ped != 0)
					{
						if (ped->GetEntity() != 0)
						{
							g_CCore->GetGame()->SwitchWeapon(ped->GetEntity(), pw->wepID);
						}
					}
				}
			}
		break;
		case CLIENT_ENGINESTACK::ES_SHOOT:
			{
				ENGINE_STACK::PLAYER_SHOOT* pw = (ENGINE_STACK::PLAYER_SHOOT*) start->data;
				if(pw->ID == g_CCore->GetLocalPlayer()->GetOurID())
				{
					DWORD adr = *(DWORD*) (*(DWORD*)(0x006F9464)+0xE4);
					g_CCore->GetGame()->Shoot(adr,pw->pos);
				} else
				{	
					CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
					if(ped != 0)
					{
						if (ped->GetEntity() != 0)
						{
							g_CCore->GetGame()->Shoot(ped->GetEntity(), pw->pos);
						}
					}
				}
			}
		break;
		case CLIENT_ENGINESTACK::ES_THROWGRANADE:
		{
			g_CCore->GetLog()->AddLog("ThrowGranade");
			ENGINE_STACK::PLAYER_SHOOT* pw = (ENGINE_STACK::PLAYER_SHOOT*) start->data;

			if (pw->ID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				// nothing
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
				if (ped != 0)
				{
					if (ped->GetEntity() != 0)
					{
						g_CCore->GetLog()->AddLog("ThrowGranade PED");
						g_CCore->GetGame()->ThrowGranade(ped->GetEntity(), pw->pos);
						ped->OnThrowGranade();
					}
				}
			}
		}
		break;
		case CLIENT_ENGINESTACK::ES_PLAYERDEATH:
		{
			if (start->data == g_CCore->GetLocalPlayer()->GetOurID())
			{
				DWORD adr = *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
				g_CCore->GetGame()->KillPed(adr);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(start->data);
				if (ped != 0)
				{
					if (ped->GetEntity() != 0)
					{
						g_CCore->GetGame()->KillPed(ped->GetEntity());
					}
				}
			}
		}
			break;
		case CLIENT_ENGINESTACK::ES_CAMERASETPOS:
		{
			ENGINE_STACK::CAMERA_SET* pw = (ENGINE_STACK::CAMERA_SET*) start->data;
			g_CCore->GetGame()->SetCameraPos(pw->pos, pw->rot.x, pw->rot.y, pw->rot.z,0);
		}
			break;
		case CLIENT_ENGINESTACK::ES_CAMERAUNLOCK:
		{
			g_CCore->GetGame()->CameraUnlock();
		}
		break;
		// vehicles
		case CLIENT_ENGINESTACK::ES_CREATECAR:
		{
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(start->data);
			if (veh == NULL)
			{
				g_CCore->GetLog()->AddLog("no dpc");
			}
			else
			{
				g_CCore->GetLog()->AddLog("ES CREATECAR");
				DWORD base = g_CCore->GetGame()->CreateCar(veh->GetSkin(),veh->GetPosition(),veh->GetRotation());
				veh->SetEntity(base);
				for (int i = 0; i < 4; i++)
				{
					if (veh->GetSeat(i) != -1)
					{
						CPed* ped = g_CCore->GetPedPool()->Return(veh->GetSeat(i));
						if (ped != NULL)
						{
							if (ped->GetEntity() != NULL)
							{
								g_CCore->GetGame()->GivePlayerToCarFast(ped->GetEntity(), start->data, i);
							}
							else
							{
								// shit
								g_CCore->GetLog()->AddLog("ES[CreateCar] - no PED entity");
							}
							ped->InCar = start->data;
						}
					}
				}
				veh->SetDamage(veh->GetDamage());
				veh->SetShotDamage(veh->GetShotDamage());
				veh->ToggleRoof(veh->GetRoofState());
				veh->SetSirenState(veh->GetSirenState());
				/*for (int i = 0; i < 4; i++)
				{
					if (veh->seea)
				}*/
				//g_CCore->GetGame()->SetCarPosition(veh->GetEntity(), veh->GetPosition());
				//g_CCore->GetGame()->SetCarRotation(veh->GetEntity(), veh->GetRotation());
			}
		}
		break;
		case CLIENT_ENGINESTACK::ES_DELETECAR:
		{
			g_CCore->GetLog()->AddLog("ES_DELETECAR");
			ENGINE_STACK::VEH_DELETEVEH* pw = (ENGINE_STACK::VEH_DELETEVEH*) start->data;
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				CPed* ped = g_CCore->GetPedPool()->Return(i);
				if (ped != NULL)
				{
					if (ped->GetEntity() != NULL)
					{
						if (ped->InCar == pw->vehID)
						{
							g_CCore->GetGame()->KickPlayerFromCarFast(ped->GetEntity());
							ped->InCar = -1;
							ped->SetIsOnFoot(true);
						}
					}
				}
			}
			if (g_CCore->GetLocalPlayer()->IDinCar == pw->vehID)
			{
				g_CCore->GetGame()->KickPlayerFromCarFast(g_CCore->GetLocalPlayer()->GetBase());
				g_CCore->GetLocalPlayer()->SetIsOnFoot(true);
				g_CCore->GetLocalPlayer()->IDinCar = -1;
			}
			g_CCore->GetGame()->DeleteCar(pw->base);

		}
		break;
		case CLIENT_ENGINESTACK::ES_PLAYER_ENTER_VEH:
		{
			ENGINE_STACK::PLAYER_ENTER_VEH* pw = (ENGINE_STACK::PLAYER_ENTER_VEH*) start->data;
			if (pw->pID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				//DWORD adr = *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
				//g_CCore->GetGame()->SwitchWeapon(adr, pw->wepID);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->pID);
				if (ped != 0)
				{
					if (ped->GetEntity() != 0)
					{
						ped->SetIsOnFoot(false);
						g_CCore->GetGame()->GivePlayerToCar(ped->GetEntity(), pw->vehID,pw->seatID);
					}
				}
			}
		}
		break;
		case CLIENT_ENGINESTACK::ES_PLAYER_PUT_TO_VEH:
		{
			ENGINE_STACK::PLAYER_ENTER_VEH* pw = (ENGINE_STACK::PLAYER_ENTER_VEH*) start->data;
			if (pw->pID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				DWORD adr = *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
				//DWORD adr = *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
				//g_CCore->GetGame()->SwitchWeapon(adr, pw->wepID);
				g_CCore->GetLocalPlayer()->IDinCar = pw->vehID;
				g_CCore->GetLocalPlayer()->SetIsOnFoot(false);
				g_CCore->GetGame()->GivePlayerToCarFast(adr, pw->vehID, pw->seatID);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->pID);
				if (ped != 0)
				{
					if (ped->GetEntity() != 0)
					{
						ped->SetIsOnFoot(false);
						g_CCore->GetGame()->GivePlayerToCarFast(ped->GetEntity(), pw->vehID, pw->seatID);
					}
				}
			}
		}
			break;
		case CLIENT_ENGINESTACK::ES_PLAYER_EXIT_VEH:
		{
			ENGINE_STACK::PLAYER_EXIT_VEH* pw = (ENGINE_STACK::PLAYER_EXIT_VEH*) start->data;
			if (pw->pID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				//DWORD adr = *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
				//g_CCore->GetGame()->SwitchWeapon(adr, pw->wepID);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->pID);
				if (ped != 0)
				{
					if (ped->GetEntity() != 0)
					{
						ped->SetIsOnFoot(true);
						g_CCore->GetGame()->KickPlayerFromCar(ped->GetEntity(), pw->vehID);
					}
				}
			}
		}
		break;
		case CLIENT_ENGINESTACK::ES_VEHICLE_JACK:
		{
			ENGINE_STACK::VEH_JACK* pw = (ENGINE_STACK::VEH_JACK*) start->data;
			if (pw->pID == g_CCore->GetLocalPlayer()->GetOurID())
			{
				CVehicle* veh = g_CCore->GetVehiclePool()->Return(pw->vehID);
				g_CCore->GetLocalPlayer()->IDinCar = -1;
				g_CCore->GetLocalPlayer()->SetIsOnFoot(true);
				g_CCore->GetGame()->CarJack(g_CCore->GetLocalPlayer()->GetBase(), veh->GetEntity(), pw->seatID);
			}
			else
			{
				CPed* ped = g_CCore->GetPedPool()->Return(pw->pID);
				CVehicle* veh = g_CCore->GetVehiclePool()->Return(pw->vehID);
				if (ped != 0)
				{
					if (ped->GetEntity() != 0)
					{
						if (veh != 0)
						{
							if (veh->GetEntity() != 0)
							{
								int jackedID = veh->GetSeat(pw->seatID);
								veh->PlayerExit(jackedID);
								if (jackedID == g_CCore->GetLocalPlayer()->GetOurID())
								{
									g_CCore->GetLocalPlayer()->IDinCar = -1;
									g_CCore->GetLocalPlayer()->SetIsOnFoot(true);
								} else {
									CPed* pedJacked = g_CCore->GetPedPool()->Return(jackedID);
									if (pedJacked != NULL)
										pedJacked->SetIsOnFoot(true);
									g_CCore->GetGame()->CarJack(ped->GetEntity(), veh->GetEntity(),pw->seatID);
								}
							}
						}
					}
				}
			}
		}
			break;

		case CLIENT_ENGINESTACK::ES_DOOR_SET_STATE:
		{
			g_CCore->GetLog()->AddLog("ES_DOORSETSTATE");
			ENGINE_STACK::DOOR_SET_STATE* pw = (ENGINE_STACK::DOOR_SET_STATE*) start->data;
			g_CCore->GetGame()->SetDoorState(pw->buff, pw->state);
		}
		break;
		case CLIENT_ENGINESTACK::ES_CHANGEMAP:
		{
			g_CCore->GetLog()->AddLog("ES_CHANGEMAP"); 
			g_CCore->GetLog()->AddLog((char*)start->data);
			//ENGINE_STACK::DOOR_SET_STATE* pw = (ENGINE_STACK::DOOR_SET_STATE*) start->data;
			//g_CCore->GetGame()->SetDoorState(pw->buff, pw->state);
			/*if (*(DWORD*)((*(DWORD*)0x6F9464) + 0x24) == 0x0)
				return;
			*/

			// Reload tables/predmety.def
			_asm {
				MOV ECX, 0x00658330; Game.00658330
				MOV EAX, 0x00592080;
				CALL EAX; Game.00592080;  loads predmety.def
			}



			g_CCore->GetGame()->ChangeMap((char*)start->data,"");

			strcpy(g_CCore->GetGame()->mapName, (char*)start->data);
			/*if (*(DWORD*)((*(DWORD*)0x6F9464) + 0x24) == 0x0)
			{
				g_CCore->GetChat()->AddMessage("este nenacital, pice !");
			}*/
		}
			break;
		case CLIENT_ENGINESTACK::ES_CAREXPLODE:
		{
			g_CCore->GetLog()->AddLog("ES_CAREXPLODE");
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(start->data);
			if (veh != NULL)
			{
				if (veh->GetEntity() != NULL)
				{
					g_CCore->GetGame()->ExplodeCar(veh->GetEntity());
				}
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					CPed* ped = g_CCore->GetPedPool()->Return(i);
					if (ped != NULL)
					{
						if (ped->GetEntity() != NULL)
						{
							if (ped->InCar == start->data)
							{
								ped->InCar = -1;
								ped->SetIsOnFoot(true);
							}
						}
					}
				}
				for (int i = 0; i < 4; i++)
				{
					veh->PlayerExit(veh->GetSeat(i));
				}
				if (g_CCore->GetLocalPlayer()->IDinCar == start->data)
						g_CCore->GetLocalPlayer()->IDinCar = -1;
			}
		}
			break;
		case CLIENT_ENGINESTACK::ES_CARRESPAWN:
		{
			g_CCore->GetLog()->AddLog("ES_CARRESPAWN");

			CVehicle* veh = g_CCore->GetVehiclePool()->Return(start->data);
			if (veh != NULL)
			{
				if (veh->GetEntity() != NULL)
				{
					veh->SetDamage(100.0f);
					if (g_CCore->GetLocalPlayer()->IDinCar != start->data)
					{
						g_CCore->GetGame()->DeleteCar(veh->GetEntity());
						if (veh->GetExplodedCar())
						{
							g_CCore->GetGame()->DeleteCar(veh->GetExplodedCar());
						}
						veh->SetEntity(NULL);
						veh->SetExplodedCar(NULL);

						veh->SetEntity(g_CCore->GetGame()->CreateCar(veh->GetSkin()));
						veh->SetDamage(veh->GetDamage());
						veh->SetShotDamage(veh->GetShotDamage());
						veh->ToggleRoof(veh->GetRoofState());
					}
					else {
						veh->PlayerExit(g_CCore->GetLocalPlayer()->GetOurID());
						g_CCore->GetLocalPlayer()->IDinCar = -1;
						g_CCore->GetGame()->KickPlayerFromCarFast(veh->GetEntity());
					}
					//g_CCore->GetGame()->ChangeSkin(veh->GetEntity(), veh->GetSkin());

				}
			}
		}
			break;

		case CLIENT_ENGINESTACK::ES_CREATEPICKUP:
		{
			int ID = start->data;
			CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
			if (pickup)
			{

				if(pickup->IsVisible())
				{

					pickup->SetEntity(g_CCore->GetGame()->CreateFrame(pickup->GetModel()));
					g_CCore->GetGame()->SetFrameScale(pickup->GetEntity(), pickup->GetSize(), pickup->GetSize(), pickup->GetSize());
					g_CCore->GetGame()->SetFramePos(pickup->GetEntity(), pickup->GetPosition().x, pickup->GetPosition().y, pickup->GetPosition().z);
					
					//g_CCore->GetGame()->SetFrameRot(pickup->GetEntity(),0,0,0,0);

					//char buff[200];
					//sprintf(buff, "pice %f", pickup->GetSize());
					//g_CCore->GetChat()->AddMessage(buff);
				}
			}
		}
			break;
		case CLIENT_ENGINESTACK::ES_DELETEPICKUP:
		{
			int ID = start->data;
			CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
			if (pickup)
			{
				DWORD entity = pickup->GetEntity();
				if (entity)
				{
					_asm
					{
						mov eax, entity
							push eax
							mov ecx, [eax]
							call dword ptr ds : [ecx]
					}
					pickup->SetEntity(NULL);
				}
			}
		}
			break;
		case CLIENT_ENGINESTACK::ES_SETPICKUPVISIBLE:
		{
			int ID = start->data;
			CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
			if (pickup)
			{
				if (!pickup->IsVisible())
				{
					DWORD entity = pickup->GetEntity();
					if (entity)
					{
						_asm
						{
							mov eax, entity
								push eax
								mov ecx, [eax]
								call dword ptr ds : [ecx]
						}
						pickup->SetEntity(NULL);
					}

					//g_CCore->GetChat()->AddMessage("pickup hide");
				}
				else {
					if (!pickup->GetEntity())
					{
						pickup->SetEntity(g_CCore->GetGame()->CreateFrame(pickup->GetModel()));
						g_CCore->GetGame()->SetFrameScale(pickup->GetEntity(), pickup->GetSize(), pickup->GetSize(), pickup->GetSize());
						g_CCore->GetGame()->SetFramePos(pickup->GetEntity(), pickup->GetPosition().x, pickup->GetPosition().y, pickup->GetPosition().z);

						//g_CCore->GetChat()->AddMessage("pickup visible");
					}
				}
			}
		}
			break;
		default:
			g_CCore->GetLog()->AddLog("Default ENGINE STACK");
			//char buffer[255];
			//sprintf(buffer,"Spravy iduuuuuuuuuu hura %i",start->data);
			//g_CCore->GetChat()->AddMessage(buffer);
			//g_CCore->GetGame()->CreatePED();

		}
		EngineStackMessage* handle = start;
		start = handle->next;
		if(end == handle)
		{
			end = 0;
			start = 0;
		}
		delete[] handle;
	}
}