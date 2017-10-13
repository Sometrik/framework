#include <FWApplication.h>

#include <Dialog.h>
#include <GridView.h>
#include <TableLayout.h>
#include <PlatformThread.h>
#include <SysEvent.h>
#include <LinearLayout.h>

using namespace std;

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
      c.setValue(2);
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
