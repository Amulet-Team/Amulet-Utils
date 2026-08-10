#include <amulet/utils/threading/mutex.hpp>

class TestMutex {
private:
    astd::mutex m;
    int v ASTD_GUARDED_BY(m);
public:
    void test_access();
    void test_lock_unlock();
    void test_lock();
    void test_lock_lock();
    void test_try_lock();
    void test_try_lock_unlock_1();
    void test_try_lock_unlock_2();
    void test_try_lock_unlock_3();
    void test_lock_try_lock();
    void test();
};

void TestMutex::test_access(){
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestMutex::test_lock_unlock(){
    m.lock();
    v += 1;
    m.unlock();
}

void TestMutex::test_lock(){
    m.lock(); // expected-note {{mutex acquired here}}
} // expected-error {{mutex 'm' is still held at the end of function}}

void TestMutex::test_lock_lock(){
    m.lock(); // expected-note 2 {{mutex acquired here}}
    m.lock(); // expected-error {{acquiring mutex 'm' that is already held}}
} // expected-error {{mutex 'm' is still held at the end of function}}

void TestMutex::test_try_lock(){
    auto locked = m.try_lock(); // expected-note {{mutex acquired here}}
    if (locked){
        v += 1;
    } else {
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    }
} // expected-error {{mutex 'm' is not held on every path through here}}

void TestMutex::test_try_lock_unlock_1(){
    auto locked = m.try_lock();
    if (locked){
        v += 1;
        m.unlock();
    } else {
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
        m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
    }
}

void TestMutex::test_try_lock_unlock_2(){
    auto locked = m.try_lock(); // expected-note {{mutex acquired here}}
    if (locked){
        v += 1;
    } else {
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    }
    m.unlock(); // expected-error {{mutex 'm' is not held on every path through here}} expected-error {{releasing mutex 'm' that was not held}}
}

void TestMutex::test_try_lock_unlock_3(){
    auto locked = m.try_lock();
    m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
}

void TestMutex::test_lock_try_lock(){
    m.lock(); // expected-note 2 {{mutex acquired here}}
    auto locked = m.try_lock(); // expected-error {{acquiring mutex 'm' that is already held}}
    if (locked){
        v += 1;
        m.unlock();
    }
} // expected-error {{mutex 'm' is not held on every path through here}}

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
    int v ASTD_GUARDED_BY(m);
public:
    void test_access();
    void test();
    void test_fail();
};

void TestLockGuard::test_access(){
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
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
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}


class TestUniqueLock{
private:
    astd::mutex m;
    int v ASTD_GUARDED_BY(m);

public:
    void test_empty_constructor();
    void test_scoped_constructor();
    void test_scoped_constructor_unlock_1();
    void test_scoped_constructor_unlock_2();
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
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestUniqueLock::test_scoped_constructor(){
    {
        astd::unique_lock lock(m);
        v += 1;
    }
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestUniqueLock::test_scoped_constructor_unlock_1(){
    {
        astd::unique_lock lock(m);
        v += 1;
        lock.unlock();
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    }
}

void TestUniqueLock::test_scoped_constructor_unlock_2(){
    {
        astd::unique_lock lock(m);
        v += 1;
        lock.unlock();
    }
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestUniqueLock::test_lock_constructor_locked(){
    m.lock(); // expected-note {{mutex acquired here}}
    astd::unique_lock lock(m); // expected-error {{acquiring mutex 'm' that is already held}}
    m.unlock();
}

void TestUniqueLock::test_defer_constructor_unlocked(){
    astd::unique_lock lock(m, std::defer_lock);
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    lock.lock();
    v += 1;
}

void TestUniqueLock::test_defer_constructor_locked(){
    m.lock();
    astd::unique_lock lock(m, std::defer_lock); // expected-error {{cannot call function 'unique_lock' while mutex 'm' is held}}
}

void TestUniqueLock::test_adopt_constructor_unlocked(){
    astd::unique_lock lock(m, std::adopt_lock); // expected-error {{calling function 'unique_lock' requires holding mutex 'm' exclusively}}
}

void TestUniqueLock::test_adopt_constructor_locked(){
    m.lock();
    v += 1;
    astd::unique_lock lock(m, std::adopt_lock);
    v += 1;
}

void TestUniqueLock::test_try_lock_constructor(){
    astd::unique_lock lock(m, std::try_to_lock);
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestUniqueLock::test_assign_lock_unlock(){
    astd::unique_lock<astd::mutex> lock1;
    astd::unique_lock lock2(m);
}
