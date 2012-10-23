#ifndef _OS_WORKQUEUE_HPP_
#define _OS_WORKQUEUE_HPP_

#include <queue>

#include "os_thread.hpp"

namespace os {


class WorkQueue;

class WorkQueueWork {
protected:
    friend class WorkQueue;

public:
    virtual void run(void) = 0;
    virtual ~WorkQueueWork(void) { }
};

extern "C"
void *WorkQueue__entry_thunk(void *data);

class WorkQueue {
    std::queue<WorkQueueWork *> work_queue;

    bool busy;
    bool exit_workqueue;
    os::condition_variable wake_cond;
    os::condition_variable complete_cond;

    os::mutex mutex;

    os::thread thread;

    void wake_up_thread(void);
    void thread_entry(void);
    int run_tasks(void);
    friend void *WorkQueue__entry_thunk(void *data);
public:
    void queue_work(WorkQueueWork *work);
    void flush(void);
    void destroy(void);

    WorkQueue(void);
    ~WorkQueue();
};

}

#endif
