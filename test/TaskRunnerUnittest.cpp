// #include <atomic>
#include <gtest/gtest.h>
#include "TaskRunner.h"

class TaskRunnerTest : public ::testing::Test {
public:
    TaskRunnerTest()
            : sync_task_runner_(TaskRunner::Mode::SYNC)
            , async_task_runner_(TaskRunner::Mode::ASYNC)
            , task_was_ran_(false)
            , task_([was_ran = &task_was_ran_](){ *was_ran = true; })
            , on_finished_was_ran_(false)
            , on_finished_task_([was_ran = &on_finished_was_ran_](){ *was_ran = true; })
    {}

    ~TaskRunnerTest() {
        // task_runner_.Clear();
    }

protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

    TaskRunner sync_task_runner_;
    TaskRunner async_task_runner_;

    bool task_was_ran_;
    Task task_;

    bool on_finished_was_ran_;
    Task on_finished_task_;
};

TEST_F(TaskRunnerTest, PostTaskSync) {
    sync_task_runner_.PostTaskSync(task_);
    EXPECT_TRUE(task_was_ran_);
    EXPECT_FALSE(on_finished_was_ran_);
}

TEST_F(TaskRunnerTest, PostTaskSyncWithReply) {
    task_.OnFinishedDo(on_finished_task_);
    sync_task_runner_.PostTaskSync(task_);

    EXPECT_TRUE(task_was_ran_);
    EXPECT_TRUE(on_finished_was_ran_);
}

TEST_F(TaskRunnerTest, PostTaskAsync) {
    async_task_runner_.PostTaskAsync(task_);
    EXPECT_FALSE(task_was_ran_);
    EXPECT_FALSE(on_finished_was_ran_);

    async_task_runner_.RunOnce();
    EXPECT_TRUE(task_was_ran_);
    EXPECT_FALSE(on_finished_was_ran_);
}

TEST_F(TaskRunnerTest, PostTaskAsyncWithReply) {
    task_.OnFinishedDo(on_finished_task_);
    async_task_runner_.PostTaskAsync(task_);
    EXPECT_FALSE(task_was_ran_);
    EXPECT_FALSE(on_finished_was_ran_);

    async_task_runner_.RunOne();
    EXPECT_TRUE(task_was_ran_);
    EXPECT_FALSE(on_finished_was_ran_);

    async_task_runner_.RunOne();
    EXPECT_TRUE(task_was_ran_);
    EXPECT_TRUE(on_finished_was_ran_);
}

int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
