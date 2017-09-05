// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_Errors.h
 * @brief Definitions of error codes
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_ERRORS_H
#define MORPHO_ERRORS_H

/** @brief No error */
#define MORPHO_OK											0
/** @brief Biometrics device performed an internal error */
#define MORPHOERR_INTERNAL									-1
/** @brief Communication protocole error */
#define MORPHOERR_PROTOCOLE									-2
/** @brief Can not connect biometrics device */
#define MORPHOERR_CONNECT									-3
/** @brief Error while closing communication port */
#define MORPHOERR_CLOSE_COM									-4
/** @brief Invalid parameter */
#define MORPHOERR_BADPARAMETER								-5
/** @brief Not enough memory (in the PC). */
#define MORPHOERR_MEMORY_PC									-6
/** @brief Not enough memory for the creation of a database in the MSO */
#define MORPHOERR_MEMORY_DEVICE								-7
#define MORPHOERR_NO_SPACE_LEFT								-7
/** @brief Authentication or Identification failed */
#define MORPHOERR_NO_HIT									-8
/** @brief MSO returned an unknown status error */
#define	MORPHOERR_STATUS									-9
/** @brief The database is full */
#define MORPHOERR_DB_FULL									-10
/** @brief The database is empty */
#define MORPHOERR_DB_EMPTY									-11
/** @brief User has already been enrolled */
#define MORPHOERR_ALREADY_ENROLLED							-12
/** @brief The specified base does not exist */
#define MORPHOERR_BASE_NOT_FOUND							-13
/** @brief The specified base already exist */
#define MORPHOERR_BASE_ALREADY_EXISTS						-14
/** @brief User object has been instanciated without C_MORPHO_Database::GetUser */
#define MORPHOERR_NO_ASSOCIATED_DB							-15
/** @brief Database object has been instanciated without C_MORPHO_Device::GetDatabase */
#define MORPHOERR_NO_ASSOCIATED_DEVICE						-16
/** @brief The template is not valid */
#define MORPHOERR_INVALID_TEMPLATE							-17
/** @brief Command not yet implemented in this release */
#define MORPHOERR_NOT_IMPLEMENTED							-18
/** @brief No response after defined time */
#define MORPHOERR_TIMEOUT									-19
/** @brief No templates have been registered (using C_MORPHO_TemplateList::PutTemplate). */
#define	MORPHOERR_NO_REGISTERED_TEMPLATE					-20
/** @brief Field does not exist */
#define MORPHOERR_FIELD_NOT_FOUND							-21
/** @brief Class has been corrupted */
#define MORPHOERR_CORRUPTED_CLASS							-22
/** @brief There are too many templates */
#define MORPHOERR_TO_MANY_TEMPLATE							-23
/** @brief There are too many fields */
#define MORPHOERR_TO_MANY_FIELD								-24
/** @brief Templates with differents formats are mixed */
#define MORPHOERR_MIXED_TEMPLATE							-25
/** @brief Command has been aborted */
#define MORPHOERR_CMDE_ABORTED								-26
/** @brief Invalid PK format */
#define MORPHOERR_INVALID_PK_FORMAT							-27
/** @brief User gave twice the same finger */
#define MORPHOERR_SAME_FINGER								-28
/** @brief The number of the additional field is more than 128 */
#define MORPHOERR_OUT_OF_FIELD								-29
/** @brief UserID is not valid: either the record identifier does not exist in the database (Consulting operation), either it already exists (Enroll operation). */
#define MORPHOERR_INVALID_USER_ID							-30
/** @brief The user data are not valid */
#define MORPHOERR_INVALID_USER_DATA							-31
/** @brief Additional field name length is more than MORPHO_FIELD_NAME_LEN */
#define MORPHOERR_FIELD_INVALID								-32
/** @brief User is not found */
#define MORPHOERR_USER_NOT_FOUND							-33
/** @brief Serial COM has not been opened */
#define MORPHOERR_COM_NOT_OPEN								-34
/** @brief This element is already present in the list */
#define MORPHOERR_ELT_ALREADY_PRESENT						-35
/** @brief You have to call C_MORPHO_Database::DbQueryFirst to initialize the querry */
#define MORPHOERR_NOCALLTO_DBQUERRYFIRST					-36
/** @brief The communication callback functions returns error between -10000 and -10499 */
#define MORPHOERR_USER										-37
/** @brief The Compression is not valid */
#define MORPHOERR_BAD_COMPRESSION							-38
/** @brief Security error */
#define MORPHOERR_SECU										-39
/** @brief The MSO has not the certificate necessary to verify the signature */
#define MORPHOERR_CERTIF_UNKNOW								-40
/** @brief The class has been destroyed */
#define MORPHOERR_INVALID_CLASS								-41
/** @brief The specified Usb device is not plugged */
#define MORPHOERR_USB_DEVICE_NAME_UNKNOWN					-42
/** @brief The certificate is not valid */
#define MORPHOERR_CERTIF_INVALID							-43
/** @brief The certificate identity is not the same than the X984 certificate identity */
#define MORPHOERR_SIGNER_ID									-44
/** @brief The X984 certificate identity size is different to 20 octets (SHA_1 size). */
#define MORPHOERR_SIGNER_ID_INVALID							-45
/** @brief False Finger Detected */
#define MORPHOERR_FFD										-46
/** @brief The finger can be too moist or the scanner is wet */
#define MORPHOERR_MOIST_FINGER								-47
/** @brief The Morpho MorphoSmart Service Provider Usb Server is stopped or not installed */
#define MORPHOERR_NO_SERVER									-48
/** @brief No parameter has been initialized */
#define MORPHOERR_OTP_NOT_INITIALIZED						-49
/** @brief Code pin is needed : it is the first enrollment */
#define MORPHOERR_OTP_PIN_NEEDED							-50
/** @brief User is not allowed to be reenrolled */
#define MORPHOERR_OTP_REENROLL_NOT_ALLOWED					-51
/** @brief Enrollment failed */
#define MORPHOERR_OTP_ENROLL_FAILED							-52
/** @brief Identification failed */
#define MORPHOERR_OTP_IDENT_FAILED							-53
/** @brief No more OTP available (sequence number = 0). */
#define MORPHOERR_NO_MORE_OTP								-54
/** @brief Authentication or Identification failed */
#define MORPHOERR_OTP_NO_HIT								-55
/** @brief Enrollment needed before generating OTP */
#define MORPHOERR_OTP_ENROLL_NEEDED							-56
/** @brief The device is locked */
#define MORPHOERR_DEVICE_LOCKED								-57
/** @brief The device is not locked */
#define MORPHOERR_DEVICE_NOT_LOCK							-58
/** @brief ILV_OTP_GENERATE Locked */
#define MORPHOERR_OTP_LOCK_GEN_OTP							-59
/** @brief ILV_OTP_SET_PARAMETERS  Locked */
#define MORPHOERR_OTP_LOCK_SET_PARAM						-60
/** @brief ILV_OTP_ENROLL_USER Locked */
#define MORPHOERR_OTP_LOCK_ENROLL							-61
/** @brief Security level mismatch: attempt to match fingerprint template in high security level (MorphoSmart&tm; FINGER VP only). */
#define MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH			-62
/** @brief Misplaced or withdrawn finger has been detected during acquisition (MorphoSmart&tm; FINGER VP only). */
#define MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN			-63
/** @brief A required license is missing */
#define MORPHOERR_LICENSE_MISSING							-64
/** @brief The MorphoSmart&tm; FINGER VP was unsuccessful in making a multimodal template compatible with advanced security levels */
#define MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH			-65
/** @brief The MorphoSmart&tm; was unsuccessful in capturing the fingerprint with a quality greater than or equal to the specified threshold */
#define MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY			-66
/** @brief Image information are mismatching for ISO-19794-4 FIR creation */
#define MORPHOERR_ISO19794_FIR_IMAGES_MISMATCH_PARAMETER	-67
/** @brief Misplaced finger has been detected by FFD during acquisition (MorphoSmart&tm; MSO 3x1 FFD only) */
#define MORPHOERR_FFD_FINGER_MISPLACED						-68
/** @brief A required key is missing */
#define MORPHOERR_KEY_NOT_FOUND								-69
/** @brief Compression ratio higher than that recommended by the norme  ISO-19794-4 (15).<br>The compression ratio must be less than or equal to 15.*/
#define MORPHOWARNING_WSQ_COMPRESSION_RATIO					-70
/** @brief The MorphoSmart&tm; FINGER VP was unsuccessful in making a multimodal template compatible with advanced security levels */
#define MORPHOERR_ADVANCED_SECURITY_LEVEL_NOT_AVAILABLE		-71
/*! @brief A functionality has been requested, but is not available on the currently connected device */
#define MORPHOERR_UNAVAILABLE								-72


#endif // MORPHO_ERRORS_H

