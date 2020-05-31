library(testthat)
library(typer)

# Test 1 Simple Happy Path --------------------------------------

df = data.frame(a = 1:5, b = 3, c = "ccc", stringsAsFactors = FALSE)

df_type = type.data.frame(cols = list(
    a = type.integer(),
    b = type.numeric(),
    c = type.character()
))

expect_true(check_variable(df, df_type))

# Test 2 Simple Bad Path --------------------------------------

df_type2 = type.data.frame(cols = list(
    a = type.integer(),
    b = type.numeric(),
    c = type.factor()
))

expect_false(check_variable(df, df_type2))
