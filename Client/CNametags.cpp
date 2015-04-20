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
			playerPosition.y += 2.1;
			if (ped->IsDucking())
				playerPosition.y -= 1.0;
			// check whether he is close to local player
			if (Tools::GetDistanceBetween3DPointsEx(position, playerPosition, 50.0f) < 2)
			{
				// ok, remote player is close enough
				// now check whether his nametag texture exists - TODO
				// if doesn't exist, create it & render text to it
				//if (ped->nametag == NULL)
				//{
				float distance = Tools::GetDistanceBetween3DPoints(playerPosition, g_CCore->GetGame()->GetCameraPos());
				float maxDistance = 17.5;

				int alpha = 255 * (1.25 - (distance / maxDistance));
				if (alpha > 255) alpha = 255;
				if (alpha <= 0) alpha = 0;
				int alpha2 = alpha * 0.5;

				this->RenderToTexture(i, alpha);
				//}

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

						/*float ratio = (5 / Tools::GetDistanceBetween3DPoints(playerPosition, g_CCore->GetGame()->GetCameraPos()));
						//if (ratio < 1.0f)
							ratio = 1.0f;*/

						int barWidth = 75;
						int barHeight = 7;
						float healthPercentage = ped->GetHealth() / 200.0f;

						alpha = alpha * 0.85;

						D3DCOLOR healthBarColor = D3DCOLOR_RGBA(217, 0, 0, alpha);
						D3DCOLOR healthBarColorBg = D3DCOLOR_RGBA(166, 0, 0, alpha);

						D3DCOLOR healthBarColorTop = D3DCOLOR_RGBA(171, 14, 12, alpha2);
						D3DCOLOR healthBarColorBottom = D3DCOLOR_RGBA(47, 0, 0, alpha2);

						//if player has more than 50% of life
						if (ped->GetHealth() > 100.0f) {
							healthBarColor = D3DCOLOR_RGBA(0, 217, 0, alpha); // green color
							healthBarColorBg = D3DCOLOR_RGBA(0, 166, 0, alpha);

							healthBarColorTop = D3DCOLOR_RGBA(0, 217, 2, alpha2);
							healthBarColorBottom = D3DCOLOR_RGBA(0, 95, 0, alpha2);
						}

						// render the health bar's background
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2.0f) - 1.0f), (int)(screen.y - barHeight - 2), screen.z, barWidth + 2, barHeight + 2, D3DCOLOR_RGBA(0, 0, 0, alpha));
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2.0f)), (int)(screen.y - barHeight - 1.0f), screen.z, barWidth, barHeight, healthBarColorBg);

						// now render the health bar itself
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2.0f)), (int)(screen.y - barHeight - 1.0f), screen.z, barWidth*healthPercentage, barHeight, healthBarColor);

						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2)), (int)(screen.y - barHeight - 1), screen.z, barWidth, 2, healthBarColorTop);
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2)), (int)(screen.y - 2), screen.z, barWidth, 2, healthBarColorBottom);

						// now render player's nick (scaled using ratio)
						g_CCore->GetGraphics()->RenderTexture(screen.x - (0.5f*desc.Width), screen.y - (desc.Height) - barHeight + 114, screen.z, desc.Width, desc.Height, ped->nametag);
							
						//bool state = (*(byte*)(ped->GetEntity() + 0x5E0) == 1);
						/*byte state = ped->GetState();
						char buff[500];
						sprintf(buff, "State: %x", state);
						g_CCore->GetGraphics()->D3DDrawText(buff,screen.x,screen.y,0xFFFF0000,true);*/
					}
				}
			}
		}
	}
}


void CNametags::RenderToTexture(int playerID, int alpha)
{
	CPed* ped = g_CCore->GetPedPool()->Return(playerID);
	if (ped)
	{
		// at first, get size 
		SIZE size = g_CCore->GetGraphics()->GetFont()->GetFontWidth(ped->GetName());

		if (ped->nametag == NULL) {
			D3DXCreateTexture(g_CCore->GetGraphics()->GetDevice(), 256, 256,
				1, D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &ped->nametag);
		}
			
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

			alpha = alpha * 2.33;
			if (alpha > 255) alpha = 255;

			g_CCore->GetGraphics()->GetFont()->DrawTextA(ped->GetName(), 128 - (size.cx / 2) + 1, 128 - (size.cy / 2) + 1, D3DCOLOR_RGBA(0, 0, 0, alpha), false);
			g_CCore->GetGraphics()->GetFont()->DrawTextA(ped->GetName(), 128 - (size.cx / 2), 128 - (size.cy / 2), D3DCOLOR_RGBA(255, 255, 255, alpha), false);

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