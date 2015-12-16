#ifndef COMUTILITIES
#define COMUTILITIES
#include <sstream>
#include <fstream>
#include <string>
#include <comdef.h>
#include <AtlConv.h>


#define TO_STRING( ret )  (std::string(CT2A(_com_error(ret).ErrorMessage())))


#define COM_RESULT_CHECK( ret )  \
    if ( ret != S_OK ) {    \
    std::stringstream ss;	\
    ss << "failed " #ret " " << std::hex << ret << std::endl;	\
    throw std::runtime_error( ss.str().c_str() );			\
    }

#endif // COMUTILITIES

