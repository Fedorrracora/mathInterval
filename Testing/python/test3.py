import _mathInterval as interval
import verify_python as verify

def test_multi_add():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("answers/test3.a"))

    a.add_interval(1, 10)
    a.add_point(10)
    assert verify.same(str(a), line(), True, True, False), "error in step 1: adding border point"

    b = a.add_interval(10, 11)
    assert verify.same(str(a), line(), True, True, False) and b, "error in step 2: meet 2 intervals: last - interval"

    a.add_interval(11, 20)
    b = a.add_point(11)
    assert verify.same(str(a), line(), True, True, False) and b, "error in step 3: meet 2 intervals: last - point"

    a.add_interval(20, 30)
    a.add_point(30)
    a.add_point(1)
    assert verify.same(str(a), line(), True, True, False), "error in step 4: random additions"

    b = False
    b = b or a.add_point(1)
    b = b or a.add_point(30)
    b = b or a.add_point(25)
    b = b or a.add_point(10)
    b = b or a.add_interval(10, 10)
    b = b or a.add_interval(10, 15)
    b = b or a.add_interval(10, 20)
    b = b or a.add_interval(1, 15)
    b = b or a.add_interval(25, 30)
    assert verify.same(str(a), line.last, True, True, False) and not b, "error in step 5: adding intervals/points that not changing data"

    a.add_point(-1)
    a.add_point(0)
    a.add_point(31)
    a.add_point(32)
    assert verify.same(str(a), line(), True, True, False), "error in step 6: points in left and right"

    a.add_interval(-10, 1)
    assert verify.same(str(a), line(), True, True, False), "error in step 7: coating interval"

    a.add_point(-15)
    a.add_point(-14)
    a.add_point(-13)
    a.add_point(-12)
    a.add_point(-11)
    a.add_point(-10)
    a.add_interval(-20, -15)
    a.add_point(-21)
    assert verify.same(str(a), line(), True, True, False), "error in step 8: points formatting"

    a.add_interval(a.minimal, -21)
    a.add_interval(35, a.maximal)
    assert verify.same(str(a), line(), True, True, False), "error in step 9: +-INF"
