import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)


class TestType:
    def __init__(self, a):
        self.a = a
    def __str__(self):
        return str(self.a)
    def __lt__(self, other):
        return self.a < other.a
    def __eq__(self, other):
        return self.a == other.a


def test_many_funcs():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("custom_type/answers/test1.a"))
    a.add_interval(TestType(1), TestType(2))
    a.add_interval(TestType(3), TestType(10))
    a.add_point(TestType(2))
    a.add_point(TestType(3))
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 1: adding intervals and points"
    a.remove_point(TestType(5))
    a.remove_interval(TestType(7), TestType(8))
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 2: remove intervals and points"
    a = a.inverse()
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 3: inverse"
    a.clear()
    a.add_interval(TestType(1), a.maximal)
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 4: +INF"
    a.add_interval(a.minimal, TestType(2))
    assert(verify.same(a.__str__(), line(), *stp) and a.full()), "error in step 5: full"
    a.clear()
    a.add_point(TestType(3))
    assert(a.any() == TestType(3)), "error in step 6: any"
