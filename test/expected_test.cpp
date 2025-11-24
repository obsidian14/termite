#include <termite/expected.h>

#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Expected test suites", "[expected]") {
  using termite::Expected;
  using termite::kInPlace;
  using termite::kUnexpect;
  using termite::Unexpected;

  {
    Expected<int, int> e = 1;
    REQUIRE(e);
    REQUIRE(*e == 1);
  }
  {
    Expected<int, int> e = Unexpected(42);
    REQUIRE(!e);
    REQUIRE(e.Error() == 42);
  }
  {
    Expected<int, int> e(kInPlace, 42);
    REQUIRE(e);
    REQUIRE(*e == 42);
  }
  {
    Expected<int, int> e(kUnexpect, 42);
    REQUIRE(!e);
    REQUIRE(e.Error() == 42);
  }
  {
    Expected<std::vector<int>, int> e(kInPlace, {24, 42});
    REQUIRE(e);
    REQUIRE((*e)[0] == 24);
    REQUIRE((*e)[1] == 42);
  }
  {
    Expected<int, std::vector<int>> e(kUnexpect, {24, 42});
    REQUIRE(!e);
    REQUIRE(e.Error()[0] == 24);
    REQUIRE(e.Error()[1] == 42);
  }
  {
    Expected<int, int> e1 = 42;
    Expected<short, short> e2 = e1;
    REQUIRE(e2);
    REQUIRE(*e2 == 42);
  }
  {
    Expected<int, int> e1 = Unexpected(42);
    Expected<int, int> e2 = e1;
    REQUIRE(!e2);
    REQUIRE(e2.Error() == 42);
  }
  {
    auto f = []() -> Expected<int, int> { return 42; };
    Expected<int, int> e = f();
    REQUIRE(e);
    REQUIRE(*e == 42);
  }
  {
    Expected<int, int> e;
    e = 42;
    REQUIRE(e);
    REQUIRE(*e == 42);
  }
  {
    Expected<int, int> e;
    e = Unexpected(42);
    REQUIRE(!e);
    REQUIRE(e.Error() == 42);
  }
  {
    Expected<int, int> e1 = 42;
    Expected<int, int> e2;
    e2 = e1;
    REQUIRE(e2);
    REQUIRE(*e2 == 42);
  }
  {
    Expected<std::vector<int>, int> e = Unexpected(42);
    e.Emplace(std::vector<int>{24, 42});
    REQUIRE(e);
    REQUIRE((*e)[0] == 24);
    REQUIRE((*e)[1] == 42);
  }
  {
    Expected<int, int> e1 = 42;
    Expected<int, int> e2 = Unexpected(24);
    using std::swap;
    swap(e1, e2);
    REQUIRE(!e1);
    REQUIRE(e2);
    REQUIRE(e1.Error() == 24);
    REQUIRE(*e2 == 42);
  }
  {
    Expected<int, int> e1 = 42;
    REQUIRE(e1);
    REQUIRE(e1.HasValue());
    REQUIRE(*e1 == 42);
    REQUIRE(e1.ErrorOr(24) == 24);
    Expected<int, int> e2 = Unexpected(24);
    REQUIRE(!e2);
    REQUIRE(!e2.HasValue());
    REQUIRE(e2.Error() == 24);
    REQUIRE(e2.ValueOr(42) == 42);
  }
  {
    Expected<int, int> e = 42;
    e = e.AndThen([](int x) -> Expected<int, int> { return x + 21; });
    REQUIRE(e);
    REQUIRE(*e == 63);
  }
  {
    Expected<int, int> e = Unexpected(24);
    e = e.OrElse([](int x) -> Expected<int, int> { return x + 18; });
    REQUIRE(e);
    REQUIRE(*e == 42);
  }
  {
    Expected<int, int> e = 42;
    e = e.Transform([](int x) { return x * 2; });
    REQUIRE(e);
    REQUIRE(*e == 84);
  }
  {
    Expected<int, int> e = Unexpected(24);
    e = e.TransformError([](int x) { return x / 2; });
    REQUIRE(!e);
    REQUIRE(e.Error() == 12);
  }
  {
    Expected<void, int> e = Unexpected(42);
    REQUIRE(!e);
    REQUIRE(e.Error() == 42);
  }
}
