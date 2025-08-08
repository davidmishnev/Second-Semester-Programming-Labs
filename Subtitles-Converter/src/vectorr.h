template <typename T>
class vectorr {
public:
    vectorr() noexcept : data_(nullptr), size_(0), capacity_(0) {}
    vectorr(const vectorr& other);
    ~vectorr() { clear(); }

    vectorr& operator=(const vectorr& other);

    void clear() noexcept;
    void reserve(size_t new_capacity) noexcept;
    void resize(size_t new_size) noexcept;
    void push_back(T& value) noexcept;
    size_t size() const noexcept;
    T& operator[](size_t index) noexcept;

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

template <typename T>
void vectorr<T>::clear() noexcept {
    for (size_t i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    operator delete[](data_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <typename T>
void vectorr<T>::reserve(size_t new_capacity) noexcept {
    T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
    for (size_t i = 0; i < size_; ++i) {
        new (new_data + i) T(data_[i]);
        data_[i].~T();
    }
    operator delete[](data_);
    data_ = new_data;
    capacity_ = new_capacity;
}

template <typename T>
void vectorr<T>::resize(size_t new_size) noexcept {
    if (new_size > capacity_) {
        reserve(new_size);
    }
    for (size_t i = size_; i < new_size; ++i) {
        new (data_ + i) T();
    }
    for (size_t i = new_size; i < size_; ++i) {
        data_[i].~T();
    }
    size_ = new_size;
}

template <typename T>
void vectorr<T>::push_back(T& value) noexcept {
    if (size_ >= capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(value);
    ++size_;
}

template <typename T>
size_t vectorr<T>::size() const noexcept {
    return size_;
}

template <typename T>
T& vectorr<T>::operator[](size_t index) noexcept {
    return data_[index];
}

template <typename T>
vectorr<T>& vectorr<T>::operator=(const vectorr& other) {
    if (this != &other) {
        clear();
        reserve(other.size_);
        for (size_t i = 0; i < other.size_; ++i) {
            new (data_ + i) T(other.data_[i]);
        }
        size_ = other.size_;
    }
    return *this;
}
