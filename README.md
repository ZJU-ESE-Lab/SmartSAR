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
| Function Name                  | Parameters                                                   | Description                                                                                                                                                                                                                         |  
|--------------------------------|--------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------| 
| ActivateTask   | `TaskType TaskID`                | Activates a task in the suspend state, placing it in the ready queue, and executes task scheduling based on the user-defined scheduling policy, if conditions are met.                                       |  
| GetTaskState   | `TaskType TaskID, TaskStateRefType state` | Retrieves the state of the specified task (TaskID).                                                                                         |  
| TerminateTask  | `void`                           | Suspends the current task if conditions are met. It cannot be called in an interrupt service routine. The current task must have released any acquired external resources. After suspension, task scheduling is executed based on the user-defined scheduling policy. |  
| GetTaskID      | `TaskRefType PTaskID`            | Retrieves the ID of the currently running task.                                                                                      |  
| ChainTask      | `TaskType TaskID`                | Suspends itself and activates a task in the suspend state (TaskID), placing it in the ready queue, and executes task scheduling based on the user-defined scheduling policy, if conditions are met. This API cannot be called in an interrupt service routine, and the current task must have released any acquired external resources. |

 
| `EnableAllInterrupts`          | `void`                                                       | Enables all interrupts.                                                                                                                                                                                                           |  
| `ResumeAllInterrupts`          | `void`                                                       | Enables all interrupts, corresponding to `SuspendAllInterrupts`.                                                                                                                                                                   |  
| `DisableAllInterrupts`         | `void`                                                       | Disables all interrupts without retaining those triggered during the disabled period.                                                                                                                                            |  
| `SuspendOSInterrupts`          | `void`                                                       | Disables category 2 interrupts while retaining those triggered during the disabled period.                                                                                                                                         |  
| `SuspendAllInterrupts`         | `void`                                                       | Disables all interrupts while retaining those triggered during the disabled period.                                                                                                                                              |  
| `ResumeOSInterrupts`           | `void`                                                       | Enables category 2 interrupts, corresponding to `SuspendOSInterrupts`.                                                                                                                                                             |  
| `GetAlarmBase`                 | `AlarmType AlarmID, AlarmBaseRefType Info`                    | Queries the counter status to obtain information about the counter for the Alarm with ID `<AlarmID>`.                                                                                                                             |  
| `GetAlarm`                     | `AlarmType Alarm,ID TickRefType tick`                         | Queries the alarm statusType to increment obtain, the Tick relativeType ticks cycle value from the current time to the trigger time for the Alarm with ID `<AlarmID>`, and returns it.                                                                           |  
| `SetRelAlarm`                  | `AlarmType AlarmID, Tick`       | After this function is called, it occupies the Alarm with ID `<AlarmID>`. After `<increment>` clock ticks, the corresponding task or event will be triggered. If `Cycle != 0`, the ALARM will be triggered every `Cycle` ticks.         |  
| `SetAbsAlarm`                  | `AlarmType AlarmID, TickType start, TickType cycle`           | After this module is called, it occupies the Alarm with ID `<AlarmID>`. When the system clock reaches `<start>`, the corresponding task or event will be triggered. If `Cycle != 0`, the ALARM will be triggered every `Cycle` ticks.   |  
| `CancelAlarm`                  | `AlarmType AlarmID`                                          | Cancels the use of the Alarm with ID `<AlarmID>`.                                                                                                                                                                                 |  
| `SetEvent`                     | `TaskType TaskID, EventMaskType Mask`                         | Sets the event for the task represented by `TaskID` according to the `Mask`. Once any of the awaited events are set, the task enters the ready state, and scheduling is executed based on the user-defined policy.                     |  
| `GetEvent`                     | `TaskType TaskID, EventMaskRefType Event`                     | Returns the event status of the task represented by `TaskID`. It is only effective for extended tasks that are not in the suspended state.                                                                                          |  
| `ClearEvent`                   | `EventMaskType Mask`                                         | Clears the current task's events according to the `Mask`. It is only effective for extended tasks and cannot be called at the interrupt level.                                                                                       |  
| `WaitEvent`                    | `EventMaskType Mask`                                         | Puts the current task into a waiting state until the event represented by `Mask` occurs. If the awaited event is already set, the task continues without scheduling. It is only effective for extended tasks and cannot be called at the interrupt level. The task should have released any acquired external resources. |  
| `GetResource`                  | `ResourceType resId`                                         | The current task acquires the external or scheduler resource identified by `resId`, setting the task's priority to the resource's priority.                                                                                           |  
| `ReleaseResource`              | `ResourceType resId`                                         | Releases the external or scheduler resource held by the current task, restores the task's priority, and then executes task scheduling based on the user-defined policy.                                                               |
## Contributing
We welcome all contributions to this project. Please refer to [the documents](https://github.com/ZJU-ESE-Lab/SmartSAR/blob/main/Contribution_guides.md)
<a id="jump_8"></a> for relevant information.

## License
This project is released under the [Apache 2.0 license](https://github.com/ZJU-ESE-Lab/SmartSAR/blob/main/LICENSE).
