from pytest import raises
import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)

def test_add_interval():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("one_set/answers/test2.a"))

    b = True
    b = b and a.add_interval(1, 5)
    b = b and a.add_interval(6, 10)
    b = b and a.add_interval(-100, 1)
    assert verify.same(str(a), line(), *stp) and b, "error in step 1: adding independent intervals"

    b = False
    b = b or a.add_interval(11, 11)
    b = b or a.add_interval(1, 1)
    b = b or a.add_interval(11, 11)
    b = b or a.add_interval(-1, -1)
    assert verify.same(str(a), line.last, *stp) and not b, "error in step 2: adding/removing empty intervals"

    with raises(Exception):
        a.add_interval(1, -1)

    with raises(Exception):
        a.add_interval(1, a.minimal)

    assert verify.same(str(a), line.last, *stp), "error in steps 3-4: throw intervals: saving data"

    b = False
    b = b or a.add_interval(1, 5)
    b = b or a.add_interval(1, 4)
    b = b or a.add_interval(2, 5)
    assert verify.same(str(a), line.last, *stp) and not b, "error in step 5: adding intervals that not changing data"

    b = a.add_interval(3, 6)
    assert verify.same(str(a), line(), *stp) and b, "error in step 6: adding cross intervals"

    b = a.add_interval(0, 8)
    assert verify.same(str(a), line(), *stp) and b, "error in step 7: adding cross intervals"

    b = True
    b = b and a.add_interval(12, 13)
    b = b and a.add_interval(11, 12)
    assert verify.same(str(a), line(), *stp) and b, "error in step 8: adding independent intervals"

    b = a.add_interval(11, 13)
    assert verify.same(str(a), line(), *stp) and b, "error in step 9: adding cross intervals"

    b = a.add_interval(15, a.maximal)
    assert verify.same(str(a), line(), *stp) and b, "error in step 10: adding cross infinity intervals"

    b = a.add_interval(a.minimal, 12)
    assert verify.same(str(a), line(), *stp) and b, "error in step 11: adding cross infinity intervals"

    b = a.add_interval(-1000000000, 1000000000)
    assert verify.same(str(a), line(), *stp) and b, "error in step 12: adding cross intervals"

    b = a.add_interval(a.minimal, a.maximal)
    assert verify.same(str(a), line.last, *stp) and not b, "error in step 13: adding independent infinity intervals"