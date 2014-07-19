#include <Environment.hpp>

#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <log4cplus/configurator.h>

#include <memory>
#include <iostream>

void ConfigureLogger();

int main(int /*argc*/, char ** /*argv*/) {
  ConfigureLogger();

  std::shared_ptr<boost::asio::io_service> io_service =
      std::make_shared<boost::asio::io_service>();
  std::shared_ptr<boost::asio::io_service::work> worker =
      std::make_shared<boost::asio::io_service::work>(*io_service);
  boost::thread_group threads;
  for (auto i = 0; i < 5; ++i) {
    threads.create_thread([=] () {io_service->run();});
  }

  env::Environment::pointer_type environment =
      std::make_shared<env::Environment>(io_service);
  environment->Initialize({45, 10, 20, 32, 15, 25});

  environment->AddTask(12, 8);
  environment->AddTask(20, 5);
  environment->AddTask(4, 2);

  worker.reset();

  threads.join_all();

  return 0;
}

void ConfigureLogger() {
#if defined(_WIN32)
  TCHAR path_buffer[MAX_PATH];
  DWORD buffer_size = GetModuleFileName(NULL, path_buffer, MAX_PATH);
  // if buffer_size == MAX_PATH the buffer was truncated and GetLastError() returns ERROR_INSUFFICIENT_BUFFER
  // TODO: if buffer_size == 0 check GetLastError()
  boost::filesystem::path log_path(path_buffer);
#elif defined(__APPLE__)
  char path_buffer[MAXPATHLEN];
  std::uint32_t buffer_size = sizeof(path_buffer);
  int got_path = _NSGetExecutablePath(path_buffer, &buffer_size);
  // TODO: check if got_path != 0, in which case the call failed.
  boost::filesystem::path log_path(path_buffer);
#else // Assuming linux
# if !defined(NDEBUG)
  // On debug the configuration file is in the same directory as the executable
  char path_buffer[8192];
  std::uint32_t buffer_size = sizeof(path_buffer);
  ssize_t path_size = readlink("/proc/self/exe", path_buffer, buffer_size);
  // TODO: if path_size == -1 check errno
  path_buffer[path_size] = '\0';
  boost::filesystem::path log_path(path_buffer);
# else
  // On release mode, it should be in a common directory
  boost::filesystem::path log_path("/var/log/actor/logging.conf");
#   endif
#endif
  log_path.remove_filename();
  log_path /= "logging.conf";

  // only in version 1.1.1 and beyond
  // log4cplus::initialize();
  if (boost::filesystem::exists(log_path) &&
        boost::filesystem::is_regular_file(log_path)) {
    log4cplus::PropertyConfigurator configurator(log_path.string());
    configurator.configure();
  } else {
    log4cplus::BasicConfigurator configurator;
    configurator.configure();

    // TODO: make this code work
    /*log4cplus::SharedAppenderPtr appender(
      new log4cplus::ConsoleAppender(false, false));
    appender->setName("RootAppender");
    std::auto_ptr<log4cplus::Layout> layout(
      new log4cplus::PatternLayout("%D{%d.%m.%Y %H:%M:%S.%q} [%8t] %-5p - "
                                   "%c{2} - %m%n"));
    appender->setLayout(layout);

    auto logger = log4cplus::Logger::getRoot();
    logger.addAppender(appender);
    logger.setLogLevel(log4cplus::TRACE_LOG_LEVEL);*/
  }
}
