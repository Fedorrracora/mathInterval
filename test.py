# import sys
# sys.path.append("libs")
import mathInterval

a = mathInterval.abc_double()
print(a.print())
a.add_interval(0, 1)
print(a.print())
print(dir(mathInterval.abc_double))
print(str(a))