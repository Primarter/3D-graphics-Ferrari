mkdir .\build_tools\build
cd .\build_tools\build && cmake ..\..\ -G "MinGW Makefiles" && cmake --build . && move ..\3DGraphics.exe ..\.. && cd ..\.. && .\3DGraphics.exe