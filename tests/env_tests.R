library(testthat)
library(typer)

# Test 1 Simple Happy Path --------------------------------------

my.env = new.env()

my.env$a = 1
my.env$b = 1L
my.env$c = "1"

my_env_type = type.env(
    a = type.numeric(),
    b = type.integer(),
    c = type.character()
)

expect_true(check_variable(my.env, my_env_type))

# Test 2 Simple Bad Path --------------------------------------

my_env_type2 = type.env(
    a = type.numeric(),
    b = type.integer(),
    c = type.integer()
)

expect_false(check_variable(my.env, my_env_type2))

# Test 3 Recursive with sub elements --------------------------------------

my_env3 = new.env()

my_env3$a = 123

my_env3$b = "bbb"

my_env3$c = list(
    name = "John",
    age = 20L,
    weight = 65
)

my_env_type3 = type.env(
    a = type.numeric(),
    b = type.character(),
    c = type.list(
        name = type.character(),
        age = type.integer(),
        weight = type.numeric()
    )
)

expect_true(check_variable(my_env3, my_env_type3))
