#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>

#define COMMAND_FLAG_PASSWORD 1

class Command {
 public:
  enum Type {
    CREATE_APPLICATION = 0,
    CREATE_BASICVIEW,
    CREATE_FORMVIEW,
    CREATE_NAVIGATIONVIEW,
    CREATE_OPENGL_VIEW,
    CREATE_TEXTFIELD, // For viewing single value
    CREATE_TEXTVIEW, // For viewing multiline text
    CREATE_LISTVIEW, // For viewing lists
    CREATE_SIMPLELISTVIEW,
    CREATE_GRIDVIEW, // For viewing tables
    CREATE_BUTTON,
    CREATE_SWITCH,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_RELATIVE_LAYOUT,
    CREATE_TABLE_LAYOUT,
    CREATE_AUTO_COLUMN_LAYOUT,
    CREATE_PANEL,
    CREATE_TEXT,
    CREATE_DIALOG, // For future
    CREATE_IMAGEVIEW,
    CREATE_ACTION_SHEET,
    CREATE_CHECKBOX,
    CREATE_RADIO_GROUP,
    CREATE_SEPARATOR,
    CREATE_SLIDER,
    CREATE_ACTIONBAR,
    CREATE_NAVIGATIONBAR,
    CREATE_TOAST,
    DELETE_ELEMENT,
    END_MODAL,
    SHOW_DIALOG,
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    SHOW_ACTION_SHEET,
    LAUNCH_BROWSER,
    HISTORY_GO_BACK,
    HISTORY_GO_FORWARD,
    CLEAR, // Clears the contents of GridView
    SET_INT_VALUE, // Sets value of radio groups, checkboxes and pickers
    SET_TEXT_VALUE, // Sets value of textfields and labels
    SET_INT_DATA,
    SET_TEXT_DATA, // Sets the cell value of GridView
    SET_LABEL, // Sets label for buttons and checkboxes
    SET_ENABLED,
    SET_READONLY,
    SET_VISIBILITY,
    SET_SHAPE, // Specifies the number of rows and columns in a GridView
    SET_STYLE,
    SET_ERROR,
    SET_IMAGE,
    FLUSH_VIEW, // Flushes GridView content
    UPDATE_PREFERENCE,
    COMMIT_PREFERENCES,
    ADD_OPTION,
    ADD_SHEET,
    ADD_COLUMN,
    RESHAPE_TABLE,
    RESHAPE_SHEET,
    QUIT_APP,

    // Timers
    CREATE_TIMER,
    
    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE
  };

 Command(Type _command, int _internalId, int _childInternalId = 0) : commandType(_command), internalId(_internalId), childInternalId(_childInternalId) { }
 Command(Type _command, int _internalId, const std::string & _textValue) : commandType(_command), internalId(_internalId), childInternalId(0), textValue(_textValue) { }
 Command(Type _command, int _internalId, const std::string & _textValue, const std::string & _textValue2) : commandType(_command), internalId(_internalId), childInternalId(0), textValue(_textValue), textValue2(_textValue2) { }
  
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

  void setWidth(int _width) { width = _width; }
  int getWidth() const { return width; }
  void setHeight(int _height) { height = _height; }
  int getHeight() const { return height; }

  void setRow(int r) { row = r; }
  int getRow() const { return row; }

  void setColumn(int c) { column = c; }
  int getColumn() const { return column; }
  
  void setSheet(unsigned short s) { sheet = s; }
  unsigned short getSheet() const { return sheet; }

  void setFlags(unsigned int _flags) { flags = _flags; }
  unsigned int getFlags() const { return flags; }

 private:
  Type commandType;
  int internalId, childInternalId;
  int value = 0;
  std::string key;
  std::string textValue, textValue2;
  int row = 0, column = 0;
  unsigned short sheet = 0;
  int width = 0, height = 0;
  unsigned int flags = 0;
};

#endif
