library(testthat)
library(typer)

# Test 1 Simple Happy Path --------------------------------------

demo_func = function(a, b) a + b

demo_func_safe = check_function(
    demo_func,
    input = list(a = type.numeric(), b = type.numeric()),
    output = type.numeric()
)

expect_equal(demo_func_safe(1, 2), 3)

# Test 2 Simple Bad Path --------------------------------------

expect_error(demo_func_safe(1, "2"))

# Test 3 Complicated Structures --------------------------------------

demo_func2 = function(a, b) {

    c = list(
        name = a$name,
        age = b$age,
        weight = 0.5 * (a$weight + b$weight)
    )

}

person_type = type.list(
    name = type.character(len = 1),
    age = type.integer(len = 1),
    weight = type.numeric(len = 1)
)

demo_func2_safe = check_function(
    demo_func2,
    input = list(a = person_type, b = person_type),
    output = person_type
)

x = list(name = "John", age = 26L, weight = 70)

demo_func2_safe(x, x)

expect_error(demo_func2_safe(list()))
