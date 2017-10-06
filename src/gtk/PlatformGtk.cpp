#include <FWPlatform.h>
#include <CurlClient.h>
#include <Logger.h>
#include <ContextCairo.h>
#include <SDLSoundCanvas.h>
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

#include <PosixThread.h>

#include <gtk/gtk.h>
#include <gio/gio.h>

#include <unordered_map>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <fstream>

using namespace std;

struct sheet_data_s {
  string name;
  bool is_created;
};

extern FWApplication * applicationMain();

static int width = 800, height = 600;

class PlatformGtk;

struct event_data_s {
  PlatformGtk * platform;
  Event * event;
  int internal_id;
};

class PlatformGtk : public FWPlatform {
public:
  friend class GtkThread;
  friend class GtkMainThread;
  
  PlatformGtk() {
    CurlClientFactory::globalInit();
    registerElement(this);
  }

  void pushEvent(int internal_id, const Event & ev) override {
    event_data_s * ed = new event_data_s;
    ed->platform = this;
    ed->event = ev.dup();
    ed->internal_id = internal_id;
    g_idle_add(idle_callback, ed);
  }
  
#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    return std::make_shared<SDLSoundCanvas>();
  }
#endif

  void startModal() override { }

  void endModal() override { }
  
  void activate(GtkApplication * _gtk_app) {
    gtk_app = _gtk_app;
        
    window = gtk_application_window_new(gtk_app);
    gtk_window_set_title (GTK_WINDOW(window), "Window");
    gtk_window_set_default_size (GTK_WINDOW(window), width, height);
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_window), this);
  }

protected:
  int handleCommand(const Command & command) {
    if (command.getType() == Command::CREATE_FORMVIEW || command.getType() == Command::CREATE_OPENGL_VIEW) {
      auto & app = getApplication();
      if (!app.getActiveViewId()) {
	app.setActiveViewId(command.getChildInternalId());
      }
    }
    
    switch (command.getType()) {
    case Command::CREATE_PLATFORM:
      cerr << "ignore CREATE_PLATFORM\n";
      break;

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

    case Command::CREATE_SIMPLELISTVIEW: {
      auto box = gtk_box_new(command.getValue() == 1 ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 5); // FIXME: spacing
      addView(command, box);
    }
      break;

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
	auto next = gtk_button_new_from_icon_name("go-next", GTK_ICON_SIZE_BUTTON);
	g_signal_connect(previous, "clicked", G_CALLBACK(on_previous_button), this);
	g_signal_connect(next, "clicked", G_CALLBACK(on_next_button), this);

	gtk_box_pack_start((GtkBox*)box, previous, 0, 0, 0);
	gtk_box_pack_start((GtkBox*)box, next, 0, 0, 0);
	
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
      string s = "assets/" + command.getTextValue();
      auto image = gtk_image_new_from_file(s.c_str());
      addView(command, image);
    }
      break;

    case Command::HISTORY_GO_BACK:
    case Command::HISTORY_GO_FORWARD: {
      auto & app = getApplication();
      int id;
      if (command.getType() == Command::HISTORY_GO_BACK) {
	id = app.popViewBackHistory();
      } else {
	id = app.popViewForwardHistory();
      }
      if (id) {
	cerr << "id = " << id << endl;
	auto view = views_by_id[id];
	if (view) {
	  cerr << "found view\n";
	  auto & app = getApplication();
	  app.setActiveViewId(id);
	  gtk_stack_set_visible_child((GtkStack*)stack, view);

	  if (header) {
	    string title;
	    auto e = getRegisteredElement(id);
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
      } else if (gtk_widget_get_parent(view) == stack) {
	auto & app = getApplication();
	app.addToHistory(app.getActiveViewId());
	app.setActiveViewId(command.getInternalId());
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
	assert(GTK_IS_TREE_VIEW(treeview));
	if (GTK_IS_TREE_VIEW(treeview)) {
	  auto model = gtk_tree_view_get_model((GtkTreeView*)view);
	  gtk_tree_store_clear((GtkTreeStore*)model);
	}
      } else if (GTK_IS_ENTRY(view)) {
	gtk_entry_set_text((GtkEntry*)view, "");
      }
    }
      break;
    
    case Command::SET_ENABLED: {
      auto view = views_by_id[command.getInternalId()];
      if (view) gtk_widget_set_sensitive(view, command.getValue() ? 1 : 0);
    }
      break;

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
    }
      break;

    case Command::END_MODAL: {
      auto it = views_by_id.find(command.getInternalId());
      assert(it != views_by_id.end());
      if (it != views_by_id.end()) {
	auto dialog = it->second;
	gtk_dialog_response(GTK_DIALOG(dialog), 0);
      }
    }
      break;
      
    case Command::SHOW_DIALOG: {
      auto it = views_by_id.find(command.getInternalId());
      assert(it != views_by_id.end());
      if (it != views_by_id.end()) {
	auto dialog = it->second;
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	views_by_id.erase(command.getInternalId());
      }
    }
      break;
      
    case Command::SHOW_MESSAGE_DIALOG: {
      GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
      auto dialog = gtk_message_dialog_new(GTK_WINDOW(window),
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

    case Command::SHOW_INPUT_DIALOG: {
      GtkDialogFlags flags = GtkDialogFlags(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT);
      auto dialog = gtk_dialog_new_with_buttons(command.getTextValue().c_str(),
						GTK_WINDOW(window),
						flags,
						"_OK",
						GTK_RESPONSE_ACCEPT,
						"_Cancel",
						GTK_RESPONSE_REJECT,
						0
						);

      auto vbox = gtk_vbox_new(TRUE, TRUE);
      
      auto label = gtk_label_new(command.getTextValue2().c_str());
      gtk_widget_show(label);
 
      auto entry = gtk_entry_new ();
      gtk_widget_show(entry);

      gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
      gtk_box_pack_start (GTK_BOX(vbox), entry, TRUE, TRUE, 0);
      gtk_widget_show (vbox);

      auto a = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
      gtk_container_add(GTK_CONTAINER(a), vbox);

      gint result = gtk_dialog_run(GTK_DIALOG (dialog));
      if (result == GTK_RESPONSE_ACCEPT) {
	getThread().setModalResultValue(1);
	getThread().setModalResultText(gtk_entry_get_text((GtkEntry*)entry));
      } else {
	getThread().setModalResultValue(0);
	getThread().setModalResultText("");
      }
      gtk_widget_destroy (dialog);
      
      return getThread().getModalResultValue();
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
            
    case Command::CREATE_FORMVIEW: {
      auto sw = gtk_scrolled_window_new(0, 0);
      gtk_container_set_border_width((GtkContainer*)sw, 10);
      gtk_scrolled_window_set_policy((GtkScrolledWindow*)sw,
				     GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
      addView(0, command.getChildInternalId(), sw);
      string s = "form" + to_string(command.getChildInternalId());
      gtk_stack_add_named((GtkStack *)stack, sw, s.c_str());
      
      if (!initial_view_shown) {
	if (header) {
	  gtk_header_bar_set_subtitle((GtkHeaderBar*)header, command.getTextValue().c_str());
	}
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
    return 0;
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
	gtk_box_pack_start((GtkBox*)parent, widget, expand ? 1 : 0, 0, 5);
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
      } else {
	gtk_container_add(parent, widget);
      }
    }
    views_by_id[id] = widget;
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

  static string getTextProperty(gpointer object, const char * key);
  static string getTextProperty(GtkContainer * c, GtkWidget * w, const char * key);

  static size_t getChildCount(GtkContainer * widget);
  
  static void send_int_value(GtkWidget * widget, gpointer data);
  static void send_bool_value(GtkWidget * widget, GParamSpec *pspec, gpointer data);
  static void send_toggled_value(GtkWidget * widget, gpointer user_data);
  static void send_combo_value(GtkWidget * widget, gpointer data);
  static void send_text_value(GtkWidget * widget, gpointer data);
  static void send_selection_value(GtkWidget * widget, gpointer data);
  static void send_activation_value(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * column, gpointer data);
  static void on_previous_button(GtkWidget * widget, gpointer data);
  static void on_next_button(GtkWidget * widget, gpointer data);
  static void on_bar_button(GtkWidget * widget, gpointer data);
  static gboolean idle_callback(gpointer data);
  static gboolean delete_window(GtkWidget *widget, GdkEvent  *event, gpointer user_data);
  static gboolean timer_callback(gpointer data);
  
private:
  GtkApplication * gtk_app = nullptr;
  GtkWidget * window = nullptr;
  GtkWidget * header = nullptr;
  GtkWidget * footer = nullptr;
  GtkWidget * stack = nullptr;
  unordered_map<int, GtkWidget *> views_by_id;
  bool initial_view_shown = false;
  unordered_map<int, vector<sheet_data_s> > sheets_by_id;
  vector<sheet_data_s> null_sheets;
  map<GtkWidget *, int> widget_values;
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
    ValueEvent ev(1);
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
    ValueEvent ev(v ? 1 : 0);
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
    ValueEvent ev(v ? 1 : 0);
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
    ValueEvent ev(v ? 1 : 0);
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
    ValueEvent ev(s);
    platform->postEvent(id, ev);
  }
}

void
PlatformGtk::send_selection_value(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  assert(GTK_IS_TREE_VIEW(widget));
  auto treeview = (GtkTreeView*)widget;
  auto selection = gtk_tree_view_get_selection(treeview);
  int id = platform->getId(gtk_widget_get_parent(widget));
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
      auto & sheets = platform->getSheetsById(id);

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
      platform->postEvent(id, ev);
      
      g_list_free_full(l, (GDestroyNotify)gtk_tree_path_free);
    }
  }
}

void
PlatformGtk::send_activation_value(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * column, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  // auto selection = gtk_tree_view_get_selection(treeview);
  int id = platform->getId(gtk_widget_get_parent(GTK_WIDGET(treeview)));
  assert(id);
  if (id) {
    int depth;
    auto i = gtk_tree_path_get_indices_with_depth(path, &depth);
    assert(i);
    auto & sheets = platform->getSheetsById(id);

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
    platform->postEvent(id, ev);
    
    // g_list_free_full(l, (GDestroyNotify)gtk_tree_path_free);
  }
}

void
PlatformGtk::on_previous_button(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  platform->handleCommand(Command(Command::HISTORY_GO_BACK, 0));  
}

void
PlatformGtk::on_next_button(GtkWidget * widget, gpointer data) {
  cerr << "got next\n";
  PlatformGtk * platform = (PlatformGtk*)data;
  platform->handleCommand(Command(Command::HISTORY_GO_FORWARD, 0));  
}

void
PlatformGtk::on_bar_button(GtkWidget * widget, gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  int id = 0;
  for (GtkWidget * w = widget; !id && w; w = gtk_widget_get_parent(w)) {
    // cerr << "trying to get id from " << (long long)w << endl;
    id = platform->getId(w);
  }
  assert(id);
  if (id) {
    int value = platform->getValue(widget);
    cerr << "sending bar button click value: " << value << endl;
    ValueEvent ev(value, value);
    platform->postEvent(id, ev);
  }
}

gboolean
PlatformGtk::idle_callback(gpointer data) {
  event_data_s * ed = (event_data_s*)data;
  PlatformGtk * platform = ed->platform;
  Event * ev = ed->event;
  int internal_id = ed->internal_id;
  delete ed;
  platform->postEvent(internal_id ? internal_id : platform->getInternalId(), *ev);
  delete ev;
  return FALSE;
}

gboolean
PlatformGtk::delete_window(GtkWidget *widget, GdkEvent  *event, gpointer user_data) {
  PlatformGtk * platform = (PlatformGtk*)user_data;
  auto & thread = platform->getThread();
  thread.terminateThreads();
  if (thread.getNumRunningThreads()) {
    // platform->exit_when_threads_terminated = true; // FIXME
    return TRUE;
  } else {
    return FALSE;
  }
}

gboolean
PlatformGtk::timer_callback(gpointer data) {
  PlatformGtk * platform = (PlatformGtk*)data;
  TimerEvent ev(0);
  platform->postEvent(platform->getInternalId(), ev);
  return TRUE;
}

class GtkThread : public PosixThread {
public:
  GtkThread(PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
    : PosixThread(_parent_thread, _platform, _runnable) { }

  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }
  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return make_shared<GtkThread>(this, &(getPlatform()), runnable);
  }
  int sendCommand(const Command & command) {
    auto & gtkPlatform = dynamic_cast<PlatformGtk&>(getPlatform());
    return gtkPlatform.handleCommand(command);
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
};

class GtkMainThread : public PlatformThread {
public:
  GtkMainThread(FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : PlatformThread(0, _platform, _runnable)
  {
    
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

  bool start() override { return false; }
  bool testDestroy() override { return false; }
  void terminate() override { }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return make_shared<GtkThread>(this, &(getPlatform()), runnable);
  }
  int sendCommand(const Command & command) {
    auto & gtkPlatform = dynamic_cast<PlatformGtk&>(getPlatform());
    return gtkPlatform.handleCommand(command);
  }

  void sleep(double t) override {
    usleep((unsigned int)(t * 1000000));
  }

  void sendEvent(int internal_id, const Event & ev) {

  }

  void recvEvents(EventHandler & evh) override {

  }

  std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents() {
    std::vector<std::pair<int, std::shared_ptr<Event> > > r;
    return r;
  }
};

static void activate(GtkApplication * gtk_app, gpointer user_data) {
  cerr << "activate\n";
  PlatformGtk * platform = (PlatformGtk*)user_data;

  std::shared_ptr<Runnable> runnable(0);
  auto mainThread = new GtkMainThread(platform, runnable);
  mainThread->setActualDisplayWidth(width);
  mainThread->setActualDisplayHeight(height);

  platform->create();
  platform->initialize(mainThread);
  platform->initializeChildren();
  platform->load();

  platform->activate(gtk_app);

  FWApplication * application = applicationMain();
  platform->addChild(std::shared_ptr<Element>(application));  
}

int main (int argc, char *argv[]) {
  GtkApplication * gtk_app;

  PlatformGtk platform;  

  cerr << "creating application\n";
  
  gtk_app = gtk_application_new("com.sometrik.test", G_APPLICATION_FLAGS_NONE); // FIXME: add correct name
  g_signal_connect (gtk_app, "activate", G_CALLBACK (activate), &platform);
  int status = g_application_run (G_APPLICATION(gtk_app), argc, argv);
  g_object_unref(gtk_app);
    
#if 0
  SysEvent ev(SysEvent::DESTROY);
  platform.postEvent(application->getInternalId(), ev);
#endif
  
  return status;
}

