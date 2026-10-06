// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#include <dune/common/std/scope.hh>
#include <dune/common/classname.hh>
#include <dune/common/test/testsuite.hh>

static int status = 0;
void f0() {++status;}
void f1() noexcept {++status;}

template<class Scope, class Fn>
void testScopeTerminate(Dune::TestSuite& test, Fn&& fn1, Fn&& fn2, int final_status = 1) {
  status = 0;
  auto name = Dune::className<Scope>();
  {
    Scope se{std::forward<Fn>(fn1)};
    test.check(status == 0, name + ": TestScope - Constructor");
    if constexpr (std::is_move_constructible_v<Scope>) {
      Scope se2{std::move(se)};
      test.check(status == 0, name + ": TestScope - Move Constructor");
      se2.release();
    } else {
      se.release();
    }
  }
  test.check(status == 0, name + ": TestScope - Release");
  {
    Scope se{std::forward<Fn>(fn2)};
    test.check(status == 0, name + ": TestScope - Constructor");
    if constexpr (std::is_move_constructible_v<Scope>)
      std::ignore = Scope{std::move(se)};
  }
  test.check(status == final_status, name + ": TestScope - Destructor");
}

template<class Factory>
void testScope(Dune::TestSuite& test, Factory&& factory) {
  using Fn = decltype(factory());
  testScopeTerminate<Dune::Std::scope_exit<std::decay_t<Fn>>, Fn>(test, factory(), factory());
  testScopeTerminate<Dune::Std::scope_success<std::decay_t<Fn>>, Fn>(test, factory(), factory());
  testScopeTerminate<Dune::Std::scope_fail<std::decay_t<Fn>>, Fn>(test, factory(), factory(), 0);
}

template<class Scope, class Fn>
void testScopeTerminateConstructorThrow(Dune::TestSuite& test, Fn&& fn, int final_status = 1) {
  status = 0;
  auto name = Dune::className<Scope>();
  {
    test.checkThrow([&]{
      Scope se{std::forward<Fn>(fn)};
    }, name + ": TestScopeConstructorThrow - Constructor Exception");
    test.check(status == final_status, name + ": TestScopeConstructorThrow - Constructor");
  }
}

template<class Factory>
void testScopeConstructorThrow(Dune::TestSuite& test, Factory&& factory) {
  using Fn = decltype(factory());
  testScopeTerminateConstructorThrow<Dune::Std::scope_exit<std::decay_t<Fn>>, Fn>(test, factory());
  // success path does not require to call the exit function if constructor fails
  testScopeTerminateConstructorThrow<Dune::Std::scope_success<std::decay_t<Fn>>, Fn>(test, factory(), 0);
  testScopeTerminateConstructorThrow<Dune::Std::scope_fail<std::decay_t<Fn>>, Fn>(test, factory());
}


template<class Scope, class Fn>
void testScopeTerminateMoveThrow(Dune::TestSuite& test, Fn&& fn, int final_status = 1) {
  status = 0;
  auto name = Dune::className<Scope>();
  {
    Scope se{std::forward<Fn>(fn)};
    test.check(status == 0, name + ": MoveThrow - Constructor");
    test.checkThrow([&]{
      Scope sem{std::move(se)};
    }, name + ": MoveThrow - Move Exception");
    // failed move didn't close the cope
    test.check(status == 0, name + ": MoveThrow - Move");
  }
  // since move failed, fist scope still made the exit
  test.check(status == final_status, name + ": MoveThrow - Failed Move");
}

template<class Factory>
void testScopeMoveThrow(Dune::TestSuite& test, Factory&& factory) {
  using Fn = decltype(factory());
  testScopeTerminateMoveThrow<Dune::Std::scope_exit<std::decay_t<Fn>>, Fn>(test, factory());
  testScopeTerminateMoveThrow<Dune::Std::scope_success<std::decay_t<Fn>>, Fn>(test, factory());
  testScopeTerminateMoveThrow<Dune::Std::scope_fail<std::decay_t<Fn>>, Fn>(test, factory(), 0);
}

int main(int argc, char **argv)
{
  Dune::TestSuite test;
  auto f0_factory = []{return f0;};
  testScope(test, f0_factory);
  auto f1_factory = []{return f1;};
  testScope(test, f1_factory);
  {
    struct F {
      F() {}
      F(F&&) noexcept {}
      F(const F&) { throw std::exception{};}
      void operator()() {
        ++status;
      }
    };
    // since move constructor doesn't throw, it chooses move constructor
    testScope(test, []{return F{};});
  }

  {
    struct H {
      H() {}
      H(H&&) { }
      H(const H&) { throw std::exception{};}
      void operator()() {
        ++status;
      }
    };
    // since move constructor throws, it chooses copy constructor
    testScopeConstructorThrow(test, []{return H{};});
  }

  {
    struct I {
      I() {}
      I(I&& f) : count{f.count+1} { if (count >= 2) std::terminate(); } // should not be called
      I(const I& f) : count{f.count+1} { if (count >= 2) throw std::exception{}; }
      void operator()() {
        ++status;
      }
      int count = 0;
    };
    I f0{};
    I f1{std::move(f0)};
    test.checkThrow([&]{
      std::ignore = I{f1};
    });

    // since move constructor may throw, it chooses copy constructor
    testScopeMoveThrow(test, []{return I{};});
  }

  // check reverse order of deletion
  {
    int exit_count = 0;
    Dune::Std::scope_exit scp_exit{[&]{
      test.check(3 == exit_count++, "Wrong exit count - Exit");
    }};

    Dune::Std::scope_success scp_success{[&]{
      test.check(2 == exit_count++, "Wrong exit count - Success");
    }};

    try {
      Dune::Std::scope_fail scp_fail{[&]{
        test.check(0 == exit_count++, "Wrong exit count - Fail 0");
      }};
      throw std::runtime_error{"0"};
    } catch (...) {
      try {
      Dune::Std::scope_fail scp_fail{[&]{
        test.check(1 == exit_count++, "Wrong exit count - Fail 1");
      }};
      throw std::runtime_error{"1"};
      } catch (...) {
        Dune::Std::scope_fail scp_fail{[&]{
          test.check(false, "Wrong exit count - Fail NEVER");
        }};
      }
    }

  }

  return test.exit();
}
