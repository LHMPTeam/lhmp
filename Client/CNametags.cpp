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
			Vector3D playerPosition = ped->GetPosition();
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
				else {
					ped->nametag->Release();
					this->RenderToTexture(i);
				}

				// now calculate player position on screen - TODO

				// now just render the texture :)
				
				if (ped->nametag != NULL)
				{
					Vector3D screen;
					g_CCore->GetGraphics()->CalcScreenPosition(playerPosition, &screen);
					// if object is on screen
					if (screen.z > 0.0f)
					{
						screen.z -= 0.001f; // sligtly push more forward to local player
						D3DSURFACE_DESC desc;
						ped->nametag->GetLevelDesc(0, &desc);

						float ratio = (5 / Tools::GetDistanceBetween3DPoints(playerPosition, g_CCore->GetLocalPlayer()->GetLocalPos()));
						if (ratio < 1.0f)
							ratio = 1.0f;

						//g_CCore->GetGraphics()->RenderTexture(screen.x - (0.5f*desc.Width*ratio), screen.y - (0.5f*desc.Height*ratio), screen.z, (desc.Width*ratio), (desc.Height*ratio), ped->nametag);
						g_CCore->GetGraphics()->RenderTexture(screen.x, screen.y, screen.z,500, 500, g_CCore->GetChat()->chatTexture);

						int width = 100;
						int height = 10;
						float healthratio = ped->GetHealth() / 200.0f;
						g_CCore->GetGraphics()->FillARGB(screen.x - (0.5f*desc.Width*ratio), screen.y + (0.5f*height*ratio) + 10, screen.z, (desc.Width*ratio), (height*ratio), D3DCOLOR_XRGB(0, 0, 0));

						if (healthratio > 0.5f)
							g_CCore->GetGraphics()->FillARGB(screen.x - (0.5f*width*ratio), screen.y + (0.5f*height*ratio) + 10, screen.z, (width*ratio*healthratio), (height*ratio), D3DCOLOR_XRGB(0, 255, 0));
						else
							g_CCore->GetGraphics()->FillARGB(screen.x - (0.5f*width*ratio), screen.y + (0.5f*height*ratio) + 10, screen.z, (width*ratio*healthratio), (height*ratio), D3DCOLOR_XRGB(255, 0, 0));
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

		/*D3DXCreateTexture(g_CCore->GetGraphics()->GetDevice(), size.cx + 10, size.cy+5,
			1, D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &ped->nametag);
			*/
		D3DXCreateTexture(g_CCore->GetGraphics()->GetDevice(), 64, 64,
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
			g_CCore->GetGraphics()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
			// now we are ready to render to texture

			//g_CCore->GetGraphics()->GetFont()->DrawTextA(ped->GetName(), 2, 2, 0xFFFFFFFF, false);
			//g_CCore->GetGraphics()->FillARGB(0, 0, 500, 500, 0xFFFF0000);
			//g_CCore->GetGraphics()->DrawTextA(ped->GetName(), 2, 2, D3DCOLOR_XRGB(255,255,255), false);
			//g_CCore->GetGraphics()->GetFont()->DrawTextA(ped->GetName(), 2, 2, D3DCOLOR_XRGB(255, 255, 255), false);
			//g_CCore->GetGraphics()->DrawTextA("petoooo", 2, 2, 0xFFFFFFFF, false);
			//g_CCore->GetGraphics()->GetFont()->DrawTextA("petoooo", 2, 2, 0xFFFFFFFF, false);

			g_CCore->GetGraphics()->DrawTextA(ped->GetName(), 20, 20, D3DCOLOR_XRGB(255, 255, 255), false);
			g_CCore->GetGraphics()->DrawTextA(ped->GetName(), 20, 0, D3DCOLOR_XRGB(255, 255, 255), false);
			g_CCore->GetGraphics()->DrawTextA(ped->GetName(), 20, 40, D3DCOLOR_XRGB(255, 255, 255), false);

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