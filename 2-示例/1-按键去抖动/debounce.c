/* 按键去抖动状态机中的三个状态 */
#define KEY_STATE_RELEASE  1  // 按键未按下
#define KEY_STATE_WAITING  2  // 等待（消抖）
#define KEY_STATE_PRESSED  3  // 按键按下（等待释放）

/* 等待状态持续时间
 * 需要根据单片机速度和按键消抖程序被调用的速度来进行调整
 */
#define DURIATION_TIME 40

/* 按键检测函数的返回值，按下为 1，未按下为 0 */
#define PRESSED      1
#define NOT_PRESSED  0

/* 按键扫描程序所处的状态
 * 初始状态为：按键按下（KEY_STATE_RELEASE）
 */
uint8_t keyState = KEY_STATE_RELEASE;

/* 按键检测函数，通过有限状态机实现
 * 函数在从等待状态转换到按键按下状态时返回 PRESSED，代表按键已被触发
 * 其他情况返回 NOT_PRESSED
 */
uint8_t keyDetect(void)
{
    static uint8_t duriation;  // 用于在等待状态中计数
    switch(keyState)
    {
        case KEY_STATE_RELEASE:
            if(readKey() == 1)     // 如果按键按下
            {
                keyState = KEY_STATE_WAITING;  // 转换至下一个状态
            }
            return NOT_PRESSED;    // 返回：按键未按下
            break;
        case KEY_STATE_WAITING:
            if(readKey() == 1)     // 如果按键按下
            {
                duriation++;
                if(duriation >= DURIATION_TIME)    // 如果经过多次检测，按键仍然按下
                {   // 说明没有抖动了，可以确定按键已按下
                    duriation = 0;
                    keyState = KEY_STATE_PRESSED;  // 转换至下一个状态
                    return PRESSED;
                }
            }
            else  // 如果此时按键松开
            {   // 可能存在抖动或干扰
                duriation = 0;  // 清零的目的是便于下次重新计数
                keyState = KEY_STATE_RELEASE;  // 重新返回按键松开的状态
                return NOT_PRESSED;
            }
            break;
        case KEY_STATE_PRESSED:
            if(readKey() == 0)       // 如果按键松开
            {
                keyState = KEY_STATE_RELEASE;  // 回到按键松开的状态
            }
            return NOT_PRESSED;
            break;
        default:
            keyState = KEY_STATE_RELEASE;
            return NOT_PRESSED;
    }
}