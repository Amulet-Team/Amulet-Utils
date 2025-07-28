from unittest import TestCase
from typing import Any
from threading import Lock, RLock, Condition
import time
import weakref
import gc

from amulet.utils.event import ConnectionMode

from test_amulet_utils.test_event_ import EventTest


class EventTestCase(TestCase):
    def test_event(self) -> None:
        cls = EventTest()

        var: Any = None
        count_0 = 0
        count_1 = 0
        count_2 = 0
        count_3 = 0

        def on_0():
            nonlocal count_0, var
            count_0 += 1
            var = ()

        def on_1(a: int):
            nonlocal count_1, var
            count_1 += 1
            var = (a,)

        def on_2(a: int, b: float):
            nonlocal count_2, var
            count_2 += 1
            var = (a, b)

        def on_3(a: int, b: float, c: str, d: int):
            nonlocal count_3, var
            count_3 += 1
            var = (a, b, c, d)

        token_0 = cls.event_0.connect(on_0)
        token_1 = cls.event_1.connect(on_1)
        token_2 = cls.event_2.connect(on_2)
        token_3 = cls.event_3.connect(on_3)

        cls.event_0.dispatch()
        self.assertEqual(1, count_0)
        self.assertEqual((), var)

        cls.event_1.dispatch(1)
        self.assertEqual(1, count_1)
        self.assertEqual((1,), var)

        cls.event_2.dispatch(2, 2.5)
        self.assertEqual(1, count_2)
        self.assertEqual((2, 2.5), var)

        cls.event_3.dispatch(3, 3.5, "3", 4)
        self.assertEqual(1, count_3)
        self.assertEqual((3, 3.5, "3", 4), var)

        cls.dispatch()
        self.assertEqual(2, count_0)
        self.assertEqual(2, count_1)
        self.assertEqual(2, count_2)
        self.assertEqual(2, count_3)
        self.assertEqual((1, 1.5, "Hello World", 2), var)

        cls.event_0.disconnect(token_0)
        cls.event_1.disconnect(token_1)
        cls.event_2.disconnect(token_2)
        cls.event_3.disconnect(token_3)

        cls.event_0.dispatch()
        cls.event_1.dispatch(4)
        cls.event_2.dispatch(5, 5.5)
        cls.event_3.dispatch(6, 6.5, "6", 7)
        self.assertEqual((1, 1.5, "Hello World", 2), var)

        self.assertEqual(2, count_0)
        self.assertEqual(2, count_1)
        self.assertEqual(2, count_2)
        self.assertEqual(2, count_3)

    def test_event_async(self) -> None:
        cls = EventTest()
        lock = RLock()
        condition = Condition(lock)

        var: Any = None
        count_0 = 0
        count_1 = 0
        count_2 = 0
        count_3 = 0

        step = 0

        def increment_step():
            nonlocal step
            with condition:
                step += 1
                condition.notify_all()

        def on_0():
            nonlocal count_0, var
            with lock:
                count_0 += 1
                var = ()
                increment_step()

        def on_1(a: int):
            nonlocal count_1, var
            with lock:
                count_1 += 1
                var = (a,)
                increment_step()

        def on_2(a: int, b: float):
            nonlocal count_2, var
            with lock:
                count_2 += 1
                var = (a, b)
                increment_step()

        def on_3(a: int, b: float, c: str, d: int):
            nonlocal count_3, var
            with lock:
                count_3 += 1
                var = (a, b, c, d)
                increment_step()

        token_0 = cls.event_0.connect(on_0, ConnectionMode.Async)
        token_1 = cls.event_1.connect(on_1, ConnectionMode.Async)
        token_2 = cls.event_2.connect(on_2, ConnectionMode.Async)
        token_3 = cls.event_3.connect(on_3, ConnectionMode.Async)

        with condition:
            cls.event_0.dispatch()
            time.sleep(1)
            self.assertEqual(0, count_0)
            self.assertTrue(condition.wait_for(lambda: step == 1, timeout=10))
        self.assertEqual(1, count_0)
        self.assertEqual((), var)

        with condition:
            cls.event_1.dispatch(1)
            time.sleep(1)
            self.assertEqual(0, count_1)
            self.assertTrue(condition.wait_for(lambda: step == 2, timeout=10))
        self.assertEqual(1, count_1)
        self.assertEqual((1,), var)

        with condition:
            cls.event_2.dispatch(2, 2.5)
            time.sleep(1)
            self.assertEqual(0, count_2)
            self.assertTrue(condition.wait_for(lambda: step == 3, timeout=10))
        self.assertEqual(1, count_2)
        self.assertEqual((2, 2.5), var)

        with condition:
            cls.event_3.dispatch(3, 3.5, "3", 4)
            time.sleep(1)
            self.assertEqual(0, count_3)
            self.assertTrue(condition.wait_for(lambda: step == 4, timeout=10))
        self.assertEqual(1, count_3)
        self.assertEqual((3, 3.5, "3", 4), var)

        with condition:
            cls.dispatch()
            time.sleep(1)
            self.assertEqual(1, count_0)
            self.assertEqual(1, count_1)
            self.assertEqual(1, count_2)
            self.assertEqual(1, count_3)
            self.assertTrue(condition.wait_for(lambda: step == 8, timeout=10))
        self.assertEqual(2, count_0)
        self.assertEqual(2, count_1)
        self.assertEqual(2, count_2)
        self.assertEqual(2, count_3)
        self.assertEqual((1, 1.5, "Hello World", 2), var)

        cls.event_0.disconnect(token_0)
        cls.event_1.disconnect(token_1)
        cls.event_2.disconnect(token_2)
        cls.event_3.disconnect(token_3)

        cls.event_0.dispatch()
        cls.event_1.dispatch(4)
        cls.event_2.dispatch(5, 5.5)
        cls.event_3.dispatch(6, 6.5, "6", 7)
        time.sleep(2)
        self.assertEqual((1, 1.5, "Hello World", 2), var)

        self.assertEqual(2, count_0)
        self.assertEqual(2, count_1)
        self.assertEqual(2, count_2)
        self.assertEqual(2, count_3)

    def test_exception(self) -> None:
        cls = EventTest()

        call_count = 0

        def callback():
            nonlocal call_count
            call_count += 1
            raise Exception("The following output is intended")

        token = cls.event_0.connect(callback)
        cls.event_0.dispatch()
        self.assertEqual(1, call_count)
        cls.event_0.disconnect(token)

    def test_time(self) -> None:
        cls = EventTest()

        count = 0

        def callback():
            nonlocal count
            time.sleep(1)
            count += 1

        # Synchronous
        token = cls.event_0.connect(callback)
        t = time.time()
        cls.event_0.dispatch()
        dt = time.time() - t
        self.assertEqual(1, count)
        self.assertLess(0.99, dt)
        self.assertGreater(1.5, dt)
        cls.event_0.disconnect(token)

        # Asynchronous
        token = cls.event_0.connect(callback, ConnectionMode.Async)
        t = time.time()
        cls.event_0.dispatch()
        dt = time.time() - t
        self.assertGreater(0.1, dt)
        self.assertEqual(1, count)
        time.sleep(1.5)
        self.assertEqual(2, count)
        cls.event_0.disconnect(token)

    def test_lifetime(self) -> None:
        cls = EventTest()
        event = cls.event_0
        cls_ref = weakref.ref(cls)
        del cls
        gc.collect()
        self.assertIsNotNone(cls_ref())
