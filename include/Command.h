#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>

#define COMMAND_FLAG_PASSWORD 1

class Command {
 public:
  enum Type {
    CREATE_APPLICATION = 0,
    CREATE_FRAMEVIEW,
    CREATE_NAVIGATIONVIEW,
    CREATE_OPENGL_VIEW,
    CREATE_TEXTFIELD, // For viewing single value
    CREATE_TEXTVIEW, // For viewing multiline text
    CREATE_LISTVIEW, // For viewing lists
    CREATE_GRIDVIEW, // For viewing tables
    CREATE_BUTTON,
    CREATE_SWITCH,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_FRAME_LAYOUT,
    CREATE_EVENT_LAYOUT,
    CREATE_SCROLL_LAYOUT,
    CREATE_FLIPPER_LAYOUT,
    CREATE_TABLE_LAYOUT,
    CREATE_AUTO_COLUMN_LAYOUT,
    CREATE_TEXT,
    CREATE_LINK,
    CREATE_DIALOG,
    CREATE_ALERT_DIALOG,
    CREATE_IMAGEVIEW,
    CREATE_ACTION_SHEET,
    CREATE_PAGER,
    CREATE_CHECKBOX,
    CREATE_RADIO_GROUP,
    CREATE_SEPARATOR,
    CREATE_SLIDER,
    CREATE_ACTIONBAR,
    CREATE_SEARCHBAR,
    CREATE_NAVIGATIONBAR,
    CREATE_NAVIGATIONBAR_ITEM,
    CREATE_PROGRESS_SPINNER,
    CREATE_TOAST,
    CREATE_NOTIFICATION,
    CREATE_PAGE_CONTROL,
    CREATE_SEGMENTED_CONTROL,
    DELETE_ELEMENT,
    REMOVE_CHILD,
    REORDER_CHILD,
    LAUNCH_BROWSER,
    HISTORY_GO_BACK,
    CLEAR, // Clears the contents of GridView
    RELEASE,
    SET_INT_VALUE, // Sets value of radio groups, checkboxes and pickers
    SET_TEXT_VALUE, // Sets value of textfields and labels
    SET_INT_DATA,
    SET_TEXT_DATA, // Sets the cell value of GridView
    SET_VISIBILITY,
    SET_SHAPE, // Specifies the number of rows and columns in a GridView
    SET_STYLE,
    SET_ERROR,
    ADD_IMAGE_URL,
    FLUSH_VIEW, // Flushes GridView content
    UPDATE_PREFERENCE,
    DELETE_PREFERENCE,
    COMMIT_PREFERENCES,
    ADD_OPTION,
    ADD_SHEET,
    ADD_COLUMN,
    RESHAPE_TABLE,
    RESHAPE_SHEET,
    STOP,
    QUIT_APP,

    // Timers
    CREATE_TIMER,
    
    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE,

    // Other
    SHARE_LINK,
    SELECT_FROM_GALLERY
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
