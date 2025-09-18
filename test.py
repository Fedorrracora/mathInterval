# import sys
# sys.path.append("libs")
import mathInterval

a = mathInterval.mathInterval_float()
print(a)
a.add_interval(0, 1)
print(a)
print(dir(mathInterval.mathInterval_float))
print(str(a))