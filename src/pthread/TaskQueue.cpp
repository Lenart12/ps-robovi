#include "TaskQueue.h"
#include <cassert>

void CannyTaskQueue::add_task(CannyTask &&task)
{
    pthread_mutex_lock(&queue_lock);
    tasks.emplace(std::forward<CannyTask>(task));
    processing_tasks++;
    pthread_mutex_unlock(&queue_lock);
}

optional<CannyTask> CannyTaskQueue::get_task()
{
    pthread_mutex_lock(&queue_lock);
    if (tasks.empty()) {
        pthread_mutex_unlock(&queue_lock);
        return std::nullopt;
    }

    auto task = std::move(tasks.front());
    tasks.pop();
    pthread_mutex_unlock(&queue_lock);
    return task;
}

void CannyTaskQueue::mark_task_finished()
{
    pthread_mutex_lock(&queue_lock);
    processing_tasks--;
    assert(processing_tasks >= 0);
    pthread_mutex_unlock(&queue_lock);
}

void CannyTaskQueue::finish_adding_tasks()
{
    mark_task_finished();
}

bool CannyTaskQueue::all_complete()
{
    pthread_mutex_lock(&queue_lock);
    bool all_done = processing_tasks == 0;
    pthread_mutex_unlock(&queue_lock);
    return all_done;
}

CannyTask CannyTask::create_from(Task &&new_task)
{
    return CannyTask { properties, std::move(new_task) };
}
