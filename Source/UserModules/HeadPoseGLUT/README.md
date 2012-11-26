Ändringar i ikaros
-----------------
Jag har lagt till raden `link_directories(${MODULES_LIBS_DIR})` i ikaros CMakeLists.txt. Du behöver egentligen inte bry dig.

Installera OpenNI, OpenCV, SensorKinect och NITE.
-----------------
Jag använder 
* `OpenCV-2.4.3`
* `nite-bin-linux-x64-v1.5.2.21` (Unstable)
* `openni-bin-dev-linux-x64-v1.5.4.0` (Unstable)
* `SensorKinect093-Bin-Linux-x64-v5.1.2.1` (Unstable)

Du kanske måste ändra i HeadPose/CMakeLists.txt om du inte har installerat OpenNI och OpenCV på samma plats som jag.

Installera freeglut. 
-----------------
För att freeglut ska fungera och kompileras med ikaros behöver du installera följande:

    sudo apt-get install freeglut3-dev libxmu-dev libxi-dev

libxmu-dev och libxi-dev krävs för att kunna kompilera ikaros med freeglut. Det behövdes inte för den fristående head pose estimatorn och jag vet faktiskt inte riktigt varför vi behöver det nu. Men det löste mina problem.

Jag läste någonstans att build-essential behövdes för att kompilera freeglut. Jag installerade det, men det löste inte mitt dåvarande problem så jag är inte säker på om det behövs eller inte. Du installerar det med:

    sudo apt-get install build-essential


Ändra i HeadPose.cc
-----------------
För att slippa läsa in en konfigurationsfil hårdkodade jag konfigurationen för HeadPose. Det måste lösas bättre i framtiden, men just nu måste du ändra g_treepath i HeadPose::Init() så att det stämmer för ditt system.

Fixa OpenNIs plattforms-kod
-----------------
Det är en bugg i XnPlatform.h (/usr/include/ni/XnPlatform.h) som gör att Linux inte känns igen som operativsystem. Du måste ändra `linux` till `__linux__` och `i386` till `__i386__` för att OpenNI ska kompilera på Ubuntu. Jag ändrade så att berörda rader ser ut såhär:

    #elif (__linux__ && (__i386__ || __x86_64__))
        #include "Linux-x86/XnPlatformLinux-x86.h"
    #elif (__linux__ && __arm__)
        #include "Linux-Arm/XnPlatformLinux-Arm.h"

Källa på det: http://sourceforge.net/p/predef/wiki/Home/

Bygg och ikaros
-----------------
Därefter ska du kunna bygga ikaros med cmake och make.

    cd Build
    cmake ..
    make
    
Kör sedan ikaros med Examples/HeadPose/HeadPose.ikc

    cd ../Bin
    ./ikaros ./../Examples/HeadPose/HeadPose.ikc


