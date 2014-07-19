#pragma once

#include <executor/IExecutor.hpp>
#include <executor/IExecutorObserver.hpp>

#include <scheduler/IScheduler.hpp>
#include <scheduler/ISchedulerObserver.hpp>

#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

#include <mutex>
#include <memory>
#include <cstdint>
#include <unordered_map>

namespace env {
class Environment : public executor::IExecutorObserver
                  , public scheduler::ISchedulerObserver
                  , public std::enable_shared_from_this<Environment>
                  , protected boost::noncopyable {
 public:
  typedef std::shared_ptr<Environment> pointer_type;

  // TODO: Construct passing factories as parameters.
  Environment(std::shared_ptr<boost::asio::io_service> io_service);
  ~Environment();

  // To avoid using this in the constructor (which almost always ends up in
  // cathastorphe. Use this method after construction.
  void Initialize(const std::vector<std::uint32_t> &executor_resources);
  void AddTask(std::uint32_t required_resources, std::uint32_t required_uptime);

 protected:
  // IExecutorObserver interface
  /// @{
  void ResourcesFreed(std::uint64_t node_id, std::uint32_t freed_resources);
  /// @}

  // ISchedulerObserver interfac
  /// @{
  void AssignTask(std::uint64_t node_id, std::uint32_t resources_required,
                  std::uint32_t time_required,
                  std::function<void(boost::system::error_code,
                                     std::uint64_t, // Node Id
                                     std::uint32_t, // Resources Required
                                     std::uint32_t  // Time Required
                                     )> callback);
  /// @}

 private:
  std::shared_ptr<boost::asio::io_service> io_service_;
  scheduler::IScheduler::pointer_type scheduler_;
  std::unordered_map<std::uint64_t, executor::IExecutor::pointer_type>
    executors_;
  std::mutex executors_mutex_;
};
} // namespace env
