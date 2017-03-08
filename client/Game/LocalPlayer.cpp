#include <stdinc.h>

LocalPlayer::LocalPlayer(std::string modelName)
	: mModelName(modelName), 
	mIsRespawning(false)
{
	
}

LocalPlayer::~LocalPlayer()
{

}

void LocalPlayer::Spawn()
{
	MafiaSDK::I3D_Frame* playerFrame = new MafiaSDK::I3D_Frame;
	mPlayer = reinterpret_cast<MafiaSDK::C_Player*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::Player));
	playerFrame->LoadModel(mModelName.c_str());

	mPlayer->Init(playerFrame);
	mPlayer->SetActive(true);

	MafiaSDK::GetMission()->GetGame()->GetCamera()->Unlock();
	MafiaSDK::GetMission()->GetGame()->GetCamera()->SetPlayer(mPlayer);
	MafiaSDK::GetMission()->GetGame()->SetLocalPlayer(mPlayer);
	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(mPlayer);

	mPlayer->GetInterface()->humanObject.entity.position = { -1984.884277f, -5.032383f, 23.144674f };

}

void LocalPlayer::Respawn()
{
	mIsRespawning = true;
	mRespawnStage = RespawnStages::RESPAWN_INIT;
	mRespawnTimer = RakNet::GetTimeMS();
}

void LocalPlayer::SetPosition(Vector3D position)
{
	mPlayer->GetInterface()->humanObject.entity.position = position;
}

Vector3D LocalPlayer::GetPosition()
{
	return mPlayer->GetInterface()->humanObject.entity.position;
}

void LocalPlayer::SetRotation(unsigned short rotation)
{
	mPlayer->GetInterface()->humanObject.entity.rotation = Utils::DegreeToRotation(Utils::DegreeShortToMap(rotation));
}

unsigned short LocalPlayer::GetRotation()
{
	return Utils::DegreeMapToShort(Utils::RotationToDegree(mPlayer->GetInterface()->humanObject.entity.rotation));
}

void LocalPlayer::SetModel(const std::string & modelName)
{
	mPlayer->Intern_ChangeModel(modelName.c_str());
	mModelName = modelName;
}

std::string LocalPlayer::GetModel()
{
	return mModelName;
}

void LocalPlayer::SetHealth(float health)
{
	mPlayer->GetInterface()->humanObject.health = health;
}

float LocalPlayer::GetHealth()
{
	return mPlayer->GetInterface()->humanObject.health;
}

void LocalPlayer::SetAnimationState(char state)
{
	mPlayer->GetInterface()->humanObject.animState = state;
}

char LocalPlayer::GetAnimationState()
{
	return mPlayer->GetInterface()->humanObject.animState;
}

void LocalPlayer::SetIsCrouching(bool isCrouching)
{
	mPlayer->GetInterface()->humanObject.isDucking = isCrouching;
}

bool LocalPlayer::GetIsCrouching()
{
	return mPlayer->GetInterface()->humanObject.isDucking;
}

void LocalPlayer::SetIsAiming(bool isCrouching)
{
	mPlayer->GetInterface()->humanObject.isAiming = isCrouching;
}

bool LocalPlayer::GetIsAiming()
{
	return mPlayer->GetInterface()->humanObject.isAiming;
}

void LocalPlayer::Tick()
{
	if (Core::GetCore()->GetNetwork()->IsConnected())
	{
		OnFootSyncStruct footSync;

		footSync.isAiming = GetIsAiming();
		footSync.animationState = GetAnimationState();
		footSync.Position = GetPosition();
		footSync.Rotation = GetRotation();
		footSync.isCrouching = GetIsCrouching();

		RakNet::BitStream bitStream;
		bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_SYNC));
		bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_SYNC_ONFOOT));
		bitStream.Write(footSync);
		Core::GetCore()->GetNetwork()->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Core::GetCore()->GetNetwork()->GetServerAddress(), false);
	}

	if (mIsRespawning)
	{
		if (RakNet::GetTimeMS() - mRespawnTimer > 2000)
		{
			switch (mRespawnStage)
			{
				case RespawnStages::RESPAWN_INIT:
				{
					MafiaSDK::GetMission()->GetGame()->GetIndicators()->FadeInOutScreen(true, 2000, 0x000000);
					//TODO(DavoSK): Add freeze here
					mRespawnStage = RespawnStages::RESPAWN_PLAYER;
				}
				break;

				case RespawnStages::RESPAWN_PLAYER:
				{
					//Unlock camera
					MafiaSDK::GetMission()->GetGame()->GetCamera()->Unlock();

					//Create new player 
					MafiaSDK::I3D_Frame* playerFrame = new MafiaSDK::I3D_Frame;
					MafiaSDK::C_Player* newPlayer = reinterpret_cast<MafiaSDK::C_Player*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::Player));
					playerFrame->LoadModel(mModelName.c_str());
					newPlayer->Init(playerFrame);
					newPlayer->SetActive(true);
					MafiaSDK::GetMission()->GetGame()->GetCamera()->Unlock();
					MafiaSDK::GetMission()->GetGame()->SetLocalPlayer(newPlayer);
					MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(newPlayer);
					newPlayer->GetInterface()->humanObject.entity.position = { -1984.884277f, -5.032383f, 23.144674f };

					//Remove old player from world
					if (mPlayer != nullptr)
						MafiaSDK::GetMission()->GetGame()->RemoveTemporaryActor(mPlayer);

					MafiaSDK::GetMission()->GetGame()->GetCamera()->SetPlayer(newPlayer);
					MafiaSDK::GetMission()->GetGame()->GetCamera()->SetMode(true, 1);
					MafiaSDK::GetMission()->GetGame()->GetIndicators()->PlayerSetWingmanLives(100);
					mPlayer = newPlayer;

					//Send respawn :)
					if (Core::GetCore()->GetNetwork()->IsConnected())
					{
						RakNet::BitStream outStream;
						outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
						outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_RESPAWN));
						Core::GetCore()->GetNetwork()->GetPeer()->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Core::GetCore()->GetNetwork()->GetServerAddress(), false);
					}

					MafiaSDK::GetMission()->GetGame()->GetIndicators()->FadeInOutScreen(false, 2000, 0x000000);
					mRespawnStage = RespawnStages::RESPAWN_DONE;
				}
				break;

				case RespawnStages::RESPAWN_DONE:
				{
					//TODO(DavoSK): Add unfreeze here 
					mIsRespawning = false;
					break;
				}
			}

			mRespawnTimer = RakNet::GetTimeMS();
		}
		
	}
}