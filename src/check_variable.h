#ifndef CHECK_VARIABLE_H_
#define CHECK_VARIABLE_H_

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

SEXP R_check_input_params(const SEXP params, const SEXP param_types);
SEXP R_check_variable(const SEXP var_obj, const SEXP var_type, const SEXP var_name);
SEXP R_get_type_name(const SEXP var_type_id);

int check_variable(const SEXP var_obj, const SEXP var_type, const char *var_name);
int check_variable_attribute(const SEXP var_obj, const SEXP var_type, const char *attr_name, const char *var_name);
int check_variable_type(const int type_id, const SEXP var_obj, const char *var_name);
int check_variable_class(const SEXP var_obj, const char *class_name, const char *var_name);
int check_variable_dims(const SEXP var_obj, const SEXP var_type, const char *var_name);
int check_variable_NAs(const SEXP var_obj, const SEXP var_type, const char *var_name);
int check_variable_list(const SEXP var_obj, const SEXP var_type, const char *var_name);
int check_variable_envir(const SEXP var_obj, const SEXP var_type, const char *var_name);

int get_position_safe(const SEXP list, const char *item_name);
int get_position_by_name(const SEXP list, const char *item_name);
int has_NAs(const SEXP var_obj);

const char *get_r_object_type_name(const int r_type_id);
const char *get_var_type_name(const int var_type_id);

#endif