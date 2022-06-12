#include "paj.h"
#include "common.h"
int duration=1000;
int temptime=0;
int lock1=0;//0为锁屏，1为解锁
int exist=0;//img控件是否存在1为存在，0为不存在
lv_obj_t *img=NULL;//图片控件句柄
LV_IMG_DECLARE(Open_Lock);
LV_IMG_DECLARE(Lock);
const char *active_type_info[] = {"TURN_RIGHT", "RETURN",
                                  "TURN_LEFT", "UP",
                                  "DOWN", "GO_FORWORD",
                                  "SHAKE", "UNKNOWN"};
PAJ::PAJ()
{
    action_info.isValid = true;
    action_info.active = UNKNOWN;
    action_info.long_time = true;
    this->order = 0; // 表示方位
}
void PAJ::init(uint8_t order)
{
	
	lv_obj_set_click (lv_layer_top(),false);
	
    this->setOrder(order);//设置方向
    Wire.begin(IMU_I2C_SDA,IMU_I2C_SCL);
    Wire.setClock(400000);
    //unsigned long timeout=5000;
    //unsigned long preMillis=millis();
    uint8_t error = 0;
    error = paj7620Init();
  
    if (error)
    {
        Serial.print(F("Unable to connect to MPU6050.\n"));
        return;
    }
    else
    {
        Serial.println("INIT OK");
    }
    //Serial.print(F("Initialization PAJ7629 now, Please don't move.\n"));
    
	Serial.println(this->order);
}
void PAJ::setOrder(uint8_t order)
{
    this->order =order;
}
bool PAJ::Encoder_GetIsPush(void)
{
    #ifdef PEAK
     return (digitalRead(CONFIG_ENCODER_PUSH_PIN) == LOW);
    #else
     return false;
    #endif
}
ImuAction *PAJ::update(int interval)
{
	//lv_obj_t *p=lv_scr_act();//获取当前活动屏幕
	lv_obj_t *p=lv_layer_top();//获取top层指针
	

	
	if(millis()-temptime>duration)
	{
		if(exist==1)
		{
			lv_obj_del(img);
			exist=0;
		}
		
	}
    uint8_t data = 0,error1;
	
	

    if(millis() - last_update_time > interval)
    {
		if(!action_info.isValid)
		{
			error1 = paj7620ReadReg(0x43, 1, &data);				// Read Bank_0_Reg_0x43/0x44 for gesture result.
			// Serial.println(error1);
			// Serial.println(data);
			if (!error1) 
			{
				switch (data) 									// When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
				{
					case GES_RIGHT_FLAG:
						if(lock1==0)
						{
							if(millis()-temptime>duration)
							{
								img=lv_img_create(p,NULL);//新建图片
								lv_obj_set_size(img,100,100);
								lv_obj_set_pos(img,70,140);//移动到指定位置
								lv_obj_move_foreground(img);//移动到顶层
								lv_img_set_src(img,&Lock);//显示图像
								exist=1;
								
								temptime=millis();
							}
						}
						else if(lock1==1)
						{
							if(this->order==0)//旋转paj7620操作方向，顺时针旋转0==丝印在上，1==丝印在右，2==丝印在下，3==丝印在左
							{
								//Serial.println("Right");
								encoder_diff++;
								action_info.isValid = 1;
								action_info.active = TURN_RIGHT;
							}
							else if(this->order==1)
							{
								//Serial.println("Down");
								action_info.isValid = 1;
								action_info.active = RETURN;
								encoder_state = LV_INDEV_STATE_REL;
							}
							else if(this->order==2)
							{
								//Serial.println("Left");
								encoder_diff--;
								action_info.isValid = 1;
								action_info.active = TURN_LEFT;
							}
							else if(this->order==3)
							{
								//Serial.println("Up");
									
								action_info.isValid = 1;
								action_info.active = GO_FORWORD;
								encoder_state = LV_INDEV_STATE_PR;
							}
						}
						
						
					     
						break;
					case GES_LEFT_FLAG: 
						if(lock1==0)
						{
							if(millis()-temptime>duration)
							{
								img=lv_img_create(p,NULL);//新建图片
								lv_obj_set_size(img,100,100);
								lv_obj_set_pos(img,70,140);//移动到指定位置
								lv_obj_move_foreground(img);//移动到顶层
								lv_img_set_src(img,&Lock);//显示图像
								exist=1;
								
								temptime=millis();
							}
						}
						else if(lock1==1)
						{
							if(this->order==0)
							{
								//Serial.println("Left");
								encoder_diff--;
								action_info.isValid = 1;
								action_info.active = TURN_LEFT;
							}
							else if(this->order==1)
							{
								//Serial.println("Up");
									
								action_info.isValid = 1;
								action_info.active = GO_FORWORD;
								encoder_state = LV_INDEV_STATE_PR;
							}
							else if(this->order==2)
							{
								//Serial.println("Right");
								encoder_diff++;
								action_info.isValid = 1;
								action_info.active = TURN_RIGHT;
							}
							else if(this->order==3)
							{
								//Serial.println("Down");
								action_info.isValid = 1;
								action_info.active = RETURN;
								encoder_state = LV_INDEV_STATE_REL;
							}
							
						}
						
						     
						break;
					case GES_UP_FLAG:
						if(lock1==0)
						{
							if(millis()-temptime>duration)
							{
								img=lv_img_create(p,NULL);//新建图片
								lv_obj_set_size(img,100,100);
								lv_obj_set_pos(img,70,140);//移动到指定位置
								lv_obj_move_foreground(img);//移动到顶层
								lv_img_set_src(img,&Lock);//显示图像
								exist=1;
								
								temptime=millis();
							}
						}
						else if(lock1==1)
						{
							if(this->order==0)
							{
								//Serial.println("Up");
									
								action_info.isValid = 1;
								action_info.active = GO_FORWORD;
								encoder_state = LV_INDEV_STATE_PR;
							}
							else if(this->order==1)
							{
								//Serial.println("Right");
								encoder_diff++;
								action_info.isValid = 1;
								action_info.active = TURN_RIGHT;
							}
							else if(this->order==2)
							{
								//Serial.println("Down");
								action_info.isValid = 1;
								action_info.active = RETURN;
								encoder_state = LV_INDEV_STATE_REL;
							}
							else if(this->order==3)
							{
								//Serial.println("Left");
								encoder_diff--;
								action_info.isValid = 1;
								action_info.active = TURN_LEFT;
							}
						}
						
						
						     
						break;
					case GES_DOWN_FLAG:
						if(lock1==0)
						{
							if(millis()-temptime>duration)
							{
								img=lv_img_create(p,NULL);//新建图片
								lv_obj_set_size(img,100,100);
								lv_obj_set_pos(img,70,140);//移动到指定位置
								lv_obj_move_foreground(img);//移动到顶层
								lv_img_set_src(img,&Lock);//显示图像
								exist=1;
								
								temptime=millis();
							}
						}
						else if(lock1==1)
						{
							if(this->order==0)
							{
								//Serial.println("Down");
								action_info.isValid = 1;
								action_info.active = RETURN;
								encoder_state = LV_INDEV_STATE_REL;
							}
							else if(this->order==1)
							{
								//Serial.println("Left");
								encoder_diff--;
								action_info.isValid = 1;
								action_info.active = TURN_LEFT;
							}
							else if(this->order==2)
							{
								//Serial.println("Up");
									
								action_info.isValid = 1;
								action_info.active = GO_FORWORD;
								encoder_state = LV_INDEV_STATE_PR;
							}
							else if(this->order==3)
							{
								//Serial.println("Right");
								encoder_diff++;
								action_info.isValid = 1;
								action_info.active = TURN_RIGHT;
							}
						}
						
						
							
						     
						break;
					case GES_FORWARD_FLAG:
						//Serial.println("Forward");
						//action_info.isValid = 1;
						action_info.active = UNKNOWN;
						action_info.isValid = 0;
						
						
						break;
					case GES_BACKWARD_FLAG:		  
						//Serial.println("Backward");
						//action_info.isValid = 1;
						action_info.active = UNKNOWN;
						action_info.isValid = 0;
						
						break;
					case GES_CLOCKWISE_FLAG:          //顺时针解锁
						//Serial.println("Clockwise");
						
						if(millis()-temptime>duration)
						{
							img=lv_img_create(p,NULL);//新建图片
							lv_obj_set_size(img,100,100);
							lv_obj_set_pos(img,70,140);//移动到指定位置
							lv_obj_move_foreground(img);//移动到顶层
							//lv_layer_top();
							
							lv_img_set_src(img,&Open_Lock);//显示图像
							exist=1;
							
							temptime=millis();
						}
						

						action_info.isValid = 1;
						lock1=1;
						Serial.println("open lock screentt");
						//action_info.active = OPEN_LOCK;
						// action_info.isValid = 0;
						break;
					case GES_COUNT_CLOCKWISE_FLAG:         //逆时针锁屏
						//Serial.println("anti-clockwise");
						
						if(millis()-temptime>duration)
						{
							img=lv_img_create(p,NULL);//新建图片
							lv_obj_set_size(img,100,100);
							lv_obj_set_pos(img,70,140);//移动到指定位置
							lv_obj_move_foreground(img);//移动到顶层
							lv_img_set_src(img,&Lock);//显示图像
							exist=1;

							temptime=millis();
						}
						

						action_info.isValid = 1;
						lock1=0;
						Serial.println("lock screen");
						action_info.active = LOCK;
						// action_info.isValid = 0;
						break;  
					default:
						//Serial.println("wave");
						//action_info.isValid = 1;
						action_info.active = UNKNOWN;
						action_info.isValid = 0;
						// paj7620ReadReg(0x44, 1, &data1);
						// if (data1 == GES_WAVE_FLAG) 
						// {
						// 	Serial.println("wave");
						// }
						// break;
				}
				
			}	
		}
		
		last_update_time = millis();
    }
    return &action_info;
}