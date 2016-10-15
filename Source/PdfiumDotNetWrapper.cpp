// This is the main DLL file.

#include "PdfiumDotNetWrapper.h"
#include <stdio.h> // for sprintf_s


using namespace PdfiumDotNetWrapper;
using namespace System::IO;
using namespace System::Runtime::InteropServices;


void Pdfium::InitLibrary()
{
  FPDF_InitLibrary();
}


void Pdfium::CloseLibrary()
{
  FPDF_DestroyLibrary();
}


IntPtr Pdfium::LoadDocument(FPDF_BYTESTRING fileName, FPDF_BYTESTRING password)
{
  return (IntPtr)FPDF_LoadDocument(fileName, password);
}


IntPtr Pdfium::LoadMemDocument(byte data_buf[], int size, FPDF_BYTESTRING password)
{
  return (IntPtr)FPDF_LoadMemDocument((void*)data_buf, size, password);
}


IntPtr Pdfium::CreateNewDocument()
{
  return (IntPtr)FPDF_CreateNewDocument();
}


int WriteBlock(struct FPDF_FILEWRITE_* pThis, const void* pBuffer, unsigned long size)
{
  int result = 0;

  if(Pdfium::fpStream != nullptr)
  {
    array<byte>^ buffer = gcnew array<byte>(size);
    System::Runtime::InteropServices::Marshal::Copy(IntPtr((byte*)pBuffer), buffer, 0, size);
    //Class1::fpStream->Write(buffer, (int)Class1::fpStream->Length, size);

    Pdfium::fpWriter->Write(buffer);
    result = 1;
  }

  return result;
}


bool Pdfium::SaveDocument(IntPtr document, String^ filename)
{
  bool result = FALSE;

  FPDF_FILEWRITE* pSaveData = new FPDF_FILEWRITE();

  fpStream = File::Create(filename);

  if(fpStream != nullptr)
  {
    try
    {
      fpWriter = gcnew BinaryWriter(fpStream);

      pSaveData->WriteBlock = WriteBlock;
      result = FPDF_SaveAsCopy((FPDF_DOCUMENT)document, pSaveData, 0) ? true : false;
    }
    finally
    {
      delete (IDisposable^)fpStream;
      fpStream = nullptr;
    }
  }

  return result;
}


void Pdfium::CloseDocument(IntPtr document)
{
  FPDF_CloseDocument((FPDF_DOCUMENT)document);
}


IntPtr Pdfium::LoadPage(IntPtr document, int pageIndex)
{
  return (IntPtr)FPDF_LoadPage((FPDF_DOCUMENT)document, pageIndex);
}


IntPtr Pdfium::CreatePage(IntPtr document, int pageIndex, double width, double height)
{
  return (IntPtr)FPDFPage_New((FPDF_DOCUMENT)document, pageIndex, width, height);
}


void Pdfium::AddBitmapToPage(IntPtr document, IntPtr page, BitmapInfo *pInfo, TransformationMatrix *pMatrix)
{
  FPDF_PAGE pdfPage = (FPDF_PAGE)page;
  FPDF_PAGEOBJECT imageObject = FPDFPageObj_NewImgeObj((FPDF_DOCUMENT)document);

  // Add the bitmap to an image object and add the image object to the output page.
  FPDF_BITMAP bitmap = FPDFBitmap_CreateEx2(pInfo->Width, pInfo->Height, (FPDFPixel)pInfo->PixelFormat, (void*)pInfo->Data, pInfo->Stride);
  FPDFImageObj_SetBitmap(&pdfPage, 1, imageObject, bitmap);

  FPDFPageObj_Transform(imageObject, pMatrix->a, pMatrix->b, pMatrix->c, pMatrix->d, pMatrix->e, pMatrix->f);
  FPDFPage_InsertObject(pdfPage, imageObject);
  FPDFPage_GenerateContent(pdfPage);

  FPDFBitmap_Destroy(bitmap);
}


void Pdfium::AddJpegImageToPage(IntPtr document, IntPtr page, JpegInfo *pInfo, TransformationMatrix *pMatrix)
{
	FPDF_PAGE pdfPage = (FPDF_PAGE)page;
	FPDF_PAGEOBJECT imageObject = FPDFPageObj_NewImgeObj((FPDF_DOCUMENT)document);

	// Add the bitmap to an image object and add the image object to the output page.
	FPDFImageObj_SetJpeg(&pdfPage, 1, imageObject, (void*)pInfo->Data, pInfo->Size);
  
  FPDFPageObj_Transform(imageObject, pMatrix->a, pMatrix->b, pMatrix->c, pMatrix->d, pMatrix->e, pMatrix->f);
  FPDFPage_InsertObject(pdfPage, imageObject);
	FPDFPage_GenerateContent(pdfPage);
}


void Pdfium::ClosePage(IntPtr page)
{
  FPDF_ClosePage((FPDF_PAGE)page);
}


int Pdfium::GetPageCount(IntPtr document)
{
  return FPDF_GetPageCount((FPDF_DOCUMENT)document);
}


double Pdfium::GetPageWidth(IntPtr page)
{
  return FPDF_GetPageWidth((FPDF_PAGE)page);
}


double Pdfium::GetPageHeight(IntPtr page)
{
  return FPDF_GetPageHeight((FPDF_PAGE)page);
}


int Pdfium::GetNumObjectsInPage(IntPtr page)
{
  return FPDFPage_CountObject((FPDF_PAGE)page);
}


IntPtr Pdfium::GetObjectFromPage(IntPtr page, int index)
{
  return (IntPtr)FPDFPage_GetObject((FPDF_PAGE)page, index);
}


bool Pdfium::GetImageFromPageObject(IntPtr pageObject, PdfImageInfo *pInfo)
{
  bool result = false;
  FPDF_PAGEOBJECT po = (FPDF_PAGEOBJECT)pageObject;
  FPDF_IMAGEINFO imageInfo;

  if (FPDFPageObject_GetImageInfo(po, &imageInfo))
  {
    pInfo->Height = imageInfo.Height;
    pInfo->Width = imageInfo.Width;
    pInfo->BitsPerComponent = imageInfo.BitsPerComponent;
    pInfo->ColorSpace = (ColorSpaceEnum)imageInfo.ColorSpace;
    pInfo->Filter = (DecoderEnum)imageInfo.Filter;
    pInfo->DataSize = imageInfo.DataSize;
    pInfo->pData = imageInfo.pData;
    pInfo->Matrix.a = imageInfo.Matrix.a;
    pInfo->Matrix.b = imageInfo.Matrix.b;
    pInfo->Matrix.c = imageInfo.Matrix.c;
    pInfo->Matrix.d = imageInfo.Matrix.d;
    pInfo->Matrix.e = imageInfo.Matrix.e;
    pInfo->Matrix.f = imageInfo.Matrix.f;
    result = true;
  }

  return result;
}


bool Pdfium::CopyPage(IntPtr destDoc, IntPtr sourceDoc, int pageNumber, int insertIndex)
{
  char pageNum[10];
  sprintf_s(pageNum, "%d", pageNumber);
  return FPDF_ImportPages((FPDF_DOCUMENT)destDoc, (FPDF_DOCUMENT)sourceDoc, pageNum, insertIndex) ? true : false;
}


void Pdfium::RenderPageToBitmap(IntPtr page, BitmapInfo *pInfo)
{
  FPDF_BITMAP handle = FPDFBitmap_CreateEx2(pInfo->Width, pInfo->Height, (FPDFPixel)pInfo->PixelFormat, (void*)pInfo->Data, pInfo->Stride);

  try
  {
    FPDF_RenderPageBitmap(handle, (FPDF_PAGE)page, 0, 0, pInfo->Width, pInfo->Height, 0, 0);
  }
  finally
  {
    FPDFBitmap_Destroy(handle);
  }
}