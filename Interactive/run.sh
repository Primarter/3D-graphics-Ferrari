mkdir ./build_tools/build
cd ./build_tools/build && cmake ../../ -G "Unix Makefiles" && cmake --build . && move ../DingusParty ../.. && cd ../.. && ./DingusParty