//
// Created by iocout on 6/12/20.
//

#ifndef TINSERVER_NONCOPYABLE_H
#define TINSERVER_NONCOPYABLE_H

class noncopyable {
public:
    noncopyable(const noncopyable &) = delete;

    void operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;

    ~noncopyable() = default;
};


#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#endif //TINSERVER_NONCOPYABLE_H
