#' Assigns Type Safety Decorator to a Function
#'
#' This functions creates a decorator over provided function. This decorate performs checks of input / output params by using provided types.
#'
#' @param func Function to be decorated
#' @param input List with types descriptions for input function parameters
#' @param output Type description for output parameter
#'
#' @return Returns output value of provided function without any modification in case of success.
#'         Will raise an exception if input / output params didn't pass checks.
#'
#' @export
#'
#' @examples
#' \dontrun{
#'   demo_func = function(a, b) a + b
#'   
#'   demo_func_safe = check_function(
#'       demo_func,
#'       input = list(a = type.numeric(), b = type.numeric()),
#'       output = type.numeric()
#'   )
#'   
#'   demo_func_safe(1, 2)
#'   
#'   demo_func_safe(1, "2")
#'
#' }
#'
check_function <- function(func, input, output) {

    decorated_func <- function(...) {

        if (!is.null(input) && !.Call(R_check_input_params, params = list(...), type = input)) {
            stop("Type check failed for function input")
        }

        res <- func(...)

        if (!is.null(output) && !check_variable(res, output, var_name = "res")) {
            stop("Type check failed for function output")
        }

        res
    }

    attr(decorated_func, 'input')  <- input
    attr(decorated_func, 'output') <- output
    class(decorated_func) <- c("typer", class(decorated_func))

    return(decorated_func)
}

#' Helper function which checks variable compatibility with provided type description
#'
#' @param var_obj Variable to be checked
#' @param var_type Variable type description to be matched
#' @param var_name Variable name for logging purposes
#'
#' @return TRUE if types checks passed, FALSE otherwise. Also function writes detailed output to console.
#'
#' @export
#'
check_variable <- function(var_obj, var_type, var_name = "foo") {
    .Call(R_check_variable, var_obj, var_type, var_name)
}
