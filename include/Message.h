#ifndef _MESSAGE_H_
#define _MESSAGE_H_

class Message {
 public:

  enum MessageType {
    SHOW_VIEW,
    CREATE_FORMVIEW,
    CREATE_BUTTON,
    CREATE_LINEAR_LAYOUT,
    CREATE_OPENGL_VIEW,
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
    QUIT_APP,

    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE
  };

 Message(MessageType _message, int _elementId = 0, int _parentElementId = 0) : messageType(_message), elementId(_elementId), parentElementId(_parentElementId) { }
 Message(MessageType _message, const std::string & _textValue) : messageType(_message), elementId(0), parentElementId(0), textValue(_textValue) { }
 Message(MessageType _message, const std::string & _textValue, const std::string & _textValue2) : messageType(_message), elementId(0), parentElementId(0), textValue(_textValue), textValue2(_textValue2) { }
  ~Message() { }
  
  MessageType getType() const { return messageType; }
  int getElementId() const { return elementId; }
  int getParentElementId() const { return parentElementId; }
  
  int getValue() const { return value; }
  void setValue(int v) { value = v; }
  
  const std::string & getTextValue() const { return textValue; }
  void setTextValue(const std::string & s) { textValue = s; }
  
  const std::string & getKey() const { return key; }
  void setKey(const std::string & _key) { key = _key; }

 private:
  MessageType messageType;
  int elementId, parentElementId;
  int value;
  std::string key;
  std::string textValue, textValue2;
};

#endif
