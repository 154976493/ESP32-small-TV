#ifndef PAJ_H
#define PAJ_H

#include "lvgl.h"
#include<I2Cdev.h>
#include<paj7620.h>
#include"lv_port_indev.h"

extern int32_t encoder_diff;
extern lv_indev_state_t encoder_state;

extern const char *active_type_info[];
enum ACTIVE_TYPE
{
    TURN_RIGHT = 0,
    RETURN,//返回
    TURN_LEFT,//向右
    UP,//向上
    DOWN,//向下
    GO_FORWORD,//进入app
    SHAKE,//摇动
    UNKNOWN,//未知动作
    OPEN_LOCK,//解锁
    LOCK//开锁
};
// enum MPU_DIR_TYPE
// {
//     NORMAL_DIR_TYPE = 0,
//     X_DIR_TYPE = 0x01,
//     Y_DIR_TYPE = 0x02,
//     Z_DIR_TYPE = 0x04,
//     XY_DIR_TYPE = 0x08
// };

struct SysMpuConfig
{
    int16_t x_gyro_offset;
    int16_t y_gyro_offset;
    int16_t z_gyro_offset;

    int16_t x_accel_offset;
    int16_t y_accel_offset;
    int16_t z_accel_offset;
};
struct ImuAction
{
    ACTIVE_TYPE active;
    boolean isValid;
    boolean long_time;
    // int16_t v_ax; // v表示虚拟参数（用于调整6050的初始方位）
    // int16_t v_ay;
    // int16_t v_az;
    // int16_t v_gx;
    // int16_t v_gy;
    // int16_t v_gz;
};
class PAJ
{
    private:
        uint8_t order; 
        long last_update_time;
    public:
        ImuAction action_info;
    public:
        PAJ();
        void init(uint8_t order);
        void setOrder(uint8_t order);
        bool Encoder_GetIsPush(void);
        ImuAction *update(int interval);
        //void getVirtureMotion6(ImuAction *action_info);
};
#endif
