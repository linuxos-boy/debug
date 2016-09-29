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
  | Author:linuxo  657764270@qq.com                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_EDEBUG_H
#define PHP_EDEBUG_H

#include <stdint.h>
#include <sys/resource.h>
#include <sys/time.h>

#define PT_USEC_PER_SEC 1000000l
extern zend_module_entry EDebug_module_entry;
#define phpext_EDebug_ptr &EDebug_module_entry

#define PHP_EDEBUG_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_EDEBUG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EDEBUG_API __attribute__ ((visibility("default")))
#else
#	define PHP_EDEBUG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(EDebug);
PHP_MSHUTDOWN_FUNCTION(EDebug);
PHP_RINIT_FUNCTION(EDebug);
PHP_RSHUTDOWN_FUNCTION(EDebug);
PHP_MINFO_FUNCTION(EDebug);

PHP_FUNCTION(confirm_EDebug_compiled);	/* For testing, remove later. */


static  long edebug_time_sec()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (long) tv.tv_sec;
    // return (long) tv.tv_usec;
}

static  long edebug_time_usec()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (long) tv.tv_sec * PT_USEC_PER_SEC + (long) tv.tv_usec;
}

static  long edebug_cputime_usec()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return (long) usage.ru_utime.tv_sec * PT_USEC_PER_SEC +
           (long) usage.ru_utime.tv_usec +
           (long) usage.ru_stime.tv_sec * PT_USEC_PER_SEC +
           (long) usage.ru_stime.tv_usec;
}

#define EDEBUG_LOGO_BASE64    "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"
#define EDEBUG_LOGO_DATA      "data:image/png;base64," EDEBUG_LOGO_BASE64

#define EDEBUG_TRACE "<div id=\"edebug_page_trace\" style=\"position: fixed;bottom:0;right:0;font-size:14px;width:100%;z-index: 999999;color: #000;text-align:left;font-family:'微软雅黑';\">" \
                     "<div id=\"edebug_page_trace_tab\" style=\"display: none;background:white;margin:0;height: 250px;\">" \
                     "<div id=\"edebug_page_trace_tab_tit\" style=\"height:30px;padding: 6px 12px 0;border-bottom:1px solid #ececec;border-top:1px solid #ececec;font-size:16px\">" \
                         "<span style=\"color:#000;padding-right:12px;height:30px;line-height: 30px;display:inline-block;margin-right:3px;cursor: pointer;font-weight:700\">EDEBUG</span>" \
                     "</div>" \
                     "<div id=\"edebug_page_trace_tab_cont\" style=\"overflow:auto;height:212px;padding: 0; line-height: 24px\">" \
                     "<div style=\"display:none;\"><ol style=\"padding: 0; margin:0\">"

#define EDEBUG_TRACE_END "</ol></div></div></div>" \
                         "<div id=\"edebug_page_trace_close\" style=\"display:none;text-align:right;height:15px;position:absolute;top:10px;right:12px;cursor: pointer;\"><img style=\"vertical-align:top;\" src=\"data:image/gif;base64,R0lGODlhDwAPAJEAAAAAAAMDA////wAAACH/C1hNUCBEYXRhWE1QPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgNS4wLWMwNjAgNjEuMTM0Nzc3LCAyMDEwLzAyLzEyLTE3OjMyOjAwICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIiB4bWxuczpzdFJlZj0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlUmVmIyIgeG1wOkNyZWF0b3JUb29sPSJBZG9iZSBQaG90b3Nob3AgQ1M1IFdpbmRvd3MiIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6MUQxMjc1MUJCQUJDMTFFMTk0OUVGRjc3QzU4RURFNkEiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6MUQxMjc1MUNCQUJDMTFFMTk0OUVGRjc3QzU4RURFNkEiPiA8eG1wTU06RGVyaXZlZEZyb20gc3RSZWY6aW5zdGFuY2VJRD0ieG1wLmlpZDoxRDEyNzUxOUJBQkMxMUUxOTQ5RUZGNzdDNThFREU2QSIgc3RSZWY6ZG9jdW1lbnRJRD0ieG1wLmRpZDoxRDEyNzUxQUJBQkMxMUUxOTQ5RUZGNzdDNThFREU2QSIvPiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/PgH//v38+/r5+Pf29fTz8vHw7+7t7Ovq6ejn5uXk4+Lh4N/e3dzb2tnY19bV1NPS0dDPzs3My8rJyMfGxcTDwsHAv769vLu6ubi3trW0s7KxsK+urayrqqmop6alpKOioaCfnp2cm5qZmJeWlZSTkpGQj46NjIuKiYiHhoWEg4KBgH9+fXx7enl4d3Z1dHNycXBvbm1sa2ppaGdmZWRjYmFgX15dXFtaWVhXVlVUU1JRUE9OTUxLSklIR0ZFRENCQUA/Pj08Ozo5ODc2NTQzMjEwLy4tLCsqKSgnJiUkIyIhIB8eHRwbGhkYFxYVFBMSERAPDg0MCwoJCAcGBQQDAgEAACH5BAAAAAAALAAAAAAPAA8AAAIdjI6JZqotoJPR1fnsgRR3C2jZl3Ai9aWZZooV+RQAOw==\" /></div></div><div id=\"edebug_page_trace_open\" style=\"height:30px;float:right;text-align: right;overflow:hidden;position:fixed;bottom:0;right:0;color:#000;line-height:30px;cursor:pointer;\"><div style=\"background:#232323;color:#FFF;padding:0 6px;float:right;line-height:30px;font-size:14px\"></div><img width=\"30\"  title=\"ShowPageTrace\" src=\"" EDEBUG_LOGO_DATA "\"></div>\n"

#define EDEBUG_JS   "<script type=\"text/javascript\">\n" \
                     "(function(){\n" \
                     "var tab_tit  = document.getElementById('edebug_page_trace_tab_tit').getElementsByTagName('span');\n" \
                     "var tab_cont = document.getElementById('edebug_page_trace_tab_cont').getElementsByTagName('div');\n" \
                     "var open     = document.getElementById('edebug_page_trace_open');\n" \
                     "var close    = document.getElementById('edebug_page_trace_close').childNodes[0];\n" \
                     "var trace    = document.getElementById('edebug_page_trace_tab'); \n" \
                     "var cookie   = document.cookie.match(/edebugphp_show_page_trace=(\\d\\|\\d)/); \n" \
                     "var history  = (cookie && typeof cookie[1] != 'undefined' && cookie[1].split('|')) || [0,0];\n" \
                     "open.onclick = function(){\n" \
                     "trace.style.display = 'block';\n" \
                     "this.style.display = 'none';\n" \
                     "close.parentNode.style.display = 'block';\n" \
                     "history[0] = 1;\n" \
                     "document.cookie = 'edebugphp_show_page_trace='+history.join('|')\n" \
                     "}\n" \
                     "close.onclick = function(){\n" \
                     "trace.style.display = 'none';\n" \
                     "this.parentNode.style.display = 'none';\n" \
                     "open.style.display = 'block';\n" \
                     "history[0] = 0;\n" \
                     "document.cookie = 'edebugphp_show_page_trace='+history.join('|')\n" \
                     "}\n" \
                     "for(var i = 0; i < tab_tit.length; i++){\n" \
                     "tab_tit[i].onclick = (function(i){\n" \
                     "return function(){\n" \
                     "for(var j = 0; j < tab_cont.length; j++){\n" \
                     "tab_cont[j].style.display = 'none';\n" \
                     "tab_tit[j].style.color = '#999';\n" \
                     "}\n" \
                     "tab_cont[i].style.display = 'block';\n" \
                     "tab_tit[i].style.color = '#000';\n" \
                     "history[1] = i;\n" \
                     "document.cookie = 'edebugphp_show_page_trace='+history.join('|')\n" \
                     "}\n" \
                     "})(i)\n" \
                     "}\n" \
                     "parseInt(history[0]) && open.click();\n" \
                     "(tab_tit[history[1]] || tab_tit[0]).click();\n" \
                     "})();\n" \
                     "</script>\n"


typedef struct {
    uint8_t functype[256];
    uint8_t p;

    uint32_t lineno[256];            /* entry lineno */
    char *filename[256];               /* entry filename */
    char *class[256];                  /* class name */
    char *function[256];               /* function name */

    uint32_t arg_count[256];         /* arguments number */
    char *args[256][256];                  /* arguments represent string */
    char *retval[256];                 /* return value represent string */

    int64_t wall_time[256];      /* wall time */
    int64_t wall_time_t[256];      /* wall time */
    int64_t mem[256];            /* memory usage */
    int64_t pmem[256];        /* memory peak */

} edebug_return_struct;

#ifdef ZTS
#define EDEBUG_G(v) TSRMG(EDebug_globals_id, zend_EDebug_globals *, v)
#else
#define EDEBUG_G(v) (EDebug_globals.v)
#endif

#endif	/* PHP_EDEBUG_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
