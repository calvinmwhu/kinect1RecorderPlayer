#include "comutilities.h"

//void ComUtilities::COM_RESULT_CHECK(HRESULT ret) {
//    if (ret != S_OK) {
//        std::string error = TO_STRING(ret);
//        throw std::runtime_error(error.c_str());
//    }
//}

std::string ComUtilities::TO_STRING(HRESULT ret) {
    _com_error err(ret);
    LPCTSTR errMsg = err.Description();
    std::string result = CT2A(errMsg);
    return result;
}

//#define TO_STRING( ret )  \
//    _com_error err(ret);   \
//    LPCTSTR errMsg = err.ErrorMessage();  \
//    std::string s = CT2A( lpctstr );    \
//    return s;
