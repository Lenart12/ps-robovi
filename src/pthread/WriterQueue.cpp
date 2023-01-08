#include "WriterQueue.h"
#include "../ImageOps.h"
#include <cstdio>

WriterQueue::WriterQueue()
    : map_lock(PTHREAD_MUTEX_INITIALIZER)
{}

WriterQueue::~WriterQueue()
{
    pthread_mutex_destroy(&map_lock);
}

void WriterQueue::write(unique_ptr<Mat> image, string filename, int image_order)
{
    // -1 is out of order
    if (image_order == -1) {
        ImageOps::write_image(filename, *image);
    }

    pthread_mutex_lock(&map_lock);

    if (image_order == current_write) {
        ImageOps::write_image(filename, *image);
        current_write++;
        // printf("Writing image %d  [%s] in order\n", image_order, filename.c_str());
    } else {
        // printf("Queueing image %d [%s]\n", image_order, filename.c_str());
        pending_writes.emplace(image_order, PendingWrite {
            .image = std::move(image),
            .filename = std::move(filename)
        });
    }

    while (!pending_writes.empty()) {
        if (pending_writes.count(current_write) > 0) {
            auto it = pending_writes.find(current_write);

            auto& pending = it->second;

            ImageOps::write_image(pending.filename, *pending.image);
            // printf("Emptying queue %d [%s]\n", current_write, pending.filename.c_str());
            current_write++;

            pending_writes.erase(it);
        } else {
            break;
        }
    }

    pthread_mutex_unlock(&map_lock);
}
