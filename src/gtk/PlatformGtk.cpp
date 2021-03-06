#include <CurlClient.h>
#include <Logger.h>
#include <ContextCairo.h>
#include <Command.h>
#include <FWApplication.h>
#include <FWViewBase.h>

#include <OpenGLInitEvent.h>
#include <TouchEvent.h>
#include <DrawEvent.h>
#include <SysEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <TimerEvent.h>
#include <ValueEvent.h>
#include <CommandEvent.h>
#include <ImageRequestEvent.h>
#include <URI.h>
#include <ImageSet.h>

#include <PosixThread.h>

#include <InternalFormat.h>

#include <gtk/gtk.h>
#include <gio/gio.h>

#include <unordered_map>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#define DEFAULT_DISPLAY_WIDTH 800
#define DEFAULT_DISPLAY_HEIGHT 600

using namespace std;

struct sheet_data_s {
  string name;
  bool is_created;
};

extern FWApplication * applicationMain();

class GtkMainThread;

static shared_ptr<GtkMainThread> mainThread;

struct image_data_s {
  ImageSet images;
  unsigned int width, height;
  bool is_fixed;
};

struct event_data_s {
  event_data_s(GtkMainThread * _mainThread, Event * _event, int _internal_id) : mainThread(_mainThread), event(_event), internal_id(_internal_id) { }
  GtkMainThread * mainThread;
  Event * event;
  int internal_id;
};

struct bitmap_data_s {
  bitmap_data_s(PlatformThread * _mainThread, int _internal_id, std::shared_ptr<canvas::PackedImageData> _image) : mainThread(_mainThread), internal_id(_internal_id), image(_image) { }
  
  PlatformThread * mainThread;
  int internal_id;
  std::shared_ptr<canvas::PackedImageData> image;  
};

struct command_data_s {
  command_data_s(PlatformThread * _mainThread, const std::vector<Command> & _commands) : mainThread(_mainThread), commands(_commands) { }
  
  PlatformThread * mainThread;
  std::vector<Command> commands;
};

static gboolean command_callback(gpointer data) {
  command_data_s * cd = (command_data_s*)data;
  cd->mainThread->sendCommands(cd->commands);
  delete cd;
  return FALSE;
}

static gboolean bitmap_callback(gpointer data) {
  bitmap_data_s * bd = (bitmap_data_s*)data;
  bd->mainThread->setImageData(bd->internal_id, bd->image);
  delete bd;
  return FALSE;
}

class GtkLogger : public Logger {
 public:
  GtkLogger(const std::string & _name) : Logger(_name) { }
  
  void println(const char * s) override {
    std::cerr << s << std::endl;
  }
};

class GtkThread : public PosixThread {
public:
  GtkThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
    : PosixThread(_application, _runnable) { }

  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }
  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return make_shared<GtkThread>(application, runnable);
  }
  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override {
    auto thread = getApplication().getThreadPtr();
    bitmap_data_s * bd = new bitmap_data_s(thread.get(), internal_id, image);
    g_idle_add(bitmap_callback, bd);
  }
  void setSurface(int internal_id, canvas::Surface & surface) override {
    std::shared_ptr<canvas::PackedImageData> packedImage = surface.createPackedImage();
    auto thread = getApplication().getThreadPtr();
    bitmap_data_s * bd = new bitmap_data_s(thread.get(), internal_id, packedImage);
    g_idle_add(bitmap_callback, bd);    
  }
  void sendCommands(const vector<Command> & commands) override {
    auto thread = getApplication().getThreadPtr();
    command_data_s * cd = new command_data_s(thread.get(), commands);
    g_idle_add(command_callback, cd);
  }
  string getLocalFilename(const char * fn, FileType type) override {
    string s = "assets/";
    return s + fn;
  }
  std::string getBundleFilename(const char * filename) override {
    string s = "assets/";
    return s + filename;
  }
  std::string loadTextAsset(const char * filename) override {
    std::ifstream t(getBundleFilename(filename));
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
  }
  
  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new GtkLogger(name));
  }

  int startModal() override {
    return getModalResultValue();
  }

  void endModal(int value) override {
    setModalResultValue(value);
  }

};

class GtkMainThread : public PlatformThread {
public:
  GtkMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable)
    : PlatformThread(_application, _runnable) {
    CurlClientFactory::globalInit();

    string fn = g_get_user_config_dir();
    fn += "/framework.ini";

    keyFile = g_key_file_new();
    
    if (g_key_file_load_from_file(keyFile, fn.c_str(), G_KEY_FILE_NONE, NULL)) {
      FWPreferences prefs;
    
      gchar * groupName = g_key_file_get_start_group(keyFile);
      if (groupName) {
	gchar ** keys = g_key_file_get_keys(keyFile, groupName, NULL, NULL);
	for (unsigned int i = 0; keys[i] != 0; i++) {
	  gchar * value = g_key_file_get_string(keyFile, groupName, keys[i], NULL);
	  if (value) prefs.setText(keys[i], value);
	}
	g_strfreev(keys);
      }

      prefs.clearChanges();
      application->setPreferences(prefs);
    }
  }

  ~GtkMainThread() {
    g_key_file_free(keyFile);
  }
  
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }

  string getLocalFilename(const char * fn, FileType type) override {
    string s = "assets/";
    return s + fn;
  }
  
  std::string getBundleFilename(const char * filename) override {
    string s = "assets/";
    return s + filename;
  }
  
  std::string loadTextAsset(const char * filename) override {
    std::ifstream t(getBundleFilename(filename));
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
  }

  bool startThread(std::shared_ptr<PlatformThread> thread) override { return false; }
  bool testDestroy() override { return false; }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return make_shared<GtkThread>(application, runnable);
  }
  
  void sleep(double t) override {
    usleep((unsigned int)(t * 1000000));
  }

  void sendEvent(int internal_id, const Event & ev) override {
    event_data_s * ed = new event_data_s(this, ev.dup(), internal_id);
    g_idle_add(event_callback, ed);
  }

  std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents(bool blocking = true) {
    std::vector<std::pair<int, std::shared_ptr<Event> > > r;
    return r;
  }
      
  void activate(GtkApplication * _gtk_app) {
    cerr << "creating window\n";
    
    gtk_app = _gtk_app;
        
    window = gtk_application_window_new(gtk_app);

    setDisplayScale(gtk_widget_get_scale_factor(window));
  
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), DEFAULT_DISPLAY_WIDTH, DEFAULT_DISPLAY_HEIGHT);
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_window), this);
    
    cerr << "activate done\n";
  }

  void startEventLoop() override { }

  int startModal() override {
    if (!dialog_stack.empty()) {
      auto view = views_by_id[dialog_stack.back()];
      return gtk_dialog_run(GTK_DIALOG(view));
    } else {
      return 0;
    }
  }

  void endModal(int value) override {
    if (!dialog_stack.empty()) {
      auto view = views_by_id[dialog_stack.back()];
      gtk_dialog_response(GTK_DIALOG(view), value);
      dialog_stack.pop_back();
    }
  }
  
  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new GtkLogger(name));
  }

protected:
  void setDestroyed() override { }
  
  static inline GtkStateFlags getGtkState(Selector selector) {
    switch (selector) {
    case Selector::NORMAL: return GTK_STATE_FLAG_NORMAL;
    case Selector::ACTIVE: return GTK_STATE_FLAG_ACTIVE;
    case Selector::HOVER: return GTK_STATE_FLAG_PRELIGHT;
    case Selector::SELECTED: return GTK_STATE_FLAG_SELECTED;
    case Selector::DISABLED: return GTK_STATE_FLAG_INSENSITIVE;
    case Selector::LINK: return GTK_STATE_FLAG_LINK;
    }
    return GTK_STATE_FLAG_NORMAL;
  }

  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override {
    auto view = views_by_id[internal_id];
    if (view) {  
      bool has_alpha = true; // format == canvas::RGBA8;
      GdkPixbuf * pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
					  has_alpha,
					  8,
					  image->getWidth(),
					  image->getHeight());
      assert(pixbuf);
      auto pixels = gdk_pixbuf_get_pixels(pixbuf);
      size_t n = image->calculateSizeForFirstLevel();
      auto input = image->getData();
      for (size_t i = 0; i < n; i += 4) {
	pixels[i + 0] = input[i + 2];
	pixels[i + 1] = input[i + 1];
	pixels[i + 2] = input[i + 0];
	pixels[i + 3] = input[i + 3];
      }
      gtk_image_set_from_pixbuf(GTK_IMAGE(view), pixbuf);
    }
  }

  void setSurface(int internal_id, canvas::Surface & _surface) override {
    auto view = views_by_id[internal_id];
    if (view) {
      auto & surface = dynamic_cast<canvas::CairoSurface&>(_surface);
      gtk_image_set_from_surface(GTK_IMAGE(view), surface.getCairoSurface());
    }
  }

  void sendCommands(const vector<Command> & commands) {
    for (auto & command : commands) {
    
      switch (command.getType()) {
      case Command::QUIT_APP:
	exit(0);
	break;

      case Command::CREATE_APPLICATION: {
	cerr << "creating stack\n";
	assert(window);
	stack = gtk_stack_new();
	gtk_stack_set_transition_type((GtkStack*)stack, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	// ignore parent id
	gtk_container_add(GTK_CONTAINER(window), stack);

	addView(0, command.getChildInternalId(), stack);

	gtk_widget_show_all(window);
      }
	break;

      case Command::CREATE_TIMER: {
	cerr << "creating timer for " << command.getValue() << endl;
	g_timeout_add(command.getValue(), timer_callback, this);
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

      case Command::CREATE_GRIDVIEW:
      case Command::CREATE_LISTVIEW: {
	auto view = gtk_scrolled_window_new(0, 0);
	gtk_scrolled_window_set_min_content_height((GtkScrolledWindow*)view, 400);
	auto store = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	auto gridview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(gridview), 1);
	// gtk_widget_set_vexpand((GtkWidget*)gridview, 1);
	// gtk_widget_set_hexpand((GtkWidget*)gridview, 1);
	// g_signal_connect(gridview, "cursor-changed", G_CALLBACK(send_selection_value), this);
	g_signal_connect(gridview, "row-activated", G_CALLBACK(send_activation_value), this);
	gtk_container_add(GTK_CONTAINER(view), gridview);
	addView(command, view, true);
	if (initial_view_shown) gtk_widget_show(gridview);
      }
	break;

      case Command::ADD_SHEET: {
	auto & sheets = sheets_by_id[command.getInternalId()];
	sheets.push_back({ command.getTextValue(), false });
      }
	break;

      case Command::RESHAPE_TABLE: {
	cerr << "RESHAPE_TABLE\n";
	size_t max_size = command.getValue();
	auto view = views_by_id[command.getInternalId()];
	if (view) {
	  if (GTK_IS_BIN(view)) {
	    cerr << "is bin\n";
	    auto treeview = gtk_bin_get_child(GTK_BIN(view));
	    auto model = gtk_tree_view_get_model((GtkTreeView*)treeview);
	  
	    auto & sheets = sheets_by_id[command.getInternalId()];
	    for (int i = int(sheets.size()) - 1; i >= max_size; i--) {
	      auto & sheet = sheets[i];
	      if (sheet.is_created) {
		sheet.is_created = false;
		GtkTreeIter parent;
		gtk_tree_model_iter_nth_child(model, &parent, 0, i);
		gtk_tree_store_remove(GTK_TREE_STORE(model), &parent);
	      }	  
	    }
	  }
	}
      }
	break;
      
      case Command::RESHAPE_SHEET: {
	cerr << "RESHAPE_SHEET\n";
	size_t max_size = command.getValue();
	auto view = views_by_id[command.getInternalId()];
	assert(view);
	if (view) {
	  auto & sheets = sheets_by_id[command.getInternalId()];
	  if (command.getSheet() < sheets.size()) {
	    auto treeview = gtk_bin_get_child(GTK_BIN(view));
	    auto model = gtk_tree_view_get_model((GtkTreeView*)treeview);
	    GtkTreeIter iter, parent;
	    gtk_tree_model_iter_nth_child(model, &parent, 0, command.getSheet());
	    while (gtk_tree_model_iter_nth_child(model, &iter, &parent, max_size)) {
	      gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
	    }
	  }	  
	}
      }
	break;
      
      case Command::ADD_COLUMN: {
	auto view = views_by_id[command.getInternalId()];
	if (view) {
	  if (GTK_IS_ACTION_BAR(view)) {
	    auto button = gtk_button_new_from_icon_name("properties", GTK_ICON_SIZE_BUTTON);
	    widget_values[button] = command.getValue();
	    g_signal_connect(button, "clicked", G_CALLBACK(on_bar_button), this);
	    gtk_action_bar_pack_start((GtkActionBar*)view, button);
	    gtk_widget_show(button);
	  } else {
	    auto treeview = gtk_bin_get_child(GTK_BIN(view));
	    if (!treeview) {
	      cerr << "no actual child for ADD_COLUMN\n";
	    } else if (!GTK_IS_TREE_VIEW(treeview)) {
	      cerr << "actual child is not tree view\n";
	    } else {
	      int i = gtk_tree_view_get_n_columns(GTK_TREE_VIEW(treeview));
	      auto renderer = gtk_cell_renderer_text_new();
	      float xalign = 0.0f;
	      bool autosize = false;
	      if (command.getValue() == 2) {
		xalign = 1.0f;
		autosize = true;
	      }
	      gtk_cell_renderer_set_alignment(renderer, xalign, 0.0f);
	      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
							  -1,
							  command.getTextValue().c_str(),
							  renderer,
							  "text", i, NULL);
	      if (autosize) {
		auto column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), i);
		gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	      }
	    }
	  }
	} else {
	  cerr << "view not found for ADD_COLUMN\n";
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
	cerr << "ADD_OPTION\n";
	auto view = views_by_id[command.getInternalId()];
	if (view) {
	  if (GTK_IS_HEADER_BAR(view)) {
	    auto button = gtk_button_new_from_icon_name("properties", GTK_ICON_SIZE_BUTTON);
	    widget_values[button] = command.getValue(); 
	    g_signal_connect(button, "clicked", G_CALLBACK(on_bar_button), this);
	    gtk_header_bar_pack_start((GtkHeaderBar*)view, button);
	    gtk_widget_show(button);
	  } else if (GTK_IS_COMBO_BOX_TEXT(view)) {
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
	gtk_button_set_image_position(GTK_BUTTON(button), GTK_POS_TOP);
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
	gtk_label_set_xalign((GtkLabel*)label, 0.0f);
	gtk_label_set_yalign((GtkLabel*)label, 0.0f);
	gtk_label_set_justify((GtkLabel*)label, GTK_JUSTIFY_LEFT);
	addView(command, label);
      }
	break;

      case Command::CREATE_LINK: {
	auto link = gtk_link_button_new_with_label(command.getTextValue().c_str(),
						   command.getTextValue2().c_str());
	addView(command, link);
      }
	break;      

      case Command::CREATE_FLIPPER_LAYOUT: {
	auto flipper = gtk_stack_new();
	gtk_stack_set_transition_type((GtkStack*)flipper, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	addView(command, stack);
      }
	break;
	
      case Command::CREATE_FRAME_LAYOUT: {
	auto frame = gtk_overlay_new();
	addView(command, frame);
      }
	break;
	
      case Command::CREATE_LINEAR_LAYOUT: {
	auto box = gtk_box_new(command.getValue() == 1 ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0);
	addView(command, box);
      }
	break;

      case Command::CREATE_EVENT_LAYOUT: {
	auto box = gtk_event_box_new();
	gtk_widget_set_events(box, GDK_BUTTON_PRESS_MASK);
	g_signal_connect(G_OBJECT(box), "button_press_event", G_CALLBACK(on_button_press), this);
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

      case Command::CREATE_NAVIGATIONBAR: {
	auto bar = gtk_action_bar_new();
	addView(command, bar);
      }
	break;
      
      case Command::CREATE_ACTIONBAR: {
	if (!header) {
	  header = gtk_header_bar_new();
	  gtk_header_bar_set_title((GtkHeaderBar*)header, "App");
	  gtk_header_bar_set_show_close_button((GtkHeaderBar*)header, 1);
	  gtk_header_bar_set_decoration_layout((GtkHeaderBar*)header, "close");

	  auto box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	  gtk_style_context_add_class(gtk_widget_get_style_context(box), "linked");
	  auto previous = gtk_button_new_from_icon_name("go-previous", GTK_ICON_SIZE_BUTTON);
	  auto settings = gtk_button_new_from_icon_name("settings", GTK_ICON_SIZE_BUTTON);

	  g_signal_connect(previous, "clicked", G_CALLBACK(on_previous_button), this);
	  g_signal_connect(settings, "clicked", G_CALLBACK(on_settings_button), this);

	  gtk_box_pack_start((GtkBox*)box, previous, 0, 0, 0);
	  gtk_box_pack_start((GtkBox*)box, settings, 0, 0, 0);
	
	  gtk_header_bar_pack_start((GtkHeaderBar*)header, box);
	
	  gtk_widget_show_all(header);
	  gtk_window_set_titlebar(GTK_WINDOW(window), header);

	  addView(0, command.getChildInternalId(), header);
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
	GtkWidget * image = gtk_image_new();
	addView(command, image, true);

	g_signal_connect(G_OBJECT(image), "size-allocate", G_CALLBACK(on_size_allocate), this);

	auto & uri_text = command.getTextValue();
	if (!uri_text.empty()) {
	  URI uri(uri_text);
	  if (uri.getScheme() == "http" || uri.getScheme() == "https") {
	    addImageUrl(command.getChildInternalId(),
			command.getWidth(), command.getHeight(), uri_text);
	  } else {
	    auto pixbuf = loadPixbuf(uri_text);
	    if (pixbuf) {
	      gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
	    }
	  }
	}
      }
	break;

      case Command::HISTORY_GO_BACK: {
	auto & app = getApplication();
	int id = app.popViewBackHistory();
	if (id) {
	  cerr << "id = " << id << endl;
	  auto view = views_by_id[id];
	  if (view) {
	    cerr << "found view\n";
	    app.setActiveViewId(id);
	    gtk_stack_set_visible_child((GtkStack*)stack, view);

	    if (header) {
	      string title;
	      auto e = Element::getRegisteredElement(id);
	      if (e) {
		auto e2 = dynamic_cast<FWViewBase*>(e);
		if (e2) title = e2->getTitle().c_str();
	      }
	      gtk_header_bar_set_subtitle((GtkHeaderBar*)header, title.c_str());
	    }
	  }
	}
      }
	break;
      
      case Command::SET_INT_VALUE: {
	auto view = views_by_id[command.getInternalId()];
	if (!view) {
	  cerr << "no view " << command.getInternalId() << " for SET_INT_VALUE\n";
	  assert(0);
	} else if (GTK_IS_STACK(view)) { // Flipper
	  GtkStack * stack = GTK_STACK(view);
	  GList *children = gtk_container_get_children(GTK_CONTAINER(view));
	  int n = command.getValue();
	  while (n >= 0 && (children = g_list_next(children)) != NULL) {
	    if (n == 0) {
	      GtkWidget* widget = (GtkWidget*)children->data;
	      gtk_stack_set_visible_child(stack, widget);
	    } else {
	      n--;
	    }	    
	  }	  
	} else if (gtk_widget_get_parent(view) == stack) {
	  gtk_stack_set_visible_child((GtkStack*)stack, view);
	  gtk_header_bar_set_subtitle((GtkHeaderBar*)header, command.getTextValue().c_str());
	} else if (GTK_IS_SWITCH(view)) {
	  gtk_switch_set_active((GtkSwitch*)view, command.getValue() ? true : false);
	} else {
	  cerr << "got spurious int value\n";
	}
      }
	break;
      
      case Command::SET_TEXT_VALUE: {
	auto view = views_by_id[command.getInternalId()];
	if (!view) {
	  cerr << "view not found for SET_TEXT_VALUE\n";
	} else if (GTK_IS_LABEL(view)) {
	  gtk_label_set_text((GtkLabel*)view, command.getTextValue().c_str());
	} else if (GTK_IS_TEXT_VIEW(view)) {
	  auto buffer = gtk_text_view_get_buffer((GtkTextView*)view);
	  gtk_text_buffer_set_text(buffer, command.getTextValue().c_str(),
				   command.getTextValue().size());
	} else if (GTK_IS_HEADER_BAR(view)) {
	  gtk_header_bar_set_subtitle((GtkHeaderBar*)view, command.getTextValue().c_str());
	} else if (GTK_IS_BUTTON(view)) {
	  gtk_button_set_label(GTK_BUTTON(view), command.getTextValue().c_str());
	} else if (GTK_IS_IMAGE(view)) {
	  auto & uri_text = command.getTextValue();
	  URI uri(uri_text);
	  if (uri.getScheme() == "http" || uri.getScheme() == "https") {
	    int w = gtk_widget_get_allocated_width(view);
	    requestImage(command.getInternalId(), uri_text, w);
	  } else {
	    auto pixbuf = loadPixbuf(uri_text);
	    if (pixbuf) {
	      gtk_image_set_from_pixbuf(GTK_IMAGE(view), pixbuf);
	    }
	  }
	} else {
	  cerr << "unable to set text value: " << command.getTextValue() << "\n";
	  // assert(0);
	}
      }
	break;

      case Command::SET_TEXT_DATA: {
	auto view = views_by_id[command.getInternalId()];
	assert(view);
	if (GTK_IS_BIN(view)) {
	  auto treeview = gtk_bin_get_child(GTK_BIN(view));
	  assert(GTK_IS_TREE_VIEW(treeview));
	  if (GTK_IS_TREE_VIEW(treeview)) {
	    auto & sheets = sheets_by_id[command.getInternalId()];
	    if (command.getSheet() < sheets.size() || sheets.empty()) {
	      auto model = gtk_tree_view_get_model((GtkTreeView*)treeview);
	      
	      GtkTreeIter iter;
	      if (!sheets.empty()) {
		for (unsigned int si = 0; si <= command.getSheet(); si++) {
		  auto & sheet = sheets[si];
		  if (!sheet.is_created) {
		    sheet.is_created = true;
		    GtkTreeIter iter;
		    gtk_tree_store_append((GtkTreeStore*)model, &iter, 0);
		    gtk_tree_store_set((GtkTreeStore*)model, &iter,
				       0,
				       sheet.name.c_str(),
				       -1);
		  }
		}
		
		GtkTreeIter parent;
		gtk_tree_model_iter_nth_child(model, &parent, 0, command.getSheet());
		while (!gtk_tree_model_iter_nth_child(model, &iter, &parent, command.getRow())) {
		  gtk_tree_store_append((GtkTreeStore*)model, &iter, &parent);
		}
	      } else {
		while (!gtk_tree_model_iter_nth_child(model, &iter, 0, command.getRow())) {
		  gtk_tree_store_append((GtkTreeStore*)model, &iter, 0);
		}
	      }
	      gtk_tree_store_set((GtkTreeStore*)model, &iter,
				 command.getColumn(),
				 command.getTextValue().c_str(),
				 -1);
	    }
	  } else {
	    cerr << "unable to handle SET_TEXT_DATA\n";
	  }
	}
      }
	break;
      
      case Command::CLEAR: {
	auto view = views_by_id[command.getInternalId()];
	assert(view);
	if (GTK_IS_BIN(view)) {
	  auto it = sheets_by_id.find(command.getInternalId());
	  if (it != sheets_by_id.end()) {
	    for (auto & sd : it->second) {
	      sd.is_created = false;
	    }
	  }
      
	  auto treeview = gtk_bin_get_child(GTK_BIN(view));
	  // assert(GTK_IS_TREE_VIEW(treeview));
	  if (GTK_IS_TREE_VIEW(treeview)) {
	    auto model = gtk_tree_view_get_model((GtkTreeView*)view);
	    gtk_tree_store_clear((GtkTreeStore*)model);
	  }
	} else if (GTK_IS_ENTRY(view)) {
	  gtk_entry_set_text((GtkEntry*)view, "");
	} else if (GTK_IS_IMAGE(view)) {
	  gtk_image_clear(GTK_IMAGE(view));
	}
      }
	break;

#if 0
      case Command::SET_ENABLED: {
	auto view = views_by_id[command.getInternalId()];
	if (view) gtk_widget_set_sensitive(view, command.getValue() ? 1 : 0);
      }
	break;
#endif
      
      case Command::SET_VISIBILITY: {
	auto view = views_by_id[command.getInternalId()];
	if (view) {
	  if (command.getValue()) gtk_widget_show(view);
	  else gtk_widget_hide(view);
	}
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

      case Command::SET_STYLE: {
	setStyle(command.getInternalId(), Selector(command.getValue()), command.getTextValue(), command.getTextValue2());
      }
	break;

      case Command::CREATE_DIALOG: {
	GtkDialogFlags flags = GtkDialogFlags(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT);
	auto dlg = gtk_dialog_new_with_buttons(command.getTextValue().c_str(),
					       GTK_WINDOW(window),
					       flags,
					       0
					       );
	// gtk_window_set_modal(GTK_WINDOW(dlg), 1);
	// gtk_window_set_transient_for(GTK_WINDOW(dlg), GTK_WINDOW(parent));
	addView(0, command.getChildInternalId(), dlg);
	dialog_stack.push_back(command.getChildInternalId());
      }
	break;

      case Command::DELETE_ELEMENT: {
	auto it = views_by_id.find(command.getInternalId());
	if (it != views_by_id.end()) {
	  auto widget = it->second;
	  if (GTK_IS_DIALOG(widget)) {
	    gtk_dialog_response(GTK_DIALOG(widget), 0);
	  } else if (GTK_IS_WIDGET(widget)) {
	    gtk_widget_destroy(it->second); // autom. removed from container
	  }
	  views_by_id.erase(it);
	}
      }
	break;
            
      case Command::CREATE_TOAST: {
	// GApplication *application = g_application_new ("hello.world", G_APPLICATION_FLAGS_NONE);
	// g_application_register (application, NULL, NULL);
	GNotification * notification = g_notification_new(command.getTextValue().c_str());
	g_notification_set_body(notification, command.getTextValue().c_str());
	GIcon *icon = g_themed_icon_new ("dialog-information");
	g_notification_set_icon (notification, icon);
	g_application_send_notification(G_APPLICATION(gtk_app), NULL, notification);
	g_object_unref (icon);
	g_object_unref (notification);
      }
	break;

      case Command::CREATE_FRAMEVIEW: {
	auto box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	addView(0, command.getChildInternalId(), box);
	string s = "form" + to_string(command.getChildInternalId());
	gtk_stack_add_named((GtkStack *)stack, box, s.c_str());

	if (!initial_view_shown) {
	  if (header) {
	    gtk_header_bar_set_subtitle((GtkHeaderBar*)header, command.getTextValue().c_str());
	  }
	  gtk_stack_set_visible_child((GtkStack*)stack, box);
	  gtk_widget_show_all(window);
	  initial_view_shown = true;
	}
      }
	break;
      
      case Command::CREATE_SCROLL_LAYOUT: {
	auto sw = gtk_scrolled_window_new(0, 0);
	// gtk_scrolled_window_set_min_content_height((GtkScrolledWindow*)sw, 400);
	gtk_scrolled_window_set_policy((GtkScrolledWindow*)sw, GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	addView(command, sw, true);
      }
	break;      

      case Command::REMOVE_CHILD: {
	auto it = views_by_id.find(command.getChildInternalId());
	if (it != views_by_id.end()) {
	  auto widget = it->second;
	  if (GTK_IS_WIDGET(widget)) {
	    gtk_widget_destroy(widget); // autom. removed from container
	  }
	  views_by_id.erase(it);
	}
      }
	break;

      case Command::REORDER_CHILD: {
	auto view = views_by_id[command.getInternalId()];
	auto child = views_by_id[command.getChildInternalId()];
	if (view && child) {
	  gtk_box_reorder_child(GTK_BOX(view), child, command.getValue());
	}
      }
	break;

      case Command::UPDATE_PREFERENCE: {
	g_key_file_set_string(keyFile, "app", command.getTextValue().c_str(), command.getTextValue2().c_str());	
      }
	break;

      case Command::COMMIT_PREFERENCES: {
	string fn = g_get_user_config_dir();
	fn += "/framework.ini";
    
	g_key_file_save_to_file(keyFile, fn.c_str(), NULL);
      }
	break;

      case Command::LAUNCH_BROWSER: {
#if 0
	gtk_show_uri_on_window(GTK_WINDOW(window),
			       command.getTextValue().c_str(),
			       GDK_CURRENT_TIME,
			       0);
#else
	gtk_show_uri(0,
		     command.getTextValue().c_str(),
		     GDK_CURRENT_TIME,
		     0);
#endif
      }
	break;
      
      default:
	break;
      }
    }
  }

  PangoFontDescription * getFont(int view_id) {
    auto it = fonts_by_id.find(view_id);
    if (it != fonts_by_id.end()) {
      return it->second;
    } else {
      auto font = pango_font_description_new(); // FIXME: NOT FREED
      fonts_by_id[view_id] = font;
      return font;
    }
  }

  static GtkPositionType parseIconAttachment(const std::string & value) {
    if (value == "left") return GTK_POS_LEFT;
    else if (value == "right") return GTK_POS_RIGHT;
    else if (value == "top") return GTK_POS_TOP;
    else if (value == "bottom") return GTK_POS_BOTTOM;
    else return GTK_POS_TOP;
  }
  
  void setStyle(int id, Selector selector, const std::string & key, const std::string & value) {
    auto widget = views_by_id[id];
    if (!widget) return;

    if (key == "min-height") {
      if (GTK_IS_SCROLLED_WINDOW(widget)) {
	int h = stoi(value);
	gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(widget), h);
      }
    } else if (key == "height") {
      GtkWidget * parent = gtk_widget_get_parent(widget);
      bool match_parent = value == "match-parent";
      bool wrap_content = value == "wrap-content";
      if (GTK_IS_BOX(parent) && 0) {
	gtk_box_set_child_packing(GTK_BOX(parent),
				  widget,
				  match_parent,
				  match_parent,
				  0,
				  GTK_PACK_START);
      }
      if (!match_parent && !wrap_content) {
	gtk_widget_set_size_request(widget, -1, stoi(value));
      }
      if (!match_parent && !wrap_content && GTK_IS_IMAGE(widget)) {
	setImageWidth(id, stoi(value), true);
      }
    } else if (key == "width") {
      GtkWidget * parent = gtk_widget_get_parent(widget);
      bool match_parent = value == "match-parent";
      bool wrap_content = value == "wrap-content";
      if (GTK_IS_BOX(parent) && 0) {
	gtk_box_set_child_packing(GTK_BOX(parent),
				  widget,
				  match_parent,
				  match_parent,
				  0,
				  GTK_PACK_START);
      }

      if (!match_parent && !wrap_content && GTK_IS_IMAGE(widget)) {
	setImageHeight(id, stoi(value), true);
      }
    } else if (key == "color") {
      GdkRGBA color;
      if (gdk_rgba_parse(&color, value.c_str())) {
	gtk_widget_override_color(widget, getGtkState(selector), &color);
      }
    } else if (key == "background-color") {
      GdkRGBA color;
      if (gdk_rgba_parse(&color, value.c_str())) {
	gtk_widget_override_background_color(widget, getGtkState(selector), &color);
      }
    } else if (key == "font-size") {
      int size = 0;
      if (value == "small") {
	size = 9;
      } else if (value == "medium") {
	size = 12;
      } else if (value == "large") {
	size = 15;
      } else {
	size = stoi(value);
      }
      if (size) {
	auto font = getFont(id);
	pango_font_description_set_size(font, size * PANGO_SCALE);
	gtk_widget_override_font(widget, font);
      }
    } else if (key == "margin") {
      int m = stoi(value);
      gtk_widget_set_margin_start(widget, m);
      gtk_widget_set_margin_end(widget, m);
      gtk_widget_set_margin_top(widget, m);
      gtk_widget_set_margin_bottom(widget, m);
    } else if (key == "margin-left") {
      gtk_widget_set_margin_start(widget, stoi(value));
    } else if (key == "margin-right") {
      gtk_widget_set_margin_end(widget, stoi(value));
    } else if (key == "margin-top") {
      gtk_widget_set_margin_top(widget, stoi(value));
    } else if (key == "margin-bottom") {
      gtk_widget_set_margin_bottom(widget, stoi(value));
    } else if (key == "font-weight") {
      auto font = getFont(id);
      PangoWeight weight = PANGO_WEIGHT_NORMAL;
      if (value == "bold") {
	weight = PANGO_WEIGHT_BOLD;
      }
      pango_font_description_set_weight(font, weight);
      gtk_widget_override_font(widget, font);
    } else if (key == "font-variant") {
      auto font = getFont(id);
      PangoVariant variant = PANGO_VARIANT_NORMAL;
      if (value == "small-caps") {
	variant = PANGO_VARIANT_SMALL_CAPS;
      }
      pango_font_description_set_variant(font, variant);
      gtk_widget_override_font(widget, font);
    } else if (key == "font-style") {
      auto font = getFont(id);
      PangoStyle style = PANGO_STYLE_NORMAL;
      if (value == "italic") {
	style = PANGO_STYLE_ITALIC;
      } else if (value == "oblique") {
	style = PANGO_STYLE_OBLIQUE;
      }
      pango_font_description_set_style(font, style);
      gtk_widget_override_font(widget, font);
    } else if (key == "font-family") {
      auto font = getFont(id);
      pango_font_description_set_family(font, value.c_str());
      gtk_widget_override_font(widget, font);
    } else if (GTK_IS_BUTTON(widget)) {
      if (key == "icon") {
	auto pixbuf = loadPixbuf(value);
	if (pixbuf) {
	  GtkWidget * image = gtk_image_new();
	  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
	  gtk_button_set_always_show_image(GTK_BUTTON(widget), true);
	  gtk_button_set_image(GTK_BUTTON(widget), image);
	}
      } else if (key == "icon-attachment") {
	gtk_button_set_image_position(GTK_BUTTON(widget), parseIconAttachment(value));
      }
    } else if (GTK_IS_LABEL(widget)) {
      auto label = GTK_LABEL(widget);
      if (key == "white-space") {
	bool wrap = true;
	if (value == "no-wrap") {
	  wrap = false;
	}
	gtk_label_set_line_wrap(label, wrap);
      } else if (key == "text-align") {
	GtkJustification j = GTK_JUSTIFY_LEFT;
	float align = 0.0f;
	if (value == "right") {
	  j = GTK_JUSTIFY_RIGHT;
	  align = 1.0f;
	} else if (value == "center") {
	  j = GTK_JUSTIFY_CENTER;
	  align = 0.5f;
	} else if (value == "justify") {
	  j = GTK_JUSTIFY_FILL;
	}
	gtk_label_set_justify(label, j);
	gtk_label_set_xalign((GtkLabel*)label, align);
      } else if (key == "font-size") {
      } else if (key == "font-weight") {
      } else if (key == "font-style") {
      } else if (key == "text-overflow") {
	PangoEllipsizeMode m = PANGO_ELLIPSIZE_NONE;
	if (value == "ellipsis") m = PANGO_ELLIPSIZE_END;
	gtk_label_set_ellipsize(label, m);
      } else if (key == "border") {
      } else if (key == "single-line") {
	gtk_label_set_single_line_mode(label, true);
      }    
    } else if (GTK_IS_FRAME(widget)) {
      auto frame = GTK_FRAME(widget);
      if (key == "shadow") {
	GtkShadowType s = GTK_SHADOW_NONE;
	if (!value.empty()) {
	  s = GTK_SHADOW_ETCHED_OUT;
	}
	gtk_frame_set_shadow_type(frame, s);
      }
    }
  }
  
  void addView(int parent_id, int id, GtkWidget * widget, bool expand = false) {
    assert(widget);
    if (initial_view_shown) gtk_widget_show(widget);    
    if (parent_id) {
      auto parent = (GtkContainer *)views_by_id[parent_id];
      if (!parent) {
	cerr << "no parent " << parent_id << " for view " << id << endl;
	// assert(0);
      } else if (GTK_IS_DIALOG(parent)) {
	auto a = gtk_dialog_get_content_area(GTK_DIALOG(parent));
	gtk_container_add(GTK_CONTAINER(a), widget);
      } else if (GTK_IS_BOX(parent)) {
	gtk_box_pack_start((GtkBox*)parent, widget, expand ? 1 : 0, expand ? 1 : 0, 0);
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
	// cerr << "n = " << n << ", size = " << rows << "x" << columns << ", pos = (" << row << " " << col << ")\n";
	gtk_table_attach((GtkTable*)parent, widget,
			 col, col + 1,
			 row, row + 1,
			 GtkAttachOptions(GTK_EXPAND | GTK_FILL),
			 GtkAttachOptions(GTK_SHRINK),
			 0, 0
			 );
      } else if (GTK_IS_OVERLAY(parent)) {
	cerr << "add overlay " << parent_id << ", " << id << ", " << parent << ", " << widget << endl;
	gtk_overlay_add_overlay(GTK_OVERLAY(parent), widget);
	cerr << "overlay added\n";
      } else {
	gtk_container_add(parent, widget);
	if (GTK_IS_STACK(parent)) {
	  gtk_stack_set_visible_child(GTK_STACK(stack), widget);
	}
      }
    }
    views_by_id[id] = widget;
  }

  void requestImage(int viewId, unsigned int width = 0, unsigned int height = 0) {
    cerr << "requestImage(), viewId = " << viewId << endl;
    ImageRequestEvent ev(ImageRequestEvent::REQUEST, viewId, width, height);
    sendEvent(viewId, ev);
  }

  void requestImage(int viewId, const std::string & uri, unsigned int width = 0, unsigned int height = 0) {
    cerr << "requestImage(), viewId = " << viewId << ", uri = " << uri << endl;
    ImageRequestEvent ev(ImageRequestEvent::REQUEST, viewId, uri, width, height);
    sendEvent(viewId, ev);
  }

  void cancelImage(int viewId) {
    ImageRequestEvent ev(ImageRequestEvent::CANCEL, viewId);
    sendEvent(viewId, ev);
  }

  GdkPixbuf * loadPixbuf(const string & filename) {
    string s = getBundleFilename(filename.c_str());
    GError * error = 0;
    GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file(s.c_str(), &error);
    if (!pixbuf) {
      cerr << "failed to load image " << filename << endl;
    } else if (getDisplayScale() < 2.0f) {
      int new_w = int(getDisplayScale() * gdk_pixbuf_get_width(pixbuf) / 2.0f);
      int new_h = int(getDisplayScale() * gdk_pixbuf_get_height(pixbuf) / 2.0f);
      auto new_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 
						new_w,
						new_h,
						GDK_INTERP_BILINEAR);
      g_object_unref(pixbuf);
      pixbuf = new_pixbuf;
    }
    return pixbuf;
  }
  
  void addView(const Command & c, GtkWidget * widget, bool expand = false) {
    addView(c.getInternalId(), c.getChildInternalId(), widget, expand);
  }

  int getId(GtkWidget * widget) const {
    for (auto v : views_by_id) {
      if (v.second == widget) return v.first;
    }
    return 0;
  }

  int getValue(GtkWidget * widget) const {
    auto it = widget_values.find(widget);
    if (it != widget_values.end()) {
      return it->second;
    } else {
      return 0;
    }
  }

  const vector<sheet_data_s> & getSheetsById(int id) const {
    auto it = sheets_by_id.find(id);
    if (it != sheets_by_id.end()) {
      return it->second;
    } else {
      return null_sheets;
    }
  }

  void addImageUrl(int id, unsigned short w, unsigned short h, const std::string & url) {
    image_data[id].images.insert(w, h, url);
  }

  void setImageWidth(int id, unsigned int w, bool fixed = false) {
    image_data[id].width = w;
    if (fixed) image_data[id].is_fixed = true;
  }
  
  void setImageHeight(int id, unsigned int h, bool fixed = false) {
    image_data[id].height = h;
    if (fixed) image_data[id].is_fixed = true;
  }

  const image_data_s & getImageData(int id) const {
    auto it = image_data.find(id);
    if (it != image_data.end()) {
      return it->second;
    } else {
      return null_image_data;
    }
  }

  static string getTextProperty(gpointer object, const char * key);
  static string getTextProperty(GtkContainer * c, GtkWidget * w, const char * key);

  static size_t getChildCount(GtkContainer * widget);
  
  static gboolean on_button_press(GtkWidget * widget, GdkEvent * event, gpointer user_data);
  static void send_int_value(GtkWidget * widget, gpointer data);
  static void send_bool_value(GtkWidget * widget, GParamSpec *pspec, gpointer data);
  static void send_toggled_value(GtkWidget * widget, gpointer user_data);
  static void send_combo_value(GtkWidget * widget, gpointer data);
  static void send_text_value(GtkWidget * widget, gpointer data);
  static void send_selection_value(GtkWidget * widget, gpointer data);
  static void send_activation_value(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * column, gpointer data);
  static void on_previous_button(GtkWidget * widget, gpointer data);
  static void on_settings_button(GtkWidget * widget, gpointer data);
  static void on_bar_button(GtkWidget * widget, gpointer data);
  static void on_size_allocate(GtkWidget * widget, GtkAllocation * allocation, gpointer data);
  static gboolean event_callback(gpointer data);
  static gboolean delete_window(GtkWidget *widget, GdkEvent  *event, gpointer user_data);
  static gboolean timer_callback(gpointer data);
  
private:
  GtkApplication * gtk_app = nullptr;
  GtkWidget * window = nullptr;
  GtkWidget * header = nullptr;
  GtkWidget * footer = nullptr;
  GtkWidget * stack = nullptr;
  unordered_map<int, GtkWidget *> views_by_id;
  unordered_map<int, PangoFontDescription *> fonts_by_id;
  bool initial_view_shown = false;
  unordered_map<int, vector<sheet_data_s> > sheets_by_id;
  vector<sheet_data_s> null_sheets;
  map<GtkWidget *, int> widget_values;
  unordered_map<int, image_data_s> image_data;
  image_data_s null_image_data;
  GKeyFile * keyFile;
  vector<int> dialog_stack;
};

string
GtkMainThread::getTextProperty(gpointer object, const char * key) {
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
GtkMainThread::getTextProperty(GtkContainer * c, GtkWidget * w, const char * key) {
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
GtkMainThread::getChildCount(GtkContainer * widget) {
  size_t n = 0;
  if (GTK_IS_CONTAINER(widget)) {
    auto children = gtk_container_get_children(GTK_CONTAINER(widget));
    n = g_list_length(children);
  }
  return n;
}

gboolean
GtkMainThread::on_button_press(GtkWidget * widget, GdkEvent * event, gpointer data) {
  cerr << "button press\n";
  GtkMainThread * mainThread = (GtkMainThread*)data;
  int id = mainThread->getId(widget);
  assert(id);
  if (id) {
    cerr << "int value: id = " << id << endl;
    ValueEvent ev(1);
    Element::postEventToElement(id, ev);
  }
  return TRUE; // stop propagation
}

void
GtkMainThread::send_int_value(GtkWidget * widget, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  int id = mainThread->getId(widget);
  assert(id);
  if (id) {
    cerr << "int value: id = " << id << endl;
    ValueEvent ev(1);
    Element::postEventToElement(id, ev);
  }
}

void
GtkMainThread::send_bool_value(GtkWidget * widget, GParamSpec *pspec, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  int id = mainThread->getId(widget);
  assert(id);
  if (id) {
    bool v;
    if (GTK_IS_SWITCH(widget)) {
      v = gtk_switch_get_active(GTK_SWITCH(widget));
    } else {
      assert(0);
    }
    cerr << "bool value: id = " << id << ", v = " << v << endl;
    ValueEvent ev(v ? 1 : 0);
    Element::postEventToElement(id, ev);
  }
}

void
GtkMainThread::send_combo_value(GtkWidget * widget, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  int id = mainThread->getId(widget);
  assert(id);
  if (id) {
    string v0 = gtk_combo_box_text_get_active_text((GtkComboBoxText*)widget);
    cerr << "combo value: id = " << id << ", v = " << v0 << endl;
    int v = atoi(v0.c_str());
    ValueEvent ev(v ? 1 : 0);
    Element::postEventToElement(id, ev);
  }
}

void
GtkMainThread::send_toggled_value(GtkWidget * widget, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  int id = mainThread->getId(widget);
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
    ValueEvent ev(v ? 1 : 0);
    Element::postEventToElement(id, ev);
  }
}

void
GtkMainThread::send_text_value(GtkWidget * widget, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  int id = mainThread->getId(widget);
  assert(id);
  if (id) {
    string s;
    if (GTK_IS_ENTRY(widget)) {
      s = gtk_entry_get_text((GtkEntry*)widget);
    } else {
      assert(0);
    }
    cerr << "text value: id = " << id << ", text = " << s << endl;
    ValueEvent ev(s);
    Element::postEventToElement(id, ev);
  }
}

void
GtkMainThread::send_selection_value(GtkWidget * widget, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  assert(GTK_IS_TREE_VIEW(widget));
  auto treeview = (GtkTreeView*)widget;
  auto selection = gtk_tree_view_get_selection(treeview);
  int id = mainThread->getId(gtk_widget_get_parent(widget));
  assert(id);
  if (id) {
    GtkTreeModel * model;
    GList * l = gtk_tree_selection_get_selected_rows(selection, &model);
    if (l) {
      assert(model);
      auto path = (GtkTreePath*)l->data;
      int depth;
      auto i = gtk_tree_path_get_indices_with_depth(path, &depth);
      assert(i);
      auto & sheets = mainThread->getSheetsById(id);

      int sheet = 0, row = 0;
      if (sheets.empty() && depth == 1) {
	row = i[0];
      } else if (!sheets.empty() && depth == 2) {
	sheet = i[0];
	row = i[1];
      } else {
	cerr << "unable to handle selection\n";
	return;
      }
      
      ValueEvent ev(row, sheet);
      Element::postEventToElement(id, ev);
      
      g_list_free_full(l, (GDestroyNotify)gtk_tree_path_free);
    }
  }
}

void
GtkMainThread::send_activation_value(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * column, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  // auto selection = gtk_tree_view_get_selection(treeview);
  int id = mainThread->getId(gtk_widget_get_parent(GTK_WIDGET(treeview)));
  assert(id);
  if (id) {
    int depth;
    auto i = gtk_tree_path_get_indices_with_depth(path, &depth);
    assert(i);
    auto & sheets = mainThread->getSheetsById(id);

    int sheet = 0, row = 0;
    if (sheets.empty() && depth == 1) {
      row = i[0];
    } else if (!sheets.empty() && depth == 2) {
      sheet = i[0];
      row = i[1];
    } else {
      cerr << "unable to handle selection\n";
      return;
    }
      
    ValueEvent ev(row, sheet);
    Element::postEventToElement(id, ev);
    
    // g_list_free_full(l, (GDestroyNotify)gtk_tree_path_free);
  }
}

void
GtkMainThread::on_previous_button(GtkWidget * widget, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  vector<Command> commands;
  commands.push_back(Command(Command::HISTORY_GO_BACK, 0));
  mainThread->sendCommands(commands);  
}

void
GtkMainThread::on_settings_button(GtkWidget * widget, gpointer data) {
  cerr << "got settings\n";
  GtkMainThread * mainThread = (GtkMainThread*)data;
  SysEvent ev(SysEvent::SHOW_DEBUG);
  Element::postEventToElement(mainThread->getApplication().getInternalId(), ev);
}

void
GtkMainThread::on_bar_button(GtkWidget * widget, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  int id = 0;
  for (GtkWidget * w = widget; !id && w; w = gtk_widget_get_parent(w)) {
    // cerr << "trying to get id from " << (long long)w << endl;
    id = mainThread->getId(w);
  }
  assert(id);
  if (id) {
    int value = mainThread->getValue(widget);
    cerr << "sending bar button click value: " << value << endl;
    ValueEvent ev(value, value);
    Element::postEventToElement(id, ev);
  }
}

void
GtkMainThread::on_size_allocate(GtkWidget * widget, GtkAllocation * allocation, gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  if (GTK_IS_IMAGE(widget)) {
    int id = mainThread->getId(widget);
    auto & d = mainThread->getImageData(id);
    if (d.width != allocation->width) {
      cerr << "imageSize changed " << allocation->width << ", " << allocation->height << endl;
      mainThread->setImageWidth(id, allocation->width);
      mainThread->setImageHeight(id, allocation->height);
      if (!d.images.empty()) {
	auto & image = d.images.getSuitable(allocation->width);
	mainThread->requestImage(id, image.url, allocation->width);
      } else {
	mainThread->requestImage(id, allocation->width, allocation->height);
      }
    }
  }
}

gboolean
GtkMainThread::event_callback(gpointer data) {
  event_data_s * ed = (event_data_s*)data;
  GtkMainThread * mainThread = ed->mainThread;
  Event * ev = ed->event;
  int internal_id = ed->internal_id;
  delete ed;
  Element::postEventToElement(internal_id ? internal_id : mainThread->getApplication().getInternalId(), *ev);
  delete ev;
  return FALSE;
}

gboolean
GtkMainThread::delete_window(GtkWidget *widget, GdkEvent  *event, gpointer user_data) {
  GtkMainThread * mainThread = (GtkMainThread*)user_data;
  if (!mainThread->terminate()) {
    return TRUE;
  } else {
    return FALSE;
  }
}

gboolean
GtkMainThread::timer_callback(gpointer data) {
  GtkMainThread * mainThread = (GtkMainThread*)data;
  TimerEvent ev(0);
  Element::postEventToElement(mainThread->getApplication().getInternalId(), ev);
  return TRUE;
}

static void activate(GtkApplication * gtk_app, gpointer user_data) {
  cerr << "activate\n";
  mainThread->activate(gtk_app);

  auto & app = mainThread->getApplication();
  app.initialize(mainThread);
}

int main (int argc, char *argv[]) {
  std::shared_ptr<FWApplication> application(applicationMain());
			      
  cerr << "creating mainloop\n";
  
  mainThread = make_shared<GtkMainThread>(application, application);
  mainThread->setActualDisplayWidth(DEFAULT_DISPLAY_WIDTH);
  mainThread->setActualDisplayHeight(DEFAULT_DISPLAY_HEIGHT);
  
  auto gtk_app = gtk_application_new(application->getName().c_str(), G_APPLICATION_FLAGS_NONE);
  g_signal_connect (gtk_app, "activate", G_CALLBACK (activate), 0);
  int status = g_application_run(G_APPLICATION(gtk_app), argc, argv);
  g_object_unref(gtk_app);
    
#if 0
  SysEvent ev(SysEvent::DESTROY);
  Element::postEventToElement(application->getInternalId(), ev);
#endif

  mainThread.reset();
  
  return status;
}

