// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_Types.h
 * @brief Definition of various MORPHO types
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 *
 */

#ifndef MORPHO_TYPES_H
#define MORPHO_TYPES_H

#if !defined(__MOBI5) && !defined(FVP_V1)
	#ifndef HOST
		#define HOST
	#endif
#endif

#if (defined(WIN32) || defined(_WIN32_WCE)) && !defined(__GNUC__)
	#include "windows.h"

		#include "crtdbg.h"
		#define ENABLE_SECU_ILV


	//Definition for compatibility
	/* _countof helper */
	#if !defined(_countof)
	#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
	#endif

	#if ( _MSC_VER <= 1300 ) || defined(_WIN32_WCE)
	// Simples String Manipulation Definition for compatibility
	__inline int strncpy_s ( char *strDest, size_t numberOfElements, const char *strSource, size_t count )
	{
		_ASSERT ( numberOfElements > count );
		strDest[count] = 0;
		strncpy ( strDest, strSource, count );
		return 0;
	}
	#define strncat_s(A,B,C,D){strncat(A,C,D);}
	#define strcat_s(Dst,NElt,Src){strncat(Dst,Src,NElt-1);}
	#define strcpy_s(Dst,NElt,Src)	{	char* tmp;					\
										tmp = Dst+NElt-1;			\
										*tmp = 0;					\
										strncpy(Dst,Src,NElt-1);	\
									}
	#endif
#endif

// Types
#if !defined(WIN32) && !defined(_WIN32_WCE)
#ifndef BOOL
typedef unsigned char			BOOL;
#endif
#endif

typedef unsigned char			BOOLEAN;
typedef unsigned char			B;

typedef unsigned char			UC;
typedef char					C;
typedef unsigned short			US;
typedef short					S;
#ifndef MORPHO_TYPES_UI_NO_DEFINE
typedef unsigned int			UI;
#define MORPHO_TYPES_UI_NO_DEFINE
#endif

/* PortageLP64 :
 * This is the modification to ensure the portability of code on x64 Linux platforms
 * Change type: unsigned long by T__UINT32
 * Change type: long by T__SINT32
 */
#if defined( __LP64__ ) || defined( _LP64 )
    typedef int				L;			/* L forced to 32bits on 64bits platform */
    typedef int				T__SINT32;
    typedef unsigned int	UL;			/* UL forced to 32bits on 64bits platform */
    typedef unsigned int	T__UINT32;
#else
    typedef long			L;
    typedef long			T__SINT32;
    typedef unsigned long	UL;
    typedef unsigned long	T__UINT32;
#endif

typedef int						I;
typedef unsigned long long		ULL;

typedef unsigned char 			T__UINT8;
typedef char 					T__SINT8;
typedef unsigned short			T__UINT16;
typedef short 					T__SINT16;
#if defined(WIN32)
typedef unsigned __int64		T__UINT64;
typedef __int64 				T__SINT64;
#else
typedef unsigned long long int	T__UINT64;
typedef long long int 			T__SINT64;
#endif

#define VOID	void
#define PUC 	UC*
#define PC		C*
#define PS		S*
#define PUS		US*
#define PUI		unsigned int *
#define PI		I*
#define PUL		UL*
#define PL		L*
#define PVOID	VOID*

/** @brief This type is used to identify the connection to a specific MorphoSmart™ */
typedef void*				MORPHO_HANDLE;

#if defined(__GNUC__) && defined(HOST) && !defined(MA2G)
	#include "CompatLinux.h"
#endif

#if defined(MA2G)
	#include "CompatMA2G.h"
#endif

// constants
/** @brief User unique identifier maximum size in local database */
#define MORPHO_USER_ID_MAXLEN			24
/** @brief The timeout error of the callback receive function */
#define COMERR_TIMEOUT_RECEIVE			-10000
/** @brief Concatenation of MorphoSmart™ product ID and MorphoSmart™ serial number
           Real length is 21, but KT authentication requires this length to be multiple of 8 */
#define MSO_SERIAL_NUMBER_LEN			24
/** @brief The maximum number of template authorized */
#define	MORPHO_LIST_CANDIDATE_PK		2

#define	LIC_MAX_SIZE				1024
#define	LIC_ALEA_SIZE				4

#define LOCK_KEY_NUMBER				5
#define	LOCK_ID_KEY_SIZE			8
#define	LOCK_KEY_SIZE				24
#define	LOCK_ALEA_SIZE				8

/** @brief If this bit is set, the tunneling protocol is activated */
#define SECU_TUNNELING				0x01	// default = no secure pipe
/** @brief If this bit is set, the offered Security protocol is activated. */
#define SECU_OFFERED_SECURITY		0x02	// default = no secure pipe
/** @brief If this bit is set, only templates with a digital signature are accepted. */
#define SECU_PK_ONLY_SIGNED			0x04	// default = accept all PK
/** @brief If this bit is set, the firmware must be digitally signed by Morpho.
           Note: For a secure MorphoSmart™, this option is always activated
           and can not be modified. */
#define SECU_SIGNED_DOWNLOAD		0x08	// default = downloaded modules are not signed. Once set, it is not anymore possible to discard this flag.
/** @brief If this bit is set, the score can not be exported */
#define SECU_NEVER_EXPORT_SCORE		0x10	// default = score is exported
/** @brief  */
#define SECU_FFD					0x20
/** @brief  */
#define SECU_LOCK					0x40	// Lock secret loaded
#define SECU_ENCRYPTED_DB			0x80    // Database is encrypted


// MSO
/** @brief Low False Finger Detection security level */
#define FFD_SECURITY_LEVEL_LOW_HOST                       0x0000
/** @brief Medium False Finger Detection security level */
#define FFD_SECURITY_LEVEL_MEDIUM_HOST					  0x0001
/** @brief High False Finger Detection security level */
#define FFD_SECURITY_LEVEL_HIGH_HOST                      0x0002
/** @brief This level allows disabling FFD functionality */
#define FFD_SECURITY_LEVEL_NONE_HOST                      0x0003

/** @brief Undefined Security level */
#define FFD_SECURITY_LEVEL_DEFAULT_HOST					  0xFFFF

//FVP
/** @brief Low False Multimodal Detection security level */
#define  MULTIMODAL_SECURITY_STANDARD					 0x0000
/** @brief Medium False Multimodal Detection security level */
#define  MULTIMODAL_SECURITY_MEDIUM					     0x0200
/** @brief High False Multimodal Detection security level */
#define  MULTIMODAL_SECURITY_HIGH						 0x0100

//FVP 1.10 ************ START *****************
/** @brief Default Startegy Acquisition Mode */
#define MORPHO_ACQ_EXPERT_MODE               0
/** @brief Fast Univeral Startegy Acquisition Mode */
#define MORPHO_ACQ_UNIVERSAL_FAST_MODE       1
/** @brief Slow but Accurate Univeral Startegy Acquisition Mode */
#define MORPHO_ACQ_UNIVERSAL_ACCURATE_MODE   2
/** @brief Full Multimodal Startegy Acquisition Mode */
#define MORPHO_ACQ_FULL_MULTIMODAL_MODE      3
/** @brief Detecting Spoofing Startegy Acquisition Mode */
#define MORPHO_ACQ_ANTI_SPOOFING_MODE        4

//Campaign tool enhancement
/** Export one best FP image out of all attempt (This option is default) */
#define MORPHO_EXPORT_BEST_FP_IMAGE_ALL_ATTEMPTS 1
/** Export one FP image per attempt */
#define MORPHO_EXPORT_BEST_FP_IMAGE_EACH_ATTEMPT 0

//CODER CHOICE
/** @brief Default coder, this setting exists since the 08.04 firmware version and allow the MorphoSmart™ to keep its default choice*/
#define	MORPHO_DEFAULT_CODER			0
/** @brief Standard coder selection (this is the default choice) */
#define	MORPHO_MSO_V9_CODER				3
/** @brief Juvenile coder selection */
#define MORPHO_MSO_V9_JUV_CODER			7
/** @brief Thin finger coder selection */
#define MORPHO_MSO_V9_THIN_FINGER_CODER	8

//DETECT MODE CHOICE
/** @brief the detection mode is choosen by the scanner*/
#define	MORPHO_DEFAULT_DETECT_MODE					0
/** @brief fast detection mode */
#define	MORPHO_VERIF_DETECT_MODE					1
/** @brief stronger detection mode */
#define	MORPHO_ENROLL_DETECT_MODE					2
/** @brief uses a 'led off' presence detection (only on MorphoSmart&tm; MSOxx1) */
#define MORPHO_WAKEUP_LED_OFF						4
/** @brief the finger must cover an area starting at the top of the image (only with the enrolement detection mode)*/
#define MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE		16
/** @brief uses a 'led on' presence detection (only on MorphoSmart&tm; FINGER VP) */
#define MORPHO_WAKEUP_LED_ON						64

//MATCHING STRATEGY CHOICE
/** @brief Standard matching strategy selection (this is the default) */
#define	MORPHO_STANDARD_MATCHING_STRATEGY		0
/** @brief Advanced matching strategy selection - less FRR but more processing time */
#define MORPHO_ADVANCED_MATCHING_STRATEGY	1

//LATENT DETECT CHOICE
/** disable the latency detection (this is the default) */
#define LATENT_DETECT_DISABLE		0
/** enable the latency detection*/
#define LATENT_DETECT_ENABLE		1

// constructed types
/** @brief Those values are used to define how tight the matching threshold is.
           The table below defines the available FAR values. For more information on False
		   Acceptance Rate, please refer to the MorphoSmart Overview.pdf documentation. */
typedef enum {
	MORPHO_FAR_0 = 0,	/**< Very low threshold for test purpose only */
	MORPHO_FAR_1,		/**< FAR < 1 % */
	MORPHO_FAR_2,		/**< FAR < 0.5 % */
	MORPHO_FAR_3,		/**< FAR < 0.1 % */
	MORPHO_FAR_4,		/**< FAR < 0.05 % */
	MORPHO_FAR_5,		/**< Recommended value: FAR < 0.01 % */
	MORPHO_FAR_6,		/**< FAR < 0.001 % */
	MORPHO_FAR_7,		/**< FAR < 0.0001 % */
	MORPHO_FAR_8,		/**< FAR < 0.00001 % */
	MORPHO_FAR_9,		/**< FAR < 0.0000001 % */
	MORPHO_FAR_10		/**< Very high threshold for test purpose only */
} T_MORPHO_FAR,*PT_MORPHO_FAR;

/** @brief This structure contains information about the image format. */
typedef struct
{
	UC					m_uc_HeaderRevision;	/**< Header revision (0 with current release) */
	UC					m_uc_HeaderSize;		/**< Header size (10 bytes) */
	US					m_us_NbRow;				/**< Image row number in pixels, */
	US					m_us_NbCol;				/**< Image column number in pixels */
	US					m_us_ResY;				/**< Vertical resolution in dpi */
	US					m_us_ResX;				/**< Horizontal resolution in dpi */
	UC					m_uc_CompressionType;	/**< Compression algorithm used, ID_COMPRESSION_NULL, ID_COMPRESSION_WSQ or ID_COMPRESSION_V1 */
	UC					m_uc_NbBitsPerPixel;	/**< Number of bits per pixel */
}T_MORPHO_IMAGE_HEADER;

/** @brief This structure contains information about the WSQ image format. */
typedef struct
{
	US					m_us_HeaderRevision;	/**< Header revision (0 with current release) */
	US					m_us_HeaderSize;		/**< Header size (10 bytes) */
	UL					m_ul_CompressionRatio;	/**< Compression ratio * 1000, reached for output image	4 bytes */
	UL					m_ul_WSQImageSize;	    /**< WSQ Image size in bytes	4 bytes */
}T_MORPHO_IMAGE_WSQ_HEADER;


/** @brief Those values are used to define the asynchronous status events returned by the
    T_MORPHO_CALLBACK_ FUNCTIONS. There are also used to create the binary mask that describes
	the asynchronous status events that will trig the callback function.
 */
typedef enum {
	MORPHO_CALLBACK_COMMAND_CMD				= (1U<<0),		/**< This asynchronous status event identifies a command status information (see T_MORPHO_COMMAND_STATUS) */
	MORPHO_CALLBACK_IMAGE_CMD				= (1U<<1),		/**< This asynchronous status event identifies a low-resolution image (see T_MORPHO_CALLBACK_IMAGE_STATUS) */
	MORPHO_CALLBACK_ENROLLMENT_CMD			= (1U<<2),		/**< This asynchronous status event identifies an enrollment status (see T_MORPHO_CALLBACK_ENROLLMENT_STATUS) */
	MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD	= (1U<<3),		/**< This asynchronous status event identifies the last image from a live acquisition which is returned in full resolution (see T_MORPHO_CALLBACK_IMAGE_STATUS) */
	MORPHO_CALLBACK_RESERVED1				= (1U<<4),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED2				= (1U<<5),		/**< NOT USED */
	MORPHO_CALLBACK_CODEQUALITY				= (1U<<6),		/**< This asynchronous status event identifies the status of the quality note of the image detained to be coded. (please see the MorphoSmartHostSystemInterfaceSpecification document for the details) */
	MORPHO_CALLBACK_DETECTQUALITY			= (1U<<7),		/**< This asynchronous status event identifies the status of the quality note calculated by the "presence detection" function. (please see the MorphoSmartHostSystemInterfaceSpecification document for the details) */
	MORPHO_CALLBACK_RESERVED3				= (1U<<8),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED4				= (1U<<9),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED5				= (1U<<10),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED11				= (1U<<11),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED12				= (1U<<12),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED13				= (1U<<13),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED14				= (1U<<14),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED15				= (1U<<15),		/**< NOT USED */

	MORPHO_CALLBACK_RESERVED16				= (1U<<16),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED17				= (1U<<17),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED18				= (1U<<18),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED19				= (1U<<19),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED20				= (1U<<20),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED21				= (1U<<21),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED22				= (1U<<22),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED23				= (1U<<23),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED24				= (1U<<24),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED25				= (1U<<25),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED26				= (1U<<26),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED27				= (1U<<27),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED28				= (1U<<28),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED29				= (1U<<29),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED30				= (1U<<30),		/**< NOT USED */
	MORPHO_CALLBACK_RESERVED31				= (1U<<31)		/**< NOT USED */
} T_MORPHO_CALLBACK_COMMAND;

/** @brief Those values are used to define a finger status */
typedef enum {
	MORPHO_MOVE_NO_FINGER,			/**< The terminal waits for the user's finger. */
	MORPHO_MOVE_FINGER_UP,			/**< The user must move his/her finger up. */
	MORPHO_MOVE_FINGER_DOWN,		/**< The user must move his/her finger down. */
	MORPHO_MOVE_FINGER_LEFT,		/**< The user must move his/her finger to the left. */
	MORPHO_MOVE_FINGER_RIGHT, 		/**< The user must move his/her finger to the right. */
	MORPHO_PRESS_FINGER_HARDER,		/**< The user must press his/her finger harder for the device to acquire a larger fingerprint image. */
	MORPHO_LATENT,					/**< The system has detected a latent fingerprint in the input fingerprint. Please change finger position. */
	MORPHO_REMOVE_FINGER,			/**< User must remove his finger. */
	MORPHO_FINGER_OK,				/**< The finger acquisition was correctly completed. */
	MORPHO_FINGER_DETECTED,			/**< The finger has been detected by DTPR HARD : palm and finger are detected (MorphoSmart&tm; FINGER VP only) */
	MORPHO_FINGER_MISPLACED,		/**< The finger has been detected misplaced by DTPR HARD : only palm or finger are detected (MorphoSmart&tm; FINGER VP only) */
	MORPHO_LIVE_OK,					/**< The fingerprint acquisition was correctly completed. (MorphoSmart&tm; FINGER VP only) */
	MORPHO_LAST_MESSAGE
} T_MORPHO_COMMAND_STATUS;

/** @brief This parameter defines a type of compression to apply to the image.*/
typedef enum {
	MORPHO_NO_COMPRESS,			/**< No image compression is applied. */
	MORPHO_COMPRESS_V1,			/**< V1 image compression (Morpho private fingerprint image compression algorithm)
								is applied. The compression ratio depends on the image to be compressed but it is
								usually close to 4. To be displayed, the compressed image must be uncompressed using
								the ImageCompress.dll library provided with the MorphoSmart&tm; SDK. This compression
								algorithm is recommended when the device is connected by a RS232 port. */
	MORPHO_COMPRESS_WSQ,		/**< WSQ image compression (Wavelets Scalar Quantization algorithm) is applied. Please
								note that this algorithm is used by several police services to store fingerprint
								images. The MorphoSmart&tm; MSO and CBM require a specific firmware (which disables
								the internal biometric database) to export WSQ images. The MorphoSmart&tm; FINGER VP
								requires a specific license to export WSQ images. This compression algorithm is
								recommended when the device is connected by a RS232 port. */
	MORPHO_COMPRESS_BMP,		/**< Bitmap image compression is applied (uncompressed bit packed). */
	MORPHO_COMPRESS_JPEG,		/**< JPEG image compression is applied. */
	MORPHO_COMPRESS_JPEG2000,	/**< JPEG-2000 image compression is applied. */
	MORPHO_COMPRESS_PNG,		/**< PNG image compression is applied. */
	MORPHO_COMPRESS_UNCHANGED	/**< Current image compression is kept. */
} T_MORPHO_COMPRESS_ALGO;

/** @brief This structure is used to define a status or a step returned during the enrollment process. */
typedef struct {
	UC				m_uc_nbFinger;			/**< Number of the finger currently being enrolled (starts from 1). */
	UC				m_uc_nbFingerTotal;		/**< Total number of fingers to enroll (1 or 2). */
	UC				m_uc_nbCapture;			/**< Current acquisition number of the finger being currently enrolled (starts from 1). */
	UC				m_uc_nbCaptureTotal;	/**< Total number of acquisitions for one finger (3). */
} T_MORPHO_CALLBACK_ENROLLMENT_STATUS;

/** @brief Structure defining an image. This structure is used to display the image or
           returned by the MorphoSmart™ in the callback function. @n
			In RAW compression, image is in m_puc_Image, size is in ImageHeader.@n
			In WSQ compression, image is in m_puc_CompressedImage, size is in ImageWSQHeader, original size are for information in ImageHeader.@n
			In V1 compression, V1 image is in m_puc_CompressedImage, V1 size is in m_ul_imageSize,
			in V1 compression, RAW image is also available in m_puc_Image and size is in ImageHeader so "m_uc_CompressionType" returned
			is set with ID_COMPRESSION_NULL.@n
			@note If @ref EnableDataEncryption option is enabled, the encrypted image is stored in the field m_puc_Image and the size of the image in the field m_ul_imageSize regardless of the type of encoding applied
			*/
typedef struct {
	T_MORPHO_IMAGE_HEADER 	    m_x_ImageHeader;		/**< Image header */
	T_MORPHO_IMAGE_WSQ_HEADER 	m_x_ImageWSQHeader;     /**< WSQ Image header */
	UL                          m_ul_imageSize;         /**< Only used for image in V1 compression, otherwise see  in Header information*/
	PUC						    m_puc_Image;            /**< The buffer of uncompressed image (Not used if compression WSQ is used)*/
	PUC						    m_puc_CompressedImage; /**<  This buffer contains the compressed image (V1 or WSQ) if you chose to get the compressed image*/
} T_MORPHO_IMAGE, T_MORPHO_CALLBACK_IMAGE_STATUS;

/** @brief T_MORPHO_CALLBACK_FUNCTION */
/**
 * This callback function is called each time the MorphoSmart™ send an asynchronous
 * message while processing a biometric function.
 *
 * This callback function is not reentered for a same C_MORPHO_Device object.
 * As a result, if a callback function is associated to a C_MORPHO_Device object,
 * it is forbidden to call a biometric function (like Verify, Capture, Identify …)
 * on this same C_MORPHO_Device object within this callback function.
 * To call a biometric function (like Verify, Capture, Identify …) within this callback
 * function:
 * -	First, you need to create and open a second C_MORPHO_Device object
 *(outside of this callback function and before using this callback function)
 * -	Then, call the biometric function on this second C_MORPHO_Device object
 * within this same callback function.
 *
 * Example:
 * @code
 * I BiometricCallback(
 * 	PVOID									i_pv_context,
 * 	T_MORPHO_CALLBACK_COMMAND	i_i_command,
 * 	PVOID									i_pv_param
 * )
 * {
 * 	T_MORPHO_IMAGE_HEADER* l_px_headerImage;
 *
 * 	if (i_i_command == MORPHO_CALLBACK_COMMAND_CMD)
 * 	{
 * 		switch(*(PI)i_pv_param)
 * 		{
 * 			case MORPHO_REMOVE_FINGER:
 * 			...
 * 		}
 * 	}
 * 	else if (i_i_command == MORPHO_CALLBACK_ENROLLMENT_CMD)
 * 	{
 * 		l_px_headerImage = (T_MORPHO_IMAGE_HEADER*)((I)i_pv_param);
 * 		...
 * 	}
 * 	else if (i_i_command == MORPHO_CALLBACK_IMAGE_CMD) {
 * 		...
 * 	}
 * 	return 0;
 * }
 * @endcode
 *
 * @param[in] i_pv_context
 * - context pointer.
 * @param[in] i_i_command
 * - asynchronous received status.
 * @param[in] i_pv_param
 * - structure depending on the I_I_command value.
 *
 * The following table gives the relation between the I_I_command and the type of
 * this structure:
 * - MORPHO_CALLBACK_COMMAND_CMD		T_MORPHO_COMMAND_STATUS
 * - MORPHO_CALLBACK_IMAGE_CMD			T_MORPHO_CALLBACK_IMAGE_STATUS
 * - MORPHO_CALLBACK_ENROLLMENT_CMD		T_MORPHO_CALLBACK_ENROLLMENT_STATUS
 * - MORPHO_CALLBACK_CODEQUALITY		UC.The recovered value is a PUC that contains a UC.
 * - MORPHO_CALLBACK_DETECTQUALITY		UC.The recovered value is a PUC that contains a UC.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 */
typedef I	(*T_MORPHO_CALLBACK_FUNCTION)(
					PVOID						i_pv_context,
					T_MORPHO_CALLBACK_COMMAND	i_i_command,
					PVOID						i_pv_param	);

/** @brief Those values are used to define whether a field is private or public. */
typedef enum {
		MORPHO_PUBLIC_FIELD, /**< Field is public and can be freely retrieved any time. */
		MORPHO_PRIVATE_FIELD, /**< Field is private and can only be retrieved after a hit condition. */
		MORPHO_STAT_FIELD, /**< Public field to manage user’s statistics and can be freely retrieved at any time. */
		MORPHO_FILTER_FIELD /**< Public field to manage DataBase filter and can be freely retrieved at any time. */
} T_MORPHO_FIELD_ATTRIBUTE;

/** @brief Those values are used to determine the kind of filter to use on a database. */
typedef enum {
	MORPHO_FILTER_TYPE_BINARY,
	MORPHO_FILTER_TYPE_INTEGER
} T_MORPHO_FILTER_TYPE;

/** @brief Those values identify a fingerprint template format. For more information on template format,
 * please refer to the MorphoSmart Overview documentation. */
typedef enum {
		MORPHO_PK_COMP,				/**< Compressed standard template format. This is the template format that should be used in most of the developments. This is the template that gives the best results. */
		MORPHO_PK_MAT_NORM,			/**< Specific template format reserved for compatibility with existing systems or specific usage. */
		MORPHO_PK_COMP_NORM,		/**< Specific template format reserved for compatibility with existing systems or specific usage. */
		MORPHO_PK_MAT,				/**< Specific template format reserved for compatibility with existing systems or specific usage. */
		MORPHO_PK_ANSI_378,			/**< International standard template format defined as ANSI INCITS 378 standard. */
		MORPHO_PK_MINEX_A,			/**< International standard template format according to MINEX specification. */
		MORPHO_PK_ISO_FMR,			/**< International standard template format defined as ISO/IEC 19794-2 standard, at section Finger Minutiae Record. */
		MORPHO_PK_ISO_FMC_NS,		/**< International standard template format defined as ISO/IEC 19794-2 standard, at section Finger Minutiae Card Record Normal Size. */
		MORPHO_PK_ISO_FMC_CS,		/**< International standard template format defined as ISO/IEC 19794-2 standard, at section Finger Minutiae Card Record Compact Size. */
		MORPHO_PK_ILO_FMR,			/**<  */
		MORPHO_PK_FROM_SMART_CARD,	/**<  */
		MORPHO_PK_V10,
		MORPHO_PK_MOC,				/**< Specific template format reserved for compatibility with existing systems or specific usage. */
		MORPHO_PK_DIN_V66400_CS,	/**< International standard template format defined as DIN-V66400 Compact Size. */
		MORPHO_PK_DIN_V66400_CS_AA,	/**< International standard template format defined as DIN-V66400 Compact Size. Ordered ascending and angle. */
		MORPHO_PK_ISO_FMC_CS_AA,	/**< International standard template format defined as ISO/IEC 19794-2 standard, at section Finger Minutiae Card Record Compact Size. Ordered in ascending angle. */
		MORPHO_PK_CFV,				/**< Morpho proprietary CFV Fingerprint Template (reserved for future use). */
		MORPHO_PK_BIOSCRYPT,		/**< Bioscrypt Fingerprint Template (reserved for future use). */
		MORPHO_NO_PK_FP,			/**< Do not return fingerprint template. */
		MORPHO_PK_ANSI_378_2009,	/**< International standard template format defined as ANSI INCITS 378 standard Version 2009. */
		MORPHO_PK_ISO_FMR_2011,		/**< International standard template format defined as ISO/IEC 19794-2 standard Version 2011, at section Finger Minutiae Record. */
		MORPHO_PK_PKLITE			/**< (MorphoSmart&tm; CBM-E3 only) This is the template format that should be used in most of the developments. This is the template that gives the best results. */
} T_MORPHO_TYPE_TEMPLATE;

/** @brief Those values identify a multimodal template format including fingerprint data and vein data. For more information
 * on template format, please refer to the MorphoSmart Overview documentation. */
typedef enum {
	MORPHO_NO_PK_FVP,				/**< Do not return multimodal template. */
	MORPHO_PK_FVP,					/**< Multimodal template format supported by MorphoSmart&tm; FINGER VP. */
	MORPHO_PK_FVP_MATCH				/**< Multimodal template format supported by MorphoSmart&tm; FINGER VP. */
} T_MORPHO_FVP_TYPE_TEMPLATE;

/** @brief This parameter defines a biometric template envelope. */
typedef enum {
		MORPHO_RAW_TEMPLATE,			/**< raw template without a signature. */
		MORPHO_X984_SIGNED_TEMPLATE		/**< template with a digital signature and an X9.84 envelope. */
}T_MORPHO_TEMPLATE_ENVELOP;

// Workflow For SMARTCARD
typedef enum {
		MORPHO_WORFLOW_SMARTCARD_SAGEM_DEMO,
		MORPHO_WORFLOW_SMARTCARD_CLIENT_GB
} T_MORPHO_SMARTCARD_WORKFLOW_TYPE;

/** @brief Structure defining a match on device Smart Card Template */
typedef struct _T_MORPHO_TEMPLATE_IN_SMARTCARD
{
	T_MORPHO_SMARTCARD_WORKFLOW_TYPE	m_x_WorkFlowCardType;	// Work Flow type to read the templates in the Smart Card
	T_MORPHO_TYPE_TEMPLATE				m_x_typTemplate;		// template type stored in the Smart Card
	UC									m_uc_TemplateNbr;		// Number of Templates to read in the Smart Card
} T_MORPHO_TEMPLATE_IN_SMARTCARD, *PT_MORPHO_TEMPLATE_IN_SMARTCARD;

/** @brief This parameter defines the Ciphered  type in the T_MORPHO_MOC_PARAMETERS for a match on device Smart Card Template */
typedef enum
{
	MORPHO_MOC_CIPHERED_NO,			/**< No Ciphered  is need */
	MORPHO_MOC_CIPHERED_3DES_ECB,	/**< 3DES ECB */
	MORPHO_MOC_CIPHERED_3DES_CBC	/**< 3DES CBC */
} T_MORPHO_MOC_CIPHERED_TYPE;

/** @brief This parameter defines the Diversification type in the T_MORPHO_TEMPLATE_FOR_MOC for a match on device Smart Card Template */
typedef enum
{
	MORPHO_MOC_DIVERSIFICATION_NO,			/**< No Diversification  is need */
	MORPHO_MOC_DIVERSIFICATION_TYPE_1		/**< Diversification type 1 */
} T_MORPHO_MOC_DIVERSIFICATION_TYPE;

/** @brief This parameter defines the APDU Padding type in the T_MORPHO_MOC_PARAMETERS for a match on device Smart Card Template */
typedef enum
{
	MORPHO_MOC_APDU_PADDING_TYPE_NO,	/**< No Padding is need */
	MORPHO_MOC_APDU_PADDING_TYPE_1		/**< padding type 1 */
} T_MORPHO_MOC_APDU_PADDING_TYPE;

/** @brief This parameter defines the APDU Chain Type for chaining the APDU in the T_MORPHO_MOC_PARAMETERS for a match on device Smart Card Template */
typedef enum
{
	MORPHO_MOC_APDU_CHAIN_TYPE_NO,			/**< No chain type is need */
	MORPHO_MOC_APDU_CHAIN_TYPE_1,		/**< Chain type 1 */
	MORPHO_MOC_APDU_CHAIN_TYPE_2		/**< Chain type 1 */
} T_MORPHO_MOC_APDU_CHAIN_TYPE;

/** @brief Structure defining a match on Smart Card APDU Parameters */
typedef struct _T_MORPHO_MOC_APDU_PARAMETERS
{
	PUC 								m_puc_ApduToTrack;					/**< Defines the tracked APDU to be replaced by the Send APDU. */
	I 									m_i_ApduToTrackLen;					/**< Defines the tracked APDU length  */

	PUC			 						m_puc_ApduHeaderToSend_Unique;				/**< Can be NULL when chain mode is used. Defines the APDU Header (CLA INS P1 P2 P3) to be replaced instead of the tracked APDU. If the chain mode is not used */
	PUC			 						m_puc_ApduHeaderToSend_First;				/**< Can be NULL when chain mode is not used. Defines the APDU Header (CLA INS P1 P2 P3) to be replaced instead of the tracked APDU. Used for the first APDU in the chain mode */
	PUC			 						m_puc_ApduHeaderToSend_Middle;				/**< Can be NULL when chain mode is not used. Defines the APDU Header (CLA INS P1 P2 P3) to be replaced instead of the tracked APDU. Used for the middle (if not first or last) APDU in the chain mode */
	PUC			 						m_puc_ApduHeaderToSend_Last;				/**< Can be NULL when chain mode is not used. Defines the APDU Header (CLA INS P1 P2 P3) to be replaced instead of the tracked APDU. Used for the last APDU in the chain mode */

	PUC									m_puc_DataToWriteBeforeTemplate;			/**< Can be NULL if no data are required. Defines the Data to be added before the Template in the APDU */
	I									m_i_DataToWriteBeforeTemplateLen;			/**< Defines the Data length to be added before the Template in the APDU */
	PUC									m_puc_DataToWriteAfterTemplate;				/**< Can be NULL if no data are required. Defines the Data to be added after the Template in the APDU */
	I									m_i_DataToWriteAfterTemplateLen;			/**< Defines the Data length to be added after the Template in the APDU */

	PUC									m_puc_DiversificationData;			/**< Can be NULL. If the parameter m_x_DiversificationType is not 	MORPHO_MOC_DIVERSIFICATION_NO, Defines the diversification data for the ciphering key */
	I	 								m_i_DiversificationDataLen;			/**< Diversification Data Len */

	T_MORPHO_MOC_DIVERSIFICATION_TYPE	m_x_DiversificationType;			/**< Diversification Type */
	T_MORPHO_MOC_CIPHERED_TYPE			m_x_CipheredType;					/**< Template Ciphered  Type */
	T_MORPHO_MOC_APDU_PADDING_TYPE		m_x_PaddingType;					/**< APDU Padding type */
	T_MORPHO_MOC_APDU_CHAIN_TYPE		m_x_ChainType;						/**< APDU Chain Type for chainig the APDU if necessary */
} T_MORPHO_MOC_APDU_PARAMETERS, *PT_MORPHO_MOC_APDU_PARAMETERS;

/** @brief Structure defining a match on Smart Card Capture Parameters */
typedef struct _T_MORPHO_MOC_PARAMETERS
{
	I									m_i_TimeOut;							/**< Time (in seconds) the template is kept in the MSO */
} T_MORPHO_MOC_PARAMETERS, *PT_MORPHO_MOC_PARAMETERS;



/**
 * @brief This structure is used to provide additional image information
 * required by ISO-19794-4 FIR format.
 */
typedef struct
{
	UC	m_uc_FingerPosition;
	/**< Finger position code represented in the image.<br />
	 * Must be one of the following values:
	 *	<center>
	 *		<table>
	 *			<caption>Finger position codes</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Finger position</th>
	 *				<th style="text-align: center;">Code</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Unknown</td>
	 *				<td style="text-align: left;">0x00 <i>(0)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right thumb</td>
	 *				<td style="text-align: left;">0x01 <i>(1)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right index finger</td>
	 *				<td style="text-align: left;">0x02 <i>(2)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right middle finger</td>
	 *				<td style="text-align: left;">0x03 <i>(3)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right ring finger</td>
	 *				<td style="text-align: left;">0x04 <i>(4)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right little finger</td>
	 *				<td style="text-align: left;">0x05 <i>(5)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left thumb</td>
	 *				<td style="text-align: left;">0x06 <i>(6)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left index finger</td>
	 *				<td style="text-align: left;">0x07 <i>(7)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left middle finger</td>
	 *				<td style="text-align: left;">0x08 <i>(8)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left ring finger</td>
	 *				<td style="text-align: left;">0x09 <i>(9)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left little finger</td>
	 *				<td style="text-align: left;">0x0A <i>(10)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Plain right four fingers</td>
	 *				<td style="text-align: left;">0x0D <i>(13)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Plain left four fingers</td>
	 *				<td style="text-align: left;">0x0E <i>(14)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Plain thumbs (2)</td>
	 *				<td style="text-align: left;">0x0F <i>(15)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Unknown palm</td>
	 *				<td style="text-align: left;">0x14 <i>(20)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right full palm</td>
	 *				<td style="text-align: left;">0x15 <i>(21)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right writer's palm</td>
	 *				<td style="text-align: left;">0x16 <i>(22)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left full palm</td>
	 *				<td style="text-align: left;">0x17 <i>(23)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left writer's palm</td>
	 *				<td style="text-align: left;">0x18 <i>(24)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right lower palm</td>
	 *				<td style="text-align: left;">0x19 <i>(25)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right upper palm</td>
	 *				<td style="text-align: left;">0x1A <i>(26)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left lower palm</td>
	 *				<td style="text-align: left;">0x1B <i>(27)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left upper palm</td>
	 *				<td style="text-align: left;">0x1C <i>(28)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right other</td>
	 *				<td style="text-align: left;">0x1D <i>(29)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left other</td>
	 *				<td style="text-align: left;">0x1E <i>(30)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right interdigital</td>
	 *				<td style="text-align: left;">0x1F <i>(31)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right thenar</td>
	 *				<td style="text-align: left;">0x20 <i>(32)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right hypothenar</td>
	 *				<td style="text-align: left;">0x21 <i>(33)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left interdigital</td>
	 *				<td style="text-align: left;">0x22 <i>(34)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left thenar</td>
	 *				<td style="text-align: left;">0x23 <i>(35)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left hypothenar</td>
	 *				<td style="text-align: left;">0x24 <i>(36)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	UC	m_uc_FingerImageQuality;
	/**< Finger image quality as defined
	 * in the ISO-19794-4 FIR format (0
	 * if quality is unknown). */
	UC	m_uc_ImpressionType;
	/**< Finger image impression type.<br />
	 * Must be one of the following values:
	 *	<center>
	 *		<table>
	 *			<caption>Finger image impression type code</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Finger image impression type</th>
	 *				<th style="text-align: center;">Code</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Live-scan plain</td>
	 *				<td style="text-align: left;">0x00 <i>(0)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Live-scan rolled</td>
	 *				<td style="text-align: left;">0x01 <i>(1)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Nonlive-scan plain</td>
	 *				<td style="text-align: left;">0x02 <i>(2)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Nonlive-scan rolled</td>
	 *				<td style="text-align: left;">0x03 <i>(3)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Latent</td>
	 *				<td style="text-align: left;">0x07 <i>(7)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Swipe</td>
	 *				<td style="text-align: left;">0x08 <i>(8)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Live-scan contactless</td>
	 *				<td style="text-align: left;">0x09 <i>(9)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
} T_MORPHO_IMAGE_INFO, * PT_MORPHO_IMAGE_INFO;


/**
 * @brief This structure is used to represent a finger image record
 * header from ISO-19794-4 FIR format.
 * For more detail about this structure field, please refer to the
 * ISO-19794-4 FIR format documentation
 */
typedef struct
{
	unsigned int	m_ui_FingerDataBlockLength;
	/**< Size of the finger image record header including the finger
	 * image data block.
	 * Warning: this value is different than
	 * sizeof(T_MORPHO_ISO19794_FIR_IMAGE).
	 */
	UC	m_uc_FingerPosition;
	/**< Finger position code represented in the image.<br />
	 * Must be one of the following values:
	 *	<center>
	 *		<table>
	 *			<caption>Finger position codes</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Finger position</th>
	 *				<th style="text-align: center;">Code</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Unknown</td>
	 *				<td style="text-align: left;">0x00 <i>(0)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right thumb</td>
	 *				<td style="text-align: left;">0x01 <i>(1)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right index finger</td>
	 *				<td style="text-align: left;">0x02 <i>(2)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right middle finger</td>
	 *				<td style="text-align: left;">0x03 <i>(3)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right ring finger</td>
	 *				<td style="text-align: left;">0x04 <i>(4)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right little finger</td>
	 *				<td style="text-align: left;">0x05 <i>(5)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left thumb</td>
	 *				<td style="text-align: left;">0x06 <i>(6)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left index finger</td>
	 *				<td style="text-align: left;">0x07 <i>(7)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left middle finger</td>
	 *				<td style="text-align: left;">0x08 <i>(8)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left ring finger</td>
	 *				<td style="text-align: left;">0x09 <i>(9)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left little finger</td>
	 *				<td style="text-align: left;">0x0A <i>(10)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Plain right four fingers</td>
	 *				<td style="text-align: left;">0x0D <i>(13)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Plain left four fingers</td>
	 *				<td style="text-align: left;">0x0E <i>(14)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Plain thumbs (2)</td>
	 *				<td style="text-align: left;">0x0F <i>(15)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Unknown palm</td>
	 *				<td style="text-align: left;">0x14 <i>(20)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right full palm</td>
	 *				<td style="text-align: left;">0x15 <i>(21)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right writer's palm</td>
	 *				<td style="text-align: left;">0x16 <i>(22)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left full palm</td>
	 *				<td style="text-align: left;">0x17 <i>(23)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left writer's palm</td>
	 *				<td style="text-align: left;">0x18 <i>(24)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right lower palm</td>
	 *				<td style="text-align: left;">0x19 <i>(25)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right upper palm</td>
	 *				<td style="text-align: left;">0x1A <i>(26)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left lower palm</td>
	 *				<td style="text-align: left;">0x1B <i>(27)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left upper palm</td>
	 *				<td style="text-align: left;">0x1C <i>(28)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right other</td>
	 *				<td style="text-align: left;">0x1D <i>(29)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left other</td>
	 *				<td style="text-align: left;">0x1E <i>(30)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right interdigital</td>
	 *				<td style="text-align: left;">0x1F <i>(31)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right thenar</td>
	 *				<td style="text-align: left;">0x20 <i>(32)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Right hypothenar</td>
	 *				<td style="text-align: left;">0x21 <i>(33)</i></td>
	 *			</tr>
	 *			<tr style="background: #7F7F7F;">
	 *				<td>&nbsp;</td>
	 *				<td>&nbsp;</td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left interdigital</td>
	 *				<td style="text-align: left;">0x22 <i>(34)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left thenar</td>
	 *				<td style="text-align: left;">0x23 <i>(35)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Left hypothenar</td>
	 *				<td style="text-align: left;">0x24 <i>(36)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	UC	m_uc_CountOfViews;
	/**< The count of views for the current finger position. */
	UC	m_uc_ViewNumber;
	/**< The view number within the count of views for the current
	 * finger position. */
	UC	m_uc_FingerImageQuality;
	/**< Finger image quality as defined in the ISO-19794-4 FIR
	 * format (0 if quality is unknown). */
	UC	m_uc_ImpressionType;
	/**< Finger image impression type.<br />
	 * Must be one of the following values:
	 *	<center>
	 *		<table>
	 *			<caption>Finger image impression type code</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Finger image impression type</th>
	 *				<th style="text-align: center;">Code</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Live-scan plain</td>
	 *				<td style="text-align: left;">0x00 <i>(0)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Live-scan rolled</td>
	 *				<td style="text-align: left;">0x01 <i>(1)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Nonlive-scan plain</td>
	 *				<td style="text-align: left;">0x02 <i>(2)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Nonlive-scan rolled</td>
	 *				<td style="text-align: left;">0x03 <i>(3)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Latent</td>
	 *				<td style="text-align: left;">0x07 <i>(7)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Swipe</td>
	 *				<td style="text-align: left;">0x08 <i>(8)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Live-scan contactless</td>
	 *				<td style="text-align: left;">0x09 <i>(9)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	US	m_us_HorizontalLineLength;
	/**< Number of pixel per horizontal line (image width).<br />
	 * Here the values used for the Morpho devices :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device image size</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Image size (horizontal x vertical)</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;"><b>416</b>x<i>416</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;"><b>256</b>x<i>416</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;"><b>400</b>x<i>400</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;">
	 *					<b>1600</b>x<i>1500</i><br />
	 *					Should be different if decimation factor is different than 2.<br />
	 *					Please refer the the MorphoTop SDK User Guide for more information.
	 *				</td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	US	m_us_VerticalLineLength;
	/**< Number of pixel per vertical line (image height).<br />
	 * Here the values used for the Morpho devices :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device image size</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Image size (horizontal x vertical)</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;"><i>416</i>x<b>416</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;"><i>256</i>x<b>416</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;"><i>400</i>x<b>400</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;">
	 *					<i>1600</i>x<b>1500</b><br />
	 *					Should be different if decimation factor is different than 2.<br />
	 *					Please refer the the MorphoTop SDK User Guide for more information.
	 *				</td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	UC	m_uc_Reserved;
	/**< Reserved for future ISO-19794-4 FIR format revisions. */
	PVOID  m_pv_ImageData;
	/**< Image data buffer containing the image pixels. */
} T_MORPHO_ISO19794_FIR_IMAGE, * PT_MORPHO_ISO19794_FIR_IMAGE;


/**
 * @brief This structure is used to represent a finger general record
 * header from ISO-19794-4 FIR format.
 * For more detail about this structure field, please refer to the
 * ISO-19794-4 FIR format documentation
 */
typedef struct
{
	C	m_pc_FormatIdentifier[4];
	/**< The ISO-19794-4 FIR format
	 * identifier. "FIR" in this
	 * version. */
	C	m_pc_VersionNumber[4];
	/**< The ISO-19794-4 FIR format
	 * version number. "010" in this
	 * version. */
	ULL	m_ull_RecordLength;
	/**< Size of the ISO-19794-4 FIR
	 * export (including size of the
	 * finger image record headers).
	 * Warning: this value is different
	 * than sizeof(T_MORPHO_ISO19794_FIR).
	 */
	US	m_us_CaptureDeviceID;
	/**< The manufacturer capture device
	 * ID from which the images were
	 * taken. */
	US	m_us_ImageAcquisitionLevel;
	/**< Images acquisition level. For
	 * more details, please refer to the
	 * ISO-19794-4 FIR format
	 * documentation.<br />
	 * Here the values used for the Morpho devices :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device image acquisition levels</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Image acquisition level</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;">0x001F <i>(31)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;">0x001F <i>(31)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;">0x001F <i>(31)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;">0x001F <i>(31)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	UC	m_uc_NumberOfFinger;
	/**< Number of fingers contained in
	 * this export. It consist to count
	 * the number of different
	 * "Finger Position" values contain
	 * whithin each finger image record
	 * header contained in this export. */
	UC	m_uc_ScaleUnits;
	/**< The scale units used for the 4
	 * next values. Must be equal to 0x01
	 * for Pixels per Inch or 0x02 for
	 * Pixels per Centimeters.<br />
	 * Here the values used for the Morpho devices :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device scale units</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Scale units</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;">0x01 <i>(Pixel/Inch)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;">0x01 <i>(Pixel/Inch)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;">0x01 <i>(Pixel/Inch)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;">0x01 <i>(Pixel/Inch)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	US	m_us_HorizontalScanResolution;
	/**< The horizontal resolution of the
	 * sensor used to acquire images
	 * contained in this export.<br />
	 * Here the values used for the Morpho devices :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device scan resolutions</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Scan resolution (horizontal x vertical)</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	US	m_us_VerticalScanResolution;
	/**< The vertical resolution of the
	 * sensor used to acquire images
	 * contained in this export.<br />
	 * Here the values used for the Morpho devices :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device scan resolutions</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Scan resolution (horizontal x vertical)</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	US	m_us_HorizontalImageResolution;
	/**< The horizontal resolution of the
	 * images contained in this export.<br />
	 * Here the values used for the Morpho devices
	 * if image was not modified since it was captured :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device image resolutions</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Image resolution (horizontal x vertical)</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;"><b>500</b>x<i>500</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	US	m_us_VerticalImageResolution;
	/**< The vertical resolution of the
	 * images contained in this export.<br />
	 * Here the values used for the Morpho devices
	 * if image was not modified since it was captured :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device image resolutions</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Image resolution (horizontal x vertical)</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;"><i>500</i>x<b>500</b></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	UC	m_uc_PixelDepth;
	/**< Number of bits used to represent a single pixel.<br />
	 * Here the values used for the Morpho devices :
	 *	<center>
	 *		<table>
	 *			<caption>Morpho Device pixel depth</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Morpho Device</th>
	 *				<th style="text-align: center;">Pixel depth</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MSO</td>
	 *				<td style="text-align: left;">0x08 <i>(8 Pixel/Inch)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">CBM</td>
	 *				<td style="text-align: left;">0x08 <i>(8 Pixel/Inch)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">FingerVP</td>
	 *				<td style="text-align: left;">0x08 <i>(8 Pixel/Inch)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">MorphoTop 100/100-L</td>
	 *				<td style="text-align: left;">0x08 <i>(8 Pixel/Inch)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	UC	m_uc_ImageCompressionAlgorithm;
	/**< A value indicating which image
	 * encoding format is used in the
	 * images contained in this export.<br />
	 * This value should be one of the following :
	 *	<center>
	 *		<table>
	 *			<caption>Image compression algorithm code</caption>
	 *			<tr>
	 *				<th style="text-align: center;">Image compression algorithm</th>
	 *				<th style="text-align: center;">Code</th>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Uncompressed - No bit packing <i>(RAW)</i></td>
	 *				<td style="text-align: left;">0x00 <i>(0)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Uncompressed - Bit packed <i>(BMP)</i></td>
	 *				<td style="text-align: left;">0x01 <i>(1)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Compressed - WSQ</td>
	 *				<td style="text-align: left;">0x02 <i>(2)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Compressed - JPEG</td>
	 *				<td style="text-align: left;">0x03 <i>(3)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Compressed - JPEG 2000</td>
	 *				<td style="text-align: left;">0x04 <i>(4)</i></td>
	 *			</tr>
	 *			<tr>
	 *				<td style="text-align: left;">Compressed - PNG</td>
	 *				<td style="text-align: left;">0x05 <i>(5)</i></td>
	 *			</tr>
	 *		</table>
	 *	</center>
	 */
	US	m_us_Reserved;
	/**< Reserved for future ISO-19794-4
	 * FIR format revisions. */
	unsigned int	m_ui_ImageCount;
	/**< The number of finger image record
	 * contained in the next parameter. */
	PT_MORPHO_ISO19794_FIR_IMAGE *	m_ppx_ISO19794_FIR_Images;
	/**< An array of pointer to the finger
	 * image record contained in this
	 * export. */
} T_MORPHO_ISO19794_FIR, * PT_MORPHO_ISO19794_FIR;


//////////////////////////////////////////////////////////////////
//		CALLBACK COM
//////////////////////////////////////////////////////////////////

/** @brief T_MORPHO_CALLBACK_COM_OPEN */
/**
 * The function initializes/opens a specific customer communication link.
 *
 * @param[out] o_ph_MsoHandle
 * - Pointer to the MORPHO_HANDLE of the initialized connection to the specific MSO.
 * This parameter is filled after a correct processing of this function.
 * If the processing failed, the value of this parameter is NULL.
 * @param[in] i_pc_String
 * - String parameter (for example, "COM1" if using the serial link).
 * @param[in] i_ul_Param
 * - Long integer parameter, (for example, "115200" if using the serial link).
 * @param[in,out] io_pv_Param
 * - Any further specific parameter.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_COM_OPEN)(
					MORPHO_HANDLE	*o_ph_MsoHandle,
					PC				i_pc_String,
					UL				i_ul_Param,
					PVOID			io_pv_Param	);

typedef F_MORPHO_CALLBACK_COM_OPEN *T_MORPHO_CALLBACK_COM_OPEN;

#ifdef _ANDROID_USB
/** @brief T_MORPHO_CALLBACK_COM_OPEN_FD */
/**
 * The function initializes/opens a specific customer communication link.
 *
 * @param[out] o_ph_MsoHandle
 * - Pointer to the MORPHO_HANDLE of the initialized connection to the specific MSO.
 * This parameter is filled after a correct processing of this function.
 * If the processing failed, the value of this parameter is NULL.
 * @param[in] i_i_fd
 * - file descriptor.
 * @param[in] i_ul_Param
 * - Long integer parameter, (for example, "115200" if using the serial link).
 * @param[in,out] io_pv_Param
 * - Any further specific parameter.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_COM_OPEN_FD)(
					MORPHO_HANDLE	*o_ph_MsoHandle,
					I 				i_i_bus,
					I 				i_i_address,
					I 				i_i_fd,
					UL				i_ul_Param,
					PUC 			*o_ppuc_sn,
					PVOID			io_pv_Param	);

typedef F_MORPHO_CALLBACK_COM_OPEN_FD *T_MORPHO_CALLBACK_COM_OPEN_FD;
#endif //_ANDROID_USB

/** @brief T_MORPHO_CALLBACK_COM_SEND */
/**
 * The function sends an ILV frame, using a specific customer communication link.
 *
 * @param[in] i_h_MsoHandle
 * - identifier of the connection to a specific MSO.
 * @param[in] i_ul_Timeout
 * - timeout in milliseconds.
 * @param[in] i_puc_Data
 * - buffer to transmit.
 * @param[in] i_ul_DataSize
 * - buffer size.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors.
 */
typedef I	(F_MORPHO_CALLBACK_COM_SEND)(
					MORPHO_HANDLE	i_h_MsoHandle,
					UL				i_ul_Timeout,
					PUC				i_puc_Data,
					UL				i_ul_DataSize);

typedef F_MORPHO_CALLBACK_COM_SEND *T_MORPHO_CALLBACK_COM_SEND;

/** @brief T_MORPHO_CALLBACK_COM_RECEIVE */
/**
 * The function receives an ILV frame using a specific customer link.
 * Allocation shall be performed by the callback function.
 *
 * @param[in] i_h_MsoHandle
 * - identifier of the connection to a specific MSO.
 * @param[in] i_ul_Timeout
 * - timeout in milliseconds.
 * @param[in,out] io_ppuc_Data
 * - reception buffer. This buffer shall be allocated by the callback function.
 * @param[out] o_pul_DataSize
 * - buffer size.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors.
 * If the timeout expired, the function must return COMERR_TIMEOUT_RECEIVE.
 */
typedef I	(F_MORPHO_CALLBACK_COM_RECEIVE)(
					MORPHO_HANDLE	i_h_MsoHandle,
					UL				i_ul_Timeout,
					PUC*			io_ppuc_Data,
					PUL				o_pul_DataSize);

typedef F_MORPHO_CALLBACK_COM_RECEIVE *T_MORPHO_CALLBACK_COM_RECEIVE;

/** @brief T_MORPHO_CALLBACK_COM_RECEIVE_FREE */
/**
 * The function releases memory allocated by T_MORPHO_CALLBACK_COM_RECEIVE.
 *
 * @param[in] i_h_MsoHandle
 * - identifier of the connection to a specific MSO.
 * @param[in,out] io_ppuc_Data
 * - buffer
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_COM_RECEIVE_FREE)(
					MORPHO_HANDLE	i_h_MsoHandle,
					PUC*			io_ppuc_Data);

typedef F_MORPHO_CALLBACK_COM_RECEIVE_FREE *T_MORPHO_CALLBACK_COM_RECEIVE_FREE;

/** @brief T_MORPHO_CALLBACK_COM_CLOSE */
/**
 * The function closes a specific customer communication link.
 *
 * @param[in] i_h_MsoHandle
 * - identifier of the connection to a specific MSO.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_COM_CLOSE)(
					MORPHO_HANDLE	*i_ph_MsoHandle);

typedef F_MORPHO_CALLBACK_COM_CLOSE *T_MORPHO_CALLBACK_COM_CLOSE;

//////////////////////////////////////////////////////////////////
//		CALLBACK TUNNELING
//////////////////////////////////////////////////////////////////

/** @brief T_MORPHO_CALLBACK_TUNNELING_INIT1 */
/**
 * The function performs the mutual authentication and key exchange between
 * the Host and the MorphoSmart™ (according to Needham-Schroeder protocol for asymmetric keys).
 *
 * @param[in] i_ul_TokenMso1Size
 * - Token size.
 * @param[in] i_puc_TokenMso1
 * - Token generated by the MorphoSmart™. This token is encrypted with the host public key.
 * This token contains S1 (24 bytes) | C1 (24 bytes) | MSO ident (20 bytes).
 * S1 and C1 are used to compute triple DES signature and encryption keys (see above).
 * @param[in] i_ul_CertifMsoSize
 * - MorphoSmart™ certificate size.
 * @param[in] i_puc_CertifMso
 * - MorphoSmart™ X509 certificate. This certificate is encoded in DER.
 * @param[in] i_ul_HashSizeMso
 * - MSO identifier length.
 * @param[in] i_puc_HashMso
 * - MSO identifier. This identifier is the hash of MSO distinguish name.
 * @param[in] i_ul_HashSizeHost
 * - Host identifier length.
 * @param[in] i_puc_HashHost
 * - Host identifier. This identifier is the hash of Host distinguish name.
 * @param[out] o_pul_TokenHostSize
 * - Token size.
 * @param[out] o_ppuc_TokenHostMSO
 * - Token generated by the Host. This token is encrypted with the MorphoSmart™ public key.
 * This token contains:
 * MSO ident (20 bytes) | S2 (24 bytes) | C2 (24 bytes) | S1 (24 bytes) | C1 (24 bytes)
 * S2 and C2 are random numbers generated by the host
 * The symmetric signature key is S = S1 xor S2
 * The symmetric encryption key is C = C1 xor C2
 * @param[in,out] io_pv_context
 * - Pointer that can be used for customer convenience.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_TUNNELING_INIT1)(
					UL					i_ul_TokenMso1Size,
					PUC					i_puc_TokenMso1,
					UL					i_ul_CertifMsoSize,
					PUC					i_puc_CertifMso,
					UL					i_ul_HashSizeMso,
					PUC					i_puc_HashMso,
					UL					i_ul_HashSizeHost,
					PUC					i_puc_HashHost,
					PUL					o_pul_TokenHostSize,
					PUC*				o_ppuc_TokenHostMSO,
					PVOID);

typedef F_MORPHO_CALLBACK_TUNNELING_INIT1 *T_MORPHO_CALLBACK_TUNNELING_INIT1;

/** @brief T_MORPHO_CALLBACK_TUNNELING_INIT2 */
/**
 * The function performs the mutual authentication and key exchange between
 * the Host and the MorphoSmart™ (according to Needham-Schroeder protocol
 * for asymmetric keys).
 * * Note: an implementation example using OpenSSL™ is given with its source code
 * (please refer to the MsoSecu Visual C++ project within the MorphoSmart SDK CD-ROM).
 *
 * @param[in] i_ul_TokenMso2Size
 * - Size of the following token.
 * @param[in] i_puc_TokenMso2
 * - Token encrypted with the Host public key. This token contains S2 (24 bytes) | C2 (24 bytes)
 *
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_TUNNELING_INIT2)(
					UL					i_ul_TokenMso2Size,
					PUC					i_puc_TokenMso2);

typedef F_MORPHO_CALLBACK_TUNNELING_INIT2 *T_MORPHO_CALLBACK_TUNNELING_INIT2;

/** @brief T_MORPHO_CALLBACK_TRIPLE_DES_SIGN */
/**
 * The function signs a buffer under triple DES EDE3 CBC MAC. (IV is 0).
 * Note: an implementation example using OpenSSL™ is given with its source code
 * (please refer to the MsoSecu Visual C++ project within the MorphoSmart SDK CD-ROM).
 *
 * @param[in] i_ul_DataSize
 * - Data length.
 * @param[in] i_puc_Data
 * - Data to sign.
 * @param[out] o_puc_Sign
 * - Triple signature performed on i_puc_Data. Signature key is S = S1 xor S2
 * (see T_MORPHO_CALLBACK_TUNNELING_INIT1).
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_TRIPLE_DES_SIGN)(
					UL		i_ul_DataSize,
					PUC		i_puc_Data,
					PUC		o_puc_Sign);

typedef F_MORPHO_CALLBACK_TRIPLE_DES_SIGN *T_MORPHO_CALLBACK_TRIPLE_DES_SIGN;

/** @brief T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT */
/**
 * The function encrypts a buffer under triple DES EDE3 CBC. (IV is 0)
 * Note: an implementation example using OpenSSL™ is given with its source code
 * (please refer to the MsoSecu Visual C++ project within the MorphoSmart SDK CD-ROM).
 *
 * @param[in] i_ul_DataSize
 * - Data length.
 * @param[in] i_puc_Data
 * -Data to encrypt.
 * @param[out] o_puc_Crypt
 * - Encrypted data. Encryption key is C = C1 xor C2
 * (see T_MORPHO_CALLBACK_TUNNELING_INIT1).
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_TRIPLE_DES_CRYPT)(
					UL		i_ul_DataSize,
					PUC		i_puc_Data,
					PUC		o_puc_Crypt);

typedef F_MORPHO_CALLBACK_TRIPLE_DES_CRYPT *T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT;

/** @brief T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN */
/**
 * The function verifies a triple DES EDE3 CBC MAC signature. (IV is 0).
 * Note: an implementation example using OpenSSL™ is given with its source code
 * (please refer to the MsoSecu Visual C++ project within the MorphoSmart SDK CD-ROM).
 *
 * @param[in] i_ul_DataSize
 * - Data length.
 * @param[in] i_puc_Data
 * - Signed data.
 * @param[in] i_puc_Sign
 * Triple signature performed on i_puc_Data. Signature key is S = S1 xor S2
 * (see T_MORPHO_CALLBACK_TUNNELING_INIT1).
 * @param[out] o_pi_Result
 * - 1 if signature is correct. 0 if signature is not correct.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN)(
					UL		i_ul_DataSize,
					PUC		i_puc_Data,
					PUC		i_puc_Sign,
					PI		o_pi_Result);

typedef F_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN *T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN;

/** @brief T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT */
/**
 * The function decrypts a buffer under triple DES EDE3 CBC. (IV is 0).
 * Note: an implementation example using OpenSSL™ is given with its source code
 * (please refer to the MsoSecu Visual C++ project within the MorphoSmart SDK CD-ROM).
 *
 * @param[in] i_ul_DataSize
 * - Data length.
 * @param[in] i_puc_Data
 * - Data to encrypt.
 * @param[out] o_puc_Decrypt
 * - Decrypted data. Encryption key is C = C1 xor C2 (see T_MORPHO_CALLBACK_TUNNELING_INIT1).
 *
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT)(
					UL		i_ul_DataSize,
					PUC		i_puc_Data,
					PUC		o_puc_Decrypt);

typedef F_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT *T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT;

//////////////////////////////////////////////////////////////////
//		CALLBACK OFFERED SECURITY
//////////////////////////////////////////////////////////////////
/** @brief T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN */
/**
 * The function checks the digital signature performed by the MorphoSmart™.
 * This callback only applies to MorphoSmart™ S with "offered security" protocol.
 * Note: an implementation example using OpenSSL™ is given with its source code
 * (please refer to the MsoSecu Visual C++ project within the MorphoSmart SDK CD-ROM).
 *
 * @param[in] i_ul_MsoCertificateSize
 * - MorphoSmart™ X509 certificate size.
 * @param[in] i_puc_MsoCertificate
 * - MorphoSmart™ X509 certificate size, coded in DER.
 * @param[in] i_ul_SignatureSize
 * - digital signature size.
 * @param[in] i_puc_Signature
 * - digital signature.
 * @param[in] i_ul_DataSize
 * - signed data length.
 * @param[in] i_puc_Data
 * - signed data.
 * @param[out] o_pi_Result
 * - 1 if signature is correct, 0 if signature is not correct.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN)(
					UL		i_ul_MsoCertificateSize,
					PUC		i_puc_MsoCertificate,
					UL		i_ul_SignatureSize,
					PUC		i_puc_Signature,
					UL		i_ul_DataSize,
					PUC		i_puc_Data,
					PI		o_pi_Result);

typedef F_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN *T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN;

/** @brief T_MORPHO_CALLBACK_RAND */
/**
 * The function generates the random number used to compute the anti-replay integer.
 * This callback only applies to MorphoSmart™ S with "offered security" protocol
 * Note: an implementation example using OpenSSL™ is given with its source code
 * (please refer to the MsoSecu Visual C++ project within the MorphoSmart SDK CD-ROM).
 *
 * @param[in] i_ul_RandomSize
 * - Size of the random number that must be generated.
 * @param[out] o_puc_Random
 * - Generated random number.
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_RAND)(
					UL		i_ul_RandomSize,
					PUC		o_puc_Random);

typedef F_MORPHO_CALLBACK_RAND *T_MORPHO_CALLBACK_RAND;

/** @brief T_MORPHO_CALLBACK_VERIF_CERTIF */
/**
 *
 *
 * @param[in]  i_ul_MsoCertificateSize
 * @param[in]  i_puc_MsoCertificate
 * @param[out] o_pi_Result
 *
 * @retval MORPHO_OK	The execution of the function was successful.
 * @retval -10001_to_-10499 Other errors
 */
typedef I	(F_MORPHO_CALLBACK_VERIF_CERTIF)(
					UL		i_ul_MsoCertificateSize,
					PUC		i_puc_MsoCertificate,
					PI		o_pi_Result);

typedef F_MORPHO_CALLBACK_VERIF_CERTIF *T_MORPHO_CALLBACK_VERIF_CERTIF;

//////////////////////////////////////////////////////////////////
//		CALLBACK VERIFY SIGNED DATA
//////////////////////////////////////////////////////////////////
#define T_MORPHO_CALLBACK_VERIF_SIGN	T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN


//////////////////////////////////////////////////////////////////
//		OTP
//////////////////////////////////////////////////////////////////

// OTP Options presence
/** @brief If this bit is set, the value of the sequence number N is valid */
#define OTP_VALID_SEQUENCE_NUMBER				0x0001
/** @brief If this bit is set, the value of the OTP password is valid. */
#define OTP_VALID_PASSWORD						0x0002
/** @brief If this bit is set, the value of the re-enrollment code pin is valid. */
#define OTP_VALID_PIN							0x0004
/** @brief If this bit is set, the value of the OTP parameters is valid */
#define OTP_VALID_PARAM							0x0008
/** @brief If this bit is set, the value of the hash algorithm is valid. */
#define OTP_VALID_HASH							0x0010
/** @brief If this bit is set, the value of the User ID is valid. */
#define OTP_VALID_USERID						0x0020
/** @brief If this bit is set, the value of the additional user data field 1 is valid. */
#define OTP_VALID_ADD1							0x0040
/** @brief If this bit is set, the value of the additional user data field 2 is valid. */
#define OTP_VALID_ADD2							0x0080
/** @brief If this bit is set, the value of the seed is valid. */
#define OTP_VALID_SEED							0x0100
/** @brief If this bit is set, the value of the OTP User Data is valid. */
#define OTP_VALID_OTP_USER_DATA					0x0200

// OTP Password Size
/** @brief Size of the OTP password. */
#define OTP_STRING_SIZE							16

// Pin Size
/** @brief Size of the Pin code */
#define OTP_PIN_SIZE							10

// OTP Algorithm parameters
/** @brief SHA1 hash algorithm */
#define OTP_HASH_ALGO_SHA1						1
/** @brief Default hash algorithm */
#define OTP_HASH_ALGO_DEFAULT					OTP_HASH_ALGO_SHA1
/** @brief  HOTP 6 digits hash algorithm */
#define OTP_HASH_ALGO_HOTP6						2
/** @brief  HOTP 7 digits hash algorithm */
#define OTP_HASH_ALGO_HOTP7						3
/** @brief  HOTP 8 digits hash algorithm */
#define OTP_HASH_ALGO_HOTP8						4

/** @brief Maximum number of hash algorithm */
#define OTP_HASH_ALGO_NB_MAX					OTP_HASH_ALGO_HOTP8


// Database
/** @brief Size Max of User additionnal fields */
#define	OTP_DB_FIELD_MAX_SIZE					15
/** @brief Number of Database Fields (User ID + 2 User data fields) */
#define OTP_DB_NB_FIELDS						3 // user ID + 2 Additional fields
/** @brief Number of Reference template */
#define OTP_DB_PK_MAX							2


// Key Check Value
/** @brief Length of a KCV buffer. */
#define	KCV_LEN									3
/** @brief ID of the Template Encoding Key for Match On Card. */
#define	ID_KENC									0
/** @brief ID of the Locking Secret. */
#define ID_KSECRET								1
/** @brief ID of the symmetric encryption key. */
#define ID_KS									2


// Max Size
#define PKMAT_MAXSIZE				512
#define PKCOMPV2_MAXSIZE			256
#define PKBASE_MAXSIZE				258
#define PKMOC_MAXSIZE				1234
//#define PKFVP_MAXSIZE				1024
#define PKFVP_MAXSIZE				4607 //(FVP)

#define IMAGE_MAXSIZE				200000

// Configuration Keys
/** @brief Sensor Window Position Configuration Key. */
#define CONFIG_SENSOR_WIN_POSITION_TAG   0x0E10
/** @brief UI Configuration Key. */
#define CONFIG_UI_CONFIG_TAG			 0x1410
/** @brief UI Reset Key. */
#define CONFIG_UI_RESET_TAG				 0x1411

// ISO-19794-4 FIR Default Values
/** @brief Lenght of a General Record Header.  */
#define GRH_LEN									32
/** @brief Lenght of a Finger Image Header.  */
#define FIH_LEN									14
/** @brief Default Format Identifier.  */
#define FORMAT_IDENT							"FIR"
/** @brief Default Version Number.  */
#define VERSION_NUMBER							"010"
/** @brief Default size of Record Lenght.  */
#define RL_SIZE									6
/** @brief Default value of reserved field of T_MORPHO_ISO19794_FIR_IMAGE.  */
#define RESERVED_FIR_IMAGE						0x00
/** @brief Default value of reserved field of T_MORPHO_ISO19794_FIR.  */
#define RESERVED_FIR							0x0000
/** @brief Default value of scale units.  */
#define SCALE_UNITS								0x01
/** @brief Default value of Capture device ID.  */
#define CAPTURE_DEVICE_ID						0x0000
/** @brief Default value of Image Acquisition Level.  */
#define IMG_ACQUISITION_LEVEL					0x001F
/** @brief Unsupported comression for ISO19794-4 format. */
#define UNSUPPORTED_COMPRESSION					0xFF

// Binary Descriptors
/** @brief Descriptor of the firmware version. */
#define BINDESC_VERSION				116
/** @brief Descriptor of the maximum database size. */
#define BINDESC_MAX_USER			117
/** @brief Descriptor of the maximum database number. */
#define BINDESC_MAX_DB				118
/** @brief Descriptor of the software type. */
#define BINDESC_SOFT				182
/** @brief Descriptor of the FLASH memory size. */
#define BINDESC_FLASH_SIZE			183
/** @brief Descriptor of the product name. */
#define BINDESC_PRODUCT_NAME		184
/** @brief Descriptor of the product identifier. */
#define BINDESC_PID					185
/** @brief Descriptor of the serial number. */
#define BINDESC_SN					186
/** @brief Descriptor of the OEM product identifier. */
#define BINDESC_OEM_PID				187
/** @brief Descriptor of the OEM serial number. */
#define BINDESC_OEM_SN				188
/** @brief Descriptor of the sensor identifier. */
#define BINDESC_SENSOR_ID			189
/** @brief Descriptor of the sensor serial number. */
#define BINDESC_SENSOR_SN			190
/** @brief Descriptor of the list of available licenses. */
#define BINDESC_LICENSES			191
/** @brief Descriptor of custom information on special devices. */
#define BINDESC_CUSTOM_DESCRIPTOR	195

// Log
/** @brief Enumeration containing the logging mode. */
typedef enum
{
	MORPHO_LOG_ENABLE = 0, /**< The logging is enabled */
	MORPHO_LOG_DISABLE = 1 /**< The logging is disabled whatever the configuration of the groups */
} T_MORPHO_LOG_Mode;

/** @brief Enumeration containing the logging level. */
typedef enum
{
	MORPHO_LOG_NOLOG = 0,   /**< Logging disabled */
	MORPHO_LOG_ERROR = 1,   /**< For recording the traces with the level ERROR */
	MORPHO_LOG_WARNING = 2, /**< for recording the traces with the level ERROR or WARNING */
	MORPHO_LOG_INFO = 3,    /**< For recording the traces with the level ERROR or WARNING or INFO */
	MORPHO_LOG_DEBUG = 4    /**< For recording the traces with the level ERROR or WARNING or INFO or DEBUG */
} T_MORPHO_LOG_Level;

#if defined(WIN32) || defined(_WIN32_WCE)
	#define INSTANCE_LIB		HINSTANCE
#else
	#define INSTANCE_LIB		PVOID
#endif

#endif // MORPHO_TYPES_H


