cd config
nmake -f makefile.w32 %1
cd ..
cd src
nmake -f makefile.w32 %1
cd ..
