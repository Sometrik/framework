#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>

#define COMMAND_FLAG_PASSWORD 1

class Command {
 public:

  enum Type {
    CREATE_PLATFORM = 0,
    CREATE_APPLICATION,
    SHOW_VIEW,
    CREATE_FORMVIEW,
    CREATE_BUTTON,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_NATIVE_OPENGL_VIEW,
    CREATE_TEXTFIELD,
    CREATE_TEXTLABEL,
    CREATE_DIALOG, // For future
    CREATE_IMAGE_ELEMENT,
    CREATE_ACTION_SHEET,
    DELETE_ELEMENT,
    REQUEST_REDRAW,
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    SHOW_ACTION_SHEET,
    LAUNCH_BROWSER,
    POST_NOTIFICATION,
    SET_VALUE, //Sets value of textfields.
    SET_LABEL, //this sets label for buttons and labels.
    SET_ATTRIBUTE,
    SET_CAPTION,
    UPDATE_PREFERENCE,
    ADD_OPTION,
    QUIT_APP,

    // Timers
    CREATE_TIMER,
    DELETE_TIMER,
    
    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE
  };

 Command(Type _command, int _internalId, int _childInternalId = 0) : commandType(_command), internalId(_internalId), childInternalId(_childInternalId) { }
 Command(Type _command, int _internalId, const std::string & _textValue) : commandType(_command), internalId(_internalId), childInternalId(0), textValue(_textValue) { }
 Command(Type _command, int _internalId, const std::string & _textValue, const std::string & _textValue2) : commandType(_command), internalId(_internalId), childInternalId(0), textValue(_textValue), textValue2(_textValue2) { }
  ~Command() { }
  
  Type getType() const { return commandType; }
  int getInternalId() const { return internalId; }
  int getChildInternalId() const { return childInternalId; }
  
  int getValue() const { return value; }
  void setValue(int v) { value = v; }
  
  const std::string & getTextValue() const { return textValue; }
  void setTextValue(const std::string & s) { textValue = s; }
  const std::string & getTextValue2() const { return textValue2; }
  void setTextValue2(const std::string & s) { textValue2 = s; }
  
  const std::string & getKey() const { return key; }
  void setKey(const std::string & _key) { key = _key; }

  void setWidth(int _width){ width = _width; }
  int getWidth(){ return width; }
  void setHeight(int _height) { height = _height; }
  int getHeight() { return height; }

  void setFlags(unsigned int _flags) { flags = _flags; }
  unsigned int getFlags() const { return flags; }

 private:
  Type commandType;
  int internalId, childInternalId;
  int value;
  std::string key;
  std::string textValue, textValue2;
  int width = 0, height = 0;
  unsigned int flags = 0;
};

#endif
