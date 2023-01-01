#include <vector>
#include <string>
#include <array>
#include <iostream>

#include <pthread.h>


using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;

extern "C" void* test_thread(void* _arg) {
    auto id = reinterpret_cast<size_t>(_arg);
    cout << "Pthread " << id << endl;
    
    return nullptr;
}

void pthread_main(vector<string> args) {
    array<pthread_t, 5> threads;

    for (size_t i = 0; i < threads.size(); i++) {
        pthread_create(&threads[i], nullptr, test_thread, reinterpret_cast<void*>(i));
    }

    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }
}


