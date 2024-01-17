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
compression levels in zstd can be negative, but we cant specify a negative here, so anything over 22 has 23 substracted from it, else multiply by -1, the following illustration should explain how to set dynamicCompressionLevel & staticCompressionLevel
```
(lowest)    (middle)      (highest)
1 2 3 ... 22 (0 23) 24 ... 43 44 45
```
```xml
<httpCompression>
    ...
    <scheme name="zstd" dll="%ProgramFiles%\path\to\zstd.dll" dynamicCompressionLevel="30" staticCompressionLevel="35" />
    ...
</httpCompression>
```

## Todo
 - Improve this readme
 - A better solution for negative compression levels?

Thanks to [@saucecontrol](https://github.com/saucecontrol) for Brotli-IIS being a great starting point at getting this done
