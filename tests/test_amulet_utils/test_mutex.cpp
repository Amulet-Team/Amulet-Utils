#include <amulet/utils/threading/mutex.hpp>

class TestMutex {
private:
    astd::mutex m;
    int v __attribute__((guarded_by(m)));
public:
    void test_access();
    void test_lock_unlock();
    void test_lock();
    void test_try_lock();
    void test_try_lock_unlock();
    void test_lock_lock();
    void test_lock_try_lock();
    void test();
};

void TestMutex::test_access(){
    v += 1;  // writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
}

void TestMutex::test_lock_unlock(){
    m.lock();
    v += 1;
    m.unlock();
}

void TestMutex::test_lock(){
    m.lock(); //mutex acquired here
} //            mutex 'm' is still held at the end of function [-Wthread-safety-analysis]

void TestMutex::test_try_lock(){
    auto locked = m.try_lock(); // mutex acquired here
    if (locked){
        v += 1;
    } else {
        v += 1;//writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
    }
} //            mutex 'm' is still held at the end of function [-Wthread-safety-analysis]

void TestMutex::test_try_lock_unlock(){
    auto locked = m.try_lock();
    if (locked){
        v += 1;
        m.unlock();
    } else {
        v += 1;//writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
        m.unlock();//releasing mutex 'm' that was not held [-Wthread-safety-analysis]
    }
}

void TestMutex::test_lock_lock(){
    m.lock(); //mutex acquired here
    m.lock(); //acquiring mutex 'm' that is already held [-Wthread-safety-analysis]
} //            mutex 'm' is still held at the end of function [-Wthread-safety-analysis]

void TestMutex::test_lock_try_lock(){
    m.lock(); //mutex acquired here
    auto locked = m.try_lock(); // acquiring mutex 'm' that is already held [-Wthread-safety-analysis]
    if (locked){
        v += 1;
    } else {
        v += 1;//writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
    }
} //            mutex 'm' is still held at the end of function [-Wthread-safety-analysis]

void TestMutex::test() {
    m.lock();
    v += 1;
    m.unlock();

    auto locked = m.try_lock();
    if (locked){
        v += 1;
        m.unlock();
    }
}


class TestLockGuard {
private:
    astd::mutex m;
    int v __attribute__((guarded_by(m)));
public:
    void test_access();
    void test();
    void test_fail();
};

void TestLockGuard::test_access(){
    v += 1;  //  writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
}

void TestLockGuard::test(){
    astd::lock_guard lock(m);
    v += 1;
}

void TestLockGuard::test_fail(){
    {
        astd::lock_guard lock(m);
        v += 1;
    }
    {
        astd::lock_guard lock(m);
        v += 1;
    }
    v += 1;  //  writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
}


class TestUniqueLock{
private:
    astd::mutex m;
    int v __attribute__((guarded_by(m)));

public:
    void test_empty_constructor();
    void test_lock_constructor_unlocked();
    void test_lock_constructor_locked();
    void test_defer_constructor_unlocked();
    void test_defer_constructor_locked();
    void test_adopt_constructor_unlocked();
    void test_adopt_constructor_locked();
    void test_try_lock_constructor();
    void test_assign_lock_unlock();
};

void TestUniqueLock::test_empty_constructor(){
    astd::unique_lock<astd::mutex> lock;
    v += 1;  //  writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
}

void TestUniqueLock::test_lock_constructor_unlocked(){
    {
        astd::unique_lock lock(m);
        v += 1;
    }
    v += 1;  //  writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
    {
        astd::unique_lock lock(m);
        v += 1;
        m.unlock();
        v += 1;  //  writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
    }
    v += 1;  //  writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
}

void TestUniqueLock::test_lock_constructor_locked(){
    m.lock();  // mutex acquired here
    astd::unique_lock lock(m);  // acquiring mutex 'm' that is already held [-Wthread-safety-analysis]
}

void TestUniqueLock::test_defer_constructor_unlocked(){
    astd::unique_lock lock(m, std::defer_lock);
    v += 1;  //  writing variable 'v' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
    lock.lock();
    v += 1;
}

void TestUniqueLock::test_defer_constructor_locked(){
    m.lock();
    astd::unique_lock lock(m, std::defer_lock);  // acquiring mutex 'm' that is already held [-Wthread-safety-analysis]
}

void TestUniqueLock::test_adopt_constructor_unlocked(){
    astd::unique_lock lock(m, std::adopt_lock);  // calling function 'unique_lock' requires holding mutex 'm' exclusively [-Wthread-safety-analysis]
}

void TestUniqueLock::test_adopt_constructor_locked(){
    m.lock();
    v += 1;
    astd::unique_lock lock(m, std::adopt_lock);
    v += 1;
}

void TestUniqueLock::test_try_lock_constructor(){
    astd::unique_lock lock(m, std::try_to_lock);
    v += 1;
}

void TestUniqueLock::test_assign_lock_unlock(){
    astd::unique_lock<astd::mutex> lock1;
    astd::unique_lock lock2(m);
}
