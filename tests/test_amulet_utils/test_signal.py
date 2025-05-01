from unittest import TestCase
from typing import Any
from threading import Lock, RLock, Condition
import time
import weakref
import gc

from amulet.utils.signal import ConnectionMode

from test_amulet_utils.test_signal_ import SignalTest


class SignalTestCase(TestCase):
    def test_signal(self) -> None:
        cls = SignalTest()

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

        token_0 = cls.signal_0.connect(on_0)
        token_1 = cls.signal_1.connect(on_1)
        token_2 = cls.signal_2.connect(on_2)
        token_3 = cls.signal_3.connect(on_3)

        cls.signal_0.emit()
        self.assertEqual(1, count_0)
        self.assertEqual((), var)

        cls.signal_1.emit(1)
        self.assertEqual(1, count_1)
        self.assertEqual((1,), var)

        cls.signal_2.emit(2, 2.5)
        self.assertEqual(1, count_2)
        self.assertEqual((2, 2.5), var)

        cls.signal_3.emit(3, 3.5, "3", 4)
        self.assertEqual(1, count_3)
        self.assertEqual((3, 3.5, "3", 4), var)

        cls.emit()
        self.assertEqual(2, count_0)
        self.assertEqual(2, count_1)
        self.assertEqual(2, count_2)
        self.assertEqual(2, count_3)
        self.assertEqual((1, 1.5, "Hello World", 2), var)

        cls.signal_0.disconnect(token_0)
        cls.signal_1.disconnect(token_1)
        cls.signal_2.disconnect(token_2)
        cls.signal_3.disconnect(token_3)

        cls.signal_0.emit()
        cls.signal_1.emit(4)
        cls.signal_2.emit(5, 5.5)
        cls.signal_3.emit(6, 6.5, "6", 7)
        self.assertEqual((1, 1.5, "Hello World", 2), var)

        self.assertEqual(2, count_0)
        self.assertEqual(2, count_1)
        self.assertEqual(2, count_2)
        self.assertEqual(2, count_3)

    def test_signal_async(self) -> None:
        cls = SignalTest()
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

        token_0 = cls.signal_0.connect(on_0, ConnectionMode.Async)
        token_1 = cls.signal_1.connect(on_1, ConnectionMode.Async)
        token_2 = cls.signal_2.connect(on_2, ConnectionMode.Async)
        token_3 = cls.signal_3.connect(on_3, ConnectionMode.Async)

        with condition:
            cls.signal_0.emit()
            time.sleep(1)
            self.assertEqual(0, count_0)
            self.assertTrue(condition.wait_for(lambda: step == 1, timeout=10))
        self.assertEqual(1, count_0)
        self.assertEqual((), var)

        with condition:
            cls.signal_1.emit(1)
            time.sleep(1)
            self.assertEqual(0, count_1)
            self.assertTrue(condition.wait_for(lambda: step == 2, timeout=10))
        self.assertEqual(1, count_1)
        self.assertEqual((1,), var)

        with condition:
            cls.signal_2.emit(2, 2.5)
            time.sleep(1)
            self.assertEqual(0, count_2)
            self.assertTrue(condition.wait_for(lambda: step == 3, timeout=10))
        self.assertEqual(1, count_2)
        self.assertEqual((2, 2.5), var)

        with condition:
            cls.signal_3.emit(3, 3.5, "3", 4)
            time.sleep(1)
            self.assertEqual(0, count_3)
            self.assertTrue(condition.wait_for(lambda: step == 4, timeout=10))
        self.assertEqual(1, count_3)
        self.assertEqual((3, 3.5, "3", 4), var)

        with condition:
            cls.emit()
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

        cls.signal_0.disconnect(token_0)
        cls.signal_1.disconnect(token_1)
        cls.signal_2.disconnect(token_2)
        cls.signal_3.disconnect(token_3)

        cls.signal_0.emit()
        cls.signal_1.emit(4)
        cls.signal_2.emit(5, 5.5)
        cls.signal_3.emit(6, 6.5, "6", 7)
        time.sleep(2)
        self.assertEqual((1, 1.5, "Hello World", 2), var)

        self.assertEqual(2, count_0)
        self.assertEqual(2, count_1)
        self.assertEqual(2, count_2)
        self.assertEqual(2, count_3)

#     def test_exception(self) -> None:
#         cls = SignalTest()
#
#         call_count = 0
#
#         def callback():
#             nonlocal call_count
#             call_count += 1
#             raise Exception("The following output is intended")
#
#         token = cls.signal_0.connect(callback)
#         cls.signal_0.emit()
#         self.assertEqual(1, call_count)
#         cls.signal_0.disconnect(token)
#
#     def test_time(self) -> None:
#         cls = SignalTest()
#
#         count = 0
#
#         def callback():
#             nonlocal count
#             time.sleep(1)
#             count += 1
#
#         # Synchronous
#         token = cls.signal_0.connect(callback)
#         t = time.time()
#         cls.signal_0.emit()
#         dt = time.time() - t
#         self.assertEqual(1, count)
#         self.assertLess(0.99, dt)
#         self.assertGreater(1.5, dt)
#         cls.signal_0.disconnect(token)
#
#         # Asynchronous
#         token = cls.signal_0.connect(callback, ConnectionMode.Async)
#         t = time.time()
#         cls.signal_0.emit()
#         dt = time.time() - t
#         self.assertGreater(0.1, dt)
#         self.assertEqual(1, count)
#         time.sleep(1.5)
#         self.assertEqual(2, count)
#         cls.signal_0.disconnect(token)
#
#     def test_lifetime(self) -> None:
#         cls = SignalTest()
#         signal = cls.signal_0
#         cls_ref = weakref.ref(cls)
#         del cls
#         gc.collect()
#         self.assertIsNotNone(cls_ref())
