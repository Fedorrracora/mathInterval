#ifndef FORWARD_CONTAINER_H
#define FORWARD_CONTAINER_H
#include <algorithm>
#include <optional>
namespace fbp {
    template <typename T>
    struct forward_container {
        explicit forward_container(const T &el) : val(nullptr), link(&el) {}
        explicit forward_container(T &&el) : val(new T(std::move(el))), link(val) {}
        forward_container(const forward_container &other) = delete;
        forward_container &operator=(const forward_container &other) = delete;
        forward_container(forward_container &&other) noexcept : val(other.val), link(other.link) {
            other.val = nullptr;
            other.link = nullptr;
        }
        forward_container &operator=(forward_container &&other) noexcept {
            if (this != &other) {
                delete val;
                val = other.val;
                link = other.link;
                other.val = nullptr;
                other.link = nullptr;
            }
            return *this;
        }
        [[nodiscard]] bool operator<(const forward_container &other) const {
            return *link < *other.link;
        }
        [[nodiscard]] bool operator==(const forward_container &other) const {
            return *link == *other.link;
        }
        [[nodiscard]] bool operator<=>(const forward_container &other) const = default;
        const T &call() const { return *link; }
        [[nodiscard]] T get() && {
            if (val) {
                T tmp = std::move(*val);
                delete val;
                val = nullptr;
                return tmp;
            }
            return *link;
        }
        ~forward_container() {
            delete val;
        }
    private:
        T *val;
        const T *link;
    };
    // template <typename U>
    // class forward_container {
    //     using T = std::remove_reference_t<U>;
    // public:
    //     explicit forward_container(U&& value) {
    //         if constexpr (std::is_lvalue_reference_v<U>) {
    //             ptr_ = &value;
    //         } else {
    //             obj_ = std::move(value);
    //             ptr_ = &*obj_;
    //         }
    //     }
    //
    //     forward_container(const forward_container&) = delete;
    //     forward_container& operator=(const forward_container&) = delete;
    //
    //     forward_container(forward_container&& other) noexcept
    //         : obj_(std::move(other.obj_))
    //     {
    //         if (obj_) {
    //             ptr_ = &*obj_;
    //         } else {
    //             ptr_ = other.ptr_;
    //         }
    //     }
    //
    //     forward_container& operator=(forward_container&&) noexcept = default;
    //
    //     decltype(auto) get() && {
    //         if constexpr (std::is_lvalue_reference_v<U>) {
    //             return *ptr_;            // T&
    //         } else {
    //             return std::move(*ptr_); // T&&
    //         }
    //     }
    //
    //     const T& call() const {
    //         return *ptr_;
    //     }
    //
    // private:
    //     std::optional<T> obj_;
    //     T* ptr_;
    // };
}
#endif // FORWARD_CONTAINER_H