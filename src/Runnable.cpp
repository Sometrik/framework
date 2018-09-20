#include <Runnable.h>
#include <Logger.h>

#include <PlatformThread.h>

#include <exception>

using namespace std;

Logger &
Runnable::getLogger() {
  if (!logger.get()) {
    logger = getThread().createLogger(getName());
  }
  return *logger;
}

std::string
Runnable::getName() const {
  return typeid(*this).name();
}

void
Runnable::start(std::shared_ptr<PlatformThread> _thread) {
  initialize(_thread);
  try {
    run();
  } catch (std::exception & e) {
    getLogger().println("exception: " + std::string(e.what()));
  }
}

