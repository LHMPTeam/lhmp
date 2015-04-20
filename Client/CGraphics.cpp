#include "CCore.h"
#include "CGraphics.h"
#include "../shared/structures.h"
#include "CColoredText.h"


extern CCore	*g_CCore;

CGraphics::CGraphics()
{
	m_Direct3D		= NULL;
	m_loadingscreen = NULL;
	m_gamemap		= NULL;
	m_mapGUI		= NULL;
	mapScale		= 1.0f;
	m_bShowNameTags = true;
	bShowHud = true;
	m_bUserShowNameTags = true;

	lastScreenshotTime = RakNet::GetTimeMS();
	m_dwRenderTextureBlock = NULL;
}


void	CGraphics::SetRenderTextureStateBlock()
{
	for (int i = 0; i < 2; i++){
		this->m_DirectDevice->BeginStateBlock();

		m_DirectDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_DirectDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_DirectDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_DirectDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_DirectDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_DirectDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
		m_DirectDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		m_DirectDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_DirectDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_DirectDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		m_DirectDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
		m_DirectDevice->SetRenderState(D3DRS_EDGEANTIALIAS, FALSE);
		m_DirectDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		m_DirectDevice->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
		m_DirectDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_DirectDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		//--- 
		m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		//------
		m_DirectDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_DirectDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		//------ important for rendering as sprite---
		m_DirectDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
		m_DirectDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
		m_DirectDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_NONE);

		m_DirectDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		m_DirectDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_DirectDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_DirectDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		m_DirectDevice->SetRenderState(D3DRS_WRAP0, 0);

		// now save it
		if (i == 0)
			this->m_DirectDevice->EndStateBlock(&this->m_dwRenderTextureBlock);
		else
			this->m_DirectDevice->EndStateBlock(&m_dwRenderTextureBlockDeposit);
	}
}

IDirect3DDevice8*	CGraphics::GetDevice()
{
	return this->m_DirectDevice;
}
void CGraphics::Init(IDirect3DDevice8* pDxDevice)
{
	// the most important one
	m_DirectDevice = pDxDevice;
	// place it as first to prevent startup crashes
	m_cFont = new CFont("arial", 9, D3DFONT_BOLD);

	// initialize sprite system / our sprites(textures)
	if (SUCCEEDED(D3DXCreateSprite(m_DirectDevice, &m_sprite)))
	{
		D3DXCreateTextureFromFileExA(m_DirectDevice, "lhmp/loadingscreen.png", D3DX_DEFAULT, D3DX_DEFAULT,
			1, 0, D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR, 0x1000ff00,
			NULL, NULL, &m_loadingscreen);
		D3DXCreateTextureFromFileExA(m_DirectDevice, "lhmp/gamemap.jpg", D3DX_DEFAULT, D3DX_DEFAULT,
			1, 0, D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR, 0x10000000,
			NULL, NULL, &m_gamemap);
		D3DXCreateTextureFromFileExA(m_DirectDevice, "lhmp/gamemap-gui.png", D3DX_DEFAULT, D3DX_DEFAULT,
			1, 0, D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR, 0xff8aff00,
			NULL, NULL, &m_mapGUI);
	}

	HFONT font =  CreateFont(20, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, 0, TEXT("Arial"));  //arial bold, lucida console
	D3DXCreateFont(m_DirectDevice, font, &m_chatfont);

	g_CCore->GetIngameMenu()->Init();

	// Vertex buffer is used during quad rendering
	HRESULT hr = this->GetDevice()->CreateVertexBuffer(6 * sizeof(my_vertex), //Size of memory to be allocated
		//Number of vertices * size of a vertex
		D3DUSAGE_WRITEONLY,  //We never need to read from it so
		//we specify write only, it's faster
		D3D8T_CUSTOMVERTEX,  //Our custom vertex specifier (coordinates & a colour)
		D3DPOOL_MANAGED,     //Tell DirectX to manage the memory of this resource
		&this->m_vb);              //Pointer to our Vertex Buffer, after this call


}

void DebugWeapons()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(i);
		if (ped)
		{
			if (ped->GetEntity())
			{
				PED* gamePED = (PED*)ped->GetEntity();
				Vector3D screen;
				g_CCore->GetGraphics()->CalcScreenPosition(gamePED->object.position, &screen);

				g_CCore->GetGraphics()->DrawTextA("Weapon list (ID,ammo,ammo)", (int)(screen.x + 200.0f), (int)screen.y,0xffff0000, true);

				char aha[250];
				sprintf(aha, "Current ID: %d",ped->GetCurrentWeapon());
				g_CCore->GetGraphics()->DrawTextA(aha, (int) (screen.x + 100.0f), (int) (screen.y + 40.0f), 0xffffffff, true);
				/*for (int i = 0; i < 8; i++)
				{
					SWeapon *wep = ped->GetWeapon(i);
					if (wep)
					{
						char buff[250];
						sprintf(buff, "%d %d %d", wep->wepID, wep->wepLoaded, wep->wepHidden);
						g_CCore->GetGraphics()->DrawTextA(buff, screen.x + 100, screen.y + 20 + (i * 20), 0xffffffff, true);
					}
				}*/

				for (int i = 0; i < 8; i++)
				{
					char buff[250];
					sprintf(buff, "%d %d %d", gamePED->inventary.slot[i].weaponType, gamePED->inventary.slot[i].ammoLoaded, gamePED->inventary.slot[i].ammo);
					g_CCore->GetGraphics()->DrawTextA(buff, (int) (screen.x + 300.0f), (int)(screen.y + 20.0f + (i * 20.0f)), 0xffffffff, true);
					
				}
			}
		}
	}
}

void CGraphics::Render()
{
	/*// TODO - remove this debug bullshits
	if (g_CCore->GetChat()->chatTexture != NULL)
	{
		D3DSURFACE_DESC desc;
		g_CCore->GetChat()->chatTexture->GetLevelDesc(0, &desc);

		Vector3D nullVector;
		nullVector.x = -1984.88f;
		nullVector.y = -4.03f;
		nullVector.z = 23.144f;

		float ratio = (10/Tools::GetDistanceBetween3DPoints(nullVector, g_CCore->GetLocalPlayer()->GetLocalPos()));
		if (ratio < 1.0f)
			ratio = 1.0f;

		Vector3D screen;
		this->CalcScreenPosition(nullVector, &screen);
		//char puf[500];
		//sprintf(puf, "Screen %f %f %f", screen.x, screen.y, screen.z);
		//this->DrawTextA(puf,500,300,0xFFFF0000,true);
		//this->FillARGB(screen.x, screen.y, screen.z, 40, 40, 0xFFFF0000);
		this->RenderTexture(screen.x - (0.5f*desc.Width*ratio), screen.y - (desc.Height*ratio) - 20 - 10, screen.z, (desc.Width*ratio), (desc.Height*ratio), g_CCore->GetChat()->chatTexture);

		//this->FillARGB(screen.x -(80*ratio), screen.y - 20, screen.z, 160*ratio, 20, D3DCOLOR_XRGB(0, 255, 0));
	}*/
	/*

	PED* ped = g_CCore->GetGame()->GetLocalPED();
	// Render debug - Local Playear isAlive
	if (ped)
	{
		char buff[200];
		sprintf(buff, "Local PED: %d", ped->object.isActive);
		this->DrawTextA(buff, 500, 100, 0xFFFF0000, true);
	}*/

	PED* ped = g_CCore->GetGame()->GetLocalPED();
	// Render nametags
	if (ped != NULL)
	{
		if (g_CCore->IsRunning() && g_CCore->m_bIsRespawning == false)
		{
			g_CCore->GetNametags()->Tick();
		}
	}

	// Debug as it fixes problem with font
	FillARGB(0, 0, 1, 1, 0x00000000);

	g_CCore->GetGame()->UpdatePeds();
	g_CCore->GetGame()->UpdateCars();
	if (bShowHud == true && g_CCore->GetIngameMenu()->isActive() == false)
	{
		if (g_CCore->GetLocalPlayer()->GetEntity() != NULL)
		{
			if (this->renderMap == 1)
				this->RenderMap();
		}
		if (this->renderScoreboard == 1)
			this->RenderScoreboard();
		if (this->renderNetStat == 1)
			this->RenderStatistics();

		if(g_CCore->m_bIsGameLoaded == false || g_CCore->m_bIsRespawning == true)
			RenderLoadingScreen();
		
		if (g_CCore->IsRunning())
		{

			g_CCore->GetChat()->Render(m_DirectDevice, m_chatfont);

		}
		
		g_CCore->GetFileTransfer()->Render();
	}
	g_CCore->GetIngameMenu()->Tick();

	//char buff[100];
	/*POINT curPos;
	BOOL result = GetCursorPos(&curPos);
	if (result)
	{
		this->Clear(curPos.x, curPos.y, 10, 10, 0xFFFFFFFF);
	}*/
	/*sprintf(buff, "Status: %f", g_CCore->GetGame()->loadingStatus);
	this->DrawTextA(buff, 700, 200, 0xffffffff, true);*/

	//this->FillARGB(100, 100,0.5f, 50, 50, 0xFF00adef);

}

void CGraphics::RenderLoadingScreen()
{
	//RECT rect;
	D3DXVECTOR2 scaling;
	Vector2D resolution;
	resolution = GetResolution();
	D3DSURFACE_DESC desc;
	m_loadingscreen->GetLevelDesc(0, &desc);
	
	scaling.x =	(((float) resolution.x) / desc.Width);	// 1024
	scaling.y = (((float)resolution.y) / desc.Height); // 768
	D3DXVECTOR2 pos;
	pos.x=0.0f;
	pos.y=0.0f;

	m_sprite->Begin();
	m_sprite->Draw(m_loadingscreen,NULL,&scaling,NULL,0.0,NULL,D3DCOLOR_ARGB(255,255,255,255));
	m_sprite->End();

	//FillARGB(resolution.x*0.10, resolution.y*0.90, resolution.x*0.8*g_CCore->GetGame()->loadingStatus, 20, D3DCOLOR_XRGB(255,255,255));
	FillARGB(0, (int)(resolution.y - 12), (int)(resolution.x), 12, D3DCOLOR_ARGB(150, 0, 0, 0));
	FillARGB(0, (int)(resolution.y - 12), (int)(resolution.x*g_CCore->GetGame()->loadingStatus), 12, D3DCOLOR_XRGB(200, 0, 0));
	//this->Clear(0, resolution.y*0.90, resolution.x*((int)(g_CCore->GetGame()->loadingStatus)), 1, D3DCOLOR_XRGB(200, 200, 200));

	/*char buff[10];
	sprintf(buff, "%.0f%%", g_CCore->GetGame()->loadingStatus * 100);
	g_CCore->GetGraphics()->GetFont()->DrawTextA(buff, resolution.x / 2, (int)(resolution.y*0.90f) - 40, 0xffffffff, false);*/

}

void CGraphics::RenderNametags()
{
	// if nametags are enabled on server-side & client-side (GUI show)
	if (m_bShowNameTags == true && m_bUserShowNameTags == true)
	{
		Vector3D screen;
		char buff[100];
		// for every player
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			CPed* ped = g_CCore->GetPedPool()->Return(i);
			if (ped == NULL) continue;					// free slot
			if (ped->GetEntity() == 0) continue;
			if (ped->IsActive() == 0)					// player after disconnect
			if (ped->GetHealth() == 0.0f) continue;		// death
			Vector3D pos1 = g_CCore->GetLocalPlayer()->GetLocalPos();
			Vector3D pos2 = ped->GetPosition();

			if (Tools::GetDistanceBetween3DPoints(pos1, pos2) > 50.0f) continue;
			Vector3D pos = ped->GetPosition();

			pos.y += 2.25;
			if (ped->IsDucking())
				pos.y -= 1.0;
			CalcScreenPosition(pos, &screen);
			if (screen.z < 1)
			{
				RECT rect;
				sprintf(buff, "%s", ped->GetName());
				int size = GetFontWidth(buff);
				SetRect(&rect, (int)ceil(screen.x) - (size / 2), (int)ceil(screen.y), (int)ceil(screen.x) + 100, (int)ceil(screen.y) + 100);

				//m_chatfont->DrawTextA(buff, strlen(buff), &rect, DT_SINGLELINE, D3DCOLOR_XRGB(255, 255, 255));
				g_CCore->GetGraphics()->GetFont()->DrawTextA(buff, rect.left,rect.top, D3DCOLOR_XRGB(255, 255, 255));
				D3DCOLOR farba;
				if ((ped->GetHealth() / 200) > 0.5)
				{
					farba = D3DCOLOR_ARGB(0, 0, 152, 0);
				}
				else
				{
					farba = D3DCOLOR_ARGB(0, 152, 0, 0);
				}
				D3DRECT rect2 = { (long)screen.x - 20, (long)screen.y + 19, (long)screen.x + 22, (long)screen.y + 25 };
				m_DirectDevice->Clear(1, &rect2, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 0, 0);
				rect2.x1 += 1; rect2.y1 += 1; rect2.x2 -= 1; rect2.y2 -= 1;
				m_DirectDevice->Clear(1, &rect2, D3DCLEAR_TARGET, farba, 0, 0);
				rect2.x2 = (long)(rect2.x1 + ceil(40 * (ped->GetHealth() / 200)));
				if ((ped->GetHealth() / 200) > 0.5)
				{
					farba = D3DCOLOR_ARGB(0, 0, 200, 0);
				}
				else
				{
					farba = D3DCOLOR_ARGB(0, 200, 0, 0);
				}
				m_DirectDevice->Clear(1, &rect2, D3DCLEAR_TARGET, farba, 0, 0);
			}
		}
	}
}

Vector2D CGraphics::GetResolution()
{
	D3DVIEWPORT8 viewport;
	m_DirectDevice->GetViewport(&viewport);
	Vector2D screen;
	screen.x = viewport.Width;
	screen.y = viewport.Height;
	return screen;
}

void CGraphics::TakeScreenshot()
{
	if (RakNet::GetTimeMS() - lastScreenshotTime < 200) return;
	lastScreenshotTime = RakNet::GetTimeMS();

	D3DDISPLAYMODE mode;
	m_DirectDevice->GetDisplayMode(&mode);
	HRESULT result;
	LPDIRECT3DSURFACE8 screen;
	if (FAILED(result=m_DirectDevice->CreateImageSurface(mode.Width,mode.Height,D3DFMT_A8R8G8B8,&screen)))
		return;
	m_DirectDevice->GetFrontBuffer(screen);
	char filename[255];

	time_t timeObj;
    time(&timeObj);
	//tm *pTime = gmtime(&timeObj);
	tm *pTime = localtime(&timeObj);
//    char buffer[100];
	sprintf(filename, "lhmp/screen-%d-%d-%d-%d-%d-%d.bmp", pTime->tm_year, pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
	if(bIsFullscreen)
	{
		D3DXSaveSurfaceToFile(filename, D3DXIFF_BMP, screen, NULL, NULL);
	} else
	{
		Vector2D screensize = GetResolution();
		POINT point;
		point.x = 0;
		point.y = 0;
		ScreenToClient(hwnAppWindow,&point);
		RECT clientscreen;
		clientscreen.left = abs(point.x);
		clientscreen.top = abs(point.y);
		clientscreen.bottom = abs(point.y)+screensize.y;
		clientscreen.right = abs(point.x)+screensize.x;
		//D3DXSaveSurfaceToFile(filename,D3DXIFF_JPG,screen,NULL,NULL);

		D3DXSaveSurfaceToFile(filename, D3DXIFF_BMP, screen, NULL, &clientscreen);
	}
	screen->Release();
	char buff[255];
	sprintf(buff, "#f31d2fScreenshot #e3e3e3""%s""#f31d2f taken.", filename);
	g_CCore->GetChat()->AddMessage(buff);
}

void CGraphics::CalcScreenPosition(Vector3D world,Vector3D *screen)
{
	// funguje dobre
	D3DXVECTOR3 out;
	D3DVIEWPORT8 viewPort;
	m_DirectDevice->GetViewport(&viewPort);
	D3DXVECTOR3 input =  D3DXVECTOR3(world.x,world.y,world.z); 
	D3DXMATRIX mProjection,mView,mWorld;
	m_DirectDevice->GetTransform(D3DTS_VIEW,&mView);
	D3DXMatrixIdentity(&mWorld);
	//m_DirectDevice->GetTransform(D3DTS_WORLD,&mWorld);
	m_DirectDevice->GetTransform(D3DTS_PROJECTION,&mProjection);
	
	D3DXVec3Project(&out,&input,&viewPort,&mProjection,&mView,&mWorld);
	screen->x = out.x;
	screen->y = out.y;
	screen->z = out.z;

}

int	CGraphics::GetFontWidth(char text[])
{
	if (!m_chatfont)
		return 0;
	RECT rect;
	SetRect(&rect,0,0,1000,1000);
	m_chatfont->DrawTextA(text,strlen(text),&rect,DT_NOCLIP | DT_SINGLELINE|DT_CALCRECT,D3DCOLOR_XRGB(255,255,255));
	return rect.right-rect.left;
}

void	CGraphics::DrawText(char text[],int x,int y,D3DCOLOR color,bool ifShadow,LPD3DXFONT font)
{
	if (!m_chatfont)
		return;
	RECT rect;
	SetRect(&rect,x,y,x+1000,y+1000);
	if(ifShadow == true)
	{
		SetRect(&rect, x + 1, y, x + 1000, y + 1000);
		font->DrawTextA(text, strlen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x - 1, y, x + 1000, y + 1000);
		font->DrawTextA(text, strlen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x, y+1, x + 1000, y + 1000);
		font->DrawTextA(text, strlen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x, y - 1, x + 1000, y + 1000);
		font->DrawTextA(text, strlen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x, y, x + 1000, y + 1000);
	}
	font->DrawTextA(text, strlen(text), &rect, DT_NOCLIP | DT_SINGLELINE, color);
}
void	CGraphics::DrawText(char text[],int x,int y,D3DCOLOR color,bool ifShadow)
{
	DrawText(text,x,y,color,ifShadow,m_chatfont);
}
int		CGraphics::GetLetterWidth(char c)
{
	char buf[2];
	sprintf(buf,"%c",c);
	return this->GetFontWidth(buf);
}
std::string	CGraphics::GetLastColorInText(char* text,int maxlen)
{
	for (int i = maxlen; i > 0; i--)
	{
		if (text[i] == '#')
		{
			if (maxlen - i >= 7)
			{
				if (text[i + 1] != '#')
				{
					bool stop = false;
					for (short e = i + 1; e < i + 6; e++)
					{
						//if (!(((text[e] >= 'a') && (text[e] <= 'f')) || ((text[e] >= 'A') && (text[e] <= 'F')) || ((text[e] >= '0') && (text[e] <= '9'))))
						if (Tools::isHexChar(text[e]) == false)
						{
							stop = true;
							break;
						}

					}
					if (stop == false)
					{
						std::string str = text;
						return str.substr(i, 7);
					}
				}
			}
		}
	}
	return "";
}
int		CGraphics::GetStrlenForWidth(int width, char* text)
{
	int size = 0;
	int len = strlen(text);
	for (int i = 0; i < len; i++)
	{
		if (text[i] == '#')
		{
			if (len - i >= 7)
			{
				for (short e = i+1; e < i+6; e++)
				{
					//if (!(((text[e] >= 'a') && (text[e] <= 'f')) || ((text[e] >= 'A') && (text[e] <= 'F')) || ((text[e] >= '0') && (text[e] <= '9'))))
					if (Tools::isHexChar(text[e]) == false)
					{
						size += this->GetLetterWidth(text[i]);
						if (size > width)
							return i;
						continue;
					}

				}
				i += 6; // skipping 6 letters (which are used for color tag)
				continue;
			}
			size += this->GetLetterWidth(text[i]);
			if (size > width)
				return i;
		}
		else
		{
			size += this->GetLetterWidth(text[i]);
			if(size > width)
				return i;
		}
	}
	return len;
}
int	CGraphics::GetColoredTextWidth(char text[])
{
	/*if (Tools::IsEmptyString(text) == true)
		return -1;
		char* akt;
		char* pch = strchr(text, '#');
		bool next = false;
		int size = 0;
		if (pch == NULL)
		{
			return GetFontWidth(text);
		}
		while (pch != NULL)
		{
			for (short i = 1; i < 7; i++)
			{
				//if (!(((pch[i] >= 'a') && (pch[i] <= 'f')) || ((pch[i] >= 'A') && (pch[i] <= 'F')) || ((pch[i] >= '0') && (pch[i] <= '9'))))
				if (Tools::isHexChar(pch[i]) == false)
				{
					next = true;
					break;
				}

			}
			akt = pch;
			// if we have wrong HEX color
			if (next == true)
			{
				pch = strchr(pch + 1, '#');
				if (pch == NULL)
				{
					size += GetFontWidth(akt);
					break;
				}
				else
				{
					char buf[255];
					sprintf(buf, "%s", akt);
					buf[(pch - akt)] = 0x0;	// null terminated
					//DrawText(buf, x, y, farba, ifShadow, font);
					size += GetFontWidth(buf);
					//x += size;
					next = false;
					continue;
				}
			}
			//farba = 0xFFFFFFFF;
			//farba = Tools::GetARGBFromString(pch + 1);
			pch = strchr(pch + 1, '#');
			if (pch == NULL)
			{
				size += GetFontWidth(akt+7);
				break;
			}
			else
			{
				char buf[255];
				sprintf(buf, "%s", akt + 7);
				buf[(pch - akt) - 7] = 0x0;	// null terminated
				//DrawText(buf, x, y, farba, ifShadow, font);
				size += GetFontWidth(buf);
				//x += size;
			}

		}
	
	return size;*/
	int size = 0;
	if (Tools::IsEmptyString(text) != NULL)
	{
		int start;
		char* pointer = text;
		while (1 == 1)
		{
			start = Tools::getFirstColorStamp(text);
			if (start == -1)
			{
				size += GetFontWidth(pointer);
				break;
			}
			else if (start == 0)
			{
				pointer = pointer + 7;
			}
			else {
				pointer[start] = 0x0;
				size += GetFontWidth(pointer);
				pointer[start] = '#';
				pointer = pointer + start + 7;
			}
		}
	}
	return size;
}
void	CGraphics::DrawText(char text[],int x,int y,D3DCOLOR color,bool ifShadow,bool colored,LPD3DXFONT font)
{
	if(Tools::IsEmptyString(text) == true)
		return;
	if(colored == true)
	{
		char* akt;
		char* pch	=	strchr(text,'#');
		bool next	= false;
		int size = NULL;
		D3DCOLOR farba;
		farba = color;
		//if(text[0] != '#')
		if (text != pch)
		{
			if (pch == NULL)
			{
				DrawText(text,x,y,color,ifShadow,font);

			}
			else
			{
				char bufA[255];
				sprintf(bufA,"%s",text);
				bufA[(pch-text)] = 0x0;	// null terminated
				DrawText(bufA,x,y,color,ifShadow,font);
				size = GetFontWidth(bufA);
				x += size;
			}
		}
		while(pch != NULL)
		{
/*			for(short i = 1; i < 7;i++)
			{
				if(!(((pch[i] >= 'a') && (pch[i] <= 'f')) || ((pch[i] >= 'A') && (pch[i] <= 'F')) || ((pch[i] >= '0') && (pch[i] <= '9'))))
				{
					next = true;
					break;
				}

			}
			*/
			akt = pch;
			
			if (Tools::isHEXStamp(pch) == false)
			{
	            pch	 = strchr(pch+1,'#');
				if(pch == NULL)
				{
					DrawText(akt,x,y,farba,ifShadow,font);
					break;
				} else
				{
					char buf[255];
					sprintf(buf,"%s",akt);
					buf[(pch-akt)] = 0x0;	// null terminated
					DrawText(buf,x,y,farba,ifShadow,font);
					size = GetFontWidth(buf);
					x += size;
					//next = false;
    				continue;
				}
			}
			else
			{
				if (Tools::IsEmptyString(akt + 7) == true)
					return;
				farba = 0xFFFFFFFF;
				farba = Tools::GetARGBFromString(pch + 1);
				pch = strchr(pch + 1, '#');
				if (pch == NULL)
				{
					DrawText(akt + 7, x, y, farba, ifShadow, font);
					break;
				}
				else
				{
					char buf[255];
					sprintf(buf, "%s", akt + 7);
					buf[(pch - akt) - 7] = 0x0;	// null terminated
					DrawText(buf, x, y, farba, ifShadow, font);
					size = GetFontWidth(buf);
					x += size;
				}
			}
		}
	} else
	{
		DrawText(text,x,y,color,ifShadow,font);
	}
}
void	CGraphics::DrawText(char text[],int x,int y,D3DCOLOR color,bool ifShadow,bool colored)
{
	DrawText(text,x,y,color,ifShadow,colored,m_chatfont);
}

void CGraphics::FillARGB(int x, int y, int w, int h, D3DCOLOR color)
{
	this->GetDevice()->SetTexture(0, NULL);

	m_DirectDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_DirectDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_DirectDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_DirectDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_DirectDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
	m_DirectDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_DirectDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_DirectDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_DirectDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
	m_DirectDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	m_DirectDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_DirectDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_DirectDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_DirectDevice->SetRenderState(D3DRS_EDGEANTIALIAS, TRUE);

	my_vertex g_square_vertices[] = {
		{ (float)x, (float)(y + h), 0.0f, 0.0f, color }, // x, y, z, rhw, color
		{ (float)x, (float)y, 0.0f, 0.0f, color },
		{ (float)(x + w), (float)(y + h), 0.0f, 0.0f, color },
		{ (float)(x + w), (float)y, 0.0f, 0.0f, color }
	};

	unsigned char*	buffer;
	m_vb->Lock(0, 0, &buffer, 0);
	memcpy(buffer, g_square_vertices, sizeof(g_square_vertices));
	m_vb->Unlock();


	this->GetDevice()->SetPixelShader(NULL);
	this->GetDevice()->SetVertexShader(D3D8T_CUSTOMVERTEX);

	this->GetDevice()->SetStreamSource(0, m_vb, sizeof(my_vertex));

	//Now we're drawing a Triangle Strip, 4 vertices to draw 2 triangles.
	this->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


}

void CGraphics::FillARGB(int x, int y,float z,  int w, int h, D3DCOLOR color)
{
	this->GetDevice()->SetTexture(0, NULL);

	m_DirectDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_DirectDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_DirectDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_DirectDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_DirectDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_DirectDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
	m_DirectDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_DirectDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_DirectDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_DirectDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
	m_DirectDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	m_DirectDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_DirectDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_DirectDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_DirectDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_DirectDevice->SetRenderState(D3DRS_EDGEANTIALIAS, TRUE);

	my_vertex g_square_vertices[] = {
		{ (float)x, (float)(y + h), z, 0.0f, color }, // x, y, z, rhw, color
		{ (float)x, (float)y, z, 0.0f, color },
		{ (float)(x + w), (float)(y + h), z, 0.0f, color },
		{ (float)(x + w), (float)y, z, 0.0f, color }
	};

	unsigned char*	buffer;
	m_vb->Lock(0, 0, &buffer, 0);
	memcpy(buffer, g_square_vertices, sizeof(g_square_vertices));
	m_vb->Unlock();


	this->GetDevice()->SetPixelShader(NULL);
	this->GetDevice()->SetVertexShader(D3D8T_CUSTOMVERTEX);

	this->GetDevice()->SetStreamSource(0, m_vb, sizeof(my_vertex));

	//Now we're drawing a Triangle Strip, 4 vertices to draw 2 triangles.
	this->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


}

// Render texture with precalculated position
void CGraphics::RenderTexture(int x, int y, float z, int w, int h, LPDIRECT3DTEXTURE8 texture, unsigned char alpha)
{
	if (this->m_dwRenderTextureBlock == NULL)
		this->SetRenderTextureStateBlock();

	// save all current states
	this->m_DirectDevice->CaptureStateBlock(m_dwRenderTextureBlockDeposit);
	// apply states needed to correct render
	this->m_DirectDevice->ApplyStateBlock(this->m_dwRenderTextureBlock);

#define OURVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE)
	struct thisVertex {
		float x, y, z;
		float rhw;
		DWORD color;
		float tu, tv;
	};
	this->GetDevice()->SetTexture(0, texture);
	
	// x, y, z, rhw, color
	DWORD color = (alpha << 24);
	thisVertex g_square_vertices[] = {
		{ (float)x, (float)y, z, 1.0f,color, 0.0f, 0.0f },
		{ (float)(x + w), (float)y, z, 1.0f, color, 1.0f, 0.0f },
		{ (float)x, (float)(y + h), z, 1.0f, color, 0.0f, 1.0f },
		{ (float)(x + w), (float)(y + h), z, 1.0f, color, 1.0f, 1.0f }
	};

	unsigned char*	buffer;
	m_vb->Lock(0, 0, &buffer, 0);
	memcpy(buffer, g_square_vertices, sizeof(g_square_vertices));
	m_vb->Unlock();


	this->GetDevice()->SetPixelShader(NULL);
	this->GetDevice()->SetVertexShader(OURVERTEX);

	this->GetDevice()->SetStreamSource(0, m_vb, sizeof(thisVertex));

	//Now we're drawing a Triangle Strip, 4 vertices to draw 2 triangles.
	this->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// SetTexture NULL is important, it makes texture availables Release() function
	/*m_DirectDevice->SetTexture(0,NULL);
	/m_DirectDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	m_DirectDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	*/

	// in the end just reset the previous states
	this->m_DirectDevice->ApplyStateBlock(m_dwRenderTextureBlockDeposit);

}

void CGraphics::Clear(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rec = { x,y,x+w,y+h};
	m_DirectDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void CGraphics::RenderMap()
{
	if (!g_CCore->GetGame()->IsTabMapEnabled())
		return;
	if (g_CCore->m_bIsRespawning)
		return;
	renderMap = 0;
	Vector2D screen = this->GetResolution();
//	Vector2D onscreenpos;
	Vector3D localplayer = g_CCore->GetLocalPlayer()->GetLocalPos();

	D3DXVECTOR2 translation;
	translation.x = 0;
	translation.y = 0;
	D3DXVECTOR2 scaling;
	scaling.x = mapScale;
	scaling.y = mapScale;
	int posX = 2408 - (int)((screen.x / 2.0f) / mapScale);
	int posY = 954 - (int)((screen.y / 2.0f) / mapScale);
	posX += (int)((localplayer.x / 1.665f));
	posY -= (int)((localplayer.z / 1.944f));

	if (posX < 0)
	{
		translation.x = (float)(posX *(-1)) / 2;
		posX = 0;
	}
	if (posY < 0)
	{
		translation.y = (float)(posY *(-1)) / 2;
		posY = 0;
	}
	if (posX > 3263)
	{
		translation.x = (float)(posX - 3263);
		posX = 0;
	}
	if (posY > 2044)
	{
		translation.y = (float)(posY - 2044);
		posY = 0;
	}
	RECT area;
	SetRect(&area, posX, posY, posX + (int)(screen.x / mapScale), posY + (int)(screen.y / mapScale));

	if (g_CCore->GetGraphics()->GetSprite()->Begin() != S_OK)
		return;
	g_CCore->GetGraphics()->GetSprite()->Draw(m_gamemap, &area, &scaling, NULL, 0.0, &translation, D3DCOLOR_ARGB(200, 255, 255, 255));
	g_CCore->GetGraphics()->GetSprite()->End();

	// draw cars
	Vector3D newpos, vehpos;
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(i);
		if (veh != NULL)
		{
			vehpos = veh->GetPosition();
			newpos.x = (localplayer.x - vehpos.x) / 1.665f*mapScale;
			newpos.z = (localplayer.z - vehpos.z) / 1.944f*mapScale;
			translation.x = -9-newpos.x + screen.x/2;
			translation.y = -11+newpos.z + screen.y/2;
			//this->FillARGB(-newpos.x - 5 + (screen.x / 2), +newpos.z - 5 + (screen.y / 2), 10, 10, D3DCOLOR_XRGB(100, 100, 100));

			scaling.x = 0.5f;
			scaling.y = 0.5f;
			SetRect(&area, 0, 0, 36, 44);
			g_CCore->GetGraphics()->GetSprite()->Begin();
			g_CCore->GetGraphics()->GetSprite()->Draw(m_mapGUI, &area, &scaling, NULL, 0.0, &translation, D3DCOLOR_XRGB(255, 255, 255));
			g_CCore->GetGraphics()->GetSprite()->End();
		}
	}

	D3DXMATRIX center, transform, rot, result,scale;
	Vector3D playerRot;
	// draw players
	SetRect(&area, 71, 0, 107, 44);
	scaling.x = 0.75f;
	scaling.y = 0.75f;
	D3DXMatrixTranslation(&center, -12, -15, 0);
	D3DXMatrixScaling(&scale, 0.7, 0.7, 1);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(i);
		if (ped != NULL)
		{
			vehpos = ped->GetPosition();
			newpos.x = (localplayer.x - vehpos.x) / 1.665f*mapScale;
			newpos.z = (localplayer.z - vehpos.z) / 1.944f*mapScale;
			D3DXMatrixTranslation(&transform, -newpos.x + screen.x / 2, newpos.z+screen.y / 2, 0);
			
			if (ped->InCar == -1)
			{
				playerRot = ped->GetRotation();
			}
			else
			{
				CVehicle* veh = g_CCore->GetVehiclePool()->Return(ped->InCar);
				if (veh)
				{
					playerRot = veh->GetRotation();
				}
			}
			double goalRot = acos(playerRot.z);
			if (playerRot.x < 0)
				goalRot = 6.28 - goalRot;
			D3DXMatrixRotationZ(&rot, (float)goalRot);
			D3DXMatrixMultiply(&rot, &center, &rot);
			D3DXMatrixMultiply(&rot, &scale, &rot);
			D3DXMatrixMultiply(&result, &rot, &transform);

			g_CCore->GetGraphics()->GetSprite()->DrawTransform(m_mapGUI, &area, &result, D3DCOLOR_XRGB(255, 255, 255));
		}
	}
	// draw player

	if (g_CCore->GetGraphics()->GetSprite()->Begin() != S_OK)
		return;
	SetRect(&area, 36, 0, 71, 44);
	// center is needed to rotate sprite around its center
	D3DXMatrixTranslation(&center, -12, -15, 0);
	D3DXMatrixTranslation(&transform, (float)(screen.x / 2),(float) (screen.y / 2), 0.0f);
	D3DXMatrixScaling(&scale, 0.7, 0.7, 1);

	playerRot = g_CCore->GetLocalPlayer()->GetLocalRot();
	double goalRot = acos(playerRot.z);
	if (playerRot.x < 0)
		goalRot = 6.28 - goalRot;
	D3DXMatrixRotationZ(&rot, (float)goalRot);
	D3DXMatrixMultiply(&rot, &center, &rot);
	D3DXMatrixMultiply(&rot, &scale, &rot);
	D3DXMatrixMultiply(&result, &rot,&transform);
	
	g_CCore->GetGraphics()->GetSprite()->DrawTransform(m_mapGUI, &area,&result, D3DCOLOR_XRGB(255, 255, 255));
	g_CCore->GetGraphics()->GetSprite()->End();
}

void CGraphics::RenderScoreboard()
{
	renderScoreboard = 0;
	Vector2D screen = this->GetResolution();
	int width = (int)(((screen.x*0.2) < 250)?250:(screen.x*0.2)); // min width is 250px
	int x = (int)((screen.x-width)/2), y = (int)(screen.y*0.4);
	int numRendered = 0;
	char buff[255];
	int numLines = 2;
	// Render title
	FillARGB(x, y - 25, width, 25, 0xffdb0000);
	g_CCore->GetGraphics()->GetFont()->DrawText(g_CCore->GetNetwork()->GetServerName(), x + 10, y - 22, 0xffffffff, true);
	//DrawText(g_CCore->GetNetwork()->GetServerName(),x+10,y-23,0xffffffff,true);
	// Render content
	for(int ID = 0; ID < MAX_PLAYERS;ID++)
	{
		CPed* ped =  g_CCore->GetPedPool()->Return(ID);
		if(ped == NULL) continue;
		if(ped->IsActive() == 0) continue;
		numLines++;
	}
	FillARGB(x, y, width, 20 + (20 * numLines), 0x60000000);
	// Render navigation
	g_CCore->GetGraphics()->GetFont()->DrawText("ID", x + (width * 0.02), y + 10, 0xffffffff, true);
	g_CCore->GetGraphics()->GetFont()->DrawText("Nickname", x + (width * 0.10), y + 10, 0xffffffff, true);
	g_CCore->GetGraphics()->GetFont()->DrawText("Ping", x + (width * 0.87), y + 10, 0xffffffff, true);
	// Render local player
	sprintf(buff,"%i",g_CCore->GetLocalPlayer()->GetOurID());
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + (width * 0.02), 20 + y + 10, 0xffffffff, true);
	g_CCore->GetGraphics()->GetFont()->DrawText(g_CCore->GetNetwork()->GetNick(), x + (width * 0.10), 20 + y + 10, 0xffffffff, true);
	sprintf(buff,"%i",g_CCore->GetLocalPlayer()->GetPing());
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + (width * 0.87), 20 + y + 10, 0xffffffff, true);

	//y = screen.y*0.4;
	for(int ID = 0; ID < MAX_PLAYERS;ID++)
	{
		CPed* ped =  g_CCore->GetPedPool()->Return(ID);
		if(ped == NULL) continue;
		if(ped->IsActive() == 1)
		{
			sprintf(buff,"%i",ID);
			g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + (width * 0.02), 20 + y + 30 + (numRendered * 20), 0xffffffff, true);
			g_CCore->GetGraphics()->GetFont()->DrawText(ped->GetName(), x + (width * 0.10), 20 + y + 30 + (numRendered * 20), 0xffffffff, true);
			sprintf(buff,"%i",ped->GetPing());
			g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + (width * 0.87), 20 + y + 30 + (numRendered * 20), 0xffffffff, true);
			numRendered++;
		}
	}
}

void CGraphics::RenderStatistics()
{
	renderNetStat = 0;
	//RakNet::RakNetStatistics *rs;
	//g_CCore->GetNetwork()->GetPeer()->GetStatistics(RakNet::UNASSIGNED_SYSTEM_ADDRESS,rs);
	//rs->
	//RakNet::StatisticsToString
	//RakNet::StatisticsToString
	using namespace RakNet;
	RakNetStatistics *pRakStats = g_CCore->GetNetwork()->GetPeer()->GetStatistics(UNASSIGNED_SYSTEM_ADDRESS);

	char buff[255];
	unsigned int uBytesPerSecondReceived = (unsigned int)pRakStats->valueOverLastSecond[ACTUAL_BYTES_RECEIVED];
	unsigned int uBytesPerSecondSent = (unsigned int)pRakStats->valueOverLastSecond[ACTUAL_BYTES_SENT];
	unsigned int uTotalBytesReceived = (unsigned int)pRakStats->runningTotal[ACTUAL_BYTES_RECEIVED];
	unsigned int uTotalBytesSent = (unsigned int)pRakStats->runningTotal[ACTUAL_BYTES_SENT];
	unsigned int uConnectionTime = (unsigned int)((RakNet::GetTimeUS() - pRakStats->connectionStartTime) / 1000000);
	float fCurrentPacketLoss = pRakStats->packetlossLastSecond;
	float fAveragePacketLoss = pRakStats->packetlossTotal;

	Vector2D screen = GetResolution();
	int width = 300;
	int x = (screen.x - width) / 2, y = screen.y / 2;
	// Title
	FillARGB(x, y - 25, width, 25, 0xffdb0000);
	g_CCore->GetGraphics()->GetFont()->DrawText("Statistics", x + 10, y - 22, 0xffffffff, true);

	FillARGB(x, y, width, 160, 0x60000000);
	sprintf(buff, "Bytes Per Second Received: %.2f%sB", Tools::GetMetricUnitNum((float)uBytesPerSecondReceived), Tools::MetricUnits[Tools::GetMetricUnitIndex((float)uBytesPerSecondReceived)]);
	//this->DrawTextA(buff,x+10,y+10,D3DCOLOR_XRGB(255,255,255),true);
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + 10, y + 10, D3DCOLOR_XRGB(255, 255, 255), true);
	sprintf(buff, "Bytes Per Second Sent: %.2f%sB", Tools::GetMetricUnitNum((float)uBytesPerSecondSent), Tools::MetricUnits[Tools::GetMetricUnitIndex((float)uBytesPerSecondSent)]);
	//this->DrawTextA(buff,x+10,y+30,D3DCOLOR_XRGB(255,255,255),true);
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + 10, y + 30, D3DCOLOR_XRGB(255, 255, 255), true);
	sprintf(buff, "Total Bytes Received: %.2f%sB", Tools::GetMetricUnitNum((float)uTotalBytesReceived), Tools::MetricUnits[Tools::GetMetricUnitIndex((float)uTotalBytesReceived)]);
	//this->DrawTextA(buff,x+10,y+50,D3DCOLOR_XRGB(255,255,255),true);
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + 10, y + 50, D3DCOLOR_XRGB(255, 255, 255), true);
	sprintf(buff, "Total Bytes Sent: %.2f%sB", Tools::GetMetricUnitNum((float)uTotalBytesSent), Tools::MetricUnits[Tools::GetMetricUnitIndex((float)uTotalBytesSent)]);
	//this->DrawTextA(buff,x+10,y+70,D3DCOLOR_XRGB(255,255,255),true);
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + 10, y + 70, D3DCOLOR_XRGB(255, 255, 255), true);
	sprintf(buff,"Current Packet Loss: %.02f",fCurrentPacketLoss);
	//this->DrawTextA(buff,x+10,y+90,D3DCOLOR_XRGB(255,255,255),true);
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + 10, y + 90, D3DCOLOR_XRGB(255, 255, 255), true);
	sprintf(buff,"Average Packet Loss: %.02f",fAveragePacketLoss);
	//this->DrawTextA(buff,x+10,y+110,D3DCOLOR_XRGB(255,255,255),true);
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + 10, y + 110, D3DCOLOR_XRGB(255, 255, 255), true);
	sprintf(buff,"Connection Time: %ds",uConnectionTime);
	//this->DrawTextA(buff,x+10,y+130,D3DCOLOR_XRGB(255,255,255),true);
	g_CCore->GetGraphics()->GetFont()->DrawText(buff, x + 10, y + 130, D3DCOLOR_XRGB(255, 255, 255), true);

    /*sprintf(szDispBuf,"--- Network Stats ---\n" \
                                        "\n" \
                                        "Bytes Per Second Received: %d\n" \
                                        "Bytes Per Second Sent: %d\n" \
                                        "Total Bytes Received: %d\n" \
                                        "Total Bytes Sent: %d\n" \
                                        "Current Packet Loss: %.02f\n" \
                                        "Average Packet Loss: %.02f\n" \
                                        "Connection Time: %ds\n" \
                                        "\n", 
                                        uBytesPerSecondReceived, uBytesPerSecondSent, 
                                        uTotalBytesReceived, uTotalBytesSent, 
                                        fCurrentPacketLoss, fAveragePacketLoss, 
                                        uConnectionTime);
										*/
}


void CGraphics::ToggleInGameHud(bool b)
{
	if (b != 1)
		(*(byte*)0x006613D4) |= 1 << 0;
	else
		(*(byte*)0x006613D4) &= ~(1 << 0);
}
bool CGraphics::IsInGameHudVisible()
{
	return (((*(byte*)0x006613D4) & 1 << 0) == 0);
}

__declspec(noinline) void CGraphics::OnLostDevice()
{
	if (m_chatfont)
		m_chatfont->OnLostDevice();
	if (m_sprite)
		m_sprite->OnLostDevice();
	if (m_loadingscreen != NULL)
	{
		m_loadingscreen->Release();
		m_loadingscreen = NULL;
	}
	if (m_gamemap != NULL)
	{
		m_gamemap->Release();
		m_gamemap = NULL;
	}	
	if (m_mapGUI != NULL)
	{
		m_mapGUI->Release();
		m_mapGUI = NULL;
	}
	g_CCore->GetChat()->OnLostDevice();

	g_CCore->GetIngameMenu()->OnLostDevice();
	this->m_cFont->OnDeviceLost();

	g_CCore->GetNametags()->OnLostDevice();

	if (this->m_dwRenderTextureBlock)
		this->m_DirectDevice->DeleteStateBlock(m_dwRenderTextureBlock);
	if (m_dwRenderTextureBlockDeposit)
		this->m_DirectDevice->DeleteStateBlock(m_dwRenderTextureBlockDeposit);
	this->m_dwRenderTextureBlock = NULL;
	this->m_dwRenderTextureBlockDeposit = NULL;
}
void CGraphics::OnResetDevice()
{
	if (m_chatfont)
		m_chatfont->OnResetDevice();
	if (m_sprite)
		m_sprite->OnResetDevice();
	D3DXCreateTextureFromFileExA(m_DirectDevice, "lhmp/loadingscreen.png", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR, 0x1000ff00,
		NULL, NULL, &m_loadingscreen);

	D3DXCreateTextureFromFileExA(m_DirectDevice, "lhmp/gamemap.jpg", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR, 0x10000000,
		NULL, NULL, &m_gamemap);

	D3DXCreateTextureFromFileExA(m_DirectDevice, "lhmp/gamemap-gui.png", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR, 0xff8aff00,
		NULL, NULL, &m_mapGUI);
	g_CCore->GetChat()->OnResetDevice();

	g_CCore->GetIngameMenu()->OnResetDevice();

	this->m_cFont->OnReset();
}


ID3DXSprite*	CGraphics::GetSprite()
{
	return m_sprite;
}

void	CGraphics::SetDirect3D(IDirect3D8* direct)
{
	this->m_Direct3D = direct;
}

bool	CGraphics::IsShowNameTags()
{
	return this->m_bShowNameTags;
}
void	CGraphics::SetShowNameTags(bool nm)
{
	this->m_bShowNameTags = nm;
}

void	CGraphics::DrawColoredText(CColoredText* text, int x, int y, bool ifShadow)
{
	int drawn = 0;
	text->StartGetting();
	while (CColoredStruct* strr = text->GetNext())
	{
		this->DrawTextA(strr->text, x+drawn,y, strr->color, true);
		drawn += strr->width;
	}
}



CFont*	CGraphics::GetFont()
{
	return this->m_cFont;
}