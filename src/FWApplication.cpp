#include <FWApplication.h>

#include <Dialog.h>
#include <GridView.h>
#include <TableLayout.h>
#include <PlatformThread.h>
#include <SysEvent.h>
#include <LinearLayout.h>
#include <FrameLayout.h>
#include <TextLabel.h>
#include <TextField.h>
#include <Button.h>
#include <SysInfoEvent.h>
#include <Switch.h>

using namespace std;

class AppMessageDialog : public Dialog {
 public:
  AppMessageDialog(const std::string & title, const std::string & message) : Dialog(title) {
    style("height", "wrap-content");

    auto topLayout = make_shared<FrameLayout>();
    addChild(topLayout);

    auto mainLayout = make_shared<LinearLayout>(1);
    mainLayout->style("width", "match-parent");
    mainLayout->style("height", "wrap-content");
    topLayout->addChild(mainLayout);

    auto dialogMessage = make_shared<TextLabel>(message);
    mainLayout->style("margin", "16");
    mainLayout->style("weight", 0);
    mainLayout->addChild(dialogMessage);

    auto okButton = std::make_shared<Button>("OK");
    okButton->style("width", "match-parent");
    okButton->style("height", "match-parent");
    okButton->style("color", "#ffffff");
    okButton->style("background", "#c1272d");
    okButton->style("border-radius", "4");
    okButton->style("weight", "0");
    okButton->style("margin-left", "16");
    okButton->style("margin-right", "16");
    okButton->style("margin-bottom", "16");
    mainLayout->addChild(okButton);
  }

  bool isA(const std::string & className) const override {
    if (className == "AppMessageDialog") return true;
    return Dialog::isA(className);
  }

  void onCommandEvent(CommandEvent & ev) override {
    ev.setHandled(true);
    endModal();
  }
};

class AppInputDialog : public Dialog {
 public:
  AppInputDialog(const std::string & title, const std::string & message) : Dialog(title) {
    style("height", "wrap-content");

    auto topLayout = make_shared<FrameLayout>();
    addChild(topLayout);

    auto mainLayout = make_shared<LinearLayout>(1);
    mainLayout->style("width", "match-parent");
    mainLayout->style("height", "wrap-content");
    mainLayout->style("background-color", "#ffffff");
    mainLayout->style("margin", 4);

    topLayout->addChild(mainLayout);

//    auto dialogTitle = std::make_shared<TextLabel>(title);
//    dialogTitle->style("background-color", "#ffffff");
//    dialogTitle->style("width", "match-parent");
//    dialogTitle->style("gravity", "center-horizontal");
//    dialogTitle->style("height", "wrap-content");
//    dialogTitle->style("font-size", "24");
//    dialogTitle->style("padding-top", "12");
//    dialogTitle->style("padding-bottom", "16");
//    dialogTitle->style("font-weight", "bold");
//    dialogTitle->style("padding-left", "14");
//    dialogTitle->style("padding-right", "14");
//    dialogTitle->style("color", "#c1272d");
//    mainLayout->addChild(dialogTitle);

    auto dialogMessage = make_shared<TextLabel>(message);
    dialogMessage->style("padding", "14");
    mainLayout->addChild(dialogMessage);

    textField = make_shared<TextField>();
    textField->style("width", "match-parent");
    textField->style("min-width", "100");
    textField->style("padding-bottom", "10");
    textField->style("padding-top", "10");
    textField->style("hint", "Enter code here");
    textField->style("padding-left", "14");
    textField->style("margin-bottom", 4);
    mainLayout->addChild(textField);

    auto buttonLayout = make_shared<LinearLayout>(2);

    auto okButton = std::make_shared<Button>("OK", 1);
    okButton->style("width", "match-parent");
    okButton->style("height", "match-parent");
    okButton->style("color", "#ffffff");
    okButton->style("background", "#c1272d");
    okButton->style("border-radius", "4");
    okButton->style("weight", "1");
    okButton->style("margin-left", "4");
    okButton->style("margin-right", "2");
    okButton->style("margin-bottom", "4");
    buttonLayout->addChild(okButton);
    auto cancelButton = std::make_shared<Button>("Cancel", 2);
    cancelButton->style("width", "match-parent");
    cancelButton->style("height", "match-parent");
    cancelButton->style("color", "#ffffff");
    cancelButton->style("background", "#c1272d");
    cancelButton->style("border-radius", "4");
    cancelButton->style("weight", "1");
    cancelButton->style("margin-left", "4");
    cancelButton->style("margin-right", "2");
    cancelButton->style("margin-bottom", "4");
    buttonLayout->addChild(cancelButton);

    mainLayout->addChild(buttonLayout);
  }

  bool isA(const std::string & className) const override {
    if (className == "AppInputDialog") return true;
    return Dialog::isA(className);
  }

  void onCommandEvent(CommandEvent & ev) override {
    ev.setHandled(true);
    if (ev.getElementId() == 1) {
      endModal(1);
    } else if (ev.getElementId() == 2) {
      endModal(0);
    }
  }

  const std::string & getValue() { return textField->getValue(); }

 private:
  std::shared_ptr<TextField> textField;
};

class DebugDialog : public Dialog {
public:
  DebugDialog(bool debug_enabled) : Dialog("Debug") {
    auto mainLayout = make_shared<LinearLayout>(FW_VERTICAL);
    addChild(mainLayout);    

    mainLayout->addChild("Debug screen").style("font-size", "14")
      .style("white-space", "nowrap")
      .style("margin", 5);

    auto & row = mainLayout->addChild(make_shared<LinearLayout>(FW_HORIZONTAL));
    row.style("width", "match-parent");
    row.addChild("Debug mode enabled");
    row.addChild(make_shared<Switch>(-100, debug_enabled));

    auto & row2 = mainLayout->addChild(make_shared<LinearLayout>(FW_HORIZONTAL));
    row2.style("width", "match-parent");
    row2.addChild("Bytes downloaded");
    row2.addChild(to_string(0));

    auto & row3 = mainLayout->addChild(make_shared<LinearLayout>(FW_HORIZONTAL));
    row3.style("width", "match-parent");
    row3.addChild("Bytes uploaded");
    row3.addChild(to_string(0));
    
    mainLayout->addChild(make_shared<TextLabel>("Threads")).style("font-size", "12").style("margin", 5);

    auto grid = make_shared<GridView>();
    grid->style("margin", 5);
    grid->addColumn("Runnable");
    grid->addColumn("State");
    mainLayout->addChild(grid);
  }

  void onCommandEvent(CommandEvent & ev) override {
    ev.setHandled(true);
    if (ev.getElementId() == -100) {
      getApplication().setDebugModeEnabled(ev.getValue());

      SysInfoEvent ev2(SysInfoEvent::DEBUG_MODE);
      ev2.setValue(ev.getValue() ? 1 : 0);
      ev2.dispatch(getApplication());
    }
  }

  void load() override {
    auto t = getThreadPtr();
    if (t) {
      auto & grid = find("GridView").front();
      populateThreads(dynamic_cast<GridView&>(grid), *t);
    }
  }

protected:
  void populateThreads(GridView & grid, PlatformThread & t) {
    string runnable_name, runnable_status;
    auto runnable = t.getRunnablePtr();
    if (runnable) {
      runnable_name = runnable->getName();
      auto s = runnable->getStatus();
      runnable_status = s.first;
    }
    grid.setValue(numThreadRows, 0, runnable_name);
    grid.setValue(numThreadRows, 1, runnable_status);
    numThreadRows++;
    for (auto & td : t.getSubThreads()) {
      populateThreads(grid, *td.second);
    }
  }

private:
  int numThreadRows = 0;
};

void
FWApplication::onSysEvent(SysEvent & ev) {
  if (ev.getType() == SysEvent::BACK) {
    int poppedView = popViewBackHistory();
    if (poppedView != 0) {
#ifdef __ANDROID__
      Command c(Command::SET_INT_VALUE, poppedView);
      c.setValue(3);
      sendCommand(c);
#else
      Element * e = getRegisteredElement(poppedView);
      if (e) e->show();
#endif
    } else {
      Command c(Command::QUIT_APP, poppedView);
      sendCommand(c);
    }
  } else if (ev.getType() == SysEvent::SHOW_DEBUG) {
    auto dialog = make_shared<DebugDialog>(getApplication().isDebugModeEnabled());
    showModal(dialog);
  }
}

void
FWApplication::showMessageDialog(const std::string & title, const std::string & message) {
  auto dialog = make_shared<AppMessageDialog>(title, message);
  showModal(dialog);
}

std::string
FWApplication::showInputDialog(const std::string & title, const std::string & message) {
  auto dialog = make_shared<AppInputDialog>(title, message);
  if (showModal(dialog)) {
    return dialog->getValue();
  } else {
    return "";
  }
}

void
FWApplication::run() {
  auto t = getThreadPtr();
  if (t.get()) {
    t->startEventLoop();
#if 0
    deinitializeRenderer();
#endif
    Command c(Command::QUIT_APP, getInternalId());
    t->sendCommand(c);
  }
}
