#ifndef _CNAMETAGS
#define _CNAMETAGS
/**
Lost Heaven Multiplayer

Purpose: renders nametags 
		(keeps them in textures, only for players on-screen)

@author Romop5
@version 1.0 22/1/15
*/

#include "CCore.h"
class CNametags
{
public:
	// called on EndScene
	void Tick();
	// Render nick to texture
	void RenderToTexture(int playerID);
	// Render texture
	void RenderNametag(LPDIRECT3DTEXTURE8 texture, float x, float y, float z, float scale);
	// Delete all textures
	void OnLostDevice();
};

#endif