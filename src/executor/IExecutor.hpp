#pragma once

#include <executor/IExecutorObserver.hpp>

#include <boost/noncopyable.hpp>

#include <memory>
#include <cstdint>

namespace executor {
class IExecutor : protected boost::noncopyable
{
 public:
  typedef std::shared_ptr<IExecutor> pointer_type;

  virtual void set_id(std::uint64_t node_id) = 0;
  virtual std::uint64_t id() const = 0;

  virtual std::uint32_t available_resources() const = 0;
  virtual std::uint32_t total_resources() const = 0;

  virtual void AddTask(std::uint32_t required_resources,
                       std::uint32_t usage_time) = 0;

  virtual IExecutorObserver::pointer_type observer() const = 0;
  virtual void set_observer(IExecutorObserver::pointer_type) = 0;

 protected:
  IExecutor() {}
  virtual ~IExecutor() {}
};
} // namespace executor
