#include "OpenNi.h"

#include <XnCppWrapper.h>


xn::Context OpenNi::getContext()
{
    if (false == initialized) {
        niContext.Init();
    }
    
    return niContext;
}

//OpenNi OpenNi::getInstance()
//{
//    if (NULL == OpenNi::instance) {
//        OpenNi::instance = new OpenNi();
//    }
//    
//    return OpenNi::instance;
//}

