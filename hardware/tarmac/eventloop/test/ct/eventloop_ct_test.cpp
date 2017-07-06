#include <IDispatcher.h>

#include <gtest/gtest.h>
//#include <stdio.h>
//#include <sys/stat.h>

//#include <iostream>

#include <cutils/log.h>


TEST(EventLoopTest, TestEventFunctionCalled){
    #define  LOG_TAG    "eventloop.TestEventFunctionCalled"
    ALOGI("TestEventFunctionCalled ...");

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

    ALOGI("TestEventFunctionCalled finished");
}

TEST(EventLoopTest, TestFdEventFunctionCalledReadAll){
    #define  LOG_TAG    "eventloop.TestFdEventFunctionCalledReadAll"
    ALOGI("TestFdEventFunctionCalledReadAll ...");

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

    ALOGI("TestFdEventFunctionCalledReadAll finsihed...");
}

TEST(EventLoopTest, TestFdEventFunctionCalledReadPartial){
    #define  LOG_TAG    "eventloop.TestFdEventFunctionCalledReadPartial"
    ALOGI("TestFdEventFunctionCalledReadPartial ...");

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

    ALOGI("TestFdEventFunctionCalledReadPartial finished...");
}