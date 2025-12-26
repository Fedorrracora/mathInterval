import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)


def test_inverse():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("one_set/answers/test5.a"))

    a.add_interval(1, 2)
    assert (verify.same(a.inverse().__str__(), line(), *stp)), "error in step 1: inverse one interval"
    a.clear()

    a.add_point(5)
    assert (verify.same(a.inverse().__str__(), line(), *stp)), "error in step 2: inverse one point"
    a.clear()

    a.add_interval(1, 10)
    a.add_point(1)
    a.add_point(10)
    assert (
        verify.same(a.inverse().__str__(), line(), *stp)), "error in step 3: inverse one interval with points around"
    a.clear()

    assert (verify.same(a.inverse().__str__(), line(), *stp)), "error in step 4: inverse empty interval"

    a.add_interval(a.minimal, a.maximal)
    assert (verify.same(a.inverse().__str__(), line(), *stp)), "error in step 5: inverse interval (-INF +INF)"
    a.clear()

    a.add_point(-3)
    a.add_point(0)
    a.add_point(1)
    a.add_interval(1, 2)
    a.add_point(2)
    a.add_interval(5, 10)
    a.add_point(10)
    a.add_point(11)
    a.add_interval(11, 100)
    a.add_interval(105, 110)
    a.add_point(1000)
    a.add_interval(1001, 1002)
    a.add_interval(1002, 1003)
    a.add_interval(1003, 1004)
    a.add_point(1005)
    a.add_interval(1006, 1007)
    a.add_point(1007)
    a.add_point(1008)
    a.add_interval(1009, 1010)
    a.add_interval(1012, 1013)
    a.add_interval(1015, 1016)
    assert (
        verify.same(a.inverse().__str__(), line(), *stp)), "error in step 5: inverse many random points and intervals"
    a.clear()

    a.add_interval(a.minimal, 10)
    assert (
        verify.same(a.inverse().__str__(), line(), *stp)), "error in step 5: inverse many random points and intervals"
    a.clear()

    a.add_interval(10, a.maximal)
    assert (
        verify.same(a.inverse().__str__(), line(), *stp)), "error in step 5: inverse many random points and intervals"
    a.clear()
