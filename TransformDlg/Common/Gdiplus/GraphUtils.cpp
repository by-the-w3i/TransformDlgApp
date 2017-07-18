#include "stdafx.h"
#include "GraphUtils.h"


RECT Rect2RECT(const Rect& rcIn)
{
	RECT rcReturn = {0};
	rcReturn.left = rcIn.X;
	rcReturn.top = rcIn.Y;
	rcReturn.right = rcIn.X+rcIn.Width;
	rcReturn.bottom = rcIn.Y+rcIn.Height;
	return rcReturn;
}

Rect RECT2Rect(const RECT& rcIn)
{
	Rect rcReturn;
	rcReturn.X = rcIn.left;
	rcReturn.Y = rcIn.top;
	rcReturn.Width = rcIn.right - rcIn.left;
	rcReturn.Height = rcIn.bottom - rcIn.top;
	return rcReturn;
}

void DrawMargin(Graphics& g, Image* pImage, Rect& rcDst, int nMarginLeft, int nMarginRight, int nMarginTop, int nMarginBottom, BOOL bDrawCenter/* = TRUE*/, BOOL bTile/* = TRUE*/,  float fAlpha/* = 1.0f*/)
{
	ASSERT(pImage);
	if (pImage)
	{
		int nImgWidth = pImage->GetWidth();
		int nImgHeight = pImage->GetHeight();
		Rect rcSrc(0, 0, nImgWidth, nImgHeight);

		DrawMargin(g, pImage, rcDst, rcSrc, nMarginLeft, nMarginRight, nMarginTop, nMarginBottom, bDrawCenter, bTile, fAlpha);
	}
}

void DrawMargin(Graphics& g, Image* pImage, Rect& rcDst, Rect& rcSrc, int nMarginLeft, int nMarginRight, int nMarginTop, int nMarginBottom, BOOL bDrawCenter/* = TRUE*/, BOOL bTile/* = TRUE*/,  float fAlpha/* = 1.0f*/)
{
	ASSERT(pImage);
	if (pImage)
	{
		int nImgWidth = pImage->GetWidth();
		int nImgHeight = pImage->GetHeight();
		Rect rcImage(0, 0, nImgWidth, nImgHeight);
		ASSERT(rcImage.Contains(rcSrc));

		//ASSERT((rcSrc.Width >= (nMarginLeft + nMarginRight)) && (rcSrc.Height >= (nMarginTop + nMarginBottom)));
		if((rcDst.Width < (nMarginLeft + nMarginRight)) || (rcDst.Height < (nMarginTop + nMarginBottom)))
			g.DrawImage(pImage, rcDst, rcSrc.X, rcSrc.Y, rcSrc.Width, rcSrc.Height, UnitPixel);

		ImageAttributes imgAtt;
		ColorMatrix colorMatrix = 
		{{{1.0f, 0.0, 0.0, 0.0, 0.0},
		{0.0, 1.0f, 0.0, 0.0, 0.0},
		{0.0, 0.0, 1.0f, 0.0, 0.0},
		{0.0, 0.0, 0.0, fAlpha, 0.0},
		{0.0, 0.0, 0.0, 0.0, 1.0f}}};
		imgAtt.SetColorMatrix(&colorMatrix);

		ImageAttributes imgAttr;
		imgAttr.SetColorMatrix(&colorMatrix);
		//上
		if(nMarginTop)
		{
			if(nMarginLeft)
			{
				Rect rcTopLeft(rcDst.X, rcDst.Y, nMarginLeft, nMarginTop);
				g.DrawImage(pImage, rcTopLeft, rcSrc.X, rcSrc.Y, nMarginLeft, nMarginTop, UnitPixel, &imgAttr);
			}
			Rect rcTop(rcDst.X + nMarginLeft, rcDst.Y, rcDst.Width - nMarginLeft - nMarginRight, nMarginTop);
			if(bTile)
				imgAtt.SetWrapMode(WrapModeTileFlipX);
			g.DrawImage(pImage, rcTop, rcSrc.X + nMarginLeft, rcSrc.Y, rcSrc.Width - nMarginLeft - nMarginRight, nMarginTop, UnitPixel, &imgAtt);
			if(nMarginRight)
			{
				Rect rcTopRight(rcDst.GetRight() - nMarginRight, rcDst.Y, nMarginRight, nMarginTop);
				g.DrawImage(pImage, rcTopRight, rcSrc.GetRight() - nMarginRight, rcSrc.Y, nMarginRight, nMarginTop, UnitPixel, &imgAttr);
			}
		}

		//中
		if(nMarginLeft)
		{
			Rect rcLeft(rcDst.X, rcDst.Y + nMarginTop, nMarginLeft, rcDst.Height - nMarginTop - nMarginBottom);
			if(bTile)
				imgAtt.SetWrapMode(WrapModeTileFlipY);
			g.DrawImage(pImage, rcLeft, rcSrc.X, rcSrc.Y + nMarginTop, nMarginLeft, rcSrc.Height - nMarginTop - nMarginBottom, UnitPixel, &imgAtt);
		}
		if(bDrawCenter)
		{
			Rect rcCenter(rcDst.X + nMarginLeft, rcDst.Y + nMarginTop, rcDst.Width - nMarginLeft - nMarginRight, rcDst.Height - nMarginTop - nMarginBottom);
			if(bTile)
				imgAtt.SetWrapMode(WrapModeTileFlipXY);
			g.DrawImage(pImage, rcCenter, rcSrc.X + nMarginLeft, rcSrc.Y + nMarginTop, rcSrc.Width - nMarginLeft - nMarginRight, rcSrc.Height - nMarginTop - nMarginBottom, UnitPixel, &imgAtt);
		}
		if(nMarginRight)
		{
			Rect rcRight(rcDst.GetRight() - nMarginRight, rcDst.Y + nMarginTop, nMarginRight, rcDst.Height - nMarginTop - nMarginBottom);
			if(bTile)
				imgAtt.SetWrapMode(WrapModeTileFlipY);
			g.DrawImage(pImage, rcRight, rcSrc.GetRight() - nMarginRight, rcSrc.Y + nMarginTop, nMarginRight, rcSrc.Height - nMarginTop - nMarginBottom, UnitPixel, &imgAtt);
		}

		//下
		if(nMarginTop)
		{
			if(nMarginLeft)
			{
				Rect rcBottomLeft(rcDst.X, rcDst.GetBottom() - nMarginBottom, nMarginLeft, nMarginBottom);
				g.DrawImage(pImage, rcBottomLeft, rcSrc.X, rcSrc.GetBottom() - nMarginBottom, nMarginLeft, nMarginBottom, UnitPixel, &imgAttr);
			}
			Rect rcBottom(rcDst.X + nMarginLeft, rcDst.GetBottom() - nMarginBottom, rcDst.Width - nMarginLeft - nMarginRight, nMarginBottom);
			if(bTile)
				imgAtt.SetWrapMode(WrapModeTileFlipX);
			g.DrawImage(pImage, rcBottom, rcSrc.X + nMarginLeft, rcSrc.GetBottom() - nMarginBottom, rcSrc.Width - nMarginLeft - nMarginRight, nMarginBottom, UnitPixel, &imgAtt);
			if(nMarginRight)
			{
				Rect rcBottomRight(rcDst.GetRight() - nMarginRight, rcDst.GetBottom() - nMarginBottom, nMarginRight, nMarginBottom);
				g.DrawImage(pImage, rcBottomRight, rcSrc.GetRight() - nMarginRight, rcSrc.GetBottom() - nMarginBottom, nMarginRight, nMarginBottom, UnitPixel, &imgAttr);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// rcClient: 绘图区域
// nLineCount: 分成几行
// nCountPerLine: 一行共几个
// nItemDraw: 绘制第几个item(0开始)
//////////////////////////////////////////////////////////////////////////
void DrawThirdPaintBg( Graphics& g, Image* pImage, Rect& rcClient, int nLineCount, int nCountPerLine, int nItemDraw )
{
	int x = nItemDraw % nCountPerLine;
	int y = nItemDraw / nCountPerLine;
	int nWidth = pImage->GetWidth() / nCountPerLine;
	int nHeight = pImage->GetHeight() / nLineCount;
	int nSWidth = nWidth / 3;

	if (rcClient.Width < 2 * nSWidth)
	{
		int nW2 = rcClient.Width / 2;
		Rect rcImageLeft(x*nWidth, y*nHeight, nW2, nHeight);
		Rect rcLeft(rcClient.X, rcClient.Y, nW2, rcClient.Height);

		Rect rcImageRight(x*nWidth + nWidth - (rcClient.Width - nW2), y*nHeight, (rcClient.Width - nW2), nHeight);
		Rect rcRight(rcClient.GetRight() - (rcClient.Width - nW2), rcClient.Y, (rcClient.Width - nW2), rcClient.Height);
		
		TextureBrush brushLeft(pImage,WrapModeTileFlipX, rcImageLeft);
		brushLeft.TranslateTransform((REAL)rcLeft.X, (REAL)rcLeft.Y);
		g.FillRegion(&brushLeft, &Region(rcLeft));
		TextureBrush brushRight(pImage, WrapModeTileFlipX, rcImageRight);
		brushRight.TranslateTransform((REAL)rcRight.X, (REAL)rcRight.Y);
		g.FillRegion(&brushRight, &Region(rcRight));
	}
	else
	{
		Rect rcImageLeft(x*nWidth, y*nHeight, nSWidth, nHeight);
		Rect rcLeft(rcClient.X, rcClient.Y, nSWidth, rcClient.Height);

		Rect rcImageRight(x*nWidth + nWidth - nSWidth, y*nHeight, nSWidth, nHeight);
		Rect rcRight(rcClient.GetRight() - nSWidth, rcClient.Y, nSWidth, rcClient.Height);

		Rect rcImageMiddle(x*nWidth + nSWidth, y*nHeight, nSWidth, nHeight);
		Rect rcMiddle(rcClient.X + nSWidth, rcClient.Y,  rcClient.Width - 2 * nSWidth, rcClient.Height);

		TextureBrush brushLeft(pImage,WrapModeTileFlipX, rcImageLeft);
		brushLeft.TranslateTransform((REAL)rcLeft.X, (REAL)rcLeft.Y);
		g.FillRegion(&brushLeft, &Region(rcLeft));
		TextureBrush brushRight(pImage, WrapModeTileFlipX, rcImageRight);
		brushRight.TranslateTransform((REAL)rcRight.X, (REAL)rcRight.Y);
		g.FillRegion(&brushRight, &Region(rcRight));
		TextureBrush brushCenter(pImage, WrapModeTileFlipX, rcImageMiddle);
		brushCenter.TranslateTransform((REAL)rcMiddle.X, (REAL)rcMiddle.Y);
		g.FillRegion(&brushCenter, &Region(rcMiddle));
	}
}

void DrawThirdPaintBgY( Graphics& g, Image* pImage, Rect& rcClient, int nLineCount, int nCountPerLine, int nItemDraw )
{
	int x = nItemDraw % nCountPerLine;
	int y = nItemDraw / nCountPerLine;
	int nWidth = pImage->GetWidth() / nCountPerLine;
	int nHeight = pImage->GetHeight() / nLineCount;
	//int nSWidth = nHeight / 3;
	int nSHeight = nHeight / 3;

	Rect rcImageTop(x*nWidth, y*nHeight, nWidth, nSHeight);
	Rect rcTop(rcClient.X, rcClient.Y, rcClient.Width, nSHeight);

	Rect rcImageBottom(x*nWidth, y*nHeight + nHeight - nSHeight, nWidth, nSHeight);
	Rect rcBottom(rcClient.X, rcClient.GetBottom() - nSHeight, rcClient.Width, nSHeight);

	Rect rcImageMid(x*nWidth, y*nHeight + nSHeight, nWidth, nSHeight);
	Rect rcMid(rcClient.X, rcClient.Y + nSHeight, rcClient.Width, rcClient.Height - 2*nSHeight);

	TextureBrush brushTop(pImage,WrapModeTileFlipY, rcImageTop);
	brushTop.TranslateTransform((REAL)rcTop.X, (REAL)rcTop.Y);
	g.FillRegion(&brushTop, &Region(rcTop));
	TextureBrush brushBottom(pImage, WrapModeTileFlipY, rcImageBottom);
	brushBottom.TranslateTransform((REAL)rcBottom.X, (REAL)rcBottom.Y);
	g.FillRegion(&brushBottom, &Region(rcBottom));
	TextureBrush brushMid(pImage, WrapModeTileFlipY, rcImageMid);
	brushMid.TranslateTransform((REAL)rcMid.X, (REAL)rcMid.Y);
	g.FillRegion(&brushMid, &Region(rcMid));
}

void DrawRoundedRect(Graphics& g,Brush *pBrush,Pen *pen,const Rect &rect,int nRadius)
{
	ASSERT (nRadius >= 1 && nRadius <= 100);

	GraphicsPath pt;
	GraphicsPath * path = pt.Clone();

	int nDimension=nRadius*2;
	path->AddArc(rect.X, rect.Y, nDimension, nDimension, 180, 90);

	path->AddArc(rect.GetRight()-nDimension, rect.Y, nDimension, nDimension, 270, 90);
	path->AddArc(rect.GetRight()-nDimension, rect.GetBottom()-nDimension, 
		nDimension, nDimension, 0, 90);
	path->AddArc(rect.X, rect.GetBottom()-nDimension, nDimension, nDimension, 90, 90);
	path->CloseFigure();
	
	if(pBrush!=NULL)
	   g.FillPath(pBrush,path);
	if(pen)
	   g.DrawPath(pen,path);
}


//************************************
// Method:    GetEncoderClsid
// FullName:  GetEncoderClsid
// Access:    public 
// Returns:    -1:fail;  other:success
// Qualifier:
// Parameter: const WCHAR * format
// Parameter: CLSID * pClsid
//************************************
int GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	int nRet = -1;
	ImageCodecInfo* pCodecInfo = NULL;
	UINT nNum = 0, nSize = 0;
	GetImageEncodersSize(&nNum, &nSize);
	if(nSize < 0)
	{
		return nRet;
	}
	pCodecInfo = new ImageCodecInfo[nSize];
	GetImageEncoders(nNum, nSize, pCodecInfo);
	for(UINT i = 0; i < nNum; ++i)
	{
		if(wcscmp(pCodecInfo[i].MimeType, format) == 0)
		{
			*pClsid = pCodecInfo[i].Clsid;
			nRet = i;
			break;
		}
	}
	SafeDeleteArray(pCodecInfo);

	return nRet;
}


Image* LoadImageFromFile(LPCTSTR szFileName)
{
	//TODO:
	//IStream* pStream = CreateStreamFromFileName(szFileName);
	//Image* pImage=LoadImageFromStream(pStream);
	//return pImage;
	return NULL;
}

Image* LoadImageFromStream(IStream* pStream)
{
	Image* pImage = NULL;
	if(pStream)
	{
		pImage = Image::FromStream(pStream);
		if (pImage 
			&& (pImage->GetLastStatus() != Ok))
			SafeDelete(pImage);
		pStream->Release();
	}
	return pImage;
}

Bitmap* LoadBitmapFromFile(LPCTSTR szFileName)
{
	//TODO:

	//Bitmap* pBitmap = NULL;
	//IStream* pStream = CreateStreamFromFileName(szFileName);
	//if(pStream)
	//{
	//	pBitmap = Bitmap::FromStream(pStream);
	//	if (pBitmap 
	//		&& pBitmap->GetLastStatus() != Ok)
	//		SafeDelete(pBitmap);
	//	pStream->Release();
	//}
	//return pBitmap;
	return NULL;
}

BOOL SaveImage(const CLSID& clsid, Image* pImage, const CString& strDst)
{
	BOOL bSave = FALSE;

	EncoderParameters params;
	params.Count = 1;
	EncoderParameter param;
	param.Guid = EncoderQuality;
	param.NumberOfValues = 1;
	param.Type = EncoderParameterValueTypeLong;
	int nQuality = 100;
	param.Value = &nQuality;
	params.Parameter[0] = param;

	Status status = pImage->Save(strDst, &clsid, &params);
	if(status == Ok)
		bSave = TRUE;

	return bSave;
}

BOOL SaveBestFitImage(const WCHAR *format, const CString& strDst, const CString& strSrc, int& nWidth, int& nHeight)
{
	BOOL bConvert = FALSE;
	CLSID clsid;
	int nRet = GetEncoderClsid(format, &clsid); //Get CLSID
	if(nRet < 0)
		return bConvert;

	Image* pImage = new Image(strSrc);
	if(pImage->GetLastStatus() != Ok)
	{
		SafeDelete(pImage);
		return bConvert;
	}

	//计算位置
	int nSrcWidth = pImage->GetWidth();
	int nSrcHeight = pImage->GetHeight();

	if(nSrcWidth <= nWidth && nSrcHeight <= nHeight)//图片小于目标图
	{
		bConvert = SaveImage(clsid, pImage, strDst);
		nWidth = nSrcWidth;
		nHeight = nSrcHeight;
	}
	else if(nSrcWidth <= nWidth || nSrcHeight <= nHeight) //有一条边小
	{
		if(nSrcWidth <= nWidth)
		{
			Bitmap* pbitmap = new Bitmap(nSrcWidth, nHeight, PixelFormat24bppRGB);
			Graphics* pbmpGraphics = Graphics::FromImage (pbitmap);
			pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
			pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
			Color color(255, 255, 255, 255);
			pbmpGraphics->Clear(color);
			pbmpGraphics->DrawImage(pImage, 0, 0, 0, (nSrcHeight - nHeight) / 2, nSrcWidth, nHeight, UnitPixel);
			bConvert = SaveImage(clsid, pbitmap, strDst);
			SafeDelete(pbmpGraphics);
			SafeDelete(pbitmap);
			nWidth = nSrcWidth;
		}
		else
		{
			Bitmap* pbitmap = new Bitmap(nWidth, nSrcHeight, PixelFormat24bppRGB);
			Graphics* pbmpGraphics = Graphics::FromImage(pbitmap);
			pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
			pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
			Color color(255, 255, 255, 255);
			pbmpGraphics->Clear(color);
			pbmpGraphics->DrawImage(pImage, 0, 0, (nSrcWidth - nWidth) / 2, 0, nWidth, nSrcHeight, UnitPixel);
			bConvert = SaveImage(clsid, pbitmap, strDst);
			SafeDelete(pbmpGraphics);
			SafeDelete(pbitmap);
			nHeight = nSrcHeight;
		}
	}
	else
	{
		Bitmap* pbitmap = new Bitmap(nWidth, nHeight, PixelFormat24bppRGB);
		Graphics* pbmpGraphics = Graphics::FromImage(pbitmap);
		pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
		pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		Color color(255, 255, 255, 255);
		pbmpGraphics->Clear(color);
		Rect rect(0, 0, nWidth, nHeight);
		if(nSrcWidth * nHeight > nWidth * nSrcHeight)  //太宽了, 以高来成比例缩放
			pbmpGraphics->DrawImage(pImage, rect, (nSrcWidth - nWidth * nSrcHeight / nHeight) / 2, 0, nWidth * nSrcHeight / nHeight, nSrcHeight, UnitPixel);
		else
			pbmpGraphics->DrawImage(pImage, rect, 0, (nSrcHeight - nHeight * nSrcWidth / nWidth) / 2, nSrcWidth, nHeight * nSrcWidth / nWidth, UnitPixel);
		bConvert = SaveImage(clsid, pbitmap, strDst);
		SafeDelete(pbmpGraphics);
		SafeDelete(pbitmap);
	}

	SafeDelete(pImage);

	return bConvert;
}

BOOL SaveBestFitImage(const WCHAR *format, Image* pImage, const CString& strDst, int& nWidth, int& nHeight)
{
	BOOL bConvert = FALSE;
	CLSID clsid;
	int nRet = GetEncoderClsid(format, &clsid); //Get CLSID
	if(nRet < 0)
		return bConvert;

	//计算位置
	int nSrcWidth = pImage->GetWidth();
	int nSrcHeight = pImage->GetHeight();

	if(nSrcWidth <= nWidth && nSrcHeight <= nHeight)//图片小于目标图
	{
		bConvert = SaveImage(clsid, pImage, strDst);
		nWidth = nSrcWidth;
		nHeight = nSrcHeight;
	}
	else if(nSrcWidth <= nWidth || nSrcHeight <= nHeight) //有一条边小
	{
		if(nSrcWidth <= nWidth)
		{
			Bitmap* pbitmap = new Bitmap(nSrcWidth, nHeight, PixelFormat24bppRGB);
			Graphics* pbmpGraphics = Graphics::FromImage (pbitmap);
			pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
			pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
			Color color(255, 255, 255, 255);
			pbmpGraphics->Clear(color);
			pbmpGraphics->DrawImage(pImage, 0, 0, 0, (nSrcHeight - nHeight) / 2, nSrcWidth, nHeight, UnitPixel);
			bConvert = SaveImage(clsid, pbitmap, strDst);
			SafeDelete(pbmpGraphics);
			SafeDelete(pbitmap);
			nWidth = nSrcWidth;
		}
		else
		{
			Bitmap* pbitmap = new Bitmap(nWidth, nSrcHeight, PixelFormat24bppRGB);
			Graphics* pbmpGraphics = Graphics::FromImage(pbitmap);
			pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
			pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
			Color color(255, 255, 255, 255);
			pbmpGraphics->Clear(color);
			pbmpGraphics->DrawImage(pImage, 0, 0, (nSrcWidth - nWidth) / 2, 0, nWidth, nSrcHeight, UnitPixel);
			bConvert = SaveImage(clsid, pbitmap, strDst);
			SafeDelete(pbmpGraphics);
			SafeDelete(pbitmap);
			nHeight = nSrcHeight;
		}
	}
	else
	{
		Bitmap* pbitmap = new Bitmap(nWidth, nHeight, PixelFormat24bppRGB);
		Graphics* pbmpGraphics = Graphics::FromImage(pbitmap);
		pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
		pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		Color color(255, 255, 255, 255);
		pbmpGraphics->Clear(color);
		Rect rect(0, 0, nWidth, nHeight);
		if(nSrcWidth * nHeight > nWidth * nSrcHeight)  //太宽了, 以高来成比例缩放
			pbmpGraphics->DrawImage(pImage, rect, (nSrcWidth - nWidth * nSrcHeight / nHeight) / 2, 0, nWidth * nSrcHeight / nHeight, nSrcHeight, UnitPixel);
		else
			pbmpGraphics->DrawImage(pImage, rect, 0, (nSrcHeight - nHeight * nSrcWidth / nWidth) / 2, nSrcWidth, nHeight * nSrcWidth / nWidth, UnitPixel);
		bConvert = SaveImage(clsid, pbitmap, strDst);
		SafeDelete(pbmpGraphics);
		SafeDelete(pbitmap);
	}

	return bConvert;
}

BOOL SaveImage(LPCWSTR szSrcFile, LPCWSTR szDstFile, LPCWSTR szCodecMimeType)
{
	BOOL bConvert = FALSE;
	CLSID clsid;
	int nRet = GetEncoderClsid(szCodecMimeType, &clsid); //Get CLSID
	if (nRet >= 0)
	{
		Image* pImage = new Image(szSrcFile);
		if (pImage->GetLastStatus() != Ok)
		{
			delete pImage;
			return bConvert;
		}

		EncoderParameters params;
		params.Count = 1;
		EncoderParameter param;
		param.Guid = EncoderQuality;
		param.NumberOfValues = 1;
		param.Type = EncoderParameterValueTypeLong;
		int nQuality = 100;
		param.Value = &nQuality;
		params.Parameter[0] = param;

		Status stat = pImage->Save(szDstFile, &clsid, &params);
		if (stat == Ok)
		{
			bConvert = TRUE;
		}

		SafeDelete(pImage);
	}

	return bConvert;
}

BOOL SaveImage(IStream* pStream, LPCWSTR szDstFile, LPCWSTR szCodecMimeType)
{
	BOOL bConvert = FALSE;
	CLSID clsid;
	int nRet = GetEncoderClsid(szCodecMimeType, &clsid); //Get CLSID
	if (nRet >= 0)
	{
		Image* pImage = new Image(pStream);
		if (pImage->GetLastStatus() != Ok)
		{
			delete pImage;
			return bConvert;
		}

		EncoderParameters params;
		params.Count = 1;
		EncoderParameter param;
		param.Guid = EncoderQuality;
		param.NumberOfValues = 1;
		param.Type = EncoderParameterValueTypeLong;
		int nQuality = 100;
		param.Value = &nQuality;
		params.Parameter[0] = param;

		Status stat = pImage->Save(szDstFile, &clsid, &params);
		if (stat == Ok)
		{
			bConvert = TRUE;
		}

		SafeDelete(pImage);
	}

	return bConvert;
}

BOOL SaveImage(Image* pImage, LPCWSTR szDstFile, LPCWSTR szCodecMimeType)
{
	if(!pImage)
	{
		return FALSE;
	}
	BOOL bConvert = FALSE;
	CLSID clsid;
	int nRet = GetEncoderClsid(szCodecMimeType, &clsid); //Get CLSID
	if (nRet >= 0)
	{
		EncoderParameters params;
		params.Count = 1;
		EncoderParameter param;
		param.Guid = EncoderQuality;
		param.NumberOfValues = 1;
		param.Type = EncoderParameterValueTypeLong;
		int nQuality = 100;
		param.Value = &nQuality;
		params.Parameter[0] = param;

		Status stat = pImage->Save(szDstFile, &clsid, &params);
		if (stat == Ok)
		{
			bConvert = TRUE;
		}
	}

	return bConvert;
}

BOOL SaveImage(Image* pImage, IStream ** ppStream, LPCWSTR szCodecMimeType)
{
	if(ppStream == NULL)
		return FALSE;
	ASSERT(*ppStream == NULL);
	if(!pImage)
	{
		return FALSE;
	}
	*ppStream = SaveImage2Stream(pImage, szCodecMimeType);
	if(*ppStream)
		return TRUE;
	return FALSE;
}

void SafeReleaseIStream(IStream* pObj)
{
	if(pObj)
	{
		pObj->Release();
		pObj = NULL;
	}
}

IStream* SaveImage2Stream(Image* pImage, LPCWSTR szCodecMimeType)
{
	if(!pImage)
	{
		return NULL;
	}
	IStream* pStream = NULL;
	CLSID clsid;
	int nRet = GetEncoderClsid(szCodecMimeType, &clsid); //Get CLSID
	if (nRet >= 0)
	{
		CreateStreamOnHGlobal(NULL, TRUE, &pStream);
		if(pStream)
		{
			EncoderParameters params;
			params.Count = 1;
			EncoderParameter param;
			param.Guid = EncoderQuality;
			param.NumberOfValues = 1;
			param.Type = EncoderParameterValueTypeLong;
			int nQuality = 100;
			param.Value = &nQuality;
			params.Parameter[0] = param;
			Status stat = pImage->Save(pStream, &clsid, &params);
			if (stat != Ok)
			{
				pStream->Release();
				pStream = NULL;
			}
		}
	}

	return pStream;
}


BOOL GetFitinSize(long nSrcW, long nSrcH, _Out_ long *nW, _Out_ long *nH, long nDisW, long nDisH)
{
	if (nSrcH*nSrcW == 0)
	{
		return FALSE;
	}
	long nDibWxnDisH=0, nDibHxnDisW=0;

	nDibWxnDisH = nSrcW * nDisH;
	nDibHxnDisW = nSrcH * nDisW;
	if (nDibWxnDisH > nDibHxnDisW){
		*nW = nDisW;
		*nH	= (nDibHxnDisW + (nSrcW>>1)) / nSrcW;
	}else
	{
		*nW = (nDibWxnDisH + (nSrcH>>1)) / nSrcH;
		*nH	= nDisH;
	}
	if (*nW <= 0)
		*nW = 1;
	if (*nH <= 0)
		*nH = 1;
	return TRUE;
}

BOOL GetFitinSize( long nSrcW, long nSrcH, _Out_ float *fW, _Out_ float *fH, long nDisW, long nDisH )
{
	if (nSrcH * nSrcW == 0)
	{
		return FALSE;
	}
	float fDibWxnDisH = 0.0, fDibHxnDisW = 0.0;

	fDibWxnDisH = nSrcW * nDisH;
	fDibHxnDisW = nSrcH * nDisW;

	if (fDibWxnDisH > fDibHxnDisW)
	{
		*fW = (float)nDisW;
		*fH = (fDibHxnDisW + (nSrcW>>1))/nSrcW;
	}
	else
	{
		*fW = (fDibWxnDisH + (nSrcH>>1))/nSrcH;
		*fH = (float)nDisH;
	}
	if (*fW <= 0.0)
	{
		*fW = 1.0;
	}
	if (*fH <= 0.0)
	{
		*fH = 1.0;
	}
	return TRUE;
}

//Image* GetThumbnailImage(Image* pImage, UINT nWidth, UINT nHeight)
//{
//	Image* pImgThumbnail = NULL;
//
//	//计算位置
//	UINT nSrcWidth = pImage->GetWidth();
//	UINT nSrcHeight = pImage->GetHeight();
//
//	if(nSrcWidth <= nWidth && nSrcHeight <= nHeight)//图片小于目标图
//	{
//		pImgThumbnail = pImage->Clone();
//	}
//	else
//	{
//		UINT nThumbWidth = 0;
//		UINT nThumbHeight = 0;
//		GetFitinSize(nSrcWidth, nSrcHeight, (long*)&nThumbWidth, (long*)&nThumbHeight, nWidth, nHeight);
//		pImgThumbnail = new Bitmap(nThumbWidth, nThumbHeight, PixelFormat24bppRGB);
//		Graphics* pbmpGraphics = Graphics::FromImage(pImgThumbnail);
//		pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
//		pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
//		Color color(255, 255, 255, 255);
//		pbmpGraphics->Clear(color);
//		pbmpGraphics->DrawImage(pImage, 0, 0, nThumbWidth, nThumbHeight);
//		SafeDelete(pbmpGraphics);
//	}
//
//	return pImgThumbnail;
//}
//
//Image* GetFullFitThumbnailImage(Image* pImage, UINT nWidth, UINT nHeight)
//{
//	if(pImage == NULL)
//		return NULL;
//
//	int nSrcWidth = pImage->GetWidth();
//	int nSrcHeight = pImage->GetHeight();
//	if(nSrcWidth <= nWidth || nSrcHeight <= nHeight)
//		return pImage;
//
//	int nThumbWidth;
//	int nThumbHeight;
//	if(nSrcWidth * nHeight > nSrcHeight * nWidth)
//	{
//		nThumbHeight = nHeight;
//		nThumbWidth = nSrcWidth * nHeight / nSrcHeight;
//	}
//	else
//	{
//		nThumbWidth = nWidth;
//		nThumbHeight = nSrcHeight * nWidth / nSrcWidth;
//	}
//	if(nThumbWidth <= 0)
//	{
//		nThumbWidth = 1;
//	}
//	if(nThumbHeight <= 0)
//	{
//		nThumbHeight = 1;
//	}
//
//	Image* pImgThumb = NULL;
//	pImgThumb = new Bitmap(nThumbWidth, nThumbHeight, PixelFormat24bppRGB);
//	Graphics* pbmpGraphics = Graphics::FromImage(pImgThumb);
//	pbmpGraphics->SetSmoothingMode(SmoothingModeHighQuality);
//	pbmpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
//	pbmpGraphics->DrawImage(pImage, 0, 0, nThumbWidth, nThumbHeight);
//	SafeDelete(pbmpGraphics);
//
//	return pImgThumb;
//}


BYTE* Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight)
{
	BITMAP bmpBmp;
	LPBITMAPINFO pbmiInfo;
	BITMAPINFO bmiInfo;
	WORD wBmpWidth, wBmpHeight;

	GetObject(pBitmap, sizeof(bmpBmp),&bmpBmp);
	pbmiInfo   = (LPBITMAPINFO)&bmpBmp;

	wBmpWidth  = (WORD)pbmiInfo->bmiHeader.biWidth;
	wBmpWidth -= (wBmpWidth%4);
	wBmpHeight = (WORD)pbmiInfo->bmiHeader.biHeight;

	*pwWidth  = wBmpWidth;
	*pwHeight = wBmpHeight;

	BYTE *pPixels = new BYTE[wBmpWidth*wBmpHeight*3];
	if (!pPixels) return NULL;

	HDC hDC =::GetWindowDC(NULL);

	bmiInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiInfo.bmiHeader.biWidth = wBmpWidth;
	bmiInfo.bmiHeader.biHeight = -wBmpHeight;
	bmiInfo.bmiHeader.biPlanes = 1;
	bmiInfo.bmiHeader.biBitCount = 24;
	bmiInfo.bmiHeader.biCompression = BI_RGB;
	bmiInfo.bmiHeader.biSizeImage = wBmpWidth*wBmpHeight*3;
	bmiInfo.bmiHeader.biXPelsPerMeter = 0;
	bmiInfo.bmiHeader.biYPelsPerMeter = 0;
	bmiInfo.bmiHeader.biClrUsed = 0;
	bmiInfo.bmiHeader.biClrImportant = 0;

	// get pixels from the original bitmap converted to 24bits
	int iRes = GetDIBits(hDC,pBitmap,0,wBmpHeight,(LPVOID)pPixels,&bmiInfo,DIB_RGB_COLORS);

	// release the device context
	::ReleaseDC(NULL,hDC);

	// if failed, cancel the operation.
	if (!iRes)
	{
		SafeDeleteArray(pPixels);
		return NULL;
	};

	// return the pixel array
	return pPixels;
}
HRGN GenerateRegion(HBITMAP hBitmap, COLORREF clrTransparent)
{
	WORD wBmpWidth,wBmpHeight;
	HRGN hRgn, hTmpRgn;

	// 24bit pixels from the bitmap
	BYTE *pPixels = Get24BitPixels(hBitmap, &wBmpWidth, &wBmpHeight);
	if (!pPixels) return NULL;

	// create our working region
	hRgn = CreateRectRgn(0,0,wBmpWidth,wBmpHeight);
	if (!hRgn) { SafeDeleteArray(pPixels); return NULL; }

	BYTE red = GetRValue(clrTransparent);
	BYTE green = GetGValue(clrTransparent);
	BYTE blue = GetBValue(clrTransparent);

	DWORD p=0;
	for (WORD y=0; y<wBmpHeight; y++)
	{
		for (WORD x=0; x<wBmpWidth; x++)
		{
			BYTE jRed   = pPixels[p+2];
			BYTE jGreen = pPixels[p+1];
			BYTE jBlue  = pPixels[p+0];

			if (jRed==red && jGreen==green && jBlue==blue)
			{
				// remove transparent color from region
				hTmpRgn = CreateRectRgn(x,y,x+1,y+1);
				CombineRgn(hRgn, hRgn, hTmpRgn, RGN_XOR);
				DeleteObject(hTmpRgn);
			}

			// next pixel
			p+=3;
		}
	}

	// release pixels
	SafeDeleteArray(pPixels);

	// return the region
	return hRgn;
}

//HRGN GenerateRegion(CSize size)
//{
//	ASSERT(size.cx > 10);
//	ASSERT(size.cy > 10);
//	CRgn rgn;
//	rgn.CreateRectRgn(0, 0, size.cx, size.cy);
//
//	CRgn rgnTmp;
//	rgnTmp.CreateRectRgn(0, 0, 5, 1);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, 1, 3, 2);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, 2, 2, 3);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, 3, 1, 4);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, 4, 1, 5);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//
//	rgnTmp.CreateRectRgn(0, size.cy - 5, 1, size.cy - 4);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, size.cy - 4, 1, size.cy - 3);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, size.cy - 3, 2, size.cy - 2);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, size.cy - 2, 3, size.cy - 1);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(0, size.cy - 1, 5, size.cy);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//
//	rgnTmp.CreateRectRgn(size.cx - 5, 0, size.cx, 1);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 3, 1, size.cx, 2);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 2, 2, size.cx, 3);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 1, 3, size.cx, 4);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 1, 4, size.cx, 5);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//
//	rgnTmp.CreateRectRgn(size.cx - 1, size.cy - 5, size.cx, size.cy - 4);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 1, size.cy - 4, size.cx, size.cy - 3);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 2, size.cy - 3, size.cx, size.cy - 2);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 3, size.cy - 2, size.cx, size.cy - 1);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//	rgnTmp.CreateRectRgn(size.cx - 5, size.cy - 1, size.cx, size.cy);
//	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
//	rgnTmp.DeleteObject();
//
//	return (HRGN)rgn.Detach();
//}

HRGN GenerateRegion(CSize size)
{
	ASSERT(size.cx > 10);
	ASSERT(size.cy > 10);
	CRgn rgn;
	rgn.CreateRectRgn(0, 0, size.cx, size.cy);

	CRgn rgnTmp;
	rgnTmp.CreateRectRgn(0, 0, 3, 1);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(0, 1, 2, 2);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(0, 2, 1, 3);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();

	rgnTmp.CreateRectRgn(0, size.cy - 3, 1, size.cy - 2);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(0, size.cy - 2, 2, size.cy - 1);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(0, size.cy - 1, 3, size.cy);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();

	rgnTmp.CreateRectRgn(size.cx - 3, 0, size.cx, 1);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(size.cx - 2, 1, size.cx, 2);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(size.cx - 1, 2, size.cx, 3);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();

	rgnTmp.CreateRectRgn(size.cx - 1, size.cy - 3, size.cx, size.cy - 2);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(size.cx - 2, size.cy - 2, size.cx, size.cy - 1);
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();
	rgnTmp.CreateRectRgn(size.cx - 3, size.cy - 1, size.cx, size.cy );
	rgn.CombineRgn(&rgn, &rgnTmp, RGN_XOR);
	rgnTmp.DeleteObject();

	return (HRGN)rgn.Detach();
}


BOOL GetBestFitSize(long nSrcW, long nSrcH, long *nW, long *nH, long nDisW, long nDisH)
{
	if (nDisH >= nSrcH || nDisW >= nSrcW) //one
	{
		*nW = nSrcW;
		*nH = nSrcH;
	}
	else
	{
		float fRadio = (float) nSrcH/(float)nSrcW;
		if ((long)(nDisW * fRadio) >= nDisH)
		{
			*nW = nDisW;
			*nH =(long) ((*nW) * fRadio);
		}
		else
		{
			*nH = nDisH;
			*nW =(long) ((*nH)/fRadio);
		}
	}

	if (*nW <= 0)
		*nW = 1;
	if (*nH <= 0)
		*nH = 1;

	return TRUE;
}


Rect DrawInDesignatedRect(const Rect& rcDesignated, Graphics& g, Image* pImage, ImageAttributes* imgAttr/* = NULL*/, int nAlign/* = DIDR_CENTER*/,BOOL bForceDesignated)
{
	Rect rcDraw;
	if (!pImage)
		return rcDraw;

	int nMaxHeight = rcDesignated.Height;
	int nMaxWidth = rcDesignated.Width;
	int nBmpHeight = pImage->GetHeight();
	int nBmpWidth = pImage->GetWidth();

	if (nBmpHeight <= nMaxHeight && nBmpWidth <= nMaxWidth)
	{
		if(bForceDesignated && nBmpWidth<nBmpHeight)
		{
			rcDraw.Height=nMaxHeight;
			float fZoom=(float)nMaxHeight/(float)nBmpHeight;
			rcDraw.Width=nBmpWidth*fZoom;		
		} else
		{
			rcDraw.Width = nBmpWidth;
			rcDraw.Height = nBmpHeight;
		}	
	}
	else
	{
		// 按比例缩小
		LONG nW = 0;
		LONG nH = 0;
		GetFitinSize(nBmpWidth, nBmpHeight, &nW, &nH, nMaxWidth, nMaxHeight);
		rcDraw.Width = nW;
		rcDraw.Height = nH;
	}
	switch(nAlign)
	{
	case DIDR_CENTER:
		{
			rcDraw.X = rcDesignated.X + (int)((nMaxWidth - rcDraw.Width)/ 2.0);
			rcDraw.Y = rcDesignated.Y + (int)((nMaxHeight - rcDraw.Height)/ 2.0);
		}
		break;
	case DIDR_LEFT:
		{
			rcDraw.X = rcDesignated.X;
			rcDraw.Y = rcDesignated.Y + (int)((nMaxHeight - rcDraw.Height)/ 2.0);
		}
		break;
	case DIDR_RIGHT:
		{
			rcDraw.X = rcDesignated.GetRight() - rcDraw.Width;
			rcDraw.Y = rcDesignated.Y + (int)((nMaxHeight - rcDraw.Height)/ 2.0);
		}
		break;
	case DIDR_TOP:
		{
			rcDraw.X = rcDesignated.X + (int)((nMaxWidth - rcDraw.Width)/ 2.0);
			rcDraw.Y = rcDesignated.Y;
		}
		break;
	case DIDR_BOTTOM:
		{
			rcDraw.X = rcDesignated.X + (int)((nMaxWidth - rcDraw.Width)/ 2.0);
			rcDraw.Y = rcDesignated.GetBottom() - rcDraw.Height;
		}
		break;
	}
	g.DrawImage(pImage, rcDraw, 0, 0, nBmpWidth, nBmpHeight, UnitPixel, imgAttr);

	return rcDraw;
}

RectF DrawInDesignatedRectF(const RectF& rcfDesignated, Graphics& g, Image* pImage, ImageAttributes* imgAttr /*= NULL*/, int nAlign /*= DIDR_CENTER*/)
{
	RectF rcfDraw;
	if (!pImage)
		return rcfDraw;

	int nMaxHeight = (int)rcfDesignated.Height;
	int nMaxWidth = (int)rcfDesignated.Width;
	int nBmpHeight = pImage->GetHeight();
	int nBmpWidth = pImage->GetWidth();

	if (nBmpHeight <= nMaxHeight && nBmpWidth <= nMaxWidth)
	{
		rcfDraw.Width = (float)nBmpWidth;
		rcfDraw.Height = (float)nBmpHeight;
	}
	else
	{
		// 按比例缩小
		float fW = 0.0;
		float fH = 0.0;
		GetFitinSize(nBmpWidth, nBmpHeight, &fW, &fH, nMaxWidth, nMaxHeight);
		rcfDraw.Width = fW;
		rcfDraw.Height = fH;
	}
	switch(nAlign)
	{
	case DIDR_CENTER:
		{
			rcfDraw.X = rcfDesignated.X + (float)((nMaxWidth - rcfDraw.Width)/ 2.0);
			rcfDraw.Y = rcfDesignated.Y + (float)((nMaxHeight - rcfDraw.Height)/ 2.0);
		}
		break;
	case DIDR_LEFT:
		{
			rcfDraw.X = rcfDesignated.X;
			rcfDraw.Y = rcfDesignated.Y + (float)((nMaxHeight - rcfDraw.Height)/ 2.0);
		}
		break;
	case DIDR_RIGHT:
		{
			rcfDraw.X = rcfDesignated.GetRight() - rcfDraw.Width;
			rcfDraw.Y = rcfDesignated.Y + (float)((nMaxHeight - rcfDraw.Height)/ 2.0);
		}
		break;
	case DIDR_TOP:
		{
			rcfDraw.X = rcfDesignated.X + (float)((nMaxWidth - rcfDraw.Width)/ 2.0);
			rcfDraw.Y = rcfDesignated.Y;
		}
		break;
	case DIDR_BOTTOM:
		{
			rcfDraw.X = rcfDesignated.X + (float)((nMaxWidth - rcfDraw.Width)/ 2.0);
			rcfDraw.Y = rcfDesignated.GetBottom() - rcfDraw.Height;
		}
		break;
	}

	g.DrawImage(pImage, rcfDraw, 0, 0, (REAL)nBmpWidth, (REAL)nBmpHeight, UnitPixel, imgAttr);

	return rcfDraw;
}

int GetStringLength(HWND hWnd, const CString& strText, Gdiplus::Font* pFont)
{
	ASSERT(hWnd && pFont);
	if(strText.IsEmpty())
		return 0;

	Graphics g(hWnd);
	StringFormat stringFormat;
	InitStringFormat(stringFormat, 
		StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces, 
		StringAlignmentNear, StringAlignmentCenter);
	PointF ptfOrigin(0, 0);
	RectF rcfOut(0,0,0,0);
	g.MeasureString(strText, strText.GetLength(), pFont, ptfOrigin, &stringFormat, &rcfOut);
	return (int)rcfOut.Width;
}

SizeF GetTextBounds(const Gdiplus::Font* font,const StringFormat& strFormat,const CString& szText)
{
	GraphicsPath path;
	FontFamily fontfamily;
	font->GetFamily(&fontfamily);
	path.AddString(szText,-1,&fontfamily,font->GetStyle(),font->GetSize(),PointF(0,0),&strFormat);
	RectF rcBound;
	path.GetBounds(&rcBound);
	return SizeF(rcBound.Width,rcBound.Height);
}

int FindMaxStringLength(HWND hWnd, Gdiplus::Font* pFont, const std::vector<CString>& vString, int nDefaultMaxLength/* = 2000*/)
{
	ASSERT(hWnd && pFont);

	Graphics g(hWnd);
	StringFormat stringFormat;
	InitStringFormat(stringFormat, 
		StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces, 
		StringAlignmentNear, StringAlignmentCenter);
	int nMaxLength = 0;
	for (std::vector<CString>::const_iterator citer = vString.begin();citer != vString.end();citer++)
	{
		if(!citer->IsEmpty())
		{
			PointF ptfOrigin(0, 0);
			RectF rcfOut(0,0,0,0);
			g.MeasureString(*citer, (*citer).GetLength(), pFont, ptfOrigin, &stringFormat, &rcfOut);
			if (rcfOut.Width > nMaxLength)
				nMaxLength = rcfOut.Width;
			if (nMaxLength > nDefaultMaxLength)
			{
				nMaxLength = nDefaultMaxLength;
				break;
			}
		}
	}	
	return nMaxLength;
}

int GraphicsCutString(LPCTSTR szStr, HFONT hFont, int nWidth, int wordSpace/* = 0*/, BOOL bBreakOnReturn /*= TRUE*/, SIZE * pSize /* = NULL */)
{
	ASSERT(szStr != NULL);
	ASSERT(hFont != NULL);
	if(szStr == NULL || hFont == NULL)
	{
		return -1;
	}
	if(pSize)
	{
		pSize->cx = -1;
		pSize->cy = -1;
	}

	int nCharIndex = 0;
	int nStringLen = lstrlen(szStr);
	int nBeginPos = 0;
	TEXTMETRIC tm;

	SIZE size;

	HDC hdc = ::CreateCompatibleDC(NULL);
	if(hdc == NULL)
	{
		return -1;
	}
	::SelectObject(hdc, hFont);
	::GetTextMetrics(hdc, &tm); 
	::SetTextCharacterExtra(hdc, wordSpace);

	if(nWidth <= 0)
	{
		for(; nCharIndex < nStringLen; ++ nCharIndex)
		{
			if(*(szStr + nCharIndex) == '\n')
			{
				break;
			}
		}
		if(nCharIndex != 0)
		{
			::GetTextExtentPoint32(hdc, szStr, nCharIndex, &size);
			if(pSize)
			{
				if(pSize->cx < size.cx)
				{
					pSize->cx = size.cx;
				}
				
				if(pSize->cy < size.cy)
				{
					pSize->cy = size.cy;
				}
			}
		}
	}
	else
	{
		while( nStringLen > 0 )
		{
			if(bBreakOnReturn && *(szStr + nCharIndex) == '\n')
			{
				++nCharIndex;
				break;
			}
			if(!::GetTextExtentPoint32(hdc, szStr + nCharIndex, 1, &size))
			{
				size.cx = 0;
				size.cy = 0;
			}
			-- nStringLen;
			++ nCharIndex;

			if(size.cx == 0 && size.cy == 0)
			{
				break;
			}
			if ( nBeginPos + size.cx > nWidth )
			{
				-- nCharIndex;
				break;
			}
			nBeginPos += size.cx;
			if(pSize && pSize->cy < size.cy)
			{
				pSize->cy = size.cy;
			}
		}
	}

	::DeleteDC(hdc);
	return nCharIndex;
}

RectF DrawStringSpecifyLineDistance(Graphics& g, LPCTSTR szText, int& nLength, const Gdiplus::Font* pFont, const RectF& rcfLayoutRect, 
	const StringFormat* pStringFormat, const Brush* pBrush, int nLineDistance, BOOL bOnlyMeasure/* = FALSE*/)
{
	RectF rcfResoult;

	REAL fontHeight = pFont->GetHeight(&g);
	if(fontHeight < 0)
		fontHeight = -fontHeight;
	fontHeight *= 2;
	fontHeight -= 1;

	RectF rcfOneLineRect(rcfLayoutRect);
	if(rcfOneLineRect.Height > fontHeight)
		rcfOneLineRect.Height = fontHeight;

	int nHasDrawCount = 0;
	RectF rcfBound;
	int nCount = 0;
	g.MeasureString(szText + nHasDrawCount, nLength - nHasDrawCount, pFont, rcfOneLineRect, pStringFormat, &rcfBound, &nCount);
	if(!bOnlyMeasure)
	{
		BOOL bLastLine = FALSE;
		if(rcfOneLineRect.GetTop() + nLineDistance >= rcfLayoutRect.GetBottom())
			bLastLine = TRUE;

		g.DrawString(szText + nHasDrawCount, bLastLine?(nLength - nHasDrawCount):nCount, pFont, rcfBound, pStringFormat, pBrush);
	}
	rcfResoult = rcfBound;
	nHasDrawCount += nCount;
	while(nHasDrawCount < nLength && rcfBound.GetBottom() < rcfLayoutRect.GetBottom())
	{
		rcfOneLineRect.Offset(0, nLineDistance);
		if(rcfOneLineRect.GetTop() >= rcfLayoutRect.GetBottom())
			break;

		if(rcfOneLineRect.GetBottom() > rcfLayoutRect.GetBottom())
			rcfOneLineRect.Height = rcfOneLineRect.GetBottom() - rcfOneLineRect.Y;

		nCount = 0;
		g.MeasureString(szText + nHasDrawCount, nLength - nHasDrawCount, pFont, rcfOneLineRect, pStringFormat, &rcfBound, &nCount);
		if(!bOnlyMeasure)
		{
			BOOL bLastLine = FALSE;
			if(rcfOneLineRect.GetTop() + nLineDistance >= rcfLayoutRect.GetBottom())
				bLastLine = TRUE;

			g.DrawString(szText + nHasDrawCount, bLastLine?(nLength - nHasDrawCount):nCount, pFont, rcfBound, pStringFormat, pBrush);
		}
		rcfResoult = rcfBound;
		nHasDrawCount += nCount;
	}

	nLength = nHasDrawCount;
	return rcfResoult;
}

Image* GetImageFromBase64(const char* szText)
{
	//TODO: implement base64
	return NULL;
	//int nCodeLength = strlen(szText);
	//Image* pImage = NULL;
	//int nLength = Base64::GetDataLength(nCodeLength);
	//LPBYTE pBuffer = new BYTE[nLength];
	//nLength = Base64::Decode(szText, nCodeLength, pBuffer);
	//if(nLength == 0)
	//{
	//	SafeDeleteArray(pBuffer);
	//	return NULL;
	//}
	//HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED, nLength);
	//LPBYTE pBuffer2 = (unsigned char*)GlobalLock(hGlobal);
	//memcpy_s(pBuffer2, nLength, pBuffer, nLength);
	//GlobalUnlock(hGlobal);
	//SafeDeleteArray(pBuffer);
	//IStream* pStream = NULL;
	//HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	//if(FAILED(hr))
	//{
	//	GlobalFree(hGlobal);
	//}
	//else
	//{
	//	pImage = Image::FromStream(pStream);
	//	if(pImage->GetLastStatus() != Ok)
	//		SafeDelete(pImage);
	//}
	//return pImage;
}

void DrawCustomText( Graphics& g, const CString& strText, const Rect& rcText, Gdiplus::Font* pFont, Brush* pBrush, int nFormatType /*= font_center*/ )
{
	StringFormat stringFormat;
	stringFormat.SetFormatFlags(StringFormatFlagsLineLimit | StringFormatFlagsNoWrap);
	stringFormat.SetLineAlignment(StringAlignmentCenter);
	stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);
	if (nFormatType == font_center)
		stringFormat.SetAlignment(StringAlignmentCenter);
	else if (nFormatType == font_near)
		stringFormat.SetAlignment(StringAlignmentNear);
	else
		stringFormat.SetAlignment(StringAlignmentFar);
	DrawCustomText(g, strText, rcText, pFont, pBrush, stringFormat);
}

void DrawCustomText( Graphics& g, const CString& strText, const Rect& rcText, Gdiplus::Font* pFont, Brush* pBrush, StringFormat& stringFormat )
{
	RectF rcfText = Rect2RectF(rcText);
	g.DrawString(strText, strText.GetLength(), pFont, rcfText, &stringFormat, pBrush);
}

BOOL RotateImage(const CString& strPath, RotateFlipType eRotateFlipType)
{
	BOOL bFlag = FALSE;

	Image* pImage = LoadImageFromFile(strPath);
	if(pImage == NULL)
		return FALSE;

	LPCWSTR szCodecMimeType = L"image/png";
	GUID guid = GUID_NULL;
	pImage->GetRawFormat(&guid);
	if(guid == ImageFormatJPEG)
	{
		szCodecMimeType = L"image/jpeg";
	}
	else if(guid == ImageFormatBMP)
	{
		szCodecMimeType = L"image/bmp";
	}
	else if(guid == ImageFormatGIF)
	{
		szCodecMimeType = L"image/gif";
	}
	else if(guid == ImageFormatTIFF)
	{
		szCodecMimeType = L"image/tiff";
	}
	int nRet = pImage->RotateFlip(eRotateFlipType) == Ok ? TRUE : FALSE;
	CLSID clsid;
	nRet = GetEncoderClsid(szCodecMimeType, &clsid);
	if(nRet)
	{
		if(pImage->Save(strPath, &clsid, NULL) == Ok)
		{
			bFlag = TRUE;
		}
	}

	SafeDelete(pImage);

	return bFlag;
}
BOOL RotateImage(const CString& strPath, int nAngle)
{
	RotateFlipType eRotateFlipType;
	if(nAngle == 90)
		eRotateFlipType = Rotate90FlipNone;
	else if(nAngle == 270)
		eRotateFlipType = Rotate270FlipNone;
	else if(nAngle == 180)
		eRotateFlipType = Rotate180FlipNone;
	return RotateImage(strPath,eRotateFlipType);
}

// BOOL SaveImage2Base64( Image* pImage, std::string& strBase64 )
// {
// 	BOOL bSuccess = FALSE;
// 
// 	IStream* pStream = SaveImage2Stream(pImage, TEXT("image/png"));
// 	if (pStream)
// 	{
// 		HGLOBAL hGlobal = NULL;
// 		HRESULT hr = GetHGlobalFromStream(pStream, &hGlobal);
// 		if (SUCCEEDED(hr))
// 		{
// 			LPBYTE pBuffer = (LPBYTE)GlobalLock(hGlobal);
// 			UINT uLength = GlobalSize(hGlobal);
// 			Base64::Encode(pBuffer, uLength, strBase64);
// 			GlobalUnlock(hGlobal);
// 
// 			bSuccess = TRUE;
// 		}
// 		SafeRelease(pStream);
// 	}
// 
// 	return bSuccess;
// }

void DrawInRgionClip( Graphics& g, Image* pImage, const Rect& rcInner, int nLeft, int nTop, int nRight, int nBottom, int nRoundW, int nRoundH )
{
	GraphicsPath* pOPath = new GraphicsPath();
	Point myPointArray[] = 
	{
		Point(nLeft, nTop + nRoundH), Point(nLeft + nRoundW, nTop),
		Point(nRight - nRoundW, nTop), Point(nRight, nTop + nRoundH),
		Point(nRight, nBottom - nRoundH), Point(nRight - nRoundW, nBottom),
		Point(nLeft + nRoundW, nBottom), Point(nLeft, nBottom - nRoundH),
		Point(nLeft, nTop + nRoundH)
	};
	pOPath->AddPolygon(myPointArray, 9);
	Region* pRg = new Region(pOPath);
	g.SetClip(pRg);

	DrawInDesignatedRect(rcInner, g, pImage);
	g.ResetClip();
}


////已nMaxWidth*nMaxHeight的大小自适应图片，然后从自适应的图片中居中截取并截掉水平和垂直偏移的区域
//Image* GetFitinSizeThumbByCenter(Image* pSrcImg, int nMaxWidth, int nMaxHeight, int nHorOffset/* = 0*/, int nVerOffset/* = 0*/)
//{
//	if(!pSrcImg)
//	{
//		return NULL;
//	}
//
//	Bitmap * pBmp = new Bitmap(nMaxWidth, nMaxHeight, PixelFormat32bppARGB);
//	ASSERT(pBmp);
//	if(pBmp)
//	{
//		int nSrcWidth = pSrcImg->GetWidth();
//		int nSrcHeight = pSrcImg->GetHeight();
//		LONG nW = 0;
//		LONG nH = 0;
//		GetBestFitSize(nSrcWidth, nSrcHeight, &nW, &nH, nMaxWidth + nHorOffset * 2, nMaxHeight + nVerOffset * 2);
//		
//// 		ASSERT(nW >= nMaxWidth + nHorOffset * 2 && nH >= nMaxHeight + nVerOffset * 2);
//// 			if(nW >= nMaxWidth + nHorOffset * 2 && nH >= nMaxHeight + nVerOffset * 2)
//	//	{
//			Image* pThumb = GetThumbnailImage(pSrcImg, nW, nH);
//			Graphics* pG = Graphics::FromImage(pBmp);
//			ASSERT(pG && pThumb);
//			if(pG && pThumb)
//			{
//				//ASSERT(((nW - nMaxWidth) / 2 + nHorOffset + nMaxWidth <= nW) && ((nH - nMaxHeight) / 2 + nVerOffset + nMaxHeight <= nH));
//				pG->DrawImage(pThumb, 0, 0, (nW - nMaxWidth) / 2 + nHorOffset, (nH - nMaxHeight) / 2 + nVerOffset, nMaxWidth, nMaxHeight, UnitPixel);
//			}
//			SafeDelete(pThumb);
//			SafeDelete(pG);
//	//	}	
//	}
//	return pBmp;
//}

BOOL IsAreaCrossed(const Rect& rc1, const Rect& rc2)
{
	RECT rcTmp;  
	rcTmp.left = max(rc1.X, rc2.X);  
	rcTmp.top = max(rc1.Y, rc2.Y);  
	rcTmp.right = min(rc1.X + rc1.Width, rc2.X + rc2.Width);  
	rcTmp.bottom = min(rc1.Y + rc1.Height, rc2.Y + rc2.Height);  
	return (rcTmp.right >= rcTmp.left && rcTmp.bottom >= rcTmp.top);  
}

Image* LoadImageFromBase64(const std::string& strBase64)
{
	//TODO: implement Base64
	return NULL;
	//Image* pImage = NULL;
	//int nLength = Base64::GetDataLength(strBase64.size());
	//LPBYTE pBuffer = new BYTE[nLength];
	//nLength = Base64::Decode(strBase64, strBase64.size(), pBuffer);
	//if(nLength == 0)
	//{
	//	SafeDeleteArray(pBuffer);
	//	return NULL;
	//}
	//HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED, nLength);
	//LPBYTE pBuffer2 = (LPBYTE)GlobalLock(hGlobal);
	//memcpy_s(pBuffer2, nLength, pBuffer, nLength);
	//GlobalUnlock(hGlobal);
	//SafeDeleteArray(pBuffer);
	//IStream* pStream = NULL;
	//HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	//if(FAILED(hr))
	//{
	//	GlobalFree(hGlobal);
	//}
	//else
	//{
	//	pImage = Image::FromStream(pStream);
	//	if(pImage->GetLastStatus() != Ok)
	//		SafeDelete(pImage);
	//}
	//return pImage;
}
