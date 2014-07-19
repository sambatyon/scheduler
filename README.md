# Scheduler Simulator

The Application provides a series of interfaces, an a basic implementation designed to simulate the performance of different schedulers.

## Requirements

* A modern C++ compiler. Not sure if it will compile with msvc 2013, but I am sure it won't do it in msvc 2012 (Because on use of initializer lists).
* CMake, at least 2.8.
* Log4cplus, in ubuntu just do `sudo apt-get install liblog4cplus-dev`.
* Boost 1.55, in unbuntu just do `sudo apt-get install libboost1.55-all-dev`. As a sidenote, I do remember last time I used that command, it didn't install all the required headers, so I had to fix it manually.
* You may need the log4cplus configuration file in the same directory as the executable if you use debug (when running cmake add as a parameter `-DCMAKE_BUILD_TYPE=Debug`), otherwise the program will look for it in `/var/log/actor/logging.conf`. An example of the configuration file would be:

```
# Console Appender
log4cplus.appender.C=log4cplus::ConsoleAppender

# Layout
log4cplus.appender.C.layout=log4cplus::PatternLayout
log4cplus.appender.C.layout.ConversionPattern=%D{%d.%m.%Y %H:%M:%S.%q} [%8t] %-5p - %c{2} - %m%n

log4cplus.rootLogger=TRACE, C
```
