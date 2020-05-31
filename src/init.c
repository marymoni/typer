#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include "check_variable.h"

R_CallMethodDef callMethods[] = {
    {"R_check_variable", (DL_FUNC) &R_check_variable, 3},
    {"R_check_input_params", (DL_FUNC) &R_check_input_params, 2},
    {"R_get_type_name", (DL_FUNC) &R_get_type_name, 1},
    {NULL, NULL, 0}
};

void R_init_typer(DllInfo *info) {
    R_registerRoutines(info, NULL, callMethods, NULL, NULL);
    R_useDynamicSymbols(info, FALSE);
}
