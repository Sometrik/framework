# iOS Support

The iOS work will be a mixture of Objective-C, Objective-C++ and
C++. The goal is to receive commands from the C++ application and
create and update the user interface based on these commands. Also,
when the user interacts with the UI, events such as ValueChanged will
be created and sent to the C++ application. It is not necessary to
implement the communication between the the iOS component and the C++
application.

The idea is to built the UI dynamically, so no XIB files should be
used for any widget.

Most of the work is done using the class PlatformThread, from which,
the classes iOSThread and iOSMainThread will both be inherited
from. However, iOSThread is inherited from PosixThread, which is
inherited from PlatformThread.

Most important method in PlatformThread is _sendCommands()_, which
take an array of commands as parameters. These commands are used to
update the UI on the iOS side. The Command class has an internal_id
field accessed using the method getInternalId(), which specifies the UI
element, that the command is applied to. When creating new UI
elements, there is also a field child_internal_id accessed using
getChildInternalId(), that specifies the id for the new child
element. For example, a command CREATE_BUTTON would create a Button
with the specified child id and the button would be assigned as a
child to the specified element.

The Command class also has some other usefull fields such as text
value and integer value, which will be used as needed by the different
commands.

All UI widgets constructed on the iOS side should retain the integer
id, that was provided in the Command that was used to construct the
widget. It's also useful to have a dictionary / hash map, that can be
used to find a widget by its id.


# iOSMainThread and iOSThread

iOSThread represents a standard POSIX thread with iOS functionality added. iOSMainThread represents the UI thread.

In iOSThread the methods sendCommands(), setImageData() and
setSurface() simply package the parameters in some sort of iOS event,
that would be sent to the main thread for processing.


```
  virtual bool start() = 0;
  virtual bool testDestroy() = 0;
  virtual void sendCommands(const std::vector<Command> & commands) = 0;
  virtual void sleep(double t) = 0;
  virtual std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
  virtual std::unique_ptr<canvas::ContextFactory> createContextFactory() const = 0;
  virtual std::string loadTextAsset(const char * filename) = 0; 
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
  virtual void sendEvent(int internal_id, const Event & ev) = 0;
  virtual std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents(bool blocking = false) = 0;
  virtual void startEventLoop() = 0;
  virtual void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) = 0;
  virtual void setSurface(int internal_id, canvas::Surface & surface) = 0;
  virtual int startModal() = 0;
  virtual void endModal(int value) = 0;
  virtual void sendHeartbeat() { }
  virtual std::unique_ptr<Logger> createLogger(const std::string & name) const = 0;
  virtual std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) = 0;
  virtual void initializeThread();  
  virtual void deinitializeThread();
```

## Object hierarchy

Initially, an application object is created, which is the parent
object for all UI objects. Top level views are added as children to
the application. Only single top level view can be active at the same
time. There are three types of top level views: FrameView,
NavigationView and OpenGLView. OpenGLView is not required for this
project, and it has already been implemented for iOS. NavigationView
will be drawn on top of any other active content.

UI Widgets are added as children to the FrameView and NavigationView
as needed.

## Required commands

Command | Description
------- | -----------
CREATE_APPLICATION | Creates an application object which is a parent for all views
CREATE_FRAMEVIEW | Creates a simple top level view that can show multiple overlaid child views
CREATE_NAVIGATIONVIEW | Creates a drawer navigation layout or similar
CREATE_TEXTFIELD | Creates a single-line text input
CREATE_BUTTON | Creates a button
CREATE_LINEAR_LAYOUT | Creates a linear layout, either vertical or horizontal
CREATE_FRAME_LAYOUT | Creates a layout that can show multiple overlaid child views
CREATE_SCROLL_LAYOUT | Creates a scrollable layout
CREATE_TEXT | Creates a label
CREATE_DIALOG | Creates a dialog
CREATE_IMAGEVIEW | Creates an image 
CREATE_ACTION_SHEET | Creates an action sheet / popup menu
CREATE_ACTIONBAR | Creates an action bar / a bar for buttons that is at the top of the view
CREATE_NAVIGATIONBAR | Creates a navigation bar / a bar for buttons that is at the bottom of the view
CREATE_PROGRESSBAR | Creates a progress spinner
CREATE_TOAST | Creates a short notification message that hides itself after a specified duration
DELETE_ELEMENT | Deletes an element
REMOVE_CHILD | Removes a child
REORDER_CHILD | Modifies the order of the children by moving single child to a new position
CLEAR | Clears the content of an image, for example, and frees the memory
SET_INT_VALUE | ??
SET_TEXT_VALUE | Sets the value of text fields and labels
SET_VISIBILITY | Sets the visibility of an element
SET_STYLE | Sets a style specified by a key and a values
ADD_IMAGE_URL | Adds an URL and dimensions to a 
UPDATE_PREFERENCE | Updates user preferences specified by a key and a value
DELETE_PREFERENCE | Deletes an user preference
COMMIT_PREFERENCES | Stores user preferences
CREATE_TIMER | Creates a timer with a specified interval

## Styles

Style system is based on CSS and the requirements of Android
platform. No units should be added to the style values, and the
implicit unit is considered to be density independent pixel.

No inheritance is currently needed. All widgets are sent just the
styles that they need.

However, each widget has three sets of styles for different states:
NORMAL, ACTIVE, SELECTED. If the widget is is pressed, it goes to the
ACTIVE state, and when its a toggle button that is selected, it goes
to the selected state.

Style | Description
----- | -----------
padding | Padding
padding-top | Padding for the top side
padding-right | Padding for the right side
padding-bottom | Padding for the bottom side
padding-left | Padding for the left side
margin | Margin
margin-top | Margin for the top side
margin-right | Margin for the top side
margin-bottom | Margin for the top side
margin-left | Margin for the top side
weight | ?
opacity | Opacity [0..1]
shadow | Shadow
top | Top position
left | Left position
right | Right position
bottom | Bottom positon
width | Width in dip or wrap-content or match-parent
height | Height in dip or wrap-content or match-parent
background | ?
background-color | Background color (e.g. #ff0000)
color | Text color
gravity | ?
zoom | Zoom factor for the widget
border | Border color and/or width
border-radius | Top, right, bottom and left radii for rounded corners of images as layouts
font-size | Font size: small, medium, large or font size in dib
white-space | Controls text wrapping for labels: normal or nowrap
text-overflow | Controls text clipping for labels: empty or ellipsis
font-weight | Font weight: normal, bold or weight value
font-style | Font style: italic, oblique or normal
text-align | Text alignment: left, center or right
font-family | Font familiy (e.g. Arial)
icon-attachment | Icon attachment for buttons: top, right, bottom or left
icon | Icon filename for buttons or none (loaded from application assets)

## Events sent by the iOS framework component

Event | Description
----- | -----------
ValueEvent | Sent if a button or layout has been clicked or text field value changed
SysEvent | Application lifecycle events and out of memory warning
VisibilityEvent | Sent when the navigation layout becomes visible
ScrollChangedEvent | Sent by scroll layout when scrolled
ImageRequestEvent | Sent when an image needs data to show or has been deleted and no longer needs any
TimerEvent | Sent periodically when a configurable timer has been triggered

## Events sent by the Application

Subthreads can send events to the application using
iOSThread::sendEvent(). The event must be added to the iOS main event
queue, so that the application receives it eventually.

## Modal dialogs

Dialogs and action sheets are synchronous. They are modal by default,
and they will block the execution of the caller, until they return
with a value. Synchronus dialogs are implemented on iOS by starting a
new event loop in iOSMainThread::startModal(), which calls the following iOS method:

```
CFRunLoopRun()
```

When the dialog closes, iOSMainThread::endModal() is called, which then calls:

```
CFRunLoopStop(CFRunLoopGetCurrent());
```

## Bitmap data

Images can be initialized by supplying a file name, which is then
loaded from the application assets by the iOS component. Another way
to initialize image, is to specify a list of URLs and their
dimensions. In this case the iOS component picks the URL that has the
most suitable dimension, and requests its content by issuing an
ImageRequestEvent.

If no data is specified for an image, it simply sends an
ImageRequestEvent without an URL.

When the application is ready to provide bitmap data for the image, it
calls iOSMainThread.
