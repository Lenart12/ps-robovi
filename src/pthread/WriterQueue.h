#ifndef SRC_PTHREAD_WRITER_QUEUE_H
#define SRC_PTHREAD_WRITER_QUEUE_H

#include <string>
#include <memory>
#include <queue>
#include <map>

using std::string;
using std::unique_ptr;
using std::map;

#include <pthread.h>

#include <opencv2/opencv.hpp>
using cv::Mat;

#include "../ScopeTimer.h"
#include "../Canny.h"

class WriterQueue {
public:
    WriterQueue();
    ~WriterQueue();

    void write(unique_ptr<Mat> image, string filename, int image_order);

private:
    struct PendingWrite {
        unique_ptr<Mat> image;
        string filename;
    };


    map<int, PendingWrite> pending_writes;
    
    int current_write = 0;

    pthread_mutex_t map_lock;
};

#endif /* SRC_PTHREAD_WRITER_QUEUE_H */
