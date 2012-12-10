ALMA ReadMe
=============
This is Attention Laser Mechagodzilla Arm.


Ikaros
-----------------
ALMA runs within the Ikaros infrastructure, which means that you have to install all of Ikaros' dependencies in order to run ALMA. A complete manual of how to install Ikaros can be found at [Ikaros website](http://www.ikaros-project.org/installing/).


External Dependencies (Linux)
-----------------
To compile ALMA you also need the following libraries installed on your computer:
* `OpenCV-2.4.3`
* `openni-bin-dev-linux-x64-v1.5.4.0` (Unstable)
* `nite-bin-linux-x64-v1.5.2.21` (Unstable)
* `SensorKinect093-Bin-Linux-x64-v5.1.2.1` (Unstable)

OpenCV can be downloaded at http://opencv.org/
Extract the files in the archive, step into the extracted folder and run the following to compile and install OpenCV:

```
mkdir Build
cd Build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_PYTHON_SUPPORT=ON ..
make
sudo make install
```

OpenNI can be downloaded at [OpenNI.org](http://www.openni.org/Downloads/OpenNIModules.aspx). Choose `OpenNI Binaries`, `Unstable` and `OpenNI Unstable Build for Ubuntu 12.04 x64 (64-bit) v1.5.4.0` (if you're using a 64-bit system).

NITE can also be downloaded at [OpenNI.org](http://www.openni.org/Downloads/OpenNIModules.aspx). Choose `OpenNI Compliant Middleware Binaries`, `Unstable` and `PrimeSense NITE Unstable Build for Ubuntu 12.04 x64 (64-bit) v1.5.2.21` (if you're using a 64-bit system).

SensorKinect can be downloaded at [GitHub](https://github.com/avin2/SensorKinect/tree/unstable/Bin). Download the file named `SensorKinect093-Bin-Linux-x64-v5.1.2.1.tar.bz2` (if you're using a 64-bit system).

OpenNI, NITE and SensorKienct are all installed using the same procedure. Start by extracting the contents of the archives, step into the extracted folder and run:
```
sudo ./install.sh
```


Compiling ALMA (Linux)
-----------------
To compile ALMA, start by stepping into your `alma` directory and then run:
```
cd Build
cmake ..
make
```

Running ALMA (Linux)
-----------------
You should now be able to run ALMA.
```
cd ../Bin
./ikaros ./../Examples/alma/alma.ikc
```

If you would like a web GUI for ALMA you can add the `-w` flag after `./ikaros`. You might also have to run Ikaros as root to allow for it to use the Kinect and Dynamixel devices.
