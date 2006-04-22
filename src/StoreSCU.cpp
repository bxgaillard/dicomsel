#ifdef _WIN32
# undef HAVE_CONFIG_H
#endif // _WIN32

#define opt_acse_timeout  storescu_opt_acse_timeout
#define opt_dimse_timeout storescu_opt_dimse_timeout
#define opt_blockMode     storescu_opt_blockMode
#define main              storescu_main
extern "C" static int main( int argc, char *argv[] );
#include "dcmtkapp/storescu.cc"
#undef main

#include <dicomsel/StoreSCU.h>

namespace dicomsel
{

int StoreSCU::Main( int argc, char *argv[] )
{
    return storescu_main( argc, argv );
}

} // namespace dicomsel
