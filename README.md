# Scheduler Simulator

The Application provides a series of interfaces, an a basic implementation designed to simulate the performance of different schedulers.

The code follows the [Google style guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml) conventions.

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

## Design

The application contains two main interfaces, one for the scheduler and one for the task executor. Each of this interfaces also has an observer which responds to certain events triggered by the implementations of the interfaces. On top of that there is an environment class which serves as a communication channel between the implementations. This environment class implements the observers of both classes and can be easily changed so instead of objects running in the sam process they could be located in different process or in different machines.

### Scheduler

The scheduler interface is compose of two main methods used for scheduling. Two methods for Nodes management and two for observer management. The proper scheduling methods are `ScheduleTask` which adds one task to the scheduler queue. `AddResourcesToNode` is intended to be called by the environment whenever a taks is done, then it notifies the scheduler that certain ammount of resources are free for the given node.

`RegisterNode` and `UnregisterNode` should be self explanatory, as well as `observer` and `set_observer`.

The scheduler notifies its observer that a task was assigned to a given node. Among the parameters, one has to include a callback which is used in case the task cannot be handled by the given node so the scheduler can reschedule that
task.

### Executor

The executor or node is the one that actually execute tasks. It just has one proper method `AddTask`. The rest of the methods are mostly getters to get the current state of the executor. It also has the option to set an Id and its observer.

The executor notifies its observer whenever resources are made available again.
