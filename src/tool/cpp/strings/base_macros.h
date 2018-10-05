
#ifdef _DEBUG

#define WINRT_ASSERT _ASSERTE
#define WINRT_VERIFY WINRT_ASSERT
#define WINRT_VERIFY_(result, expression) WINRT_ASSERT(result == expression)

#else

#define WINRT_ASSERT(expression) ((void)0)
#define WINRT_VERIFY(expression) (void)(expression)
#define WINRT_VERIFY_(result, expression) (void)(expression)

#endif

#if defined(_MSC_VER)
#define WINRT_EBO __declspec(empty_bases)
#define WINRT_NOVTABLE __declspec(novtable)
#define WINRT_CALL __stdcall
#define WINRT_NOINLINE  __declspec(noinline)
#define WINRT_FORCEINLINE __forceinline
#else
#define WINRT_EBO
#define WINRT_NOVTABLE
#define WINRT_CALL
#define WINRT_NOINLINE
#define WINRT_FORCEINLINE
#endif

#if defined(_MSC_VER) && _ITERATOR_DEBUG_LEVEL != 0
#define WINRT_CHECKED_ITERATORS
#endif

#define WINRT_SHIM(...) (*(abi_t<__VA_ARGS__>**)&static_cast<__VA_ARGS__ const&>(static_cast<D const&>(*this)))

#ifndef WINRT_EXTERNAL_CATCH_CLAUSE
#define WINRT_EXTERNAL_CATCH_CLAUSE
#endif

#if defined(_MSC_VER)
#ifdef _M_HYBRID
#define WINRT_LINK(function, count) __pragma(comment(linker, "/alternatename:#WINRT_" #function "@" #count "=#" #function "@" #count))
#elif _M_IX86
#define WINRT_LINK(function, count) __pragma(comment(linker, "/alternatename:_WINRT_" #function "@" #count "=_" #function "@" #count))
#else
#define WINRT_LINK(function, count) __pragma(comment(linker, "/alternatename:WINRT_" #function "=" #function))
#endif
#else
#define WINRT_LINK(function, count)
#endif

#if defined _M_ARM
#define WINRT_INTERLOCKED_READ_MEMORY_BARRIER (__dmb(_ARM_BARRIER_ISH));
#elif defined _M_ARM64
#define WINRT_INTERLOCKED_READ_MEMORY_BARRIER (__dmb(_ARM64_BARRIER_ISH));
#endif

#ifdef __IUnknown_INTERFACE_DEFINED__
#define WINRT_WINDOWS_ABI

namespace winrt::impl
{
    using hresult_type = long;
    using ref_count_type = unsigned long;
}

#else

namespace winrt::impl
{
    using hresult_type = int32_t;
    using ref_count_type = uint32_t;
}

#endif

#if defined(_DEBUG) && defined(_MSC_VER) && !defined(__clang__)

namespace winrt::impl
{
    template <typename L, typename T, typename...Args>
    using invoker_t = decltype(std::declval<L>()(std::declval<T>(), std::declval<Args>()...));
}

#define WINRT_WRAP(...) __VA_ARGS__

#define WINRT_ASSERT_DECLARATION(M, R, ...) WINRT_ASSERT_DECLARATION_(M, R, __VA_ARGS__)

#define WINRT_ASSERT_DECLARATION_(M, R, ...) \
{ \
    auto invocation = [](auto&& d, auto&&... params) -> decltype((R)d.M(params...)) {}; \
    static_assert(winrt::impl::is_detected_v<winrt::impl::invoker_t, decltype(invocation), D, __VA_ARGS__>, \
        "\n\n\tC++/WinRT: Could not find method in implementation class:\n\t\t" #R " " #M "(" #__VA_ARGS__ ");\n"); \
}

#else

#define WINRT_ASSERT_DECLARATION(...) ((void)0)

#endif

#if defined(__clang__) 

#define WINRT_WARNING_PUSH \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")

#define WINRT_WARNING_POP \
_Pragma("clang diagnostic pop")

#else

#define WINRT_WARNING_PUSH
#define WINRT_WARNING_POP

#endif