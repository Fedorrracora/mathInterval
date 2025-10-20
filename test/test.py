from mathInterval import Interval as itr

a, b = itr(), itr()
a.add_interval(a.minimal, 3)
a = a.custom_transfer(lambda x:x, 1, 2)
a.add_interval(100, a.maximal)
a = a.inverse()
b.add_interval(0, 5)
print(a)
print(b)
print(a in b)
print(not a in b)
print(a not in b)