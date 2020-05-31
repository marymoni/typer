#' Generate package HTML docs basing on types description
#' 
#' Helper function which generates HMTL documentation for published package functions with input / output checkers
#' 
#' @param package_name Package name
#'
#' @return Character vector with HTML text which contains package functions documentation
#'
#' @export
#'
#' @examples
#' \dontrun{
#'   html_report <- generate_package_doc("demopack")
#'   html_file <- tempfile(fileext = ".html")
#'   writeChar(html_report, html_file)
#'   browseURL(html_file)
#' }
#'
generate_package_doc <- function(package_name) {

    if (!require(package_name, character.only = TRUE)) stop(paste0("Cannot load package", package_name))
    
    package_func_names <- ls(paste0("package:", package_name))
    
    return(paste0(c(
        html(TRUE),
        h1(package_name),
        unlist(sapply(package_func_names, function(package_func_name) {
            package_func_body <- get(package_func_name)
            if (!any("typer" == class(package_func_body))) return(NULL)
            generate_func_doc(package_func_body, package_func_name)
        }, USE.NAMES = FALSE)),
        html(FALSE)
    )))
}

#' Generate function HTML doc basing on types description
#'
#' Helper function which generates HTML document section for function input / output parameters
#'
#' @param func_body Function body - decorator function which is returned by \code{\link{check_function}}
#' @param func_name Function name
#'
#' @return Character vector with HTML text which contains function doc section
#'
#' @export
#'
generate_func_doc <- function(func_body, func_name) {

    input = attr(func_body, "input")
    output = attr(func_body, "output")
    
    html <- c(
        h2(func_name),
        if (!is.null(input)) c(
            h3("Input Params"),
            unlist(mapply(generate_variable_doc, input, names(input), USE.NAMES = FALSE))
        ),
        if (!is.null(output)) c(
            h3("Return Value"),
            generate_variable_doc(output, "Return Value")
        )
    )

    return(html)
}

#' Generate variable HTML doc basing on types description
#'
#' Helper function which generates HTML document section for provided variable type description
#'
#' @param var_type List with variable type attributes
#' @param var_name Variable name
#'
#' @return Character vector with HTML text which contains variable doc section
#'
#' @export
#'
generate_variable_doc <- function(var_type, var_name) {
    c(
        ul(TRUE),
        li(c(
            var_name, " - ",
            "type: ", get_type_name(var_type$type), "; ",
            "allow NA's: ", var_type$allow.na, "; ",
            "description: ", if(!is.null(var_type$descr)) var_type$descr
        )),
        if (!is.null(var_type$items)) c(
            unlist(mapply(generate_variable_doc, var_type$items, names(var_type$items), USE.NAMES = FALSE))
        ),
        ul(FALSE)
    )
}

get_type_name <- function(var_type_id) {
    .Call(R_get_type_name, var_type_id)
}

html <- function(opening = TRUE) {
    ifelse(opening, "<html>", "</html>")
}

h1 <- function(value) {
    c("<h1>", value, "</h1>")
}

h2 <- function(value) {
    c("<h2>", value, "</h2>")
}

h3 <- function(value) {
    c("<h3>", value, "</h3>")
}

ul <- function(opening = TRUE) {
    ifelse(opening, "<ul>", "</ul>")
}

li <- function(value) {
    c("<li>", value, "</li>")
}
