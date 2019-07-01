#define FAN_OFF_DELAY  (60*1000*5)  // 设置延时关闭时间为5min。

// 定义3种状态，且初态为“风扇关闭”状态。
enum {
  FSM_FAN_OFF,  // “风扇关闭”状态。
  FSM_FAN_ON,   // “风扇开启”状态。
  FSM_FAN_DELAY // “风扇延时等待”状态。
} FSM_FAN_State = FSM_FAN_OFF;

void TaskFanControl(void)
{
  static uint32_t fsmFanTimer = 0;
  switch(FSM_FAN_State)
  {
    case FSM_FAN_OFF :
      FAN_OFF;
      if(RunState == true)
      {
        FSM_FAN_State = FSM_FAN_ON;
      }
      break;
    case FSM_FAN_ON :
      FAN_ON;
      if(RunState == false)
      {
        FSM_FAN_State = FSM_FAN_DELAY;
      }
      break;
    case FSM_FAN_DELAY :
      if(RunState == true)
      {
        fsmFanTimer = 0;
        FSM_FAN_State = FSM_FAN_ON;
      }
      else
      {
        fsmFanTimer++;
        if(fsmFanTimer >= FAN_OFF_DELAY)
        {
          fsmFanTimer = 0;
          FSM_FAN_State = FSM_FAN_OFF;
        }
      }
      break;
    default :
      FSM_FAN_State = FSM_FAN_OFF;
      break;
  }
}