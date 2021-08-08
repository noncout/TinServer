//
// Created by HF on 2020/8/27.
//

#ifndef TINSERVER_SINGLETON_H
#define TINSERVER_SINGLETON_H
template<typename T>
class Singleton {
public:
    static T * Instance();
    Singleton(const T& value) = delete ;

    const Singleton operator=(const Singleton&) = delete ;
protected:
    Singleton() {};

    ~Singleton() {};

private:
    static T* value_;
};

template<typename T>
T * Singleton<T>::value_ = new T();

template<typename T>
T * Singleton<T>::Instance() {
    return value_;
}

#endif //TINSERVER_SINGLETON_H
