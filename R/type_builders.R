#' Type Builder for Numeric Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.numeric = function(len = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 1L, len = len, allow.na = allow.na, desc = desc)
}

#' Type Builder for Integer Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.integer = function(len = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 2L, len = len, allow.na = allow.na, desc = desc)
}

#' Type Builder for Logical Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.logical = function(len = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 3L, len = len, allow.na = allow.na, desc = desc)
}

#' Type Builder for Character Vectors
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.character = function(len = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 4L, len = len, allow.na = allow.na, desc = desc)
}

#' Type Builder for Data Frame
#'
#' @param cols Named list with data frame columns descriptions
#' @param nrow Expected count of rows in data frame
#' @param ncol Expected count of columns in data frame
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param key Character vector with data frame key fields, may be skipped
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.data.frame = function(cols, nrow = NA_integer_, ncol = NA_integer_, key = NA_character_, desc = NULL) {
    list(type = 6L, items = cols, nrow = nrow, ncol = ncol, key = key, desc = desc)
}

#' Type Builder for Data Table
#'
#' @param cols Named list with data table columns descriptions
#' @param nrow Expected count of rows in data table
#' @param ncol Expected count of columns in data table
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param key Character vector with data table key fields, may be skipped
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.data.table = function(cols, nrow = NA_integer_, ncol = NA_integer_, key = NA_character_, desc = NULL) {
    list(type = 7L, items = cols, nrow = nrow, ncol = ncol, key = key, desc = desc)
}

#' Type Builder for Complex Numbers Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.complex = function(len = NA_integer_, allow.na = NA, desc = NULL) {
    list(type = 9L, len = len, allow.na = allow.na, desc = desc)
}

#' Type Builder for Vector of Factors
#'
#' This wraps an arbitrary function so that its input and output could be validate against predefined type safety rules. If either fails to pass, error is thrown.
#'
#' @export
#'
type.factor = function(levels = NULL, desc = NULL) {
    list(type = 10L, levels = sort(levels), desc = desc)
}

#' Type Builder for Numeric Matrix
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.matrix.numeric = function(nrow = NA_integer_, ncol = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 11L, nrow = nrow, ncol = ncol, allow.na = allow.na, desc = desc)
}

#' Type Builder for Integer Matrix
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.matrix.integer = function(nrow = NA_integer_, ncol = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 12L, nrow = nrow, ncol = ncol, allow.na = allow.na, desc = desc)
}

#' Type Builder for Logical Matrix
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.matrix.logical = function(nrow = NA_integer_, ncol = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 13L, nrow = nrow, ncol = ncol, allow.na = allow.na, desc = desc)
}

#' Type Builder for Character Matrix
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.matrix.character = function(nrow = NA_integer_, ncol = NA_integer_, allow.na = TRUE, desc = NULL) {
    list(type = 14L, nrow = nrow, ncol = ncol, allow.na = allow.na, desc = desc)
}

#' Type Builder for List
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.list = function(..., desc = NULL) {
    list(type = 5L, items = list(...), desc = desc)
}

#' Type Builder for Environments
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.env = function(..., desc = NULL) {
    list(type = 8L, items = list(...), desc = desc)
}

#' Type Builder for Raw Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.raw = function(desc = NULL) {
    list(type = 17L, desc = desc)
}

#' Type Builder for Raw Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.func = function(desc = NULL) {
    list(type = 18L, desc = desc)
}

#' Type Builder for Raw Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.expr = function(desc = NULL) {
    list(type = 19L, desc = desc)
}

#' Type Builder for Numeric Vector
#'
#' @param len Expected length of vector to be checked. Check will be skipped if len is NA
#' @param allow.na Logical flag which indicates whether vector may contain NA values
#' @param desc Optional string with variable purpose descritption
#'
#' @export
#'
type.any = function(desc = NULL) {
    list(type = 20L, desc = desc)
}
