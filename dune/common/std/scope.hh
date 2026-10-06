// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#ifndef DUNE_COMMON_STD_SCOPE_HH
#define DUNE_COMMON_STD_SCOPE_HH

#include <dune/common/std/no_unique_address.hh>

#include <exception>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>

namespace Dune::Std
{

  namespace Impl
  {
    enum class ScopeHandlerType
    {
      Fail,
      Success,
      Exit
    };

    //!
    /**
     * \brief A scope guard to execute functor on scope at exit, fail or success
     * \ingroup CxxUtilities
     * \details The class template scope_success is a general-purpose scope guard
     * intended to call its exit function when a scope is normally exited.
     *
     * Part of the Version 3 of the C++ Extensions for Library Fundamentals
     */
    template <class EF, ScopeHandlerType type>
    class ScopeHandler
    {
      static_assert((type == ScopeHandlerType::Fail) || (type == ScopeHandlerType::Success) || (type == ScopeHandlerType::Exit));

      template <class Fn>
      static constexpr bool Constructible = not std::is_base_of_v<ScopeHandler, std::remove_cv_t<std::remove_reference_t<Fn>>> and std::is_constructible_v<EF, Fn>;

      template <class Fn>
      static constexpr bool ForwardConstructor = not std::is_lvalue_reference_v<Fn> and std::is_nothrow_constructible_v<EF, Fn>;

      template <class Fn>
      static constexpr bool Movable = std::is_base_of_v<ScopeHandler, std::remove_cv_t<std::remove_reference_t<Fn>>> and (std::is_nothrow_move_constructible_v<EF> or std::is_copy_constructible_v<EF>);

      EF _fn;
      DUNE_NO_UNIQUE_ADDRESS std::conditional_t<type == ScopeHandlerType::Exit, bool, int> _count;

    public:
      template <class Fn, std::enable_if_t<Constructible<Fn> and ForwardConstructor<Fn>, int> = 0>
      [[nodiscard]] explicit ScopeHandler(Fn &&fn) noexcept(std::is_nothrow_constructible_v<EF, Fn>)
          : _fn{std::forward<Fn>(fn)}
      {
        _count = (type == ScopeHandlerType::Exit) ? 1 : std::uncaught_exceptions();
      }

      // construct from a function that may throw on construction (invoke function in case of failure)
      template <class Fn, std::enable_if_t<Constructible<Fn> and not ForwardConstructor<Fn>, int> = 0>
      [[nodiscard]] explicit ScopeHandler(Fn &&fn) noexcept(std::is_nothrow_constructible_v<EF, Fn &>)
      try : _fn{fn}
      {
        _count = (type == ScopeHandlerType::Exit) ? 1 : std::uncaught_exceptions();
      }
      catch (...)
      {
        // success type does not require to call the exit function if constructor fails
        if constexpr (type != ScopeHandlerType::Success)
          std::invoke(fn);
      }

      template <class SH = ScopeHandler, std::enable_if_t<Movable<SH> and std::is_nothrow_move_constructible_v<EF>, int> = 0>
      [[nodiscard]] ScopeHandler(SH &&other) noexcept(std::is_nothrow_move_constructible_v<EF> || std::is_nothrow_copy_constructible_v<EF>)
          : _fn{std::forward<EF>(other._fn)}, _count{other._count}
      {
        other.release();
      }

      template <class SH = ScopeHandler, std::enable_if_t<Movable<SH> and not std::is_nothrow_move_constructible_v<EF>, int> = 0>
      [[nodiscard]] ScopeHandler(SH &&other) noexcept(std::is_nothrow_copy_constructible_v<EF>)
          : _fn{other._fn}, _count{other._count}
      {
        other.release();
      }

      void release() noexcept
      {
        if (type == ScopeHandlerType::Exit)
          _count = false;
        else if (type == ScopeHandlerType::Fail)
          _count = std::numeric_limits<int>::max();
        else if (type == ScopeHandlerType::Success)
          _count = std::numeric_limits<int>::min();
      };

      ScopeHandler &operator=(ScopeHandler) = delete;

      ~ScopeHandler() noexcept
      {
        bool do_invoke = false;
        if constexpr (type == ScopeHandlerType::Exit)
        {
          do_invoke = _count;
        }
        else
        {
          auto new_count = std::uncaught_exceptions();
          do_invoke = ((type == ScopeHandlerType::Fail) and (new_count > _count)) or ((type == ScopeHandlerType::Success) and (new_count <= _count));
        }
        if (do_invoke)
        {
          std::invoke(_fn);
          release();
        }
      }
    };
  }

  template <class Fn>
  struct scope_exit : public Impl::ScopeHandler<Fn, Impl::ScopeHandlerType::Exit>
  {
    using Impl::ScopeHandler<Fn, Impl::ScopeHandlerType::Exit>::ScopeHandler;
  };

  // additional deduction guide
  template <class Fn>
  scope_exit(Fn fn) -> scope_exit<Fn>;

  template <class Fn>
  struct scope_fail : public Impl::ScopeHandler<Fn, Impl::ScopeHandlerType::Fail>
  {
    using Impl::ScopeHandler<Fn, Impl::ScopeHandlerType::Fail>::ScopeHandler;
  };

  // additional deduction guide
  template <class Fn>
  scope_fail(Fn fn) -> scope_fail<Fn>;

  template <class Fn>
  struct scope_success : public Impl::ScopeHandler<Fn, Impl::ScopeHandlerType::Success>
  {
    using Impl::ScopeHandler<Fn, Impl::ScopeHandlerType::Success>::ScopeHandler;
  };

  // additional deduction guide
  template <class Fn>
  scope_success(Fn fn) -> scope_success<Fn>;

} // namespace Dune::Std

#endif // DUNE_COMMON_STD_SCOPE_HH
