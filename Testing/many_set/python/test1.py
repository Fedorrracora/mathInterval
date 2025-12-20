import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)
    
def test_equal():
    a, b = interval.Interval(), interval.Interval()
    assert(a == b), "error in step 1: empty"
    a.add_interval(1, 2)
    b.add_interval(1, 2)
    assert(a == b), "error in step 2: equal one interval"
    a.add_point(2)
    b.add_point(2)
    assert(a == b), "error in step 3: equal one interval and point"
    b.remove_point(2)
    assert(a != b), "error in step 4: not equal one point"
    a.remove_point(2)
    assert(a == b), "error in step 5: equal one interval"
    a.add_interval(2, 3)
    b.clear()
    b.add_interval(1, 3)
    assert(a != b), "error in step 6: not equal without point"
    a.add_point(2)
    assert(a == b), "error in step 7: equal"
    a.add_interval(100, a.maximal)
    assert(a != b), "error in step 8: not equal with +INF"
    b.add_interval(100, a.maximal)
    assert(a == b), "error in step 9: equal with +INF"
