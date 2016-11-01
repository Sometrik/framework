#ifndef _MESSAGE_H_
#define _MESSAGE_H_

class Message {
 public:

  enum MessageType {

    CREATE_FORMVIEW,
    SHOW_VIEW,
    CREATE_BUTTON,
    CREATE_LINEAR_LAYOUT,
    CREATE_OPENGL_VIEW,
    CREATE_TEXTFIELD,
    CREATE_LABEL,
    CREATE_DIALOG, //For future
    CREATE_CANVAS,
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    LAUNCH_BROWSER,
    CREATE_NOTIFICATION,
    SET_VALUE, //Sets value of textfields.
    SET_LABEL, //this sets label for buttons and labels.
    SET_ATTRIBUTE

  };

  Message(MessageType _message, int _elementId = 0) : messageType(_message), elementId(_elementId){ }
  ~Message(){ }

  MessageType getType(){
    return messageType;
  }
  int getValue (){
    return value;
  }
  const std::string & getTextValue() {
    return textValue;
  }
  const std::string & getKey() { return key; }
  void setKey(const std::string & _key){ key = _key; }


 private:
  MessageType messageType;
  int elementId;
  int value;
  std::string key;
  std::string textValue;
};

#endif
