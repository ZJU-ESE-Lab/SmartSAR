#ifndef APPCFG_H
#define APPCFG_H
/*-------------The value of TaskError's attributes--------------*/

/*priority(format:name+"Prio")*/
#define		TaskErrorPrio		4

/*TaskID(format:name")*/
extern TaskType	TaskError;

/*stack size (format: name+"StackSize")*/
#define		TaskErrorStackSize		100
extern OSSTK	TaskErrorStack[];

/*DeclareTask*/

DeclareTask(TaskError);

/*-------------The value of Task3's attributes--------------*/

/*priority(format:name+"Prio")*/
#define		Task3Prio		3

/*TaskID(format:name")*/
extern TaskType	Task3;

/*stack size (format: name+"StackSize")*/
#define		Task3StackSize		100
extern OSSTK	Task3Stack[];

/*DeclareTask*/

DeclareTask(Task3);

/*-------------The value of Task2's attributes--------------*/

/*priority(format:name+"Prio")*/
#define		Task2Prio		2

/*TaskID(format:name")*/
extern TaskType	Task2;

/*stack size (format: name+"StackSize")*/
#define		Task2StackSize		100
extern OSSTK	Task2Stack[];

/*DeclareTask*/

DeclareTask(Task2);

/*-------------The value of Task1's attributes--------------*/

/*priority(format:name+"Prio")*/
#define		Task1Prio		1

/*TaskID(format:name")*/
extern TaskType	Task1;

/*stack size (format: name+"StackSize")*/
#define		Task1StackSize		100
extern OSSTK	Task1Stack[];

/*DeclareTask*/

DeclareTask(Task1);

/*-------------The priority of resources--------------*/

/*------------The declaration of event:Event1--------------*/
/*mask (format:event's name+"=mask")*/
DeclareEvent(Event1);

/*------------The declaration of event:Event2--------------*/
/*mask (format:event's name+"=mask")*/
DeclareEvent(Event2);

/*------------The declaration of counter:SystemTimer--------------*/
/*counter definition(format:counter's name)*/
#define	SystemTimer	0
/*maxallowedvalue(format:uppercase name+"MAV")*/
#define		SYSTEMTIMER_MAV	100000
/*mincycle(format:uppercase name+"MC")*/
#define		SYSTEMTIMER_MC	2
/*ticksperbase(format:uppercase name+"TS")*/
#define		SYSTEMTIMER_TPB	2

/*------------The declaration of alarm:AlarmError--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(AlarmError);

/*------------The declaration of alarm:Alarm1--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(Alarm1);

/*------------The declaration of alarm:Alarm2--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(Alarm2);

/*------------The declaration of alarm:Alarm3--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(Alarm3);

/*------------The declaration of alarm:Alarm4--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(Alarm4);

/*------------The declaration of alarm:Alarm5--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(Alarm5);

/*------------The declaration of alarm:Alarm6--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(Alarm6);

/*------------The declaration of alarm:Alarm7--------------*/
/*alarm (format:alarm's name)*/
DeclareAlarm(Alarm7);

#endif
