#include <executor/BasicExecutor.hpp>

#include <boost/format.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

#include <log4cplus/loggingmacros.h>

#include <thread>

namespace executor {
namespace impl {
BasicExecutor::BasicExecutor(
    std::shared_ptr<boost::asio::io_service> io_service,
    std::uint32_t total_resources)
  : io_service_(io_service)
  , ticker_(*io_service, boost::posix_time::seconds(1))
  , stop_ticker_(false)
  , id_(0)
  , total_resources_(total_resources)
  , available_resources_(total_resources_)
  , resource_usage_()
  , resource_usage_mutex_()
  , observer_()
  , logger_(log4cplus::Logger::getInstance("executor.impl.BasicExecutor"))
{
  ticker_.async_wait([this] (boost::system::error_code) {this->Tick();});
}

BasicExecutor::~BasicExecutor()
{
  stop_ticker_ = true;
}

void BasicExecutor::AddTask(std::uint32_t required_resources,
                            std::uint32_t usage_time)
{
  std::lock_guard<decltype(resource_usage_mutex_)> lock(resource_usage_mutex_);
  // NOTE: On a real life application, I would probably keep the iterator
  //       returned by find, so I avoid an extra lookup (which is O(log(n)))
  //       in the case the key is already in the map.
  LOG4CPLUS_TRACE(logger_, "Adding task.");
  if (std::end(resource_usage_) == resource_usage_.find(usage_time)) {
    resource_usage_.insert(std::make_pair(usage_time, required_resources));
  } else {
    resource_usage_[usage_time] += required_resources;
  }
  available_resources_ -= required_resources;
}

void BasicExecutor::Tick()
{
  LOG4CPLUS_TRACE(logger_, "Ticking.");
  {
    std::lock_guard<decltype(resource_usage_mutex_)>
        lock(resource_usage_mutex_);

    // Perhaps should I skip everything here if the resources are empty?

    std::map<std::uint32_t, std::uint32_t> updated_resource_usage;

    for (const auto &current_resource : resource_usage_) {
      if (1 == current_resource.first) {
        // Resources at the front can be freed right away.
        available_resources_ += current_resource.second;

        LOG4CPLUS_TRACE(logger_,
                        (boost::format("Freeing resources %1%")
                         % current_resource.second).str());
        auto observer = observer_.lock();
        if (observer) {
          observer->ResourcesFreed(id_, current_resource.second);
        }
      } else {
        updated_resource_usage.insert(
              std::make_pair(current_resource.first - 1,
                             current_resource.second));
      }
    }

    using std::swap;
    swap(resource_usage_, updated_resource_usage);
  }

  if (!stop_ticker_) {
    ticker_.expires_from_now(boost::posix_time::seconds(1));
    // Prevents going out of scope and therefore calling thick on a non valid
    // object.
    auto self = shared_from_this();
    ticker_.async_wait([this, self] (boost::system::error_code) {
      self->Tick();
    });
  }
}

std::uint32_t BasicExecutor::available_resources() const
{
  std::lock_guard<decltype(resource_usage_mutex_)> lock(resource_usage_mutex_);
  return available_resources_;
}

IExecutorObserver::pointer_type BasicExecutor::observer() const
{
  return observer_.lock();
}

void BasicExecutor::set_observer(IExecutorObserver::pointer_type observer)
{
  observer_ = observer;
}
} // namespace impl
} // namespace executor
