#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include <wx/msgdlg.h>
#include <wx/thread.h>


#include <dicomsel/ipc.h>
#include <dicomsel/DicomTree.h>
#include <dicomsel/BitmapPanel.h>
#include <dicomsel/TagSet.h>
#include <dicomsel/TagDialog.h>
#include <dicomsel/StoreDialog.h>
#include <dicomsel/Store.h>
#include <dicomsel/DicomFile.h>
#include <dicomsel/MainFrame.h>
#include <wx/dirdlg.h>
# include <wx/string.h>





#define INCLUDE_CSTDLIB
#define INCLUDE_CSTRING
#define INCLUDE_CSTDARG
#define INCLUDE_CCTYPE
#define INCLUDE_CSIGNAL
#include "dcmtk/ofstd/ofstdinc.h"

#ifdef _WIN32
#include <process.h>     /* needed for declaration of getpid() */
#endif

BEGIN_EXTERN_C
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>       /* needed on Solaris for O_RDONLY */
#endif
END_EXTERN_C

#ifdef HAVE_GUSI_H
#include <GUSI.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <direct.h>        /* for _mkdir() */
#endif


#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdebug.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/cmdlnarg.h>
#include <dcmtk/ofstd/ofconapp.h>
#include <dcmtk/ofstd/ofstd.h>
#include <dcmtk/ofstd/ofdatime.h>
#include <dcmtk/dcmdata/dcuid.h>         /* for dcmtk version name */
#include <dcmtk/dcmnet/dicom.h>         /* for DICOM_APPLICATION_ACCEPTOR */
#include <dcmtk/dcmdata/dcdeftag.h>      /* for DCM_StudyInstanceUID */
#include <dcmtk/dcmdata/dcostrmz.h>      /* for dcmZlibCompressionLevel */
#include <dcmtk/dcmnet/dcasccfg.h>      /* for class DcmAssociationConfiguration */
#include <dcmtk/dcmnet/dcasccff.h>      /* for class DcmAssociationConfigurationFile */

#ifdef WITH_OPENSSL
#include <dcmtk/dcmtls/tlstrans.h>
#include <dcmtk/dcmtls/tlslayer.h>
#endif

#ifdef WITH_ZLIB
#include <zlib.h>          /* for zlibVersion() */
#endif

#if defined(HAVE_MKTEMP) && !defined(HAVE_PROTOTYPE_MKTEMP)
extern "C" {
char * mktemp(char *);
}
#endif

// Solaris 2.5.1 has mkstemp() in libc.a but no prototype
#if defined(HAVE_MKSTEMP) && !defined(HAVE_PROTOTYPE_MKSTEMP)
extern "C" {
int mkstemp(char *);
}
#endif




#ifdef PRIVATE_STORESCP_DECLARATIONS
PRIVATE_STORESCP_DECLARATIONS
#else
#define OFFIS_CONSOLE_APPLICATION "storescp"
#endif

static char rcsid[] = "$dcmtk: " OFFIS_CONSOLE_APPLICATION " v" OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

#define APPLICATIONTITLE "STORESCP"     /* our application entity title */

#define PATH_PLACEHOLDER "#p"
#define FILENAME_PLACEHOLDER "#f"
#define CALLING_AETITLE_PLACEHOLDER "#a"
#define CALLED_AETITLE_PLACEHOLDER "#c"




//////////////// oK

namespace dicomsel
{
bool m_blnFinished;


int event=0;

static OFCondition processCommands(T_ASC_Association *assoc,MainFrame *frame);
static OFCondition acceptAssociation(T_ASC_Network *net,MainFrame *frame);
static OFCondition echoSCP(T_ASC_Association * assoc, T_DIMSE_Message * msg, T_ASC_PresentationContextID presID);
static OFCondition storeSCP(T_ASC_Association * assoc, T_DIMSE_Message * msg, T_ASC_PresentationContextID presID,MainFrame *frame);
static void executeOnReception();
static void executeEndOfStudyEvents();
static void executeOnEndOfStudy();
static void renameOnEndOfStudy();
static OFString replaceChars( const OFString &srcstr, const OFString &pattern, const OFString &substitute );
static void executeCommand( const OFString &cmd );
static void cleanChildren(pid_t pid, OFBool synch);
static OFCondition acceptUnknownContextsWithPreferredTransferSyntaxes(
         T_ASC_Parameters * params,
         const char* transferSyntaxes[],
         int transferSyntaxCount,
         T_ASC_SC_ROLE acceptedRole = ASC_SC_ROLE_DEFAULT);


OFBool             opt_uniqueFilenames = OFFalse;
OFString           opt_fileNameExtension;
OFBool             opt_timeNames = OFFalse;
int            timeNameCounter = -1; // "serial number" to differentiate between files with same timestamp
OFCmdUnsignedInt   opt_port = 0;
OFBool             opt_refuseAssociation = OFFalse;
OFBool             opt_rejectWithoutImplementationUID = OFFalse;
OFCmdUnsignedInt   opt_sleepAfter = 0;
OFCmdUnsignedInt   opt_sleepDuring = 0;
OFCmdUnsignedInt   opt_maxPDU = ASC_DEFAULTMAXPDU;
OFBool             opt_useMetaheader = OFTrue;
E_TransferSyntax   opt_networkTransferSyntax = EXS_Unknown;
E_TransferSyntax   opt_writeTransferSyntax = EXS_Unknown;
E_GrpLenEncoding   opt_groupLength = EGL_recalcGL;
E_EncodingType     opt_sequenceType = EET_ExplicitLength;
E_PaddingEncoding  opt_paddingType = EPD_withoutPadding;
OFCmdUnsignedInt   opt_filepad = 0;
OFCmdUnsignedInt   opt_itempad = 0;
OFCmdUnsignedInt   opt_compressionLevel = 0;
OFBool             opt_verbose = OFFalse;
OFBool             opt_debug = OFFalse;
OFBool             opt_bitPreserving = OFFalse;
OFBool             opt_ignore = OFFalse;
OFBool             opt_abortDuringStore = OFFalse;
OFBool             opt_abortAfterStore = OFFalse;
OFBool             opt_promiscuous = OFFalse;
OFBool             opt_correctUIDPadding = OFFalse;
OFBool             opt_inetd_mode = OFFalse;
OFString           callingaetitle;  // calling AE title will be stored here
OFString           calledaetitle;   // called AE title will be stored here
const char *       opt_respondingaetitle = APPLICATIONTITLE;
static OFBool      opt_secureConnection = OFFalse;    // default: no secure connection
static OFString    opt_outputDirectory("../reception");          // default: output directory equals "."
static const char *opt_sortConcerningStudies = NULL;  // default: no sorting
OFString           lastStudyInstanceUID;
OFString           subdirectoryPathAndName;
OFList<OFString>   outputFileNameArray;
static const char *opt_execOnReception = NULL;        // default: don't execute anything on reception
static const char *opt_execOnEndOfStudy = NULL;       // default: don't execute anything on end of study

OFString           lastStudySubdirectoryPathAndName;
static OFBool      opt_renameOnEndOfStudy = OFFalse;  // default: don't rename any files on end of study
static long        opt_endOfStudyTimeout = -1;        // default: no end of study timeout
static OFBool      endOfStudyThroughTimeoutEvent = OFFalse;
//static const char *opt_configFile = NULL;
//static const char *opt_profileName = NULL;
T_DIMSE_BlockingMode opt_blockMode = DIMSE_BLOCKING;
int                opt_dimse_timeout = 0;
int                opt_acse_timeout = 30;

#if defined(HAVE_FORK) || defined(_WIN32)
OFBool             opt_forkMode = OFFalse;
#endif

#ifdef _WIN32
OFBool             opt_forkedChild = OFFalse;
OFBool             opt_execSync = OFFalse;            // default: execute in background
#endif

#ifdef WITH_OPENSSL
static int         opt_keyFileFormat = SSL_FILETYPE_PEM;
static const char *opt_privateKeyFile = NULL;
static const char *opt_certificateFile = NULL;
static const char *opt_passwd = NULL;
#if OPENSSL_VERSION_NUMBER >= 0x0090700fL
static OFString    opt_ciphersuites(TLS1_TXT_RSA_WITH_AES_128_SHA ":" SSL3_TXT_RSA_DES_192_CBC3_SHA);
#else
static OFString    opt_ciphersuites(SSL3_TXT_RSA_DES_192_CBC3_SHA);
#endif
static const char *opt_readSeedFile = NULL;
static const char *opt_writeSeedFile = NULL;
static DcmCertificateVerification opt_certVerification = DCV_requireCertificate;
static const char *opt_dhparam = NULL;
#endif


#ifdef HAVE_WAITPID
/** signal handler for SIGCHLD signals that immediately cleans up
 *  terminated children.
 */
#ifdef SIGNAL_HANDLER_WITH_ELLIPSE
extern "C" void sigChildHandler(...)
#else
extern "C" void sigChildHandler(int)
#endif
{
  int status = 0;
  waitpid( -1, &status, WNOHANG );
  signal(SIGCHLD, sigChildHandler);
}
#endif


#define SHORTCOL 4
#define LONGCOL 21


///////////////// DEBUT FONCTION ////////////////////


int server(int argc, char *argv[],unsigned int port,MainFrame *frame)
{

opt_port = port;

//int debug=0;

  // network struct, contains DICOM upper layer FSM etc.
  T_ASC_Network *net;
  DcmAssociationConfiguration asccfg;

#ifdef HAVE_GUSI_H
  /* needed for Macintosh */
  GUSISetup(GUSIwithSIOUXSockets);
  GUSISetup(GUSIwithInternetSockets);
#endif

#ifdef HAVE_WINSOCK_H
  WSAData winSockData;
  /* we need at least version 1.1 */
  WORD winSockVersionNeeded = MAKEWORD( 1, 1 );
  WSAStartup(winSockVersionNeeded, &winSockData);
#endif

  //char tempstr[20];
  /*if(debug>1)*/OFConsoleApplication app(OFFIS_CONSOLE_APPLICATION , "Appel fonction DicomStore", rcsid);
  OFCommandLine cmd;


  /* evaluate command line */
  prepareCmdLineArgs(argc, argv, OFFIS_CONSOLE_APPLICATION);
  if (app.parseCommandLine(cmd, argc, argv, OFCommandLine::ExpandWildcards))
  {
    /* check exclusive options first */
	if (cmd.getParamCount() == 0) //no options
    	{
		
	}	

    /* command line parameters */

    if (cmd.getParamCount() == 1)
      app.checkParam(cmd.getParamAndCheckMinMax(1, opt_port/*int*/, 1, 65535));


      if (cmd.findOption("--acse-timeout"))
      {
        OFCmdSignedInt opt_timeout = 0;
        app.checkValue(cmd.getValueAndCheckMin(opt_timeout, 1));
        opt_acse_timeout = OFstatic_cast(int, opt_timeout);
      }

      if (cmd.findOption("--dimse-timeout"))
      {
        OFCmdSignedInt opt_timeout = 0;
        app.checkValue(cmd.getValueAndCheckMin(opt_timeout, 1));
        opt_dimse_timeout = OFstatic_cast(int, opt_timeout);
        opt_blockMode = DIMSE_NONBLOCKING;
      }


    //if (cmd.findOption("--verbose")) opt_verbose=OFTrue;

    if (cmd.findOption("--output-directory")) app.checkValue(cmd.getValue(opt_outputDirectory));

    cmd.beginOptionBlock();
    if (cmd.findOption("--prefer-uncompr"))      opt_networkTransferSyntax = EXS_Unknown;
    if (cmd.findOption("--prefer-little"))       opt_networkTransferSyntax = EXS_LittleEndianExplicit;
    if (cmd.findOption("--prefer-big"))          opt_networkTransferSyntax = EXS_BigEndianExplicit;
    if (cmd.findOption("--prefer-lossless"))     opt_networkTransferSyntax = EXS_JPEGProcess14SV1TransferSyntax;
    if (cmd.findOption("--prefer-jpeg8"))        opt_networkTransferSyntax = EXS_JPEGProcess1TransferSyntax;
    if (cmd.findOption("--prefer-jpeg12"))       opt_networkTransferSyntax = EXS_JPEGProcess2_4TransferSyntax;
    if (cmd.findOption("--prefer-j2k-lossless")) opt_networkTransferSyntax = EXS_JPEG2000LosslessOnly;
    if (cmd.findOption("--prefer-j2k-lossy"))    opt_networkTransferSyntax = EXS_JPEG2000;
    if (cmd.findOption("--prefer-rle"))          opt_networkTransferSyntax = EXS_RLELossless;
#ifdef WITH_ZLIB
    if (cmd.findOption("--prefer-deflated"))     opt_networkTransferSyntax = EXS_DeflatedLittleEndianExplicit;
#endif
    if (cmd.findOption("--implicit"))            opt_networkTransferSyntax = EXS_LittleEndianImplicit;
    cmd.endOptionBlock();



    if (cmd.findOption("--aetitle")) app.checkValue(cmd.getValue(opt_respondingaetitle));
    if (cmd.findOption("--max-pdu")) app.checkValue(cmd.getValueAndCheckMinMax(opt_maxPDU, ASC_MINIMUMPDUSIZE, ASC_MAXIMUMPDUSIZE));
    if (cmd.findOption("--disable-host-lookup")) dcmDisableGethostbyaddr.set(OFTrue);
    if (cmd.findOption("--refuse")) opt_refuseAssociation = OFTrue;
    if (cmd.findOption("--reject")) opt_rejectWithoutImplementationUID = OFTrue;
    if (cmd.findOption("--ignore")) opt_ignore = OFTrue;
    if (cmd.findOption("--sleep-after")) app.checkValue(cmd.getValueAndCheckMin(opt_sleepAfter, 0));
    if (cmd.findOption("--sleep-during")) app.checkValue(cmd.getValueAndCheckMin(opt_sleepDuring, 0));
    if (cmd.findOption("--abort-after")) opt_abortAfterStore = OFTrue;
    if (cmd.findOption("--abort-during")) opt_abortDuringStore = OFTrue;
    if (cmd.findOption("--promiscuous")) opt_promiscuous = OFTrue;
    if (cmd.findOption("--uid-padding")) opt_correctUIDPadding = OFTrue;




    cmd.beginOptionBlock();
    if (cmd.findOption("--normal")) opt_bitPreserving = OFFalse;
    if (cmd.findOption("--bit-preserving")) opt_bitPreserving = OFTrue;
    cmd.endOptionBlock();

    cmd.beginOptionBlock();
    if (cmd.findOption("--write-file")) opt_useMetaheader = OFTrue;
    if (cmd.findOption("--write-dataset")) opt_useMetaheader = OFFalse;
    cmd.endOptionBlock();

    cmd.beginOptionBlock();
    if (cmd.findOption("--write-xfer-same")) opt_writeTransferSyntax = EXS_Unknown;
    if (cmd.findOption("--write-xfer-little"))
    {
      app.checkConflict("--write-xfer-little", "--bit-preserving", opt_bitPreserving);
      app.checkConflict("--write-xfer-little", "--prefer-lossless", opt_networkTransferSyntax==EXS_JPEGProcess14SV1TransferSyntax);
      app.checkConflict("--write-xfer-little", "--prefer-jpeg8", opt_networkTransferSyntax==EXS_JPEGProcess1TransferSyntax);
      app.checkConflict("--write-xfer-little", "--prefer-jpeg12", opt_networkTransferSyntax==EXS_JPEGProcess2_4TransferSyntax);
      app.checkConflict("--write-xfer-little", "--prefer-j2k-lossy", opt_networkTransferSyntax==EXS_JPEG2000);
      app.checkConflict("--write-xfer-little", "--prefer-j2k-lossless", opt_networkTransferSyntax==EXS_JPEG2000LosslessOnly);
      app.checkConflict("--write-xfer-little", "--prefer-rle", opt_networkTransferSyntax==EXS_RLELossless);
      opt_writeTransferSyntax = EXS_LittleEndianExplicit;
    }
    if (cmd.findOption("--write-xfer-big"))
    {
      app.checkConflict("--write-xfer-big", "--bit-preserving", opt_bitPreserving);
      app.checkConflict("--write-xfer-big", "--prefer-lossless", opt_networkTransferSyntax==EXS_JPEGProcess14SV1TransferSyntax);
      app.checkConflict("--write-xfer-big", "--prefer-jpeg8", opt_networkTransferSyntax==EXS_JPEGProcess1TransferSyntax);
      app.checkConflict("--write-xfer-big", "--prefer-jpeg12", opt_networkTransferSyntax==EXS_JPEGProcess2_4TransferSyntax);
      app.checkConflict("--write-xfer-big", "--prefer-j2k-lossy", opt_networkTransferSyntax==EXS_JPEG2000);
      app.checkConflict("--write-xfer-big", "--prefer-j2k-lossless", opt_networkTransferSyntax==EXS_JPEG2000LosslessOnly);
      app.checkConflict("--write-xfer-big", "--prefer-rle", opt_networkTransferSyntax==EXS_RLELossless);
      opt_writeTransferSyntax = EXS_BigEndianExplicit;
    }
    if (cmd.findOption("--write-xfer-implicit"))
    {
      app.checkConflict("--write-xfer-implicit", "--bit-preserving", opt_bitPreserving);
      app.checkConflict("--write-xfer-implicit", "--prefer-lossless", opt_networkTransferSyntax==EXS_JPEGProcess14SV1TransferSyntax);
      app.checkConflict("--write-xfer-implicit", "--prefer-jpeg8", opt_networkTransferSyntax==EXS_JPEGProcess1TransferSyntax);
      app.checkConflict("--write-xfer-implicit", "--prefer-jpeg12", opt_networkTransferSyntax==EXS_JPEGProcess2_4TransferSyntax);
      app.checkConflict("--write-xfer-implicit", "--prefer-j2k-lossy", opt_networkTransferSyntax==EXS_JPEG2000);
      app.checkConflict("--write-xfer-implicit", "--prefer-j2k-lossless", opt_networkTransferSyntax==EXS_JPEG2000LosslessOnly);
      app.checkConflict("--write-xfer-implicit", "--prefer-rle", opt_networkTransferSyntax==EXS_RLELossless);
      opt_writeTransferSyntax = EXS_LittleEndianImplicit;
    }
#ifdef WITH_ZLIB
    if (cmd.findOption("--write-xfer-deflated"))
    {
      app.checkConflict("--write-xfer-deflated", "--bit-preserving", opt_bitPreserving);
      app.checkConflict("--write-xfer-deflated", "--prefer-lossless", opt_networkTransferSyntax==EXS_JPEGProcess14SV1TransferSyntax);
      app.checkConflict("--write-xfer-deflated", "--prefer-jpeg8", opt_networkTransferSyntax==EXS_JPEGProcess1TransferSyntax);
      app.checkConflict("--write-xfer-deflated", "--prefer-jpeg12", opt_networkTransferSyntax==EXS_JPEGProcess2_4TransferSyntax);
      app.checkConflict("--write-xfer-deflated", "--prefer-j2k-lossy", opt_networkTransferSyntax==EXS_JPEG2000);
      app.checkConflict("--write-xfer-deflated", "--prefer-j2k-lossless", opt_networkTransferSyntax==EXS_JPEG2000LosslessOnly);
      app.checkConflict("--write-xfer-deflated", "--prefer-rle", opt_networkTransferSyntax==EXS_RLELossless);
      opt_writeTransferSyntax = EXS_DeflatedLittleEndianExplicit;
    }
#endif
    cmd.endOptionBlock();



    cmd.beginOptionBlock();
    if (cmd.findOption("--length-explicit"))
    {
      app.checkConflict("--length-explicit", "--bit-preserving", opt_bitPreserving);
      opt_sequenceType = EET_ExplicitLength;
    }
    if (cmd.findOption("--length-undefined"))
    {
      app.checkConflict("--length-undefined", "--bit-preserving", opt_bitPreserving);
      opt_sequenceType = EET_UndefinedLength;
    }
    cmd.endOptionBlock();

    cmd.beginOptionBlock();
    if (cmd.findOption("--padding-off")) opt_paddingType = EPD_withoutPadding;
    if (cmd.findOption("--padding-create"))
    {
      app.checkConflict("--padding-create", "--write-dataset", ! opt_useMetaheader);
      app.checkConflict("--padding-create", "--bit-preserving", opt_bitPreserving);
      app.checkValue(cmd.getValueAndCheckMin(opt_filepad, 0));
      app.checkValue(cmd.getValueAndCheckMin(opt_itempad, 0));
      opt_paddingType = EPD_withPadding;
    }
    cmd.endOptionBlock();

#ifdef WITH_ZLIB
    cmd.beginOptionBlock();
    if (cmd.findOption("--compression-level"))
    {
        if (opt_writeTransferSyntax != EXS_DeflatedLittleEndianExplicit && opt_writeTransferSyntax != EXS_Unknown)
          app.printError("--compression-level only allowed with --write-xfer-deflated or --write-xfer-same");
        app.checkValue(cmd.getValueAndCheckMinMax(opt_compressionLevel, 0, 9));
        dcmZlibCompressionLevel.set(OFstatic_cast(int, opt_compressionLevel));
    }
    cmd.endOptionBlock();
#endif

    if (cmd.findOption("--sort-conc-studies"))
    {
      app.checkConflict("--sort-conc-studies", "--bit-preserving", opt_bitPreserving);
      app.checkValue(cmd.getValue(opt_sortConcerningStudies));
    }

    cmd.beginOptionBlock();
    if (cmd.findOption("--default-filenames")) opt_uniqueFilenames = OFFalse;
    if (cmd.findOption("--unique-filenames")) opt_uniqueFilenames = OFTrue;
    cmd.endOptionBlock();

    if (cmd.findOption("--timenames")) opt_timeNames = OFTrue;
    if (cmd.findOption("--filename-extension"))
        app.checkValue(cmd.getValue(opt_fileNameExtension));
    if (cmd.findOption("--timenames"))
        app.checkConflict("--timenames", "--unique-filenames", opt_uniqueFilenames);

    if (cmd.findOption("--sort-conc-studies"))
    {
      app.checkConflict("--sort-conc-studies", "--bit-preserving", opt_bitPreserving);
      app.checkValue(cmd.getValue(opt_sortConcerningStudies));
    }


    if (cmd.findOption("--exec-on-reception")) app.checkValue(cmd.getValue(opt_execOnReception));

    if (cmd.findOption("--exec-on-eostudy"))
    {
      app.checkDependence("--exec-on-eostudy", "--sort-conc-studies", opt_sortConcerningStudies != NULL );
      app.checkValue(cmd.getValue(opt_execOnEndOfStudy));
    }

    if (cmd.findOption("--rename-on-eostudy"))
    {
      app.checkDependence("--rename-on-eostudy", "--sort-conc-studies", opt_sortConcerningStudies != NULL );
      opt_renameOnEndOfStudy = OFTrue;
    }

    if (cmd.findOption("--eostudy-timeout"))
    {
      if( opt_sortConcerningStudies == NULL && opt_execOnEndOfStudy == NULL && opt_renameOnEndOfStudy == OFFalse )
        app.printError("--eostudy-timeout only in combination with --sort-conc-studies, --exec-on-eostudy or --rename-on-eostudy");
      app.checkValue(cmd.getValueAndCheckMin(opt_endOfStudyTimeout, 0));
    }

#ifdef _WIN32
    if (cmd.findOption("--exec-sync")) opt_execSync = OFTrue;
#endif

  }

#ifdef WITH_OPENSSL

#ifdef DEBUG
  /* prevent command line code from moaning that --add-cert-dir and --add-cert-file have not been checked */
  if (cmd.findOption("--add-cert-dir", 0, OFCommandLine::FOM_First)) /* nothing */ ;
  if (cmd.findOption("--add-cert-file", 0, OFCommandLine::FOM_First)) /* nothing */ ;
#endif

  cmd.beginOptionBlock();
  if (cmd.findOption("--disable-tls")) opt_secureConnection = OFFalse;
  if (cmd.findOption("--enable-tls"))
  {
    opt_secureConnection = OFTrue;
    app.checkValue(cmd.getValue(opt_privateKeyFile));
    app.checkValue(cmd.getValue(opt_certificateFile));
  }
  cmd.endOptionBlock();

  cmd.beginOptionBlock();
  if (cmd.findOption("--std-passwd"))
  {
    if (! opt_secureConnection) app.printError("--std-passwd only with --enable-tls");
    opt_passwd = NULL;
  }
  if (cmd.findOption("--use-passwd"))
  {
    if (! opt_secureConnection) app.printError("--use-passwd only with --enable-tls");
    app.checkValue(cmd.getValue(opt_passwd));
  }
  if (cmd.findOption("--null-passwd"))
  {
    if (! opt_secureConnection) app.printError("--null-passwd only with --enable-tls");
    opt_passwd = "";
  }
  cmd.endOptionBlock();

  cmd.beginOptionBlock();
  if (cmd.findOption("--pem-keys")) opt_keyFileFormat = SSL_FILETYPE_PEM;
  if (cmd.findOption("--der-keys")) opt_keyFileFormat = SSL_FILETYPE_ASN1;
  cmd.endOptionBlock();

  if (cmd.findOption("--dhparam"))
  {
    app.checkValue(cmd.getValue(opt_dhparam));
  }

  if (cmd.findOption("--seed"))
  {
    app.checkValue(cmd.getValue(opt_readSeedFile));
  }

  cmd.beginOptionBlock();
  if (cmd.findOption("--write-seed"))
  {
    if (opt_readSeedFile == NULL) app.printError("--write-seed only with --seed");
    opt_writeSeedFile = opt_readSeedFile;
  }
  if (cmd.findOption("--write-seed-file"))
  {
    if (opt_readSeedFile == NULL) app.printError("--write-seed-file only with --seed");
    app.checkValue(cmd.getValue(opt_writeSeedFile));
  }
  cmd.endOptionBlock();

  cmd.beginOptionBlock();
  if (cmd.findOption("--require-peer-cert")) opt_certVerification = DCV_requireCertificate;
  if (cmd.findOption("--verify-peer-cert"))  opt_certVerification = DCV_checkCertificate;
  if (cmd.findOption("--ignore-peer-cert"))  opt_certVerification = DCV_ignoreCertificate;
  cmd.endOptionBlock();

  const char *current = NULL;
  const char *currentOpenSSL;
  if (cmd.findOption("--cipher", 0, OFCommandLine::FOM_First))
  {
    opt_ciphersuites.clear();
    do
    {
      app.checkValue(cmd.getValue(current));
      if (NULL == (currentOpenSSL = DcmTLSTransportLayer::findOpenSSLCipherSuiteName(current)))
      {
        CERR << "ciphersuite '" << current << "' is unknown. Known ciphersuites are:" << endl;
        unsigned long numSuites = DcmTLSTransportLayer::getNumberOfCipherSuites();
        for (unsigned long cs=0; cs < numSuites; cs++)
        {
          CERR << "    " << DcmTLSTransportLayer::getTLSCipherSuiteName(cs) << endl;
        }
        return 1;
      }
      else
      {
        if (opt_ciphersuites.length() > 0) opt_ciphersuites += ":";
        opt_ciphersuites += currentOpenSSL;
      }
    } while (cmd.findOption("--cipher", 0, OFCommandLine::FOM_Next));
  }

#endif


#ifdef HAVE_GETEUID
  /* if port is privileged we must be as well */
  if (opt_port < 1024)
  {
    if (geteuid() != 0)
    {
      fprintf(stderr, "storescp: cannot listen on port %lu, insufficient privileges\n", opt_port);
      return 1;
    }
  }
#endif

  /* make sure data dictionary is loaded */
  if (!dcmDataDict.isDictionaryLoaded())
  {
    fprintf(stderr, "Warning: no data dictionary loaded, check environment variable: %s\n", DCM_DICT_ENVIRONMENT_VARIABLE);
  }

  /* if the output directory does not equal "." (default directory) */
  if( opt_outputDirectory.compare(".") != 0 )
  {
    /* if there is a path separator at the end of the path, get rid of it */
    if( opt_outputDirectory[opt_outputDirectory.length()-1] == PATH_SEPARATOR )
    {
      opt_outputDirectory.erase(opt_outputDirectory.length()-1, 1);
    }

    /* check if the specified directory exists and if it is a directory.
     * If the output directory is invalid, dump an error message and terminate execution.
     */
    if( !OFStandard::dirExists(opt_outputDirectory) )
    {
      CERR << "Error: invalid output directory encountered ('" << opt_outputDirectory << "')" << endl;
      return 1;
    }
  }

#ifdef HAVE_FORK
    if (opt_forkMode)
      DUL_requestForkOnTransportConnectionReceipt(argc, argv);
#elif defined(_WIN32)
  if (opt_forkedChild)
  {
    // child process
    DUL_markProcessAsForkedChild();

    char buf[256];
    DWORD bytesRead = 0;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

    // read socket handle number from stdin, i.e. the anonymous pipe
	// to which our parent process has written the handle number.
    if (ReadFile(hStdIn, buf, sizeof(buf), &bytesRead, NULL))
	{
        // make sure buffer is zero terminated
		buf[bytesRead] = '\0';
        dcmExternalSocketHandle.set(atoi(buf));
    }
    else
	{
      CERR << "Error while reading socket handle: " << GetLastError() << endl;
      return 1;
    }
  }
  else
  {
    // parent process
    if (opt_forkMode)
      DUL_requestForkOnTransportConnectionReceipt(argc, argv);
  }
#endif

  /* initialize network, i.e. create an instance of T_ASC_Network*. */
  OFCondition cond = ASC_initializeNetwork(NET_ACCEPTOR, OFstatic_cast(int, opt_port), /* timeout */opt_acse_timeout, &net);
  if (cond.bad())
  {
    DimseCondition::dump(cond);
    return 1;
  }

#ifdef HAVE_GETUID
  /* return to normal uid so that we can't do too much damage in case
   * things go very wrong.   Only does someting if the program is setuid
   * root, and run by another user.  Running as root user may be
   * potentially disasterous if this program screws up badly.
   */
  setuid(getuid());
#endif

#ifdef WITH_OPENSSL

  DcmTLSTransportLayer *tLayer = NULL;
  if (opt_secureConnection)
  {
    tLayer = new DcmTLSTransportLayer(DICOM_APPLICATION_ACCEPTOR, opt_readSeedFile);
    if (tLayer == NULL)
    {
      app.printError("unable to create TLS transport layer");
    }

    if (cmd.findOption("--add-cert-file", 0, OFCommandLine::FOM_First))
    {
      do
      {
        app.checkValue(cmd.getValue(current));
        if (TCS_ok != tLayer->addTrustedCertificateFile(current, opt_keyFileFormat))
        {
          CERR << "warning unable to load certificate file '" << current << "', ignoring" << endl;
        }
      } while (cmd.findOption("--add-cert-file", 0, OFCommandLine::FOM_Next));
    }

    if (cmd.findOption("--add-cert-dir", 0, OFCommandLine::FOM_First))
    {
      do
      {
        app.checkValue(cmd.getValue(current));
        if (TCS_ok != tLayer->addTrustedCertificateDir(current, opt_keyFileFormat))
        {
          CERR << "warning unable to certificates from directory '" << current << "', ignoring" << endl;
        }
      } while (cmd.findOption("--add-cert-dir", 0, OFCommandLine::FOM_Next));
    }

    if (opt_dhparam && ! (tLayer->setTempDHParameters(opt_dhparam)))
    {
      CERR << "warning unable to load temporary DH parameter file '" << opt_dhparam << "', ignoring" << endl;
    }

    if (opt_passwd) tLayer->setPrivateKeyPasswd(opt_passwd);

    if (TCS_ok != tLayer->setPrivateKeyFile(opt_privateKeyFile, opt_keyFileFormat))
    {
      CERR << "unable to load private TLS key from '" << opt_privateKeyFile << "'" << endl;
      return 1;
    }
    if (TCS_ok != tLayer->setCertificateFile(opt_certificateFile, opt_keyFileFormat))
    {
      CERR << "unable to load certificate from '" << opt_certificateFile << "'" << endl;
      return 1;
    }
    if (! tLayer->checkPrivateKeyMatchesCertificate())
    {
      CERR << "private key '" << opt_privateKeyFile << "' and certificate '" << opt_certificateFile << "' do not match" << endl;
      return 1;
    }

    if (TCS_ok != tLayer->setCipherSuites(opt_ciphersuites.c_str()))
    {
      CERR << "unable to set selected cipher suites" << endl;
      return 1;
    }

    tLayer->setCertificateVerification(opt_certVerification);


    cond = ASC_setTransportLayer(net, tLayer, 0);
    if (cond.bad())
    {
        DimseCondition::dump(cond);
        return 1;
    }
  }

#endif

#ifdef HAVE_WAITPID
  // register signal handler
  signal(SIGCHLD, sigChildHandler);
#endif

  while (cond.good())
  {
    /* receive an association and acknowledge or reject it. If the association was */
    /* acknowledged, offer corresponding services and invoke one or more if required. */
	cond = acceptAssociation(net,frame);

    /* remove zombie child processes */
    cleanChildren(-1, OFFalse);
#ifdef WITH_OPENSSL
    /* since storescp is usually terminated with SIGTERM or the like,
     * we write back an updated random seed after every association handled.
     */
    if (tLayer && opt_writeSeedFile)
    {
      if (tLayer->canWriteRandomSeed())
      {
        if (!tLayer->writeRandomSeed(opt_writeSeedFile))
        {
          CERR << "Error while writing random seed file '" << opt_writeSeedFile << "', ignoring." << endl;
        }
      }
      else
      {
        CERR << "Warning: cannot write random seed, ignoring." << endl;
      }
    }
#endif
    // if running in inetd mode, we always terminate after one association
    if (dcmExternalSocketHandle.get() >= 0) break;

    // if running in multi-process mode, always terminate child after one association
    if (DUL_processIsForkedChild()) break;

  }

  /* drop the network, i.e. free memory of T_ASC_Network* structure. This call */
  /* is the counterpart of ASC_initializeNetwork(...) which was called above. */
  cond = ASC_dropNetwork(&net);
  if (cond.bad())
  {
    DimseCondition::dump(cond);
    return 1;
  }

#ifdef HAVE_WINSOCK_H
  WSACleanup();
#endif

#ifdef WITH_OPENSSL
  delete tLayer;
#endif

  return 0;
}



static OFCondition acceptAssociation(T_ASC_Network *net,MainFrame *frame)
{
  char buf[BUFSIZ];
  T_ASC_Association *assoc;
  OFCondition cond;
  OFString sprofile;

#ifdef PRIVATE_STORESCP_VARIABLES
  PRIVATE_STORESCP_VARIABLES
#endif

  const char* knownAbstractSyntaxes[] =
  {
    UID_VerificationSOPClass
  };

  const char* transferSyntaxes[] = { NULL, NULL, NULL, NULL };
  int numTransferSyntaxes = 0;

  // try to receive an association. Here we either want to use blocking or
  // non-blocking, depending on if the option --eostudy-timeout is set.
  if( opt_endOfStudyTimeout == -1 )
    cond = ASC_receiveAssociation(net, &assoc, opt_maxPDU, NULL, NULL, opt_secureConnection);
  else
    cond = ASC_receiveAssociation(net, &assoc, opt_maxPDU, NULL, NULL, opt_secureConnection, DUL_NOBLOCK, OFstatic_cast(int, opt_endOfStudyTimeout));

  if (cond.code() == DULC_FORKEDCHILD)
  {
      // if (opt_verbose) DimseCondition::dump(cond);
      goto cleanup;
  }

  // if some kind of error occured, take care of it
  if (cond.bad())
  {
    // check what kind of error occurred. If no association was
    // received, check if certain other conditions are met
    if( cond == DUL_NOASSOCIATIONREQUEST )
    {
      // If in addition to the fact that no association was received also option --eostudy-timeout is set
      // and if at the same time there is still a study which is considered to be open (i.e. we were actually
      // expecting to receive more objects that belong to this study) (this is the case if lastStudyInstanceUID
      // does not equal NULL), we have to consider that all objects for the current study have been received.
      // In such an "end-of-study" case, we might have to execute certain optional functions which were specified
      // by the user through command line options passed to storescp.
      if( opt_endOfStudyTimeout != -1 && ! lastStudyInstanceUID.empty() )
      {
        // indicate that the end-of-study-event occured through a timeout event.
        // This knowledge will be necessary in function renameOnEndOFStudy().
        endOfStudyThroughTimeoutEvent = OFTrue;

        // before we actually execute those optional functions, we need to determine the path and name
        // of the subdirectory into which the DICOM files for the last study were written.
        lastStudySubdirectoryPathAndName = subdirectoryPathAndName;

        // now we can finally handle end-of-study events which might have to be executed
        executeEndOfStudyEvents();

        // also, we need to clear lastStudyInstanceUID to indicate
        // that the last study is not considered to be open any more.
        lastStudyInstanceUID.clear();

        // also, we need to clear subdirectoryPathAndName
        subdirectoryPathAndName.clear();

        // reset the endOfStudyThroughTimeoutEvent variable.
        endOfStudyThroughTimeoutEvent = OFFalse;
      }
    }
    // If something else was wrong we might have to dump an error message.
    else
    {
      if( opt_verbose ) DimseCondition::dump(cond);
    }

    // no matter what kind of error occurred, we need to do a cleanup
    goto cleanup;
  }

  if (opt_verbose)
  {
#if defined(HAVE_FORK) || defined(_WIN32)
      if (opt_forkMode)
      {
        printf("Association Received in %s process (pid: %ld)\n", (DUL_processIsForkedChild() ? "child" : "parent") , OFstatic_cast(long, getpid()));
      }
      else printf("Association Received\n");
#else
      printf("Association Received\n");
#endif
  }

  if (opt_debug)
  {
    printf("Parameters:\n");
    ASC_dumpParameters(assoc->params, COUT);

    DIC_AE callingTitle;
    DIC_AE calledTitle;
    ASC_getAPTitles(assoc->params, callingTitle, calledTitle, NULL);

    CERR << "called AE:  " << calledTitle << endl
         << "calling AE: " << callingTitle << endl;
  }

  if (opt_refuseAssociation)
  {
    T_ASC_RejectParameters rej =
    {
      ASC_RESULT_REJECTEDPERMANENT,
      ASC_SOURCE_SERVICEUSER,
      ASC_REASON_SU_NOREASON
    };

    if (opt_verbose) printf("Refusing Association (forced via command line)\n");
    cond = ASC_rejectAssociation(assoc, &rej);
    if (cond.bad())
    {
      printf("Association Reject Failed:\n");
      DimseCondition::dump(cond);
    }
    goto cleanup;
  }

  switch (opt_networkTransferSyntax)
  {
    case EXS_LittleEndianImplicit:
      /* we only support Little Endian Implicit */
      transferSyntaxes[0]  = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 1;
      break;
    case EXS_LittleEndianExplicit:
      /* we prefer Little Endian Explicit */
      transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[2]  = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 3;
      break;
    case EXS_BigEndianExplicit:
      /* we prefer Big Endian Explicit */
      transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2]  = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 3;
      break;
    case EXS_JPEGProcess14SV1TransferSyntax:
      /* we prefer JPEGLossless:Hierarchical-1stOrderPrediction (default lossless) */
      transferSyntaxes[0] = UID_JPEGProcess14SV1TransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 4;
      break;
    case EXS_JPEGProcess1TransferSyntax:
      /* we prefer JPEGBaseline (default lossy for 8 bit images) */
      transferSyntaxes[0] = UID_JPEGProcess1TransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 4;
      break;
    case EXS_JPEGProcess2_4TransferSyntax:
      /* we prefer JPEGExtended (default lossy for 12 bit images) */
      transferSyntaxes[0] = UID_JPEGProcess2_4TransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 4;
      break;
    case EXS_JPEG2000:
      /* we prefer JPEG2000 Lossy */
      transferSyntaxes[0] = UID_JPEG2000TransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 4;
      break;
    case EXS_JPEG2000LosslessOnly:
      /* we prefer JPEG2000 Lossless */
      transferSyntaxes[0] = UID_JPEG2000LosslessOnlyTransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 4;
      break;
    case EXS_RLELossless:
      /* we prefer RLE Lossless */
      transferSyntaxes[0] = UID_RLELosslessTransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 4;
      break;
#ifdef WITH_ZLIB
    case EXS_DeflatedLittleEndianExplicit:
      /* we prefer Deflated Explicit VR Little Endian */
      transferSyntaxes[0] = UID_DeflatedExplicitVRLittleEndianTransferSyntax;
      transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 4;
      break;
#endif
    default:
      /* We prefer explicit transfer syntaxes.
       * If we are running on a Little Endian machine we prefer
       * LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax.
       */
      if (gLocalByteOrder == EBO_LittleEndian)  /* defined in dcxfer.h */
      {
        transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
        transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
      }
      else
      {
        transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
        transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      }
      transferSyntaxes[2] = UID_LittleEndianImplicitTransferSyntax;
      numTransferSyntaxes = 3;
      break;
  }


    /* accept the Verification SOP Class if presented */
    cond = ASC_acceptContextsWithPreferredTransferSyntaxes( assoc->params, knownAbstractSyntaxes, DIM_OF(knownAbstractSyntaxes), transferSyntaxes, numTransferSyntaxes);
    if (cond.bad())
    {
      if (opt_verbose) DimseCondition::dump(cond);
      goto cleanup;
    }

    /* the array of Storage SOP Class UIDs comes from dcuid.h */
    cond = ASC_acceptContextsWithPreferredTransferSyntaxes( assoc->params, dcmAllStorageSOPClassUIDs, numberOfAllDcmStorageSOPClassUIDs, transferSyntaxes, numTransferSyntaxes);
    if (cond.bad())
    {
      if (opt_verbose) DimseCondition::dump(cond);
      goto cleanup;
    }

    if (opt_promiscuous)
    {
      /* accept everything not known not to be a storage SOP class */
      cond = acceptUnknownContextsWithPreferredTransferSyntaxes(
        assoc->params, transferSyntaxes, numTransferSyntaxes);
      if (cond.bad())
      {
        if (opt_verbose) DimseCondition::dump(cond);
        goto cleanup;
      }
    }
  

  /* set our app title */
  ASC_setAPTitles(assoc->params, NULL, NULL, opt_respondingaetitle);

  /* acknowledge or reject this association */
  cond = ASC_getApplicationContextName(assoc->params, buf);
  if ((cond.bad()) || strcmp(buf, UID_StandardApplicationContext) != 0)
  {
    /* reject: the application context name is not supported */
    T_ASC_RejectParameters rej =
    {
      ASC_RESULT_REJECTEDPERMANENT,
      ASC_SOURCE_SERVICEUSER,
      ASC_REASON_SU_APPCONTEXTNAMENOTSUPPORTED
    };

    if (opt_verbose) printf("Association Rejected: bad application context name: %s\n", buf);
    cond = ASC_rejectAssociation(assoc, &rej);
    if (cond.bad())
    {
      if (opt_verbose) DimseCondition::dump(cond);
    }
    goto cleanup;

  }
  else if (opt_rejectWithoutImplementationUID && strlen(assoc->params->theirImplementationClassUID) == 0)
  {
    /* reject: the no implementation Class UID provided */
    T_ASC_RejectParameters rej =
    {
      ASC_RESULT_REJECTEDPERMANENT,
      ASC_SOURCE_SERVICEUSER,
      ASC_REASON_SU_NOREASON
    };

    if (opt_verbose) printf("Association Rejected: No Implementation Class UID provided\n");
    cond = ASC_rejectAssociation(assoc, &rej);
    if (cond.bad())
    {
      if (opt_verbose) DimseCondition::dump(cond);
    }
    goto cleanup;
  }
  else
  {
#ifdef PRIVATE_STORESCP_CODE
    PRIVATE_STORESCP_CODE
#endif
    cond = ASC_acknowledgeAssociation(assoc);
    if (cond.bad())
    {
      DimseCondition::dump(cond);
      goto cleanup;
    }
    if (opt_verbose)
    {
      printf("Association Acknowledged (Max Send PDV: %lu)\n", assoc->sendPDVLength);
      if (ASC_countAcceptedPresentationContexts(assoc->params) == 0)
        printf("    (but no valid presentation contexts)\n");
      if (opt_debug) ASC_dumpParameters(assoc->params, COUT);
    }
  }

#ifdef BUGGY_IMPLEMENTATION_CLASS_UID_PREFIX
  /* active the dcmPeerRequiresExactUIDCopy workaround code
   * (see comments in dimse.h) for a implementation class UID
   * prefix known to exhibit the buggy behaviour.
   */
  if (0 == strncmp(assoc->params->theirImplementationClassUID,
      BUGGY_IMPLEMENTATION_CLASS_UID_PREFIX,
      strlen(BUGGY_IMPLEMENTATION_CLASS_UID_PREFIX)))
  {
    dcmEnableAutomaticInputDataCorrection.set(OFFalse);
    dcmPeerRequiresExactUIDCopy.set(OFTrue);
  }
#endif

  // store calling and called aetitle in global variables to enable
  // the --exec options using them. Enclose in quotation marks because
  // aetitles may contain space characters.
  DIC_AE callingTitle;
  DIC_AE calledTitle;
  if (ASC_getAPTitles(assoc->params, callingTitle, calledTitle, NULL).good())
  {
    callingaetitle = "\"";
    callingaetitle += callingTitle;
    callingaetitle += "\"";
    calledaetitle = "\"";
    calledaetitle += calledTitle;
    calledaetitle += "\"";
  }
  else
  {
    // should never happen
    callingaetitle.clear();
    calledaetitle.clear();
  }

  /* now do the real work, i.e. receive DIMSE commmands over the network connection */
  /* which was established and handle these commands correspondingly. In case of */
  /* storscp only C-ECHO-RQ and C-STORE-RQ commands can be processed. */
  cond = processCommands(assoc,frame);

  if (cond == DUL_PEERREQUESTEDRELEASE)
  {
    printf("Association Release\n");
    m_blnFinished = true; 
    cond = ASC_acknowledgeRelease(assoc);
	
  }
  else if (cond == DUL_PEERABORTEDASSOCIATION)
  {
    if (opt_verbose) printf("Association Aborted\n");
  }
  else
  {
    fprintf(stderr, "storescp: DIMSE Failure (aborting association)\n");
    /* some kind of error so abort the association */
    cond = ASC_abortAssociation(assoc);
  }

cleanup:

  if (cond.code() == DULC_FORKEDCHILD) return cond;

  cond = ASC_dropSCPAssociation(assoc);
  if (cond.bad())
  {
    DimseCondition::dump(cond);
    exit(1);
  }
  cond = ASC_destroyAssociation(&assoc);
  if (cond.bad())
  {
    DimseCondition::dump(cond);
    exit(1);
  }
	


  return cond;
}



static OFCondition
processCommands(T_ASC_Association * assoc,MainFrame *frame)
    /*
     * This function receives DIMSE commmands over the network connection
     * and handles these commands correspondingly. Note that in case of
     * storscp only C-ECHO-RQ and C-STORE-RQ commands can be processed.
     *
     * Parameters:
     *   assoc - [in] The association (network connection to another DICOM application).
     */
{
  OFCondition cond = EC_Normal;
  T_DIMSE_Message msg;
  T_ASC_PresentationContextID presID = 0;
  DcmDataset *statusDetail = NULL;

  // start a loop to be able to receive more than one DIMSE command
  while( cond == EC_Normal || cond == DIMSE_NODATAAVAILABLE || cond == DIMSE_OUTOFRESOURCES )
  {
    // receive a DIMSE command over the network
    if( opt_endOfStudyTimeout == -1 )
      cond = DIMSE_receiveCommand(assoc, DIMSE_BLOCKING, 0, &presID, &msg, &statusDetail);
    else
      cond = DIMSE_receiveCommand(assoc, DIMSE_NONBLOCKING, OFstatic_cast(int, opt_endOfStudyTimeout), &presID, &msg, &statusDetail);

    // check what kind of error occurred. If no data was
    // received, check if certain other conditions are met
    if( cond == DIMSE_NODATAAVAILABLE )
    {
      // If in addition to the fact that no data was received also option --eostudy-timeout is set and
      // if at the same time there is still a study which is considered to be open (i.e. we were actually
      // expecting to receive more objects that belong to this study) (this is the case if lastStudyInstanceUID
      // does not equal NULL), we have to consider that all objects for the current study have been received.
      // In such an "end-of-study" case, we might have to execute certain optional functions which were specified
      // by the user through command line options passed to storescp.
      if( opt_endOfStudyTimeout != -1 && ! lastStudyInstanceUID.empty() )
      {
        // indicate that the end-of-study-event occured through a timeout event.
        // This knowledge will be necessary in function renameOnEndOFStudy().
        endOfStudyThroughTimeoutEvent = OFTrue;

        // before we actually execute those optional functions, we need to determine the path and name
        // of the subdirectory into which the DICOM files for the last study were written.
        lastStudySubdirectoryPathAndName = subdirectoryPathAndName;

        // now we can finally handle end-of-study events which might have to be executed
        executeEndOfStudyEvents();

        // also, we need to clear lastStudyInstanceUID to indicate
        // that the last study is not considered to be open any more.
        lastStudyInstanceUID.clear();

        // also, we need to clear subdirectoryPathAndName
        subdirectoryPathAndName.clear();

        // reset the endOfStudyThroughTimeoutEvent variable.
        endOfStudyThroughTimeoutEvent = OFFalse;
      }
    }

    // if the command which was received has extra status
    // detail information, dump this information
    if (statusDetail != NULL)
    {
      printf("Extra Status Detail: \n");
      statusDetail->print(COUT);
      delete statusDetail;
    }

    // check if peer did release or abort, or if we have a valid message
    if (cond == EC_Normal)
    {
      // in case we received a valid message, process this command
      // note that storescp can only process a C-ECHO-RQ and a C-STORE-RQ
      switch (msg.CommandField)
      {
        case DIMSE_C_ECHO_RQ:
          // process C-ECHO-Request
          cond = echoSCP(assoc, &msg, presID);
          break;
        case DIMSE_C_STORE_RQ:
          // process C-STORE-Request
          cond = storeSCP(assoc, &msg, presID,frame);
          break;
        default:
          // we cannot handle this kind of message
          cond = DIMSE_BADCOMMANDTYPE;
          fprintf(stderr, "storescp: Cannot handle command: 0x%x\n", OFstatic_cast(unsigned, msg.CommandField));
          break;
      }
    }
  }
  return cond;
}



static OFCondition echoSCP( T_ASC_Association * assoc, T_DIMSE_Message * msg, T_ASC_PresentationContextID presID)
{
  if (opt_verbose)
  {
    printf("Received ");
    DIMSE_printCEchoRQ(stdout, &msg->msg.CEchoRQ);
  }

  /* the echo succeeded !! */
  OFCondition cond = DIMSE_sendEchoResponse(assoc, presID, &msg->msg.CEchoRQ, STATUS_Success, NULL);
  if (cond.bad())
  {
    fprintf(stderr, "storescp: Echo SCP Failed:\n");
    DimseCondition::dump(cond);
  }
  return cond;
}



struct StoreCallbackData
{
  char* imageFileName;
  DcmFileFormat* dcmff;
  T_ASC_Association* assoc;
};



static void
storeSCPCallback(
    void *callbackData,
    T_DIMSE_StoreProgress *progress,
    T_DIMSE_C_StoreRQ *req,
    char * /*imageFileName*/, DcmDataset **imageDataSet,
    T_DIMSE_C_StoreRSP *rsp,
    DcmDataset **statusDetail)
    /*
     * This function.is used to indicate progress when storescp receives instance data over the
     * network. On the final call to this function (identified by progress->state == DIMSE_StoreEnd)
     * this function will store the data set which was received over the network to a file.
     * Earlier calls to this function will simply cause some information to be dumped to stdout.
     *
     * Parameters:
     *   callbackData  - [in] data for this callback function
     *   progress      - [in] The state of progress. (identifies if this is the initial or final call
     *                   to this function, or a call in between these two calls.
     *   req           - [in] The original store request message.
     *   imageFileName - [in] The path to and name of the file the information shall be written to.
     *   imageDataSet  - [in] The data set which shall be stored in the image file
     *   rsp           - [inout] the C-STORE-RSP message (will be sent after the call to this function)
     *   statusDetail  - [inout] This variable can be used to capture detailed information with regard to
     *                   the status information which is captured in the status element (0000,0900). Note
     *                   that this function does specify any such information, the pointer will be set to NULL.
     */
{
  DIC_UI sopClass;
  DIC_UI sopInstance;

  // determine if the association shall be aborted
  if( (opt_abortDuringStore && progress->state != DIMSE_StoreBegin) ||
      (opt_abortAfterStore && progress->state == DIMSE_StoreEnd) )
  {
    if (opt_verbose)
      printf("ABORT initiated (due to command line options)\n");
    ASC_abortAssociation((OFstatic_cast(StoreCallbackData*, callbackData))->assoc);
    rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
    return;
  }

  // if opt_sleepAfter is set, the user requires that the application shall
  // sleep a certain amount of seconds after having received one PDU.
  if (opt_sleepDuring > 0)
  {
    OFStandard::sleep(OFstatic_cast(unsigned int, opt_sleepDuring));
  }

  // dump some information if required (depending on the progress state)
  if (opt_verbose)
  {
    switch (progress->state)
    {
      case DIMSE_StoreBegin:
        printf("RECV:");
        break;
      case DIMSE_StoreEnd:
        printf("\n");
        break;
      default:
        putchar('.');
        break;
    }
    fflush(stdout);
  }

  // if this is the final call of this function, save the data which was received to a file
  // (note that we could also save the image somewhere else, put it in database, etc.)
  if (progress->state == DIMSE_StoreEnd)
  {
    // do not send status detail information
    *statusDetail = NULL;

    // remember callback data
    StoreCallbackData *cbdata = OFstatic_cast(StoreCallbackData *, callbackData);

    // Concerning the following line: an appropriate status code is already set in the resp structure,
    // it need not be success. For example, if the caller has already detected an out of resources problem
    // then the status will reflect this.  The callback function is still called to allow cleanup.
    //rsp->DimseStatus = STATUS_Success;

    // we want to write the received information to a file only if this information
    // is present and the options opt_bitPreserving and opt_ignore are not set.
    if ((imageDataSet)&&(*imageDataSet)&&(!opt_bitPreserving)&&(!opt_ignore))
    {
      OFString fileName;

      // in case option --sort-conc-studies is set, we need to perform some particular
      // steps to determine the actual name of the output file
      if( opt_sortConcerningStudies != NULL )
      {
        // determine the study instance UID in the (current) DICOM object that has just been received
        // note that if findAndGetString says the resulting study instance UID equals NULL, the study
        // instance UID in the (current) DICOM object is an empty string; in general: no matter what
        // happened, we want to create a new string that will contain a corresponding value for the
        // current study instance UID
        const char *tmpstr1 = NULL;
        OFString currentStudyInstanceUID;
        DcmTagKey studyInstanceUIDTagKey( DCM_StudyInstanceUID );
        OFCondition ec = (*imageDataSet)->findAndGetString( studyInstanceUIDTagKey, tmpstr1, OFFalse );
        if( ec != EC_Normal )
        {
          fprintf(stderr, "storescp: No study instance UID found in data set.\n");
          rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
          return;
        }
        if (tmpstr1) currentStudyInstanceUID = tmpstr1;

        // if this is the first DICOM object that was received or if the study instance UID in the
        // current DICOM object does not equal the last object's study instance UID we need to create
        // a new subdirectory in which the current DICOM object will be stored
        if( lastStudyInstanceUID.empty() || lastStudyInstanceUID != currentStudyInstanceUID)
        {
          // if lastStudyInstanceUID is non-empty, we have just completed receiving all objects for one
          // study. In such a case, we need to set a certain indicator variable (lastStudySubdirectoryPathAndName),
          // so that we know that executeOnEndOfStudy() might have to be executed later. In detail, this indicator
          // variable will contain the path and name of the last study's subdirectory, so that we can still remember
          // this directory, when we execute executeOnEndOfStudy(). The memory that is allocated for this variable
          // here will be freed after the execution of executeOnEndOfStudy().
          if( ! lastStudyInstanceUID.empty() )
          {
            lastStudySubdirectoryPathAndName = subdirectoryPathAndName;
          }

          // create the new lastStudyInstanceUID value according to the value in the current DICOM object
          lastStudyInstanceUID = currentStudyInstanceUID;

          // get the current time (needed for subdirectory name)
          OFDateTime dateTime;
          dateTime.setCurrentDateTime();
          // create a name for the new subdirectory. pattern: "[opt_sortConcerningStudies]_[YYYYMMDD]_[HHMMSSMMM]" (use current datetime)
          char buf[32];
          sprintf(buf, "_%04u%02u%02u_%02u%02u%02u%03u",
            dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
            dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond());
          OFString subdirectoryName = opt_sortConcerningStudies;
          subdirectoryName += buf;

          // create subdirectoryPathAndName (string with full path to new subdirectory)
          subdirectoryPathAndName = cbdata->imageFileName;
          size_t position = subdirectoryPathAndName.rfind(PATH_SEPARATOR);
          if (position != OFString_npos) subdirectoryPathAndName.erase(position+1);
          subdirectoryPathAndName += subdirectoryName;

          // check if the subdirectory is already existent
          // if it is already existent dump a warning
          if( OFStandard::dirExists(subdirectoryPathAndName) )
          {
            fprintf(stderr, "storescp: Warning: Subdirectory for studies already existent. (%s)\n", subdirectoryPathAndName.c_str() );
          }

          // if it is not existent create it
#ifdef HAVE_WINDOWS_H
          if( _mkdir( subdirectoryPathAndName.c_str() ) == -1 )
#else
          if( mkdir( subdirectoryPathAndName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO ) == -1 )
#endif
          {
            fprintf(stderr, "storescp: Could not create subdirectory %s.\n", subdirectoryPathAndName.c_str() );
            rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
            return;
          }
          // all objects of a study have been received, so a new subdirectory is started.
          // ->timename counter can be reset, because the next filename can't cause a duplicate.
          // if no reset would be done, files of a new study (->new directory) would start with a counter in filename
          if (opt_timeNames)
            timeNameCounter = -1;
        }

        // integrate subdirectory name into file name (note that cbdata->imageFileName currently contains both
        // path and file name; however, the path refers to the output directory captured in opt_outputDirectory)
        char *tmpstr5 = strrchr( cbdata->imageFileName, PATH_SEPARATOR );
        fileName = subdirectoryPathAndName;
        fileName += tmpstr5;

        // update global variable outputFileNameArray
        // (might be used in executeOnReception() and renameOnEndOfStudy)
        outputFileNameArray.push_back(++tmpstr5);
      }
      // if option --sort-conc-studies is not set, the determination of the output file name is simple
      else
      {
        fileName = cbdata->imageFileName;

        // update global variables outputFileNameArray
        // (might be used in executeOnReception() and renameOnEndOfStudy)
        const char *tmpstr6 = strrchr( fileName.c_str(), PATH_SEPARATOR );
        outputFileNameArray.push_back(++tmpstr6);
      }

      // determine the transfer syntax which shall be used to write the information to the file
      E_TransferSyntax xfer = opt_writeTransferSyntax;
      if (xfer == EXS_Unknown) xfer = (*imageDataSet)->getOriginalXfer();

      // store file either with meta header or as pure dataset
      OFCondition cond = cbdata->dcmff->saveFile(fileName.c_str(), xfer, opt_sequenceType, opt_groupLength,
          opt_paddingType, OFstatic_cast(Uint32, opt_filepad),
          OFstatic_cast(Uint32, opt_itempad), !opt_useMetaheader);
      if (cond.bad())
      {
        fprintf(stderr, "storescp: Cannot write image file: %s\n", fileName.c_str());
        rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
      }

      // check the image to make sure it is consistent, i.e. that its sopClass and sopInstance correspond
      // to those mentioned in the request. If not, set the status in the response message variable.
      if ((rsp->DimseStatus == STATUS_Success)&&(!opt_ignore))
      {
        // which SOP class and SOP instance ?
        if (! DU_findSOPClassAndInstanceInDataSet(*imageDataSet, sopClass, sopInstance, opt_correctUIDPadding))
        {
           fprintf(stderr, "storescp: Bad image file: %s\n", fileName.c_str());
           rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
        }
        else if (strcmp(sopClass, req->AffectedSOPClassUID) != 0)
        {
          rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
        }
        else if (strcmp(sopInstance, req->AffectedSOPInstanceUID) != 0)
        {
          rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
        }
      }

    }

    // in case opt_bitPreserving is set, do some other things
    if( opt_bitPreserving )
    {
      // we need to set outputFileNameArray and outputFileNameArrayCnt to be
      // able to perform the placeholder substitution in executeOnReception()
      char *tmpstr7 = strrchr( cbdata->imageFileName, PATH_SEPARATOR );
      outputFileNameArray.push_back(++tmpstr7);
    }
  }

  // return
  return;
}


static OFCondition storeSCP(
  T_ASC_Association *assoc,
  T_DIMSE_Message *msg,
  T_ASC_PresentationContextID presID,MainFrame *frame)
    /*
     * This function processes a DIMSE C-STORE-RQ commmand that was
     * received over the network connection.
     *
     * Parameters:
     *   assoc  - [in] The association (network connection to another DICOM application).
     *   msg    - [in] The DIMSE C-STORE-RQ message that was received.
     *   presID - [in] The ID of the presentation context which was specified in the PDV which contained
     *                 the DIMSE command.
     */
{
  OFCondition cond = EC_Normal;
  T_DIMSE_C_StoreRQ *req;
  char imageFileName[2048];

  // assign the actual information of the C-STORE-RQ command to a local variable
  req = &msg->msg.CStoreRQ;

  // if opt_ignore is set, the user requires that the data shall be received but not
  // stored. in this case, we want to create a corresponding temporary filename for
  // a file in which the data shall be stored temporarily. If this is not the case,
  // create a real filename (consisting of path and filename) for a real file.
  if (opt_ignore)
  {
#ifdef _WIN32
    tmpnam(imageFileName);
#else
    strcpy(imageFileName, NULL_DEVICE_NAME);
#endif
  }
  else
  {
    //3 possibilities: create unique filenames (fn), create timestamp fn, create fn from SOP Instance UIDs
    if (opt_uniqueFilenames)
    {
      // create unique filename by generating a temporary UID and using ".X." as an infix
      char buf[70];
      dcmGenerateUniqueIdentifier(buf);
      sprintf(imageFileName, "%s%c%s.X.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), buf, opt_fileNameExtension.c_str());
    }
    else if (opt_timeNames)
    {
      // create a name for the new file. pattern: "[YYYYMMDDHHMMSSMMM]_[NUMBER].MODALITY[EXTENSION]" (use current datetime)
      // get the current time (needed for file name)
      OFDateTime dateTime;
      dateTime.setCurrentDateTime();
      // used to hold prospective filename
      char cmpFileName[2048];
      // next if/else block generates prospective filename, that is compared to last written filename
      if (timeNameCounter == -1)
      {
        // timeNameCounter not set -> last written filename has to be without "serial number"
        sprintf(cmpFileName, "%04u%02u%02u%02u%02u%02u%03u.%s%s",
          dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
          dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
          dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
      }
      else
      {
        // counter was active before, so generate filename with "serial number" for comparison
        sprintf(cmpFileName, "%04u%02u%02u%02u%02u%02u%03u_%04u.%s%s", //millisecond version
          dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
          dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
          timeNameCounter, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
      }
      if ( (outputFileNameArray.size()!=0) && (outputFileNameArray.back() == cmpFileName) )
      {
        // if this is not the first run and the prospective filename is equal to the last written filename
        // generate one with a serial number (incremented by 1)
        timeNameCounter++;
        sprintf(imageFileName, "%s%c%04u%02u%02u%02u%02u%02u%03u_%04u.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, //millisecond version
        dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
        dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
        timeNameCounter, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
      }
      else
      {
        //first run or filenames are different: create filename without serial number
        sprintf(imageFileName, "%s%c%04u%02u%02u%02u%02u%02u%03u.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, //millisecond version
        dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
        dateTime.getTime().getHour(), dateTime.getTime().getMinute(),dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
        dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
        // reset counter, because timestamp and therefore filename has changed
        timeNameCounter = -1;
      }
    }
    else
    {
      // don't create new UID, use the study instance UID as found in object
      sprintf(imageFileName, "%s%c%s.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), req->AffectedSOPInstanceUID, opt_fileNameExtension.c_str());
	//frame->m_dirPath=imageFileName;
	frame->SetDirPath(imageFileName);
    }
  }

  // dump some information if required
  if (opt_verbose)
  {
    printf("Received ");
    DIMSE_printCStoreRQ(stdout, req);
  }

  // intialize some variables
  StoreCallbackData callbackData;
  callbackData.assoc = assoc;
  callbackData.imageFileName = imageFileName;
  DcmFileFormat dcmff;
  callbackData.dcmff = &dcmff;

  // define an address where the information which will be received over the network will be stored
  DcmDataset *dset = dcmff.getDataset();

  // if opt_bitPreserving is set, the user requires that the data shall be
  // written exactly as it was received. Depending on this option, function
  // DIMSE_storeProvider must be called with certain parameters.
  if (opt_bitPreserving)
  {
    cond = DIMSE_storeProvider(assoc, presID, req, imageFileName, opt_useMetaheader, NULL,
      storeSCPCallback, &callbackData, opt_blockMode, opt_dimse_timeout);
  }
  else
  {
    cond = DIMSE_storeProvider(assoc, presID, req, NULL, opt_useMetaheader, &dset,
      storeSCPCallback, &callbackData, opt_blockMode, opt_dimse_timeout);
  }

  // if some error occured, dump corresponding information and remove the outfile if necessary
  if (cond.bad())
  {
    fprintf(stderr, "storescp: Store SCP Failed:\n");
    DimseCondition::dump(cond);
    // remove file
    if (!opt_ignore)
    {
      if (strcmp(imageFileName, NULL_DEVICE_NAME) != 0) unlink(imageFileName);
    }
  }
#ifdef _WIN32
  else if (opt_ignore)
  {
    if (strcmp(imageFileName, NULL_DEVICE_NAME) != 0) unlink(imageFileName); // delete the temporary file
  }
#endif

  // if everything was successful so far and option --exec-on-reception is set,
  // we want to execute a certain command which was passed to the application
  if( cond.good() && opt_execOnReception != NULL )
    executeOnReception();

  // if everything was successful so far, go ahead and handle possible end-of-study events
  if( cond.good() )
    executeEndOfStudyEvents();

  // if opt_sleepAfter is set, the user requires that the application shall
  // sleep a certain amount of seconds after storing the instance data.
  if (opt_sleepAfter > 0)
  {
    OFStandard::sleep(OFstatic_cast(unsigned int, opt_sleepAfter));
  }

  // return return value
  return cond;
}



static void executeEndOfStudyEvents()
    /*
     * This function deals with the execution of end-of-study-events. In detail,
     * events that need to take place are sepcified by the user through certain
     * command line options. The options that define these end-of-study-events
     * are "--rename-on-eostudy" and "--exec-on-eostudy".
     *
     * Parameters:
     *   none.
     */
{
  // if option --rename-on-eostudy is set and variable lastStudySubdirectoryPathAndName
  // does not equal NULL (i.e. we received all objects that belong to one study, or - in
  // other words - it is the end of one study) we want to rename the output files that
  // belong to the last study. (Note that these files are captured in outputFileNameArray)
  if( opt_renameOnEndOfStudy && ! lastStudySubdirectoryPathAndName.empty() )
    renameOnEndOfStudy();

  // if option --exec-on-eostudy is set and variable lastStudySubdirectoryPathAndName does
  // not equal NULL (i.e. we received all objects that belong to one study, or - in other
  // words - it is the end of one study) we want to execute a certain command which was
  // passed to the application
  if( opt_execOnEndOfStudy != NULL && ! lastStudySubdirectoryPathAndName.empty() )
    executeOnEndOfStudy();

  lastStudySubdirectoryPathAndName.clear();
}


static void executeOnReception()
    /*
     * This function deals with the execution of the command line which was passed
     * to option --exec-on-reception of the storescp. This command line is captured
     * in opt_execOnReception. Note that the command line can contain the placeholders
     * PATH_PLACEHOLDER and FILENAME_PLACEHOLDER which need to be substituted before the command line is actually
     * executed. PATH_PLACEHOLDER will be substituted by the path to the output directory into which
     * the last file was written; FILENAME_PLACEHOLDER will be substituted by the filename of the last
     * file which was written.
     *
     * Parameters:
     *   none.
     */
{
  OFString cmd = opt_execOnReception;

  // in case a file was actually written
  if( !opt_ignore )
  {
    // perform substitution for placeholder #p; note that
    //  - in case option --sort-conc-studies is set, #p will be substituted by subdirectoryPathAndName
    //  - and in case option --sort-conc-studies is not set, #p will be substituted by opt_outputDirectory
    OFString dir = (opt_sortConcerningStudies == NULL) ? OFString(opt_outputDirectory) : subdirectoryPathAndName;
    cmd = replaceChars( cmd, OFString(PATH_PLACEHOLDER), dir );

    // perform substitution for placeholder #f; note that outputFileNameArray.back()
    // always contains the name of the file (without path) which was written last.
    OFString outputFileName = outputFileNameArray.back();
    cmd = replaceChars( cmd, OFString(FILENAME_PLACEHOLDER), outputFileName );
  }

  // perform substitution for placeholder #a
  cmd = replaceChars( cmd, OFString(CALLING_AETITLE_PLACEHOLDER), callingaetitle );

  // perform substitution for placeholder #c
  cmd = replaceChars( cmd, OFString(CALLED_AETITLE_PLACEHOLDER), calledaetitle );

  // Execute command in a new process
  executeCommand( cmd );
}



static void renameOnEndOfStudy()
    /*
     * This function deals with renaming the last study's output files. In detail, these file's
     * current filenames will be changed to a filename that corresponds to the pattern [modality-
     * prefix][consecutive-numbering]. The current filenames of all files that belong to the study
     * are captured in outputFileNameArray. The new filenames will be calculated whithin this
     * function: The [modality-prefix] will be taken from the old filename,
     * [consecutive-numbering] is a consecutively numbered, 6 digit number which will be calculated
     * starting from 000001.
     *
     * Parameters:
     *   none.
     */
{
  int counter = 1;

  OFListIterator(OFString) first = outputFileNameArray.begin();
  OFListIterator(OFString) last = outputFileNameArray.end();

  // before we deal with all the filenames which are included in the array, we need to distinguish
  // two different cases: If endOfStudyThroughTimeoutEvent is not true, the last filename in the array
  // refers to a file that belongs to a new study of which the first object was just received. (In this
  // case there are at least two filenames in the array). Then, this last filename is - at the end of the
  // following loop - not supposed to be deleted from the array. If endOfStudyThroughTimeoutEvent is true,
  // all filenames that are captured in the array, refer to files that belong to the same study. Hence,
  // all of these files shall be renamed and all of the filenames within the array shall be deleted.
  if( ! endOfStudyThroughTimeoutEvent ) --last;

  // rename all files that belong to the last study
  while (first != last)
  {
    // determine the new file name: The first two characters of the old file name make up the [modality-prefix].
    // The value for [consecutive-numbering] will be determined using the counter variable.
    char modalityId[3];
    char newFileName[9];
    if (opt_timeNames)
    {
      // modality prefix are the first 2 characters after serial number (if present)
      size_t serialPos = (*first).find("_");
      if (serialPos != OFString_npos)
      {
        //serial present: copy modality prefix (skip serial: 1 digit "_" + 4 digits serial + 1 digit ".")
        OFStandard::strlcpy( modalityId, (*first).substr(serialPos+6, 2).c_str(), 3 );
      }
      else
      {
        //serial not present, copy starts directly after first "." (skip 17 for timestamp, one for ".")
        OFStandard::strlcpy( modalityId, (*first).substr(18, 2).c_str(), 3 );
      }
    }
    else
    {
      OFStandard::strlcpy( modalityId, (*first).c_str(), 3 );
    }
    sprintf( newFileName, "%s%06d", modalityId, counter );

    // create two strings containing path and file name for
    // the current filename and the future filename
    OFString oldPathAndFileName;
    oldPathAndFileName = lastStudySubdirectoryPathAndName;
    oldPathAndFileName += PATH_SEPARATOR;
    oldPathAndFileName += *first;

    OFString newPathAndFileName;
    newPathAndFileName = lastStudySubdirectoryPathAndName;
    newPathAndFileName += PATH_SEPARATOR;
    newPathAndFileName += newFileName;

    // rename file
    if( rename( oldPathAndFileName.c_str(), newPathAndFileName.c_str() ) != 0 )
      fprintf( stderr, "storescp: Cannot rename file '%s' to '%s'.\n", oldPathAndFileName.c_str(), newPathAndFileName.c_str() );

    // remove entry from list
    first = outputFileNameArray.erase(first);

    // increase counter
    counter++;
  }
}



static void executeOnEndOfStudy()
    /*
     * This function deals with the execution of the command line which was passed
     * to option --exec-on-eostudy of the storescp. This command line is captured
     * in opt_execOnEndOfStudy. Note that the command line can contain the placeholder
     * PATH_PLACEHOLDER which needs to be substituted before the command line is actually executed.
     * In detail, PATH_PLACEHOLDER will be substituted by the path to the output directory into which
     * the files of the last study were written.
     *
     * Parameters:
     *   none.
     */
{
  OFString cmd = opt_execOnEndOfStudy;

  // perform substitution for placeholder #p; #p will be substituted by lastStudySubdirectoryPathAndName
  cmd = replaceChars( cmd, OFString(PATH_PLACEHOLDER), lastStudySubdirectoryPathAndName );

  // perform substitution for placeholder #a
  cmd = replaceChars( cmd, OFString(CALLING_AETITLE_PLACEHOLDER), callingaetitle );

  // perform substitution for placeholder #c
  cmd = replaceChars( cmd, OFString(CALLED_AETITLE_PLACEHOLDER), calledaetitle );

  // Execute command in a new process
  executeCommand( cmd );
}



static OFString replaceChars( const OFString &srcstr, const OFString &pattern, const OFString &substitute )
    /*
     * This function replaces all occurrences of pattern in srcstr with substitute and returns
     * the result as a new OFString variable. Note that srcstr itself will not be changed,
     *
     * Parameters:
     *   srcstr     - [in] The source string.
     *   pattern    - [in] The pattern string which shall be substituted.
     *   substitute - [in] The substitute for pattern in srcstr.
     */
{
  OFString result = srcstr;
  size_t pos = 0;

  while( pos != OFString_npos )
  {
    pos = result.find( pattern, pos );

    if( pos != OFString_npos )
    {
      result.replace( pos, pattern.size(), substitute );
      pos += substitute.size();
    }
  }

  return( result );
}



static void executeCommand( const OFString &cmd )
    /*
     * This function executes the given command line. The execution will be
     * performed in a new process which can be run in the background
     * so that it does not slow down the execution of storescp.
     *
     * Parameters:
     *   cmd - [in] The command which shall be executed.
     */
{
#ifdef HAVE_FORK
  pid_t pid = fork();
  if( pid < 0 )     // in case fork failed, dump an error message
    fprintf( stderr, "storescp: Cannot execute command '%s' (fork failed).\n", cmd.c_str() );
  else if (pid > 0)
  {
    /* we are the parent process */
    /* remove pending zombie child processes */
    cleanChildren(pid, OFTrue);
  }
  else // in case we are the child process, execute the command etc.
  {
    // execute command through execl will terminate the child process.
    // Since we only have a single command string and not a list of arguments,
    // we 'emulate' a call to system() by passing the command to /bin/sh
    // which hopefully exists on all Posix systems.

    if (execl( "/bin/sh", "/bin/sh", "-c", cmd.c_str(), NULL ) < 0)
      fprintf( stderr, "storescp: Cannot execute /bin/sh.\n" );

    // if execl succeeds, this part will not get executed.
    // if execl fails, there is not much we can do except bailing out.
    abort();
  }
#else
  PROCESS_INFORMATION procinfo;
  STARTUPINFO sinfo;
  OFBitmanipTemplate<char>::zeroMem((char *)&sinfo, sizeof(sinfo));
  sinfo.cb = sizeof(sinfo);

  // execute command (Attention: Do not pass DETACHED_PROCESS as sixth argument to the below
  // called function because in such a case the execution of batch-files is not going to work.)
  if( !CreateProcess(NULL, OFconst_cast(char *, cmd.c_str()), NULL, NULL, 0, 0, NULL, NULL, &sinfo, &procinfo) )
    fprintf( stderr, "storescp: Error while executing command '%s'.\n" , cmd.c_str() );

  if (opt_execSync)
  {
      // Wait until child process exits (makes execution synchronous).
      WaitForSingleObject(procinfo.hProcess, INFINITE);
  }

  // Close process and thread handles to avoid resource leak
  CloseHandle(procinfo.hProcess);
  CloseHandle(procinfo.hThread);
#endif
}

static void cleanChildren(pid_t pid, OFBool synch)
    /*
     * This function removes child processes that have terminated,
     * i.e. converted to zombies. Should be called now and then.
     */
{
#ifdef HAVE_WAITPID
    int stat_loc;
#elif HAVE_WAIT3
    struct rusage rusage;
#if defined(__NeXT__)
    /* some systems need a union wait as argument to wait3 */
    union wait status;
#else
    int        status;
#endif
#endif

#if defined(HAVE_WAITPID) || defined(HAVE_WAIT3)
    int child = 1;
    int options = synch ? 0 : WNOHANG;
    while (child > 0)
    {
#ifdef HAVE_WAITPID
        child = OFstatic_cast(int, waitpid(pid, &stat_loc, options));
#elif defined(HAVE_WAIT3)
        child = wait3(&status, options, &rusage);
#endif
        if (child < 0)
        {
           if (errno != ECHILD) CERR << "wait for child failed: " << strerror(errno) << endl;
        }

        if (synch) child = -1; // break out of loop
    }
#endif
}

static
DUL_PRESENTATIONCONTEXT *
findPresentationContextID(LST_HEAD * head,
                          T_ASC_PresentationContextID presentationContextID)
{
    DUL_PRESENTATIONCONTEXT *pc;
    LST_HEAD **l;
    OFBool found = OFFalse;

    if (head == NULL)
        return NULL;

    l = &head;
    if (*l == NULL)
        return NULL;

    pc = OFstatic_cast(DUL_PRESENTATIONCONTEXT *, LST_Head(l));
    (void)LST_Position(l, OFstatic_cast(LST_NODE *, pc));

    while (pc && !found) {
        if (pc->presentationContextID == presentationContextID) {
            found = OFTrue;
        } else {
            pc = OFstatic_cast(DUL_PRESENTATIONCONTEXT *, LST_Next(l));
        }
    }
    return pc;
}

/** accept all presenstation contexts for unknown SOP classes,
 *  i.e. UIDs appearing in the list of abstract syntaxes
 *  where no corresponding name is defined in the UID dictionary.
 *  @param params pointer to association parameters structure
 *  @param transferSyntax transfer syntax to accept
 *  @param acceptedRole SCU/SCP role to accept
 */
static OFCondition acceptUnknownContextsWithTransferSyntax(
    T_ASC_Parameters * params,
    const char* transferSyntax,
    T_ASC_SC_ROLE acceptedRole)
{
    OFCondition cond = EC_Normal;
    int n, i, k;
    DUL_PRESENTATIONCONTEXT *dpc;
    T_ASC_PresentationContext pc;
    OFBool accepted = OFFalse;
    OFBool abstractOK = OFFalse;

    n = ASC_countPresentationContexts(params);
    for (i = 0; i < n; i++)
    {
        cond = ASC_getPresentationContext(params, i, &pc);
        if (cond.bad()) return cond;
        abstractOK = OFFalse;
        accepted = OFFalse;

        if (dcmFindNameOfUID(pc.abstractSyntax) == NULL)
        {
            abstractOK = OFTrue;

            /* check the transfer syntax */
            for (k = 0; (k < OFstatic_cast(int, pc.transferSyntaxCount)) && !accepted; k++)
            {
                if (strcmp(pc.proposedTransferSyntaxes[k], transferSyntax) == 0)
                {
                    accepted = OFTrue;
                }
            }
        }

        if (accepted)
        {
            cond = ASC_acceptPresentationContext(
                params, pc.presentationContextID,
                transferSyntax, acceptedRole);
            if (cond.bad()) return cond;
        } else {
            T_ASC_P_ResultReason reason;

            /* do not refuse if already accepted */
            dpc = findPresentationContextID(
                              params->DULparams.acceptedPresentationContext,
                                            pc.presentationContextID);
            if ((dpc == NULL) ||
                ((dpc != NULL) && (dpc->result != ASC_P_ACCEPTANCE)))
            {

                if (abstractOK) {
                    reason = ASC_P_TRANSFERSYNTAXESNOTSUPPORTED;
                } else {
                    reason = ASC_P_ABSTRACTSYNTAXNOTSUPPORTED;
                }
                /*
                 * If previously this presentation context was refused
                 * because of bad transfer syntax let it stay that way.
                 */
                if ((dpc != NULL) &&
                    (dpc->result == ASC_P_TRANSFERSYNTAXESNOTSUPPORTED))
                    reason = ASC_P_TRANSFERSYNTAXESNOTSUPPORTED;

                cond = ASC_refusePresentationContext(params,
                                              pc.presentationContextID,
                                              reason);
                if (cond.bad()) return cond;
            }
        }
    }
    return EC_Normal;
}


/** accept all presenstation contexts for unknown SOP classes,
 *  i.e. UIDs appearing in the list of abstract syntaxes
 *  where no corresponding name is defined in the UID dictionary.
 *  This method is passed a list of "preferred" transfer syntaxes.
 *  @param params pointer to association parameters structure
 *  @param transferSyntax transfer syntax to accept
 *  @param acceptedRole SCU/SCP role to accept
 */
static OFCondition acceptUnknownContextsWithPreferredTransferSyntaxes(
    T_ASC_Parameters * params,
    const char* transferSyntaxes[], int transferSyntaxCount,
    T_ASC_SC_ROLE acceptedRole)
{
    OFCondition cond = EC_Normal;
    /*
    ** Accept in the order "least wanted" to "most wanted" transfer
    ** syntax.  Accepting a transfer syntax will override previously
    ** accepted transfer syntaxes.
    */
    for (int i=transferSyntaxCount-1; i>=0; i--)
    {
        cond = acceptUnknownContextsWithTransferSyntax(params, transferSyntaxes[i], acceptedRole);
        if (cond.bad()) return cond;
    }
    return cond;
}



}//end scope
