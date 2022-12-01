mkdir ./build_tools/build
cd ./build_tools/build && cmake ../../ -G "Unix Makefiles" && cmake --build . && move ../3DGraphics ../.. && cd ../.. && ./3DGraphics