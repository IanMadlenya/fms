# iter - input iterator for numerics

This project solves a much simpler problem than current C++ proposals for ranges.
It provides a limited set of algorithms designed to be easily optimized by modern compilers
to make one pass over constant data that model input iterators.

An `enumerator` is an input iterators that provides `operator bool() const` which returns `true` when
it is valid to dereference the iterator. This is similar to `MoveNext` in the `IEnumerator`
C# interface, but fits in better with C++ notions of iteration. Much better.

They don't satisfy the input iterator axioms for `operator==()` but do for `operator!=()` when
comparing for end conditions because `operator bool() const` gets called. This is a small
price to pay.

The basic idea is that `enumerator<I,T>` is an input iterator that uses `I` for iteration and returns value_type `T`,
perhaps different from `I::value_type`.
The canonical example is applying a function from T -> U on an input iterator.
The resulting iterator should be of type `enumerator<T,U>`, but we still need the original iterator
in order to do lazy evaluation.
This requires bundling up everything needed for just in time delivery.
The implementation is drastically simplified at trivial to reason about by passing everything by value.

All classes deriving from `enumerator_base` are quite simple: save the iterator an necessary
accoutrements then implement `operator bool`, `operator*`, and `operator++` pre and post increment. 
That's all! 

The convention is to use `class_` for class names and `class` instead of `make_class`. Users never
need to use the class directly, just the corresponding template function.

operator, => concatenate

operator^ => monotonic cycle