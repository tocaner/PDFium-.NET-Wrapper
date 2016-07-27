# Build Notes

Environment and Tools
---------------------
Install Visual Studio Community 2015
On the custom install options select: 
- Programming Languages\Visual C++\Common Tools for Visual C++ 2015

For Windows 10
Install Windows Software Development Kit (SDK) for Windows 10
https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk

Install depot tools for PDFium build
Follow instructions at: https://pdfium.googlesource.com/pdfium/


Description
-----------
Building PDFium code generates several static libraries (*.lib) that cannot be used directly in .NET applications. This project written in Visual C++ will import these static libraries and generate a .NET DLL that can be added to a .NET project as a Reference. 

We are using PDFium code by checking it out from GIT and adding additional source files to the PDFium project. This code should be built first, following the instructions: ~\Source\External\Pdfium\Instructions.txt

After PDFium build is successful open the PdfiumDotNetWrapper.sln solution file. Build PdfiumDotNetWrapper project with the same configuration (Debug/Release, x64/x86) used for the PDFium libraries in the previous step.

If the build is successful the result will be: ~\Output\Bin\$(Configuration)\PdfiumDotNetWrapper.dll


 


