import _mathInterval as interval
import verify_python as verify

stp = (True, True, True)
    
def test_transfer_operations():
    a = interval.Interval(interval.policy.IntTypePolicy)
    line = verify.line_checker(verify.read_file("answers/test7.a"))
    a.add_interval(1, 3)
    a.add_point(8)
    a.add_point(1)
    a.add_point(100)
    a.add_interval(a.minimal, -3)
    a.add_interval(100, a.maximal)
    a += 1
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 1: += "
    a -= 101
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 2: -= "
    a -= -100
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 3: -= with negative number "
    a += 0
    assert(verify.same(a.__str__(), line.last, *stp)), "error in step 4: += with 0 "
    a *= 1
    assert(verify.same(a.__str__(), line.last, *stp)), "error in step 5: *= with 1 "
    a *= 2
    a.add_interval(-3, 1)
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 6: *= "
    a *= -1
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 7: *= with -1 "
    a *= -2
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 8: *= with negative number "
    a //= 1
    assert(verify.same(a.__str__(), line.last, *stp)), "error in step 9: /= with 1 "
    a //= 2
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 10: /= "
    a //= -1
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 11: /= with -1 "
    a *= 2
    a //= -2
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 12: /= with negative number "
    a.add_interval(16, 17)
    a.add_interval(18, 19)
    a //= 2
    assert(verify.same(a.__str__(), line(), *stp)), "error in step 13: /= with extreme cases "
