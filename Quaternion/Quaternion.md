# Quaternion Class – Lab Assignment

## Tools and Requirements

* **Language**: C++
* **Standard**: C++23 (compiled on server)
* **Testing**: The implemented class must be covered with tests.

---

## Task

Implement a quaternion class `Quat` (quaternion components stored as `float`).
The implemented class must support the following **constructors**, **operators**, and **methods**.

---

## Constructors

1. **Default constructor** – initializes all components to `0`.
2. **From 4 values**:

   ```cpp
   Quat(float a, float b, float c, float d);
   ```

   Represents the quaternion:

   $$
   q = a + b i + c j + d k
   $$
3. **Rotation quaternion constructor**:

   ```cpp
   Quat(float T, bool radians, vector3_t axis);
   ```

   * `T` – rotation angle.
   * `radians` – if `true`, the angle `T` is given in radians; otherwise in degrees.
   * `axis` – a 3D vector (`vector3_t`) with fields `x`, `y`, `z` representing the rotation axis.

---

## Operators

| Operator                    | Arguments  | Description                       |
| --------------------------- | ---------- | --------------------------------- |
| `+`, `+=`                   | quaternion | Element-wise addition             |
| `-`, `-=`                   | quaternion | Element-wise subtraction          |
| `*`, `*=`                   | quaternion | Quaternion multiplication         |
| `~`                         | —          | Conjugate                         |
| `==`, `!=`                  | quaternion | Exact equality/inequality check   |
| `explicit operator float()` | —          | Computes the quaternion magnitude |

**Additional Notes**:

* Must also support multiplication by a **scalar** and by a **vector** (`vector3_t`).
  This can be implemented via operator overloading or additional constructors.

---

## Methods

| Method              | Arguments | Description                                                                                                 |
| ------------------- | --------- | ----------------------------------------------------------------------------------------------------------- |
| `data()`            | —         | Returns a pointer to the 4 quaternion components in the order **b, c, d, a** (equivalent to **x, y, z, w**) |
| `rotation_matrix()` | —         | Returns the **rotation matrix** corresponding to the quaternion                                             |
| `matrix()`          | —         | Returns the **real matrix representation** of the quaternion                                                |

**Note**: Matrices must be stored in **row-major order**.

---

## Additional Requirements

* All quaternion components must be stored as `float`.
* The `vector3_t` structure consists of three `float` fields:

  ```cpp
  struct vector3_t {
      float x, y, z;
  };
  ```

---

This specification defines both the functional and interface requirements for the `Quat` class implementation and testing.

---

If you want, I can also prepare a **UML class diagram** and **method signatures** for `Quat` so it’s ready for coding. That would make the implementation straightforward.
Here’s your lab work description reformatted into a clear **Markdown** technical specification in English:

---

# Quaternion Class – Lab Assignment

## Tools and Requirements

* **Language**: C++
* **Standard**: C++23 (compiled on server)
* **Testing**: The implemented class must be covered with tests.

---

## Task

Implement a quaternion class `Quat` (quaternion components stored as `float`).
The implemented class must support the following **constructors**, **operators**, and **methods**.

---

## Constructors

1. **Default constructor** – initializes all components to `0`.
2. **From 4 values**:

   ```cpp
   Quat(float a, float b, float c, float d);
   ```

   Represents the quaternion:

   $$
   q = a + b i + c j + d k
   $$
3. **Rotation quaternion constructor**:

   ```cpp
   Quat(float T, bool radians, vector3_t axis);
   ```

   * `T` – rotation angle.
   * `radians` – if `true`, the angle `T` is given in radians; otherwise in degrees.
   * `axis` – a 3D vector (`vector3_t`) with fields `x`, `y`, `z` representing the rotation axis.

---

## Operators

| Operator                    | Arguments  | Description                       |
| --------------------------- | ---------- | --------------------------------- |
| `+`, `+=`                   | quaternion | Element-wise addition             |
| `-`, `-=`                   | quaternion | Element-wise subtraction          |
| `*`, `*=`                   | quaternion | Quaternion multiplication         |
| `~`                         | —          | Conjugate                         |
| `==`, `!=`                  | quaternion | Exact equality/inequality check   |
| `explicit operator float()` | —          | Computes the quaternion magnitude |

**Additional Notes**:

* Must also support multiplication by a **scalar** and by a **vector** (`vector3_t`).
  This can be implemented via operator overloading or additional constructors.

---

## Methods

| Method              | Arguments | Description                                                                                                 |
| ------------------- | --------- | ----------------------------------------------------------------------------------------------------------- |
| `data()`            | —         | Returns a pointer to the 4 quaternion components in the order **b, c, d, a** (equivalent to **x, y, z, w**) |
| `rotation_matrix()` | —         | Returns the **rotation matrix** corresponding to the quaternion                                             |
| `matrix()`          | —         | Returns the **real matrix representation** of the quaternion                                                |

**Note**: Matrices must be stored in **row-major order**.
