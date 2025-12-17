import mathInterval as interval
import verify_python as verify

stp = (True, True, False)

def test_remove_clear_empty_full():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("answers/test4.a"))

    assert verify.same(str(a), line(), *stp) and a.empty() and (not a.full()), "error in step 1: testing empty interval"

    a.add_interval(1, 111)
    a.add_point(0)
    a.add_point(111)
    a.add_interval(1111, a.maximal)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()), "error in step 2: adding random elements"

    a.clear()
    assert verify.same(str(a), line(), *stp) and a.empty() and (not a.full()), "error in step 3: testing clear"

    a.add_interval(1, 10)
    a.add_point(1)
    a.add_interval(a.minimal, 0)
    a.add_point(15)
    a.add_interval(15, 16)

    a.remove_interval(1, 10)
    a.remove_point(15)
    a.remove_interval(a.minimal, 0)
    a.remove_point(1)
    a.remove_interval(15, 16)
    assert verify.same(str(a), line.last, *stp) and a.empty() and (not a.full()), "error in step 4: remove independent intervals and points"

    b = True
    b = b and a.add_interval(a.minimal, a.maximal)
    b = b and a.remove_point(1)
    b = b and a.remove_point(0)
    b = b and a.remove_point(15)
    b = b and a.remove_point(16)
    b = b and a.remove_point(-3)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()) and b, "error in step 5: remove independent points"

    b = False
    b = b or a.remove_point(0)
    b = b or a.remove_point(0)
    b = b or a.remove_point(-3)
    assert verify.same(str(a), line.last, *stp) and (not a.empty()) and (not a.full()) and (not b), "error in step 6: remove cross points"

    b = a.remove_interval(-3, 1)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()) and b, "error in step 7: remove cross interval"

    b = True
    b = b and a.remove_interval(5, 10)
    b = b and not a.remove_point(8)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()) and b, "error in step 8: remove interval in interval"

    b = True
    b = b and a.remove_interval(4, 5)
    b = b and a.remove_interval(1, 2)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()) and b, "error in step 9: remove border interval in interval"

    b = a.remove_interval(3, 12)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()) and b, "error in step 10: remove interval in mid of intervals"

    b = a.remove_interval(a.minimal, 5)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()) and b, "error in step 11: remove interval with left infinity border"

    b = a.remove_interval(10, a.maximal)
    assert verify.same(str(a), line(), *stp) and a.empty() and (not a.full()) and b, "error in step 12: remove interval with right infinity border"

    b = True
    b = b and a.add_interval(10, a.maximal)
    b = b and a.add_interval(a.minimal, 10)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and (not a.full()) and b, "error in step 13: interval without one point"

    b = a.add_point(10)
    assert verify.same(str(a), line(), *stp) and (not a.empty()) and a.full() and b, "error in step 14: full interval"
