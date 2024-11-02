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
## API List：
| Function       | Parameter                          | Description                                                                                                                                       |  
|----------------|----------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------|  
| ActivateTask   | `TaskType TaskID`                | Activates a task in the suspend state, placing it in the ready queue, and executes task scheduling based on the user-defined scheduling policy, if conditions are met.                                       |  
| GetTaskState   | `TaskType TaskID, TaskStateRefType state` | Retrieves the state of the specified task (TaskID).                                                                                         |  
| TerminateTask  | `void`                           | Suspends the current task if conditions are met. It cannot be called in an interrupt service routine. The current task must have released any acquired external resources. After suspension, task scheduling is executed based on the user-defined scheduling policy. |  
| GetTaskID      | `TaskRefType PTaskID`            | Retrieves the ID of the currently running task.                                                                                      |  
| ChainTask      | `TaskType TaskID`                | Suspends itself and activates a task in the suspend state (TaskID), placing it in the ready queue, and executes task scheduling based on the user-defined scheduling policy, if conditions are met. This API cannot be called in an interrupt service routine, and the current task must have released any acquired external resources. |


## Contributing
We welcome all contributions to this project. Please refer to [the documents](https://github.com/ZJU-ESE-Lab/SmartSAR/blob/main/Contribution_guides.md)
<a id="jump_8"></a> for relevant information.

## License
This project is released under the [Apache 2.0 license](https://github.com/ZJU-ESE-Lab/SmartSAR/blob/main/LICENSE) .
