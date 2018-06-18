# Wemos D1 Mini控制板说明书

 使用Wemos D1 Mini控制板可以通过I2C控制两台直流马达，这个控制板是基于东芝的TB6612FNG驱动器构建的。该电路板能够独立控制每个电机的速度和方向（A，B），最大允许功率为每台电机1.2A，它支持3.2A的功率峰值。默认情况下，控制板载I2C总线上的地址0x30，该控制板带有一个C ++库以及4个Arduino代码示例，没有安装基于STM32F030通过I2C总线控制该芯片的的固件，在Aliexpress官方Wemos商店购买能买到这个控制板

### Wemos D1mini板（TB6612FNG）介绍

#### 控制板如下图：

![](https://www.projetsdiy.fr/wp-content/uploads/2017/04/wemos-d1-mini-shield-motor-deballage.jpg)

大约20cm*20cm大小，四边有相应针孔，可以根据需要设计相应的接插件来连接控制器和马达，通过I2C总线来连接和控制该控制板，默认的是使用D1，D2这两个针脚。

控制板是围绕东芝TB6612FNG电路构建的,完整的图纸和技术数据表如下控制板是围绕东芝TB6612FNG电路构建的,完整的图纸和技术数据表如下

![](https://www.projetsdiy.fr/wp-content/uploads/2017/03/wemos-d1-mini-motor-i2c-shield-TB6621FNG.jpg)

#### 技术特点：
- I2C接口（参见下一段改变默认地址0x30）
- 最大电源电压（VM引脚）：最大15Vdc
- 输出电流（引脚Iout）：1.2A（平均） -  3.2A（峰值）
- 控制板电源和电机电源独立，可以通过待机功能来减少功耗
- 支持CW / CCW /加减速/定速/短制动/停止等控制模式


#### 针脚说明：
- VM：电源+（最大15Vdc）
- GND：负电机端子 -
- A1 A2：马达A输出
- B1 B2：马达B输出
- S：待机控制端口（IO模式下有效）
- DIO/CLK:???
- Stanby控制模式选择：
- I2C模式：使用I2C协议控制TB6612FNG电路
- IO模式：通过控制板自身的输入针脚来控制TB6612FNG电路（当S导通时)

#### 复位模式
- 对应的跳线如果短接，通过重新上电或者设计一个复位按钮来进行复位
- 对应的条线如果断开，由“DTR”引脚复位

#### 如何更改控制板的I2C地址
默认情况下，控制板位于I2C总线上的地址是0x30，
在控制板的背面有2个用于跳线的点AD0和AD1。通过在AD0，AD1和GND之间连接跳线，可以改变控制板的地址

地址改变如下表


|  AD1  | AD0   | 地址   |  |
| ------------ | ------------ | ------------ | ------------|
|  -  |  -  |  0x2D  |   |
|  - | ×  |  0x2E |   |
|  × |  - |0x2F  |  |
|  × |×   | 0x30  |defaut  |

' - ' 意味着两点与GND之间是开路的, ' x ' 表示两点与GND之间是连接的


#### 连接器的焊接
控制板上有4个连接器可焊接。每个侧面上都有一个可以像往常一样将Wemos D1 Mini上的防护罩叠加在一起。
第三个（USB端）用于为外置电池上的电机供电（Wemos功能不足以驱动发动机）并驱动两个发动机
没有记录与电机连接器相对的第四个连接器。看来，这只是一个串口。黑客可以使用它来重新编程用于I2C通信的STM32F030微控制器（完整教程）。
DTR引脚允许您重置TB6612FNG。电路板背面的跳线RST必须去焊以激活此模式

为了焊接电机连接器，我建议您将其插入面包板，以在焊接过程中保持屏蔽垂直

![](https://www.projetsdiy.fr/wp-content/uploads/2017/04/wemos-d1-mini-shield-motor-soudure-connecteur-moteur-mise-en-place.jpg)

#### Arduino / ESP8266代码Arduino / ESP8266代码
Wemos为Arduino开发了一个C ++库。 GitHub上提供了源代码以及四个基本示例：
Motor_base
Motor_base2
Motor_standby_i2c
Motor_standby_io
要在Arduino程序中调用该库，只需在程序开始时声明它即可

 <#include“WEMOS_Motor.h”>

“Motor”这个方法用于为每个电机A和B初始化为一个对象。“Motor”类的相关参数：
控制板的I2C地址。默认0x30
使用_MOTOR_A或_MOTOR_B 这两个键指示对象与哪个外设相关联
PWM频率。默认情况下，它是1000Hz。
待机模式。默认情况下，它由屏蔽管理。如果您需要从Wemos控制它，请指定使用的引脚。不要忘了将STBY桥焊接到卡的背面。

下面以马达A为例，相关的设定的语句入下：
Motor M1(0X30,_MOTOR_A, 1000)

为了控制外设，我们有一个set motor方法，它需要1或2个参数。第一个参数是要执行的命令。第二个是设定值。TB6612FNG提供的命令包括以下内容：

- _SHORT_BRAKE。这里对功能的兴趣的早期解释

- _CCW表示逆时针或逆时针旋转， 通过一个0％和100％之间的参数来设定输出功率的大小，其也对应于电动机的最大速度

- _CW表示逆时针或顺时针旋转

- _STOP 停止

- _STANDBY 允许暂停电机

![](https://diyprojects.io/wp-content/uploads/2017/06/wemos-d1-mini-motor-shield-v1.jpg)

### 原版说明书链接
https://diyprojects.io/test-shield-motor-i2c-wemos-d1-mini-pro-drive-2-motors-15vdc/#.Wuzh0MNubIU

## END

