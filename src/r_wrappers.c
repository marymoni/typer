#include <R.h>
#include <Rinternals.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check_variable.h"

/*
 * Checks input variable matches type
 *
 *   var_obj: variable object
 *   var_type: variable type
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if variable matches type, FALSE otherwise
 */
SEXP R_check_variable(const SEXP var_obj, const SEXP var_type, const SEXP var_name) {

    int res = check_variable(var_obj, var_type, CHAR(STRING_ELT(var_name, 0)));
    
    return ScalarLogical(res);
}

/*
 * Checks input function parameters match types
 *
 *   params: list with input function parameters
 *   types: list with type descriptions of input parameters
 *
 *   returns: TRUE if input params match types, FALSE othwersie
 */
SEXP R_check_input_params(const SEXP params, const SEXP types) {

    int res = 1;
    int param_res;

    SEXP param_names = getAttrib(params, R_NamesSymbol);

    int param_len = length(params);

    int expected_len = length(types);

    SEXP type_names = getAttrib(types, R_NamesSymbol);

    if (isNull(param_names)) {

        // no param names, so will match them by position number

        for(int i = 0; i < MIN(param_len, expected_len); i++) {
            param_res = check_variable(VECTOR_ELT(params, i), VECTOR_ELT(types, i), CHAR(STRING_ELT(type_names, i)));
            if (param_res == 0) {
                res = 0;
            }
        }

    } else {

        int has_blank_names = 0;

        int matched_params[expected_len];
        memset(matched_params, 0, sizeof matched_params);

        // first try to match params by name
        for(int i = 0; i < length(param_names); i++) {

            const char * param_name = CHAR(STRING_ELT(param_names, i));

            if (strcmp(param_name, "") == 0) {
                has_blank_names = 1;
                continue;
            }

            int param_pos = get_position_by_name(types, param_name);

            if (param_pos == -1) {
                Rprintf("There is no type description for param '%s'\n", param_name);
                res = 0;
                continue;
            }

            matched_params[param_pos] = 1;

            param_res = check_variable(VECTOR_ELT(params, i), VECTOR_ELT(types, param_pos), param_name);
            
            res = res && param_res;
        }

        // match rest of params by position number
        if (res && has_blank_names) {

            int j = 0;
            for(int i = 0; i < length(param_names); i++) {
                if (!matched_params[i]) {
                    param_res = check_variable(VECTOR_ELT(params, i), VECTOR_ELT(types, j), CHAR(STRING_ELT(type_names, j)));
                    if (param_res == 0) {
                        res = 0;
                    }
                    j++;
                }
            }
        }
    }

    return ScalarLogical(res);
}

/*
 *   Helper function to return type name by id
 */
SEXP R_get_type_name(const SEXP var_type_id) {
    return mkChar(get_var_type_name(INTEGER(var_type_id)[0]));
}
