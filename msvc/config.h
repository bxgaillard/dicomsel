#ifndef DICOMSEL_CONFIG_H
#define DICOMSEL_CONFIG_H

#ifndef LITTLE_ENDIAN
# define LITTLE_ENDIAN 1234
#endif /* !LITTLE_ENDIAN */
#ifndef BIG_ENDIAN
# define BIG_ENDIAN    4321
#endif /* !BIG_ENDIAN */
#ifndef BYTE_ORDER
# define BYTE_ORDER    1234
#endif /* !BYTE_ORDER */

#define PACKAGE_AUTHORS         "Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz"
#define PACKAGE_AUTHORS_UNICODE "Benjamin Gaillard, Marc-Aur\u00E8le M\u00F6rk, Guillaume Spitz"
#define PACKAGE_BUGREPORT       "Johan.Moreau@ircad.u-strasbg.fr"
#define PACKAGE_NAME            "DicomSel"
#define PACKAGE_OWNER           "IRCAD"
#define PACKAGE_STRING          "DicomSel 1.9.3"
#define PACKAGE_TARNAME         "dicomsel"
#define PACKAGE_VERSION         "1.9.3"
#define PACKAGE_YEARS           "2005, 2006"

#endif /* !DICOMSEL_CONFIG_H */
