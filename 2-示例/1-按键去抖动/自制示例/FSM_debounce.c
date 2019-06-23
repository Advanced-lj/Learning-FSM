#define debounceTime  20
// 定义“按键状态机”的3种状态：按键未按下、等待、按键按下。
// 初始化状态机的默认状态为FSM_Button_Release。
enum {
  FSM_Button_Release, // “按键未按下”状态。
  FSM_Button_Waiting, // “等待”（消抖）状态。
  FSM_Button_Pressed  // “按键按下”状态。
} FSM_Button_State = FSM_Button_Release;

// 定义按钮的动作状态：按下、释放、长按、双击。。。。。
typedef enum {
  BUTTON_Pressed,
  BUTTON_Release,
  // BUTTON_LongPress,
  // BUTTON_DoubleClick
} buttonState_TypeDef;

buttonState_TypeDef curButtonState = BUTTON_Release;

// 置于while(1)中按1ms轮询。
void buttonCK(void)
{
  static uint8_t fsmButtonTimer = 0;
  switch(FSM_Button_State) 
  {
    case FSM_Button_Release :
      if(getButtonPin() == Low)
      {
        FSM_Button_State = FSM_Button_Waiting;
      }
      curButtonState = BUTTON_Release;
      break;
    case FSM_Button_Waiting :
      // fsmButtonTimer++;
      // if(fsmButtonTimer >= debounceTime)
      // {
      //   fsmButtonTimer = 0;
      //   if(getButtonPin() == High)
      //   {
      //     FSM_Button_State = FSM_Button_Release;
      //   }
      //   else 
      //   {
      //     FSM_Button_State = FSM_Button_Pressed;
      //   }
      // }
      if(getButtonPin() == Low) // 按键按下。
      {
        fsmButtonTimer++;
        if(fsmButtonTimer >= debounceTime) // 如果经过多次检测，按键仍然按下。
        {                                  // 说明没有抖动了，可以确定按键已按下。
          fsmButtonTimer = 0;
          FSM_Button_State = FSM_Button_Pressed;
          curButtonState = BUTTON_Pressed;
        }
      }
      else // 如果此时按键松开。
      {    // 可能存在抖动或干扰。
        fsmButtonTimer = 0;
        FSM_Button_State = FSM_Button_Release;
        curButtonState = BUTTON_Release;
      }
      break;
    case FSM_Button_Pressed :
      if(getButtonPin() == High)
      {
        FSM_Button_State = FSM_Button_Release;
      }
      curButtonState = BUTTON_Pressed;
      break;
    default :
      FSM_Button_State = FSM_Button_Release;
      curButtonState = BUTTON_Release;
      break;
  }
}

buttonState_TypeDef getButtonState(void)
{
  return curButtonState;
}