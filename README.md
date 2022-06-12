# ESP32-small-TV
##本固件基于AIO固件2.0.5版本进行修改

##原版AIO固件地址：https://github.com/ClimbSnail/HoloCubic_AIO

##修改内容：把AIO固件的陀螺仪控制改为了手势控制，并新写了一个监控电脑性能的APP，首字母简称cpm，手势识别模块用的是PAJ7620U2模块


默认手势识别模块排针在上边的时候，向上挥手为为UP手势，考虑到安装方向不同，可以对其手势方向进行重定义<br>

修改手势识别模块的手势方向的方法：<br>
*重定义方向在webserver APP中的 系统->控制方向中设置：
*默认为0，即模块排针在上边的时候，向上挥手为为UP手势
*设置为1时，向左挥手为UP手势（可以在安装手势识别模块的时候顺时针旋转90度安装）
*设置为2时，向下挥手为UP手势（可以在安装手势识别模块的时候顺时针旋转180度安装）
*设置为3时，向右挥手为UP手势（可以在安装手势识别模块的时候顺时针旋转270度安装）
*（控制方向依次旋转90度）
    
新写了一个监控电脑性能的APP，首字母简称cpm,使用方法：<br>
*首先在webserver APP中的 电脑性能监控这一栏中填入上位机IP地址和端口号
*本APP采用tcp协议传输数据，数据为一串字符串，格式为(CPU占用率,CPU频率,CPU温度,GPU占用率,GPU温度\n)(不带括号，中间用英文逗号隔开，最后加一个换行符)
*上位机为server，APP为client
