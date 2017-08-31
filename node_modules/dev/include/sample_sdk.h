// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef TEST_H
#define TEST_H

#include "MORPHO_Interface.h"

#define MAX_FIELD_SIZE 64
#define MAX_FIELD_NAME_LEN 6
#define INPUT_BUFFER_SIZE 128
#define MAX_RECORDS 100

#define CHECK_RESULT(cmd,testcase,expected)	do{\
	if (cmd == expected) fprintf(stdout,"OK:     %s (%d as expected)\n",testcase,cmd);\
	else fprintf(stdout,"FAILED: %s (%d, expected %d)",testcase,cmd,expected);\
	}while(0);

typedef enum {
    DEVICE_UNKNOWN,
    DEVICE_CBI,
    DEVICE_MSI,
    DEVICE_CBM,
    DEVICE_MSO,
    DEVICE_FVP
} E_DEVICE_TYPE;

typedef struct{
	C_MORPHO_Device			m_x_device;
	C_MORPHO_Database		m_x_database;
	C_MORPHO_User			m_x_user;
	C_MORPHO_TemplateList	m_x_templates;
	E_DEVICE_TYPE           m_e_deviceType;
}T_DATA, *PT_DATA;

I deviceOperation(PT_DATA io_data);
I databaseOperation(PT_DATA io_data);
I userOperation(PT_DATA io_data);
//I openDeviceOperation(PT_DATA io_data);

T_MORPHO_TYPE_TEMPLATE getTemplateType();
void getTemplateExtension(T_MORPHO_TYPE_TEMPLATE i_x_TypeTemplate,PC o_pc_extension);

I printDatabaseFields(C_MORPHO_Database &i_x_database, T_MORPHO_FIELD_ATTRIBUTE i_e_fieldType, PUL o_pul_nbFields, C_MORPHO_UserList *o_px_userList);
I printUserFields (C_MORPHO_User &i_x_user, UL i_ul_nbFields);
I listDatabaseUsers(PT_DATA io_px_data);
I storePkFromFile(const char *i_pc_fileName, C_MORPHO_TemplateList &io_x_template);
I getUserInput(char o_auc_buffer[INPUT_BUFFER_SIZE], const char*message);
I getTemplateFileFormat(T_MORPHO_TYPE_TEMPLATE *o_px_TypeTemplate, char *io_pc_fileExtension);
I fillNewUserFields(PT_DATA	io_px_data);

I eventCallback(
		PVOID						i_pv_context,
		T_MORPHO_CALLBACK_COMMAND	i_i_command,
		PVOID						i_pv_param);

int LEDEvent(
	const void *param,
	int state);

int FFDEvent(int* ffdState);

void *cancelThread(void *userParam);
void *qualityThread(void *userParam);

#if 0   // Redefinition
#ifndef CreateMutex
HANDLE CreateMutex(PUC i_puc_Inutile, UC i_b_initialOwner, PUC i_puc_Name)
{
	return NULL;
}
#endif

#ifndef CloseHandle
int CloseHandle(HANDLE i_h_Handle)
{
	return 0;
}
#endif

#ifndef WaitForSingleObject
int WaitForSingleObject(HANDLE i_h_Handle, DWORD i_dw_Timeout)
{
	return 0;
}
#endif

#ifndef ReleaseMutex
int ReleaseMutex(HANDLE i_h_Handle)
{
	return 0;
}
#endif
#endif  // Win32 synchronization objects redefinition

#endif
