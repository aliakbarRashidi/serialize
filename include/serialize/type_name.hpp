/*
  MIT License

  Copyright (c) 2018 Nicolai Trandafil

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/


#pragma once


// local
#include <serialize/when.hpp>

// std
#include <string>
#include <string_view>


namespace serialize {
namespace detail {


constexpr std::string_view getT(std::string_view p) {
    using namespace std;
#ifdef __clang__
#error "Not supported"
#elif defined(__GNUC__)
    auto start = p.find("T = ") + 4;
    auto const end = p.find(';', start);
    auto const tmp = p.rfind("::", end);
    if (tmp > start) { start = tmp + 2; }
    return string_view(p.data() + start, end - start);
#elif defined(_MSC_VER)
#error "Not supported"
#endif
}


}


template <class T, class = void>
struct UnqualifiedTypeNameImpl : UnqualifiedTypeNameImpl<T, When<true>> {};


template <class T>
struct UnqualifiedTypeNameT {
    constexpr auto operator()() const;
};


/// Get unqualified type name
template <class T>
constexpr UnqualifiedTypeNameT<T> unqualifiedTypeName;


template <class T, bool condition>
struct UnqualifiedTypeNameImpl<T, When<condition>> {
    static constexpr std::string_view apply() {
        return detail::getT(__PRETTY_FUNCTION__);
    }
};


template <>
struct UnqualifiedTypeNameImpl<std::string> {
    static constexpr std::string_view apply() {
        return "string";
    }
};


template <template <class...> class T, class... Args>
struct UnqualifiedTypeNameImpl<T<Args...>> {
    static std::string apply() {
        using namespace std;
        std::string const name{detail::getT(__PRETTY_FUNCTION__)};
        string tmp = ((string(unqualifiedTypeName<Args>()) + ", ") + ...);
        if (!tmp.empty()) { tmp.erase(tmp.end() - 2, tmp.end()); }
        return name + "<" + tmp + ">";
    }
};


template <class T>
constexpr auto UnqualifiedTypeNameT<T>::operator()() const {
    return UnqualifiedTypeNameImpl<T>::apply();
}


/// Get unqualified type name
template <class T>
constexpr std::string_view qualifiedTypeName() {
    using namespace std;

#ifdef __clang__
#error "Not supported"

#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
    auto const start = p.find("T = ") + 4;
    auto const end = p.find(';', start);
    return string_view(p.data() + start, end - start);

#elif defined(_MSC_VER)
#error "Not supported"
#endif
}


}
