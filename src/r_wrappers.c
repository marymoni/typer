#include <R.h>
#include <Rinternals.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check_variable.h"

/*
 * Function: square_the_biggest
 * ----------------------------
 *   Returns the square of the largest of its two input values
 *
 *   n1: one real value 
 *   n2: the other real value
 *
 *   returns: the square of the larger of n1 and n2 
 */
SEXP R_check_variable(const SEXP variable, const SEXP type, const SEXP variable_name) {

    int res = check_variable(variable, type, CHAR(STRING_ELT(variable_name, 0)));
    
    return ScalarLogical(res);
}

/*
 * Function: square_the_biggest
 * ----------------------------
 *   Returns the square of the largest of its two input values
 *
 *   n1: one real value 
 *   n2: the other real value
 *
 *   returns: the square of the larger of n1 and n2 
 */
SEXP R_check_input_params(const SEXP params, const SEXP type) {

    int res = 1;
    int param_res;

    SEXP param_names = getAttrib(params, R_NamesSymbol);

    int param_len = length(params);

    int expected_len = length(type);

    SEXP type_names = getAttrib(type, R_NamesSymbol);

    if (isNull(param_names)) {

        // no param names, so will match them by position number

        for(int i = 0; i < MIN(param_len, expected_len); i++) {
            param_res = check_variable(VECTOR_ELT(params, i), VECTOR_ELT(type, i), CHAR(STRING_ELT(type_names, i)));
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

            int param_pos = get_position_by_name(param_names, param_name);

            if (param_pos == -1) {
                Rprintf("There is no description for param '%s'\n", param_name);
                res = 0;
                continue;
            }

            matched_params[param_pos] = 1;

            param_res = check_variable(VECTOR_ELT(params, i), VECTOR_ELT(type, param_pos), param_name);
            if (param_res == 0) {
                res = 0;
            }
        }

        // match rest of params by position number
        if (res && has_blank_names) {

            int j = 0;
            for(int i = 0; i < length(param_names); i++) {
                if (!matched_params[i]) {
                    param_res = check_variable(VECTOR_ELT(params, i), VECTOR_ELT(type, j), CHAR(STRING_ELT(type_names, j)));
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
 *   Returns the square of the largest of its two input values
 *
 *   n1: one real value 
 *   n2: the other real value
 *
 *   returns: the square of the larger of n1 and n2 
 */
SEXP R_get_type_name(const SEXP var_type_id) {
    return mkChar(get_var_type_name(INTEGER(var_type_id)[0]));
}
