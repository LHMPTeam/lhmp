#include <stdinc.h>

Sandbox::Sandbox() :
	mEnableInterpolation(false),
	mLerpValue(0.0f),
	mPositionIndex(0)
{
}

void Sandbox::RegisterCommands()
{
	Core::GetCore()->GetGraphics()->GetChat()->RegisterChatCMD(L"c", [&](auto args) {

		Vector3D daco = { -1985.884277f, -5.032383f, 23.144674f };
		mLastPosition = daco;

		mTestingPlayer = new Player("Tommy.i3d");
		mTestingPlayer->Spawn();
		mTestingPlayer->SetNickName(L"Robert Fico");
		mTestingPlayer->SetPosition(daco);
		Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"Baka nee-san");
		mTestingPlayer->GetInterpolator()->SetTargetValue(0, mTestingPositions[mPositionIndex].x);
		mTestingPlayer->GetInterpolator()->SetTargetValue(1, mTestingPositions[mPositionIndex].y);
		mTestingPlayer->GetInterpolator()->SetTargetValue(2, mTestingPositions[mPositionIndex].z);
		mEnableInterpolation = true;
	});
}

void Sandbox::Tick()
{
	if (mEnableInterpolation) {
		mLerpValue += 0.01f + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 0.05f);
		printf("%f\n", mLerpValue);
		//Vector3D editedPosition = mLastPosition.Lerp(mTestingPositions[mPositionIndex], mLerpValue);
		float* positions = mTestingPlayer->GetInterpolator()->Interpolate();
		Vector3D pos = { positions[0], positions[1], positions[2] };
		mTestingPlayer->SetPosition(pos);
		if (mLerpValue >= 1.0f) {
			mLerpValue = 0.0f;
			mLastPosition = mTestingPositions[mPositionIndex];
			mPositionIndex = mPositionIndex == 3 ? 0 : mPositionIndex + 1;
			mTestingPlayer->GetInterpolator()->NewValues();
			mTestingPlayer->GetInterpolator()->SetInitialValue(0, pos.x);
			mTestingPlayer->GetInterpolator()->SetInitialValue(1, pos.y);
			mTestingPlayer->GetInterpolator()->SetInitialValue(2, pos.z);
			mTestingPlayer->GetInterpolator()->SetTargetValue(0, mTestingPositions[mPositionIndex].x);
			mTestingPlayer->GetInterpolator()->SetTargetValue(1, mTestingPositions[mPositionIndex].y);
			mTestingPlayer->GetInterpolator()->SetTargetValue(2, mTestingPositions[mPositionIndex].z);
		}
		//printf("LerpValue: %f\n", mLerpValue);
	}
}
