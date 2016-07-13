#ifndef PDFIUM_CUSTOM_H
#define PDFIUM_CUSTOM_H
#include <stdint.h>
#include "..\Repo\pdfium\public\fpdfview.h"


typedef enum _FPDFPixel
{
  FPDFPixel_Invalid = 0,
  FPDFPixel_1bppMask,
  FPDFPixel_1bppRgb,
  FPDFPixel_1bppCmyk,
  FPDFPixel_8bppMask,
  FPDFPixel_8bppRgb,
  FPDFPixel_8bppRgba,
  FPDFPixel_8bppCmyk,
  FPDFPixel_8bppCmyka,
  FPDFPixel_Rgb,
  FPDFPixel_Rgba,
  FPDFPixel_Rgb32,
  FPDFPixel_Argb,
  FPDFPixel_Cmyk,
  FPDFPixel_Cmyka,
}
  FPDFPixel;


typedef struct _FPDF_PAGEOBJECTINFO
{
  int Type;
  float Left;
  float Right;
  float Top;
  float Bottom;
}
  FPDF_PAGEOBJECTINFO;


typedef struct _FPDF_IMAGEINFO
{
  int Height;
  int Width;
  int BitsPerComponent;
  int ColorSpace;
  int Filter;
  long DataSize;
  const unsigned char *pData;
}
  FPDF_IMAGEINFO;


DLLEXPORT int STDCALL FPDFPageObject_GetType(FPDF_PAGEOBJECT page_obj);

DLLEXPORT bool STDCALL FPDFPageObject_GetInfo(FPDF_PAGEOBJECT page_obj, FPDF_PAGEOBJECTINFO *pInfo);

DLLEXPORT bool STDCALL FPDFPageObject_GetImageInfo(FPDF_PAGEOBJECT page_obj, FPDF_IMAGEINFO *pInfo);

DLLEXPORT FPDF_BOOL STDCALL FPDFImageObj_SetJpeg(FPDF_PAGE* pages,
                                                 int nCount,
                                                 FPDF_PAGEOBJECT image_object,
                                                 void* pData,
                                                 uint32_t size);

DLLEXPORT FPDF_BITMAP STDCALL FPDFBitmap_CreateEx2(int width,
                                                   int height,
                                                   FPDFPixel format,
                                                   void* first_scan,
                                                   int stride);

#endif
