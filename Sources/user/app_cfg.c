
OSSTK	Task1Stack[Task1StackSize];
TaskType Task1=&OSTCBTbl[Task1Prio];
OSSTK	Task2Stack[Task2StackSize];
TaskType Task2=&OSTCBTbl[Task2Prio];
OSSTK	Task3Stack[Task3StackSize];
TaskType Task3=&OSTCBTbl[Task3Prio];
OSSTK	TaskErrorStack[TaskErrorStackSize];
TaskType TaskError=&OSTCBTbl[TaskErrorPrio];
EventMaskType	Event1=0x1;
EventMaskType	Event2=0x2;
AlarmType	AlarmError=&OSAlarms[0];
AlarmType	Alarm1=&OSAlarms[1];
AlarmType	Alarm2=&OSAlarms[2];
AlarmType	Alarm3=&OSAlarms[3];
AlarmType	Alarm4=&OSAlarms[4];
AlarmType	Alarm5=&OSAlarms[5];
AlarmType	Alarm6=&OSAlarms[6];
AlarmType	Alarm7=&OSAlarms[7];

const OSTaskCfg OSTaskCfgTbl[]=
{
/*  {Task entry, High stack addr, Low stack addr, ID,Runing Priority, Information}*/
	{FuncTask1,&Task1Stack[Task1StackSize-2],Task1Stack,Task1Prio,Task1Prio,0|EXTENDED|AUTO_START},/*Task1*/
	{FuncTask2,&Task2Stack[Task2StackSize-2],Task2Stack,Task2Prio,Task2Prio,0|FULL_SCHED|AUTO_START},/*Task2*/
	{FuncTask3,&Task3Stack[Task3StackSize-2],Task3Stack,Task3Prio,Task3Prio,0|FULL_SCHED|EXTENDED|AUTO_START},/*Task3*/
	{FuncTaskError,&TaskErrorStack[TaskErrorStackSize-2],TaskErrorStack,TaskErrorPrio,TaskErrorPrio,0|FULL_SCHED|AUTO_START},/*TaskError*/
};
const AlarmBaseType OSCounterCfgTbl[]=
{
/*  {Max allowed value, Ticks per base,Minimun cycle}*/
	{100000,2
#ifdef OS_EXTENDED_STATUS
		,2
#endif
	},/*SystemTimer*/

};

const OSAlarmCfg OSAlarmCfgTbl[]=
{
/*  {Counter,Auto start,Increment,Cycle,Task priority,EventMask,Callback}*/
	{SystemTimer,0,0,0,TaskErrorPrio,NULL,NULL},/*AlarmError*/
	{SystemTimer,ALARM_AUTO_START,200000,2,Task1Prio,NULL,NULL},/*Alarm1*/
	{SystemTimer,ALARM_AUTO_START,200000,2,Task2Prio,NULL,NULL},/*Alarm2*/
	{SystemTimer,ALARM_AUTO_START,200000,2,Task3Prio,NULL,NULL},/*Alarm3*/
	{SystemTimer,ALARM_AUTO_START,200000,2,Task3Prio,&Event1,NULL},/*Alarm4*/
	{SystemTimer,ALARM_AUTO_START,200000,2,Task3Prio,&Event2,NULL},/*Alarm5*/
	{SystemTimer,ALARM_AUTO_START,200000,2,Task1Prio,NULL,NULL},/*Alarm6*/
	{SystemTimer,ALARM_AUTO_START,200000,2,Task2Prio,NULL,NULL},/*Alarm7*/
};
