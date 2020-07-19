#include <Runnable.h>
#include <Logger.h>

#include <PlatformThread.h>

#include <exception>

using namespace std;

Logger &
Runnable::getLogger() {
  if (!logger.get()) {
    auto thread = getThreadPtr();
    if (thread) {
      logger = thread->createLogger(getName());
    } else {
      logger = make_shared<DummyLogger>();
    }
  }
  return *logger;
}

std::string
Runnable::getName() const {
  return typeid(*this).name();
}

void
Runnable::start(std::shared_ptr<PlatformThread> _thread) {
  try {
    initialize(_thread);
    run();
  } catch (std::exception & e) {
    getLogger().println("exception(Runnable): " + std::string(e.what()));
  }
}

bool
Runnable::testDestroy() {
  auto ptr = getThreadPtr();
  if (ptr.get()) {
    return ptr->testDestroy();
  } else {
    return true;
  }
}
