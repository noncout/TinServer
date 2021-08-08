////
//// Created by HF on 2020/8/28.
////
//
//#ifndef TINSERVER_TEST_SINGLETON_CPP
//#define TINSERVER_TEST_SINGLETON_CPP
//
//#include <iostream>
//#include <chrono>
//#include <thread>
//#include <mutex>
//
//#include "../base/singleton/singleton.h"
//
//using std::cout;
//using std::endl;
//using std::thread;
//
//class Test {
//public:
//
//    Test() {
//        cout << "init\n";
//    }
//
//    void setValue(int t) {
//        this->value_ = t;
//    }
//
//    int getValue() {
//        return value_;
//    }
//
//private:
//    int value_;
//};
//
//int main() {
//    auto one = singleton<Test>::instance();
//    cout << "main thread id is :" << std::this_thread::get_id() << endl;
//    cout << "one is:" << one->getValue() << endl;
//    thread t2([&]() {
//        auto one = singleton<Test>::instance();
//        cout << "before one is:" << one->getValue() << endl;
//        one->setValue(5);
//        cout << "after one is:" << one->getValue() << endl;
//    });
//
//    thread t1([&]() {
//        auto one = singleton<Test>::instance();
//        cout << "one is:" << one->getValue() << endl;
//    });
//
//    thread t3([&]() {
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        auto one = singleton<Test>::instance();
//        cout << "one is:" << one->getValue() << endl;
//    });
//
//    t1.join();
//    t2.join();
//    t3.join();
//
//    return 0;
//}
//
//#endif //TINSERVER_TEST_SINGLETON_CPP
