from mathInterval import mathInterval_int as itr

a, b = itr(), itr()
a.add_interval(3, 8)
b.add_interval(5, 10)
a.add_interval(a.minimal, 0)
print(a)
print(b)
a += 3
print(a)
print(a.contains(a.minimal, 2))
print(a.contains(1, 7))
print(a.contains(7, a.maximal))