mkdir -p ../double_benchmark/dylibs/Menci
gcc double.c double_helpers.c int128.c util.c dylib.c -o ../double_benchmark/dylibs/Menci/dylib32.so -shared -fPIC -m32 -O2
gcc double.c double_helpers.c int128.c util.c dylib.c -o ../double_benchmark/dylibs/Menci/dylib64.so -shared -fPIC -m64 -O2
