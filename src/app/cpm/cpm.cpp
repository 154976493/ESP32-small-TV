#include "cpm.h"
#include "cpm_gui.h"
#include "sys/app_controller.h"
#include "../../common.h"

#define CPM_CONFIG_PATH "/cpm.cfg"


WiFiClient client;
uint16_t serverPort = 19766; 
String data[5];





int initsatte=0;//程序是否第一次运行(初始化是否完成)

int temptime1=0;





struct cpmAppRunData
{
    unsigned int wifi_init;

};

struct cpm_config
{
    String server_IP;
    //uint16_t server_Port;
    int server_Port;
};



static void write_config(const cpm_config *cfg)
{
    char tmp[16];
    //将配置数据保存在文件中（持久化）
    String w_data;
    w_data = w_data + cfg->server_IP + "\n";
    memset(tmp, 0, 16);//把tmp指向的内存空间初始化为16个0
    snprintf(tmp, 16, "%u\n", cfg->server_Port);//将可变参数cfg->updataInterval按%u\n格式
                                                    //格式化为字符串并复制16长度的字符到tmp中
    w_data += tmp;
    Serial.println(w_data);
    Serial.println(cfg->server_IP+":"+cfg->server_Port);
    g_flashCfg.writeFile(CPM_CONFIG_PATH, w_data.c_str());
}

static void read_config(cpm_config *cfg)
{
    // 如果有需要持久化配置文件 可以调用此函数将数据存在flash中
    // 配置文件名最好以APP名为开头 以".cfg"结尾，以免多个APP读取混乱
    char info[128] = {0};
    uint16_t size = g_flashCfg.readFile(CPM_CONFIG_PATH, (uint8_t *)info);
    info[size] = 0;
    if (size == 0)
    {
        // 默认值
        cfg->server_IP = "192.168.0.102";  // 服务器IP地址
        cfg->server_Port = 19766; // 服务器端口号
        write_config(cfg);
    }
    else
    {
        // 解析数据
        char *param[2] = {0};
        analyseParam(info, 2, param);
        cfg->server_IP = param[0];
        cfg->server_Port = atol(param[1]);
        Serial.println(param[0]+String(param[1]));
        
    }   

}


static cpm_config cfg_data;
static cpmAppRunData *run_data=NULL;


void Strsplit(const String &str,const char split)//文本分割
{
    if(str=="") return;
    String strs=str+split;
    
    int i=0;
    size_t pos=strs.indexOf(split);
    while(-1!=pos)
    {
        String temp=strs.substring(0,pos);
        data[i]=temp;
        i++;
        strs=strs.substring(pos+1,strs.length());
        pos=strs.indexOf(split);
    }
    return;

}

String Strint(const String &str)//保留两位小数
{
    
    String strs=str;
    
    
    size_t pos=strs.indexOf('.');
    String temp=strs.substring(0,pos+3);
   
    return temp;

}

static int cpm_init(void)
{
    cpm_gui_init();
    cpm_display1_init();
    read_config(&cfg_data);

    serverPort=(uint16_t)cfg_data.server_Port;
    initsatte=1;
    run_data=(cpmAppRunData *)malloc(sizeof(cpmAppRunData));
    run_data->wifi_init=0;

}

static void cpm_process(AppController *sys, const ImuAction *act_info)
{
    
    if (RETURN == act_info->active)
    {
        sys->app_exit(); // 退出APP
        return;
    }
    if(run_data->wifi_init==0)
    {
        sys->send_to(CPM_APP_NAME, CTRL_NAME,APP_MESSAGE_WIFI_CONN, NULL, NULL);
    }
    
    if(run_data->wifi_init==1)
    {

        

        if(client.connect(cfg_data.server_IP.c_str(),serverPort))
        {

            Serial.println("1111");
            
            if(millis()-temptime1>1000)
            {
                sys->send_to(CPM_APP_NAME, CTRL_NAME,APP_MESSAGE_WIFI_CONN, NULL, NULL);
                client.print("1");
                temptime1=millis();
                Serial.println("请求成功");
                
            }
            
            
            while(client.connected() ||client.available())
            {
                if(client.available())
                {
                    String tcp_data=client.readStringUntil('\n');
                    Serial.print("TCP读取到的数据为：");
                    Serial.println(tcp_data);
                    Strsplit(tcp_data,',');
                    
                    float lf=atoi(data[1].c_str())/1024.0;
                    
                    data[1]=Strint(String(lf));
                
                    display1_cpm(data[0].c_str(),data[1].c_str(),data[2].c_str(),data[3].c_str(),data[4].c_str());
                    break;
                }
                else if(millis()-temptime1>5000)
                {
                   break; 
                }
                
                
            }
            

        }
        else
        {
            Serial.println("连接超时！");
        }
        

    }

    

}

int cpm_exit_callback(void *param)
{
    client.stop();
    cpm_gui_del();
    free(run_data);
    run_data=NULL;
    initsatte=0;

}

static void cpm_message_handle(const char *from, const char *to,
                                    APP_MESSAGE_TYPE type, void *message,
                                    void *ext_info)
{
    switch (type)
    {
        case APP_MESSAGE_WIFI_CONN:
        {
            Serial.print(millis());
            Serial.println("[SYS] cpm_event_notification");
            run_data->wifi_init=1;
        }
        break;
        case APP_MESSAGE_WIFI_ALIVE:
        {
            // wifi心跳维持的响应 可以不做任何处理
            
        }
        break;
        case APP_MESSAGE_GET_PARAM:
        {
            char *param_key = (char *)message;
            if (!strcmp(param_key, "server_IP"))
            {
                snprintf((char *)ext_info, 32, "%s", cfg_data.server_IP.c_str());
            }
            else if (!strcmp(param_key, "server_Port"))
            {
                snprintf((char *)ext_info, 6, "%u", cfg_data.server_Port);
            }
            else
            {
                snprintf((char *)ext_info, 32, "%s", "NULL");
            }
        }
        break;
        case APP_MESSAGE_SET_PARAM:
        {
            char *param_key = (char *)message;
            char *param_val = (char *)ext_info;
            if (!strcmp(param_key, "server_IP"))
            {
                cfg_data.server_IP = param_val;
                Serial.println("APP_MESSAGE_SET_PARAM:IP"+String(param_val));
            }
            else if (!strcmp(param_key, "server_Port"))
            {
                cfg_data.server_Port = atoi(param_val);
                Serial.println("APP_MESSAGE_SET_PARAM:Port"+String(param_val));
            }
        }
        break;
        case APP_MESSAGE_READ_CFG:
        {
            read_config(&cfg_data);
        }
        break;
        case APP_MESSAGE_WRITE_CFG:
        {
            write_config(&cfg_data);
        }
        break;
        default:
            break;
    }

}


APP_OBJ cpm_app = {CPM_APP_NAME, &app_cpm, "", cpm_init,
                        cpm_process, cpm_exit_callback,
                        cpm_message_handle};
