
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
  HTTP模块对应的配置项结构体.
  HTTP模块这个大类中只有一个ngx_http_conf_ctx_t实例,
  者和其他模块有所区别
  (其他模块一般在ngx_cycle_t->conf_ctx对应项存储指向指针数组的指针,
  使得每一个模块都在ngx_cycle_t->conf_ctx有一个配置项结构体).

  另外, 这个是每个一般http模块的配置结构体.
  非核心模块内的http管理者模块(ngx_http_core_module)对应的配置项结构体是ngx_http_core_main_conf_t
  (见ngx_http_core_module.h)
*/
typedef struct {
    /*
      指针数组,每一项指向对应HTTP模块的main级别配置结构体.
      
      每一项也都还是ngx_http_conf_ctx_t指针,
      表示在http块内都写了哪些和http、server、location块相关的配置
    */
    void        **main_conf;
    /*
      指针数组,每一项指向对应HTTP模块的server级别配置结构体.
     
      每一项也都还是ngx_http_conf_ctx_t指针,
      表示在server块内都写了哪些和server、location块相关的配置,
      同时指示了该模块在http块下对应的配置结构体是哪个.
    */
    void        **srv_conf;
    /*
      指针数组,每一项指向对应HTTP模块的location级别配置结构体.
     
      每一项也都还是ngx_http_conf_ctx_t指针,
      表示在location块内都写了哪些和location块相关的配置,
      同时指示了该模块在http块和server块下对应的配置结构体分别是哪个.
    */
    void        **loc_conf;
} ngx_http_conf_ctx_t;


typedef struct {
    /*
      在解析http{...}内配置项前被调用
    */
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);
    /*
      http{...}内所有配置项解析完毕后会调用
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
