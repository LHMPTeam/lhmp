#pragma once

const std::vector<Vector3D> mTestingPositions = {
	{ -1982.884277f, -5.032383f, 23.144674f },
	{ -1987.884277f, -5.032383f, 23.144674f },
	{ -1985.884277f, -5.032383f, 27.144674f },
	{ -1985.884277f, -5.032383f, 23.144674f }
};

class Sandbox {
public:
	Sandbox();
	void RegisterCommands();
	void Tick();
private:
	Player* mTestingPlayer;
	bool mEnableInterpolation;
	int mPositionIndex;
	float mLerpValue;
	Vector3D mLastPosition;
};