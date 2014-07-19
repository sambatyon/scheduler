#include <Environment.hpp>

#include <executor/BasicExecutor.hpp>
#include <scheduler/BasicScheduler.hpp>

#include <cassert>

namespace env {
Environment::Environment(std::shared_ptr<boost::asio::io_service> io_service)
  : io_service_(io_service)
  , scheduler_(std::make_shared<scheduler::impl::BasicScheduler>(io_service))
  , executors_()
{
  assert(io_service);
}

void Environment::Initialize(
    const std::vector<std::uint32_t> &executor_resources)
{
  scheduler_->set_observer(shared_from_this());
  for (const auto &resources : executor_resources) {
    executor::IExecutor::pointer_type task_executor =
        std::make_shared<executor::impl::BasicExecutor>(io_service_, resources);
    task_executor->set_observer(shared_from_this());
    auto self = shared_from_this();
    scheduler_->RegisterNewNode(resources,
                                [task_executor, self] (std::uint64_t node_id) {
      std::lock_guard<decltype(self->executors_mutex_)>
          lock(self->executors_mutex_);
      task_executor->set_id(node_id);
      self->executors_.insert(std::make_pair(node_id, task_executor));
    });
  }
}

Environment::~Environment()
{
}

void Environment::AddTask(std::uint32_t required_resources,
                                        std::uint32_t required_uptime)
{
  scheduler_->ScheduleTask(required_resources, required_uptime);
}

void Environment::ResourcesFreed(std::uint64_t node_id,
                                 std::uint32_t freed_resources)
{
  scheduler_->AddResourcesToNode(node_id, freed_resources);
}

void Environment::AssignTask(std::uint64_t node_id,
                             std::uint32_t resources_required,
                             std::uint32_t time_required,
                             std::function<void(boost::system::error_code,
                                                std::uint64_t,
                                                std::uint32_t,
                                                std::uint32_t)> callback)
{
  namespace sys = boost::system;

  std::lock_guard<decltype(executors_mutex_)> lock(executors_mutex_);
  auto executor_iter = executors_.find(node_id);
  if (std::end(executors_) == executor_iter) {
    // The given executor doesn't exists
    sys::error_code error(sys::errc::host_unreachable, sys::generic_category());

    // Notify the scheduler that the node is unreachable, send back data so he
    // can reschdeule the task.
    io_service_->post([=] () {
      callback(error, node_id, resources_required, time_required);
    });
  } else if (executor_iter->second->available_resources() < resources_required) {
    // The executor doesn't have the available resources to process the request.
    sys::error_code error(sys::errc::device_or_resource_busy,
                          sys::generic_category());
    io_service_->post([=] () {
      callback(error, node_id, resources_required, time_required);
    });
  } else {
    executor_iter->second->AddTask(resources_required, time_required);
    sys::error_code success(sys::errc::device_or_resource_busy,
                            sys::generic_category());
    io_service_->post([=] () {
      callback(success, node_id, resources_required, time_required);
    });
  }
}
} // namespace env
