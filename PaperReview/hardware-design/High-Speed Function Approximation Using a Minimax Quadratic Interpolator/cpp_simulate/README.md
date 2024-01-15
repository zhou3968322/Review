# TestFloatMath

test and implementation for float arithmetic functions.

## Illustration

- see [log example](./src/log.cpp) 's docstring.
- - log2f_as_nvidia's error is the same as nvidia intrinsic, has a max absolute error < 2^{-22}.
- - log2_ulp1's error is as amd intrinsic,has a ulp error limit < 1 ulp.
- - - **log2 has splited table in two parts: i < 32, i>=32**, which not has mentioned by paper.but it's very important to eliminate leading zeros for mantissa near 1.

## More

- See Paper [High-speed function approximation using a minimax quadratic interpolator](https://ieeexplore.ieee.org/abstract/document/1388195) 
- - there's many tricks that can simplify hardware design in this paper.
- - - how to choose the best table?
- - - variant correction multiplication
- - - reduced special square unit.
- - - how to choose the best SD(signed-digit) format to reduce partial product sums. 