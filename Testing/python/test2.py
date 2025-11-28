import mathInterval as interval
import verify_python as verify

def test_add_interval():
    a = interval.Interval()
    line = verify.line_checker(verify.read_file("answers/test2.a"))

    b = True
    b = b and a.add_interval(1, 5)
    b = b and a.add_interval(6, 10)
    b = b and a.add_interval(-100, 1)
    assert verify.same(str(a), line(), True, True, False) and b, "error in step 1: adding independent intervals"

    b = False
    b |= a.add_interval(11, 11)
    b |= a.add_interval(1, 1)
    b |= a.add_interval(11, 11)
    b |= a.add_interval(-1, -1)
    assert verify.same(str(a), line.last, True, True, False) and b, "error in step 2: adding/removing empty intervals"
