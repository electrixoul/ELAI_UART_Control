
#include "uart.h"
#include <stdio.h>
#include <unistd.h>       // Used for UART
#include <sys/fcntl.h>    // Used for UART
#include <termios.h>      // Used for UART
#include <string>
#include <iostream>
#include <caca_conio.h>

using namespace std;


ModeSetting MS;

int main(int argc, char *argv[]) {

    /*
     * ELAI 目标跟踪上位机示例程序 : 读取跟踪器数据输出流
     *
     * [ uart.h ] -> 串口配置:
     * fid = open(uart_target, O_RDWR | O_NOCTTY ); // 阻塞式接收模式
     * ...
     * port_options.c_cc[VMIN]  = VMINX; // Read at least 1 character
     * port_options.c_cc[VTIME] = 0;           // Wait indefinetly
     *
     */

    Uart u;
    int i;

    /*
     * 每条发送指令由字节 '@' 开始, '#' 结束
     * 包含起始结束符号一共12字节
     */
    unsigned char m[20] = "@0000000000#";

	bool exit_ = false;

    system("clear");

    while (!exit_)
    {
        system("clear");

        char console_input;
        cout<<endl<<"================= 输入EL-AI模块功能配置项 ================="<<endl;

        cout<<"直接发送目标选择触发指令 ? (y/n)"<<endl;
        cin>>console_input;
        if(console_input == 'y')
        {
            MS.reset = false;
            MS.centerSelection = true;
            MS.power_off = false;
            MS.NoRecover = true;
            MS.NaiveMode = true;
            MS.trigger = true;
        }
        else
        {
            cout<<endl<<"是否重置 ? (y/n)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
            {
                MS.reset = true;
                MS.power_off = false;
            }
            else {
                MS.reset = false;
                MS.power_off = false;
            }

            cout<<endl<<"是否进入简单模式 ? (y/n)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
                MS.NaiveMode = true;
            else MS.NaiveMode = false;

            cout<<endl<<"简单模式是否采用预设ROI选择 ? (输入n则采用ROI框选初始化) "<<endl;
            cin>>console_input;
            if(console_input == 'y')
                MS.NaiveMode_ClickSelection = true;
            else MS.NaiveMode_ClickSelection = false;

            cout<<endl<<"追踪车还是行人 ? (输入 y 则进入车辆检测模式)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
                MS.ped_or_car = false;
            else MS.ped_or_car = true;

            cout<<endl<<"是否关闭目标自动重找回 ? (y/n)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
                MS.NoRecover = true;
            else MS.NoRecover = false;

            cout<<endl<<"是否采用画面准星选取目标 ? (y/n)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
                MS.centerSelection = true;
            else MS.centerSelection = false;

            cout<<endl<<"设置本次目标跟踪触发信号: (y/n)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
                MS.trigger = true;
            else MS.trigger = false;

            cout<<endl<<"设备是否关机? (y/n)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
            {
                /// 关机指令:
                /// reset -> false
                /// power_off -> true
                MS.reset = false;
                MS.power_off = true;
            }
            else
            {
                MS.power_off = false;
            }

            cout<<endl<<"是否执行软重启? (y/n)"<<endl;
            cin>>console_input;
            if(console_input == 'y')
            {
                /// 软重启指令:
                /// reset -> true
                /// power_off -> true
                MS.reset = true;
                MS.power_off = true;
            }
            else {

            }
        }

        /// 对指令编码
        MS.encode();

        for(int i=0;i<10;i++)m[i+1] = MS.IP.pack_field[i];
        m[0] = '@';
        m[12] = '#';

        u.sendUart(m);
        cout<<"================= 指令已发送 ================="<<endl;
    }

    u.closeUart();
    return 0;
}






