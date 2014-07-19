#pragma once

#include <boost/noncopyable.hpp>

#include <memory>

namespace executor {
class IExecutorObserver : protected boost::noncopyable
{
 public:
  typedef std::shared_ptr<IExecutorObserver> pointer_type;
  typedef std::weak_ptr<IExecutorObserver> weak_pointer_type;

  virtual void ResourcesFreed(std::uint64_t node_id,
                              std::uint32_t freed_resources) = 0;

 protected:
  IExecutorObserver() {}
  virtual ~IExecutorObserver() {}
};
} // namespace executor
