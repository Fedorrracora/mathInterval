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


def test_custom_transfer():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("custom_type/answers/test2.a"))
    a.add_interval(TestType(1), TestType(2))
    a.add_interval(TestType(4), TestType(10))
    a.add_point(TestType(4))
    a = a.custom_transfer(lambda t: TestType(t.a + 1))
    assert (verify.same(a.__str__(), line(), *stp)), "error in step 1: += 1"
    a.add_interval(TestType(11), a.maximal)
    a = a.custom_transfer(lambda t: TestType(-t.a), a.maximal, a.minimal)
    assert (verify.same(a.__str__(), line(), *stp)), "error in step 2: *= -1"
    a = a.custom_transfer(lambda t: t, TestType(-100), TestType(100))
    assert (verify.same(a.__str__(), line(), *stp)), "error in step 3: set borders"
