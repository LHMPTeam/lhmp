#include "CCore.h"
#include "CNametags.h"

extern CCore *g_CCore;

void CNametags::Tick()
{
	// get local player position
	Vector3D position = g_CCore->GetLocalPlayer()->GetLocalPos();
	// for every player
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(i);
		// if ped exists
		if (ped != NULL)
		{
			if (ped->GetEntity() == NULL)
				continue;
			Vector3D playerPosition = ped->GetPEDFramePosition();
			playerPosition.y += 2.25;
			if (ped->IsDucking())
				playerPosition.y -= 1.0;
			// check whether he is close to local player
			if (Tools::GetDistanceBetween3DPointsEx(position, playerPosition, 50.0f) < 2)
			{
				// ok, remote player is close enough
				// now check whether his nametag texture exists - TODO
				// if doesn't exist, create it & render text to it
				if (ped->nametag == NULL)
				{
					this->RenderToTexture(i);
				}

				if (ped->nametag != NULL)
				{
					Vector3D screen;
					// now calculate player position on screen
					g_CCore->GetGraphics()->CalcScreenPosition(playerPosition, &screen);
					// if object is on screen
					if (screen.z > 0.0f)
					{
						// sligtly push more forwards to local player
						//screen.z -= 0.001f; 
						// Retreive texture's size
						D3DSURFACE_DESC desc;
						ped->nametag->GetLevelDesc(0, &desc);


						float ratio = (5 / Tools::GetDistanceBetween3DPoints(playerPosition, g_CCore->GetGame()->GetCameraPos()));
						if (ratio < 1.0f)
							ratio = 1.0f;

						int barWidth = 100;
						int barHeight = 10;
						float healthPercentage = ped->GetHealth() / 200.0f;
						DWORD healthBarColor = 0xFFFF0000; // red color
						//if player has more than 50% of life
						if (ped->GetHealth() > 50.0f)
							healthBarColor = 0xFF00FF00; // green color

						// render the health bar's background
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2.0f) - 1.0f), (int)(screen.y - barHeight - 2), screen.z, barWidth + 2, barHeight + 2, D3DCOLOR_XRGB(0, 0, 0));
						// now render the health bar itself
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2.0f)), (int)(screen.y - barHeight - 1.0f), screen.z, barWidth*healthPercentage, barHeight, healthBarColor);

						// now render player's nick (scaled using ratio)
						g_CCore->GetGraphics()->RenderTexture(screen.x - (0.5f*desc.Width*ratio), screen.y - (desc.Height*ratio) - barHeight - 5, screen.z, (desc.Width*ratio), (desc.Height*ratio), ped->nametag);
							

					}
				}
			}
		}
	}
}


void CNametags::RenderToTexture(int playerID)
{
	CPed* ped = g_CCore->GetPedPool()->Return(playerID);
	if (ped)
	{
		// at first, get size 
		SIZE size = g_CCore->GetGraphics()->GetFont()->GetFontWidth(ped->GetName());

		D3DXCreateTexture(g_CCore->GetGraphics()->GetDevice(), size.cx + 10, size.cy+5,
			1, D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &ped->nametag);
			
		if (ped->nametag != NULL)
		{

			IDirect3DSurface8* pSurf, *pOldTarget, *oldStencil;
			if (!SUCCEEDED(ped->nametag->GetSurfaceLevel(0, &pSurf)))
				return;
			if (!SUCCEEDED(g_CCore->GetGraphics()->GetDevice()->GetRenderTarget(&pOldTarget)))
				return;
			if (!SUCCEEDED(g_CCore->GetGraphics()->GetDevice()->GetDepthStencilSurface(&oldStencil)))
				return;
			HRESULT hr;
			hr = g_CCore->GetGraphics()->GetDevice()->SetRenderTarget(pSurf, NULL);
			if (FAILED(hr)) {
				char buffer[255];
				D3DXGetErrorString(hr, buffer, 200);
				sprintf(buffer, "SetRenderTarget %s", buffer);
				MessageBoxA(NULL, buffer, buffer, MB_OK);
			}

			// clear texture to clean one
			//g_CCore->GetGraphics()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB( 0,255, 255, 255), 1.0f, 0);
			g_CCore->GetGraphics()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
			// now we are ready to render to texture

			g_CCore->GetGraphics()->DrawTextA(ped->GetName(), 2, 2, D3DCOLOR_XRGB(255, 255, 255), true);
		
			// return D3D to previos stage and release unneeded resources
			g_CCore->GetGraphics()->GetDevice()->SetRenderTarget(pOldTarget, oldStencil);
			pSurf->Release();
			pOldTarget->Release();
			oldStencil->Release();
		}
	}
}


void CNametags::RenderNametag(LPDIRECT3DTEXTURE8 texture, float x, float y, float z, float scale)
{
	if (texture != NULL)
	{
		/*D3DXVECTOR2 scaling;
		scaling.x = scale;
		scaling.y = scale;
		D3DXVECTOR2 translation;
		translation.x = x;
		translation.y = y;

		if (g_CCore->GetGraphics()->GetSprite()->Begin() != S_OK)
			return;
		g_CCore->GetGraphics()->GetSprite()->Draw(texture, NULL, &scaling, NULL, 0.0, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
		g_CCore->GetGraphics()->GetSprite()->End();*/

	}
}

void CNametags::OnLostDevice()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(i);
		// if ped exists
		if (ped != NULL)
		{
			if (ped->nametag != NULL)
				ped->nametag->Release();
			ped->nametag = NULL;
		}
	}
}