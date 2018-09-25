#include "iOSMainThread.h"

#include <ValueEvent.h>
#include <TimerEvent.h>
#include <ImageRequestEvent.h>
#include <FWViewBase.h>
#include <VisibilityEvent.h>
#include <ScrollChangedEvent.h>
#include <CommandEvent.h>
#include <UpdateEvent.h>
#include <DrawEvent.h>
#include <SysInfoEvent.h>

#include "iOSThread.h"

#import "NativeCommand.h"

#include <unordered_set>

using namespace std;

iOSMainThread::iOSMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable) : PosixThread(_application, _runnable) {
}

void
iOSMainThread::startEventLoop() {
    void * surface = 0;
    
    while (!isDestroyed && !exit_loop) {
      auto evs = pollEvents();

      bool redraw = false, update_sent = false;
      for (auto & ev : evs) {
        if (dynamic_cast<UpdateEvent*>(ev.second.get())) {
          if (update_sent) continue;
          update_sent = true;
        }

	Element::postEventToElement(ev.first, *ev.second.get());

        auto ev2 = dynamic_cast<SysEvent*>(ev.second.get());
        if (ev2) {
          if (ev2->getType() == SysInfoEvent::DESTROY) {
            exit_loop = true;
          } else if (ev2->getType() == SysInfoEvent::PAUSE) {
          }
        }

        if (ev.second.get()->isRedrawNeeded()) {
          redraw = true;
        }
      }

      if (canDraw && surface && redraw) {
        DrawEvent dev;
        postEvent(getApplication().getActiveViewId(), dev);

#if 0
        if (!eglSwapBuffers(display, surface)) {
          // Swap buffers failed
        }
#endif
      }

#if 0
      if (prev_heartbeat_time + 10 <= time(0)) {
        getApplication().showToast("Application is not responding: " + getApplication().getStatusText(), 9);
	sendHeartbeat();
      }
#endif
    }

    if (exit_loop > 0) exit_loop--;
  }

void
iOSMainThread::sendCommands(const std::vector<Command> & commands) {
  NSMutableArray * data = [[NSMutableArray alloc] init];
  for (auto & command : commands) {
    NativeCommand * nc = [[NativeCommand alloc] init];
    nc.type = (CommandType)command.getType();
    nc.internalId = command.getInternalId();
    nc.childInternalId = command.getChildInternalId();
    nc.value = command.getValue();
    nc.key = [NSString stringWithUTF8String:command.getKey().c_str()];
    nc.textValue = [NSString stringWithUTF8String:command.getTextValue().c_str()];
    nc.textValue2 = [NSString stringWithUTF8String:command.getTextValue2().c_str()];
    nc.row = command.getRow();
    nc.column = command.getColumn();
    nc.sheet = command.getSheet();
    nc.width = command.getWidth();
    nc.height = command.getHeight();
    nc.flags = command.getFlags();
    [data addObject:nc];
  }

  [viewController sendCommandsFromThread:data];
}

std::string
iOSMainThread::loadTextAsset(const char * filename) {
  NSString * filename2 = [NSString stringWithUTF8String:filename];
  NSString * path = [[NSBundle mainBundle] pathForResource:filename2 ofType:nil];
  NSString * content = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:NULL];
  string s = [content cStringUsingEncoding:NSUTF8StringEncoding];
  return s;
}

void
iOSMainThread::handleEventFromThread(int target_element_id, Event * event) {
    Element::postEventToElement(target_element_id, *event);
    delete event;
}

void
iOSMainThread::setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> input) {
  int bpp = input->getBytesPerPixel();
  int bitsPerComponent = 8;
  CGBitmapInfo bitmapInfo = 0;
  if (input->getInternalFormat() == canvas::RGBA4) {
    bitsPerComponent = 4;
    bitmapInfo |= kCGImageAlphaPremultipliedLast;
    bitmapInfo |= kCGBitmapByteOrder16Little;
  } else if (input->getInternalFormat() == canvas::RGB555 || input->getInternalFormat() == canvas::RGBA5551) {
    bitsPerComponent = 5;
    bitmapInfo |= kCGImageAlphaNoneSkipFirst;
    bitmapInfo |= kCGBitmapByteOrder16Little;
  } else if (input->getInternalFormat() == canvas::RGBA8) {
    bitmapInfo |= kCGImageAlphaPremultipliedFirst;
    bitmapInfo |= kCGBitmapByteOrder32Little;
  } else { // RGB8
    bitmapInfo |= kCGImageAlphaNoneSkipFirst;
    bitmapInfo |= kCGBitmapByteOrder32Little;
  }
  auto cfdata = CFDataCreate(0, input->getData(), input->getHeight() * input->getBytesPerRow());
  auto provider = CGDataProviderCreateWithCFData(cfdata);
  auto colorspace = CGColorSpaceCreateDeviceRGB();
  auto img = CGImageCreate(input->getWidth(), input->getHeight(), bitsPerComponent, bpp * 8, input->getBytesPerRow(), colorspace, bitmapInfo, provider, 0, true, kCGRenderingIntentDefault);

  if (img) { 
    [viewController setImageFromThread:internal_id data:img];
  } else {
    NSLog(@"Failed to decode image (w = %d, h = %d, f = %d", input->getWidth(), input->getHeight(), int(input->getInternalFormat()));
  }
  
  CGColorSpaceRelease(colorspace);
  CGDataProviderRelease(provider);
  CFRelease(cfdata);
}

void
iOSMainThread::setSurface(int internal_id, canvas::Surface & surface) {

}

int
iOSMainThread::startModal() {
  setModalResultValue(0);
  startEventLoop();
  return getModalResultValue();
}

void
iOSMainThread::endModal(int value) {
  setModalResultValue(value);
  exit_loop++;
}

std::shared_ptr<PlatformThread>
iOSMainThread::createThread(std::shared_ptr<Runnable> & runnable) {
  return std::make_shared<iOSThread>(application, runnable);
}

void
iOSMainThread::startDebugMode() {
  SysEvent ev(SysEvent::SHOW_DEBUG);
  sendEvent(getApplication().getInternalId(), ev);
}

#if 0
bool
iOSMainThread::back() {
  auto & app = getApplication();
  while ( 1 ) {
    int id = app.popViewBackHistory();
    if (!id) break;
    if (id == app.getActiveViewId()) continue;
	
    if (app.getActiveViewId()) {
      [viewController setVisibility:app.getActiveViewId() visibility:0];
    }
    app.setActiveViewId(id);

    [viewController setVisibility:app.getActiveViewId() visibility:1];

    string title;
    auto e = Element::getRegisteredElement(id);
    if (e) {
	auto e2 = dynamic_cast<FWViewBase*>(e);
	if (e2) title = e2->getTitle().c_str();
    }
    [viewController setTitle:[NSString stringWithUTF8String:title.c_str()]];

    return true;
  }
  return false;
}
#endif

void
iOSMainThread::sendIntValue(int viewId, int value) {
    ValueEvent ev(value);
    sendEvent(viewId, ev);
}

void
iOSMainThread::sendVisibilityEvent(int viewId, bool visibility) {
    VisibilityEvent ev(visibility);
    sendEvent(viewId, ev);
}

void
iOSMainThread::sendCommandEvent(int viewId, int elementId) {
    CommandEvent ev(elementId);
    sendEvent(viewId, ev);
}

void
iOSMainThread::sendScrollChangedEvent(int viewId, int scrollPos, int scrollRem, int height) {
    ScrollChangedEvent ev(scrollPos, scrollRem, height);
    sendEvent(viewId, ev);
}

void
iOSMainThread::sendTextValue(int viewId, const std::string & value) {
    ValueEvent ev(value);
    sendEvent(viewId, ev);
}

void
iOSMainThread::sendTimerEvent(int viewId) {
    TimerEvent ev(0);
    sendEvent(viewId, ev);
}

void
iOSMainThread::sendImageRequest(int viewId, unsigned int width, unsigned int height, const std::string & url, int internalFormat) {
    if (!url.empty()) {
        cerr << "sending image request, width = " << width << ", height = " << height << ", url = " << url << endl;
    }
    
    ImageRequestEvent ev(ImageRequestEvent::REQUEST, viewId, url, width, height);
    if (internalFormat != 0) {
      ev.setInternalFormat((canvas::InternalFormat)internalFormat);
    }
    sendEvent(viewId, ev);
}

void
iOSMainThread::cancelImageRequest(int viewId) {
    cerr << "cancelling image request\n";
  
    ImageRequestEvent ev(ImageRequestEvent::CANCEL, viewId);
    sendEvent(viewId, ev);
}
