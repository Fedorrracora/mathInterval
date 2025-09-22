from mathInterval import mathInterval as itr

a = itr()
a.add_interval(a.minimal, 3)
a = a.custom_transfer(lambda x:x, 1, 2)
a.add_interval(100, a.maximal)
a = a.inverse()
print(a)