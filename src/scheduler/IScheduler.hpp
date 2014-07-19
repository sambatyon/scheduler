#pragma once

#include <scheduler/ISchedulerObserver.hpp>

#include <boost/noncopyable.hpp>

#include <memory>
#include <cstdint>
#include <functional>

namespace scheduler {
class IScheduler : protected boost::noncopyable {
 public:
  typedef std::shared_ptr<IScheduler> pointer_type;

  /// \brief Message sent by the client in order to schedule a new task in the
  ///        data center.
  virtual void ScheduleTask(std::uint32_t resources_required,
                            std::uint32_t time_required) = 0;

  /// \brief Register the addition of a new node into the data center.
  ///
  /// Adds a new node to be used in the datacenter. Once the node is registered,
  /// the callback function is called and its parameter is the new id of the
  /// node.
  virtual void RegisterNewNode(std::uint32_t resources_available,
                               std::function<void(std::uint64_t)> callback) = 0;
  /// \brief Unregisters the node with the given id so no new tasks will be
  ///        scheduled for it.
  virtual void UnregisterNode(std::uint64_t node_id) = 0;

  /// \brief Message sent by a given node to notify the scheduler that new
  ///        resources were made available.
  ///
  /// Indicates the scheduler that the node with id \c node_id has updated its
  /// available resources. This can happen because an upgrade on the node
  /// or because a task is done.
  virtual void AddResourcesToNode(std::uint64_t node_id,
                                  std::uint32_t additional_resources) = 0;

  virtual ISchedulerObserver::pointer_type observer() const = 0;
  virtual void set_observer(ISchedulerObserver::pointer_type) = 0;

 protected:
  IScheduler() {}
  virtual ~IScheduler() {}
};
} // namespace scheduler
