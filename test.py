from mathInterval import mathInterval as itr

a = itr()
a.add_interval(a.minimal, 5)
# a.remove_interval(1, 2)
print(a.custom_transfer(lambda x: -x + 1))
