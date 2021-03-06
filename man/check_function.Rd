% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/check_functions.R
\name{check_function}
\alias{check_function}
\title{Assigns Type Safety Decorator to a Function}
\usage{
check_function(func, input, output)
}
\arguments{
\item{func}{Function to be decorated}

\item{input}{List with types descriptions for input function parameters}

\item{output}{Type description for output parameter}
}
\value{
Returns output value of provided function without any modification in case of success.
        Will raise an exception if input / output params didn't pass checks.
}
\description{
This functions creates a decorator over provided function. This decorate performs checks of input / output params by using provided types.
}
\examples{
\dontrun{
  demo_func = function(a, b) a + b
  
  demo_func_safe = check_function(
      demo_func,
      input = list(a = type.numeric(), b = type.numeric()),
      output = type.numeric()
  )
  
  demo_func_safe(1, 2)
  
  demo_func_safe(1, "2")

}

}
