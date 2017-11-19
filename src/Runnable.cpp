#include <Runnable.h>

#include <PlatformThread.h>

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
