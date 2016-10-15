#include "pdfium_custom.h"
#include "../repo/pdfium/public/fpdf_edit.h"
#include "../repo/pdfium/core/fpdfapi/fpdf_page/include/cpdf_imageobject.h"
#include "../repo/pdfium/core/fpdfapi/fpdf_page/include/cpdf_image.h"
#include "../repo/pdfium/core/fpdfapi/fpdf_parser/include/cpdf_stream_acc.h"
#include "../repo/pdfium/fpdfsdk/include/fsdk_define.h"


DLLEXPORT int STDCALL FPDFPageObject_GetType(FPDF_PAGEOBJECT page_obj)
{
  int result = 0;
  CPDF_PageObject* pPageObj = (CPDF_PageObject*)page_obj;
  if (pPageObj)
  {
    result = pPageObj->GetType();
  }
  return result;
}


DLLEXPORT bool STDCALL FPDFPageObject_GetInfo(FPDF_PAGEOBJECT page_obj, FPDF_PAGEOBJECTINFO *pInfo)
{
  bool result = false;
  CPDF_PageObject* pPageObj = (CPDF_PageObject*)page_obj;
  if (pPageObj)
  {
    pInfo->Type = pPageObj->GetType();
    pInfo->Left = pPageObj->m_Left;
    pInfo->Right = pPageObj->m_Right;
    pInfo->Top = pPageObj->m_Top;
    pInfo->Bottom = pPageObj->m_Bottom;
    result = true;
  }
  return result;
}


DLLEXPORT bool STDCALL FPDFPageObject_GetImageInfo(FPDF_PAGEOBJECT page_obj, FPDF_IMAGEINFO *pInfo)
{
  bool result = false;

  if (FPDFPageObject_GetType(page_obj) == FPDF_PAGEOBJ_IMAGE)
  {
    CPDF_ImageObject* pImageObj = (CPDF_ImageObject*)page_obj;
    CPDF_Image* pImage = pImageObj->m_pImage;
    CPDF_Stream* pStream = pImage->GetStream();

    CPDF_Dictionary* pDict = pImage->GetDict();

    if (pDict)
    {
      if (pDict->KeyExist("BitsPerComponent"))
      {
        pInfo->BitsPerComponent = pDict->GetIntegerBy("BitsPerComponent");
      }
      if (pDict->KeyExist("ColorSpace"))
      {
        const CFX_ByteString& colorSpace = pDict->GetStringBy("ColorSpace");

        if (colorSpace == "DeviceGray")
        {
          pInfo->ColorSpace = 1;
        }
        else if (colorSpace == "DeviceRGB")
        {
          pInfo->ColorSpace = 2;
        }
        else if (colorSpace == "DeviceCMYK")
        {
          pInfo->ColorSpace = 3;
        }
        else
        {
          pInfo->ColorSpace = 4;
        }
      }
    }

    CPDF_StreamAcc* pAcc = new CPDF_StreamAcc();
    pAcc->LoadAllData(pStream, FALSE, 0, FALSE);

    pInfo->Height = pImage->GetPixelHeight();
    pInfo->Width = pImage->GetPixelWidth();
//    pInfo->DataSize = pStream->GetRawSize();
//    pInfo->pData = pStream->GetRawData();
    pInfo->DataSize = pAcc->GetSize();
    pInfo->pData = pAcc->GetData();

    const CFX_ByteString& decoder = pAcc->GetImageDecoder();

    if (decoder == "CCITTFaxDecode")
    {
      pInfo->Filter = 1;
    }
    else if (decoder == "DCTDecode")
    {
      pInfo->Filter = 2;
    }
    else if (decoder == "FlateDecode")
    {
      pInfo->Filter = 3;
    }
    else if (decoder == "JPXDecode")
    {
      pInfo->Filter = 4;
    }
    else if (decoder == "JBIG2Decode")
    {
      pInfo->Filter = 5;
    }
    else if (decoder == "RunLengthDecode")
    {
      pInfo->Filter = 6;
    }
    else
    {
      pInfo->Filter = 0;
    }

    pInfo->Matrix.a = pImageObj->m_Matrix.a;
    pInfo->Matrix.b = pImageObj->m_Matrix.b;
    pInfo->Matrix.c = pImageObj->m_Matrix.c;
    pInfo->Matrix.d = pImageObj->m_Matrix.d;
    pInfo->Matrix.e = pImageObj->m_Matrix.e;
    pInfo->Matrix.f = pImageObj->m_Matrix.f;

    result = true;
  }

  return result;
}


DLLEXPORT FPDF_BOOL STDCALL FPDFImageObj_SetJpeg(FPDF_PAGE* pages,
                                                 int nCount,
                                                 FPDF_PAGEOBJECT image_object,
                                                 void* pData,
                                                 uint32_t size)
{
  if (!image_object || !pData || !pages)
    return FALSE;
  CPDF_ImageObject* pImgObj = (CPDF_ImageObject*)image_object;
  pImgObj->m_GeneralState.GetModify();
  for (int index = 0; index < nCount; index++)
  {
    CPDF_Page* pPage = CPDFPageFromFPDFPage(pages[index]);
    if (!pPage)
      continue;
    pImgObj->m_pImage->ResetCache(pPage, nullptr);
  }
  pImgObj->m_pImage->SetJpegImage((uint8_t*)pData, size);
  pImgObj->CalcBoundingBox();
  return TRUE;
}



DLLEXPORT FPDF_BITMAP STDCALL FPDFBitmap_CreateEx2(int width,
                                                   int height,
                                                   FPDFPixel format,
                                                   void* first_scan,
                                                   int stride)
{
  FXDIB_Format fx_format;
  switch (format)
  {
    case FPDFPixel_1bppMask:
      fx_format = FXDIB_1bppMask;
      break;
    case FPDFPixel_1bppRgb:
      fx_format = FXDIB_1bppRgb;
      break;
    case FPDFPixel_1bppCmyk:
      fx_format = FXDIB_1bppCmyk;
      break;
    case FPDFPixel_8bppMask:
      fx_format = FXDIB_8bppMask;
      break;
    case FPDFPixel_8bppRgb:
      fx_format = FXDIB_8bppRgb;
      break;
    case FPDFPixel_8bppRgba:
      fx_format = FXDIB_8bppRgba;
      break;
    case FPDFPixel_8bppCmyk:
      fx_format = FXDIB_8bppCmyk;
      break;
    case FPDFPixel_8bppCmyka:
      fx_format = FXDIB_8bppCmyka;
      break;
    case FPDFPixel_Rgb:
      fx_format = FXDIB_Rgb;
      break;
    case FPDFPixel_Rgba:
      fx_format = FXDIB_Rgba;
      break;
    case FPDFPixel_Rgb32:
      fx_format = FXDIB_Rgb32;
      break;
    case FPDFPixel_Argb:
      fx_format = FXDIB_Argb;
      break;
    case FPDFPixel_Cmyk:
      fx_format = FXDIB_Cmyk;
      break;
    case FPDFPixel_Cmyka:
      fx_format = FXDIB_Cmyka;
      break;
    default:
      return nullptr;
  }
  CFX_DIBitmap* pBitmap = new CFX_DIBitmap;
  pBitmap->Create(width, height, fx_format, (uint8_t*)first_scan, stride);
  return pBitmap;
}


