#include "d3d8.h"
#include "CCore.h"

extern CCore *g_CCore;

class CDirect3DDevice8Proxy: public IDirect3DDevice8
{
private:
	IDirect3DDevice8*	p_Dev;
	LPD3DXFONT			chatFont;
	LPDIRECT3DTEXTURE8	pTexture;
	LPD3DXSPRITE		sprite;
public:
	CDirect3DDevice8Proxy(IDirect3DDevice8* DID) : p_Dev(DID)
	{
		g_CCore->GetGraphics()->Init(this);
		/*D3DXCreateFont(this,CreateFont(18,0,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,0,TEXT("Arial")),&chatFont);
		if (SUCCEEDED(D3DXCreateSprite(this,&sprite)))
		{
			//D3DXCreateTextureFromFileA( d3ddev, "test.bmp", &pTexture);
			//D3DXCreateTextureFromFileExA( d3ddev, "test.bmp", &pTexture);
			D3DXCreateTextureFromFileExA(this, "lhmp/loadingscreen.png",D3DX_DEFAULT, D3DX_DEFAULT, 
			   1, 0, D3DFMT_A8R8G8B8, 
			   D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
			   D3DX_FILTER_LINEAR, 0x10000000, 
			   NULL, NULL, &pTexture);

		}*/
		//MessageBox(NULL,"Device start","Ahoj",MB_OK);
	}
	
		/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->QueryInterface(riid, ppvObj);
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		//ATLTRACE(TEXT("MyDirectDevice::AddRef"));
		return p_Dev->AddRef();
	}

	STDMETHOD_(ULONG,Release)()
	{
		//ATLTRACE(TEXT("MyDirectDevice::Release"));
		ULONG count = p_Dev->Release();
		if(0 == count)
			delete this;

		return count;
	}
	/***	IDirect3DDevice8 methods***/
	STDMETHOD(TestCooperativeLevel)()
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->TestCooperativeLevel();
	}
	STDMETHOD_(UINT,GetAvailableTextureMem)()
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetAvailableTextureMem();
	}
	STDMETHOD(ResourceManagerDiscardBytes)(DWORD bytes)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->ResourceManagerDiscardBytes(bytes);
	}
	STDMETHOD(GetDirect3D)(IDirect3D8 **ppdev)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetDirect3D(ppdev);
	}
	STDMETHOD(GetDeviceCaps)(D3DCAPS8 *pCaps)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetDeviceCaps(pCaps);
	}
	STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE *pMode)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetDisplayMode(pMode);
	}
	/*STDMETHOD(GetDeviceCaps)(D3DCAPS8 *pCaps)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetDeviceCaps(pCaps);
	}*/
	STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS *pParam)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetCreationParameters(pParam);
	}
	STDMETHOD(SetCursorProperties)(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface8 *pCursorBitmap)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetCursorProperties(XHotSpot,YHotSpot,pCursorBitmap);
	}
	STDMETHOD_(void,SetCursorPosition)(int x,int y, DWORD flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetCursorPosition(x,y, flags);
	}
	//STDMETHOD_(bool,ShowCursor)(bool yesno)
	//BOOL _stdcall ShowCursor(bool yesno)
	STDMETHOD_(BOOL, ShowCursor)(BOOL bShow)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->ShowCursor(bShow);
	}
	STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS *presentParam,IDirect3DSwapChain8 **pSwapChain)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateAdditionalSwapChain(presentParam,pSwapChain);
	}
	STDMETHOD(Reset)(D3DPRESENT_PARAMETERS *presentParam)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		//g_CCore->GetGraphics()->m_chatfont;
		g_CCore->GetGraphics()->OnLostDevice();
		HRESULT hr = p_Dev->Reset(presentParam);
		if (SUCCEEDED(hr))
		{
			g_CCore->GetGraphics()->OnResetDevice();
		}
		else if (FAILED(hr))
		{
			char errorstr[255];
			D3DXGetErrorStringA(hr, errorstr, 254);
			char str[255];
			sprintf_s(str, "Error: %x error description: %s \n", hr, errorstr);
			MessageBox(NULL,str,str,MB_OK);
		}
		return hr;
	}
	STDMETHOD(Present)(CONST RECT *pSourceRect,CONST RECT *pDestRect,HWND DestWindowOverride,const RGNDATA *pDirtyRegion)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->Present(pSourceRect,pDestRect,DestWindowOverride,pDirtyRegion);
	}
	STDMETHOD(GetBackBuffer)(UINT backbuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8 **pBackBuffer)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetBackBuffer(backbuffer,Type,pBackBuffer);
	}
	STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS *pRasterStat)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetRasterStatus(pRasterStat);
	}
	STDMETHOD_(void,SetGammaRamp)(DWORD flags, const D3DGAMMARAMP *pRamp)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetGammaRamp(flags,pRamp);
	}
	STDMETHOD_(void,GetGammaRamp)(D3DGAMMARAMP *pRamp)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetGammaRamp(pRamp);
	}
	STDMETHOD(CreateTexture)(UINT Width, UINT Heigh,UINT Level,DWORD usage,D3DFORMAT Format,D3DPOOL pool, IDirect3DTexture8 **ppTexture)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateTexture(Width, Heigh, Level, usage, Format, pool, ppTexture);
	}
	STDMETHOD(CreateVolumeTexture)(UINT Width, UINT Heigh,UINT Deph,UINT Level,DWORD usage,D3DFORMAT Format,D3DPOOL pool, IDirect3DVolumeTexture8 **ppTexture)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateVolumeTexture(Width, Heigh, Deph,Level, usage, Format, pool, ppTexture);
	}
	STDMETHOD(CreateCubeTexture)(UINT Edgelength,UINT Level,DWORD usage,D3DFORMAT Format,D3DPOOL pool, IDirect3DCubeTexture8 **ppTexture)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateCubeTexture(Edgelength,Level,usage,Format,pool,ppTexture);
	}
	STDMETHOD(CreateVertexBuffer)(UINT Length,DWORD usage,DWORD FVF,D3DPOOL pool, IDirect3DVertexBuffer8 **ppTexture)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateVertexBuffer(Length,usage,FVF,pool,ppTexture);
	}
	STDMETHOD(CreateIndexBuffer)(UINT Length,DWORD usage,D3DFORMAT Format,D3DPOOL pool, IDirect3DIndexBuffer8 **ppBuff)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateIndexBuffer(Length,usage,Format,pool, ppBuff);
	}
	STDMETHOD(CreateRenderTarget)(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,BOOL Lockable,IDirect3DSurface8 **ppSurface)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateRenderTarget(Width,Height,Format,MultiSample,Lockable,ppSurface);
	}
	STDMETHOD(CreateDepthStencilSurface)(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,IDirect3DSurface8 **ppSurface)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateDepthStencilSurface(Width,Height,Format,MultiSample,ppSurface);
	}
	STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface8 **ppZStencilSurface)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetDepthStencilSurface(ppZStencilSurface);
	}
	STDMETHOD(CreateImageSurface)(UINT Width,UINT Height,D3DFORMAT Format,IDirect3DSurface8 **ppSurface)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateImageSurface(Width,Height,Format,ppSurface);
	}
	STDMETHOD(CopyRects)(IDirect3DSurface8 *ppSourceSurface,const RECT *pSource,UINT cRects,IDirect3DSurface8 *ppDestSurface,const POINT *pDestPointsArray)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CopyRects(ppSourceSurface,pSource,cRects,ppDestSurface,pDestPointsArray);
	}
	STDMETHOD(UpdateTexture)(IDirect3DBaseTexture8 *ppSourceTexture,IDirect3DBaseTexture8 *ppDestTexture)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->UpdateTexture(ppSourceTexture,ppDestTexture);
	}
	STDMETHOD(GetFrontBuffer)(IDirect3DSurface8 *pSurface)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetFrontBuffer(pSurface);
	}
	STDMETHOD(SetRenderTarget)(IDirect3DSurface8 *pRendTarg,IDirect3DSurface8 *ZStencil)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetRenderTarget(pRendTarg,ZStencil);
	}
	STDMETHOD(GetRenderTarget)(IDirect3DSurface8 **pRendTarg)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetRenderTarget(pRendTarg);
	}
	STDMETHOD(BeginScene)()
	{
		//char buf[255];
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		//sprintf(buf,"%x",this);
		//g_CCore->GetChat()->AddMessage(buf);
		return p_Dev->BeginScene();
	}
	STDMETHOD(EndScene)()
	{
		//if(g_CCore->IsRunning())
			g_CCore->GetGraphics()->Render();
		return p_Dev->EndScene();

	}
	STDMETHOD(Clear)(DWORD count,const D3DRECT *pRects,DWORD Flags, D3DCOLOR Color,float Z, DWORD Stencil)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->Clear(count,pRects,Flags,Color,Z,Stencil);
	}
	STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State,const D3DMATRIX *pMatrix)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		//if(State == D3DTS_VIEW)
		//	g_CCore->GetChat()->AddMessage("SetTransform View");
		//else if(State == D3DTS_PROJECTION)
		//	g_CCore->GetChat()->AddMessage("SetTransform Projection");
		//else if(State == D3DTS_WORLD)
		//	g_CCore->GetChat()->AddMessage("SetTransform World");
		if(State == D3DTS_VIEW && g_CCore->GetGraphics()->IsCamFreezed == true)
		{
			D3DMATRIX m;
			p_Dev->GetTransform(State,&m);
			return p_Dev->SetTransform(State,&m);
		}
		return p_Dev->SetTransform(State,pMatrix);
	}
	STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State,D3DMATRIX *pMatrix)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetTransform(State,pMatrix);
	}
	STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE State,const D3DMATRIX *pMatrix)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->MultiplyTransform(State,pMatrix);
	}
	STDMETHOD(SetViewport)(const D3DVIEWPORT8 *pViewport)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		//g_CCore->GetChat()->AddMessage("SetViewport");
		return p_Dev->SetViewport(pViewport);
	}
	STDMETHOD(GetViewport)(D3DVIEWPORT8 *pViewport)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetViewport(pViewport);
	}
	STDMETHOD(SetMaterial)(const D3DMATERIAL8 *pMaterial)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetMaterial(pMaterial);
	}
	STDMETHOD(GetMaterial)(D3DMATERIAL8 *pMaterial)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetMaterial(pMaterial);
	}
	STDMETHOD(SetLight)(DWORD index,const D3DLIGHT8 *pLight)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetLight(index,pLight);
	}
	STDMETHOD(GetLight)(DWORD index,D3DLIGHT8 *pLight)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetLight(index,pLight);
	}
	/*STDMETHOD(GetLight)(DWORD index,D3DLIGHT8 *pLight)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetLight(index,pLight);
	}*/
	STDMETHOD(LightEnable)(DWORD index,BOOL enable)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->LightEnable(index,enable);
	}
	STDMETHOD(GetLightEnable)(DWORD index,BOOL *pEnable)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetLightEnable(index,pEnable);
	}
	STDMETHOD(SetClipPlane)(DWORD index,const float *pPlane)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetClipPlane(index,pPlane);
	}
	STDMETHOD(GetClipPlane)(DWORD index,float *pPlane)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetClipPlane(index,pPlane);
	}
	STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State,DWORD Value)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		char buffer[255];
		sprintf(buffer,"SetRenderState %x %x",State,Value);
		g_CCore->GetLog()->AddLog(buffer);
		return p_Dev->SetRenderState(State,Value);
	}
	STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State,DWORD *pValue)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetRenderState(State,pValue);
	}
	STDMETHOD(BeginStateBlock)()
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->BeginStateBlock();
	}
	STDMETHOD(EndStateBlock)(DWORD *pToken)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->EndStateBlock(pToken);
	}
	STDMETHOD(ApplyStateBlock)(DWORD Token)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->ApplyStateBlock(Token);
	}
	STDMETHOD(CaptureStateBlock)(DWORD Token)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CaptureStateBlock(Token);
	}
	STDMETHOD(DeleteStateBlock)(DWORD Token)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DeleteStateBlock(Token);
	}
	STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type,DWORD *pToken)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateStateBlock(Type,pToken);
	}
	STDMETHOD(SetClipStatus)(const D3DCLIPSTATUS8 *pStatus)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetClipStatus(pStatus);
	}
	STDMETHOD(GetClipStatus)(D3DCLIPSTATUS8 *pStatus)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetClipStatus(pStatus);
	}
	STDMETHOD(GetTexture)(DWORD State,IDirect3DBaseTexture8 **pTexture)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetTexture(State,pTexture);
	}
	STDMETHOD(SetTexture)(DWORD State,IDirect3DBaseTexture8 *pTexture)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetTexture(State,pTexture);
	}
	STDMETHOD(GetTextureStageState)(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD *pValue)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetTextureStageState(Stage,Type,pValue);
	}
	STDMETHOD(SetTextureStageState)(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetTextureStageState(Stage,Type,Value);
	}
	STDMETHOD(ValidateDevice)(DWORD *pNumPasses)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->ValidateDevice(pNumPasses);
	}
	STDMETHOD(GetInfo)(DWORD DevInfoId, void *pDevInfoStruct,DWORD DevInfoStructSize)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetInfo(DevInfoId, pDevInfoStruct,DevInfoStructSize);
	}
	STDMETHOD(SetPaletteEntries)(UINT PalletNumber,const PALETTEENTRY *Entries)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetPaletteEntries(PalletNumber,Entries);
	}
	STDMETHOD(GetPaletteEntries)(UINT PalletNumber,PALETTEENTRY *Entries)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetPaletteEntries(PalletNumber,Entries);
	}
	STDMETHOD(SetCurrentTexturePalette)(UINT PalletNumber)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetCurrentTexturePalette(PalletNumber);
	}
	STDMETHOD(GetCurrentTexturePalette)(UINT *PalletNumber)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetCurrentTexturePalette(PalletNumber);
	}
	STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE DP,UINT StartVertex, UINT PrimitiveCount)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DrawPrimitive(DP,StartVertex,PrimitiveCount);
	}
	STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE DP,UINT MinIndex, UINT Vertices, UINT startIndex, UINT PrimitiveCount)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DrawIndexedPrimitive(DP,MinIndex,Vertices, startIndex,PrimitiveCount);
	}
	STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE DP,UINT PrimitiveCount,const void* pVertexStreamZeroData,UINT VertexStreamZeroSize)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DrawPrimitiveUP(DP,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroSize);
	}
	STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE DP,UINT MinVertexIndex, UINT NumVertecIndices, UINT PrimitiveCount,const void* pIndexData,D3DFORMAT IndexDataFormat, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DrawIndexedPrimitiveUP(DP,MinVertexIndex, NumVertecIndices, PrimitiveCount,pIndexData,IndexDataFormat,pVertexStreamZeroData, VertexStreamZeroStride);
	}
	STDMETHOD(ProcessVertices)(UINT UI1,UINT UI2,UINT UI3,IDirect3DVertexBuffer8 *pDestBuffer,DWORD Flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->ProcessVertices(UI1,UI2,UI3,pDestBuffer,Flags);
	}
	STDMETHOD(CreateVertexShader)(const DWORD *pDW1,const DWORD *pDW2,DWORD *pHandle,DWORD usage)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreateVertexShader(pDW1,pDW2,pHandle,usage);
	}
	STDMETHOD(SetVertexShader)(DWORD Handle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetVertexShader(Handle);
	}
	STDMETHOD(GetVertexShader)(DWORD *pHandle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetVertexShader(pHandle);
	}
	STDMETHOD(DeleteVertexShader)(DWORD Handle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DeleteVertexShader(Handle);
	}
	STDMETHOD(SetVertexShaderConstant)(DWORD Register, const void * pConstData,DWORD ConstantCount)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetVertexShaderConstant(Register, pConstData,ConstantCount);
	}
	STDMETHOD(GetVertexShaderConstant)(DWORD Register, void * pConstData,DWORD ConstantCount)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetVertexShaderConstant(Register, pConstData,ConstantCount);
	}
	STDMETHOD(GetVertexShaderDeclaration)(DWORD Handle, void * pData,DWORD *pSizeOfData)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetVertexShaderDeclaration(Handle, pData,pSizeOfData);
	}
	STDMETHOD(GetVertexShaderFunction)(DWORD Handle, void * pData,DWORD *pSizeOfData)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetVertexShaderFunction(Handle,pData,pSizeOfData);
	}
	STDMETHOD(SetStreamSource)(UINT StreamNumber,IDirect3DVertexBuffer8 *pStreamData,UINT Stride)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetStreamSource(StreamNumber,pStreamData,Stride);
	}
	STDMETHOD(GetStreamSource)(UINT StreamNumber,IDirect3DVertexBuffer8 **pStreamData,UINT *pStride)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetStreamSource(StreamNumber,pStreamData,pStride);
	}
	STDMETHOD(GetIndices)(IDirect3DIndexBuffer8 **pIndexData,UINT *pBaseVertexIndex)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetIndices(pIndexData,pBaseVertexIndex);
	}
	STDMETHOD(SetIndices)(IDirect3DIndexBuffer8 *pIndexData,UINT pBaseVertexIndex)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetIndices(pIndexData,pBaseVertexIndex);
	}
	STDMETHOD(CreateIndexBuffer)(IDirect3DIndexBuffer8 *pIndexData,UINT pBaseVertexIndex)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetIndices(pIndexData,pBaseVertexIndex);
	}
	STDMETHOD(CreatePixelShader)(const DWORD *pFunc,DWORD *pHandle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->CreatePixelShader(pFunc,pHandle);
	}
	STDMETHOD(SetPixelShader)(DWORD Handle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetPixelShader(Handle);
	}
	STDMETHOD(GetPixelShader)(DWORD *pHandle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetPixelShader(pHandle);
	}
	STDMETHOD(DeletePixelShader)(DWORD Handle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DeletePixelShader(Handle);
	}
	STDMETHOD(SetPixelShaderConstant)(DWORD Register,const void *pConstData,DWORD ConstantCount)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->SetPixelShaderConstant(Register,pConstData,ConstantCount);
	}
	STDMETHOD(GetPixelShaderConstant)(DWORD Register,void *pConstData,DWORD ConstantCount)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetPixelShaderConstant(Register,pConstData,ConstantCount);
	}
	STDMETHOD(GetPixelShaderFunction)(DWORD Handle,void *pData,DWORD *pSizeOfData)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->GetPixelShaderFunction(Handle,pData,pSizeOfData);
	}
	STDMETHOD(DrawRectPatch)(UINT Handle,const float *pNumSegs, const D3DRECTPATCH_INFO *pRI)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DrawRectPatch(Handle,pNumSegs,pRI);
	}
	STDMETHOD(DrawTriPatch)(UINT Handle,const float *pNumSegs, const D3DTRIPATCH_INFO *pRI)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DrawTriPatch(Handle,pNumSegs, pRI);
	}
	STDMETHOD(DeletePatch)(UINT Handle)
	{
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_Dev->DeletePatch(Handle);
	}
};