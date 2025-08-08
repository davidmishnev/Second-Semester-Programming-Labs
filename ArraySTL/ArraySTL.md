# Lab Assignment №4 – STL-Compatible Container

## Tools and Requirements

* **Language**: C++
* **Standard**: C++23 (compiled on server).
* **Testing**: All implemented classes and functions must be covered with unit tests.

---

## Task

Implement a **template class** `NDArray` representing an **STL-compatible container** for multi-dimensional arrays.
Template parameters:

* `T` – type of stored elements
* `N` – number of dimensions

The container must meet the **ContiguousContainer** requirements.
**Restrictions**:

* **No allocators** (`std::allocator`) allowed.
* **No use of STL** or other standard C++ classes with **non-trivial destructors** (anything other than the default destructor doing nothing).
* All constructors/operators from the **rule of five** must be implemented:

  * Copy constructor
  * Copy assignment
  * Move constructor
  * Move assignment
  * Destructor
* Error handling (e.g., failed memory allocation) must use **C++ exceptions**.
* Must provide **exception safety guarantees** for correct operation in the presence of exceptions.

---

## Container Description

The container is designed to work with **multi-dimensional arrays** (e.g., `N=2` → 2D array).
Elements must be stored in **contiguous memory**, with adjacent elements in the last dimension also being adjacent in memory.

**Key requirements**:

* Element memory addresses remain **stable** during the lifetime of the data (no invalidation of pointers/iterators/views except on destruction).
* Access pattern should be similar to C multi-dimensional arrays with multiple `[]`.

---

## Constructors

The `NDArray` class must support:

1. **Rule of five** – all special members implemented.
2. **Initializer list** – must work for code like:

   ```cpp
   NDArray<int, 2> Matrix{{1, 2}, {3, 4}};
   ```
3. **From sizes** – construct by passing the size of each dimension.
4. **From sizes + fill value** – construct with given sizes and fill all elements with a value.
5. **From NDArrayView or NDArrayConstView**.
6. **From two iterators** – given iterators to a contiguous block of memory representing an array of the correct total size.

---

## Member Functions / Operators

| Function / Operator                                                                                                               | Description                                                                                                       |
| --------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| `size_type count() const;`                                                                                                        | Number of elements in the outermost (first) dimension.                                                            |
| `size_type total_count() const;`                                                                                                  | Total number of elements in all dimensions.                                                                       |
| `size_type dim() const;`                                                                                                          | Number of dimensions (`N`).                                                                                       |
| `view operator[](size_type i) const;`                                                                                             | Returns a `view` into the `i`-th element along the outermost dimension.                                           |
| `template <typename SizeList> view at(initializer_list<size_type> indexes) const;`                                                | Same as `operator[]`, but with bounds checking like `vector::at()`.                                               |
| `bool is_equal(const NDArray& rhs) const;`                                                                                        | If `T` is EqualityComparable, compares dimensions and elements.                                                   |
| `view begin(); view end(); const_view begin() const; const_view end() const; const_view cbegin() const; const_view cend() const;` | Iterators over the outermost dimension, similar to `std::array`.                                                  |
| `template <typename SizeList> view reshape(const SizeList& sizes);`                                                               | Returns a `view` over the same memory interpreted with different dimensions. `total_count` must remain unchanged. |
| `void swap(NDArray &other);`                                                                                                      | Swap contents with another container. Dimensions must match.                                                      |

---

## `NDArrayView` Description

In addition to the main container, implement a **`NDArrayView`** class:

* Satisfies **View** and **ContiguousIterator** concepts.
* Has the **same interface** as `NDArray` but **does not own memory**.
* No code duplication between `NDArray`, `NDArrayView`, and `NDArrayConstView`.

---

### `NDArrayView` Constructors

* Rule of five (copy/move constructor, destructor).
* From `NDArray`.
* From two iterators (start inclusive, end exclusive) for correct dimensional size.

---

### Operators for `NDArrayView` Iterators

Must support (all **O(1)** complexity):

* `++`, `+`, `+=`
* `==`, `!=`, `<`, `<=`, `>`, `>=`
* `--`, `-`, `-=`
* `[]`
* `*` (dereference)
* `->` (member access)

**Iterator invalidation**:

* Iterators are invalidated only if the content is modified (e.g., `swap`, `operator=`).

---

## Testing

Tests must verify:

* Functional correctness of all implemented methods.
* Compliance with stated requirements and logical constraints.
* Correctness of iterators, views, exception safety, and dimensional consistency.
