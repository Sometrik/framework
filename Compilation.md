# Compilation

Instructions for compiling example application Example1.

## iOS

Clone repositories _framework_, _canvas_ and _httpclient_. Create xCode project with the following files:

```
framework/example1/Example1.cpp
framework/src/DateTime.cpp
framework/src/StringUtils.cpp
framework/src/EventHandler.cpp
framework/src/Element.cpp
framework/src/Selection.cpp
framework/src/FWApplication.cpp
framework/src/Runnable.cpp
framework/src/PosixThread.cpp
framework/src/iOS/iOSFramework.cpp
canvas/src/Color.cpp
canvas/src/ContextQuartz2D.cpp
canvas/src/Path2D.cpp
canvas/src/Image.cpp
canvas/src/ImageData.cpp
canvas/src/PackedImageData.cpp
canvas/src/rg_etc1.cpp
canvas/src/dxt.cpp
canvas/src/FloydSteinberg.cpp
httpclient/src/URI.cpp
httpclient/src/FNV.cpp
httpclient/src/OAuth.cpp
httpclient/src/Base64.cpp
httpclient/src/SHA1.cpp
httpclient/src/iOSClient.mm
```

Add the following include directories:

```
framework/include
canvas/include
httpclient/include
```

Possibly, add the following libraries:
```
libiconv
libz ??
```
Compile.
