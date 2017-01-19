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

#include <gtk/gtk.h>

#include <cstdio>
#include <cstdlib>

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
    
  }
  
  string getLocalFilename(const char * fn, FileType type) override {
    string s = "assets/";
    return s + fn;
  }
  
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::make_shared<CurlClientFactory>();
  }
    
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::shared_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }

  void sendCommand2(const Command & command) override {
    if (command.getType() == Command::SHOW_VIEW) {
      setActiveView(command.getInternalId());
    } else if (!getActiveViewId() && (command.getType() == Command::CREATE_FORMVIEW || command.getType() == Command::CREATE_OPENGL_VIEW)) {
      setActiveView(command.getChildInternalId());
    }
    
    switch (command.getType()) {
    case Command::CREATE_PLATFORM:
      cerr << "ignore CREATE_PLATFORM\n";
      break;
      
    case Command::CREATE_APPLICATION: {
      cerr << "creating stack\n";
      stack = gtk_stack_new();
      // ignore parent id
      addView(0, command.getChildInternalId(), stack);
      assert(window);
      gtk_container_add(GTK_CONTAINER(window), stack);
    }
      break;
      
    case Command::CREATE_TEXTFIELD: {
      auto textfield = gtk_entry_new();
      addView(command, textfield);
    }
      break;

    case Command::CREATE_PICKER: {
      auto picker = gtk_combo_box_new();
      addView(command, picker);
    }
      break;
      
    case Command::CREATE_BUTTON: {
      auto button = gtk_button_new_with_label(command.getTextValue().c_str());
      addView(command, button);
    }
      break;
      
    case Command::CREATE_CHECKBOX: {
      auto cb = gtk_check_button_new_with_label(command.getTextValue().c_str());
      addView(command, cb);
    }
      break;
      
    case Command::CREATE_TEXT:
    case Command::CREATE_HEADING_TEXT: {
      auto label = gtk_label_new(command.getTextValue().c_str());
      addView(command, label);
    }
      break;
      
    case Command::CREATE_LINEAR_LAYOUT: {
      auto box = gtk_box_new(command.getValue() == 1 ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 5); // FIXME: spacing
      addView(command, box);
    }
      break;
	
    case Command::CREATE_TABLE_LAYOUT: {
      auto layout = gtk_grid_new();
      addView(command, layout);
    }
      break;

    case Command::CREATE_ACTIONBAR: {
      if (!header) {
	cerr << "creating actionbar\n";
	header = gtk_header_bar_new();
	gtk_header_bar_set_title((GtkHeaderBar*)header, "Moi!");
	gtk_header_bar_set_subtitle((GtkHeaderBar*)header, "Aliotsikko");
	gtk_header_bar_set_show_close_button((GtkHeaderBar*)header, 1);
	gtk_widget_show(header);
	gtk_window_set_titlebar(GTK_WINDOW(window), header);
      }
    }
      break;

    case Command::CREATE_SWITCH: {
      auto s = gtk_switch_new();
      addView(command, s);
    }
      break;

    case Command::CREATE_IMAGEVIEW: {
      string s = "assets/" + command.getTextValue();
      auto image = gtk_image_new_from_file(s.c_str());
      addView(command, image);
    }
      break;
      
    case Command::SET_INT_VALUE:
      break;
      
    case Command::SET_TEXT_VALUE:
      // gtk_label_set_text(label, str)
      break;

    case Command::SET_ENABLED: {
      auto view = views_by_id[command.getInternalId()];
      gtk_widget_set_sensitive(view, command.getValue() ? 1 : 0);
      break;
    }
      
    case Command::SHOW_MESSAGE_DIALOG:
      break;
      
    case Command::CREATE_TIMER:
      break;
      
    case Command::CREATE_FORMVIEW: {
      cerr << "creating formview\n";
      auto box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // FIXME: spacing
      
      addView(0, command.getChildInternalId(), box);
      string s = "form" + to_string(command.getChildInternalId());
      gtk_stack_add_named((GtkStack *)stack, box, s.c_str());

      if (!initial_view_shown) {
	gtk_stack_set_visible_child((GtkStack*)stack, box);
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
      } else {
	gtk_container_add(parent, widget);
      }
    }
    views_by_id[id] = widget;
  }

  void addView(const Command & c, GtkWidget * widget) {
    addView(c.getInternalId(), c.getChildInternalId(), widget);
  }
  
private:
  GtkApplication * app = nullptr;
  GtkWidget * window = nullptr;
  GtkWidget * header = nullptr;
  GtkWidget * stack = nullptr;
  unordered_map<int, GtkWidget *> views_by_id;
  bool initial_view_shown = false;
};

static void activate(GtkApplication * app, gpointer user_data) {
  cerr << "activate\n";
  PlatformGtk * platform = (PlatformGtk*)user_data;
  platform->activate(app);
  FWApplication * application = applicationMain();
  platform->addChild(std::shared_ptr<Element>(application));  
}

int main (int argc, char *argv[]) {
  GtkApplication * app;
  int status;

  PlatformGtk platform;  
  platform.setDisplayWidth(width);
  platform.setDisplayHeight(height);

  cerr << "creating application\n";
  
  app = gtk_application_new("com.sometrik.test", G_APPLICATION_FLAGS_NONE); // FIXME: add correct name
  g_signal_connect (app, "activate", G_CALLBACK (activate), &platform);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

#if 0
  SysEvent ev(platform.getTime(), SysEvent::DESTROY);
  platform.postEvent(application->getInternalId(), ev);
#endif
  
  return status;
}

