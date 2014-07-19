#pragma once

#include <scheduler/IScheduler.hpp>

#include <boost/asio/io_service.hpp>

#include <log4cplus/logger.h>

#include <memory>

namespace scheduler {
namespace impl {
class BasicScheduler : public IScheduler
                     , public std::enable_shared_from_this<BasicScheduler> {
 public:
  BasicScheduler(std::shared_ptr<boost::asio::io_service> io_service);
  ~BasicScheduler();

  // IScheduler interface
  /// @{
  void ScheduleTask(std::uint32_t resources_required,
                    std::uint32_t time_required) override;
  void RegisterNewNode(std::uint32_t resources_available,
                       std::function<void(std::uint64_t)> callback);
  void UnregisterNode(std::uint64_t node_id) override;
  void AddResourcesToNode(std::uint64_t node_id,
                          std::uint32_t additional_resources) override;
  ISchedulerObserver::pointer_type observer() const override;
  void set_observer(ISchedulerObserver::pointer_type observer) override;
  /// @}

 private:
  void DoScheduleTask(std::uint32_t resources_required,
                      std::uint32_t time_required);
  std::shared_ptr<boost::asio::io_service> io_service_;
  ISchedulerObserver::weak_pointer_type observer_;
  log4cplus::Logger logger_;
};
} // namespace impl
} // namespace scheduler
