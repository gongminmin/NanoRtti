// Copyright(c) Minmin Gong. All rights reserved.
// Licensed under the MIT License.

#include <type_traits>

namespace NanoRtti
{
    namespace Detail
    {
        struct TypeEntry;
    }

    using TypeInfo = const Detail::TypeEntry*;
} // namespace NanoRtti

#include "NanoRtti/Detail/Detail.hpp"

namespace NanoRtti
{
    template <typename T>
    inline TypeInfo TypeId() noexcept
    {
        using NoRefType = std::remove_reference_t<T>;
        using NoCvType = std::remove_cv_t<NoRefType>;
        using TagType = typename NoCvType::NanoRttiTagType;
        return Detail::TypeId<NoCvType>(TagType());
    }

    template <class To, class From, std::enable_if_t<Detail::IsInterConvertible<From, To>::value, bool> = true>
    inline To DynCast(From* value)
    {
        if (value == nullptr)
        {
            return nullptr;
        }

        using NoPtrToType = std::remove_pointer_t<To>;
        using NoCvToType = std::remove_cv_t<NoPtrToType>;

        const TypeInfo from = value->NanoRttiTypeId();
        const TypeInfo to = TypeId<NoCvToType>();
        return Detail::IsDynConvertible(from, to) ? static_cast<To>(value) : nullptr;
    }

    template <class To, class From, std::enable_if_t<!Detail::IsInterConvertible<From, To>::value, bool> = true>
    inline To DynCast(From*)
    {
        return nullptr;
    }
} // namespace NanoRtti

#if defined(__clang__)
    #define IGNORE_MISSING_OVERRIDE_BEGIN \
        _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Winconsistent-missing-override\"")

    #define IGNORE_MISSING_OVERRIDE_END _Pragma("clang diagnostic pop")
#else
    #define IGNORE_MISSING_OVERRIDE_BEGIN
    #define IGNORE_MISSING_OVERRIDE_END
#endif

#define NANO_RTTI_REGISTER_RUNTIME_CLASS(...)                        \
    using NanoRttiTagType = NanoRtti::Detail::BasesTag<__VA_ARGS__>; \
    IGNORE_MISSING_OVERRIDE_BEGIN                                    \
    virtual NanoRtti::TypeInfo NanoRttiTypeId() const noexcept       \
    {                                                                \
        return NanoRtti::TypeId<decltype(*this)>();                  \
    }                                                                \
    IGNORE_MISSING_OVERRIDE_END
