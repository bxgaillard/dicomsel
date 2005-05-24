// Exception.h
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

namespace dicom
{
    namespace exception
    {
		class StringException
		{
			static const std::string FormatNotDefined;
			static const std::string FileNotFound ;
			static const std::string ModalityNotManaged ;
			static const std::string NoPictureFound ;
			static const std::string SizeNotEqual ;
		};
        /*
		static const char * const FormatNotDefined="Format DICOM inconnu ou fichier DICOM incorrect" ;
        static const char * const FileNotFound="File specification that could not be matched or Invalid filename specification (perhaps not read permissions)" ;
        static const char * const ModalityNotManaged="This modality is not managed at this time" ;
        static const char * const NoPictureFound="This file have not picture" ;
        static const char * const SizeNotEqual="Size not egual in all files" ;
		*/
    }

}// end namespace
#endif // EXCEPTION_H
