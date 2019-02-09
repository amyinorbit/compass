//===--------------------------------------------------------------------------------------------===
// invoke_compat.hpp - Compatibility layer for std::invoke before C++17
// This source is part of the Compass Engine
//
// Created on 2019-02-09 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once


#if __cplusplus >= 201703L

using std::invoke;
template <typename F, typename... ArgTypes>
using invoke_result_t = std::invoke_result_t<F,ArgTypes...>;

#else

template <typename F, typename... ArgTypes>
using invoke_result_t = std::result_of_t<F(ArgTypes...)>;

template<typename Fn, typename... Args,
std::enable_if_t<std::is_member_pointer<std::decay_t<Fn>>{}, int> = 0 >
constexpr decltype(auto) invoke(Fn&& f, Args&&... args)
noexcept(noexcept(std::mem_fn(f)(std::forward<Args>(args)...)))
{
    return std::mem_fn(f)(std::forward<Args>(args)...);
}

template<typename Fn, typename... Args,
std::enable_if_t<!std::is_member_pointer<std::decay_t<Fn>>{}, int> = 0>
constexpr decltype(auto) invoke(Fn&& f, Args&&... args)
noexcept(noexcept(std::forward<Fn>(f)(std::forward<Args>(args)...)))
{
    return std::forward<Fn>(f)(std::forward<Args>(args)...);
}
#endif
