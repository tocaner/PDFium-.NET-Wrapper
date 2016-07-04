// Pdfium.h

#pragma once
#include "fpdfview.h"


using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::IO;


namespace PdfiumDotNetWrapper 
{
  public enum class ColorSpaceEnum : int 
  { 
    Unknown = 0, 
    Gray, 
    RGB, 
    CMYK 
  };


  public enum class DecoderEnum : int
  {
    Unknown = 0,
    CCITTFaxDecode,
    DCTDecode,
    FlateDecode,
    JPXDecode,
    JBIG2Decode,
    RunLengthDecode
  };


  // Redefining DIB formats in fpdfview.h
  public enum class PixelFormatEnum : int
  {
    Invalid = 0,
    // 1 pixel indexed
    Bits1_Indexed = FPDFPixel_1bppMask,
    // 1 pixel Black and while
    Bits1_BW = FPDFPixel_1bppRgb,
    // 1 byte indexed
    Bits8_Indexed = FPDFPixel_8bppMask,
    // 1 byte Gray
    Bits8_Gray = FPDFPixel_8bppRgb,
    // 3 bytes per pixel, byte order: blue, green, red.
    Bits24_Rgb = FPDFPixel_Rgb,
    // 4 bytes per pixel, byte order: blue, green, red, unused.
    Bits32_Rgb = FPDFPixel_Rgb32,
    // 4 bytes per pixel, byte order: blue, green, red, alpha.
    Bits32_Argb = FPDFPixel_Argb,
  };


  public value struct PdfImageInfo
  {
    int Height;
    int Width;
    int BitsPerComponent;
    ColorSpaceEnum ColorSpace;
    DecoderEnum Filter;
    long DataSize;
    const unsigned char *pData;
  };


  public value struct JpegInfo
  {
    int Height;
    int Width;
    IntPtr Data;
    int Size;
  };


  public value struct BitmapInfo
  {
    int Height;
    int Width;
    PixelFormatEnum PixelFormat;
    IntPtr Data;
    int Stride;
  };


  public value struct TransformationMatrix  // width, 0, 0, height, x-shift, y-shift
  {
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
  };


  public ref class Pdfium
	{
		// TODO: Add your methods for this class here.
  public:
    static void InitLibrary();

    static void CloseLibrary();

    static IntPtr LoadDocument(FPDF_BYTESTRING fileName, FPDF_BYTESTRING password);

    static IntPtr LoadMemDocument(byte data_buf[], int size, FPDF_BYTESTRING password);

    static IntPtr CreateNewDocument();

    static bool SaveDocument(IntPtr document, String^ filename);

    static void CloseDocument(IntPtr document);

    static IntPtr LoadPage(IntPtr document, int pageNumber);

    static IntPtr CreatePage(IntPtr document, int pageIndex, double width, double height);

    static void AddBitmapToPage(IntPtr document, IntPtr page, BitmapInfo *pInfo, TransformationMatrix *pMatrix);

	  static void AddJpegImageToPage(IntPtr document, IntPtr page, JpegInfo *pInfo, TransformationMatrix *pMatrix);

	  static void ClosePage(IntPtr page);

    static int GetPageCount(IntPtr document);

    static double GetPageWidth(IntPtr page);

    static double GetPageHeight(IntPtr page);

    static int GetNumObjectsInPage(IntPtr page);

    static IntPtr GetObjectFromPage(IntPtr page, int index);

    static bool GetImageFromPageObject(IntPtr pageObject, PdfImageInfo *pInfo);

    static bool CopyPage(IntPtr destDoc, IntPtr sourceDoc, int pageNumber, int insertIndex);
    
    static void RenderPageToBitmap(IntPtr page, BitmapInfo *pInfo);
    
    static FileStream^ fpStream;

    static BinaryWriter^ fpWriter;
  };
}
