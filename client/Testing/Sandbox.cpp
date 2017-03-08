#include <stdinc.h>

Sandbox::Sandbox() :
	mEnableInterpolation(false),
	mLerpValue(0.0f),
	mPositionIndex(0)
{
}

void Sandbox::RegisterCommands()
{
	Core::GetCore()->GetGraphics()->GetChat()->RegisterChatCMD(L"createPed", [&](auto args) {

		Vector3D daco = { -1985.884277f, -5.032383f, 23.144674f };
		mLastPosition = daco;

		mTestingPlayer = new Player("Tommy.i3d");
		mTestingPlayer->Spawn();
		mTestingPlayer->SetNickName(L"Robert Fico");
		mTestingPlayer->SetPosition(daco);
		Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"Baka nee-san");

		mEnableInterpolation = true;
	});
}

void Sandbox::Tick()
{
	if (mEnableInterpolation) {
		mLerpValue += 0.01f;
		Vector3D editedPosition = mLastPosition.Lerp(mTestingPositions[mPositionIndex], mLerpValue);
		mTestingPlayer->SetPosition(editedPosition);
		if (mLerpValue >= 1.0f) {
			mLerpValue = 0.0f;
			mLastPosition = mTestingPositions[mPositionIndex];
			mPositionIndex = mPositionIndex == 3 ? 0 : mPositionIndex + 1;
		}
		printf("LerpValue: %f\n", mLerpValue);
	}
}
