#include <memory>

class RuntimeError : public std::runtime_error {
    public:
        RuntimeError(const std::string &msg):
               std::runtime_error(msg) {
       }
};


class SStackImpl;
typedef std::shared_ptr<SStackImpl> SStackImplP;

class SStack {
    public:
        SStack();
        SStack(const char*);
        SStack(const SStack &);
        ~SStack();

        void push(const char *s);
        char *pop();
        const char *peek();

        SStack &operator=(const SStack &);
        SStack &operator+=(const SStack &);
        SStack operator+(const SStack &) const;
        char *operator-(char*);
        operator char*() const;

        int length() const;
        int capacity() const;

   private:
        SStackImplP m_impl;
};
