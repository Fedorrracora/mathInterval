#ifndef FBP_FORWARD_CONTAINER_H
#define FBP_FORWARD_CONTAINER_H
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
        [[nodiscard]] auto operator<=>(const forward_container &other) const {
            return *link <=> *other.link;
        }
        [[nodiscard]] auto operator<=>(const T &other) const {
            return *link <=> other;
        }
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
}
#endif // FBP_FORWARD_CONTAINER_H