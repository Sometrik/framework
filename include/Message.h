#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

class Message {
 public:

  enum MessageType {
    CREATE_APPLICATION = 0,
    SHOW_VIEW,
    CREATE_FORMVIEW,
    CREATE_BUTTON,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_OPENGL_VIEW,
    CREATE_NATIVE_OPENGL_VIEW,
    CREATE_TEXTFIELD,
    CREATE_TEXTLABEL,
    CREATE_DIALOG, //For future
    CREATE_CANVAS,
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    LAUNCH_BROWSER,
    POST_NOTIFICATION,
    SET_VALUE, //Sets value of textfields.
    SET_LABEL, //this sets label for buttons and labels.
    SET_ATTRIBUTE,
    SET_CAPTION,
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

 Message(MessageType _message, int _internalId, int _childInternalId = 0) : messageType(_message), internalId(_internalId), childInternalId(_childInternalId) { }
 Message(MessageType _message, int _internalId, const std::string & _textValue) : messageType(_message), internalId(_internalId), childInternalId(0), textValue(_textValue) { }
 Message(MessageType _message, int _internalId, const std::string & _textValue, const std::string & _textValue2) : messageType(_message), internalId(_internalId), childInternalId(0), textValue(_textValue), textValue2(_textValue2) { }
  ~Message() { }
  
  MessageType getType() const { return messageType; }
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

 private:
  MessageType messageType;
  int internalId, childInternalId;
  int value;
  std::string key;
  std::string textValue, textValue2;
};

#endif
