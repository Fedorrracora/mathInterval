from mathInterval import mathInterval_int as itr

a, b = itr(), itr()
a.add_interval(3, 8)
b.add_interval(5, 10)
a.add_interval(a.minimal, 0)
print(a)
print(b)
a *= 2
print(a)
a //= 10000000000000
print(a)
# a //= 0
# print(a)