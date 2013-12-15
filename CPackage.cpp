#include "CPackage.h"
#include <string.h>

CPackage::CPackage()
{
  m_srcfile.open(PUREPACKAGE,std::ifstream::binary);
  m_srcfile.seekg(std::ifstream::end);
  m_databegpos=m_srcfile.tellg();
  m_databegpos=0;
  m_default_dk_property=DP_NONE;
  m_default_fk_prorerty=FP_NONE;
  m_default_fk_compresstype=FCT_NONE;

}

CPackage::CPackage(std::string package_name)
{
  m_srcfile.open(PUREPACKAGE,std::ifstream::binary);
  m_srcfile.seekg(std::ifstream::end);
  m_databegpos=m_srcfile.tellg();
  m_totaldatalen=0;
  m_package.open(package_name.c_str());
  m_default_dk_property=DP_NONE;
  m_default_fk_prorerty=FP_NONE;
  m_default_fk_compresstype=FCT_NONE;
  
}
CPackage::~CPackage()
{
  m_package.close();

}
CX_INT32 CPackage::CX_CreatePackage(std::string package_name)
{
  m_package.open(package_name.c_str());

  return CX_OK;
}
CX_INT32 CPackage::CX_CreateTopDirectory(std::string dirname)
{
  CX_Dir_File::iterator ret=m_dir_file.find(dirname);
  if(ret!=m_dir_file.end())
    return CX_DIRALREADYEXIST;
  //在目录映射中增加一个目录，并且设置目录为Top目录
  m_dir_file.insert(CX_Dir_Pair(dirname,CX_File()));
  m_dir_file[dirname].insert(CX_File_Pair(".",std::vector<std::string>()));
  CX_UINT32 tmp=TYPE_DIR;
  std::string tmp_str;
  tmp_str.assign((char*)&tmp,sizeof(CX_UINT32));
  m_dir_file[dirname]["."].push_back(tmp_str);  //VALUE_TYPE
  tmp=DT_YES;
  tmp_str.assign((char*)&tmp,sizeof(CX_UINT32));//TopDirectory or not
  m_dir_file[dirname]["."].push_back(tmp_str);
  tmp_str.assign((char*)&m_default_dk_property,sizeof(CX_UINT32));  
  m_dir_file[dirname]["."].push_back(tmp_str);  //DK_PROPERTY

					   
  return CX_OK;

}
CX_INT32 CPackage::CX_CreateSubDirectory(std::string dirname,std::string subdir)
{
  
  CX_Dir_File::iterator ret=m_dir_file.find(dirname+subdir);
  if(ret!=m_dir_file.end())
    return CX_FILEALREADYEXIST;
  
  //给父目录增加一个类型为TYPE_DIR文件，让父目录知道增加了一个子目录
  CX_UINT32 tmp=TYPE_DIR;
  std::string tmp_str;
  tmp_str.assign((char*)&tmp,sizeof(CX_UINT32));
  m_dir_file[dirname][subdir].push_back(tmp_str);  //VALUE_TYPE
  tmp=DT_YES;
  tmp_str.assign((char*)&tmp,sizeof(CX_UINT32));
  m_dir_file[dirname][subdir].push_back(tmp_str);  //TopDirectory or not
  tmp_str.assign((char*)&m_default_dk_property,sizeof(CX_UINT32));  
  m_dir_file[dirname][subdir].push_back(tmp_str);  //DK_PROPERTY
  std::string fullsubdir=dirname+subdir;
  //在目录映射中增加一个新目录，并且标记为子目录
  m_dir_file.insert(CX_Dir_Pair(fullsubdir,CX_File()));
  m_dir_file[fullsubdir].insert(CX_File_Pair(".",std::vector<std::string>()));
  tmp=TYPE_DIR;
  tmp_str.assign((char*)&tmp,sizeof(CX_UINT32));  
  m_dir_file[fullsubdir]["."].push_back(tmp_str);  //VALUE_TYPE
  tmp=DT_NO;
  tmp_str.assign((char*)&tmp,sizeof(CX_UINT32));
  m_dir_file[fullsubdir]["."].push_back(tmp_str);
  tmp_str.assign((char*)&m_default_dk_property,sizeof(CX_UINT32));  
  m_dir_file[fullsubdir]["."].push_back(tmp_str);  //DK_PROPERTY

  return CX_OK;
}

CX_INT32 CPackage::CX_CreateFile(std::string dirname,std::string filename)
{

  CX_File::iterator file_ret=m_dir_file[dirname].find(filename);
  if(file_ret!=m_dir_file[dirname].end())
    return CX_FILEALREADYEXIST;

  m_dir_file[dirname].insert(CX_File_Pair(filename,std::vector<std::string>()));
  CX_UINT32 tmp=TYPE_FILE;
  std::string tmp_str;
  tmp_str.assign((char*)&tmp,sizeof(CX_UINT32));
  m_dir_file[dirname][filename].push_back(tmp_str);  //VALUE_TYPE
  m_dir_file[dirname][filename].push_back(filename);  //FK_SRCNAME
  tmp=0;
  tmp_str.assign((char*)&m_default_fk_prorerty,sizeof(CX_UINT32));
  m_dir_file[dirname][filename].push_back(tmp_str); //FK_PROPERTY
  tmp_str.assign((char*)&m_default_fk_compresstype,sizeof(CX_UINT32));
  m_dir_file[dirname][filename].push_back(tmp_str); //FK_COMPRESSTYPE
  return CX_OK;


}

CX_INT32 CPackage::CX_SetDirectoryProperty(std::string dirname,CX_UINT32 value)
{

  m_dir_file[dirname]["."][DK_PROPERTY].assign((char*)&value,sizeof(CX_UINT32));
  return CX_OK;
  
}


CX_INT32 CPackage::CX_SetFileInputFileName(std::string dirname,std::string filename,std::string inputfilename)
{
  m_dir_file[dirname][filename][FK_SRCNAME].assign(inputfilename.c_str(),inputfilename.size());
  return CX_OK;
}

CX_INT32 CPackage::CX_SetFileProperty(std::string dirname,std::string filename,CX_UINT32 value)
{

  m_dir_file[dirname][filename][FK_PROPERTY].assign((char*)&value,sizeof(CX_UINT32));
  return CX_OK;

}

CX_INT32 CPackage::CX_SetFileCompressType(std::string dirname,std::string filename,CX_UINT32 value)
{
  m_dir_file[dirname][filename][FK_COMPRESSTYPE].assign((char*)&value,sizeof(CX_UINT32));
  return CX_OK;

}


CX_INT32 CPackage::CX_GetDirectoryProperty(std::string dirname,CX_UINT32* value)
{

  memcpy(value,m_dir_file[dirname]["."][DK_PROPERTY].c_str(),sizeof(CX_UINT32));
  return CX_OK;
}

CX_INT32 CPackage::CX_GetFileInputFileName(std::string dirname, std::string filename,std::string& inputfilename)
{

  inputfilename.assign(m_dir_file[dirname][filename][FK_SRCNAME].c_str(),m_dir_file[dirname][filename][FK_SRCNAME].size());
  return CX_OK;
}

CX_INT32 CPackage::CX_GetFileProperty(std::string dirname,std::string filename,CX_UINT32* value)
{
  memcpy(value,m_dir_file[dirname][filename][FK_PROPERTY].c_str(),sizeof(CX_UINT32));
  return CX_OK;
}

CX_INT32 CPackage::CX_GetFileCompressType(std::string dirname,std::string filename,CX_UINT32* value)
{
  memcpy(value,m_dir_file[dirname][filename][FK_COMPRESSTYPE].c_str(),sizeof(CX_UINT32));
  return CX_OK;
}


std::vector<std::string> CPackage::CX_GetAllTopDirectoryName()
{
  std::vector<std::string> ret;
  CX_Dir_File::iterator it;
  CX_UINT32 dirtype;
  for(it=m_dir_file.begin();it!=m_dir_file.end();it++)
    {
      memcpy(&dirtype,m_dir_file[it->first]["."][DK_TOPDIR].c_str(),sizeof(CX_UINT32));
      if(dirtype==DT_YES)
	ret.push_back(it->first);
    }
  return ret;
}

std::vector<std::string> CPackage::CX_GetAllFileInDirectoryName(std::string dirname)
{
  std::vector<std::string> ret;
  CX_File files=m_dir_file[dirname];
  CX_File::iterator it;
  CX_UINT32 valuetype;
  for(it=files.begin();it!=files.end();it++)
    {
      memcpy(&valuetype,it->second[VALUE_TYPE].c_str(),sizeof(CX_UINT32));
      if(valuetype==TYPE_DIR)
	ret.push_back(std::string("Directory--")+it->first);
      else
	ret.push_back(std::string("File---")+it->first);
    }
  return ret;
}



















