#include <stdinc.h>

LocalPlayer::LocalPlayer(std::string modelName)
	: mModelName(modelName), mIsSpawned(false)
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

	MafiaSDK::GetMission()->GetGame()->GetCamera()->SetPlayer(mPlayer);
	MafiaSDK::GetMission()->GetGame()->SetLocalPlayer(mPlayer);
	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(mPlayer);


	mPlayer->GetInterface()->humanObject.entity.position = { -1984.884277f, -5.032383f, 23.144674f };
	mIsSpawned = true;
}

void LocalPlayer::SetPosition(Vector3D position)
{
	mPlayer->GetInterface()->humanObject.entity.position = position;
}

Vector3D LocalPlayer::GetPosition()
{
	return mPlayer->GetInterface()->humanObject.entity.position;
}

void LocalPlayer::SetRotation(Vector3D rotation)
{
	mPlayer->GetInterface()->humanObject.entity.rotation = rotation;
}

Vector3D LocalPlayer::GetRotation()
{
	return mPlayer->GetInterface()->humanObject.entity.rotation;
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

void LocalPlayer::Tick()
{
	if (mIsSpawned)
	{
		//if (mPlayer->GetInterface()->humanObject.playersCar == nullptr)
		//{
			OnFootSync footSync;
			footSync.animationState = GetAnimationState();
			footSync.Position = GetPosition();
			footSync.Rotation = GetRotation();
			Core::GetCore()->GetNetwork()->SendPlayerOnFoot(footSync);
		//}
	}
}