#pragma once
#include <initializer_list>
#include <type_traits>

#include <algorithm>
#include <stdexcept>

template< typename T, size_t N, bool IsConst >
class NDArrayBase;

template< typename T, size_t N >
class NDArray;

template< typename T, size_t N >
class NDArrayView;

template< typename T, size_t N >
class NDArrayConstView;

template< typename T, size_t N, bool IsConst >
class NDArrayBase
{
  public:
	using PointerType = std::conditional_t< IsConst, const T*, T* >;
	using ReferenceType = std::conditional_t< IsConst, const T&, T& >;

	PointerType data_ = nullptr;
	size_t strides_[N];
	size_t total_count_ = 0;

	void compute_strides()
	{
		if (N == 0)
		{
			return;
		}
		strides_[N - 1] = 1;
		for (size_t i = N - 1; i > 0; --i)
		{
			strides_[i - 1] = strides_[i] * dimensions_[i];
		}
	}

	size_t compute_total() const
	{
		size_t c = 1;
		for (size_t i = 0; i < N; ++i)
		{
			c *= dimensions_[i];
		}
		return c;
	}

	size_t dimensions_[N];
	// public:
	NDArrayBase() = default;

	NDArrayBase(PointerType ptr, const size_t (&d)[N], const size_t (&s)[N]) : data_(ptr)
	{
		std::copy(d, d + N, dimensions_);
		std::copy(s, s + N, strides_);
		total_count_ = compute_total();
	}

	template< typename It >
	NDArrayBase(It first, It last)
	{
		static_assert(N == 1, "Iterator constructor only for 1D");
		data_ = &*first;
		dimensions_[0] = std::distance(first, last);
		strides_[0] = 1;
		total_count_ = dimensions_[0];
	}

	[[nodiscard]] size_t dim() const { return N; }
	[[nodiscard]] size_t count() const { return N > 0 ? dimensions_[0] : 0; }
	[[nodiscard]] size_t total_count() const { return total_count_; }

	ReferenceType at(std::initializer_list< size_t > indexes) const
	{
		if (!data_)
		{
			throw std::runtime_error("array/view is empty");
		}
		if (indexes.size() != N)
		{
			throw std::out_of_range("wrong rank");
		}
		size_t offset = 0, i = 0;
		for (auto index : indexes)
		{
			if (index >= dimensions_[i])
			{
				throw std::out_of_range("index out of range");
			}
			offset += index * strides_[i++];
		}
		if (offset >= total_count_)
		{
			throw std::out_of_range("computed offset out of range");
		}
		return data_[offset];
	}

	auto operator[](size_t i) const
	{
		if (!data_)
			throw std::runtime_error("array/view is empty");
		if (i >= dimensions_[0])
			throw std::out_of_range("index out of range");

		if constexpr (N == 1)
		{
			if constexpr (IsConst)
			{
				return NDArrayConstView< T, 0 >(data_ + i);
			}
			else
			{
				return NDArrayView< T, 0 >(data_ + i);
			}
		}
		else
		{
			using SubViewType = std::conditional_t< IsConst, NDArrayConstView< T, N - 1 >, NDArrayView< T, N - 1 > >;
			SubViewType sub;
			std::copy(dimensions_ + 1, dimensions_ + N, sub.dimensions_);
			std::copy(strides_ + 1, strides_ + N, sub.strides_);
			sub.total_count_ = total_count_ / dimensions_[0];
			sub.data_ = data_ + i * strides_[0];
			return sub;
		}
	}
};

template< typename T, bool IsConst >
class NDArrayBase< T, 0, IsConst >
{
  public:
	using PointerType = std::conditional_t< IsConst, const T*, T* >;
	using ReferenceType = std::conditional_t< IsConst, const T&, T& >;

	PointerType data_;

	// public:
	NDArrayBase() : data_(nullptr) {}
	explicit NDArrayBase(PointerType ptr) : data_(ptr) {}

	explicit operator ReferenceType() const { return *data_; }
	ReferenceType get() const { return *data_; }
	bool operator==(const T& other) const { return *data_ == other; }
	bool operator!=(const T& other) const { return *this != other; }

	PointerType operator&() const { return data_; }

	template< typename U = T, typename = std::enable_if_t< !IsConst > >
	NDArrayBase& operator=(const U& value)
	{
		if (!data_)
		{
			throw std::runtime_error("view is empty");
		}
		*data_ = value;
		return *this;
	}

	template< typename U = T, typename = std::enable_if_t< !IsConst > >
	NDArrayBase& operator=(U&& value)
	{
		if (!data_)
		{
			throw std::runtime_error("view is empty");
		}
		*data_ = std::forward< U >(value);
		return *this;
	}
};

template< typename T, size_t N >
class NDArray : public NDArrayBase< T, N, false >
{
  public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using iterator = NDArrayView< T, N >;
	using const_iterator = NDArrayConstView< T, N >;

	NDArray() = delete;
	~NDArray() { delete[] this->data_; }

	NDArray(const NDArray& other) : NDArrayBase< T, N, false >()
	{
		this->total_count_ = other.total_count_;
		std::copy(other.dimensions_, other.dimensions_ + N, this->dimensions_);
		std::copy(other.strides_, other.strides_ + N, this->strides_);
		this->data_ = new T[this->total_count_];
		std::copy(other.data_, other.data_ + this->total_count_, this->data_);
	}

	NDArray& operator=(const NDArray& other)
	{
		if (this == &other)
		{
			return *this;
		}
		T* new_data = new T[other.total_count_];
		std::copy(other.data_, other.data_ + other.total_count_, new_data);
		delete[] this->data_;
		this->data_ = new_data;
		this->total_count_ = other.total_count_;
		std::copy(other.dimensions_, other.dimensions_ + N, this->dimensions_);
		std::copy(other.strides_, other.strides_ + N, this->strides_);
		return *this;
	}

	NDArray(NDArray&& other) : NDArrayBase< T, N, false >()
	{
		this->data_ = other.data_;
		this->total_count_ = other.total_count_;
		std::copy(other.dimensions_, other.dimensions_ + N, this->dimensions_);
		std::copy(other.strides_, other.strides_ + N, this->strides_);
		other.data_ = nullptr;
		other.total_count_ = 0;
	}

	NDArray& operator=(NDArray&& other)
	{
		if (this == &other)
		{
			return *this;
		}
		delete[] this->data_;
		this->data_ = other.data_;
		this->total_count_ = other.total_count_;
		std::copy(other.dimensions_, other.dimensions_ + N, this->dimensions_);
		std::copy(other.strides_, other.strides_ + N, this->strides_);
		other.data_ = nullptr;
		other.total_count_ = 0;
		return *this;
	}

	template< typename... Sizes >
	explicit NDArray(Sizes... sizes) : NDArrayBase< T, N, false >()
	{
		static_assert(sizeof...(Sizes) == N, "Must provide exactly N dimensions");
		size_t tmp[N] = { static_cast< size_t >(sizes)... };
		std::copy(tmp, tmp + N, this->dimensions_);
		this->compute_strides();
		this->total_count_ = this->compute_total();
		this->data_ = new T[this->total_count_];
	}

	template< typename... Sizes >
	NDArray(const T& fill_value, Sizes... sizes) : NDArrayBase< T, N, false >()
	{
		static_assert(sizeof...(Sizes) == N, "Must provide exactly N dimensions");
		size_t tmp[N] = { static_cast< size_t >(sizes)... };
		for (size_t i = 0; i < N; ++i)
		{
			if (tmp[i] == 0)
			{
				throw std::invalid_argument("dimension size cannot be zero");
			}
		}
		std::copy(tmp, tmp + N, this->dimensions_);
		this->total_count_ = this->compute_total();
		this->data_ = new T[this->total_count_];
		this->compute_strides();
		std::fill(this->data_, this->data_ + this->total_count_, fill_value);
	}

	NDArray(std::initializer_list< typename std::conditional< N == 1, T, std::initializer_list< T > >::type > list) :
		NDArrayBase< T, N, false >()
	{
		static_assert(N == 1 || N == 2, "Initializer_list supports N=1 or N=2");
		if constexpr (N == 1)
		{
			this->dimensions_[0] = list.size();
			this->compute_strides();
			this->total_count_ = this->dimensions_[0];
			this->data_ = new T[this->total_count_];
			size_t i = 0;
			for (auto& v : list)
				this->data_[i++] = v;
		}
		else
		{
			this->dimensions_[0] = list.size();
			this->dimensions_[1] = list.begin()->size();
			this->compute_strides();
			this->total_count_ = this->compute_total();
			this->data_ = new T[this->total_count_];
			size_t i = 0;
			for (auto& row : list)
			{
				if (row.size() != this->dimensions_[1])
					throw std::invalid_argument("Rows must have same length");
				for (auto& v : row)
					this->data_[i++] = v;
			}
		}
	}

	template< typename It >
	NDArray(It first, It last) : NDArrayBase< T, N, false >(first, last)
	{
		static_assert(N == 1, "Iterator constructor only for N=1");
		this->data_ = new T[this->total_count_];
		std::copy(first, last, this->data_);
	}

	NDArray(const NDArrayView< T, N >& view) : NDArrayBase< T, N, false >()
	{
		std::copy(view.dimensions_, view.dimensions_ + N, this->dimensions_);
		std::copy(view.strides_, view.strides_ + N, this->strides_);
		this->total_count_ = view.total_count_;
		this->data_ = new T[this->total_count_];
		std::copy(view.data_, view.data_ + this->total_count_, this->data_);
	}

	NDArray(const NDArrayConstView< T, N >& view) : NDArrayBase< T, N, false >()
	{
		std::copy(view.dimensions_, view.dimensions_ + N, this->dimensions_);
		std::copy(view.strides_, view.strides_ + N, this->strides_);
		this->total_count_ = view.total_count_;
		this->data_ = new T[this->total_count_];
		std::copy(view.data_, view.data_ + this->total_count_, this->data_);
	}

	template< size_t M, typename Container >
	NDArrayView< T, M > reshape(const Container& new_dims)
	{
		static_assert(M >= 1, "reshape requires at least one dimension");
		if (new_dims.size() != M)
		{
			throw std::invalid_argument("reshape: number of new dimensions does not match M");
		}
		size_t prod = 1;
		for (auto d : new_dims)
		{
			prod *= d;
		}
		if (prod != this->total_count_)
		{
			throw std::invalid_argument("reshape: total number of elements does not match");
		}
		NDArrayView< T, M > v;
		for (size_t i = 0; i < M; ++i)
		{
			v.dimensions_[i] = new_dims[i];
		}
		v.strides_[M - 1] = 1;
		for (size_t i = M - 1; i > 0; --i)
		{
			v.strides_[i - 1] = v.strides_[i] * v.dimensions_[i];
		}
		v.total_count_ = this->total_count_;
		v.data_ = this->data_;
		return v;
	}

	bool is_equal(const NDArray& rhs) const
	{
		if (this->total_count_ != rhs.total_count_)
		{
			return false;
		}
		for (size_t i = 0; i < N; ++i)
		{
			if (this->dimensions_[i] != rhs.dimensions_[i])
			{
				return false;
			}
		}
		for (size_t i = 0; i < this->total_count_; ++i)
		{
			if (this->data_[i] != rhs.data_[i])
			{
				return false;
			}
		}
		return true;
	}

	void swap(NDArray& other)
	{
		if (this->total_count_ != other.total_count_)
		{
			throw std::invalid_argument("swap size mismatch");
		}
		std::swap(this->data_, other.data_);
		std::swap_ranges(this->dimensions_, this->dimensions_ + N, other.dimensions_);
		std::swap_ranges(this->strides_, this->strides_ + N, other.strides_);
		std::swap(this->total_count_, other.total_count_);
	}

	iterator begin() { return iterator(this->data_, this->dimensions_, this->strides_); }
	iterator end() { return iterator(this->data_ + this->total_count_, this->dimensions_, this->strides_); }
	const_iterator begin() const { return const_iterator(this->data_, this->dimensions_, this->strides_); }
	const_iterator end() const
	{
		return const_iterator(this->data_ + this->total_count_, this->dimensions_, this->strides_);
	}
	const_iterator cbegin() const { return begin(); }
	const_iterator cend() const { return end(); }
};

template< typename T, size_t N >
class NDArrayView : public NDArrayBase< T, N, false >
{
  public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using iterator = NDArrayView;
	using const_iterator = NDArrayConstView< T, N >;

	NDArrayView() = default;
	NDArrayView(T* ptr, const size_t (&d)[N], const size_t (&s)[N]) : NDArrayBase< T, N, false >(ptr, d, s) {}
	template< typename It >
	NDArrayView(It first, It last) : NDArrayBase< T, N, false >(first, last)
	{
	}

	iterator begin() { return *this; }
	iterator end() { return NDArrayView< T, N >(this->data_ + this->total_count_, this->dimensions_, this->strides_); }

	T& operator*() const { return *this->data_; }
	NDArrayView& operator++()
	{
		this->data_ += 1;
		return *this;
	}
	bool operator==(const NDArrayView& o) const { return this->data_ == o.data_; }
	bool operator!=(const NDArrayView& o) const { return !(*this == o); }

	NDArrayView& operator=(const T& value)
	{
		if (!this->data_)
			throw std::runtime_error("view is empty");
		*this->data_ = value;
		return *this;
	}

	NDArrayView& operator=(T&& value)
	{
		if (!this->data_)
			throw std::runtime_error("view is empty");
		*this->data_ = std::move(value);
		return *this;
	}
};

template< typename T, size_t N >
class NDArrayConstView : public NDArrayBase< T, N, true >
{
  public:
	using value_type = T;
	using reference = const T&;
	using iterator = NDArrayConstView;
	using const_iterator = NDArrayConstView;

	NDArrayConstView() = default;
	NDArrayConstView(const T* ptr, const size_t (&d)[N], const size_t (&s)[N]) : NDArrayBase< T, N, true >(ptr, d, s) {}
	template< typename It >
	NDArrayConstView(It first, It last) : NDArrayBase< T, N, true >(first, last)
	{
	}
	NDArrayConstView(const NDArrayView< T, N >& v) : NDArrayBase< T, N, true >(v.data_, v.dimensions_, v.strides_) {}

	const_iterator begin() const { return *this; }
	const_iterator end() const
	{
		return NDArrayConstView< T, N >(this->data_ + this->total_count_, this->dimensions_, this->strides_);
	}

	const T& operator*() const { return *this->data_; }
	NDArrayConstView& operator++()
	{
		this->data_ += 1;
		return *this;
	}
	bool operator==(const NDArrayConstView& o) const { return this->data_ == o.data_; }
	bool operator!=(const NDArrayConstView& o) const { return !(*this == o); }
};
