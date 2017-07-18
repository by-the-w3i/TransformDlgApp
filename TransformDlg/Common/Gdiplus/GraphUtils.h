#ifndef _GRAPHUTILS_H_
#define _GRAPHUTILS_H_
#include <vector>

using namespace Gdiplus;

#ifndef SafeInitImage
#define SafeInitImage(ptrRes, szRes) \
{ \
	ptrRes = LoadImageFromFile(GetImagePathName(szRes)); \
}
#endif

#ifndef SafeInitBitmap
#define SafeInitBitmap(ptrRes, szRes) \
{ \
	ptrRes = LoadBitmapFromFile(GetImagePathName(szRes)); \
}
#endif


#if !defined(SafeDestroyIcon)
#define SafeDestroyIcon(hIcon) {if(hIcon) {::DestroyIcon(hIcon); hIcon = NULL;}}
#endif

#ifndef InitStringFormat
#define InitStringFormat(stringFormat, flags, alignment, lineAlignment) \
	stringFormat.SetFormatFlags(flags); \
	stringFormat.SetAlignment(alignment); \
	stringFormat.SetLineAlignment(lineAlignment);
#endif

RECT Rect2RECT(const Rect& rcIn);
Rect RECT2Rect(const RECT& rcIn);
void DrawMargin(Graphics& g, Image* pImage, Rect& rcDst, int nMarginLeft, int nMarginRight, int nMarginTop, int nMarginBottom, BOOL bDrawCenter = TRUE, BOOL bTile = TRUE, float fAlpha = 1.0f);
void DrawMargin(Graphics& g, Image* pImage, Rect& rcDst, Rect& rcSrc, int nMarginLeft, int nMarginRight, int nMarginTop, int nMarginBottom, BOOL bDrawCenter = TRUE, BOOL bTile = TRUE, float fAlpha = 1.0f);
void DrawThirdPaintBg(Graphics& g, Image* pImage, Rect& rcClient, int nLineCount, int nCountPerLine, int nItemDraw);
void DrawThirdPaintBgY(Graphics& g, Image* pImage, Rect& rcClient, int nLineCount, int nCountPerLine, int nItemDraw);
void DrawRoundedRect(Graphics& g, Brush *pBrush, Pen *pen, const Rect &rect, int nRadius);

// 指定区域画任意大小图
enum { DIDR_CENTER, DIDR_LEFT, DIDR_RIGHT, DIDR_TOP, DIDR_BOTTOM };
Rect DrawInDesignatedRect(const Rect& rcDesignated, Graphics& g, Image* pImage, ImageAttributes* imgAttr = NULL, int nAlign = DIDR_CENTER, BOOL bForceDesignated = FALSE);
RectF DrawInDesignatedRectF(const RectF& rcfDesignated, Graphics& g, Image* pImage, ImageAttributes* imgAttr = NULL, int nAlign = DIDR_CENTER);
inline void SetRectEx(Rect& rc, int x, int y, int width, int height)
{
	rc.X = x;
	rc.Y = y;
	rc.Width = width;
	rc.Height = height;
}
inline void Rect2RectF(RectF& rcfDst, const Rect& rcSrc)
{
	rcfDst.X = static_cast<REAL>(rcSrc.X);
	rcfDst.Y = static_cast<REAL>(rcSrc.Y);
	rcfDst.Width = static_cast<REAL>(rcSrc.Width);
	rcfDst.Height = static_cast<REAL>(rcSrc.Height);
}
inline RectF Rect2RectF(const Rect& rcSrc)
{
	RectF rcfDst;
	rcfDst.X = static_cast<REAL>(rcSrc.X);
	rcfDst.Y = static_cast<REAL>(rcSrc.Y);
	rcfDst.Width = static_cast<REAL>(rcSrc.Width);
	rcfDst.Height = static_cast<REAL>(rcSrc.Height);
	return rcfDst;
}
inline Rect RectF2Rect(const RectF& rcfSrc)
{
	Rect rcDst;
	rcDst.X = static_cast<INT>(rcfSrc.X);
	rcDst.Y = static_cast<INT>(rcfSrc.Y);
	rcDst.Width = static_cast<INT>(rcfSrc.Width);
	rcDst.Height = static_cast<INT>(rcfSrc.Height);
	return rcDst;
}
int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);
Image* LoadImageFromFile(LPCTSTR szFileName);
Image* LoadImageFromStream(IStream* pStream);
Bitmap* LoadBitmapFromFile(LPCTSTR szFileName);
BOOL SaveImage(const CLSID& clsid, Image* pImage, const CString& strDst);
BOOL SaveBestFitImage(const WCHAR *format, const CString &strDst, const CString &strSrc, int& nWidth, int& nHeight);
BOOL SaveBestFitImage(const WCHAR *format, Image* pImage, const CString& strDst, int& nWidth, int& nHeight);

BOOL SaveImage(LPCWSTR szSrcFile, LPCWSTR szDstFile, LPCWSTR szCodecMimeType);
BOOL SaveImage(IStream* pStream, LPCWSTR szDstFile, LPCWSTR szCodecMimeType);
BOOL SaveImage(Image* pImage, LPCWSTR szDstFile, LPCWSTR szCodecMimeType);
BOOL SaveImage(Image* pImage, IStream** ppStream, LPCWSTR szCodecMimeType);

void SafeReleaseIStream(IStream* pObj);
IStream* SaveImage2Stream(Image* pImage, LPCWSTR szCodecMimeType);

BOOL GetFitinSize(long nSrcW, long nSrcH, _Out_ long *nW, _Out_ long *nH, long nDisW, long nDisH);
BOOL GetFitinSize(long nSrcW, long nSrcH, _Out_ float *fW, _Out_ float *fH, long nDisW, long nDisH);
HRGN GenerateRegion(CSize size);
HRGN GenerateRegion(HBITMAP hBitmap, COLORREF clrTransparent);
BYTE* Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight);

BOOL GetBestFitSize(long nSrcW, long nSrcH, long *nW, long *nH, long nDisW, long nDisH);
int GetStringLength(HWND hWnd, const CString& strText, Gdiplus::Font* pFont);
SizeF GetTextBounds(const Gdiplus::Font* font, const StringFormat& strFormat, const CString& szText);
int FindMaxStringLength(HWND hWnd, Gdiplus::Font* pFont, const std::vector<CString>& vString, int nDefaultMaxLength = 2000);
inline Rect MakeRect(Point pt1, Point pt2)
{
	Rect rc;
	if (pt1.Equals(pt2))
	{
		rc;
	}
	if (pt1.X < pt2.X)
	{
		rc.X = pt1.X;
		rc.Width = pt2.X - pt1.X;
	}
	else
	{
		rc.X = pt2.X;
		rc.Width = pt1.X - pt2.X;
	}

	if (pt1.Y < pt2.Y)
	{
		rc.Y = pt1.Y;
		rc.Height = pt2.Y - pt1.Y;
	}
	else
	{
		rc.Y = pt2.Y;
		rc.Height = pt1.Y - pt2.Y;
	}
	return rc;
}

int GraphicsCutString(LPCTSTR szStr, HFONT hFont, int nWidth = -1, int wordSpace = 0, BOOL bBreakOnReturn = TRUE, SIZE * pSize = NULL);
RectF DrawStringSpecifyLineDistance(Graphics& g, LPCTSTR szText, int& nLength, const Gdiplus::Font* pFont, const RectF& rcfLayoutRect,
	const StringFormat* pStringFormat, const Brush* pBrush, int nLineDistance, BOOL bOnlyMeasure = FALSE);
Image* GetImageFromBase64(const char* szText);

enum { font_near, font_center, font_far };
void DrawCustomText(Graphics& g, const CString& strText, const Rect& rcText, Gdiplus::Font* pFont, Brush* pBrush, int nFormatType = font_center);
void DrawCustomText(Graphics& g, const CString& strText, const Rect& rcText, Gdiplus::Font* pFont, Brush* pBrush, StringFormat& stringFormat);

BOOL RotateImage(const CString& strPath, int nAngle);
BOOL RotateImage(const CString& strPath, RotateFlipType eRotateFlipType);
//使用moloutil中的同名方法
//	BOOL SaveImage2Base64(Image* pImage, std::string& strBase64);

void DrawInRgionClip(Graphics& g, Image* pImage, const Rect& rcInner, int nLeft, int nTop, int nRight, int nBottom, int nRoundW, int nRoundH);

// Image* GetThumbnailImage(Image* pImage, UINT nWidth, UINT nHeight);
//	Image* GetFullFitThumbnailImage(Image* pImage, UINT nWidth, UINT nHeight);
//	Image* GetFitinSizeThumbByCenter(Image* pImgSrc, int nMaxWidth, int nMaxHeight, int nHorOffset = 0, int nVerOffset = 0);

BOOL IsAreaCrossed(const Rect& rc1, const Rect& rc2);

Image* LoadImageFromBase64(const std::string& strBase64);

#endif