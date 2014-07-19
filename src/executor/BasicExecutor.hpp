#pragma once

#include <executor/IExecutor.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <log4cplus/logger.h>

#include <map>
#include <mutex>
#include <memory>

namespace executor {
namespace impl {
class BasicExecutor : public IExecutor
                    , public std::enable_shared_from_this<BasicExecutor> {
 public:
  BasicExecutor(std::shared_ptr<boost::asio::io_service> io_service,
                std::uint32_t total_resources);
  ~BasicExecutor();

  void set_id(std::uint64_t node_id) override;
  std::uint64_t id() const override;

  std::uint32_t available_resources() const override;
  std::uint32_t total_resources() const override;

  void AddTask(std::uint32_t required_resources,
               std::uint32_t usage_time) override;

  IExecutorObserver::pointer_type observer() const override;
  void set_observer(IExecutorObserver::pointer_type) override;

 private:
  void Tick();

  std::shared_ptr<boost::asio::io_service> io_service_;
  boost::asio::deadline_timer ticker_;
  bool stop_ticker_;

  std::uint64_t id_;
  std::uint32_t total_resources_;
  std::uint32_t available_resources_;

  /// A map containing the number of resources being used, sorted by the
  /// the time in which they are going to be freed. It is updated after each
  /// tick. If at some point it reaches zero, we notify that resources have
  /// become available again.
  std::map<std::uint32_t, std::uint32_t> resource_usage_;
  mutable std::mutex resource_usage_mutex_;
  IExecutorObserver::weak_pointer_type observer_;
  log4cplus::Logger logger_;
};

inline
void BasicExecutor::set_id(std::uint64_t node_id)
{
  id_ = node_id;
}

inline
std::uint64_t BasicExecutor::id() const
{
  return id_;
}

inline
std::uint32_t BasicExecutor::total_resources() const
{
  return total_resources_;
}
} // namespace impl
} // namespace executor
