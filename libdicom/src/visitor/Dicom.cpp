// STL's stream
#include <vector>
#include <string>
#include <ostream>

// VIRTUALS basic type
#include "Virtuals/Type.h"

//      TAG     //
//////////////////
#include "tag/DicomTag.h"
#include "tag/PixelData.h"
#include "tag/Modality.h"
// IO
#include "io/DicomFile.h"
#include "io/SimpleDirectory.h"
// Factory
#include "DicomAbstractFactory.h"
// Base file
#include "Virtuals/mfo/mfo.h"
////
#include "visitor/Dicom.h"

#ifdef _DEBUG
uint32 dicom::visitor::CDicom::s_ui32GroupOrder = 0 ;
#endif

namespace dicom
{

namespace visitor
{

////////////////////////
CDicom::CDicom( void ) :
	m_pTag				(NULL),
	m_bPicturePresent	(false),
	m_sFullPath			(""),
    m_pFile				(NULL),
    m_vectorPatient		(std::vector<mfo::CPatient*>(0))
{
	m_vectorPatient.reserve(10);
}

/////////////////
CDicom::~CDicom()
{
    for(
         std::vector<mfo::CPatient*>::reverse_iterator iterDelete = m_vectorPatient.rbegin();
         iterDelete != m_vectorPatient.rend();
         iterDelete++
       )
    {
		assert(*iterDelete != NULL);
        delete *iterDelete;
        *iterDelete = NULL;
		assert(*iterDelete == NULL);
    }
	this->m_vectorPatient.clear();
	assert(this->m_vectorPatient.size() == 0);
}

////////////////////////////////////////////////////////////////////////
const uint32 CDicom::VisitUnknownDicomTag( dicom::tag::CDicomTag & _Tag )
{
	const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
#ifdef _DEBUG
	this->VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////
#ifdef _DEBUG
void CDicom::VisitDicomTag( dicom::tag::CDicomTag & _Tag )
{
#if defined(_DEBUG_CONSOLE_VERBOSE) && defined(_CONSOLE)
    std::cout << "(" <<_Tag.GetGroup() << "," << _Tag.GetElement() << ") "
              << _Tag.GetName() << " : " << _Tag.GetValue() << std::endl;
#endif
}
#endif

//////////////////////////////////////////////////////
void CDicom::ParseFile( dicom::io::CDicomFile & _file )
{
#ifdef _DEBUG
	dicom::visitor::CDicom::s_ui32GroupOrder = 0 ;
#endif
    bool bFirstOne = true;
	uint32 ui32Pos = 0 ;

	// 1 tag min = i32Group+i32Element+i32ElementLength+(1 * int8)
	uint32 ui32NbTagMax = _file.GetSize() / (sizeof(int32)*3+sizeof(int8)) ; 
	uint32 ui32NbTag = 0;

    while ( (ui32Pos < _file.GetSize()) && (ui32NbTag <= ui32NbTagMax) )
    {
        int32 i32Group			= 0x00 ;
		int32 i32Element		= 0x00 ;
		int32 i32ElementLength	= 0x00 ;

        // Read the next tag's header !
		// this function can throw a dicom::tag::CDicomTag::CTagException.
		this->ReadHeaderOfMyTag(_file, i32Group, i32Element, i32ElementLength, bFirstOne);

        // Create the tag with the dicom factory
        dicom::tag::CDicomTag *pTag = 
					dicom::CDicomAbstractFactory::CreateTag( i32Group, i32Element,
															i32ElementLength, _file, *this);
		try
		{
			ui32Pos = pTag->Visit(this); // Visit is a virtual function
	        delete pTag;
		}
		catch(...)
		{
            delete pTag;
            break;
		}
		ui32NbTag++;
    }

	if	( (_file.GetSize() != ui32Pos) ||	(ui32NbTag > ui32NbTagMax) )
	{
		throw dicom::io::CDicomFile::CFileException(_file.GetFullName(),
							dicom::exception::StringException::FormatNotDefined);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void CDicom::ReadHeaderOfMyTag(	dicom::io::CDicomFile & _file, int32 & _i32Group, 
						int32 & _i32Element, int32 & _i32ElementLength, bool & _bFirstOne)
{
	try
	{
		_file.ReadTag(_i32Group, _i32Element, _i32ElementLength, _bFirstOne);
	}
	catch ( ... )
	{
		// Read the header
		try
		{
			if (_bFirstOne)
			{
				_bFirstOne = false;
				_file.SetOffset(0);
				_file.ReadTag(_i32Group, _i32Element, _i32ElementLength, _bFirstOne);
			}
			else
			{
				throw;
			}
		}
		catch ( ... )
		{
			throw dicom::io::CDicomFile::CFileException(_file.GetFullName(),
								dicom::exception::StringException::FormatNotDefined);
		}
	}
}

///////////////////////////////////////////////////////////////////////
void CDicom::ParseDirectory( dicom::io::CSimpleDirectory & _Directory )
{
	std::ostringstream ost;

	try
	{
		_Directory.LoadDirectory();
	}
    catch(const dicom::exception::CBase & e)   
    {// parse error
        ost << e.what() << std::endl;
    }
    catch(const std::exception & e)           
    {// another exception
        ost << e.what() << std::endl;
    }
    catch(...)								  
    {// another exception
        ost << "unknown exception in this directory" << std::endl;
    }

    if (_Directory.IsNotEmpty())
    {
        dicom::io::CDicomFile* tmpFile = _Directory.GetLastFile();

        while(_Directory.IsNotEmpty() && tmpFile!=NULL)
        {
			assert(tmpFile != NULL);

            try
            {
                tmpFile->LoadInputFile();
#if defined(_DEBUG) && defined(_CONSOLE)
            	std::cout	<< _Directory.GetSize() << " - File '"
							<< tmpFile->GetFullName() << "' is parsing...";
#endif
                tmpFile->Visit(this);
#if defined(_DEBUG) && defined(_CONSOLE)
                std::cout <<"Done"<< std::endl ;
#endif
            }
            catch(const dicom::exception::CBase & e)   
            {// parse error
                ost << e.what() << std::endl;
            }
            catch(const std::exception & e)           
            {// another exception
                ost << tmpFile->GetFullName() << " : " << e.what() << std::endl;
            }
            catch(...)								  
            {// another exception
                ost << tmpFile->GetFullName() << " : " << "unknown exception in file of this directory" << std::endl;
            }
            tmpFile = _Directory.RemoveLastFile();
        }
    }

	if (ost.str().size() > 1)
	{
		throw dicom::exception::CBase(ost.str());
	}

#if defined(_DEBUG) && defined(_CONSOLE)
    std::cout << "Work finish ..." << std::endl;
#endif
}

/////////////////////////////////////////////////
const bool CDicom::IsPicturePresent( void ) const
{
    return this->m_bPicturePresent;
}

/////////////////////////////////////////////////////////////
void CDicom::SetPicturePresent( const bool _bPicturePresent )
{
    this->m_bPicturePresent = _bPicturePresent;
}

//////////////////////////////////////////////////////////////
std::vector<mfo::CPatient*> * CDicom::GetVectorPatient( void )
{
    return &(this->m_vectorPatient);
}

std::ostream& operator<<(std::ostream & _os, dicom::visitor::CDicom& _visitor)
{
    for(	std::vector<mfo::CPatient*>::iterator iListPatient =
			 _visitor.m_vectorPatient.begin(); // Search a patient
			iListPatient < _visitor.m_vectorPatient.end();
			iListPatient++
        )
    {
			_os << (**iListPatient);
    }
    return _os;
}

}// end namespace visitor

}// end namespace
