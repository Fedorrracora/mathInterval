from mathInterval import mathInterval as itr

a, b = itr(), itr()
a.add_interval(3, 8)
b.add_interval(5, 10)
a.add_interval(a.minimal, 0)
print(a)
print(b)
print(a.any())
print(a.any(lambda x: x - 10, lambda x: x + 1, lambda x, y: (x + y) / 2))