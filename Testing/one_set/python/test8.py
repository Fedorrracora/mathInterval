import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)
    
def test_remainder():
    a = interval.Interval(interval.policy.IntTypePolicy)
    line = verify.line_checker(verify.read_file("one_set/answers/test8.a"))
    a.add_interval(1, 3)
    a %= 4
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 1: %= with one interval not changing data"
    a %= 3
    assert(verify.same(a.__str__(), line.last, *stp)), "error in step 2: %= with one interval not changing data"
    a %= 2
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 3: %= with one interval"
    a.clear()
    a.add_interval(1, 4)
    a %= 2
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 4: %= with one interval to full interval"
    a.clear()
    a %= 3
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 5: %= with empty interval"
    a.clear()
    a.add_interval(-1, 0)
    a %= 3
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 6: %= with negative interval"
    a.clear()
    a.add_interval(-1, 1)
    a %= 3
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 7: %= with interval with positive and negative borders"
    a.clear()
    a.add_interval(0, 3)
    a %= 3
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 8: %= with interval with equal values on the balance"
    a.clear()
    a.add_interval(0, 6)
    a %= 3
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 9: %= with interval with equal values on the balance"
    a.clear()
    a.add_point(1)
    a.add_interval(1, 3)
    a.add_point(3)
    a %= 3
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 10: %= with interval and points"
    a.clear()
    a.add_point(-1)
    a %= 3
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 11: %= with negative point"
    a.clear()
    a.add_interval(0, a.maximal)
    a %= 2
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 12: %= with infinity border"
