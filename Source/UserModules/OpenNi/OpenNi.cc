#include "OpenNi.h"

#include <XnCppWrapper.h>


xn::Context OpenNi::getContext()
{
    return niContext;
}

void OpenNi::StartGeneratingAll()
{
    if (false == started) {
        niRetVal = niContext.StartGeneratingAll();
        if (niRetVal != XN_STATUS_OK) {
	        printf("Failed starting generating all %s\n", xnGetStatusString(niRetVal));
        } else {
            started = true;
            printf("OpenNI: Start generating...\n");
        }
    }
}

