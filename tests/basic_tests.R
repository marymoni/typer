library(testthat)
library(typer)

# Numeric Vectors Tests -----------------------

expect_true(check_variable(1, type.numeric()))
expect_true(check_variable(c(1, 2, 3), type.numeric(len = 3)))
expect_true(check_variable(numeric(), type.numeric(len = 0)))
expect_true(check_variable(c(1, NA_real_, 3), type.numeric(len = 3, allow.na = TRUE)))

expect_output(expect_false(check_variable(1, type.integer())), "Type doesn't match")
expect_output(expect_false(check_variable(c(1, 2, 3), type.numeric(len = 2))), "Length doesn't match")
expect_output(expect_false(check_variable(c(1, NA_real_, 3), type.numeric(len = 3, allow.na = FALSE))), "has NA elements")

# Integer Vectors Tests -----------------------

expect_true(check_variable(1L, type.integer()))

# Logical Vectors Tests

expect_true(check_variable(TRUE, type.logical()))
expect_false(check_variable(1L, type.logical()))
expect_false(check_variable(1, type.logical()))
expect_true(check_variable(c(TRUE, FALSE, TRUE), type.logical(len = 3)))
expect_true(check_variable(logical(), type.logical(len = 0)))
expect_false(check_variable(c(TRUE, NA, TRUE), type.logical(allow.na = FALSE)))
expect_true(check_variable(c(TRUE, NA, TRUE), type.logical(allow.na = TRUE)))

# Character Vectors Tests -----------------------

expect_true(check_variable("1", type.character()))
expect_true(check_variable(c("1", "2", "3"), type.character(len = 3)))
expect_true(check_variable(character(), type.character(len = 0)))
expect_false(check_variable(1, type.character()))

# Factors Tests -----------------------

expect_true(check_variable(as.factor(c("a", "b", "a")), type.factor()))
expect_true(check_variable(as.factor(c("a", "b", "a")), type.factor(len = 3)))
expect_false(check_variable(as.factor(c("a", "b", "a")), type.factor(len = 3)))
expect_true(check_variable(as.factor(c("a", "b", "a")), type.factor(levels = c("a", "b"))))
expect_false(check_variable(as.factor(c("a", "c", "a")), type.factor(levels = c("a", "b"))))

# Raw Tests -----------------------

expect_true(check_variable(as.raw(123), type.raw()))
expect_output(expect_false(check_variable(123, type.raw())), "Type doesn't match")

# Complex Numbers Tests -----------------------

expect_true(check_variable(c(1i, 2i, 3i), type.complex()))
expect_false(check_variable(c(1i, NA, 2i, 3i), type.complex(allow.na = FALSE)))

# Numeric Matrices Tests -----------------------

expect_true(check_variable(matrix(1,1,1), type.matrix.numeric()))
expect_false(check_variable(matrix(1,1,1), type.matrix.integer()))
expect_true(check_variable(matrix(1, nrow = 2, ncol = 3), type.matrix.numeric(nrow = 2, ncol = 3)))
expect_false(check_variable(matrix(1,1,1), type.matrix.numeric(nrow = 2, ncol = 2)))

# Any Variable Tests -----------------------

expect_true(check_variable(1, type.any()))
expect_true(check_variable("1", type.any()))
expect_true(check_variable(NULL, type.any()))

# Incorrect Type Description -----------------------

expect_error(check_variable(1, NULL))
expect_error(check_variable(1, list()))
