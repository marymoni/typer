#include <R.h>
#include <Rinternals.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check_variable.h"

#define T_UNKNOWN     0
#define V_NUMERIC     1
#define V_INTEGER     2
#define V_LOGICAL     3
#define V_CHARACTER   4
#define T_LIST        5
#define T_DATAFRAME   6
#define T_DATATABLE   7
#define T_ENVIR       8
#define V_COMPLEX     9
#define V_FACTOR      10
#define M_NUMERIC     11
#define M_INTEGER     12
#define M_LOGICAL     13
#define M_CHARACTER   14
#define M_FACTOR      15
#define M_COMPLEX     16
#define V_RAW         17
#define T_CLOSURE     18
#define T_LANG        19
#define T_ANY         20

#define TYPES_COUNT   21

// mapping node types to internal R types
int sexp_type_map[TYPES_COUNT] = {
    /* T_UNKNOWN     = 0  */ VECSXP,
    /* V_NUMERIC     = 1  */ REALSXP,
    /* V_INTEGER     = 2  */ INTSXP,
    /* V_LOGICAL     = 3  */ LGLSXP,
    /* V_CHARACTER   = 4  */ STRSXP,
    /* T_LIST        = 5  */ VECSXP,
    /* T_DATAFRAME   = 6  */ VECSXP,
    /* T_DATATABLE   = 7  */ VECSXP,
    /* T_ENVIR       = 8  */ ENVSXP,
    /* V_COMPLEX     = 9  */ CPLXSXP,
    /* V_FACTOR      = 10 */ INTSXP,
    /* M_NUMERIC     = 11 */ REALSXP,
    /* M_INTEGER     = 12 */ INTSXP,
    /* M_LOGICAL     = 13 */ LGLSXP,
    /* M_CHARACTER   = 15 */ STRSXP,
    /* M_CHARACTER   = 16 */ INTSXP,
    /* M_CHARACTER   = 17 */ CPLXSXP,
    /* M_CHARACTER   = 18 */ RAWSXP,
    /* M_CHARACTER   = 19 */ CLOSXP,
    /* M_CHARACTER   = 20 */ LANGSXP,
    /* M_CHARACTER   = 21 */ NILSXP
};

const char * type_names_map[TYPES_COUNT] = {
    /* T_UNKNOWN     = 0  */ "unknown",
    /* V_NUMERIC     = 1  */ "numeric",
    /* V_INTEGER     = 2  */ "integer",
    /* V_LOGICAL     = 3  */ "logical",
    /* V_CHARACTER   = 4  */ "character",
    /* T_LIST        = 5  */ "list",
    /* T_DATAFRAME   = 6  */ "data.frame",
    /* T_DATATABLE   = 7  */ "data.table",
    /* T_ENVIR       = 8  */ "environment",
    /* V_COMPLEX     = 9  */ "complex",
    /* V_FACTOR      = 10 */ "factor",
    /* M_NUMERIC     = 11 */ "matrix.numeric",
    /* M_INTEGER     = 12 */ "matrix.integer",
    /* M_LOGICAL     = 13 */ "matrix.logical",
    /* M_CHARACTER   = 15 */ "matrix.character",
    /* M_CHARACTER   = 16 */ "matrix.factor",
    /* M_CHARACTER   = 17 */ "matrix.complex",
    /* M_CHARACTER   = 18 */ "raw",
    /* M_CHARACTER   = 19 */ "function",
    /* M_CHARACTER   = 20 */ "expression",
    /* M_CHARACTER   = 21 */ "any"
};

/*
 *   Main function to check variable compatibility with provided type description
 *
 *   var_obj: variable object to be checked
 *   var_type: list object with variable type description
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if checks passed successfully, FALSE otherwise
 */
int check_variable(const SEXP var_obj, const SEXP var_type, const char * var_name) {

    Rprintf("Checking type for variable '%s'\n", var_name);

    int var_type_id = asInteger(VECTOR_ELT(var_type, get_position_safe(var_type, "type")));
    
    if (var_type_id == T_ANY) return TRUE;

    if (!check_variable_type(var_type_id, var_obj, var_name)) return FALSE;

    switch(var_type_id)
    {
        case V_NUMERIC:
        case V_INTEGER:
        case V_LOGICAL:
        case V_CHARACTER:
        case V_FACTOR:
        case V_COMPLEX:
            if (!check_variable_dims(var_obj, var_type, var_name)) return FALSE;
            if (!check_variable_NAs(var_obj, var_type, var_name)) return FALSE;
            if (var_type_id == V_FACTOR) {
                if (!check_variable_class(var_obj, "factor", var_name)) return FALSE;
                if (!check_variable_attribute(var_obj, var_type, "levels", var_name)) return FALSE;
            }
            break;
        case M_NUMERIC:
        case M_INTEGER:
        case M_LOGICAL:
        case M_CHARACTER:
        case M_FACTOR:
        case M_COMPLEX:
            if (!isMatrix(var_obj)) {
                Rprintf("Variable '%s' is not matrix", var_name);
                return FALSE;
            }
            if (!check_variable_dims(var_obj, var_type, var_name)) return FALSE;
            if (!check_variable_NAs(var_obj, var_type, var_name)) return FALSE;
            break;
        case T_LIST:
            if (!check_variable_list(var_obj, var_type, var_name)) return FALSE;
            break;
        case T_DATAFRAME:
        case T_DATATABLE:
            if (!check_variable_class(var_obj, "data.frame", var_name)) return FALSE;
            if (!check_variable_list(var_obj, var_type, var_name)) return FALSE;
            if (var_type_id == T_DATATABLE) {
                if (!check_variable_class(var_obj, "data.table", var_name)) return FALSE;
                if (!check_variable_attribute(var_obj, var_type, "key", var_name)) return FALSE;
            }
            break;
        case T_ENVIR:
            if (!check_variable_envir(var_obj, var_type, var_name)) return FALSE;
            break;
        case T_CLOSURE:
        case T_LANG:
        case V_RAW:
            // no additional checks so far
            break;
        default:
            Rf_error("Variable '%s' has unsupported type '%d'", var_name, var_type_id);
    }

    return TRUE;
}

/*
 *   Returns descriptive type name based on R TYPEOF id
 *
 *   r_type_id: the other real value
 *
 */
const char *get_r_object_type_name(const int r_type_id) {

    switch (r_type_id) {
	    case LGLSXP:
            return type_names_map[V_LOGICAL];
	    case INTSXP:
            return type_names_map[V_INTEGER];
	    case REALSXP:
            return type_names_map[V_NUMERIC];
	    case STRSXP:
            return type_names_map[V_CHARACTER];
	    default:
            return type_names_map[T_UNKNOWN];
    }

    return type_names_map[T_UNKNOWN];
}

/*
 *   Returns descriptive type name based on custom type id
 *
 *   var_type_id: the other real value
 *
 */
const char *get_var_type_name(const int var_type_id) {

    return type_names_map[var_type_id];
}

/*
 *   Checks whether variable type match type description
 *
 *   var_type_id: custom variable type id
 *   var_obj: variable to be checked
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if types match, FALSE otherwise. Additionally function write R log output in case of mismatch.
 */
int check_variable_type(const int var_type_id, const SEXP var_obj, const char *var_name) {

    // check types

    int r_type_id = TYPEOF(var_obj);
    int expected_r_type_id = sexp_type_map[var_type_id];

    if (r_type_id != expected_r_type_id) {
        Rprintf("Type doesn't match for variable '%s': expected type is '%s' (TYPEOF = %d) and actual type is '%s' (TYPEOF = %d)\n",
            var_name, type_names_map[var_type_id], expected_r_type_id, get_r_object_type_name(r_type_id), r_type_id);
        return FALSE;
    }

    return TRUE;
}

/*
 *   Checks whether variable has specified class
 *
 *   var_obj: variable to be checked
 *   class_name: class name to be checked
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if types match, FALSE otherwise. Additionally function write R log output in case if class not found.
 */
int check_variable_class(const SEXP var_obj, const char *class_name, const char *var_name) {

    SEXP obj_classes  = getAttrib(var_obj, R_ClassSymbol);

    if (!isNull(obj_classes)) {
        for(int i = 0; i < length(obj_classes); i++) {
            if (strcmp(CHAR(STRING_ELT(obj_classes, i)), class_name) == 0) {
                return TRUE;
            }
        }
    }

    Rprintf("Variable '%s' doesn't have class '%s'\n", var_name, class_name);
    return FALSE;
}

/*
 *   Checks whether variable has specified attribute and its content matches type description
 *
 *   var_obj: variable object to be checked
 *   var_type: list object with variable type description
 *   attr_name: variable name for logging purposes
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if check passes, FALSE otherwise.
 */
int check_variable_attribute(const SEXP var_obj, const SEXP var_type, const char *attr_name, const char *var_name) {

    int attr_pos = get_position_by_name(var_type, attr_name);

    if (attr_pos == -1) {
        return TRUE;
    }

    SEXP expected_attr = VECTOR_ELT(var_type, attr_pos);

    if (isNull(expected_attr)) {
        return TRUE;
    }

    SEXP attr_sexp = getAttrib(var_obj, install(attr_name));

    if (isNull(attr_sexp)) {
        Rprintf("Variable '%s' doesn't have attribute '%s'\n", var_name, attr_name);
        return FALSE;
    }

    if (TYPEOF(attr_sexp) != STRSXP) {
        Rprintf("Variable '%s' doesn't have attribute '%s'\n", var_name, attr_name);
        return FALSE;
    }

    if (length(attr_sexp) != length(expected_attr)) {
        Rprintf("Variable '%s' attribute '%s' length doesn't match expected attributes length\n", var_name, attr_name);
        return FALSE;
    }

    for(int i = 0; i < length(attr_sexp); i++) {
        if (strcmp(CHAR(STRING_ELT(attr_sexp, i)), CHAR(STRING_ELT(expected_attr, i))) != 0) {
            Rprintf("Variable '%s' attribute '%s' doesn't match expected attribute values\n", var_name, attr_name);
            return FALSE;
        }
    }

    return TRUE;
}

/*
 *   Checks whether variable vector has NA values
 *
 *   var_obj: variable object to be checked
 *   var_type: list object with variable type description
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if check passes, FALSE otherwise.
 */
int check_variable_NAs(const SEXP var_obj, const SEXP var_type, const char *var_name) {
    
    int allow_na_pos = get_position_by_name(var_type, "allow.na");

    if (allow_na_pos != -1) {

        int allow_na = asInteger(VECTOR_ELT(var_type, allow_na_pos));

        if ((allow_na != NA_INTEGER) && (allow_na == FALSE)) {
            if (has_NAs(var_obj)) {
                Rprintf("Variable '%s' has NA elements which are not allowed\n", var_name);
                return FALSE;
            }
        }
    }

    return TRUE;
}

/*
 *   Checks whether element value is NA
 *
 *   var_obj: value to be checked
 *
 *   returns: TRUE if check passes, FALSE otherwise.
 */
int has_NAs(const SEXP var_obj) {

    int type_id = TYPEOF(var_obj);
    int * pt_int;
    double * pt_double;

    switch (type_id)
    {
        case LGLSXP:
            pt_int = LOGICAL(var_obj);
            for (int i = 0; i < length(var_obj); i++)
            {
                if (pt_int[i] == NA_LOGICAL)
                {
                    return TRUE;
                }
            }
            break;
        case INTSXP:
            pt_int = INTEGER(var_obj);
            for (int i = 0; i < length(var_obj); i++)
            {
                if (pt_int[i] == NA_INTEGER)
                {
                    return TRUE;
                }
            }
            break;
        case REALSXP:
            pt_double = REAL(var_obj);
            for (int i = 0; i < length(var_obj); i++)
            {
                if (ISNA(pt_double[i]))
                {
                    return TRUE;
                }
            }
            break;
        case STRSXP:
            for (int i = 0; i < length(var_obj); i++)
            {
                if (STRING_ELT(var_obj, i) == NA_STRING)
                {
                    return TRUE;
                }
            }
            break;
        default:
            Rf_error("Unexpected type in has_NAs(%d)\n", type_id);
            return -1;
    }

    return FALSE;
}

/*
 *   Searches for given item by name in list
 *
 *   list: list to be searched
 *   name: item name
 *
 *   returns: item position number in list. Error if item is not found.
 */
int get_position_safe(const SEXP list, const char *name) {
    int pos = get_position_by_name(list, name);

    if (pos == -1)
    {
        Rf_error("Obligatory field `%s` was not found in the description!", name);
    }

    return pos;
}

/*
 *   Searches for given item by name in list
 *
 *   list: list to be searched
 *   name: item name
 *
 *   returns: item position number in list. -1 if item is not found.
 */
int get_position_by_name(const SEXP list, const char *item_name) {

    SEXP names = getAttrib(list, R_NamesSymbol);

    if (!isNull(names))
    {
        for (int i = 0; i < length(names); i++)
        {
            if (strcmp(CHAR(STRING_ELT(names, i)), item_name) == 0)
            {
                return i;
            }
        }
    }

    return -1; // unexpected value
}

/*
 *   Checks list variable - performs recursive check of included elements
 *
 *   var_obj: variable object to be checked
 *   var_type: list object with variable type description
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if check passes, FALSE otherwise.
 */
int check_variable_list(const SEXP var_obj, const SEXP var_type, const char *var_name) {

    int res = TRUE;

    SEXP var_items_names = getAttrib(var_obj, R_NamesSymbol);

    SEXP items_obj = VECTOR_ELT(var_type, get_position_safe(var_type, "items"));
    SEXP expected_items_names  = getAttrib(items_obj, R_NamesSymbol);

    if (isNull(var_items_names)) Rf_error("List variable must have names");
    if (isNull(expected_items_names)) Rf_error("List type must have names");

    for (int i = 0; i < length(expected_items_names); i++) {

        int found = FALSE;

        const char * type_item_name = CHAR(STRING_ELT(expected_items_names, i));

        for(int j = 0; j < length(var_items_names); j++) {
            if (strcmp(type_item_name, CHAR(STRING_ELT(var_items_names, j))) == 0) {
                found = TRUE;

                SEXP type_item = VECTOR_ELT(items_obj, i);
                SEXP var_item = VECTOR_ELT(var_obj, j);

                int item_res = check_variable(var_item, type_item, type_item_name);

                if (item_res != 1) {
                    res = FALSE;
                }

                break;
            }
        }

        if (!found) {
            Rprintf("Item '%s' is not found in list variable '%s'\n", type_item_name, var_name);
            res = FALSE;
        }
    }

    return res;
}

/*
 *   Checks environment variable - performs recursive check of included elements
 *
 *   var_obj: variable object to be checked
 *   var_type: list object with variable type description
 *   var_name: variable name for logging purposes
 *
 *   returns: TRUE if check passes, FALSE otherwise.
 */
int check_variable_envir(const SEXP var_obj, const SEXP var_type, const char *var_name) {

    int res = TRUE;

    SEXP items_obj = VECTOR_ELT(var_type, get_position_safe(var_type, "items"));
    SEXP expected_items_names  = getAttrib(items_obj, R_NamesSymbol);

    for (int i = 0; i < length(items_obj); i++)
    {
        const char * ivar_name = CHAR(STRING_ELT(expected_items_names, i));
        SEXP type_item = VECTOR_ELT(items_obj, i);
        SEXP sym_sexp = Rf_installChar(STRING_ELT(expected_items_names, i));

        SEXP obj = Rf_findVarInFrame(var_obj, sym_sexp);

        if (obj == R_UnboundValue) {
            Rprintf("Item '%s' is not found in environment variable '%s'\n", ivar_name, var_name);
            res = FALSE;
            continue;
        }

        int item_res = check_variable(obj, type_item, ivar_name);

        if (item_res != 1) {
            res = FALSE;
        }
    }

    return(res);
}

/*
 *   Returns the square of the largest of its two input values
 *
 *   var_obj: variable object to be checked
 *   var_type: list object with variable type description
 *   var_name: variable name for logging purposes
 *
 *   returns: the square of the larger of n1 and n2 
 */
int check_variable_dims(const SEXP var_obj, const SEXP var_type, const char *var_name) {


    int len_pos = get_position_by_name(var_type, "len");

    if (len_pos != -1) {
        int expected_len = asInteger(VECTOR_ELT(var_type, len_pos));
        int var_len = length(var_obj);

        if ((expected_len != NA_INTEGER) && (var_len != expected_len)) {
            Rprintf("Length doesn't match for variable '%s': expected length is %d and actual length is %d\n",
                var_name, expected_len, var_len);
            return FALSE;
        }
    }

    int nrow_pos = get_position_by_name(var_type, "nrow");

    if (nrow_pos != -1) {
        int expected_len = asInteger(VECTOR_ELT(var_type, nrow_pos));
        int var_len = Rf_nrows(var_obj);

        if ((expected_len != NA_INTEGER) && (var_len != expected_len)) {
            Rprintf("Rows count doesn't equal expected value for variable '%s': expected length %d and actual length %d\n",
                var_name, expected_len, var_len);
            return FALSE;
        }
    }

    int ncol_pos = get_position_by_name(var_type, "ncol");

    if (ncol_pos != -1) {
        int expected_len = asInteger(VECTOR_ELT(var_type, ncol_pos));
        int var_len = Rf_ncols(var_obj);

        if ((expected_len != NA_INTEGER) && (var_len != expected_len)) {
            Rprintf("Cols count doesn't equal expected value for variable '%s': expected length %d and actual length %d\n",
                var_name, expected_len, var_len);
            return FALSE;
        }
    }

    return TRUE;
}
