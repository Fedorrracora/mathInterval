import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)


def test_in_inverse_points_only():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("one_set/answers/test6.a"))
    a.add_interval(1, 3)
    assert (1 - a.points_only()), "error in step 1: points_only on one interval"
    a.remove_interval(1, 2)
    a.remove_interval(2, 3)
    assert (verify.same(a.__str__(), line(), *stp) and a.points_only()), "error in step 2: points_only on one point"
    a.add_interval(a.minimal, 2)
    assert (1 - a.points_only()), "error in step 3: points_only on interval with -INF"
    a.add_interval(2, a.maximal)
    assert (verify.same(a.__str__(), line(),
                        *stp) and 1 - a.points_only() and a.full()), "error in step 4: points_only on interval with -INF"
    a = a.inverse()
    assert (verify.same(a.__str__(), line(),
                        *stp) and a.points_only() and 1 - a.full()), "error in step 5: inverse (-INF +INF)"
    a.add_interval(1, 10)
    a.add_interval(100, a.maximal)
    a.add_point(10)
    a.add_point(28)
    a.add_point(28)
    assert (1 - a.contains(-3) and
            1 - a.contains(1) and
            a.contains(3) and
            a.contains(10) and
            1 - a.contains(14) and
            a.contains(28) and
            1 - a.contains(-7) and
            1 - a.contains(100) and
            a.contains(1000) and
            1 - a.contains(a.maximal) and
            1 - a.contains(a.minimal)), "error in step 6: in with point"
    a = a.inverse()
    assert (verify.same(a.__str__(), line(), *stp) and
            a.contains(3, 3) and
            a.contains(10, 28) and
            a.contains(10, 27) and
            a.contains(11, 28) and
            a.contains(11, 27) and
            1 - a.contains(9, 28) and
            1 - a.contains(10, 29) and
            1 - a.contains(9, 29) and
            a.contains(-1000, 1) and
            a.contains(a.minimal, -100) and
            1 - a.contains(a.minimal, a.maximal) and
            1 - a.contains(15, 55) and
            1 - a.contains(1000, 1001) and
            a.contains(1000, 1000) and
            a.contains(1, 1)), "error in step 7: in with interval after inverse"
