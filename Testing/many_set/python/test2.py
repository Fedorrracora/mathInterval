import _mathInterval as interval

def test_subset():
    a, b = interval.Interval(), interval.Interval()
    assert(a.issubset(b) and a.issuperset(b) and a.isdisjoint(b)), "error in step 1: empty"
    a.add_point(2)
    assert(1 - a.issubset(b) and a.issuperset(b) and a.isdisjoint(b)), "error in step 2: " + a.__str__() + " and " + b.__str__() + ""
    b.add_point(2)
    assert(a.issubset(b) and a.issuperset(b) and 1 - a.isdisjoint(b)), "error in step 3: " + a.__str__() + " and " + b.__str__() + ""
    a.add_interval(8, 10)
    assert(1 - a.issubset(b) and a.issuperset(b) and 1 - a.isdisjoint(b)), "error in step 4: " + a.__str__() + " and " + b.__str__() + ""
    a.clear()
    b.clear()
    a.add_interval(8, 9)
    b.add_interval(9, 10)
    assert(1 - a.issubset(b) and 1 - a.issuperset(b) and a.isdisjoint(b)), "error in step 5: " + a.__str__() + " and " + b.__str__() + ""
    a = a.inverse()
    assert(1 - a.issubset(b) and a.issuperset(b) and 1 - a.isdisjoint(b)), "error in step 6: " + a.__str__() + " and " + b.__str__() + ""
    assert(a.issubset(a) and a.issuperset(a) and 1 - a.isdisjoint(a)), "error in step 7: self"
    a.clear()
    assert(a.issubset(a) and a.issuperset(a) and a.isdisjoint(a)), "error in step 8: self with empty"
