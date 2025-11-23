# termite

Single header implementation of `std::expected` written in c++20.

Accroding to the standard document,
[`std::expected`](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0323r3.pdf)
is a vocabulary type which contains an expected value of type `T` , or an error `E`.
The class skews towards behaving like a `T`, because its intended use is when the
expected type is contained.
When something unexpected occurs, more typing is required. When all is good, code
mostly looks as if a `T` were being handled.
This implementation provides a number of utilities to make coding with 
`Expected Object` cleaner.

Calling the `Error()` method on an expected value, 
or using the `*` or `->` operators on an unexpected value, is undefined behavior.
In this implementation it causes an assertion failure.
The implementation of assertions can be overridden by defining the marco
`TERMITE_ASSERT(boolean_condition)` before #including <termite/expected.h>;
by default, the assertion causes an unreachable failure, which uses
compiler specific extensions if possible, otherwise, does nothing.
Note that correct code would not rely on these assertions.

Ine the implementation,
the difference between calling the `Value()` method and using the `*` operator 
on an unexpected value is only that one will throw a exception, another will
cause a assertion failure.
Considering programers coding in a range of environment,
every environment has its own requirement. 
In some scene which the size of binary file should be considered,
you had better disable expection handling to get smaller size.
In order to achieve the arch-independent goal,
the implementation delete the `Value()` method, and users should using
the `*` operator to get the expected value directly.
If you wish to use the `Value()` method no matter how the expection handling
performs worse, you can report an issue to this library and the developers would
like to adopt your opinion.

