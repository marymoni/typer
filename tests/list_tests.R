library(testthat)
library(typer)

# Test 1 Simple Happy Path --------------------------------------

lst1 = list(a = 1, b = 2L, c = "3")

lst1_type = type.list(
    a = type.numeric(), 
    b = type.integer(), 
    c = type.character()
)

expect_true(check_variable(lst1, lst1_type))

# Test 2 Simple Bad Path --------------------------------------

lst2 = list(a = 1, b = 2L, c = "3")

lst2_type = type.list(
    a = type.numeric(), 
    b = type.integer(), 
    c = type.numeric()
)

expect_false(check_variable(lst2, lst2_type))

# Test 3 Recursive Lists --------------------------------------

lst3 = list(a = 1, b = list(b1 = 1, b2 = "c", b3 = list(b31 = 1, b32 = 1)), c = list(c1 = 1, c2 = 2))

lst3_type = type.list(
    a = type.numeric(len = 1), 
    b = type.list(
        b1 = type.numeric(),
        b2 = type.character(),
        b3 = type.list(
            b31 = type.numeric(),
            b32 = type.numeric()
        )
    ), 
    c = type.list(
        c1 = type.numeric(),
        c2 = type.numeric()
    )
)

expect_true(check_variable(lst3, lst3_type))
