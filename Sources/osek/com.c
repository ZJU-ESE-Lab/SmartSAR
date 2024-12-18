/*
*****************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�com.c
* ժ    Ҫ��comģ��API���ڲ�����ʵ��
*
* ��ǰ�汾��1.3
* ��    �ߣ�����
* ������ڣ�2005��7��18��
*
* ȡ���汾��1.2
* ��    �ߣ������
* ������ڣ�2005��7��8��
*
* ȡ���汾��1.1
* ��    �ߣ�������
* ������ڣ�2005��3��8��
*
* ȡ���汾��1.0
* ԭ����  ����ࡢ����
* ������ڣ�2004��12��10��
*********************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif
#include "com_int.c"/*�ڲ�ͨ��*/

#ifndef COM_INVALID_MODE
#define COM_INVALID_MODE 255 /*�Ƿ�ģʽ*/
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
static COMApplicationModeType COMCurrentMode;/*��ǰ����ģʽ*/
void COMInternalInit();
StatusType COMSendIntMessage(SymbolicName sender,ApplicationDataRef data);
StatusType COMReceiveIntMessage(SymbolicName sender,ApplicationDataRef data);
/*
*************************************************************************************
* StatusType StartCOM(ApplicationMode mode)
*
* ������            OSEK COM API������COM
*
* ���õĺ�����      COMInternalInit
*					StartCOMExtension
*
* �������õ�λ�ã�
*
* ������	        
*
* ���أ�	        ��׼״̬��E_OK                 �����ɹ�
*                             E_COM_START_FAILED   ��Ϣ����������Ϣ���󣬳�ʼ��ʧ�� 
*                             
*                   ��չ״̬��E_COM_ID			   ����modeԽ��
*					
*
* ע�����	    �ķ���ֵ
*
* �����ˣ�          ����
*
* ʱ�䣺            2004��12��27��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*
* �޸��ˣ�          ����
*
* ʱ�䣺            2005��7��18��
*************************************************************************************
*/
StatusType StartCOM(COMApplicationModeType mode)
{
#ifdef COM_EXTENDED_STATUS
    /*����mode�ķ�Χȷ����*/
    if(mode>=COM_INVALID_MODE) 
    {     
        COMRETERROR(E_COM_ID,COMServiceId_StartCOM,mode);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/
	COMCurrentMode=mode; 
    COMInternalInit();
#ifdef	COM_START_EXTENTSION    
    return StartCOMExtension();/*�û���չ�ĳ�ʼ��������дInitMessage*/
#else/*#ifdef	COM_START_EXTENTSION*/    
    return E_OK;
#endif/*#ifdef	COM_START_EXTENTSION*/ 
}
/*
*************************************************************************************
* StatusType StopCOM (COMShutdownModeType mode)
*
* ������            OSEK COM API���ر�COM������COM��ʹ�õ���Դ�����ػ����ڷǻ״̬��
*                   ���ݿ��ܶ�ʧ 
*
* ���õĺ�����      
*
* �������õ�λ�ã�
*
* ������	        mode��    �ر�ģʽ��ֻ����COM_SHUTDOWN_IMMEDIATE
*
* ���أ�	        ��׼״̬��E_OK          COM�رճɹ�
*
*				    ��չ״̬: E_COM_ID      ��Ϣ����message�Ƿ�
*
* ע�����	    
*
* �����ˣ�          ������
*
* ʱ�䣺            2005��3��17��
*
* �޸��ˣ�������
* 
* ʱ�䣺2005.11.10
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
* ������            OSEK COM API�����ص�ǰCOMӦ��ģʽ����������д������ģʽ��Ӧ�ó���
*
* ���õĺ�����      
*
* �������õ�λ�ã�COM����֮�󡣷�������δ�������Ϊ��
*
* ������	        ��
*
* ���أ�	        ��ǰCOMӦ��ģʽ  
*
* ע�����	    COMӦ��ģʽ������COM Application Mode
*
* �����ˣ�          ������
*
* ʱ�䣺            2005��3��17��
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
* ������            OSEK COM API����dataref������ݷ��͵���Ϣ����message�
*
* ���õĺ�����      FilterMessage
*					SetNotification
*
* �������õ�λ�ã�
*
* ������	        message   ������Ϣ��������
*					data:     �û��������ڵ�ַ
*
* ���أ�	        ��׼״̬��E_OK          ��Ϣ���ͳɹ�
* 
*				    ��չ״̬: E_COM_ID      ��Ϣ����messageԽ�磻messageΪ������Ϣ����
*                                           messageΪ�㳤�Ȼ�̬������Ϣ
*
* ע�����	    
*
* �����ˣ�          ����
*
* ʱ�䣺            2004��12��22��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*
* �޸��ˣ�          �����
*
* ʱ�䣺            2005��6��28��
*
* �޸��ˣ�          ����
*
* ʱ�䣺            2005��7��18��
*************************************************************************************
*/
StatusType SendMessage(SymbolicName message,ApplicationDataRef data)
{
#ifndef COM_EXTERNAL_EN
    return COMSendIntMessage(message,data);	/*�ڲ�ͨ��*/
#endif/*#ifndef COM_EXTERNAL_EN*/    
}

/*
*************************************************************************************
* StatusType ReceiveMessage(SymbolicName message,ApplicationDataRef data)
*
* ������            OSEK COM API���ѽ�����Ϣ����message������ݽ��յ�data������²�
*					��message�����������flag
*
* ���õĺ�����      FilterMessage
*
* �������õ�λ�ã�
*
* ������	        message     ������Ϣ��������
*					dataref     �û��������ڵ�ַ
*
* ���أ�	        ��׼״̬��  E_OK         ��Ϣ���ճɹ�
*					            E_COM_NOMSG  ������Ϣ����messageΪ��
*                               E_COM_LIMIT  ��Ϣ�������
*
*         
*                   ��չ״̬��  E_COM_ID     ������Ϣ����messageԽ�磻messageΪ����
*                                            ��Ϣ���󣻸���ϢΪ�㳤�Ȼ�Ϊ��̬����							             
*
* ע�����	    
*
* �����ˣ�          ����
*
* ʱ�䣺            2004��12��22��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*
* �޸��ˣ�          ����
*
* ʱ�䣺            2005��7��18��
*************************************************************************************
*/
StatusType ReceiveMessage(SymbolicName message,ApplicationDataRef data)
{
#ifndef COM_EXTERNAL_EN
    return COMReceiveIntMessage(message,data);	/*�ڲ�ͨ��*/
#endif/*#ifndef COM_EXTERNAL_EN*/  
}
