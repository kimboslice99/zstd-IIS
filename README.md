zstd IIS Compression Scheme Plugin
==================================

## Build 
```
git clone https://github.com/kimboslice99/zstd-IIS.git
cd zstd-IIS
git submodule init
git submodule update
pushd zstd\build\cmake
cmake .
msbuild zstd.sln -target:libzstd_static:Rebuild /p:Configuration=Release
popd
cd src
msbuild zstdIIS.vcxproj /p:Configuration=Release;Platform=x64
```
## Setup
compression levels in zstd can be negative, but we cant specify a negative here, so anything up to 100 is a negative range and anything over 100 is a positive range
```
(fastest)      (middle)      (slowest)
-5 -4 ... -1 101 ... 120 121 122 <-- dont use these extreme compression levels, they *will* cause issues
```
From zstd manual
```
value 0 means default, which is controlled by ZSTD_CLEVEL_DEFAULT.
Default level is ZSTD_CLEVEL_DEFAULT==3
```
```xml
<httpCompression>
    ...
    <scheme name="zstd" dll="%ProgramFiles%\path\to\zstd.dll" dynamicCompressionLevel="0" staticCompressionLevel="106" />
    ...
</httpCompression>
```

## Todo
 - Improve this readme
 - Make it faster? Brotli is still more efficient, according to my limited testing

Thanks to [@saucecontrol](https://github.com/saucecontrol) for Brotli-IIS being a great starting point at getting this done
