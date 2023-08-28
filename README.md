# Learning-FSM（有限状态机）
## 概念
1. 定义：有限状态机（finite-state machine，FSM，简称状态机）是表示有限个状态
        以及在这些状态之间的转移和动作等行为的数学模型。
2. 在嵌入式程序设计中，如果一个系统需要处理一系列连续发生的任务，或在不同的模式
   下对输入进行不同的处理，常常使用有限状态机。
## 作用
1. 分解耗时过长的任务。
2. 避免软件延时对CPU资源的浪费。
3. 使程序逻辑更加清晰。
## 实现
1. 通过switch-case语句实现。
2. 通过Arduino的[FSM开源库](http://playground.arduino.cc/Code/FiniteStateMachine)实现。
3. 其它方式：对于一些更复杂的任务，用switch-case语句，代码会不太简洁。
## 流程图
1. 状态机的实现需要完善的流程图指导。
2. 好的流程图才能设计出好的代码。
3. 流程图绘制软件推荐：[draw.io](https://app.diagrams.net)
