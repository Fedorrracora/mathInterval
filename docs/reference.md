# 📑 Reference Documentation

# Core Objects

## C++
### interval::interval\<T, [policy]\>
**Description:** Stores a set of points and intervals. Supports open, closed, and infinite boundaries.
* The type T must have a comparison operator `<`

### interval::minimal\<T\> and interval::maximal\<T\>
* Represent −∞ and +∞. They can be used in any function of the object.
* `interval::minimal<T>()` — negative infinity.
* `interval::maximal<T>()` — positive infinity.

### namespace interval::policy::
**Description:** namespace for processing policies. They are described below.

## Python
### Interval([policy])
**Description:** Stores a set of points and intervals. Supports open, closed, and infinite boundaries.
* Can contain any data type, but all used objects must support the `<` operator.

### namespace policy.
**Description:** namespace for processing policies. They are described below.

---

# Set Operations

| Operation in C++                                                   | Operation in Python                               | Complexity | Description                                                                                                                                                                                            |
|--------------------------------------------------------------------|---------------------------------------------------|------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Creation `interval::interval<T, [policy]>`                         | Creation `Interval(policy)`                       | `O(1)`     | —                                                                                                                                                                                                      |
| Separate classes `interval::minimal<T>` and `interval::maximal<T>` | `minimal` and `maximal` (static class attributes) | `O(1)`     | Represent −∞ and +∞. They can be used in any function of the object.                                                                                                                                   |
| Operators `==` and `!=`                                            | Operators `==` and `!=`                           | `O(n)`     | Compare intervals (It is impossible to compare different types and different policies).                                                                                                                |
| `add_interval(x, y)`, `remove_interval(x, y)`                      | `add_interval(x, y)`, `remove_interval(x, y)`     | `O(log n)` | Add or remove an interval (note that interval boundaries are not included). Return true if the set changes. Note that the added interval must be valid in length (x <= y).                             |
| `add_point(x)`, `remove_point(x)`                                  | `add_point(x)`, `remove_point(x)`                 | `O(log n)` | Add or remove a point. Return true if the set changes. Note that the added/removed point cannot be -INF and +INF                                                                                       |
| `empty()`                                                          | `empty()`                                         | `O(1)`     | Return true if the set is empty.                                                                                                                                                                       |
| `full()`                                                           | `full()`                                          | `O(1)`     | Return true if the set is (-INF; +INF).                                                                                                                                                                |
| `clear()`                                                          | `clear()`                                         | `O(1)`     | Clear the set.                                                                                                                                                                                         |
| `in(x)`                                                            | Operator `in` and `contains(x)`                   | `O(log n)` | Return true if the given point (x) belongs to the set.                                                                                                                                                 |
| `in(x, y)`                                                         | `contains(x, y)`                                  | `O(log n)` | Return true if the given interval (x, y) belongs to the set (note that interval boundaries are not included).                                                                                          |
| `in(x)` and `issubset(x)` (x - another set)                        | Operator `in` and `issubset(x)` (x - another set) | `O(n)`     | Return true if the current set is a subset of the given set (x).                                                                                                                                       |
| `issuperset(x)` (x - another set)                                  | `issuperset(x)` (x - another set)                 | `O(n)`     | Return true if the current set is a superset of the given set (x).                                                                                                                                     |
| `isdisjoint(x)` (x - another set)                                  | `isdisjoint(x)` (x - another set)                 | `O(n)`     | Return true if the current set has no common points with the given set (x).                                                                                                                            |
| `points_only()`                                                    | `points_only()`                                   | `O(1)`     | Return true if the current set contains only points (an empty set also returns true).                                                                                                                  |
| Operators `+` and `\|`                                             | Operators `+` and `\|`                            | `O(n)`     | Generate the union of sets.                                                                                                                                                                            |
| Operators `+=` and `\|=`                                           | Operators `+=` and `\|=`                          | `O(n)`     | Generate the union of sets, storing the result in the first set.                                                                                                                                       |
| Operator `-` between two sets                                      | Operator `-` between two sets                     | `O(n)`     | Generate the difference of sets.                                                                                                                                                                       |
| Operator `-=` between two sets                                     | Operator `-=` between two sets                    | `O(n)`     | Generate the difference of sets, storing the result in the first set.                                                                                                                                  |
| Operators `*` and `&` between two sets                             | Operators `*` and `&` between two sets            | `O(n)`     | Generate the intersection of sets.                                                                                                                                                                     |
| Operators `*=` and `&=` between two sets                           | Operators `*=` and `&=` between two sets          | `O(n)`     | Generate the intersection of sets, storing the result in the first set.                                                                                                                                |
| Operator `^` between two sets                                      | Operator `^` between two sets                     | `O(n)`     | Generate the symmetric difference of sets.                                                                                                                                                             |
| Operator `^=` between two sets                                     | Operator `^=` between two sets                    | `O(n)`     | Generate the symmetric difference of sets, storing the result in the first set.                                                                                                                        |
| `inverse()`                                                        | `inverse()`                                       | `O(n)`     | Return a set containing all points except those in the current set (difference of the set (-INF, +INF) and the current set).                                                                           |
| `to_string()`                                                      | `str()` (external built-in operator)              | `O(n)`     | Return a string representation of the current set (mathematical notation). In C++, the stored type must have the \<\< operator for `std::ostream`. In Python, a type must have the `__str__` operator. |

---

# Operations Available Only for Numeric Sets
* In C++: `T` - numeric type (defined by policies (described below))
* In Python: defined by policies (described below)
* Complexity of all operations: `O(n)`

| Operation in C++                                   | Operation in Python                                         | Description                                                                                                                                                                                                                                                             |
|----------------------------------------------------|-------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Operators `+` and `-` between a set and a number   | Operators `+` and `-` between a set and a number            | Generate a set where elements of the current set are shifted by ±x (each point y in the set is mapped to a point equal to y ± x).                                                                                                                                       |
| Operators `+=` and `-=` between a set and a number | Operators `+=` and `-=` between a set and a number          | Generate a set where elements of the current set are shifted by ±x (each point y in the set is mapped to a point equal to y ± x), storing the result in the first set.                                                                                                  |
| Operator `*` between a set and a number            | Operator `*` between a set and a number                     | Generate a set where elements of the current set are multiplied by x (each point y in the set is mapped to a point equal to y * x). Supports x <= 0.                                                                                                                    |
| Operator `*=` between a set and a number           | Operator `*=` between a set and a number                    | Generate a set where elements of the current set are multiplied by x (each point y in the set is mapped to a point equal to y * x), storing the result in the first set. Supports x <= 0.                                                                               |
| Operator `/` between a set and a number            | Operator `/` in float policy; Operator `//` in int policy   | Generate a set where elements of the current set are divided by x (each point y in the set is mapped to a point equal to y / x). Supports x < 0. Division by 0 is not allowed.                                                                                          |
| Operator `/=` between a set and a number           | Operator `/=` in float policy; Operator `//=` in int policy | Generate a set where elements of the current set are divided by x (each point y in the set is mapped to a point equal to y / x), storing the result in the first set. Supports x < 0. Division by 0 is not allowed.                                                     |
| Operator `%` between a set and a number            | Operator `%` between a set and a number                     | Works only in integer sets (defined by policies). Generates a set where elements of the current set are replaced with the remainder of division by x (each point y in the set is mapped to a point equal to y % x). x must be > 0.                                      |
| Operator `%=` between a set and a number           | Operator `%=` between a set and a number                    | Works only in integer sets (defined by policies). Generates a set where elements of the current set are replaced with the remainder of division by x (each point y in the set is mapped to a point equal to y % x), storing the result in the first set. x must be > 0. |

---

# Advanced Operations

All operations listed here have complexity `O(n)`.

### any()

**Description:** Attempts to find any element belonging to the given set.

In C++ only: can be called with `any(true)`. The difference is described below.

In C++, returns `std::optional<T>`, as a point may not always be found. In Python, `None` is returned in such cases.

- If a known point exists, it will be returned.
- In C++ only: If the set equals the interval (-INF, +INF) and simple `any()` is called, `T{}` will be returned. If `any(true)` is called, `std::nullopt` will be returned.
- If the set is numeric, the built-in algorithm will search for points within the set's intervals.
- In C++ only: If the set is a string set, the built-in algorithm will search for points within the set's intervals. Strings must contain only lowercase English letters.

### any(func1, func2, func3, x)

**Description:** Attempts to find any element belonging to the given set.

In C++, returns `std::optional<T>`, as a point may not always be found. In Python, `None` is returned in such cases.

- **func1(x)** – called if there is an interval `(-INF; x)`. 
Takes one argument.
- **func2(x)** – called if there is an interval `(x; +INF)`. 
Takes one argument.
- **func3(x, y)** – called if there is an interval `(x; y)`. 
Takes two arguments.
- **x** – result for the interval `(-INF, +INF)`.

In C++, lambdas return `std::optional<T>` and take variables of type `const T&`.
If there are no matching points in the given interval, `std::nullopt` can be returned.

In Python, lambdas return a variable of the appropriate type.
If there are no matching points in the given interval, `None` can be returned.

### custom_transfer(func)

**Description:** Transfers all elements in this set to a new set.
The transfer is handled by a lambda.

- **func(x)** – called for a point or interval boundary, 
takes one argument. Returns the new value of the point or interval boundary.

`-INF` and `+INF` remain unchanged.

If the first value of an interval becomes greater than the second, the function automatically swaps them.

### custom_transfer(func, x, y)

**Description:** Transfers all elements in this set to a new set.
The transfer is handled by a lambda.

- **func(x)** – called for a point or interval boundary, takes one argument. Returns the new value of the point or interval boundary.
- **x** – new value for the interval boundary starting at `-INF`.
- **y** – new value for the interval boundary ending at `+INF`.

If the first value of an interval becomes greater than the second, the function automatically swaps them.

### _custom_transfer(func1, func2) (C++ only)

**Description:** Transfers all elements from the data and returns a new `interval::interval`.

**Warning!:**
This function is intended for working with **raw memory**.

- **func1(x, y)** – returns the new value of the interval in its original form (`std::pair<inner_type, inner_type>`).
Takes one argument `const std::pair<inner_type, inner_type>&` (previous interval value).
- **func2(x)** – returns the new value of the point in its original form, takes one argument `const T&` (previous point value).
The point cannot be `-INF` or `+INF`, so it is processed in a standard way.

Here, "inner_type" is `std::pair<int, T>`.
Values of `inner_type` elements:

| Value of first | Real value                                              | Value of second                                                                          |
|---------------:|---------------------------------------------------------|------------------------------------------------------------------------------------------|
|            `0` | `-INF`                                                  | Automatically becomes `T{}`, as `-INF` is unique, and the value of second is irrelevant. |
|            `1` | Standard value                                          | Contains the required value.                                                             |
|            `2` | `+INF`                                                  | Automatically becomes `T{}`, as `+INF` is unique, and the value of second is irrelevant. |
|          Other | **Undefined behavior** → throws `std::range_error`      |                                                                                          |

Additional conditions:

- If the first value of an interval becomes greater than the second, an `std::overflow_error` is automatically generated (invalid interval).
- If the first value of an interval equals the second, that interval will not be added.

---

# Policy

## Type policies
**Description:** Allows you to override how the type detects.
* The availability of additional operations depends on this. They are described above.
* In C++, the policy goes as the second argument in the template when creating an object. In Python, policy comes as a single element in the constructor.
* In C++, this can be useful for wrapper classes. The object must have all the same operations as the type that T claims to be.
* In Python, type policies play a more important role. If you apply a policy of a certain type to an `interval()` object, it will only be able to have that type inside. Only in this case, additional operations will be available.
* You need to understand that type policies exist only for convenience. Any operation that depends on type policies can be done through advanced operations (described above).

### Default values
* In C++ - `interval::policy::standard_type_policy`.
* In Python - `policy.UnknownTypePolicy`.

| Operation in C++                         | Operation in Python        | Description                                                                                                                                                            |
|------------------------------------------|----------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `interval::policy::standard_type_policy` | -                          | A standard policy for type handling. That the type is numeric is defined by `std::<T>is_arithmetic_v`. That the type is integer is defined by `std::<T>is_integral_v`. |
| `interval::policy::unknown_type_policy`  | `policy.UnknownTypePolicy` | In C++, the policy says that the type is unknown. It will not have numeric set operations. In Python, it is used as the default.                                       |
| `interval::policy::int_type_policy`      | `policy.IntTypePolicy`     | In C++, the policy says that the type is a numeric integer. In Python, it opens up the possibilities of numeric integer sets.                                          |
| `interval::policy::float_type_policy`    | `policy.FloatTypePolicy`   | In C++, the policy says that the type is a numeric non-integer. In Python, it opens up the possibilities of numeric non-integer sets.                                  |
| `interval::policy::string_type_policy`   | -                          | In C++, the policy says that the type is a string. Almost useless, but has a built-in generator for `any()`                                                            |

# Exceptions and Errors
## C++
### _custom_transfer(func1, func2)
- `std::range_error` if the format of inner_type is violated.

### add_interval(x, y) and remove_interval(x, y)
- `std::range_error` if the interval has a negative length.

### add_point(x) and remove_point(x)
- `std::range_error` if the point is +INF or -INF.

### Operators % and %=
- `std::logic_error` if the modulus coefficient is <= 0.

### in(x, y)
- `std::logic_error` if the interval has a negative length.

## Python

### Same errors as above, but:
- `std::range_error` is replaced with `IndexError`.
- `std::logic_error` is replaced with `RuntimeError`.

### If contained objects do not have the `<` operator:
- `TypeError`

### If you pass an unknown object to the `Interval([policy])` constructor, rather than type policy
- `TypeError`

---

# Example
## C++
```c++
#include <iostream>
#include <optional>
#include "interval.h"

int main() {
    // Adding points and intervals
    interval::interval<int> a;
    std::cout << a.print() << "\n"; // *empty*
    a.add_point(5);
    std::cout << a.print() << "\n"; // {5}
    a.add_interval(1, 3);
    std::cout << a.print() << "\n"; // (1; 3) U {5}

    // Intersection of sets
    a.add_interval(interval::minimal<int>(), 2);
    std::cout << a.print() << "\n"; // (-INF; 3) U {5}

    interval::interval<int> b;
    b.add_interval(2, 10);

    auto c = a * b;
    std::cout << c.print() << "\n"; // (2; 3) U {5}
    std::cout << c.inverse().print() << "\n"; // (-INF; 2] U [3; 5) U (5; +INF)
    
    // Custom operations
    c.clear();
    c.add_interval(1, 4);
    // This is one of the implementations of the any() function for integers.
    // It will always return some point in the set if it exists.
    auto d = c.any(
            [](const auto &x)->std::optional<int> {return x - 1;},
            [](const auto &x)->std::optional<int> {return x + 1;},
            [](const auto &x, const auto &y)->std::optional<int>
            {
                if (x + 1 < y) {return x + 1;} return std::nullopt;
            },
            0);
    std::cout << d.value() << "\n"; // 2
    
    // Create a large set
    c.clear();
    c.add_interval(interval::minimal<int>(), 3);
    c.add_interval(90, interval::maximal<int>());
    c.add_interval(12, 50);
    c.add_interval(56, 70);
    c.add_point(50);
    c.add_point(71);
    c.add_point(90);
    c.add_point(9);
    std::cout << c.print() << "\n"; // (-INF; 3) U {9} U (12; 50) U (56; 70) U {71} U [90; +INF)
    
    // This function shifts all points in the set 4 units to the right,
    // limiting the range of values to 0–100
    // Essentially the same as:
    // c += 4
    // auto x = interval::interval<int>();
    // x.add_interval(0, 100)
    // c &= x
    c = c.custom_transfer(
        [](const auto &x) {return x + 4;},
        0,
        100
        );
    std::cout << c.print() << "\n"; // (0; 7) U {13} U (16; 54] U (60; 74) U {75} U [94; 100)
}
```
## Python

```python
from mathInterval import Interval, policy

# Adding points and intervals
a = Interval(policy.IntTypePolicy)
print(a)  # *Empty*
a.add_point(5)
print(a)  # {5}
a.add_interval(1, 3)
print(a)  # (1; 3) U {5}

# Intersection of sets
a.add_interval(a.minimal, 2)
print(a)  # (-INF; 3) U {5}

b = Interval(policy.IntTypePolicy)
b.add_interval(2, 10)

c = a * b
print(c)  # (2; 3) U {5}
print(c.inverse())  # (-INF; 2] U [3; 5) U (5; +INF)

# Custom operations
c = Interval()
c.add_interval(1, 4)
# This is one of the implementations of the any() function for integers.
# It will always return some point in the set if it exists.
# Note - it works in the Interval class without adding IntTypePolicy
d = c.any(lambda x: x - 1,
          lambda x: x + 1,
          lambda x, y: x + 1 if x + 1 < y else None,
          0)
print(d)  # 2

# Create a large set
c.clear()
c.add_interval(c.minimal, 3)
c.add_interval(90, c.maximal)
c.add_interval(12, 50)
c.add_interval(56, 70)
c.add_point(50)
c.add_point(71)
c.add_point(90)
c.add_point(9)
print(c)  # (-INF; 3) U {9} U (12; 50] U (56; 70) U {71} U [90; +INF)

# This function shifts all points in the set 4 units to the right,
# limiting the range of values to 0–100
# Essentially the same as:
# c += 4
# x = Interval()
# x.add_interval(0, 100)
# c &= x
# Note - it works in the Interval class without adding IntTypePolicy
# However, the implementation written above will not work,
# because the Interval class does not support the += operation.
# It is only available only when applying numeric policies
c = c.custom_transfer(lambda x: x + 4, 0, 100)
print(c)  # (0; 7) U {13} U (16; 54] U (60; 74) U {75} U [94; 100)
```