//////
////// Created by iocout on 6/13/20.
//////
////
//#include <iostream>
//#include <sys/types.h>
//#include <unistd.h>
//#include <thread>
//
//#include "../base/thread_pool/ThreadPool.h"
//
//int main(int argc, char *argv[]) {
//    printf("start server\n");
//    ThreadPool pool;
//    pool.start(2, 1000);
//    pool.setThreadInitCallback(std::bind([]() {
//        cout << "initlize before start\n";
//    }));
//    int total = 0;
//    for (int i = 0; i < 5; ++i) {
//        pool.run(std::bind([&pool, i]() {
////            int queue_size = pool.queueSize();
////            cout << "queue size:" << queue_size << endl;
////            std::this_thread::sleep_for(2s);
////            std::thread::id tid = std::this_thread::get_id();
////            cout << "thread id=" << tid << endl;
//            cout << "i:"<< i << " => " << std::this_thread::get_id()<<endl;
//        }));
//        total += 1;
//    }
//
//    cout << "total insert: " << total << endl;
//    cout << "finish!!" << endl;
//    return 0;
//}
