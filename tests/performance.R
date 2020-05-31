library(microbenchmark)
library(typer)

demo_func <- function(a, b) a + b

demo_func_safe <- check_function(
    demo_func,
    input = list(a = type.numeric(), b = type.numeric()),
    output = type.numeric()
)

demo_func_safe(1, 2)

microbenchmark(list = list(
    demo_func = quote({ demo_func(1, 2) }),
    demo_func_safe = quote({ demo_func_safe(1, 2) })
), times  = 1000)

rastrigin <- function(x, y) {
    20 + x^2 - 10*cos(2*pi*x) + y^2 - 10*cos(2*pi*y)
}

rastrigin_safe <- check_function(
    rastrigin,
    input = list(x = type.numeric(), y = type.numeric()),
    output = type.numeric()
)

microbenchmark(list = list(
    rastrigin = quote({ rastrigin(1, 2) }),
    rastrigin_safe = quote({ rastrigin_safe(1, 2) })
), times  = 1000)
