#include <FWPlatform.h>
#include <CurlClient.h>
#include <Logger.h>
#include <ContextCairo.h>
#include <SDLSoundCanvas.h>
#include <Command.h>
#include <FWApplication.h>

#include <OpenGLInitEvent.h>
#include <TouchEvent.h>
#include <DrawEvent.h>
#include <SysEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <TimerEvent.h>
#include <ValueEvent.h>
#include <CommandEvent.h>

#include <gtk/gtk.h>

#include <unordered_map>
#include <cstdio>
#include <cstdlib>
#include <cassert>

using namespace std;

extern FWApplication * applicationMain();

static int width = 800, height = 600;

class PlatformGtk : public FWPlatform {
public:
  PlatformGtk() : FWPlatform(1.0f) {
    CurlClientFactory::globalInit();
    sendCommand2(Command(Command::CREATE_PLATFORM, getParentInternalId(), getInternalId()));
  }

  void pushEvent(const Event & ev) override {
    assert(0);
  }
  
  string getLocalFilename(const char * fn, FileType type) override {
    string s = "assets/";
    return s + fn;
  }
  
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }
    
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }

  void sendCommand2(const Command & command) override {
    if (!getActiveViewId() && (command.getType() == Command::CREATE_FORMVIEW || command.getType() == Command::CREATE_OPENGL_VIEW)) {
      setActiveViewId(command.getChildInternalId());
    }
    
    switch (command.getType()) {
    case Command::CREATE_PLATFORM:
      cerr << "ignore CREATE_PLATFORM\n";
      break;
      
    case Command::CREATE_APPLICATION: {
      cerr << "creating stack\n";
      stack = gtk_stack_new();
      gtk_stack_set_transition_type((GtkStack*)stack, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
      // ignore parent id
      addView(0, command.getChildInternalId(), stack);
      assert(window);
      gtk_container_add(GTK_CONTAINER(window), stack);
    }
      break;
      
    case Command::CREATE_TEXTFIELD: {
      auto textfield = gtk_entry_new();
      g_signal_connect(textfield, "changed", G_CALLBACK(send_text_value), this);
      addView(command, textfield);
    }
      break;

    case Command::CREATE_TEXTVIEW: {
      auto textview = gtk_text_view_new();
      addView(command, textview);
    }
      break;

    case Command::CREATE_GRIDVIEW: {
      auto store = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
      auto gridview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
      addView(command, gridview);
    }
      break;

    case Command::ADD_COLUMN: {
      auto view = views_by_id[command.getInternalId()];
      if (view) {
	// auto column = gtk_tree_view_column_new();
	// gtk_tree_view_column_set_title((GtkTreeViewColumn*)column, command.getTextValue().c_str());
	// int i = gtk_tree_view_append_column((GtkTreeView*)view, column);
	int i = gtk_tree_view_get_n_columns((GtkTreeView*)view);
	gtk_tree_view_insert_column_with_attributes((GtkTreeView*)view,
						    -1,
						    command.getTextValue().c_str(),
						    gtk_cell_renderer_text_new(),
						    "text", i, 0);
      }
    }
      break;

    case Command::CREATE_PICKER: {
      auto picker = gtk_combo_box_text_new();
      g_signal_connect(picker, "changed", G_CALLBACK(send_combo_value), this);
      addView(command, picker);
    }
      break;

    case Command::ADD_OPTION: {
      auto view = views_by_id[command.getInternalId()];
      if (view) {
	if (GTK_IS_COMBO_BOX_TEXT(view)) {
	  auto av = gtk_combo_box_text_get_active_text((GtkComboBoxText*)view);
	  string id = to_string(command.getValue());
	  gtk_combo_box_text_append((GtkComboBoxText*)view,
				    id.c_str(), command.getTextValue().c_str());
	  if (!av) {
	    gtk_combo_box_set_active((GtkComboBox*)view, 0);
	  }		      
	}
      }
    }
      break;
      
    case Command::CREATE_BUTTON: {
      auto button = gtk_button_new_with_label(command.getTextValue().c_str());
      g_signal_connect(button, "clicked", G_CALLBACK(send_int_value), this);
      addView(command, button);
    }
      break;
      
    case Command::CREATE_CHECKBOX: {
      auto cb = gtk_check_button_new_with_label(command.getTextValue().c_str());
      g_signal_connect(cb, "toggled", G_CALLBACK(send_toggled_value), this);

      addView(command, cb);
    }
      break;
      
    case Command::CREATE_TEXT: {
      auto label = gtk_label_new(command.getTextValue().c_str());
      gtk_label_set_line_wrap((GtkLabel*)label, true);
      gtk_label_set_yalign((GtkLabel*)label, 0.0f);
      addView(command, label);
    }
      break;
      
    case Command::CREATE_HEADING_TEXT: {
      string s = "<b>" + command.getTextValue() + "</b>";
      auto label = gtk_label_new(0);
      gtk_label_set_markup(GTK_LABEL(label), s.c_str());
      gtk_label_set_line_wrap((GtkLabel*)label, true);
      addView(command, label);
    }
      break;

#if 0
    case Command::CREATE_LINK: {
      auto link = gtk_link_button_new_with_label(uri, label);
    };
      break;
#endif
      
    case Command::CREATE_LINEAR_LAYOUT: {
      auto box = gtk_box_new(command.getValue() == 1 ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 5); // FIXME: spacing
      addView(command, box);
    }
      break;
	
    case Command::CREATE_TABLE_LAYOUT: {
      auto layout = gtk_table_new(1, command.getValue(), false);
      gtk_table_set_homogeneous((GtkTable*)layout, false);
      addView(command, layout);
    }
      break;

    case Command::CREATE_AUTO_COLUMN_LAYOUT: {
      auto layout = gtk_flow_box_new();
      gtk_flow_box_set_homogeneous((GtkFlowBox*)layout, false);
      gtk_orientable_set_orientation((GtkOrientable*)layout, GTK_ORIENTATION_HORIZONTAL);
      gtk_flow_box_set_selection_mode((GtkFlowBox*)layout, GTK_SELECTION_NONE);
      addView(command, layout);
    }
      break;

    case Command::CREATE_ACTIONBAR: {
      if (!header) {
	cerr << "creating actionbar\n";
	header = gtk_header_bar_new();
	gtk_header_bar_set_title((GtkHeaderBar*)header, "App");
	gtk_header_bar_set_show_close_button((GtkHeaderBar*)header, 1);
	gtk_header_bar_set_decoration_layout((GtkHeaderBar*)header, "close");

	auto box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(box), "linked");
	auto previous = gtk_button_new_from_icon_name("go-previous", GTK_ICON_SIZE_BUTTON);
	auto next = gtk_button_new_from_icon_name("go-next", GTK_ICON_SIZE_BUTTON);
	g_signal_connect(previous, "clicked", G_CALLBACK(on_previous_button), this);
	g_signal_connect(next, "clicked", G_CALLBACK(on_next_button), this);
	gtk_box_pack_start((GtkBox*)box, previous, 0, 0, 0);
	gtk_box_pack_start((GtkBox*)box, next, 0, 0, 0);
	
	gtk_header_bar_pack_start((GtkHeaderBar*)header, box);
	
	gtk_widget_show_all(header);
	gtk_window_set_titlebar(GTK_WINDOW(window), header);
      }
    }
      break;

    case Command::CREATE_SWITCH: {
      auto s = gtk_switch_new();
      g_signal_connect(s, "notify::active", G_CALLBACK(send_bool_value), this);

      addView(command, s);
    }
      break;

    case Command::CREATE_IMAGEVIEW: {
      string s = "assets/" + command.getTextValue();
      auto image = gtk_image_new_from_file(s.c_str());
      addView(command, image);
    }
      break;

    case Command::HISTORY_GO_BACK:
    case Command::HISTORY_GO_FORWARD: {
      int id;
      if (command.getType() == Command::HISTORY_GO_BACK) {
	id = popViewBackHistory();
      } else {
	id = popViewForwardHistory();
      }
      if (id) {
	cerr << "id = " << id << endl;
	auto view = views_by_id[id];
	if (view) {
	  cerr << "found view\n";
	  setActiveViewId(id);
	  gtk_stack_set_visible_child((GtkStack*)stack, view);
	  string title = getTextProperty((GtkContainer*)stack, view, "title");
	  gtk_header_bar_set_subtitle((GtkHeaderBar*)header, title.c_str());
	}
      }
    }
      break;
      
    case Command::SET_INT_VALUE: {
      auto view = views_by_id[command.getInternalId()];
      if (gtk_widget_get_parent(view) == stack) {
	if (command.getValue() != 2) {
	  addToHistory(getActiveViewId());
	}
	setActiveViewId(command.getInternalId());
	gtk_stack_set_visible_child((GtkStack*)stack, view);
	string title = getTextProperty((GtkContainer*)stack, view, "title");
	gtk_header_bar_set_subtitle((GtkHeaderBar*)header, title.c_str());
      } else if (GTK_IS_SWITCH(view)) {
	gtk_switch_set_active((GtkSwitch*)view, command.getValue() ? true : false);
      } else {
	cerr << "got spurious int value\n";
      }
    }
      break;
      
    case Command::SET_TEXT_VALUE: {
      auto view = views_by_id[command.getInternalId()];
      if (GTK_IS_LABEL(view)) {
	gtk_label_set_text((GtkLabel*)view, command.getTextValue().c_str());
      } else if (GTK_IS_TEXT_VIEW(view)) {
	cerr << "setting text value\n";
	auto buffer = gtk_text_view_get_buffer((GtkTextView*)view);
	gtk_text_buffer_set_text(buffer, command.getTextValue().c_str(),
				 command.getTextValue().size());
      } else if (GTK_IS_TREE_VIEW(view)) {
	auto model = gtk_tree_view_get_model((GtkTreeView*)view);
	GtkTreeIter iter;
	if (!gtk_tree_model_iter_nth_child(model, &iter, 0, command.getRow())) {
	  gtk_tree_store_append((GtkTreeStore*)model, &iter, NULL);
	}
	gtk_tree_store_set((GtkTreeStore*)model, &iter,
			   command.getColumn(),
			   command.getTextValue().c_str(),
			   -1);
      } else {
	assert(0);
      }
    }
      break;

    case Command::SET_TEXT_DATA: {
      auto view = views_by_id[command.getInternalId()];
      if (GTK_IS_TREE_VIEW(view)) {
	auto model = gtk_tree_view_get_model((GtkTreeView*)view);
	GtkTreeIter iter;
	if (!gtk_tree_model_iter_nth_child(model, &iter, 0, command.getRow())) {
	  gtk_tree_store_append((GtkTreeStore*)model, &iter, NULL);
	}
	gtk_tree_store_set((GtkTreeStore*)model, &iter,
			   command.getColumn(),
			   command.getTextValue().c_str(),
			   -1);
      } else {
	assert(0);
      }
    }
      break;
      
    case Command::CLEAR: {
      auto view = views_by_id[command.getInternalId()];
      if (GTK_IS_TREE_VIEW(view)) {
	auto model = gtk_tree_view_get_model((GtkTreeView*)view);
	gtk_tree_store_clear((GtkTreeStore*)model);
      }
    }
      break;
    
    case Command::SET_ENABLED: {
      auto view = views_by_id[command.getInternalId()];
      if (view) gtk_widget_set_sensitive(view, command.getValue() ? 1 : 0);
    }
      break;

    case Command::SET_ERROR: {
      auto view = views_by_id[command.getInternalId()];
      if (view) {
	auto context = gtk_widget_get_style_context(view);
	if (command.getValue() != 0) {
	  gtk_style_context_add_class(context, "error");
	} else {
	  gtk_style_context_remove_class(context, "error");
	}
      }
    }
      break;
      
    case Command::SHOW_MESSAGE_DIALOG: {
      GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
      auto dialog = gtk_message_dialog_new((GtkWindow*)window,
					   flags,
					   GTK_MESSAGE_ERROR,
					   GTK_BUTTONS_CLOSE,
					   "%s",
					   command.getTextValue2().c_str()
					   );
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
    }
      break;
      
    case Command::CREATE_TIMER:
      break;
      
    case Command::CREATE_FORMVIEW: {
      cerr << "creating formview\n";
      auto sw = gtk_scrolled_window_new(0, 0);
      gtk_container_set_border_width((GtkContainer*)sw, 10);
      gtk_scrolled_window_set_policy((GtkScrolledWindow*)sw,
				     GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
      addView(0, command.getChildInternalId(), sw);
      string s = "form" + to_string(command.getChildInternalId());
      auto & title = command.getTextValue();
      if (title.empty()) {
	gtk_stack_add_named((GtkStack *)stack, sw, s.c_str());
      } else {
	gtk_stack_add_titled((GtkStack *)stack, sw, s.c_str(), title.c_str());
      }

      if (!initial_view_shown) {
	gtk_header_bar_set_subtitle((GtkHeaderBar*)header, title.c_str());
	gtk_stack_set_visible_child((GtkStack*)stack, sw);
	gtk_widget_show_all(window);
	initial_view_shown = true;
      }
    }
      break;

    case Command::DELETE_ELEMENT: {
      auto it = views_by_id.find(command.getChildInternalId());
      if (it != views_by_id.end()) {
	gtk_widget_destroy(it->second); // autom. removed from container
	views_by_id.erase(it);
      }
    }
      break;
      
    default:
      break;
    }
  }
  
#ifdef HAS_SOUNDCANVAS
    std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    return std::make_shared<SDLSoundCanvas>();
  }
#endif
    
  std::string getBundleFilename(const char * filename) override {
    string s = "assets/";
    return s + filename;
  }

  std::string showTextEntryDialog(const std::string & title, const std::string & message) override {
    return "";
  }

  void activate(GtkApplication * _app) {
    app = _app;
        
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), width, height);
  }

  void addView(int parent_id, int id, GtkWidget * widget) {
    assert(widget);
    if (initial_view_shown) gtk_widget_show(widget);
    if (parent_id) {
      auto parent = (GtkContainer *)views_by_id[parent_id];
      assert(parent);
      if (GTK_IS_BOX(parent)) {
	gtk_box_pack_start((GtkBox*)parent, widget, 0, 0, 5);
      } else if (GTK_IS_FLOW_BOX(parent)) {
	gtk_flow_box_insert((GtkFlowBox*)parent, widget, -1);
      } else if (GTK_IS_TABLE(parent)) {
	unsigned int n = getChildCount(parent);
	unsigned int rows, columns;
	gtk_table_get_size((GtkTable*)parent, &rows, &columns);
	if (n == rows * columns) {
	  gtk_table_resize((GtkTable*)parent, rows + 1, columns);
	}
	unsigned int row = n / columns;
	unsigned int col = n % columns;
	cerr << "n = " << n << ", size = " << rows << "x" << columns << ", pos = (" << row << " " << col << ")\n";
	gtk_table_attach((GtkTable*)parent, widget,
			 col, col + 1,
			 row, row + 1,
			 GtkAttachOptions(GTK_EXPAND | GTK_FILL),
			 GtkAttachOptions(GTK_SHRINK),
			 0, 0
			 );
      } else {
	gtk_container_add(parent, widget);
      }
    }
    views_by_id[id] = widget;
  }

  void addView(const Command & c, GtkWidget * widget) {
    addView(c.getInternalId(), c.getChildInternalId(), widget);
  }

  int getId(GtkWidget * widget) const {
    for (auto v : views_by_id) {
      if (v.second == widget) return v.first;
    }
    return 0;
  }

protected:
  static string getTextProperty(gpointer object, const char * key);
  static string getTextProperty(GtkContainer * c, GtkWidget * w, const char * key);

  static size_t getChildCount(GtkContainer * widget);
  
  static void send_int_value(GtkWidget * widget, gpointer data);
  static void send_bool_value(GtkWidget * widget, GParamSpec *pspec, gpointer data);
  static void send_toggled_value(GtkWidget * widget, gpointer user_data);
  static void send_combo_value(GtkWidget * widget, gpointer data);
  static void send_text_value(GtkWidget * widget, gpointer data);
  static void on_previous_button(GtkWidget * widget, gpointer data);
  static void on_next_button(GtkWidget * widget, gpointer data);
  
private:
  GtkApplication * app = nullptr;
  GtkWidget * window = nullptr;
  GtkWidget * header = nullptr;
  GtkWidget * stack = nullptr;
  unordered_map<int, GtkWidget *> views_by_id;
  bool initial_view_shown = false;
};

string
PlatformGtk::getTextProperty(gpointer object, const char * key) {
  gchar * strval = 0;
  g_object_get(object, key, &strval, NULL);
  string s;
  if (strval) {
    s = strval;
    g_free(strval);
  }
  return s;
}

string
PlatformGtk::getTextProperty(GtkContainer * c, GtkWidget * w, const char * key) {
  gchar * strval = 0;
  gtk_container_child_get(c, w, key, &strval, NULL);
  string s;
  if (strval) {
    s = strval;
    g_free(strval);
  }
  return s;
}

size_t
PlatformGtk::getChildCount(GtkContainer * widget) {
  size_t n = 0;
  if (GTK_IS_CONTAINER(widget)) {
    auto children = gtk_container_get_children(GTK_CONTAINER(widget));
    n = g_list_length(children);
  }
  return n;
}

void
PlatformGtk::send_int_value(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  int id = platform->getId(widget);
  assert(id);
  if (id) {
    cerr << "int value: id = " << id << endl;
    ValueEvent ev(platform->getTime(), 1);
    platform->postEvent(id, ev);
  }
}

void
PlatformGtk::send_bool_value(GtkWidget * widget, GParamSpec *pspec, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  int id = platform->getId(widget);
  assert(id);
  if (id) {
    bool v;
    if (GTK_IS_SWITCH(widget)) {
      v = gtk_switch_get_active(GTK_SWITCH(widget));
    } else {
      assert(0);
    }
    cerr << "bool value: id = " << id << ", v = " << v << endl;
    ValueEvent ev(platform->getTime(), v ? 1 : 0);
    platform->postEvent(id, ev);
  }
}

void
PlatformGtk::send_combo_value(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  int id = platform->getId(widget);
  assert(id);
  if (id) {
    string v0 = gtk_combo_box_text_get_active_text((GtkComboBoxText*)widget);
    cerr << "combo value: id = " << id << ", v = " << v0 << endl;
    int v = atoi(v0.c_str());
    ValueEvent ev(platform->getTime(), v ? 1 : 0);
    platform->postEvent(id, ev);
  }
}

void
PlatformGtk::send_toggled_value(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  int id = platform->getId(widget);
  assert(id);
  if (id) {
    bool v;
    if (GTK_IS_CHECK_BUTTON(widget)) {
      gtk_toggle_button_get_active((GtkToggleButton*)widget);
      v = gtk_toggle_button_get_active((GtkToggleButton*)widget);
    } else {
      assert(0);
    }
    cerr << "toggled value: id = " << id << ", v = " << v << endl;
    ValueEvent ev(platform->getTime(), v ? 1 : 0);
    platform->postEvent(id, ev);
  }
}

void
PlatformGtk::send_text_value(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  int id = platform->getId(widget);
  assert(id);
  if (id) {
    string s;
    if (GTK_IS_ENTRY(widget)) {
      s = gtk_entry_get_text((GtkEntry*)widget);
    } else {
      assert(0);
    }
    cerr << "text value: id = " << id << ", text = " << s << endl;
    ValueEvent ev(platform->getTime(), s);
    platform->postEvent(id, ev);
  }
}

void
PlatformGtk::on_previous_button(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  platform->sendCommand2(Command(Command::HISTORY_GO_BACK, 0));  
}

void
PlatformGtk::on_next_button(GtkWidget * widget, gpointer data) {
  cerr << "got next\n";
  PlatformGtk * platform = (PlatformGtk*)data;
  platform->sendCommand2(Command(Command::HISTORY_GO_FORWARD, 0));  
}

static void activate(GtkApplication * app, gpointer user_data) {
  cerr << "activate\n";
  PlatformGtk * platform = (PlatformGtk*)user_data;
  platform->activate(app);
  FWApplication * application = applicationMain();
  platform->addChild(std::shared_ptr<Element>(application));  
}

int main (int argc, char *argv[]) {
  GtkApplication * app;

  PlatformGtk platform;  
  platform.setDisplayWidth(width);
  platform.setDisplayHeight(height);

  cerr << "creating application\n";
  
  app = gtk_application_new("com.sometrik.test", G_APPLICATION_FLAGS_NONE); // FIXME: add correct name
  g_signal_connect (app, "activate", G_CALLBACK (activate), &platform);
  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  
#if 0
  SysEvent ev(platform.getTime(), SysEvent::DESTROY);
  platform.postEvent(application->getInternalId(), ev);
#endif
  
  return status;
}

