# Compilation

Instructions for compiling example application Example1.

## iOS

Clone repositories _framework_, _canvas_ and _httpclient_. Create xCode project with the following files:

```
framework/example1/Example1.cpp
framework/example1/ImageLoader.cpp
framework/src/DateTime.cpp
framework/src/StringUtils.cpp
framework/src/EventHandler.cpp
framework/src/Element.cpp
framework/src/Selection.cpp
framework/src/FWApplication.cpp
framework/src/Runnable.cpp
framework/src/PosixThread.cpp
framework/src/iOS/ViewController.mm
framework/src/iOS/AppDelegate.m
framework/src/iOS/FWButton.m
framework/src/iOS/NativeCommand.m
framework/src/iOS/FWImageView.m
framework/src/iOS/InAppPurchaseManager.m
framework/src/iOS/FWScrollView.m
framework/src/iOS/FWPicker.m
framework/src/iOS/ViewManager.m
framework/src/iOS/ViewStyle.m
framework/src/iOS/FrameLayoutView.m
framework/src/iOS/LayoutParams.m
framework/src/iOS/PaddedLabel.m
framework/src/iOS/LinearLayoutView.m
framework/src/iOS/FWImage.m
framework/src/iOS/ImageWrapper.m
framework/src/iOS/EventWrapper.m
framework/src/iOS/iOSMainThread.mm
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
httpclient/src/iOSCFClient.mm
```

Add the following include directories (in the project settings: Build Settings / Search Paths / Header Search paths):

```
framework/include
canvas/include
httpclient/include
```

Add the following frameworks / libraries:

```
ImageIO.framework
```

Compile.
