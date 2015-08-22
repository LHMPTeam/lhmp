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
			
			Vector3D playerPosition = g_CCore->GetGame()->GetPEDNeckPosition((PED*)ped->GetEntity());
			playerPosition.y += 0.3f;
			//--------------------------------------------------------
			/*Vector3D playerPosition = ped->GetPEDFramePosition();
			playerPosition.y += 2.1f;
			if (ped->IsDucking() && ped->InCar == -1)
				playerPosition.y -= 0.75;
			*/
			//--------------------------------------------------------

			// check whether he is close to local player
			if (Tools::GetDistanceBetween3DPointsEx(position, playerPosition, 80.0f) < 2)
			{
				// ok, remote player is close enough
				// now check whether his nametag texture exists - TODO
				// if doesn't exist, create it & render text to it
				//if (ped->nametag == NULL)
				//{
				float distance = Tools::GetDistanceBetween3DPoints(playerPosition, g_CCore->GetGame()->GetCameraPos());
				float maxDistance = 50;                
				
				if (ped->InCar != -1) {
					maxDistance = 80;

					playerPosition.y += 0.3f;
				}

				int alpha = (int)(255*(1.25f - (distance / maxDistance)));

				// alpha be from interval <0,255>
				alpha = Tools::Clamp(alpha, 0, 255);
				char textureAlpha = alpha;
				int alpha2 = (int)(alpha * 0.5f);
				int alpha3 = (int)(alpha * 1.2f);
				if (alpha3 > 255) alpha3 = 255;
				
				if (ped->nametag == NULL)
					this->RenderToTexture(i);

				if (ped->nametag != NULL)
				{
					Vector3D screen;
					// now calculate player position on screen
					g_CCore->GetGraphics()->CalcScreenPosition(playerPosition, &screen);
					// if object is on screen
					if (screen.z > 0.0f)
					{
						// Retreive texture's size
						D3DSURFACE_DESC desc;
						ped->nametag->GetLevelDesc(0, &desc);

						int barWidth = 75;
						int barHeight = 7;
						float healthPercentage = ped->GetHealth() / 200.0f;

						alpha = (int)(alpha * 0.85f);

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
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2.0f)), (int)(screen.y - barHeight - 1.0f),screen.z, barWidth*healthPercentage, barHeight, healthBarColor);

						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2)), (int)(screen.y - barHeight - 1), screen.z, barWidth - 1, 2, healthBarColorTop);
						g_CCore->GetGraphics()->FillARGB((int)(screen.x - (barWidth / 2)), (int)(screen.y - 2), screen.z, barWidth - 1, 2, healthBarColorBottom);

						// now render player's nick (scaled using ratio)
						// alpha - 0-255
						g_CCore->GetGraphics()->RenderTexture(screen.x - (0.5f*desc.Width), screen.y - (desc.Height) - barHeight + 114, screen.z, desc.Width, desc.Height, ped->nametag, alpha3);
							
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

			g_CCore->GetGraphics()->GetFont()->DrawTextA(ped->GetName(), 128 - (size.cx / 2) + 1, 128 - (size.cy / 2) + 1, D3DCOLOR_RGBA(0, 0, 0, 255), false);
			// original color: D3DCOLOR_RGBA(255, 255, 255, 255)
			g_CCore->GetGraphics()->GetFont()->DrawTextA(ped->GetName(), 128 - (size.cx / 2), 128 - (size.cy / 2), 0xFFFFFFFF, false);

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