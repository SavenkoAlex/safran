// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef MSO_TYPES_H
#define MSO_TYPES_H

#include "MORPHO_Types.h"

#ifndef HOST
// Embedded Rules
	#if defined(__ARMCC_VERSION)			// RVDS 
		#include "sections_mapping.h"
		#ifdef APPLI
			#include "swi_num.h"
		#endif
	#elif defined(__GNUC__)					// GCC
		//#define PACKED __packed
		#define __swi(a) //
		#include "sections_mapping.h"
		#ifdef APPLI
			#include "arm-swi.h"
			#include "swi_num.h"
		#endif
	#endif
#else
	// Host Rules
	#ifndef PKG_KERNEL
		#define PKG_KERNEL
	#endif
	#ifndef PKG_SPILV
		#define PKG_SPILV
	#endif
	#ifndef PKG_SECU
		#define PKG_SECU
	#endif
	#ifndef PKG_SENSOR
		#define PKG_SENSOR
	#endif

	#ifdef __linux__
		#if !defined( LINUX )
			#define LINUX
		#endif
	#endif
#endif

/* PortageLP64 : Chnage the types long and unsigned long */
typedef T__UINT32				mot;
typedef T__SINT32				MSO_HANDLE;
typedef unsigned char 			T_EVT, *PT_EVT;


#ifndef TRUE
#define TRUE	(1)
#endif
#ifndef FALSE
#define FALSE	(0)
#endif
#ifndef NULL
#define NULL	(0)
#endif

typedef struct T_HANDLE
{
	B	m_b_Open;
	UC	m_uc_Num;
}T_HANDLE, * PT_HANDLE;


typedef struct
{
	UL				m_ul_bufLen;
	PUC				m_puc_buffer;
	UL				m_ul_current;
	BOOLEAN			m_b_ToAllocate;			/* FALSE means that 'm_puc_buffer' must be released by the caller, TRUE means 'm_puc_buffer' will be automatically desallocated after a transfer  */
	BOOLEAN			m_b_DMA;				/* TRUE means data must be allign on 4 bytes and will be sent by DMA */
	US				m_us_I;					/* id of the ILV main*/
} T_BUFFER, *PT_BUFFER;


typedef struct
{
	UL			m_ul_Size;
	PUC			m_puc_Buf;
}T_BUF, *PT_BUF;


typedef struct
{
	B			m_b_IsAuthorized;
	B			m_b_SecureRequest;
}T_ILV_AUTHORIZED_SECUREREQUEST, *PT_ILV_AUTHORIZED_SECUREREQUEST;

typedef struct
{
	I			m_i_len;
	PUC			m_puc_buffer;
}T_SECU_BUF, *PT_SECU_BUF;

typedef struct
{
	T_BUF		m_x_Buf;
	UC			m_uc_IdPk;

// Options for Iso Pk, see ID_ISO_PK_PARAM ILV
	UC			m_uc_Index;					// Need for Iso Pk

// Options for Smart Card, see ID_PK_FROM_SMARTCARD
	UC			m_uc_SMC_WorkFlowCardType;	// Work Flow type to read the templates in the Smart Card
	UC			m_uc_SMC_typTemplate;		// Template type in the Smart Card
	UC			m_uc_SMC_TemplateNumber;	// Number of Templates to read in the Smart Card

	UC			m_uc_Pad[3];
}T_BUF_PK, *PT_BUF_PK;


typedef T_MORPHO_IMAGE_HEADER		T_IMAGE_HEADER;
#define IMAGE_HEADER_REVISION		0
#define	IMAGE_HEADER_SIZE(l_x_hdr)	(sizeof(l_x_hdr)-sizeof(l_x_hdr.m_uc_HeaderRevision)-sizeof(l_x_hdr.m_uc_HeaderSize))

/** @brief This structure contains information about the WSQ image format. */
typedef struct
{
    US					m_us_HeaderRevision;	/**< Header revision (0 with current release) */
	US					m_us_HeaderSize;		/**< Header size (8 bytes) */
	//short				padding;				/**< padding*/
	L					m_l_CompressionReached;	/**< Compression reached in WSQ */
	L					m_l_DimImgCompresed;	/**< image size */
}T_WSQ_IMAGE_HEADER;

#define WSQ_IMAGE_HEADER_REVISION	0
#define WSQ_IMAGE_HEADER_SIZE       8

typedef struct
{
	I						m_i_lenTemplate;
	T_MORPHO_TYPE_TEMPLATE	m_x_typTemplate;	// MORPHO_PK_COMP, MORPHO_PK_MAT_NORM, MORPHO_PK_COMP_NORM, MORPHO_PK_MAT
	UC						m_auc_pad[3];
	PUC						m_puc_Template;
}T_TEMPLATE, *PT_TEMPLATE;


//////////////////////////////////////////////////////////////////
//		OTP
//////////////////////////////////////////////////////////////////
// Constantes OTP : tailles paramètres et valeurs min / max
#define OTP_SIZE_MAX_PASSWORD					63
#define OTP_SIZE_MIN_PASSWORD					10
#define OTP_SIZE_MAX_SEED						16
#define OTP_SIZE_MIN_SEED						1

#define OTP_SIZE_PIN_PARAMETER					5		//5 octets: 2 (Pin)+2(Alea)+1(CRC)
#define OTP_SIZE_USER_DATA						1024


// Structure des paramètres OTP
typedef struct
{
	T__UINT64		m_u64_SequenceNumber;
	UC		m_auc_padding[2];
	US		m_us_Param;
	UC		m_auc_Password[OTP_SIZE_MAX_PASSWORD+1];
	UC		m_auc_Pin[OTP_SIZE_PIN_PARAMETER + 1];
	UC		m_auc_Pad[2];
	UC		m_auc_OtpUserData[OTP_SIZE_USER_DATA];
	UL		m_ul_OtpUserDataSize;
}
T_OTP_PARAMETERS_STRUCT,*PT_OTP_PARAMETERS_STRUCT;

// Structure de génération des OTP
typedef struct
{
	UL		m_ul_AsynchronousEvt;
	T__UINT64	m_u64_SequenceNumber;
	UC		m_auc_padding[2];
	UC		m_uc_AlgoHash;
	UC		m_auc_Seed[OTP_SIZE_MAX_SEED+1];
}
T_OTP_GENERATE_STRUCT,*PT_OTP_GENERATE_STRUCT;

#endif

