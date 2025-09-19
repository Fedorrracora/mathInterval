# import sys
# sys.path.append("libs")
import mathInterval

a = mathInterval.mathInterval() # использует py::object
a.add_interval(a.minimal, 2)
a.add_interval(5, 8)
a.add_interval(15, a.maximal)
print(a)
a.add_interval(2, 15)
print(a)
a.add_interval(1, 16)
print(a)

print(dir(mathInterval.mathInterval))
b = mathInterval.mathInterval_int()
b.add_interval(b.minimal, 2)
b.add_interval(5, 8)
b.add_interval(15, b.maximal)
print(b)