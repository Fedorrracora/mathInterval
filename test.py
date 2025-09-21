from mathInterval import mathInterval as itr

a = itr()
a.add_interval(a.minimal, a.maximal)
print(a.any())
