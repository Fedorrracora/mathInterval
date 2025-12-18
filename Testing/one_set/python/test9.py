import mathInterval as interval

def correct(a, empty : bool):
    x = a.any()
    if empty:
        return x is None
    if x is None:
        return False
    return x in a

def test_any():
    a = interval.Interval(interval.policy.IntTypePolicy)
    a.add_interval(1, 4)
    assert(correct(a, False)), "error in step 1: " + a.__str__() +  ""
    a.clear()
    a.add_interval(1, 3)
    assert(correct(a, False)), "error in step 2: " + a.__str__() +  ""
    a.clear()
    a.add_interval(1, 2)
    assert(correct(a, True )), "error in step 3: " + a.__str__() +  ""
    a.clear()
    assert(correct(a, True )), "error in step 4: " + a.__str__() +  ""
    a.clear()
    a.add_interval(1, 2)
    a.add_interval(2, 3)
    a.add_interval(3, 4)
    assert(correct(a, True )), "error in step 5: " + a.__str__() +  ""
    a.add_point(4)
    assert(correct(a, False)), "error in step 6: " + a.__str__() +  ""
    a.clear()
    a.add_interval(-3, -2)
    assert(correct(a, True )), "error in step 7: " + a.__str__() +  ""
    a.clear()
    a.add_interval(-3, -1)
    assert(correct(a, False)), "error in step 8: " + a.__str__() +  ""
    a.clear()
    a.add_interval(a.minimal, -1)
    assert(correct(a, False)), "error in step 9: " + a.__str__() +  ""
    a.clear()
    a.add_interval(1, a.maximal)
    assert(correct(a, False)), "error in step 10: " + a.__str__() +  ""
    a.clear()
    a.add_interval(a.minimal, a.maximal)
    assert(correct(a, True)), "error in step 11: " + a.__str__() +  ""
