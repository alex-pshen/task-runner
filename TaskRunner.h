#ifndef TASKRUNNER__H
#define TASKRUNNER__H
#include "Task.h"

#include <functional>
#include <set>

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

class TaskRunner {
public:
    using Handler = std::shared_ptr<boost::asio::steady_timer>;
/*
    static TaskRunner& Current() {
        static TaskRunner instance;
        return instance;
    }
*/
    void RunOnce() {
        io_service_.run();
    }

    void RunOne() {
        io_service_.run_one();
    }

    enum class Mode {
        SYNC,
        ASYNC,
        DEFAULT = SYNC,
    };

    void PostTaskSync(Task task) {
        task();
        if (task.HasOnFinishedCallback())
            PostTaskSync(task.OnFinishedCallback());
    }

    void PostTaskAsync(Task task) {
        // TODO: remember thread, task was posted from

        // TODO: don't use timers in primitive cases
        auto timer = std::make_shared<boost::asio::steady_timer>(io_service_);
        timer->expires_from_now(std::chrono::seconds(0));
        timer->async_wait(std::bind(&TaskRunner::Invoke, this, std::placeholders::_1, task));
        handlers_.insert(timer);
    }
/*
    void PostTask(Task task) {
        if (mode_ == Mode::SYNC)
            return PostTaskSync(task);
        if (mode_ == Mode::ASYNC)
            return PostTaskAsync(task);
    }
*/
    void Clear() {
        // for (auto handler : handlers_)
            // handler->cancel();
        handlers_.clear();
    }

    ~TaskRunner() {
        io_service_.stop();
        Clear();
    }

    TaskRunner(Mode mode = Mode::DEFAULT)
            : mode_(mode)
            , io_service_()
            , handlers_()
    {}
private:

    TaskRunner(const TaskRunner&) = delete;
    TaskRunner& operator = (TaskRunner&) = delete;

    void Invoke(const boost::system::error_code& error, Task task) {
        if (error != boost::asio::error::operation_aborted) {
            task();
            if (task.HasOnFinishedCallback())
                PostTaskAsync(task.OnFinishedCallback());
        }
    }

    Mode mode_;
    boost::asio::io_service io_service_;
    std::set<Handler> handlers_;
};
#endif  // TASKRUNNER__H
