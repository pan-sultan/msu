#include "sstack.h"
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <cctype>

#define throw_error(msg) throw RuntimeError(std::string(msg) + " at " + __FILE__ + ":" + std::to_string(__LINE__))

namespace {
bool isEqual(const char *s1, const char *s2, bool ignoreSpace) {
    if(!s1 || !s2) {
        return false;
    }

    while(*s1 && *s2) {
        if(ignoreSpace) {
            while(isspace(*s1))++s1;
            while(isspace(*s2))++s2;
        }

        if(*s1 != *s2) {
            return false;
        }

        if(ignoreSpace) {
            if(*s1) {
                ++s1;
            }
            if(*s2) {
                ++s2;
            }
        } else {
            ++s1;
            ++s2;
        }
    }

    return true;
}

char *mystrdup(const char *s) {
    const size_t len = strlen(s);
    char *newstr = new char[len+1];
    std::copy(s, s+len, newstr);
    newstr[len] = 0;
    return newstr;
}

}

class SStackImpl {
    public:
        size_t m_index;
        size_t m_length;
        char** m_storage;

        SStackImpl(): m_index(0), m_length(0), m_storage(nullptr){
            realloc(1);
        }

        SStackImpl(const SStackImpl &impl): m_index(0), m_length(0), m_storage(nullptr) {
            *this = impl;
        }

        SStackImpl &operator=(const SStackImpl &impl) {
            clear();
            realloc(impl.m_length);
            copyAllStrings(impl);
            m_index = impl.m_index;

            return *this;
        }

        void push(const char *s) {
            if (m_index == m_length) {
                const size_t newlength = m_length == 0 ? 1 : m_length * 2;
                realloc(newlength);
            }

            m_storage[m_index] = mystrdup(s);
            ++m_index;
        }

        char *pop() {
            if (m_index == 0) {
                throw_error("stack empty");
            }

            --m_index;
            return m_storage[m_index];
        }

        const char *peek() const {
            if (m_index == 0) {
                throw_error("stack empty");
            }

            return m_storage[m_index-1];
        }

        void realloc(size_t newlen) {
            char **newstorage = new char*[newlen];
            std::copy(m_storage, m_storage + std::min(m_index, newlen), newstorage);

            delete []m_storage;
            m_storage = newstorage;
            m_length = newlen;
        }

        void copyAllStrings(const SStackImpl &impl) {
            for(size_t i = 0; i < impl.m_index; ++i) {
               m_storage[i] = mystrdup(impl.m_storage[i]); 
            }
        }

       void clear() {
           for(size_t i = 0; i < m_index; i++) {
               delete []m_storage[i];
           }
           delete []m_storage;
           m_storage = nullptr;
           m_index = 0;
           m_length = 0;
       }

       ~SStackImpl() {
           clear();
       }
};


SStack::SStack(): m_impl(new SStackImpl) {
}

SStack::SStack(const char *s): m_impl(new SStackImpl) {
        m_impl->push(s);
}

SStack::SStack(const SStack &stack):
    m_impl(new SStackImpl(*stack.m_impl)) {
}

SStack::~SStack(){
}

void SStack::push(const char *s) {
    m_impl->push(s);
}

char *SStack::pop() {
    return m_impl->pop();
}

const char *SStack::peek() const {
    return m_impl->peek();
}

SStack &SStack::operator=(const SStack &stack) {
    *m_impl = *stack.m_impl;

    return *this;
}

SStack &SStack::operator+=(const SStack &stack) {
    for(size_t i = 0; i < stack.length(); ++i) {
        this->push(stack.m_impl->m_storage[i]);
    }

    return *this;
}

SStack SStack::operator+(const SStack &stack) const {
    SStack newstack(*this);
    for(size_t i = 0; i < stack.length(); ++i) {
        newstack.push(stack.m_impl->m_storage[i]);
    }
    
    return newstack;
}

char *SStack::operator-(char *s) {
    const char *tmp = pop();
    const size_t len = strlen(tmp);
    std::copy(tmp, tmp + len, s);
    s[len] = 0;
    delete []tmp;

    return s;
}

size_t SStack::length() const {
    return m_impl->m_index;
}

size_t SStack::capacity() const {
    return m_impl->m_length;
}

SStack::operator char*() const {
    size_t maxlen = 1;

    for(size_t i = 0; i < m_impl->m_index; ++i) {
        maxlen += strlen(this->m_impl->m_storage[i]) + 1;
    }
    maxlen++;

    char *res = new char[maxlen];
    char *tmp = res;

    for(size_t i = 0; i < m_impl->m_index; ++i) {
        const char *s = this->m_impl->m_storage[i];
        const size_t len = strlen(s);
        std::copy(s, s+len, tmp);
        tmp = tmp + len;
        tmp[0] = '\n';
        ++tmp;
    }
    tmp[0] = 0;

    return res;
}

bool SStack::contains(const char *s, bool ignoreSpaces) const {
    for(size_t i = 0; i < m_impl->m_index; ++i) {
        if(isEqual(m_impl->m_storage[i], s, ignoreSpaces)) {
            return true;
        }
    }

    return false;
}

void SStack::write(std::ostream& os) const {
    char *s = (char*)(*this);
    os << s;
    delete []s;
}
