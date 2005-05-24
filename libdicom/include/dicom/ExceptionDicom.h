// ExceptionDicom.h
#ifndef EXCEPTIONDICOM_H
#define EXCEPTIONDICOM_H

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DICOM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DicomLib_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//#ifdef _MSC_VER
#ifdef DICOM_EXPORTS
#define DicomLib_API __declspec(dllexport)
#else
#define DicomLib_API __declspec(dllimport)
#endif
//#endif

#include <string>
/*#include <exception>
#include <iostream>
#include <fstream>
*/

//class DicomLib_API std::exception;

namespace dicom
{

	namespace exception
	{

		/**
		 * CDicom
		 *********************
		 * Namespace : dicom::exception
		 *********************
		 * Description:
		 *
		 *********************
		 */
		class StringException
		{
			public:
			static const std::string FormatNotDefined;
			static const std::string FileNotFound ;
			static const std::string ModalityNotManaged ;
			static const std::string NoPictureFound ;
			static const std::string SizeNotEqual ;
		};

		/**
		 * CBase
		 *********************
		 * Namespace : dicom::exception
		 *********************
		 * Description:
		 *
		 *********************
		 */
		class
//#ifdef _MSC_VER
//		DicomLib_API
//#endif
//#if defined( _MSC_VER )
//#pragma warning (disable : 4275)
//#endif
		CBase /*: public std::exception*/
		{
//#if defined( _MSC_VER )
//#pragma warning (disable : 4251)
//#endif
		private:
			std::string m_sString;
//#if defined( _MSC_VER )
//#pragma warning (default : 4251)
//#endif
		public:
			CBase(const std::string _stringID)
			{
				this->m_sString = _stringID;
#ifdef _DEBUG_STREAM
/*				std::ofstream ofsTxt;
				ofsTxt.open("e:\\data2\\logFileDicom.txt", std::ios_base::ate);
				ofsTxt << this->m_sString;
				ofsTxt.flush();
				ofsTxt.close();*/
#endif
			}

			//const char* what(void) const throw()
			const char *what() const
			{
				return this->m_sString.c_str();
			}

			virtual ~CBase()
			//virtual ~CBase() throw()
			{
			}
		};
//#if defined( _MSC_VER )
//#pragma warning (default : 4275)
//#endif

	}

}// end namespace
#endif // EXCEPTIONDICOM_H
