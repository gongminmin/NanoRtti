// Copyright(c) Minmin Gong. All rights reserved.
// Licensed under the MIT License.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace NanoRtti
{
    template <typename T>
    TypeInfo TypeId() noexcept;

    namespace Detail
    {
        template <typename... Bases>
        struct BasesTag
        {
        };

        struct TypeEntry
        {
            const char* sig;
            std::size_t sig_hash;
            std::uint8_t num_bases;
            const TypeEntry** base_entries;
        };

        template <typename T>
        inline const char* N() noexcept
        {
#ifdef __FUNCDNAME__
            const char* sig = __FUNCDNAME__;
#else
            const char* sig = __PRETTY_FUNCTION__;
#endif
            // For each T, this function has a unique signature. This makes it a unique ID for each type.
            return sig;
        }

        constexpr std::size_t CtHash(const char* str, size_t seed = 0) noexcept
        {
            constexpr std::size_t PRIME_NUM = 0x9e3779b9U;
            return (*str == 0) ? seed : CtHash(str + 1, seed ^ (static_cast<std::size_t>(*str) + PRIME_NUM + (seed << 6) + (seed >> 2)));
        }

        template <typename... Bases>
        inline TypeInfo* BaseEntriesTable() noexcept
        {
            static TypeInfo bases[] = {
                NanoRtti::TypeId<Bases>()...,
            };

            return bases;
        }

        template <>
        inline TypeInfo* BaseEntriesTable<>() noexcept
        {
            return nullptr;
        }

        template <typename T, typename... Bases>
        inline TypeInfo TypeId(BasesTag<Bases...>&&) noexcept
        {
            static TypeEntry type_entry{N<T>(), CtHash(N<T>()), sizeof...(Bases), BaseEntriesTable<Bases...>()};
            return &type_entry;
        }

        inline bool IsDynConvertible(TypeInfo from, TypeInfo to)
        {
            assert((from != nullptr) && (to != nullptr));

            if ((from != to) && ((from->sig_hash != to->sig_hash) || (std::strcmp(from->sig, to->sig) != 0)))
            {
                for (std::uint8_t index = 0; index < from->num_bases; ++index)
                {
                    if (IsDynConvertible(from->base_entries[index], to))
                    {
                        return true;
                    }
                }

                return false;
            }

            return true;
        }

        template <typename From, typename To>
        struct IsInterConvertible
        {
            using NoPtrFromType = std::remove_pointer_t<From>;
            using NoCvFromType = std::remove_cv_t<NoPtrFromType>;
            using NoPtrToType = std::remove_pointer_t<To>;
            using NoCvToType = std::remove_cv_t<NoPtrToType>;

            static constexpr bool value = std::is_pointer<To>::value && ((std::is_convertible<NoCvFromType*, NoCvToType*>::value) ||
                                                                            (std::is_convertible<NoCvToType*, NoCvFromType*>::value));
        };
    } // namespace Detail
} // namespace NanoRtti
