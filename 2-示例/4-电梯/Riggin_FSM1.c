/**
  * @Description : 状态机
  * @Version     : V1.0.0
  * @Author      : Riggin
  * @Date        : 2021-11-28 21:02:51
  * @LastEditors : Riggin
  * @LastEditTime: 2023-08-28 09:55:07
  * @   █████▒█    ██  ▄████▄   ██ ▄█▀       ██████╗ ██╗   ██╗ ██████╗
  * @ ▓██   ▒ ██  ▓██▒▒██▀ ▀█   ██▄█▒        ██╔══██╗██║   ██║██╔════╝
  * @ ▒████ ░▓██  ▒██░▒▓█    ▄ ▓███▄░        ██████╔╝██║   ██║██║  ███╗
  * @ ░▓█▒  ░▓▓█  ░██░▒▓▓▄ ▄██▒▓██ █▄        ██╔══██╗██║   ██║██║   ██║
  * @ ░▒█░   ▒▒█████▓ ▒ ▓███▀ ░▒██▒ █▄       ██████╔╝╚██████╔╝╚██████╔╝
  * @  ▒ ░   ░▒▓▒ ▒ ▒ ░ ░▒ ▒  ░▒ ▒▒ ▓▒       ╚═════╝  ╚═════╝  ╚═════╝
  * @  ░     ░░▒░ ░ ░   ░  ▒   ░ ░▒ ▒░
  * @  ░ ░    ░░░ ░ ░ ░        ░ ░░ ░
  * @           ░     ░ ░      ░  ░
  */

#include <stdint.h>
#include <stddef.h>

typedef unsigned char uint8_t;

// 状态。
typedef enum
{
    S0 = 0,
    S1 = 1,
    S2 = 2,
    num_of_state
} state_t;

// 事件。
typedef enum
{
    E0 = 0,
    E1 = 1,
    E2 = 2,
    num_of_event
} event_t;


// 状态机节点。
struct fsm_node
{
    void (*fpAction)(void *pEvnt); // 动作。
    uint8_t u8NxtStat;             // 下一个状态。
};

void action_S0E0(void *pEvnt) {

}
void action_S0E1(void *pEvnt) {

}
void action_S0E2(void *pEvnt) {

}
void action_S1E0(void *pEvnt) {

}
void action_S1E1(void *pEvnt) {

}
void action_S1E2(void *pEvnt) {

}
void action_S2E0(void *pEvnt) {

}
void action_S2E1(void *pEvnt) {

}
void action_S2E2(void *pEvnt) {

}

struct fsm_node Node_S0E0 = {action_S0E0, S1};
struct fsm_node Node_S0E1 = {action_S0E1, S2};
struct fsm_node Node_S0E2 = {action_S0E2, S0};
struct fsm_node Node_S1E0 = {action_S1E0, S1};
struct fsm_node Node_S1E1 = {action_S1E1, S0};
struct fsm_node Node_S1E2 = {action_S1E2, S2};
struct fsm_node Node_S2E0 = {action_S2E0, S0};
struct fsm_node Node_S2E1 = {action_S2E1, S0};
struct fsm_node Node_S2E2 = {action_S2E2, S0};

// 状态机驱动表格。
struct fsm_node g_arFsmDrvTbl[][num_of_event] =
{
          //    E0         E1         E2
    [S0] = { Node_S0E0, Node_S0E1, Node_S0E2 }, // S0
    [S1] = { Node_S1E0, Node_S1E1, Node_S1E2 }, // S1
    [S2] = { Node_S2E0, Node_S2E1, Node_S2E2 }  // S2
};


uint8_t u8CurStat = S0;                 /*状态暂存*/
uint8_t u8EvntTyp = E0;                 /*事件类型暂存*/
struct fsm_node stNodeTmp = {NULL, S0}; /*状态机节点暂存*/
void *pEvnt = NULL;                     /*事件变量地址暂存*/

void set_cur_state(uint8_t state) {
    u8CurStat = state;
}

uint8_t get_cur_state(void) {
    return u8CurStat;
}

// get_cur_evnt_typ() 采用一个消息队列来存放事件，队列会保留事件发生的先后顺序。（包括内部事件和外部事件）
// get_cur_evnt_ptr() 获取事件的指针，（事件指针包括事件的类型和事件的内容）
int main(void) {
    u8CurStat = get_cur_state();                     /*读取当前状态*/
    u8EvntTyp = get_cur_evnt_typ();                  /*读取当前触发事件类型*/
    pEvnt = (void *)get_cur_evnt_ptr();              /*读取事件变量地址*/
    stNodeTmp = g_arFsmDrvTbl[u8CurStat][u8EvntTyp]; /*定位状态机节点*/
    stNodeTmp.fpAction(pEvnt);                       /*动作响应*/
    set_cur_state(stNodeTmp.u8NxtStat);              /*状态迁移*/
}
