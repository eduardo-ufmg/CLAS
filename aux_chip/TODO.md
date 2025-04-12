Refactor `Expert`. Consider C++20.
I need a base class, `BaseExpert` and three derivate classes, `ExpertCHIP`, `ExpertRCHIP` and `ExpertPred`.
`ExpertCHIP` and `ExpertRCHIP` have the same constructors, attributes, and methods as the `BaseExpert`. They only differ in the implementation of the functions `computeNormal` and `computeBias`.
`ExpertPred` has only `id`, `midpoint`, `normal` and `bias` as attributes. Its constructor has those as input and sets them. No other method is needed for this derivate class.
