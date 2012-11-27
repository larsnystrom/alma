#ifndef OpenNi_
#define OpenNi_


#include <XnCppWrapper.h>

class OpenNi {
public:    

    static OpenNi& getInstance()
    {
        static OpenNi    instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
    
    xn::Context getContext();
    
private:
    OpenNi() {
        initialized = false;
    }
    
    ~OpenNi() {
        niContext.Release();
    }
    
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    OpenNi(OpenNi const&);              // Don't Implement
    void operator=(OpenNi const&); // Don't implement
    
    bool initialized;
    xn::Context         niContext;
};

#endif
