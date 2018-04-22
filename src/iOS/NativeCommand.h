#import <Foundation/NSObject.h>

typedef enum {
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
    CREATE_NAVIGATIONBAR,
    CREATE_NAVIGATIONBAR_ITEM,
    CREATE_PROGRESS_SPINNER,
    CREATE_TOAST,
    CREATE_NOTIFICATION,
    CREATE_PAGE_CONTROL,
    DELETE_ELEMENT,
    REMOVE_CHILD,
    REORDER_CHILD,
    LAUNCH_BROWSER,
    HISTORY_GO_BACK,
    HISTORY_GO_FORWARD,
    CLEAR, // Clears the contents of GridView
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
    SET_BACK_BUTTON_VISIBILITY,
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
} CommandType;

@interface NativeCommand : NSObject
@property (nonatomic) CommandType type;
@property (nonatomic) int internalId;
@property (nonatomic) int childInternalId;
@property (nonatomic) int value;
@property (nonatomic, strong) NSString * key;
@property (nonatomic, strong) NSString * textValue;
@property (nonatomic, strong) NSString * textValue2;
@property (nonatomic) int row;
@property (nonatomic) int column;
@property (nonatomic) int sheet;
@property (nonatomic) int width;
@property (nonatomic) int height;
@property (nonatomic) int flags;
@end
