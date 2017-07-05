#ifndef TASK__H
#define TASK__H

#include <functional>
#include <memory>

#include <boost/asio/steady_timer.hpp>

using Closure = std::function<void()>;

// template <class Handle>
class Task : public Closure {
public:
    Task(const Closure& f)
            : Closure(f)
            , on_finished_callback_(nullptr)
    {}

    void OnFinishedDo(Closure cb) {
        on_finished_callback_ = cb;
    }

    // std::size_t Cancel() {
        // return handle_->cancel();
    // }

    bool HasOnFinishedCallback() const {
        return bool(on_finished_callback_);
    }

    Closure OnFinishedCallback() {
        return on_finished_callback_;
    }

private:
    // using Handle = std::shared_ptr<boost::asio::steady_timer>;
    // Handle handle_;

    Closure on_finished_callback_;
};

#endif  // TASK__H
