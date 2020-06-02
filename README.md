# typer
Declarative Checks for R Functions Input / Output Parameters

The package allows to describe expected variable types, annotate functions input / output parameters with types, and verify correctness of passed input variables and function return values during execution.

In short, package simplifies checks for function input / output parameters. It adds strictness to R dynamic typing approach and allows to write more robust code.

Simple example

```
func <- function(a, b) {
    return(a + b)
}

func(1, "1") # This will throw error

func(1, TRUE) # This may result in incorrect behaviour
```

Typical approach for dealing with such issues is to add explicit input params checks.

```
func_safer <- function(a, b) {
    
    if (!is.numeric(a)) stop("param a must be numeric")
    if (!is.numeric(b)) stop("param b must be numeric")

    return(a + b)
}
```

Typer package simplifies writing such checks.

```
func_nonsafe <- function(a, b) {
    return(a + b)
}

func_safe <- check_function(
    func_nonsafe,
    input = list(
        a = type.numeric(),
        b = type.numeric()
    ),
    output = type.numeric()
)

func_safe(1, 2) # call annotated function as usual

func_safe(1, TRUE) # input params will be checked automatically and error will be thrown
```

By using typer one can describe more complicated structures - e.g. lists, data frames, environments - and recursive structures (e.g. list of lists)

Example:

```
func <- function(a, b) {
    rbind(
        data.frame(name = a$name, age = a$age, weight = a$weight),
        data.frame(name = b$name, age = b$age, weight = b$weight)
    )
}

func(
    list(name = "John", age = 30L, weight = 90),
    list(name = "Bill", age = 40L, weight = 80)
)
```

With typer we can annotate this function as follows:

```
func_safe <- check_function(
    function(a, b) {
        rbind(
            data.frame(name = a$name, age = a$age, weight = a$weight),
            data.frame(name = b$name, age = b$age, weight = b$weight)
        )
    },
    input = list(
        a = type.list(name = type.character(), age = type.integer(), weight = type.numeric()),
        b = type.list(name = type.character(), age = type.integer(), weight = type.numeric())
    ),
    output = type.data.frame(cols = list(name = type.factor(), age = type.integer(), weight = type.numeric()))
)

# all is correct
func_safe(
    list(name = "John", age = 30L, weight = 90),
    list(name = "Bill", age = 40L, weight = 80)
)

# age is of wrong type
func_safe(
    list(name = "John", age = 30, weight = 90),
    list(name = "Bill", age = 40, weight = 80)
)
```

And also one can use type metadata to automatically generate docs:

```
html_report <- generate_func_doc(func_safe, "func_safe")
html_file <- tempfile(fileext = ".html")
writeChar(paste0(html_report), html_file)
browseURL(html_file)
```
