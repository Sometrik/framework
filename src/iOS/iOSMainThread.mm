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

#include "iOSThread.h"

#import "EventWrapper.h"
#import "NativeCommand.h"

#include <unordered_set>

using namespace std;

iOSMainThread::iOSMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable) : PosixThread(0, _application, _runnable) {
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
          if (ev2->getType() == SysEvent::DESTROY) {
            exit_loop = true;
          } else if (ev2->getType() == SysEvent::PAUSE) {
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
    if (command.getType() == Command::CREATE_FRAMEVIEW || command.getType() == Command::CREATE_OPENGL_VIEW) {
      auto & app = getApplication();
      if (!app.getActiveViewId()) {
        app.setActiveViewId(command.getChildInternalId());
      }

      if (command.getType() == Command::SET_INT_VALUE && command.getInternalId() == 1 && command.getChildInternalId() != app.getActiveViewId()) {
	if (app.getActiveViewId()) {
	  app.addToHistory(app.getActiveViewId());
	}
	app.setActiveViewId(command.getChildInternalId());
      }
    }

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

#ifdef USE_THREAD
  [viewController sendCommandsFromThread:data];
#else
  [viewController handleCommands:data];
#endif
}

std::string
iOSMainThread::loadTextAsset(const char * filename) {
  NSString * filename2 = [NSString stringWithUTF8String:filename];
  NSString * path = [[NSBundle mainBundle] pathForResource:filename2 ofType:nil];
  NSString * content = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:NULL];
  string s = [content cStringUsingEncoding:NSUTF8StringEncoding];
  return s;
}

#ifndef USE_THREAD
void
iOSMainThread::sendEvent(int internal_id, const Event & ev) {
    EventWrapper * ew = [[EventWrapper alloc] init];
    ew.targetElementId = internal_id;
    ew.eventPtr = ev.dup();
    [viewController sendEventToMainThread:ew];
}
#endif

void
iOSMainThread::handleEventFromThread(int target_element_id, Event * event) {
    Element::postEventToElement(target_element_id, *event);
    delete event;
}

void
iOSMainThread::setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> input) {
  int bpp = input->getBytesPerPixel(input->getInternalFormat());
  auto cfdata = CFDataCreate(0, input->getData(), bpp * input->getWidth() * input->getHeight());
  auto provider = CGDataProviderCreateWithCFData(cfdata);
  auto colorspace = CGColorSpaceCreateDeviceRGB();
  CGBitmapInfo bitmapInfo = kCGBitmapByteOrder32Little;
  if (input->getInternalFormat() == canvas::RGBA8) {
    bitmapInfo |= kCGImageAlphaPremultipliedFirst;
  } else {
    bitmapInfo |= kCGImageAlphaNoneSkipFirst;
  }
  auto img = CGImageCreate(input->getWidth(), input->getHeight(), 8, bpp * 8, bpp * input->getWidth(), colorspace, bitmapInfo, provider, 0, true, kCGRenderingIntentDefault);

  [viewController setImageFromThread:internal_id data:img];

  CGColorSpaceRelease(colorspace);
  CGDataProviderRelease(provider);
  CFRelease(cfdata);
}

void
iOSMainThread::setSurface(int internal_id, canvas::Surface & surface) {

}

int
iOSMainThread::startModal() {
#ifdef USE_THREAD
  setModalResultValue(0);
  startEventLoop();
#else
  CFRunLoopRun();
#endif
  return getModalResultValue();
}

void
iOSMainThread::endModal(int value) {
  setModalResultValue(value);
#ifdef USE_THREAD
  exit_loop++;
#else
  CFRunLoopStop(CFRunLoopGetCurrent());
#endif
}

std::shared_ptr<PlatformThread>
iOSMainThread::createThread(std::shared_ptr<Runnable> & runnable) {
  return std::make_shared<iOSThread>(this, application, runnable);
}

void
iOSMainThread::startDebugMode() {
  SysEvent ev(SysEvent::SHOW_DEBUG);
#if 1
  sendEvent(getApplication().getInternalId(), ev);
#else
  Element::postEventToElement(getApplication().getInternalId(), ev);
#endif
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
#if 1
    sendEvent(viewId, ev);
#else
    Element::postEventToElement(viewId, ev);
#endif
}

void
iOSMainThread::sendVisibilityEvent(int viewId, bool visibility) {
    VisibilityEvent ev(visibility);
#if 1
    sendEvent(viewId, ev);
#else
    Element::postEventToElement(viewId, ev);
#endif
}

void
iOSMainThread::sendCommandEvent(int viewId, int elementId) {
    CommandEvent ev(elementId);
#if 1
    sendEvent(viewId, ev);
#else
    Element::postEventToElement(viewId, ev);
#endif
}

void
iOSMainThread::sendScrollChangedEvent(int viewId, int scrollPos, int scrollRem, int height) {
    ScrollChangedEvent ev(scrollPos, scrollRem, height);
#if 1
    sendEvent(viewId, ev);
#else
    Element::postEventToElement(viewId, ev);
#endif
}

void
iOSMainThread::sendTextValue(int viewId, const std::string & value) {
    ValueEvent ev(value);
#if 1
    sendEvent(viewId, ev);
#else
    Element::postEventToElement(viewId, ev);
#endif
}

void
iOSMainThread::sendTimerEvent(int viewId) {
    TimerEvent ev(0);
#if 1
    sendEvent(viewId, ev);
#else
    Element::postEventToElement(viewId, ev);
#endif
}

void
iOSMainThread::sendImageRequest(int viewId, unsigned int width, unsigned int height, const std::string & url, int internalFormat) {
    cerr << "sending image request, width = " << width << ", height = " << height << ", url = " << url << endl;
  
    float scale = getDisplayScale();
    if (scale != 1) {
        width = (unsigned int)(width * scale);
        height = (unsigned int)(height * scale);
    }
  
    ImageRequestEvent ev(ImageRequestEvent::REQUEST, viewId, url, width, height);
    if (internalFormat != 0) {
      ev.setInternalFormat((canvas::InternalFormat)internalFormat);
    }
    sendEvent(getApplication().getInternalId(), ev);
}
