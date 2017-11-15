#include <FWApplication.h>

#include <Dialog.h>
#include <GridView.h>
#include <TableLayout.h>
#include <PlatformThread.h>
#include <SysEvent.h>
#include <LinearLayout.h>
#include <TextLabel.h>
#include <TextField.h>
#include <Button.h>

using namespace std;

class AppMessageDialog : public Dialog {
 public:
  AppMessageDialog(const std::string & title, const std::string & message) : Dialog(title) {
    style("height", "wrap-content");
    auto mainLayout = make_shared<LinearLayout>(1);
    mainLayout->style("width", "match-parent");
    mainLayout->style("height", "wrap-content");
    addChild(mainLayout);

    auto dialogMessage = make_shared<TextLabel>(message);
    mainLayout->addChild(dialogMessage);

    auto okButton = std::make_shared<Button>("OK");
  }

  bool isA(const std::string & className) const override {
    if (className == "AppMessageDialog") return true;
    return Dialog::isA(className);
  }

  void onCommandEvent(CommandEvent & ev) override {
    endModal();
  }
};

class AppInputDialog : public Dialog {
 public:
  AppInputDialog(const std::string & title, const std::string & message) : Dialog(title) {
    style("height", "wrap-content");
    auto mainLayout = make_shared<LinearLayout>(1);
    mainLayout->style("width", "match-parent");
    mainLayout->style("height", "wrap-content");
    addChild(mainLayout);

    auto dialogTitle = std::make_shared<TextLabel>(title);
    mainLayout->addChild(dialogTitle);

    auto dialogMessage = make_shared<TextLabel>(message);
    mainLayout->addChild(dialogMessage);

    textField = make_shared<TextField>();
    mainLayout->addChild(textField);

    auto buttonLayout = make_shared<LinearLayout>(2);
    auto okButton = std::make_shared<Button>("OK", 1);
    buttonLayout->addChild(okButton);
    auto cancelButton = std::make_shared<Button>("Cancel", 0);
    buttonLayout->addChild(cancelButton);

    mainLayout->addChild(buttonLayout);
  }

  bool isA(const std::string & className) const override {
    if (className == "AppInputDialog") return true;
    return Dialog::isA(className);
  }

  void onCommandEvent(CommandEvent & ev) override {
    if (ev.getElementId() == 1) {
      endModal(1);
    } else {
      endModal(0);
    }
  }

  const std::string & getValue() { return textField->getValue(); }

 private:
  std::shared_ptr<TextField> textField;
};

class DebugDialog : public Dialog {
public:
  DebugDialog() : Dialog("Debug") {
    auto mainLayout = make_shared<LinearLayout>(FW_VERTICAL);
    addChild(mainLayout);

    mainLayout->addText("Debug screen").style("font-size", "14")
      .style("white-space", "nowrap")
      .style("margin", 5);

    mainLayout->addText("Stuff").style("font-size", "12").style("margin", 5);

    
    auto table = make_shared<TableLayout>(2);
    table->style("margin", 5);
    mainLayout->addChild(table);

    mainLayout->addText("Threads").style("font-size", "12").style("margin", 5);

    auto grid = make_shared<GridView>();
    grid->style("margin", 5);
    grid->addColumn("Runnable");
    grid->addColumn("State");
    mainLayout->addChild(grid);
  }

  void load() {
    auto & grid = find("GridView").front();
    populateThreads(dynamic_cast<GridView&>(grid), getThread());
  }

protected:
  void populateThreads(GridView & grid, PlatformThread & thread) {
    string runnable_name, runnable_status;
    auto runnable = thread.getRunnablePtr();
    if (runnable) {
      runnable_name = typeid(*runnable).name();
      runnable_status = runnable->getStatusText();
    }
    grid.setValue(numThreadRows, 0, runnable_name);
    grid.setValue(numThreadRows, 1, runnable_status);
    numThreadRows++;
    for (auto & td : thread.getSubThreads()) {
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
      Command c(Command::SET_INT_VALUE, poppedView);
      c.setValue(3);
      sendCommand(c);
    } else {
      Command c(Command::QUIT_APP, poppedView);
      sendCommand(c);
    }
  } else if (ev.getType() == SysEvent::DEBUG) {
    auto dialog = make_shared<DebugDialog>();
    dialog->showModal(this);
  }
}

Logger &
FWApplication::getLogger() {
  if (!logger.get()) {
    logger = getThread().createLogger(name);
  }
  return *logger;
}

void
FWApplication::showMessageDialog(const std::string & title, const std::string & message) {
  auto dialog = make_shared<AppMessageDialog>(title, message);
  dialog->showModal(this);
}

std::string
FWApplication::showInputDialog(const std::string & title, const std::string & message) {
  auto dialog = make_shared<AppInputDialog>(title, message);
  if (dialog->showModal(this)) {
    return dialog->getValue();
  } else {
    return "";
  }
}
