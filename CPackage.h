#ifndef CPACKAGE_H
#define CPACKAGE_H

#if defined(unix)
#define PUREPACKAGE "CXInstall"
#elif defined(_WIN32)
#define PUREPACKAGE "CXInstall.exe"
#else
#define PUREPACKAGE "CXInstall"
#endif

#include "CXinclude.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>



class CPackage
{
 public:
  typedef std::map<std::string,std::map<std::string,std::vector<std::string> > > CX_Dir_File;
  typedef std::map<std::string,std::vector<std::string> > CX_File;
  typedef std::vector<std::string> CX_File_Option;
  typedef std::pair<std::string,CX_File> CX_Dir_Pair;
  typedef std::pair<std::string,CX_File_Option> CX_File_Pair;
 public:
  CPackage();
  CPackage(std::string package_name);//安装程序的名字
  ~CPackage();
 public:
  CX_INT32 CX_CreatePackage(std::string package_name);
  CX_INT32 CX_CreateTopDirectory(std::string dirname);
  CX_INT32 CX_CreateSubDirectory(std::string dirname,std::string subdir);
  CX_INT32 CX_CreateFile(std::string dirname,std::string filename);

  CX_INT32 CX_SetDirectoryProperty(std::string dirname,CX_UINT32 value);
  CX_INT32 CX_SetFileInputFileName(std::string dirname,std::string filename,std::string inputfilename);
  CX_INT32 CX_SetFileProperty(std::string dirname,std::string filename,CX_UINT32 value);
  CX_INT32 CX_SetFileCompressType(std::string dirname,std::string filename,CX_UINT32 value);
  CX_INT32 CX_GetDirectoryProperty(std::string dirname,CX_UINT32* value);
  CX_INT32 CX_GetFileInputFileName(std::string dirname, std::string filename,std::string& intputfilename);
  CX_INT32 CX_GetFileProperty(std::string dirname,std::string filename,CX_UINT32* value);
  CX_INT32 CX_GetFileCompressType(std::string dirname,std::string filename,CX_UINT32* value);
  std::vector<std::string> CX_GetAllTopDirectoryName();

  std::vector<std::string> CX_GetAllFileInDirectoryName(std::string dirname);
 private:
  CX_Dir_File m_dir_file;
  std::ofstream m_package;
  std::ifstream m_srcfile;
  CX_UINT32 m_default_dk_property,m_default_fk_prorerty,m_default_fk_compresstype;
  CX_UINT64 m_databegpos,m_totaldatalen;
  
};



#endif
















