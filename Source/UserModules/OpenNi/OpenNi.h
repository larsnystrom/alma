#ifndef OpenNi_
#define OpenNi_

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "./../Source/UserModules/OpenNi/SamplesConfig.xml"

#include <XnCppWrapper.h>
#include <libgen.h> 

class OpenNi {
public:    

    static OpenNi& getInstance()
    {
        static OpenNi    instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
    
    xn::Context getContext();
    void        StartGeneratingAll();
    
private:
    OpenNi() {
	    xn::EnumerationErrors	errors;
        niRetVal = XN_STATUS_OK;
        
//        const char* file = __FILE__;
//        char* dFile = strdup(file);
//        dFile = strcat(dirname(dFile), "/SamplesConfig.xml");
        
	    niRetVal = niContext.InitFromXmlFile(SAMPLE_XML_PATH, niScriptNode, &errors);
	    if (niRetVal == XN_STATUS_NO_NODE_PRESENT)
	    {
		    XnChar strError[1024];
		    errors.ToString(strError, 1024);
		    printf("%s\n", strError);
//    		return (niRetVal);
	    }
	    
//        niContext.Init();
        printf("\nOpenNI: Context setup...\n");
    }
    
    ~OpenNi() {
        niContext.Release();
        printf("\nOpenNI: Context shutdown.\n\n");
    }
    
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    OpenNi(OpenNi const&);              // Don't Implement
    void operator=(OpenNi const&); // Don't implement
    
    xn::Context         niContext;
    bool                started;
    XnStatus            niRetVal;
    xn::ScriptNode	    niScriptNode;
};

#endif
