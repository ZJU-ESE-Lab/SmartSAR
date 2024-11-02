## Introduction：
 SmartOSEK Platform is designed to meet the domestic demand for automotive electronic control systems in China's automotive electronics field. It focuses on developing an embedded real-time operating system that complies with the international OSEK standard for automotive electronic control. Prioritizing ease of use, reliability, and efficiency, it provides a complete integrated development environment toolset. SmartOSEK OS implements the OSEK standard OS specifications BCC1 and ECC1, the OSEK COM standard CCCA and CCCB, and supports OSEK OIL implementation. It supports various international mainstream embedded chips and includes a toolset for graphical modeling programming design, visual system configuration, visual OIL parsing and configuration, graphical OS runtime tracking, and graphical task scheduling analysis.


## Features:
**1. Ultra-micro real-time operating system kernel:**

    1)Kernel size < 10k
    
    2)Context switch time < 8us
  
    3)Interrupt response time < 36us
**2. Complies with OSEK OS, OSEK COM, OSEK OIL standards:**

  1)OSEK standard-compliant API
  
  2)OSEK standard-compliant system development process
  
  3)OSEK standard-compliant external communication mechanisms
**3. Supports multiple scheduling mechanisms:**

  1)Preemptive scheduling
  
  2)Non-preemptive scheduling
  
  3)Hybrid scheduling
**4. Supports static configuration**
**5. Supports various mainstream embedded processors**


## Getting started: 
This repository is the trial version of SmartOSEK 2.0 standard status, specifically for the HCS12 platform. It has been compiled and runs successfully on CodeWarrior version CW12 V3.1. Users can run the program using the CW12 simulator or burn it onto the target board using P&E Multilink, with results output via the serial port. 
## Note:
The differences in usage from the official version are as follows:
1. Supports extended status.
2. Task scheduling is hybrid (MIX). To achieve full preemption, define all tasks as  
preemptive; for non-preemption, define all tasks as non-preemptive.
3. Does not support OS_HOOK_GET_SERVICEID, OS_HOOK_PARAM_ACCESS.
4. Does not support internal communication.
5. The system kernel does not support static reduction.
## API列表：
| 序号/分类 | Function          | Parameter                            | description                                                                                                                                           |  
| --------- | --------------- | ------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------- |  
| 1         | ActivateTask    | `TaskType TaskID`               | 激活处于暂停状态的任务（TaskID），将其置于准备队列中，并根据用户定义的调度策略执行任务调度（如果条件满足）。                                       |  
| 2         | GetTaskState    | `TaskType TaskID, TaskStateRefType state` | 获取指定任务（TaskID）的状态，并将其存储在`state`中。                                                                                         |  
| 3         | TerminateTask   | `void`                          | 如果条件满足，则挂起当前任务。它不能在中断服务例程中调用。当前任务必须释放已获取的所有外部资源。挂起后，根据用户定义的调度策略执行任务调度。         |  
| 4         | GetTaskID       | `TaskRefType PTaskID`           | 获取当前正在运行的任务的ID，并将其存储在`PTaskID`中。                                                                                           |  
| 5         | ChainTask       | `TaskType TaskID`               | 挂起当前任务，并激活处于暂停状态的任务（TaskID），将其置于准备队列中，并根据用户定义的调度策略执行任务调度（如果条件满足）。此API不能在中断服务例程中调用，当前任务必须释放已获取的所有外部资源。 |


## Contributing
We welcome all contributions to this project. Please refer to   
[the documents](https://github.com/ZJU-ESE-Lab/SmartSAR/blob/main/Contribution_guides.md)
<a id="jump_8"></a> for relevant information.
