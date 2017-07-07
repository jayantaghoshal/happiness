#include <IDispatcher.h>

#include <gtest/gtest.h>
#include <cutils/log.h>

#include <future>


TEST(EventLoopTest, TestEventFunctionCalled){
    #define  LOG_TAG    "eventloop.TestEventFunctionCalled"
    ALOGI("Starting...");

    bool event_func_called = false;

    tarmac::eventloop::IDispatcher::EnqueueTask(
        [&event_func_called] () {
            ALOGI("Task executed!");
            event_func_called = true;
        });

    ALOGI("Wait for task to be executed");
    uint16_t max_iterations = 50;
    uint16_t num_iterations = 0;
    while (!event_func_called)
    {
        usleep(100000);

        if (num_iterations >= max_iterations){
            ALOGI("Task not executed within 5 sec, breaking loop");
            break;
        }

        ++num_iterations;
    }
    ALOGI("Done waiting for task to be executed");

    //Check that task was executed, if not event_func_called will be false
    EXPECT_TRUE(event_func_called);

    ALOGI("Finished...");
}

TEST(EventLoopTest, TestFdEventFunctionCalledReadAll){
    #define  LOG_TAG    "eventloop.TestFdEventFunctionCalledReadAll"
    ALOGI("Starting...");

    int fd[2];

    char string[] = "Hello, world!\n";
    char readbuffer[80];
    uint16_t nbytes = 0;
    uint16_t task_exec_count = 0;

    EXPECT_NE(pipe(fd), -1);

    tarmac::eventloop::IDispatcher::GetDefaultDispatcher().AddFd(fd[0],
        [&readbuffer, &nbytes, &task_exec_count, fd] () {
            ALOGI("Task executed!");
            nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
            ++task_exec_count;
        });

    //Write to file descriptor to trigger task
    write(fd[1], string, (strlen(string)+1));

    ALOGI("Wait for task to be executed");
    uint16_t max_iterations = 50;
    uint16_t num_iterations = 0;
    while (nbytes != strlen(string)+1)
    {
        usleep(100000);

        if (num_iterations >= max_iterations){
            ALOGI("Task not executed within 5 sec, breaking loop");
            break;
        }

        ++num_iterations;
    }
    ALOGI("Done waiting for task to be executed");

    EXPECT_EQ(nbytes, strlen(string)+1);
    EXPECT_EQ(strcmp(string, readbuffer), 0);
    EXPECT_EQ(task_exec_count, 1);

    //Wait an additional 500ms to check that task isn't executed twice by mistake
    usleep(500000);
    EXPECT_EQ(task_exec_count, 1);

    close(fd[0]);
    close(fd[1]);

    ALOGI("Finished...");
}

TEST(EventLoopTest, TestFdEventFunctionCalledReadPartial){
    #define  LOG_TAG    "eventloop.TestFdEventFunctionCalledReadPartial"
    ALOGI("Starting...");

    int fd[2];

    char string[] = "Hello, world!\n";
    char readbuffer[80] = "";
    uint16_t nbytes = 0;

    EXPECT_NE(pipe(fd), -1);

    tarmac::eventloop::IDispatcher::GetDefaultDispatcher().AddFd(fd[0],
        [&readbuffer, &nbytes, fd] () {
            char subreadbuffer[6];

            uint16_t readbytes = read(fd[0], subreadbuffer, 5);
            subreadbuffer[5] = 0;
            strcat(readbuffer, subreadbuffer);

            ALOGI("Task: \"%s\" ", readbuffer);
            nbytes += readbytes;
        });

    //Write to file descriptor to trigger task
    write(fd[1], string, (strlen(string)+1));

    ALOGI("Wait for task to be executed");
    uint16_t max_iterations = 50;
    uint16_t num_iterations = 0;
    while (nbytes != strlen(string)+1)
    {
        usleep(100000);

        if (num_iterations >= max_iterations){
            ALOGI("Task not executed within 5 sec, breaking loop");
            break;
        }

        ++num_iterations;
    }
    ALOGI("Done waiting for task to be executed");

    EXPECT_EQ(nbytes, strlen(string)+1);
    EXPECT_EQ(strcmp(string, readbuffer), 0);

    close(fd[0]);
    close(fd[1]);

    ALOGI("Finished...");
}

TEST(EventLoopTest, TestDelayedEventFunctionCalled){
    #define  LOG_TAG    "eventloop.TestDelayedEventFunctionCalled"
    ALOGI("Starting...");

    std::promise<int> p;
    std::future<int> f = p.get_future();

    auto start = std::chrono::steady_clock::now();
    auto end = start;

    tarmac::eventloop::IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(
        std::chrono::microseconds(500000),
        [&end, &p] () {
            ALOGI("Task executed!");
            end = std::chrono::steady_clock::now();
            p.set_value(1);
        });

    //Wait for 100ms and check that task wasn't dispatched immediatelly
    usleep(100000);
    EXPECT_TRUE(start == end);

    ALOGI("Wait for task to be executed");
    std::future_status status = f.wait_for(std::chrono::seconds(5));
    ALOGI("Done waiting for task to be executed");

    //Check that task was executed, if not event_func_called will be false
    EXPECT_TRUE(status == std::future_status::ready);

    if (status == std::future_status::ready) {
        std::chrono::duration<double> diff = end-start;
        EXPECT_NEAR(diff.count(), 0.5f, 0.01f);
        ALOGI("Expected delay time is 0.5s, measured delay time is %f, allowed margin of error is 0.01s", diff.count());
    }

    ALOGI("Finished...");
}

TEST(EventLoopTest, TestDelayedEventFunctionCancelled){
    #define  LOG_TAG    "eventloop.TestDelayedEventFunctionCancelled"
    ALOGI("Starting...");

    std::promise<int> p;
    std::future<int> f = p.get_future();

    tarmac::eventloop::IDispatcher::JobId jobid = tarmac::eventloop::IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(
        std::chrono::microseconds(500000),
        [&p] () {
            ALOGI("Task executed!");
            p.set_value(1);
        });

    //Wait for 100ms and check that task wasn't dispatched
    std::future_status status = f.wait_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(status == std::future_status::timeout);

    //Cancel event, expect it to be cancelled correctly
    EXPECT_TRUE(tarmac::eventloop::IDispatcher::GetDefaultDispatcher().Cancel(jobid));

    //Cancel event again, expect Cancel() to return false since task is already cancelled
    EXPECT_FALSE(tarmac::eventloop::IDispatcher::GetDefaultDispatcher().Cancel(jobid));

    ALOGI("Wait for task to be executed");
    status = f.wait_for(std::chrono::seconds(2));
    ALOGI("Done waiting for task to be executed");

    //Check that task was executed, if not event_func_called will be false
    EXPECT_TRUE(status == std::future_status::timeout);

    ALOGI("Finished...");
}