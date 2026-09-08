#include <amulet/utils/threading/mutex.hpp>
#include <amulet/utils/threading/shared_mutex.hpp>

class TestMutex {
private:
    astd::mutex m;
    int v ASTD_GUARDED_BY(m);
public:
    void test_access();
    void test_lock_unlock();
    void test_lock();
    void test_unlock();
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

void TestMutex::test_unlock(){
    m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
}

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
    lock1 = std::move(lock2);
}

class TestSharedMutex {
private:
    astd::shared_mutex m;
    int v ASTD_GUARDED_BY(m);
public:
    void test_access();
    void test_lock_unlock();
    void test_lock_shared_unlock_shared();
    void test_lock_shared_unlock_shared_write();
    void test_lock_unlock_shared();
    void test_lock_shared_unlock();
    void test_lock();
    void test_unlock();
    void test_lock_shared();
    void test_unlock_shared();
    void test_lock_lock();
    void test_lock_shared_lock_shared();
    void test_try_lock();
    void test_try_lock_unlock_1();
    void test_try_lock_unlock_2();
    void test_try_lock_unlock_3();
    void test_try_lock_shared();
    void test_try_lock_shared_unlock_shared_1();
    void test_try_lock_shared_unlock_shared_2();
    void test_try_lock_shared_unlock_shared_3();
    void test_lock_try_lock();
    void test_lock_shared_try_lock_shared();
    void test();
};

void TestSharedMutex::test_access(){
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestSharedMutex::test_lock_unlock(){
    m.lock();
    v += 1;
    m.unlock();
}

void TestSharedMutex::test_lock_shared_unlock_shared(){
    m.lock_shared();
    auto a = v;
    m.unlock_shared();
}

void TestSharedMutex::test_lock_shared_unlock_shared_write(){
    m.lock_shared();
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    m.unlock_shared();
}

void TestSharedMutex::test_lock_unlock_shared(){
    m.lock(); // expected-note {{mutex acquired here}}
    v += 1;
    m.unlock_shared(); // expected-error {{releasing mutex 'm' using shared access, expected exclusive access}} expected-error {{releasing shared-mode 'm.shared_capability' that was not held}}
}

void TestSharedMutex::test_lock_shared_unlock(){
    m.lock_shared(); // expected-note {{mutex acquired here}} expected-note {{shared-mode acquired here}}
    auto a = v;
    m.unlock(); // expected-error {{releasing mutex 'm' using exclusive access, expected shared access}}
} // expected-error {{shared-mode 'm.shared_capability' is still held at the end of function}}

void TestSharedMutex::test_lock(){
    m.lock(); // expected-note {{mutex acquired here}}
} // expected-error {{mutex 'm' is still held at the end of function}}

void TestSharedMutex::test_unlock(){
    m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
}

void TestSharedMutex::test_lock_shared(){
    m.lock_shared(); // expected-note {{mutex acquired here}} expected-note {{shared-mode acquired here}}
} // expected-error {{mutex 'm' is still held at the end of function}} expected-error {{shared-mode 'm.shared_capability' is still held at the end of function}}

void TestSharedMutex::test_unlock_shared(){
    m.unlock_shared(); // expected-error {{releasing mutex 'm' that was not held}} expected-error {{releasing shared-mode 'm.shared_capability' that was not held}}
}

void TestSharedMutex::test_lock_lock(){
    m.lock(); // expected-note 2 {{mutex acquired here}}
    m.lock(); // expected-error {{acquiring mutex 'm' that is already held}}
} // expected-error {{mutex 'm' is still held at the end of function}}

void TestSharedMutex::test_lock_shared_lock_shared(){
    m.lock_shared(); // expected-note 2 {{mutex acquired here}} expected-note 2 {{shared-mode acquired here}}
    m.lock_shared(); // expected-error {{acquiring mutex 'm' that is already held}} expected-error {{acquiring shared-mode 'm.shared_capability' that is already held}}
} // expected-error {{mutex 'm' is still held at the end of function}} expected-error {{shared-mode 'm.shared_capability' is still held at the end of function}}

void TestSharedMutex::test_try_lock(){
    auto locked = m.try_lock(); // expected-note {{mutex acquired here}}
    if (locked){
        v += 1;
    } else {
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    }
} // expected-error {{mutex 'm' is not held on every path through here}}

void TestSharedMutex::test_try_lock_unlock_1(){
    auto locked = m.try_lock();
    if (locked){
        v += 1;
        m.unlock();
    } else {
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
        m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
    }
}

void TestSharedMutex::test_try_lock_unlock_2(){
    auto locked = m.try_lock(); // expected-note {{mutex acquired here}}
    if (locked){
        v += 1;
    } else {
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    }
    m.unlock(); // expected-error {{mutex 'm' is not held on every path through here}} expected-error {{releasing mutex 'm' that was not held}}
}

void TestSharedMutex::test_try_lock_unlock_3(){
    auto locked = m.try_lock();
    m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
}

void TestSharedMutex::test_try_lock_shared(){
    auto locked = m.try_lock_shared(); // expected-note {{mutex acquired here}} expected-note {{shared-mode acquired here}}
    if (locked){
        auto a = v;
    } else {
        auto a = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
    }
} // expected-error {{mutex 'm' is not held on every path through here}} expected-error {{shared-mode 'm.shared_capability' is not held on every path through here}}

void TestSharedMutex::test_try_lock_shared_unlock_shared_1(){
    auto locked = m.try_lock_shared();
    if (locked){
        auto a = v;
        m.unlock_shared();
    } else {
        auto a = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
        m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
    }
}

void TestSharedMutex::test_try_lock_shared_unlock_shared_2(){
    auto locked = m.try_lock_shared(); // expected-note {{mutex acquired here}} expected-note {{shared-mode acquired here}}
    if (locked){
        auto a = v;
    } else {
        auto a = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
    }
    m.unlock_shared(); // expected-error {{mutex 'm' is not held on every path through here}} expected-error {{releasing mutex 'm' that was not held}} expected-error {{shared-mode 'm.shared_capability' is not held on every path through here}} expected-error {{releasing shared-mode 'm.shared_capability' that was not held}}
}

void TestSharedMutex::test_try_lock_shared_unlock_shared_3(){
    auto locked = m.try_lock_shared();
    m.unlock(); // expected-error {{releasing mutex 'm' that was not held}}
}

void TestSharedMutex::test_lock_try_lock(){
    m.lock(); // expected-note 2 {{mutex acquired here}}
    auto locked = m.try_lock(); // expected-error {{acquiring mutex 'm' that is already held}}
    if (locked){
        v += 1;
        m.unlock();
    }
} // expected-error {{mutex 'm' is not held on every path through here}}

void TestSharedMutex::test_lock_shared_try_lock_shared(){
    m.lock_shared(); // expected-note 2 {{mutex acquired here}} expected-note 2 {{shared-mode acquired here}}
    auto locked = m.try_lock_shared(); // expected-error {{acquiring mutex 'm' that is already held}} expected-error {{acquiring shared-mode 'm.shared_capability' that is already held}}
    if (locked){
        auto a = v;
        m.unlock_shared();
    }
} // expected-error {{mutex 'm' is not held on every path through here}} expected-error {{shared-mode 'm.shared_capability' is not held on every path through here}}

void TestSharedMutex::test() {
    m.lock();
    v += 1;
    m.unlock();

    m.lock_shared();
    auto a = v;
    m.unlock_shared();

    auto unique_locked = m.try_lock();
    if (unique_locked){
        v += 1;
        m.unlock();
    }

    auto shared_locked = m.try_lock_shared();
    if (shared_locked){
        auto b = v;
        m.unlock_shared();
    }
}

class TestSharedLock{
private:
    astd::shared_mutex m;
    int v ASTD_GUARDED_BY(m);

public:
    void test_unique_empty_constructor();
    void test_shared_empty_constructor();

    void test_unique_scoped_constructor();
    void test_shared_scoped_constructor();

    void test_unique_scoped_constructor_unlock_1();
    void test_unique_scoped_constructor_unlock_2();
    void test_shared_scoped_constructor_unlock_1();
    void test_shared_scoped_constructor_unlock_2();

    void test_unique_lock_constructor_locked();
    void test_shared_lock_constructor_locked();

    void test_unique_defer_constructor_unlocked();
    void test_shared_defer_constructor_unlocked();

    void test_unique_defer_constructor_locked();
    void test_shared_defer_constructor_locked();

    void test_unique_adopt_constructor_unlocked();
    void test_shared_adopt_constructor_unlocked();

    void test_unique_adopt_constructor_locked_unique();
    void test_unique_adopt_constructor_locked_shared();
    void test_shared_adopt_constructor_locked_unique();
    void test_shared_adopt_constructor_locked_shared();

    void test_unique_try_lock_constructor();
    void test_shared_try_lock_constructor();

    void test_unique_assign_lock_unlock();
    void test_shared_assign_lock_unlock();
};

void TestSharedLock::test_unique_empty_constructor(){
    astd::unique_lock<astd::shared_mutex> lock;
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestSharedLock::test_shared_empty_constructor(){
    astd::shared_lock<astd::shared_mutex> lock;
    auto a = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
}

void TestSharedLock::test_unique_scoped_constructor(){
    {
        astd::unique_lock lock(m);
        v += 1;
    }
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestSharedLock::test_shared_scoped_constructor(){
    {
        astd::shared_lock lock(m);
        auto a = v;
    }
    auto b = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
}

void TestSharedLock::test_unique_scoped_constructor_unlock_1(){
    {
        astd::unique_lock lock(m);
        v += 1;
        lock.unlock();
        v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    }
}

void TestSharedLock::test_unique_scoped_constructor_unlock_2(){
    {
        astd::unique_lock lock(m);
        v += 1;
        lock.unlock();
    }
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestSharedLock::test_shared_scoped_constructor_unlock_1(){
    {
        astd::shared_lock lock(m);
        auto a = v;
        lock.unlock();
        auto b = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
    }
}

void TestSharedLock::test_shared_scoped_constructor_unlock_2(){
    {
        astd::shared_lock lock(m);
        auto a = v;
        lock.unlock();
    }
    auto b = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
}

void TestSharedLock::test_unique_lock_constructor_locked(){
    m.lock(); // expected-note {{mutex acquired here}}
    astd::unique_lock lock(m); // expected-error {{acquiring mutex 'm' that is already held}}
    m.unlock();
}

void TestSharedLock::test_shared_lock_constructor_locked(){
    m.lock(); // expected-note {{mutex acquired here}}
    astd::shared_lock lock(m); // expected-error {{acquiring mutex 'm' that is already held}}
    m.unlock();
}

void TestSharedLock::test_unique_defer_constructor_unlocked(){
    astd::unique_lock lock(m, std::defer_lock);
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
    lock.lock();
    v += 1;
}

void TestSharedLock::test_shared_defer_constructor_unlocked(){
    astd::shared_lock lock(m, std::defer_lock);
    auto a = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
    lock.lock();
    auto b = v;
}

void TestSharedLock::test_unique_defer_constructor_locked(){
    m.lock();
    astd::unique_lock lock(m, std::defer_lock); // expected-error {{cannot call function 'unique_lock' while mutex 'm' is held}}
}

void TestSharedLock::test_shared_defer_constructor_locked(){
    m.lock();
    astd::shared_lock lock(m, std::defer_lock); // expected-error {{cannot call function 'shared_lock' while mutex 'm' is held}}
}

void TestSharedLock::test_unique_adopt_constructor_unlocked(){
    astd::unique_lock lock(m, std::adopt_lock); // expected-error {{calling function 'unique_lock' requires holding mutex 'm' exclusively}}
}

void TestSharedLock::test_shared_adopt_constructor_unlocked(){
    astd::shared_lock lock(m, std::adopt_lock); // expected-error {{calling function 'shared_lock' requires holding mutex 'm'}} expected-error {{calling function 'shared_lock' requires holding shared-mode 'm.shared_capability'}}
}

void TestSharedLock::test_unique_adopt_constructor_locked_unique(){
    m.lock();
    v += 1;
    {
        astd::unique_lock lock(m, std::adopt_lock);
        v += 1;
    }
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestSharedLock::test_unique_adopt_constructor_locked_shared(){
    m.lock_shared(); // expected-note {{shared-mode acquired here}}
    astd::unique_lock lock(m, std::adopt_lock); // expected-error {{calling function 'unique_lock' requires holding mutex 'm' exclusively}}
} // expected-error {{shared-mode 'm.shared_capability' is still held at the end of function}}

void TestSharedLock::test_shared_adopt_constructor_locked_shared(){
    m.lock_shared();
    int a = v;
    {
        astd::shared_lock lock(m, std::adopt_lock);
        int b = v;
    }
    int c = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
}

void TestSharedLock::test_shared_adopt_constructor_locked_unique(){
    m.lock();
    astd::shared_lock lock(m, std::adopt_lock); // expected-error {{calling function 'shared_lock' requires holding shared-mode 'm.shared_capability'}}
}

void TestSharedLock::test_unique_try_lock_constructor(){
    astd::unique_lock lock(m, std::try_to_lock);
    v += 1; // expected-error {{writing variable 'v' requires holding mutex 'm' exclusively}}
}

void TestSharedLock::test_shared_try_lock_constructor(){
    astd::shared_lock lock(m, std::try_to_lock);
    auto a = v; // expected-error {{reading variable 'v' requires holding mutex 'm'}}
}

void TestSharedLock::test_unique_assign_lock_unlock(){
    astd::unique_lock<astd::shared_mutex> lock1;
    astd::unique_lock lock2(m);
    lock1 = std::move(lock2);
}

void TestSharedLock::test_shared_assign_lock_unlock(){
    astd::shared_lock<astd::shared_mutex> lock1;
    astd::shared_lock lock2(m);
    lock1 = std::move(lock2);
}
