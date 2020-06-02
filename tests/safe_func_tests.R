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

person_x = list(name = "John", age = 26L, weight = 70)

demo_func2_safe(person_x, person_x)

expect_error(demo_func2_safe(list()))

# Test 4 Named & default params check --------------------------------------

demo_func3 = function(a = 0L, b = 1, c = "test", d = FALSE) {

    return(TRUE)
}

demo_func3_safe = check_function(demo_func3, input = list(
    a = type.integer(),
    b = type.numeric(),
    c = type.character(),
    d = type.logical()
), output = NULL)

expect_true(demo_func3_safe())

expect_error(demo_func3_safe(a = 1))
expect_true(demo_func3_safe(a = 1L))

expect_error(demo_func3_safe(a = 1, b = 1, c = 1))
expect_true(demo_func3_safe(a = 1L, b = 1, c = "1"))

expect_error(demo_func3_safe(a = 1, b = 1, c = 1, d = TRUE))
expect_true(demo_func3_safe(a = 1L, b = 1, c = "1", d = TRUE))

expect_error(demo_func3_safe(b = 1, c = 1, d = TRUE))
expect_true(demo_func3_safe(b = 1, c = "1", d = TRUE))

# Test 5 Complicated Structures --------------------------------------

demo_func4 = function(a, b, c, d) {
    return(TRUE)
}

demo_func4_safe = check_function(demo_func4, input = list(
    a = person_type,
    b = type.env(
        b1 = person_type,
        b2 = person_type
    ),
    c = person_type,
    b = person_type
), output = type.any())

e = new.env()

e$b1 = person_x
e$b2 = person_x

expect_error(demo_func4_safe(person_x, NULL, person_x, person_x))

expect_true(demo_func4_safe(person_x, e, person_x, person_x))
