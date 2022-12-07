mkdir .\build_tools\build
cd .\build_tools\build && cmake ..\..\ -G "MinGW Makefiles" && cmake --build . && move ..\DingusParty.exe ..\.. && cd ..\..