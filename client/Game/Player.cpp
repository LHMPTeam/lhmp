#include <stdinc.h>

Player::Player(std::string modelName)
	: mModelName(modelName),
	mPlayer(nullptr),
	mIsShooting(false),
	mInterpolator(3)
{
}

Player::~Player()
{

}

void Player::Spawn()
{
	MafiaSDK::I3D_Frame* playerFrame = new MafiaSDK::I3D_Frame();
	playerFrame->LoadModel(mModelName.c_str());
	mPlayer = reinterpret_cast<MafiaSDK::C_Human*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::Enemy));
	mPlayer->Init(playerFrame);
	mPlayer->SetBehavior(MafiaSDK::C_Human_Enum::BehaviorStates::DoesntReactOnWeapon);
	mPlayer->SetActive(true);
	mPlayer->SetShooting(1.0f);
	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(mPlayer);

	mPlayer->GetInterface()->entity.position = { -1985.884277f, -5.032383f, 23.144674f };
	mInterpolator.SetInitialValue(0, -1985.884277f);
	mInterpolator.SetInitialValue(1, -5.032383f);
	mInterpolator.SetInitialValue(2, 23.144674f);

	mInterpolator.SetTargetValue(0, -1985.884277f);
	mInterpolator.SetTargetValue(1, -5.032383f);
	mInterpolator.SetTargetValue(2, 23.144674f);
	printf("spawn");
}

void Player::Respawn()
{
	MafiaSDK::I3D_Frame* playerFrame = new MafiaSDK::I3D_Frame();
	playerFrame->LoadModel(mModelName.c_str());

	MafiaSDK::C_Human* newPlayer = reinterpret_cast<MafiaSDK::C_Human*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::Enemy));
	newPlayer->Init(playerFrame);
	newPlayer->SetBehavior(MafiaSDK::C_Human_Enum::BehaviorStates::DoesntReactOnWeapon);
	newPlayer->SetActive(true);
	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(newPlayer);

	if (mPlayer != nullptr)
		MafiaSDK::GetMission()->GetGame()->RemoveTemporaryActor(mPlayer);

	printf("respawn\n");
	newPlayer->GetInterface()->entity.position = { -1985.884277f, -5.032383f, 23.144674f };
	mInterpolator.SetInitialValue(0, -1985.884277f);
	mInterpolator.SetInitialValue(1, -5.032383f);
	mInterpolator.SetInitialValue(2, 23.144674f);

	mInterpolator.SetTargetValue(0, -1985.884277f);
	mInterpolator.SetTargetValue(1, -5.032383f);
	mInterpolator.SetTargetValue(2, 23.144674f);

	mPlayer = newPlayer;
}

void Player::SetPosition(Vector3D position)
{
	mPlayer->GetInterface()->entity.position = position;
}

Vector3D Player::GetPosition()
{
	return mPlayer->GetInterface()->entity.position;
}

void Player::SetRotation(unsigned short rotation)
{
	mPlayer->GetInterface()->entity.rotation = Utils::DegreeToRotation(Utils::DegreeShortToMap(rotation));
}

unsigned short Player::GetRotation()
{
	return Utils::DegreeMapToShort(Utils::RotationToDegree(mPlayer->GetInterface()->entity.rotation));
}

void Player::SetModel(const std::string & modelName)
{
	mPlayer->Intern_ChangeModel(modelName.c_str());
	mModelName = modelName;
}

std::string Player::GetModel()
{
	return mModelName;
}

void Player::SetHealth(float health)
{
	mPlayer->GetInterface()->health = health;
}

float Player::GetHealth()
{
	return mPlayer->GetInterface()->health;
}

void Player::SetAnimationState(char state)
{
	mPlayer->GetInterface()->animState = state;
}

char Player::GetAnimationState()
{
	return mPlayer->GetInterface()->animState;
}

void Player::SetIsCrouching(bool isCrouching)
{
	mPlayer->GetInterface()->isDucking = isCrouching;
}

bool Player::GetIsCrouching()
{
	return mPlayer->GetInterface()->isDucking;
}

void Player::SetIsAiming(bool aiming)
{
	mPlayer->GetInterface()->isAiming = aiming;
}

bool Player::GetIsAiming()
{
	return mPlayer->GetInterface()->isAiming;
}

void Player::UpdateGameObject()
{
	float* positions = mInterpolator.Interpolate();
	mPlayer->GetInterface()->entity.position = { positions[0], positions[1], positions[2] };

	if (IsShooting())
	{
		Vector3D* shootVec = &GetShootVec();
		mPlayer->Do_Shoot(true, shootVec);
		mPlayer->Do_Shoot(true, NULL);
		printf("Shoot At %f %f %f\n", GetShootVec().x, GetShootVec().y, GetShootVec().z);
		SetIsShooting(false);
	}
}
