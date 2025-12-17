from pytest import raises
import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)

def test_add_point():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("answers/test1.a"))

    b = True
    b = b and a.add_point(1)
    b = b and a.add_point(2)
    b = b and a.add_point(-5)
    b = b and a.add_point(0)
    assert verify.same(str(a), line(), *stp) and b, "error in step 1: adding independent points"

    b = False
    b = b or a.add_point(2)
    b = b or a.add_point(0)
    assert verify.same(str(a), line.last, *stp) and not b, "error in step 2: adding cross points"

    with raises(Exception):
        a.add_point(a.minimal)

    with raises(Exception):
        a.add_point(a.maximal)

    assert verify.same(str(a), line.last, *stp), "error in steps 3-4: throw points: saving data"

    b = a.add_point(-3)
    assert verify.same(str(a), line(), *stp) and b, "error in step 5: adding independent points"