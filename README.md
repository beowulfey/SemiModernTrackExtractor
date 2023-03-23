2023-03-23

###Getting TrackExtractor to work on modern computers. 
The big issue was finding the right version of the dependencies, and compiling *those* for use on 64-bit machines. These are:

* OpenCV
* YAML-CPP

After some effort, I determined that it uses OpenCV ~1.0 (the source code for which is not on SourceForge), and YAML ~0.2.3 or so. Based on the date in the source files... (circa 2008-2009)

To get the OpenCV 1.0 source, including the build files for Windows:

1. visit http://opencvlibrary.cvs.sourceforge.net/viewvc/opencvlibrary/
2. Using CVS, checkout the source code: 
``` cvs -z3 -d:pserver:anonymous@a.cvs.sourceforge.net:/cvsroot/opencvlibrary co -D 2008/10/25 -P opencv ```
3. Open the resulting sln file in Visual Studio. VS will convert it to the modern version. 


To get YAML-CPP: 
1. visit https://github.com/jbeder/yaml-cpp/releases/tag/release-0.2.3 (from Oct 22, 2009)
2. Download the zip