#include <scheduler/BasicScheduler.hpp>


namespace scheduler {
namespace impl {
BasicScheduler::BasicScheduler(
    std::shared_ptr<boost::asio::io_service> io_service)
  : io_service_(io_service)
  , observer_()
  , logger_(log4cplus::Logger::getInstance("scheduler.impl.BasicScheduler"))
{

}

BasicScheduler::~BasicScheduler()
{
}

void BasicScheduler::ScheduleTask(std::uint32_t resources_required,
                                  std::uint32_t time_required)
{
  auto self = shared_from_this();
  io_service_->post([=] () {self->DoScheduleTask(resources_required,
                                                 time_required);});
}

void BasicScheduler::DoScheduleTask(std::uint32_t resources_required,
                                    std::uint32_t time_required)
{
}

void BasicScheduler::RegisterNewNode(std::uint32_t resources_available,
                                     std::function<void(std::uint64_t)> callback)
{
}

void BasicScheduler::UnregisterNode(std::uint64_t node_id)
{
}

void BasicScheduler::AddResourcesToNode(std::uint64_t node_id,
                                        std::uint32_t additional_resources)
{
}

ISchedulerObserver::pointer_type BasicScheduler::observer() const
{
  return observer_.lock();
}

void BasicScheduler::set_observer(ISchedulerObserver::pointer_type observer)
{
  observer_ = observer;
}
} // namespace impl
} // namespace scheduler
