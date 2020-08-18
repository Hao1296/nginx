
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_CONFIG_H_INCLUDED_
#define _NGX_HTTP_CONFIG_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/*
  HTTPģ���Ӧ��������ṹ��.
  HTTPģ�����������ֻ��һ��ngx_http_conf_ctx_tʵ��,
  �ߺ�����ģ����������
  (����ģ��һ����ngx_cycle_t->conf_ctx��Ӧ��洢ָ��ָ�������ָ��,
  ʹ��ÿһ��ģ�鶼��ngx_cycle_t->conf_ctx��һ��������ṹ��).

  ����, �����ÿ��һ��httpģ������ýṹ��.
  �Ǻ���ģ���ڵ�http������ģ��(ngx_http_core_module)��Ӧ��������ṹ����ngx_http_core_main_conf_t
  (��ngx_http_core_module.h)
*/
typedef struct {
    /*
      ָ������,ÿһ��ָ���ӦHTTPģ���main�������ýṹ��.
      
      ÿһ��Ҳ������ngx_http_conf_ctx_tָ��,
      ��ʾ��http���ڶ�д����Щ��http��server��location����ص�����
    */
    void        **main_conf;
    /*
      ָ������,ÿһ��ָ���ӦHTTPģ���server�������ýṹ��.
     
      ÿһ��Ҳ������ngx_http_conf_ctx_tָ��,
      ��ʾ��server���ڶ�д����Щ��server��location����ص�����,
      ͬʱָʾ�˸�ģ����http���¶�Ӧ�����ýṹ�����ĸ�.
    */
    void        **srv_conf;
    /*
      ָ������,ÿһ��ָ���ӦHTTPģ���location�������ýṹ��.
     
      ÿһ��Ҳ������ngx_http_conf_ctx_tָ��,
      ��ʾ��location���ڶ�д����Щ��location����ص�����,
      ͬʱָʾ�˸�ģ����http���server���¶�Ӧ�����ýṹ��ֱ����ĸ�.
    */
    void        **loc_conf;
} ngx_http_conf_ctx_t;


typedef struct {
    /*
      �ڽ���http{...}��������ǰ������
    */
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);
    /*
      http{...}�����������������Ϻ�����
    */
    ngx_int_t   (*postconfiguration)(ngx_conf_t *cf);

    void       *(*create_main_conf)(ngx_conf_t *cf);
    char       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void       *(*create_srv_conf)(ngx_conf_t *cf);
    char       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

    void       *(*create_loc_conf)(ngx_conf_t *cf);
    char       *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;


#define NGX_HTTP_MODULE           0x50545448   /* "HTTP" */

#define NGX_HTTP_MAIN_CONF        0x02000000
#define NGX_HTTP_SRV_CONF         0x04000000
#define NGX_HTTP_LOC_CONF         0x08000000
#define NGX_HTTP_UPS_CONF         0x10000000
#define NGX_HTTP_SIF_CONF         0x20000000
#define NGX_HTTP_LIF_CONF         0x40000000
#define NGX_HTTP_LMT_CONF         0x80000000


#define NGX_HTTP_MAIN_CONF_OFFSET  offsetof(ngx_http_conf_ctx_t, main_conf)
#define NGX_HTTP_SRV_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, srv_conf)
#define NGX_HTTP_LOC_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, loc_conf)


#define ngx_http_get_module_main_conf(r, module)                             \
    (r)->main_conf[module.ctx_index]
#define ngx_http_get_module_srv_conf(r, module)  (r)->srv_conf[module.ctx_index]
#define ngx_http_get_module_loc_conf(r, module)  (r)->loc_conf[module.ctx_index]


#define ngx_http_conf_get_module_main_conf(cf, module)                        \
    ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[module.ctx_index]
#define ngx_http_conf_get_module_srv_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->srv_conf[module.ctx_index]
#define ngx_http_conf_get_module_loc_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->loc_conf[module.ctx_index]

#define ngx_http_cycle_get_module_main_conf(cycle, module)                    \
    (cycle->conf_ctx[ngx_http_module.index] ?                                 \
        ((ngx_http_conf_ctx_t *) cycle->conf_ctx[ngx_http_module.index])      \
            ->main_conf[module.ctx_index]:                                    \
        NULL)


#endif /* _NGX_HTTP_CONFIG_H_INCLUDED_ */
