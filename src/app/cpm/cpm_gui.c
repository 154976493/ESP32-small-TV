#include "cpm_gui.h"

#include "driver/lv_port_indev.h"
#include "lvgl.h"
LV_FONT_DECLARE(cpmGUI1);
LV_FONT_DECLARE(font_unicode1);
static lv_obj_t * display1=NULL;
static lv_obj_t * display2=NULL;

//屏幕1组件
static lv_obj_t * display1_bar_cpu1=NULL;
static lv_obj_t * display1_bar_cpu2=NULL;
static lv_obj_t * display1_bar_cpu3=NULL;
static lv_obj_t * display1_bar_gpu1=NULL;
static lv_obj_t * display1_bar_gpu2=NULL;

static lv_obj_t * display1_CPU=NULL;//CPU占用率标签(屏幕1)
static lv_obj_t * display1_CPUlabel1=NULL;//CPU占用率标签(屏幕1)
static lv_obj_t * display1_CPUlabel2=NULL;//CPU频率标签(屏幕1)
static lv_obj_t * display1_CPUlabel3=NULL;//CPU温度标签(屏幕1)
static lv_obj_t * display1_GPU=NULL;//CPU占用率标签(屏幕1)
static lv_obj_t * display1_GPUlabel1=NULL;//GPU占用率标签(屏幕1)
static lv_obj_t * display1_GPUlabel2=NULL;//GPU温度标签(屏幕1)

static lv_obj_t * display1_CPUlabel1_show=NULL;//CPU占用率标签(屏幕2)
static lv_obj_t * display1_CPUlabel2_show=NULL;//CPU频率标签(屏幕2)
static lv_obj_t * display1_CPUlabel3_show=NULL;//CPU温度标签(屏幕2)
static lv_obj_t * display1_GPUlabel1_show=NULL;//GPU占用率标签(屏幕2)
static lv_obj_t * display1_GPUlabel2_show=NULL;//GPU温度标签(屏幕2)

//屏幕2组件
static lv_obj_t *img=NULL;//图片句柄
static lv_obj_t * display2_CPUlabel1=NULL;//CPU占用率标签(屏幕2)
static lv_obj_t * display2_CPUlabel2=NULL;//CPU频率标签(屏幕2)
static lv_obj_t * display2_CPUlabel3=NULL;//CPU温度标签(屏幕2)
static lv_obj_t * display2_GPUlabel1=NULL;//GPU占用率标签(屏幕2)
static lv_obj_t * display2_GPUlabel2=NULL;//GPU温度标签(屏幕2)

static lv_style_t display;//屏幕风格
static lv_style_t d2_CPUlabel1;//标签风格
static lv_style_t bar_style;//进度条风格



void cpm_gui_init(void)
{
    
    lv_style_init(&display);
    lv_style_set_bg_color(&display, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&display, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&display, LV_STATE_FOCUSED, LV_COLOR_BLACK);
    lv_style_set_bg_color(&display, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

    lv_style_init(&d2_CPUlabel1);
    lv_style_set_text_opa(&d2_CPUlabel1,LV_STATE_DEFAULT,LV_OPA_COVER);//设置文本不透明度
    lv_style_set_text_color(&d2_CPUlabel1,LV_STATE_DEFAULT,LV_COLOR_WHITE);//设置文本颜色
    lv_style_set_text_font(&d2_CPUlabel1,LV_STATE_DEFAULT,&font_unicode1);//设置文本字体

    lv_style_init(&bar_style);
    lv_style_set_bg_color(&bar_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);//设置背景颜色
    lv_style_set_border_width(&bar_style, LV_STATE_DEFAULT, 2);//设置边框宽度
    lv_style_set_border_color(&bar_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);//设置边框颜色
    lv_style_set_pad_top(&bar_style, LV_STATE_DEFAULT, 1); //设置顶部填充（指示器到背景四周的距离）
    lv_style_set_pad_bottom(&bar_style, LV_STATE_DEFAULT, 1);//设置底部填充（指示器到背景四周的距离）
    lv_style_set_pad_left(&bar_style, LV_STATE_DEFAULT, 1);//设置左侧填充（指示器到背景四周的距离）
    lv_style_set_pad_right(&bar_style, LV_STATE_DEFAULT, 1);//设置右侧填充（指示器到背景四周的距离）


}

void cpm_display1_init(void)
{
    //cpm_gui_init();
    lv_obj_t *act_obj=lv_scr_act();// 获取当前活动页
    if(act_obj==display1)
        return;
      
    //cpm_gui_del();// 清空对象
    cpm_obj_del();
    lv_obj_clean(act_obj); // 清空此前页面
    display1=lv_obj_create(NULL,NULL);

    display1_CPU=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_CPU,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_CPU, true);
    //lv_obj_align(display1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
    lv_obj_set_pos(display1_CPU,1,5);

    display1_CPUlabel1=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_CPUlabel1,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_CPUlabel1, true);
    lv_obj_align(display1_CPUlabel1, display1_CPU, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 10);
    //lv_obj_set_pos(display1_CPUlabel1,5,17);
    
    display1_CPUlabel2=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_CPUlabel2,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_CPUlabel2, true);
    lv_obj_align(display1_CPUlabel2, display1_CPU, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 40);
    //lv_obj_set_pos(display1_CPUlabel2,5,57);

    display1_CPUlabel3=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_CPUlabel3,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_CPUlabel3, true);
    lv_obj_align(display1_CPUlabel3, display1_CPU, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 70);
    //lv_obj_set_pos(display1_CPUlabel3,5,97);

    display1_GPU=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_GPU,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_GPU, true);
    //lv_obj_align(display1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 155);
    lv_obj_set_pos(display1_GPU,1,125);

    display1_GPUlabel1=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_GPUlabel1,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_GPUlabel1, true);
    lv_obj_align(display1_GPUlabel1, display1_GPU, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 10);
    //lv_obj_set_pos(display1_GPUlabel1,5,167);

    display1_GPUlabel2=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_GPUlabel2,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_GPUlabel2, true);
    lv_obj_align(display1_GPUlabel2, display1_GPU, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 40);
    //lv_obj_set_pos(display1_GPUlabel2,5,207);
    

    display1_bar_cpu1 = lv_bar_create(display1, NULL);
    lv_obj_add_style(display1_bar_cpu1, LV_BAR_TYPE_NORMAL, &bar_style);
    lv_bar_set_range(display1_bar_cpu1, 0, 100); // 设置进度条表示的温度为-20~50
    lv_obj_set_size(display1_bar_cpu1, 110, 16);
    lv_obj_set_style_local_bg_color(display1_bar_cpu1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_bar_set_value(display1_bar_cpu1, 10, LV_ANIM_ON);
    lv_bar_set_anim_time(display1_bar_cpu1,200);
    lv_obj_align(display1_bar_cpu1, display1_CPUlabel1, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);

    display1_bar_cpu2 = lv_bar_create(display1, NULL);
    lv_obj_add_style(display1_bar_cpu2, LV_BAR_TYPE_NORMAL, &bar_style);
    lv_bar_set_range(display1_bar_cpu2, 0, 360); // 设置进度条表示的温度为-20~50
    lv_obj_set_size(display1_bar_cpu2, 110, 16);
    lv_obj_set_style_local_bg_color(display1_bar_cpu2, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_bar_set_value(display1_bar_cpu2, 100, LV_ANIM_ON);
    lv_bar_set_anim_time(display1_bar_cpu2,200);
    lv_obj_align(display1_bar_cpu2, display1_CPUlabel2, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);

    display1_bar_cpu3 = lv_bar_create(display1, NULL);
    lv_obj_add_style(display1_bar_cpu3, LV_BAR_TYPE_NORMAL, &bar_style);
    lv_bar_set_range(display1_bar_cpu3, 0, 100); // 设置进度条表示的温度为-20~50
    lv_obj_set_size(display1_bar_cpu3, 110, 16);
    lv_obj_set_style_local_bg_color(display1_bar_cpu3, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_bar_set_value(display1_bar_cpu3, 10, LV_ANIM_ON);
    lv_bar_set_anim_time(display1_bar_cpu3,200);
    lv_obj_align(display1_bar_cpu3, display1_CPUlabel3, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);

    display1_bar_gpu1 = lv_bar_create(display1, NULL);
    lv_obj_add_style(display1_bar_gpu1, LV_BAR_TYPE_NORMAL, &bar_style);
    lv_bar_set_range(display1_bar_gpu1, 0, 100); // 设置进度条表示的温度为-20~50
    lv_obj_set_size(display1_bar_gpu1, 110, 16);
    lv_obj_set_style_local_bg_color(display1_bar_gpu1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_bar_set_value(display1_bar_gpu1, 10, LV_ANIM_ON);
    lv_bar_set_anim_time(display1_bar_gpu1,200);
    lv_obj_align(display1_bar_gpu1, display1_GPUlabel1, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);

    display1_bar_gpu2 = lv_bar_create(display1, NULL);
    lv_obj_add_style(display1_bar_gpu2, LV_BAR_TYPE_NORMAL, &bar_style);
    lv_bar_set_range(display1_bar_gpu2, 0, 100); // 设置进度条表示的温度为-20~50
    lv_obj_set_size(display1_bar_gpu2, 110, 16);
    lv_obj_set_style_local_bg_color(display1_bar_gpu2, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_bar_set_value(display1_bar_gpu2, 10, LV_ANIM_ON);
    lv_bar_set_anim_time(display1_bar_gpu2,200);
    lv_obj_align(display1_bar_gpu2, display1_GPUlabel2, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);

    display1_CPUlabel1_show=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_CPUlabel1_show,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_CPUlabel1_show, true);
    lv_obj_align(display1_CPUlabel1_show, display1_bar_cpu1, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);

    display1_CPUlabel2_show=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_CPUlabel2_show,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_CPUlabel2_show, true);
    lv_obj_align(display1_CPUlabel2_show, display1_bar_cpu2, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    
    display1_CPUlabel3_show=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_CPUlabel3_show,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_CPUlabel3_show, true);
    lv_obj_align(display1_CPUlabel3_show, display1_bar_cpu3, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);

    display1_GPUlabel1_show=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_GPUlabel1_show,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_GPUlabel1_show, true);
    lv_obj_align(display1_GPUlabel1_show, display1_bar_gpu1, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);

    display1_GPUlabel2_show=lv_label_create(display1,NULL);
    lv_obj_add_style(display1_GPUlabel2_show,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display1_GPUlabel2_show, true);
    lv_obj_align(display1_GPUlabel2_show, display1_bar_gpu2, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);

    lv_obj_add_style(display1,LV_BTN_PART_MAIN,&display);

    lv_label_set_text_fmt(display1_CPU, "GPU:");

    lv_label_set_text_fmt(display1_CPUlabel1, "占用率");

    lv_label_set_text_fmt(display1_CPUlabel2, "频  率");

    lv_label_set_text_fmt(display1_CPUlabel3, "温  度");

    lv_label_set_text_fmt(display1_GPU, "GPU:");

    lv_label_set_text_fmt(display1_GPUlabel1, "占用率");

    lv_label_set_text_fmt(display1_GPUlabel2, "温  度");

    

    lv_label_set_text_fmt(display1_CPUlabel1_show, " ");

    lv_label_set_text_fmt(display1_CPUlabel2_show, " ");

    lv_label_set_text_fmt(display1_CPUlabel3_show, " ");

    lv_label_set_text_fmt(display1_GPUlabel1_show, " ");

    lv_label_set_text_fmt(display1_GPUlabel2_show, " ");


    lv_scr_load(display1);
    
}

void cpm_display2_init(void)
{
    //cpm_gui_init();
    lv_obj_t *act_obj=lv_scr_act();// 获取当前活动页
    if(act_obj==display2)
        return;
    
    cpm_obj_del();         // 清空对象
    lv_obj_clean(act_obj); // 清空此前页面

    display2=lv_obj_create(NULL,NULL);
    //lv_disp_set_bg_image(display1,&cpmGUI1);
    img=lv_img_create(display2,NULL);//新建图片
	lv_obj_set_size(img,240,240);
	lv_obj_set_pos(img,0,0);//移动到指定位置
	lv_obj_move_foreground(img);//移动到顶层
	lv_img_set_src(img,&cpmGUI1);//显示图像

    display2_CPUlabel1=lv_label_create(img,NULL);
    lv_obj_add_style(display2_CPUlabel1,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display2_CPUlabel1, true);
    lv_obj_align(display2_CPUlabel1, NULL, LV_ALIGN_IN_TOP_LEFT, 110, 7);
    
    display2_CPUlabel2=lv_label_create(img,NULL);
    lv_obj_add_style(display2_CPUlabel2,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display2_CPUlabel2, true);
    lv_obj_align(display2_CPUlabel2, NULL, LV_ALIGN_IN_TOP_LEFT, 110, 47);

    display2_CPUlabel3=lv_label_create(img,NULL);
    lv_obj_add_style(display2_CPUlabel3,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display2_CPUlabel3, true);
    
    lv_obj_align(display2_CPUlabel3, NULL, LV_ALIGN_IN_TOP_LEFT, 110, 87);

    display2_GPUlabel1=lv_label_create(img,NULL);
    lv_obj_add_style(display2_GPUlabel1,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display2_GPUlabel1, true);
    lv_obj_align(display2_GPUlabel1, NULL, LV_ALIGN_IN_TOP_LEFT, 110, 145);

    display2_GPUlabel2=lv_label_create(img,NULL);
    lv_obj_add_style(display2_GPUlabel2,LV_BTN_PART_MAIN,&d2_CPUlabel1);
    lv_label_set_recolor(display2_GPUlabel2, true);
    lv_obj_align(display2_GPUlabel2, NULL, LV_ALIGN_IN_TOP_LEFT, 110, 185);
    

    lv_obj_add_style(display2,LV_BTN_PART_MAIN,&display);

    lv_label_set_text_fmt(display2_CPUlabel1, "CPU占用率:   %%");

    lv_label_set_text_fmt(display2_CPUlabel2, "CPU频率:   Ghz");

    lv_label_set_text_fmt(display2_CPUlabel3, "CPU温度:   C");

    lv_label_set_text_fmt(display2_GPUlabel1, "GPU占用率:   %%");

    lv_label_set_text_fmt(display2_GPUlabel2, "GPU温度:   C");

    lv_scr_load(display2);
    

}



void display1_cpm(const char *CPU1,const char *CPU2,const char *CPU3,const char *GPU1,const char *GPU2)
{
    //cpm_display_init();
    // lv_obj_add_style(display1,LV_BTN_PART_MAIN,&display);

    // lv_label_set_text_fmt(display2_CPUlabel1, "CPU占用率:%s %%", CPU1);

    // lv_label_set_text_fmt(display2_CPUlabel2, "CPU频率:%s Ghz", CPU2);

    // lv_label_set_text_fmt(display2_CPUlabel3, "CPU温度:%s C", CPU3);

    // lv_label_set_text_fmt(display2_GPUlabel1, "GPU占用率:%s %%", GPU1);

    // lv_label_set_text_fmt(display2_GPUlabel2, "GPU温度:%s C", GPU2);

    
    lv_bar_set_value(display1_bar_cpu1, atoi(CPU1), LV_ANIM_OFF);
    //lv_bar_set_anim_time(display1_bar_cpu1,100);

    lv_bar_set_value(display1_bar_cpu2, atoi(CPU2)*100, LV_ANIM_OFF);
    //lv_bar_set_anim_time(display1_bar_cpu2,100);

    lv_bar_set_value(display1_bar_cpu3, atoi(CPU3), LV_ANIM_OFF);
    //lv_bar_set_anim_time(display1_bar_cpu3,100);

    lv_bar_set_value(display1_bar_gpu1, atoi(GPU1), LV_ANIM_OFF);
    //lv_bar_set_anim_time(display1_bar_gpu1,100);

    lv_bar_set_value(display1_bar_gpu2, atoi(GPU2), LV_ANIM_OFF);
    //lv_bar_set_anim_time(display1_bar_gpu2,100);

    if(atoi(CPU1)>=80)
    {
        lv_obj_set_style_local_bg_color(display1_bar_cpu1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_RED);
    }
    else
    {
        lv_obj_set_style_local_bg_color(display1_bar_cpu1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    }

    if(atoi(CPU2)*100<=200)
    {
        lv_obj_set_style_local_bg_color(display1_bar_cpu2, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    }
    else if(atoi(CPU2)*100<=150)
    {
        lv_obj_set_style_local_bg_color(display1_bar_cpu2, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_RED);
    }
    else
    {
        lv_obj_set_style_local_bg_color(display1_bar_cpu2, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    }

    if(atoi(CPU3)>=80)
    {
        lv_obj_set_style_local_bg_color(display1_bar_cpu3, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_RED);
    }
    else
    {
        lv_obj_set_style_local_bg_color(display1_bar_cpu3, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    }

    if(atoi(GPU1)>=80)
    {
        lv_obj_set_style_local_bg_color(display1_bar_gpu1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_RED);
    }
    else
    {
        lv_obj_set_style_local_bg_color(display1_bar_gpu1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    }

    if(atoi(GPU2)>=80)
    {
        lv_obj_set_style_local_bg_color(display1_bar_gpu2, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_RED);
    }
    else
    {
        lv_obj_set_style_local_bg_color(display1_bar_gpu2, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    }

    lv_label_set_text_fmt(display1_CPUlabel1_show, "%s %%", CPU1);

    lv_label_set_text_fmt(display1_CPUlabel2_show, "%s Ghz", CPU2);

    lv_label_set_text_fmt(display1_CPUlabel3_show, "%s C", CPU3);

    lv_label_set_text_fmt(display1_GPUlabel1_show, "%s %%", GPU1);

    lv_label_set_text_fmt(display1_GPUlabel2_show, "%s C", GPU2);

    //lv_scr_load(display1);
}

void cpm_obj_del(void)
{
    if(display2_CPUlabel1!=NULL)
    {
        lv_obj_clean(display1_CPU);
        lv_obj_clean(display1_CPUlabel1);
        lv_obj_clean(display1_CPUlabel2);
        lv_obj_clean(display1_CPUlabel3);
        lv_obj_clean(display1_GPU);
        lv_obj_clean(display1_GPUlabel1);
        lv_obj_clean(display1_GPUlabel2);
        
        lv_obj_clean(display1_bar_cpu1);
        lv_obj_clean(display1_bar_cpu2);
        lv_obj_clean(display1_bar_cpu3);
        lv_obj_clean(display1_bar_gpu1);
        lv_obj_clean(display1_bar_gpu2);

        lv_obj_clean(display1_CPUlabel1_show);
        lv_obj_clean(display1_CPUlabel2_show);
        lv_obj_clean(display1_CPUlabel3_show);
        lv_obj_clean(display1_GPUlabel1_show);
        lv_obj_clean(display1_GPUlabel2_show);

        lv_obj_clean(display2_CPUlabel1);
        lv_obj_clean(display2_CPUlabel2);
        lv_obj_clean(display2_CPUlabel3);
        lv_obj_clean(display2_GPUlabel1);
        lv_obj_clean(display2_GPUlabel2);
        lv_obj_clean(img);
        display1_CPU=NULL;
        display1_CPUlabel1=NULL;
        display1_CPUlabel2=NULL;
        display1_CPUlabel3=NULL;
        display1_GPU=NULL;
        display1_GPUlabel1=NULL;
        display1_GPUlabel2=NULL;
        
        display1_bar_cpu1=NULL;
        display1_bar_cpu2=NULL;
        display1_bar_cpu3=NULL;
        display1_bar_gpu1=NULL;
        display1_bar_gpu2=NULL;

        display1_CPUlabel1_show=NULL;
        display1_CPUlabel2_show=NULL;
        display1_CPUlabel3_show=NULL;
        display1_GPUlabel1_show=NULL;
        display1_GPUlabel2_show=NULL;

        display2_CPUlabel1=NULL;
        display2_CPUlabel2=NULL;
        display2_CPUlabel3=NULL;
        display2_GPUlabel1=NULL;
        display2_GPUlabel2=NULL;
        img=NULL;
    }

}

void cpm_gui_del(void)
{
    cpm_obj_del();
    if(NULL!=display1)
    {
        lv_obj_clean(display1);
        display1=NULL;

    }
    if(NULL!=display2)
    {
        lv_obj_clean(display2);
        display2=NULL;
    }
}