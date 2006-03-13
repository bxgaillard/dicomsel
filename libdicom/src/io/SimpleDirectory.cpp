#include <iostream>
#include <algorithm>
#include <cctype>
#include <sys/stat.h>

// Assertions
#include <cassert>

#ifndef _WIN32            //assuming this means the os is unix
#include <unistd.h>
#include <dirent.h>
#else
#include <io.h>
#endif

#include "Virtuals/Type.h"

#include "io/SimpleFile.h"
#include "io/SimpleDirectory.h"

namespace dicom
{

namespace io
{

//////////////////////////////////////////////////////////////////////
CSimpleDirectory::CSimpleDirectory(const std::string& _stringFullName):
    m_vectorFile(std::vector<dicom::io::CDicomFile*>(0))
{
	m_vectorFile.reserve(1000);

	assert(_stringFullName.size() > 2);
    const char final = _stringFullName[_stringFullName.size()-1];
    this->m_stringFullName = (final == '\\' && _stringFullName.size() != 3)?
		_stringFullName.substr(0, _stringFullName.size()-1):
		_stringFullName;

    this->VerifyCreatePath(this->m_stringFullName);
	assert(this->m_stringFullName.size() > 2);
}

/////////////////////////////////////
CSimpleDirectory::~CSimpleDirectory()
{
    std::vector<dicom::io::CDicomFile*>::iterator iterFile = this->m_vectorFile.begin();
    while(iterFile<this->m_vectorFile.end())
    {
        dicom::io::CDicomFile* tempFile = (*iterFile);
        iterFile++;
        if (tempFile!=NULL) delete tempFile;
        tempFile = NULL;
        assert(!tempFile);
    }
    this->m_vectorFile.clear();
}

////////////////////////////////////////////
void CSimpleDirectory::LoadDirectory( void )
{
	std::string stringError = "";

	if (GetSize() > 0)
		return;

#ifdef _WIN32
    long hFile = 0;
    struct _finddata_t fileInfo;

    std::string sTemp = (this->m_stringFullName + std::string("/**"));

    if( (hFile = _findfirst(sTemp.c_str(), &fileInfo)) == -1L)
	{
		stringError += (sTemp + " : ") + dicom::exception::StringException::FileNotFound;
	}
	else
	{
		this->addFile(this->m_stringFullName, "", std::string(fileInfo.name));
	}

    while(_findnext(hFile, &fileInfo) == 0)
    {
        this->addFile(this->m_stringFullName, "", std::string(fileInfo.name));
    }

    _findclose(hFile);

#else
    DIR* dirp;
    struct dirent* dp;

    dirp = opendir(this->m_stringFullName.c_str());
    if (dirp == 0)
	{
		stringError += dicom::exception::StringException::FileNotFound;
	}

    while ((dp = readdir(dirp)) != NULL)
    {
        addFile(this->m_stringFullName, "", std::string(dp->d_name));
    }
    closedir(dirp);
#endif

	if (stringError.length() != 0)
	{
        throw CSimpleFile::CFileException(this->m_stringFullName, stringError);
	}
}

//////////////////////////////////////////////////////////////////////
void CSimpleDirectory::LoadDirectory( const std::string _stringPath )
{
	std::string stringError = "";

#ifdef _WIN32
    long hFile = 0;
    struct _finddata_t fileInfo;

    std::string sTemp = (_stringPath + std::string("/**"));

    if( (hFile = _findfirst(sTemp.c_str(), &fileInfo)) == -1L)
	{
		stringError += (sTemp + " : ")+ dicom::exception::StringException::FileNotFound;
	}
	else
	{
	    this->addFile(this->m_stringFullName,
				_stringPath.substr(this->m_stringFullName.length()+1, _stringPath.length()),
				std::string(fileInfo.name));
	}

    while(_findnext(hFile, &fileInfo) == 0)
	{
        this->addFile(this->m_stringFullName,
				_stringPath.substr(this->m_stringFullName.length()+1,
				_stringPath.length()), std::string(fileInfo.name));
	}
    _findclose(hFile);

#else
    DIR* dirp;
    struct dirent* dp;

    dirp = opendir(_stringPath.c_str());
    if (dirp == 0)
	{
		stringError += dicom::exception::StringException::FileNotFound;
	}

    while ((dp = readdir(dirp)) != NULL)
    {
	this->addFile(_stringPath, "", std::string(dp->d_name));
    }
    closedir(dirp);
#endif

	if (stringError.length() != 0)
	{
        throw CSimpleFile::CFileException(this->m_stringFullName, stringError);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
const std::string CSimpleDirectory::addFile(	const std::string _stringFolder,
												const std::string _stringRelatifName,
												const std::string _stringName )
{
    assert(_stringFolder.size() >= 3);
    assert(_stringName.size() >= 1);

	std::string sString = _stringFolder + std::string("/");
	if (_stringRelatifName != "")
		sString += _stringRelatifName + std::string("/");
	sString += _stringName;

#ifdef _WIN32
    struct _stat buf;
    int err = _stat(sString.c_str(), &buf);
#else
    struct stat buf;
    int err = stat(sString.c_str(), &buf);
#endif
    if (!err)
    {
#ifdef _WIN32
        int flag = ((buf.st_mode & _S_IFDIR) != 0);
#else
	// NOTE: this isn't correct: int flag = ((buf.st_mode & S_IFDIR) != 0);
	int flag = (S_ISDIR(buf.st_mode) != 0);
#endif
        if(flag)
        {
            if(
                // This test cannot create a fatal error,
                // beacause the minus path contains the
                // drive letter with double dot, slash and
                // a alpha-num for the file (eg :  d:\a )
                // The minimun size is 4 !
               !(
                 sString.c_str()[sString.size()-2] == '/' &&
                 sString.c_str()[sString.size()-1] == '.'
                ) &&
               !(
                 sString.c_str()[sString.size()-3] == '/' &&
                 sString.c_str()[sString.size()-2] == '.' &&
                 sString.c_str()[sString.size()-1] == '.'
                )
              )
			{
				try
				{
					this->LoadDirectory(sString);
				}
				catch(const std::exception & e)
				{
					return e.what();
				}
			}
#ifdef _DEBUG_STREAM
	std::ofstream ofsDicomDirLog("DicomDirLog.txt", std::ios_base::out | std::ios_base::app );
	ofsDicomDirLog << "Load \"" << sString << "\" directory\n" << std::endl;
#endif
        }
        else
        {
	    std::string original = sString;

            try
            {
				std::transform (
							sString.begin(), sString.end(),
							sString.begin(),
							tolower
						  );
                if(
                   !(// text file
                        sString[sString.size()-4] == '.' &&
						(
							(
							sString[sString.size()-3] == 't' &&
							sString[sString.size()-2] == 'x' &&
							sString[sString.size()-1] == 't'
							)||
							(
							sString[sString.size()-3] == 'z' &&
							sString[sString.size()-2] == 'i' &&
							sString[sString.size()-1] == 'p'
							)||
							(
							sString[sString.size()-3] == 'h' &&
							sString[sString.size()-2] == 't' &&
							sString[sString.size()-1] == 'm'
							)||
							(
							sString[sString.size()-3] == 'x' &&
							sString[sString.size()-2] == 'm' &&
							sString[sString.size()-1] == 'l'
							)||
							(
							sString[sString.size()-3] == 'e' &&
							sString[sString.size()-2] == 'x' &&
							sString[sString.size()-1] == 'e'
							)
						)
                    ) &&
                   !(// compress (zip) file
                        sString[sString.size()-5] == '.' &&
                        sString[sString.size()-4] == 'h' &&
                        sString[sString.size()-3] == 't' &&
                        sString[sString.size()-2] == 'm' &&
                        sString[sString.size()-1] == 'l'
                    ) &&
                   !(// compress (gz) file
                        sString[sString.size()-3] == '.' &&
                        sString[sString.size()-2] == 'g' &&
                        sString[sString.size()-1] == 'z'
                    ) &&
                   !(// dicomdir file
                        sString[sString.size()-3] == 'd' &&
                        sString[sString.size()-2] == 'i' &&
                        sString[sString.size()-1] == 'r'
                    )&&
                   buf.st_size > 128
                  )
                {
                    m_vectorFile.push_back(new dicom::io::CDicomFile(original));
#ifdef _DEBUG_STREAM
					std::ofstream ofsDicomDirLog("DicomDirLog.txt", std::ios_base::out | std::ios_base::app );
					ofsDicomDirLog << "." ;
#endif
                }
                else
                {
                    // Do not make ...
                }
            }
            catch(std::exception e)
            {
                std::cerr << original << e.what() << std::endl;
            }
        }
    }
    else
    {
        return ( (this->m_stringFullName + " : ") + dicom::exception::StringException::FileNotFound);
    }

	return ("");
}

////////////////////////////////////////////////////////////////////////////////
void CSimpleDirectory::VerifyCreatePath( const std::string _stringPath ) const
{
#ifdef _MSC_VER
    struct _stat buf;
    int err = _stat(_stringPath.c_str(), &buf);
#else
    struct stat buf;
    int err = stat(_stringPath.c_str(), &buf);
#endif

    if (err == 0)
    {
#ifdef _MSC_VER
        int flag = ((buf.st_mode & _S_IFDIR) != 0);
#else
        int flag = (S_ISDIR(buf.st_mode));
#endif
        if (!flag)  throw CSimpleFile::CFileException(this->m_stringFullName,
                                                      dicom::exception::StringException::FileNotFound);
    }
    else
    {
        throw CSimpleFile::CFileException(this->m_stringFullName,
                                          dicom::exception::StringException::FileNotFound);
    }
}

//////////////////////////////////////////////////////////////
const std::string& CSimpleDirectory::GetFullName( void ) const
{
    return this->m_stringFullName;
}

////////////////////////////////////////////////////
const uint32 CSimpleDirectory::GetSize( void ) const
{
    return this->m_vectorFile.size();
}

////////////////////////////////////////////////////////////////////////////////////
void CSimpleDirectory::Visit( dicom::visitor::CAbstractDicomFile * const _pVisitor )
{
    assert(_pVisitor!=NULL);
    _pVisitor->VisitDicomDirectory(*this);
}

/////////////////////////////////////////////////////////////
dicom::io::CDicomFile* CSimpleDirectory::GetLastFile ( void )
{
    assert(this->m_vectorFile.size()!=0);
    return this->m_vectorFile.back();
}

///////////////////////////////////////////////////////////////
dicom::io::CDicomFile* CSimpleDirectory::RemoveLastFile ( void )
{
    assert(this->m_vectorFile.size()!=0);
    delete (this->m_vectorFile.back());
    m_vectorFile.erase(this->m_vectorFile.end()-1);
    return (this->m_vectorFile.size()!=0)?(this->m_vectorFile.back()):NULL;
}

//////////////////////////////////////////////////////
const bool CSimpleDirectory::IsNotEmpty ( void ) const
{
    return (this->m_vectorFile.size()!=0)?true:false;
}

} // end namespace io

} // end namespace dicom
