/*
*****************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：com.c
* 摘    要：com模块API及内部函数实现
*
* 当前版本：1.3
* 作    者：厉蒋
* 完成日期：2005年7月18日
*
* 取代版本：1.2
* 作    者：孟晓楠
* 完成日期：2005年7月8日
*
* 取代版本：1.1
* 作    者：吴先明
* 完成日期：2005年3月8日
*
* 取代版本：1.0
* 原作者  ：孙洁、厉蒋
* 完成日期：2004年12月10日
*********************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif
#include "com_int.c"/*内部通信*/

#ifndef COM_INVALID_MODE
#define COM_INVALID_MODE 255 /*非法模式*/
#endif/*#ifndef COM_INVALID_MODE*/

#ifdef COM_ERROR_HOOK
    INT8U  COMNestedErrorHook=0;
#ifdef COM_HOOK_GET_SERVICEID    
    COMServiceIdType  ServiceId=COMServiceId_NoService;
#endif /*ifdef OS_HOOK_GET_SERVICEID*/ 

#ifdef COM_HOOK_PARA_ACCESS
    INT32U    COMObjId; 
#endif/*#ifdef COM_USE_PARAMETERACCESS*/ 

#endif/*#ifdef COM_ERROR_HOOK*/ 
static COMApplicationModeType COMCurrentMode;/*当前运行模式*/
void COMInternalInit();
StatusType COMSendIntMessage(SymbolicName sender,ApplicationDataRef data);
StatusType COMReceiveIntMessage(SymbolicName sender,ApplicationDataRef data);
/*
*************************************************************************************
* StatusType StartCOM(ApplicationMode mode)
*
* 描述：            OSEK COM API。启动COM
*
* 调用的函数：      COMInternalInit
*					StartCOMExtension
*
* 允许被调用的位置：
*
* 参数：	        
*
* 返回：	        标准状态：E_OK                 启动成功
*                             E_COM_START_FAILED   消息对象配置信息错误，初始化失败 
*                             
*                   扩展状态：E_COM_ID			   参数mode越界
*					
*
* 注意事项：	    的返回值
*
* 创建人：          厉蒋
*
* 时间：            2004年12月27日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
*
* 修改人：          厉蒋
*
* 时间：            2005年7月18日
*************************************************************************************
*/
StatusType StartCOM(COMApplicationModeType mode)
{
#ifdef COM_EXTENDED_STATUS
    /*参数mode的范围确定后*/
    if(mode>=COM_INVALID_MODE) 
    {     
        COMRETERROR(E_COM_ID,COMServiceId_StartCOM,mode);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/
	COMCurrentMode=mode; 
    COMInternalInit();
#ifdef	COM_START_EXTENTSION    
    return StartCOMExtension();/*用户扩展的初始化，比如写InitMessage*/
#else/*#ifdef	COM_START_EXTENTSION*/    
    return E_OK;
#endif/*#ifdef	COM_START_EXTENTSION*/ 
}
/*
*************************************************************************************
* StatusType StopCOM (COMShutdownModeType mode)
*
* 描述：            OSEK COM API。关闭COM。所有COM在使用的资源被返回或置于非活动状态。
*                   数据可能丢失 
*
* 调用的函数：      
*
* 允许被调用的位置：
*
* 参数：	        mode：    关闭模式。只能是COM_SHUTDOWN_IMMEDIATE
*
* 返回：	        标准状态：E_OK          COM关闭成功
*
*				    扩展状态: E_COM_ID      消息参数message非法
*
* 注意事项：	    
*
* 创建人：          吴先明
*
* 时间：            2005年3月17日
*
* 修改人：郁利吉
* 
* 时间：2005.11.10
*************************************************************************************
*/
StatusType StopCOM  ( COMShutdownModeType mode )
{
#ifdef COM_EXTENDED_STATUS
    if(mode != COM_SHUTDOWN_IMMEDIATE)
    {
        COMRETERROR(E_COM_ID,COMServiceId_StopCOM,mode);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/
    
    mode=mode;
    COMInternalStop();
    return E_OK;
}
/*
*************************************************************************************
* COMApplicationModeType GetCOMApplicationMode (void)
*
* 描述：            OSEK COM API。返回当前COM应用模式。可能用于写依赖于模式的应用程序
*
* 调用的函数：      
*
* 允许被调用的位置：COM启动之后。否则会出现未定义的行为。
*
* 参数：	        无
*
* 返回：	        当前COM应用模式  
*
* 注意事项：	    COM应用模式翻译自COM Application Mode
*
* 创建人：          吴先明
*
* 时间：            2005年3月17日
*************************************************************************************
*/
COMApplicationModeType GetCOMApplicationMode (void)
{
    return COMCurrentMode;
}
/*
*************************************************************************************
* StatusType SendMessage(SymbolicName message,ApplicationDataRef data)
*
* 描述：            OSEK COM API。把dataref里的数据发送到消息对象message里。
*
* 调用的函数：      FilterMessage
*					SetNotification
*
* 允许被调用的位置：
*
* 参数：	        message   发送消息对象名称
*					data:     用户数据所在地址
*
* 返回：	        标准状态：E_OK          消息发送成功
* 
*				    扩展状态: E_COM_ID      消息参数message越界；message为接收消息对象；
*                                           message为零长度或动态长度消息
*
* 注意事项：	    
*
* 创建人：          厉蒋
*
* 时间：            2004年12月22日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
*
* 修改人：          孟晓楠
*
* 时间：            2005年6月28日
*
* 修改人：          厉蒋
*
* 时间：            2005年7月18日
*************************************************************************************
*/
StatusType SendMessage(SymbolicName message,ApplicationDataRef data)
{
#ifndef COM_EXTERNAL_EN
    return COMSendIntMessage(message,data);	/*内部通信*/
#endif/*#ifndef COM_EXTERNAL_EN*/    
}

/*
*************************************************************************************
* StatusType ReceiveMessage(SymbolicName message,ApplicationDataRef data)
*
* 描述：            OSEK COM API。把接收消息对象message里的数据接收到data里，并更新参
*					数message相关联的所有flag
*
* 调用的函数：      FilterMessage
*
* 允许被调用的位置：
*
* 参数：	        message     接收消息对象名称
*					dataref     用户数据所在地址
*
* 返回：	        标准状态：  E_OK         消息接收成功
*					            E_COM_NOMSG  接收消息对象message为空
*                               E_COM_LIMIT  消息队列溢出
*
*         
*                   扩展状态：  E_COM_ID     接收消息对象message越界；message为发送
*                                            消息对象；该消息为零长度或为动态长度							             
*
* 注意事项：	    
*
* 创建人：          厉蒋
*
* 时间：            2004年12月22日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
*
* 修改人：          厉蒋
*
* 时间：            2005年7月18日
*************************************************************************************
*/
StatusType ReceiveMessage(SymbolicName message,ApplicationDataRef data)
{
#ifndef COM_EXTERNAL_EN
    return COMReceiveIntMessage(message,data);	/*内部通信*/
#endif/*#ifndef COM_EXTERNAL_EN*/  
}
