#pragma once

#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

#include <memory>
#include <cstdint>

namespace scheduler {
class ISchedulerObserver : protected boost::noncopyable {
 public:
  typedef std::shared_ptr<ISchedulerObserver> pointer_type;
  typedef std::weak_ptr<ISchedulerObserver> weak_pointer_type;

  /// \brif Schedules a task in the given Node.
  ///
  /// The callback is used in case there was an error (the node dissappear, for
  /// example), so the scheduler can reschedule.
  virtual void AssignTask(
      std::uint64_t node_id,
      std::uint32_t resources_required,
      std::uint32_t time_required,
      std::function<void(boost::system::error_code,
                         std::uint64_t, // Node Id
                         std::uint32_t, // Resources Required
                         std::uint32_t  // Time Required
                         )> callback) = 0;

 protected:
  ISchedulerObserver() {}
  virtual ~ISchedulerObserver() {}
};
} // namespace scheduler
