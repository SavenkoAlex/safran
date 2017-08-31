// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef MORPHO_INTERNAL_H
#define MORPHO_INTERNAL_H

#define MAGIC_SDK_START		0x55
#define MAGIC_SDK_END		0xAA

#define LOG_SDK_FILE		"log_sdk.log" // File to create for sdk logs

/*#define FCT_ENTER			0
#define FCT_EXIT			1
#define FCT_ELSE			2*/

typedef enum {	CBM, 
				CBMELV,
				CBMEMV,
				MSO_ENROL, 
				MSO_VERIF, 
				FVP, MTOP
} MPHCS_SensorType;


typedef enum {	LIVE_FINGER = 1, 
				FAKE_FINGER = 2
} MPHCS_Status;

typedef enum {	LUT,
				NOLUT 
} MPHCS_Bio;


typedef enum {	LOW,
				MEDIUM,
				HIGH
} MPHCS_SecurityLevel;

// define pour la lib MSOCS
typedef I (*F_MSOCS_Init)(PI);
typedef VOID (*F_MSOCS_Close)(VOID);
typedef I (*F_MSOCS_Check)(
							const I						i_i_Seed,
							const PUC					i_puc_ScrambledImage,
							const UL					i_ul_ScrambledImageSize,
							const MPHCS_SensorType		i_x_SensorType,
							const MPHCS_Bio				i_x_ImageType,
							const MPHCS_SecurityLevel	i_x_SecurityLevel);


#if defined(WIN32)
	#include "tchar.h"
#endif


#if 0	//TODO: move to wrapper module
#if defined(WIN32) || defined(_WIN32_WCE)
	#define GETFUNCADDRESS(a,b)	GetProcAddress(a,b)
	#define LOADDLL(a,b)		LoadLibrary(a)	
	#define CLOSEDLL(a)			FreeLibrary(a)
	#define LIB_MSO_CS			_T("LibMSOCS.dll")
	#define RTLD_LAZY			""	
#else
	#include <dlfcn.h>
	#define GETFUNCADDRESS(a,b) dlsym(a,b)
	#define LOADDLL(a,b)		dlopen(a,b)
	#define CLOSEDLL(a)			dlclose(a)
	#define LIB_MSO_CS			"libmsocs.so"
#endif
#endif//wrapper

// end define pour la lib MSOCS



#define CHECK_FATHER(i_str_FctStr,i_px_Var,i_i_ErrorNumber)							\
		if (i_px_Var == NULL)																		\
		{																							\
			MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d)",i_i_ErrorNumber);	\
			return MORPHOERR_NO_ASSOCIATED_DEVICE;													\
		}

#define CHECK_MAGIC(i_str_FctStr)																						\
		if ( (m_ul_magicFirst != MAGIC_SDK_START) || (m_ul_magicEnd != MAGIC_SDK_END) )													\
		{																																\
			MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_CORRUPTED_CLASS)",MORPHOERR_CORRUPTED_CLASS);	\
			return MORPHOERR_CORRUPTED_CLASS;																							\
		}

#define CHECK_MAGIC_NRV(i_str_FctStr)									\
		if ( (m_ul_magicFirst != MAGIC_SDK_START) || (m_ul_magicEnd != MAGIC_SDK_END) )	\
		{																				\
			MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,NULL);				\
			return ;																	\
		}

#define REGISTER_MAGIC																	\
		m_ul_magicFirst = MAGIC_SDK_START;												\
		m_ul_magicEnd = MAGIC_SDK_END;

#define	REGISTER_CHECK(i_px_Var)														\
		i_px_Var=NULL;

#define	CHECK(i_str_FctStr,i_px_Var)			CHECK_MAGIC(i_str_FctStr)								\
						if (i_px_Var == NULL)																										\
						{																															\
							MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_INVALID_CLASS)",MORPHOERR_INVALID_CLASS);	\
							return MORPHOERR_INVALID_CLASS;		 																					\
						} 

#define	CHECK_WRAPPER(i_str_FctStr,i_px_Var,o_ppx_LocalPtr)																											\
						if (i_px_Var == NULL)																														\
						{																																			\
							MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_NO_ASSOCIATED_DEVICE)",MORPHOERR_NO_ASSOCIATED_DEVICE);				\
							return MORPHOERR_NO_ASSOCIATED_DEVICE;		 																							\
						}																																			\
						else /*if (o_ppx_LocalPtr != NULL)*/																												\
						{																																			\
							*o_ppx_LocalPtr = (PT_MORPHO_WRAPPER_INTERFACE)i_px_Var;																					\
						}

#define	CHECK_NRV(i_str_FctStr,i_px_Var)		CHECK_MAGIC_NRV(i_str_FctStr)	\
						if (!i_px_Var)																		\
						{																					\
							MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,NULL);					\
							return;																			\
						}

//#define REGISTER_CALLBACK_COMMAND(i_str_FctStr)
			//MSO_RegisterAsyncILV(	l_h_Mso100HandleInternal,
#define REGISTER_CALLBACK_COMMAND(i_str_FctStr,px_wrapper)																	\
			if (i_ul_CallbackCmd & MORPHO_CALLBACK_COMMAND_CMD)																				\
			{																																\
				if (i_pf_Callback == NULL)																									\
				{																															\
					MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_BADPARAMETER)",MORPHOERR_BADPARAMETER);	\
					return MORPHOERR_BADPARAMETER;																							\
				}																															\
				px_wrapper->RegisterAsyncILV(	l_h_Mso100HandleInternal,																	\
										MORPHO_CALLBACK_COMMAND_CMD,																		\
										(T_pFuncILV_Buffer)i_pf_Callback,																	\
										i_pv_CallbackArgument	);																			\
			}

//#define REGISTER_CALLBACK_IMAGE(i_str_FctStr)
#define REGISTER_CALLBACK_IMAGE(i_str_FctStr,px_wrapper)																		\
			if (i_ul_CallbackCmd & MORPHO_CALLBACK_IMAGE_CMD)																				\
			{																																\
				if (i_pf_Callback == NULL)																									\
				{																															\
					MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_BADPARAMETER)",MORPHOERR_BADPARAMETER);	\
					return MORPHOERR_BADPARAMETER;																							\
				}																															\
				px_wrapper->RegisterAsyncILV(	l_h_Mso100HandleInternal,																	\
										MORPHO_CALLBACK_IMAGE_CMD, 																			\
										(T_pFuncILV_Buffer)i_pf_Callback,																	\
										i_pv_CallbackArgument	);																			\
			}																																\
	
//#define REGISTER_CALLBACK_ENROLL(i_str_FctStr)
#define REGISTER_CALLBACK_ENROLL(i_str_FctStr,px_wrapper)																	\
  		if (i_ul_CallbackCmd & MORPHO_CALLBACK_ENROLLMENT_CMD)																				\
		{																																	\
			if (i_pf_Callback == NULL)																										\
			{																															\
				MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_BADPARAMETER)",MORPHOERR_BADPARAMETER);	\
				return MORPHOERR_BADPARAMETER;																							\
			}																															\
			px_wrapper->RegisterAsyncILV(	l_h_Mso100HandleInternal,																		\
									MORPHO_CALLBACK_ENROLLMENT_CMD,																			\
									(T_pFuncILV_Buffer)i_pf_Callback,																		\
									i_pv_CallbackArgument	);																				\
		}																																	\

//#define REGISTER_CALLBACK_LAST_IMAGE(i_str_FctStr)
#define REGISTER_CALLBACK_LAST_IMAGE(i_str_FctStr,px_wrapper)															\
  		if (i_ul_CallbackCmd & MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD)																	\
		{																																\
			if (i_pf_Callback == NULL)																									\
			{																															\
				MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_BADPARAMETER)",MORPHOERR_BADPARAMETER);	\
				return MORPHOERR_BADPARAMETER;																							\
			}																															\
			px_wrapper->RegisterAsyncILV(	l_h_Mso100HandleInternal,																	\
									MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD,															\
									(T_pFuncILV_Buffer)i_pf_Callback,																	\
									i_pv_CallbackArgument	);																			\
		}																																\


 //#define REGISTER_CALLBACK_CODEQUALITY(i_str_FctStr)
#define REGISTER_CALLBACK_CODEQUALITY(i_str_FctStr,px_wrapper)																\
			if (i_ul_CallbackCmd & MORPHO_CALLBACK_CODEQUALITY)																				\
			{																																\
				if (i_pf_Callback == NULL)																									\
				{																															\
					MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_BADPARAMETER)",MORPHOERR_BADPARAMETER);	\
					return MORPHOERR_BADPARAMETER;																							\
				}																															\
				px_wrapper->RegisterAsyncILV(	l_h_Mso100HandleInternal,																	\
										MORPHO_CALLBACK_CODEQUALITY,																		\
										(T_pFuncILV_Buffer)i_pf_Callback,																	\
										i_pv_CallbackArgument	);																			\
			}

//#define REGISTER_CALLBACK_DETECTQUALITY(i_str_FctStr)
#define REGISTER_CALLBACK_DETECTQUALITY(i_str_FctStr,px_wrapper)																\
			if (i_ul_CallbackCmd & MORPHO_CALLBACK_DETECTQUALITY)																			\
			{																																\
				if (i_pf_Callback == NULL)																									\
				{																															\
					MORPHO_Wrapper_Log(NULL,FCT_EXIT,i_str_FctStr,"Ret = %d (MORPHOERR_BADPARAMETER)",MORPHOERR_BADPARAMETER);	\
					return MORPHOERR_BADPARAMETER;																							\
				}																															\
				px_wrapper->RegisterAsyncILV(	l_h_Mso100HandleInternal,																	\
										MORPHO_CALLBACK_DETECTQUALITY,																		\
										(T_pFuncILV_Buffer)i_pf_Callback,																	\
										i_pv_CallbackArgument	);																			\
			}

	


//#define	UNREGISTER_CALLBACK_COMMAND
#define	UNREGISTER_CALLBACK_COMMAND(px_wrapper)								\
		if (i_ul_CallbackCmd & MORPHO_CALLBACK_COMMAND_CMD)					\
		{																	\
			px_wrapper->UnregisterAsyncILV(	l_h_Mso100HandleInternal,		\
									MORPHO_CALLBACK_COMMAND_CMD );			\
		}

//#define	UNREGISTER_CALLBACK_IMAGE
#define	UNREGISTER_CALLBACK_IMAGE(px_wrapper)								\
		if (i_ul_CallbackCmd & MORPHO_CALLBACK_IMAGE_CMD)					\
		{																	\
			px_wrapper->UnregisterAsyncILV(	l_h_Mso100HandleInternal,		\
									MORPHO_CALLBACK_IMAGE_CMD );			\
		}

//#define	UNREGISTER_CALLBACK_LAST_IMAGE
#define	UNREGISTER_CALLBACK_LAST_IMAGE(px_wrapper)								\
		if (i_ul_CallbackCmd & MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD)			\
		{																		\
			px_wrapper->UnregisterAsyncILV(	l_h_Mso100HandleInternal,			\
									MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD );	\
		}

//#define	UNREGISTER_CALLBACK_ENROLL
#define	UNREGISTER_CALLBACK_ENROLL(px_wrapper)								\
		if (i_ul_CallbackCmd & MORPHO_CALLBACK_ENROLLMENT_CMD)				\
		{																	\
			px_wrapper->UnregisterAsyncILV(	l_h_Mso100HandleInternal,		\
									MORPHO_CALLBACK_ENROLLMENT_CMD );		\
		}

//#define	UNREGISTER_CALLBACK_CODEQUALITY
#define	UNREGISTER_CALLBACK_CODEQUALITY(px_wrapper)							\
		if (i_ul_CallbackCmd & MORPHO_CALLBACK_CODEQUALITY)					\
		{																	\
			px_wrapper->UnregisterAsyncILV(	l_h_Mso100HandleInternal,		\
									MORPHO_CALLBACK_CODEQUALITY );			\
		}

//#define	UNREGISTER_CALLBACK_DETECTQUALITY
#define	UNREGISTER_CALLBACK_DETECTQUALITY(px_wrapper)						\
		if (i_ul_CallbackCmd & MORPHO_CALLBACK_DETECTQUALITY)				\
		{																	\
			px_wrapper->UnregisterAsyncILV(	l_h_Mso100HandleInternal,		\
									MORPHO_CALLBACK_DETECTQUALITY );		\
		}


#if 0
I	ConvertStatus(I & io_i_Ret, UC i_uc_Status);

I	ConvertError(MORPHO_HANDLE i_h_Mso100Handle,
				 I & io_i_Ret, 
				 UC i_uc_Status, 
				 UL i_ul_EmbeddedError,
				 PT_MORPHO_WRAPPER_INTERFACE i_px_wrapper);

I	ConvertError(MORPHO_HANDLE i_h_Mso100Handle,
				 I & io_i_Ret,
				 UC i_uc_Status,
				 UL i_ul_EmbeddedError);

void MorphoLog(int i_i_InOut, const char * i_pc_FctName, const char * format,...);
#endif


UC ConvertMorphoTypeTemplateToMsoId(T_MORPHO_TYPE_TEMPLATE i_x_typTemplate);
T_MORPHO_TYPE_TEMPLATE ConvertMsoIdToMorphoTypeTemplate(UC i_uc_MsoId);

UC ConvertMorphoTypeTemplateToBioAlgoParamId(T_MORPHO_TYPE_TEMPLATE i_x_TemplateType);
UC ConvertMorphoTypeTemplateToFVPId(T_MORPHO_FVP_TYPE_TEMPLATE  i_x_typTemplate);
T_MORPHO_TYPE_TEMPLATE ConvertBioAlgoParamIdToMorphoTypeTemplate(UC i_uc_BioId);

UC ConvertMorphoSmartCardWorkflowTypeToMsoId(T_MORPHO_SMARTCARD_WORKFLOW_TYPE i_x_MorphoSmartCardWorkflowType);


#endif
