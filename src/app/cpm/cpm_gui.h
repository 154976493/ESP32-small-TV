#ifndef APP_CPM_GUI_H
#define APP_CPM_GUI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

#define ANIEND                      \
    while (lv_anim_count_running()) \
        lv_task_handler(); //等待动画完成
    void cpm_gui_init(void);
    void cpm_display1_init(void);
    void cpm_display2_init(void);
    void display1_cpm(const char *CPU1,const char *CPU2,const char *CPU3,const char *GPU1,const char *GPU2);
    void cpm_obj_del(void);
    void cpm_gui_del(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
    extern const lv_img_dsc_t app_cpm;
    

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif