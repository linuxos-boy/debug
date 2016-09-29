/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  linuxo  657764270@qq.com                                    |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_EDebug.h"

#include "zend_extensions.h"

static int le_EDebug;
static int p;
static edebug_return_struct re_struct;
static void (*edebug_copy_execute)(zend_op_array *op_array TSRMLS_DC);
static void re_struct_display(edebug_return_struct *re_struct);
static char *edebug_display_zval(zval *value);
ZEND_API void edebug_execute(zend_op_array *op_array TSRMLS_DC);
ZEND_API void edebug_execute_main(edebug_return_struct *re_struct,zend_execute_data *execute_data, zend_op_array *op_array TSRMLS_DC);
const zend_function_entry EDebug_functions[] = {
	PHP_FE(confirm_EDebug_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in EDebug_functions[] */
};

zend_module_entry EDebug_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"EDebug",
	EDebug_functions,
	PHP_MINIT(EDebug),
	PHP_MSHUTDOWN(EDebug),
	PHP_RINIT(EDebug),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(EDebug),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(EDebug),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_EDEBUG_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_EDEBUG
ZEND_GET_MODULE(EDebug)
#endif

PHP_MINIT_FUNCTION(EDebug)
{
    edebug_copy_execute = zend_execute;
    zend_execute = edebug_execute; //zend_execute  自动执行的
	return SUCCESS;
}
PHP_MSHUTDOWN_FUNCTION(EDebug)
{
    zend_execute = edebug_copy_execute;
	return SUCCESS;
}

PHP_RINIT_FUNCTION(EDebug)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(EDebug)
{
    /* php_printf("%d",sizeof(re_struct)); */
    edebug_return_struct *rs = &re_struct;
    re_struct_display(rs);
	return SUCCESS;
}

PHP_MINFO_FUNCTION(EDebug)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "EDebug support", "enabled");
	php_info_print_table_end();

}

PHP_FUNCTION(confirm_EDebug_compiled)
{
}

ZEND_API void edebug_execute(zend_op_array *op_array TSRMLS_DC)
{
    edebug_execute_main(&re_struct,EG(current_execute_data), op_array TSRMLS_CC);
}

ZEND_API void edebug_execute_main(edebug_return_struct *re_struct,zend_execute_data *execute_data, zend_op_array *op_array TSRMLS_DC)
{
    zval **args;
    zval *retval = NULL;
    zend_bool dobailout = 0;
    int i;
    zend_function *zf;
    zend_execute_data *exdata = execute_data;
    zf = (zend_function *) op_array;
    args = NULL;
    if (zf->common.function_name) {
        if (exdata && exdata->object) {
            if (zf->common.scope) {
                re_struct->class[p] = (char *)zf->common.scope->name;
            }
        } else if (zf->common.scope) {
            re_struct->class[p] = (char *)zf->common.scope->name;
        }else{
            re_struct->function[p] = (char *)zf->common.function_name;
        }
        if (exdata && exdata->function_state.arguments) {
            re_struct->arg_count[p] = (int)(zend_uintptr_t) *(exdata->function_state.arguments);
            args = (zval **)(exdata->function_state.arguments - re_struct->arg_count[p]);
        }
        if (re_struct->arg_count[p] > 0) {
            for (i = 0; i < re_struct->arg_count[p]; i++) {
                re_struct->args[p][i] = edebug_display_zval(args[i]);
            }
        }
    }

    if (exdata && exdata->opline) {
        re_struct->lineno[p] = exdata->opline->lineno;
    } else if (exdata && exdata->prev_execute_data && exdata->prev_execute_data->opline) {
        re_struct->lineno[p] = exdata->prev_execute_data->opline->lineno;
    } else if (op_array && op_array->opcodes) {
        re_struct->lineno[p] = op_array->opcodes->lineno;
    } else {
        re_struct->lineno[p] = 0;
    }

    if (exdata && exdata->op_array) {
        re_struct->filename[p] = (char *)exdata->op_array->filename;
    } else if (exdata && exdata->prev_execute_data && exdata->prev_execute_data->op_array) {
        re_struct->filename[p] = (char *)exdata->prev_execute_data->op_array->filename;
    } else if (op_array) {
        re_struct->filename[p] = (char *)op_array->filename;
    } else {
        re_struct->filename[p] = NULL;
    }
    if (EG(return_value_ptr_ptr) == NULL) {
        EG(return_value_ptr_ptr) = &retval;
    }
    int64_t  start_time = edebug_time_usec();
    int64_t start_mem =  zend_memory_usage(0 TSRMLS_CC);
    zend_try {
        edebug_copy_execute(op_array TSRMLS_CC);
    }zend_catch{
        dobailout = 1;
    }zend_end_try();
    /* php_printf("\n%.6f\n",(end_time-start_time)/1000000); */
    re_struct->wall_time_t[p] = start_time;
    re_struct->mem[p] = start_mem;
    re_struct->pmem[p] =  zend_memory_peak_usage(0 TSRMLS_CC);
    if(p == 0){
        re_struct->wall_time[p] = 0;
    }else{
        re_struct->wall_time[p-1] = start_time - re_struct->wall_time_t[p-1];
    }
    if (!dobailout) {
        if (*EG(return_value_ptr_ptr)) {
            retval = *EG(return_value_ptr_ptr);
        }
        if (retval) {
            re_struct->retval[p] = edebug_display_zval(retval);
        }
    }
    if (dobailout) {
        zend_bailout();
    }
    re_struct->p = p;
    if(re_struct->filename[p] != NULL){
        p++;
    }
}

static void re_struct_display(edebug_return_struct *re_struct){
  int i,j;
  php_printf("%s",EDEBUG_TRACE);
  for (i = 0; i < re_struct->p; i++) {
      if(re_struct->filename[i] != NULL){
          php_printf("<li style=\"border-bottom:1px solid #EEE;font-size:14px;padding:0 12px\">");
          php_printf("%d.",i+1);
          if(re_struct->function[i] != NULL){
              php_printf("<span style=\"color:red\">function</span>:%s&nbsp;&nbsp;",re_struct->function[i]);
          }else if(re_struct->class[i] != NULL){
              php_printf("<span style=\"color:red\">class</span>:%s&nbsp;&nbsp;",re_struct->class[i]);
          }
          php_printf("<span style=\"color:red\">filename</span>:%s&nbsp;&nbsp;",re_struct->filename[i]);
          if(re_struct->arg_count[i] > 0){
              php_printf("<span style=\"color:red\">args</span>:(");
              for (j = 0; j < re_struct->arg_count[i]; j++) {
                  php_printf("%s",re_struct->args[i][j]);
                  if(re_struct->arg_count[i] > j+1){
                    php_printf(",");
                  }
              }
              php_printf(")&nbsp;&nbsp;");
          }
          php_printf("<span style=\"color:red\">lineno</span>:%d&nbsp;&nbsp;",re_struct->lineno[i]);
          if(re_struct->wall_time[i] >= 0){
              php_printf("<span style=\"color:red\">wall_time</span>:%.6fs&nbsp;&nbsp;",(float)(re_struct->wall_time[i])/1000000);
          }
          php_printf("<span style=\"color:red\">mem</span>:%d&nbsp;&nbsp;",re_struct->mem[i]);
          php_printf("<span style=\"color:red\">pmem</span>:%d&nbsp;&nbsp;",re_struct->pmem[i]);
          if(re_struct->retval[i] != NULL){
              php_printf("<span style=\"color:red\">return</span>:%s&nbsp;&nbsp;",re_struct->retval[i]);
          }
          php_printf("</li>");
      }
  }
  php_printf("%s",EDEBUG_TRACE_END);
  php_printf("%s",EDEBUG_JS);
  memset(re_struct, 0, sizeof(edebug_return_struct));
  p = 0;
}

static char *makenew(const char *init)
{
    char *re_val;
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    re_val = malloc(initlen+1);
    memcpy(re_val, init, initlen);
    re_val[initlen] = '\0';
    return (char*)re_val;
}

static char *edebug_display_zval(zval *value){

    static char val[256];
    switch (Z_TYPE_P(value)) {
        case IS_NULL:
            snprintf(val, sizeof(val), "%s", "null");
            return makenew(val);
        case IS_BOOL:
            if (Z_LVAL_P(value)) {
                snprintf(val, sizeof(val), "%s", "ture");
                return makenew(val);
            } else {
                snprintf(val, sizeof(val), "%s", "false");
                return makenew(val);
            }
        case IS_LONG:
            snprintf(val, sizeof(val), "%ld", Z_LVAL_P(value));
            return makenew(val);
        case IS_DOUBLE:
            snprintf(val, sizeof(val), "%.*G", (int) EG(precision), Z_DVAL_P(value));
            return makenew(val);
        case IS_STRING:
            snprintf(val, sizeof(val), "%s", Z_STRVAL_P(value));
            return makenew(val);
        case IS_ARRAY:
            snprintf(val,sizeof(val), "array(%d)", zend_hash_num_elements(Z_ARRVAL_P(value)));
            return makenew(val);
        case IS_OBJECT:
            snprintf(val, sizeof(val), "%s", "object");
            return makenew(val);
        default:
            snprintf(val, sizeof(val), "%s", "unknown");
            return makenew(val);
  }
}
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
