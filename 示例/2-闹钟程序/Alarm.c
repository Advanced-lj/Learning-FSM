#include "App_Alarm.h"
#include "USART1.h"
#include <stdio.h>
#include "diag/Trace.h"

/* 相关常量定义 */
#define ALARM_MUSIC_END  0      // 闹钟音乐播放完毕
#define FORMAT_OK        0      // 格式正确
#define FORMAT_ERROR     (-1)   // 格式错误

/* 输入信息定义
 * 作为函数的返回值供函数 getInput() 使用
 * getInput() 将获取并返回键盘或触摸屏等设备中输入的控制命令或闹钟时间值
 */
#define INPUT_ERROR    (-1)    // 输入格式错误
#define INPUT_CANCEL   (-2)    // 输入了「取消」命令
#define INPUT_SNOOZE   (-3)    // 输入了「小睡」命令
#define INPUT_ALARM_ON (-4)    // 输入了「打开闹钟」命令
#define NO_INPUT       (-10)   // 没有输入

/* 输出信息定义
 * 作为为函数的参数供函数 displayMessege() 使用
 * displayMessege() 用于在显示屏上显示相关的提示信息
 */
#define MESSEGE_SET_ALARM_TIME     (0)  // 提示：设置闹钟时间
#define MESSEGE_CLEAR              (1)  // 提示：已取消
#define MESSEGE_ALARM_IS_ON        (2)  // 提示：闹钟已打开
#define MESSEGE_WAITING            (3)  // 提示：等待闹钟响起
#define MESSEGE_SET_SNOOZE_TIME    (4)  // 提示：设置小睡时间
#define MESSEGE_GET_UP             (5)  // 提示：该起床了

/* 闹钟的状态 */
enum alarmStates
{
    ALARM_OFF,              // 闹钟关闭
    SET_ALARM_TIME,         // 设置闹钟时间
    WATING_FOR_ALARM,       // 等待闹钟响起
    PLAY_ALARM_MUSIC,       // 播放闹钟音乐
    SET_SNOOZE_TIME         // 设置贪睡时间
} alarmState = ALARM_OFF;   // 默认状态：闹钟关闭

/* 相关函数的定义 */
int16_t getInput(void);
void displayMessege(uint8_t);
void setAlarm(int16_t);
int16_t alarmTimeDiff(void);
int8_t playAlarmMusic(void);
void setSnooze(int16_t);
uint8_t checkAlarmFormat(int16_t);
uint8_t checkSnoozeFormat(int16_t);

/*
 * 闹钟主程序，需要放入 while(1) 中循环调用
 */
void alarmApp(void)
{
    int16_t input;      // 输入值暂存在这个变量中
    switch (alarmState) // 获取闹钟状态，下面程序将根据闹钟的状态执行相应的任务
    {
        /* 状态：闹钟关闭
        * 在此状态中，将会不断检查是否打开闹钟，如果打开了闹钟，则会进入下一个状态：设置闹钟时间
        */
        case ALARM_OFF:
            if (getInput() == INPUT_ALARM_ON)  // 检查是否打开了闹钟
            {   // 如果打开了闹钟
                displayMessege(MESSEGE_SET_ALARM_TIME); // 在屏幕或串口上提示：请设置闹钟时间
                alarmState = SET_ALARM_TIME;            // 进入下一个状态：设置闹钟时间
            }
            break;
        /* 状态：设置闹钟时间
        * 在此状态中，将会检查输入值，
        * 如果
        *      输入“取消”命令，则取消闹钟设置，返回到闹钟关闭的状态
        *      输入闹钟时间格式错误，则状态不变，等待下一次重新输入
        *      输入了正确的闹钟时间，则设置闹钟，显示闹钟设置成功，并进入下一状态：等待闹钟响起
        */
        case SET_ALARM_TIME:
            input = getInput();         // 获取输入值
            if(input == INPUT_CANCEL)   // 如果输入了“取消”
            {
                displayMessege(MESSEGE_CLEAR);  // 显示“已取消”
                alarmState = ALARM_OFF;         // 进入状态：关闭闹钟
            }
            else if(checkAlarmFormat(input) == FORMAT_OK)   // 如果输入格式正确
            {
                displayMessege(MESSEGE_ALARM_IS_ON); // 显示“成功设置闹钟，闹钟已启动”
                setAlarm(input); // 根据输入值设置闹钟
                alarmState = WATING_FOR_ALARM; // 进入下一状态：等待闹钟响起
            }
            break;
        /* 状态：等待闹钟响起
        * 在此状态中，将会检查是否到达闹钟时间，如果到达，则进入下一状态：播放闹钟音乐
        * 同时，在此状态中也会检查输入，如果输入了“取消”的命令，则进入闹钟关闭的状态
        */
        case WATING_FOR_ALARM:
            displayMessege(MESSEGE_WAITING); // 显示等待闹钟响起的信息，例如离闹钟响起还有多长时间
            if (alarmTimeDiff() <= 0) // 检查离闹钟响起还有多少时间，如果时间小于等于零（到达闹钟时间）
            {
                alarmState = PLAY_ALARM_MUSIC;  // 进入下一个状态：播放闹钟音乐
            }
            if(getInput() == INPUT_CANCEL) // 如果输入了“取消”命令
            {
                displayMessege(MESSEGE_CLEAR);
                alarmState = ALARM_OFF;      // 进入闹钟关闭的状态
            }
            break;
        /* 状态：播放闹钟音乐
        * 在此状态中，将播放闹钟音乐，若播放完毕，进入闹钟关闭的状态
        * 同时，在此状态中也会检查输入，
        *      如果输入了“小睡”的命令，则进入状态：设置小睡时间
        *      如果输入了“取消”的命令，则进入状态：闹钟关闭
        */
        case PLAY_ALARM_MUSIC:
            displayMessege(MESSEGE_GET_UP);  // 显示消息：“该起床了”
            if(playAlarmMusic() == ALARM_MUSIC_END) // 播放闹钟音乐
            { // 若音乐播放完毕
                displayMessege(MESSEGE_CLEAR);
                alarmState = ALARM_OFF; // 进入状态：闹钟关闭
            }
            input = getInput();
            if(input == INPUT_SNOOZE) // 若输入了“小睡”的命令
            {
                displayMessege(MESSEGE_SET_SNOOZE_TIME); // 显示消息：“请设置小睡时间”
                alarmState = SET_SNOOZE_TIME; // 进入状态：设置小睡时间
            }
            if(input == INPUT_CANCEL) // 若输入了“取消”命令
            {
                displayMessege(MESSEGE_CLEAR);
                alarmState = ALARM_OFF;   // 进入状态：闹钟关闭
            }
            break;
        /* 状态：设置小睡时间
        * 在此状态中，将从输入获取小睡时间，并将闹钟时间加上小睡时间，进入状态：等待闹钟响起
        */
        case SET_SNOOZE_TIME:
            input = getInput(); // 获取输入
            if(input == INPUT_CANCEL)
            {   // 若输入“取消”，则进入“闹钟关闭”的状态
                displayMessege(MESSEGE_CLEAR);
                alarmState = ALARM_OFF;
            }
            else if(checkSnoozeFormat(input) == FORMAT_OK)
            {   // 若输入格式正确
                setSnooze(input);  // 设置新的闹钟时间
                alarmState = WATING_FOR_ALARM;  // 进入状态：等待闹钟响起
            }
            break;
        default:
            displayMessege(MESSEGE_CLEAR);
            alarmState = ALARM_OFF;
    }
}