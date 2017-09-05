// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_User.h
 * @brief Definitions of the C_MORPHO_User class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_USER_H
#define MORPHO_USER_H

#ifndef SWIG
#ifdef MORPHO_SDK
#define MORPHO_API __declspec(dllexport)
#else
#define MORPHO_API __declspec(dllimport)
#endif
#endif


class C_MORPHO_Database;

/**
 * @brief Class performing operations on a user templates and database.
 *
 * This class performs operations related to a user. As a user is always associated
 * to a database, C_MORPHO_User has to be instantiated with C_MORPHO_Database::GetUser().
 * The OTP operations use a specific database and not the standard previous database.
 * There is only one user enrolled in the OTP database. This specific database has the
 * following configuration: 2 OTP reference templates (i.e. 2 PKs), 1 record (=1 User ID),
 * and 2 additional public fields of 15 bytes for each one.
 * Templates are accessed thanks to inherited C_MORPHO_TemplateList class.
 * Fields are accessed thanks to inherited C_MORPHO_FieldList class.
 *
 */
class MORPHO_API C_MORPHO_User: public C_MORPHO_TemplateList, public C_MORPHO_FieldList
{
private:
	UL	m_ul_magicFirst;
public:
	friend class C_MORPHO_Database;
	friend class C_MORPHO_UserList;

	/** Default constructor */
	C_MORPHO_User();

	/**
	 * Constructor
	 * @deprecated left for compatibility, do not use in new projects
	 */
	C_MORPHO_User(	UC	i_uc_lenUserID,
					PUC	i_puc_UserID);

	/**
	 * By copy constructor
	 * @param[in] i_px_User source object to copy
	 */
	C_MORPHO_User(const C_MORPHO_User& i_px_User);

	/**
	 * Destructor
	 */
	virtual ~C_MORPHO_User();

	/**
	 * Assignation operator
	 * @param[in] i_px_User source object to copy
	 */
	C_MORPHO_User& operator= (const C_MORPHO_User& i_px_User);

	/**
	 * This function removes the user from the local database.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_USER_ID	The UserID does not exist in the database.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The database does not exist.
	 * @retval #MORPHOERR_USER_NOT_FOUND	The user doesn't exist in the database.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB	Database is created without C_MORPHO_Database::GetUser.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I DbDelete( VOID );

	/**
	 * This command creates a new record in the local database with user characteristics.
	 * If a record with the same identifier already exists the command fails
	 * (VerifyAndUpdateToDatabase() should be used in this case).
	 * Prior calling this method, you should fill up template list and field list
	 * using C_MORPHO_TemplateList:: PutTemplate() and C_MORPHO_FieldList:: PutField().
	 * Call the SetNoCheckOnTemplateForDBStore method to add user without checking that it
	 * has already been enrolled.
	 *
	 * @warning If @ref SetTemplateUpdateMask has been called with a value different from 0, no new user will be created in the database. Instead, an existing user will
	 * be updated with the newly generated templates, provided @p i_uc_SaveRecord is set to 1. In this situation, the user ID <b>must already</b> exist in the database
	 * to identify the record in the database to update with the provided templates.
	 *
	 * @retval #MORPHO_OK					The execution of the function succeeded.
	 * @retval #MORPHOERR_INTERNAL 			An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER		Invalid parameter. You should call C_MORPHO_TemplateList:: PutTemplate().
	 * @retval #MORPHOERR_INVALID_USER_ID	The record identifier already exists in the database.
	 * @retval #MORPHOERR_INVALID_TEMPLATE	The reference template is not valid: bad identifier, corrupted minutiae.
	 * @retval #MORPHOERR_ALREADY_ENROLLED	The person is already in this database. Call SetNoCheckOnTemplateForDBStore to force enrollment.
	 * @retval #MORPHOERR_BASE_NOT_FOUND		Database doesn't exist.
	 * @retval #MORPHOERR_DB_FULL			The maximum number of users that can be stored in the database has been reached.
	 * @retval #MORPHOERR_SAME_FINGER		User give twice the same finger.
	 * @retval #MORPHOERR_FIELD_NOT_FOUND	Invalid field number.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB	Database is created without C_MORPHO_Database::GetUser
	 * @retval #MORPHOERR_LICENSE_MISSING	A required license is missing (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_PROTOCOLE			Communication protocol error
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I DbStore( VOID );

	/**
	 * This command updates already existing public fields in the local database.
	 * C_MORPHO_User object has to be filled with public fields that need to be updated
	 * (see method PutField).
	 * Prior calling this method, you should fill up field list using
	 * C_MORPHO_FieldList:: PutField().
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_INVALID_USER_DATA	The input user data is not valid: bad identifier, wrong size.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified base doesn't exist.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB	Database is created without C_MORPHO_Database::GetUser.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I DbUpdatePublicFields( VOID );

	/**
	 * This command updates an already existing record in the local database.
	 * If no record with the same identifier already exists the command fails
	 * (AddToDatabase() should be used).
	 * This command updates already existing fields (public or private) in the local database.
	 * C_MORPHO_User object has to be filled with public fields that need to be updated
	 * (see method PutField).
	 * The field update is performed only if the user identity is verified.
	 * Prior calling this method, you should fill up field list using C_MORPHO_FieldList:: PutField().
	 *
	 * @param[in] i_us_Timeout
	 * - Finger detection timeout in seconds. Its value must be between 0 and 0xFFFF.
	 * 0 corresponds to an infinite timeout.
	 * @param[in] i_us_FAR
	 * - This parameter specifies how tight the matching threshold is.
	 * Morpho recommends #MORPHO_FAR_5 (see paragraph #T_MORPHO_FAR).
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements.
	 * This mask describes the asynchronous status events that will trig the callback function.
	 * 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want
	 * to receive the command status (move finger more left, remove finger,...)
	 * and low-resolution images.
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of the asynchronous status. NULL if not used.
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 * @param[in] i_i_CoderChoice
	 * - contains the biometric coder to use (#MORPHO_MSO_V9_CODER or #MORPHO_MSO_V9_JUV_CODER).
	 * Morpho recommends using #MORPHO_MSO_V9_CODER.
	 * Please refer to the MorphoSmartHostInterface document for details.
	 *
	 * @param[in] i_ul_DetectModeChoice
	 * - Bitmask of the following:
	 *     - #MORPHO_VERIF_DETECT_MODE: more permissive mode than default; MorphoSmart&tm; detects more easily finger presence, but might issue lower quality templates.
	 *     - #MORPHO_ENROLL_DETECT_MODE: strongest detection mode (default mode).
	 *     - #MORPHO_WAKEUP_LED_OFF: (only available on MorphoSmart&tm; MSO FFD) leds are turned off while waiting for a finger (impedance wakeup).
	 *     - #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE: (not available on MorphoSmart&tm; CBM-V) force the finger to cover the top of the capture area to increase quality.
	 *     - #MORPHO_WAKEUP_LED_ON: (only available on MorphoSmart&tm; FINGER VP) leds are turned on while waiting for a finger.
	 *
	 * @param[in] i_ul_MatchingStrategy
	 * - Value among of the following:
	 * 		- #MORPHO_STANDARD_MATCHING_STRATEGY: default strategy.
	 * 		- #MORPHO_ADVANCED_MATCHING_STRATEGY: less FRR, but more processing time (not available on MorphoSmart&tm; FINGER VP).
	 *
	 * @note In parameter i_ul_DetectModeChoice:
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_ENROLL_DETECT_MODE cannot be used together.
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE cannot be used together.
	 *
	 * @retval #MORPHO_OK									The matching was successful.
	 * @retval #MORPHOERR_INTERNAL 							An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER						The matching threshold value or timeout value is out of range.
	 * @retval #MORPHOERR_TIMEOUT							The finger detection timeout has expired.
	 * @retval #MORPHOERR_NO_HIT								Finger does not match.
	 * @retval #MORPHOERR_CMDE_ABORTED						Command is canceled.
	 * @retval #MORPHOERR_MEMORY_PC							Not enough memory on the PC.
	 * @retval #MORPHOERR_PROTOCOLE							Communication protocol error.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB					Database is created without C_MORPHO_Database::GetUser.
	 * @retval #MORPHOERR_CORRUPTED_CLASS					Class has been corrupted.
	 * @retval #MORPHOERR_FFD								False Finger Detected.
	 * @retval #MORPHOERR_MOIST_FINGER						The finger can be too moist or the scanner is wet.
	 * @retval #MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN	Finger was misplaced or has been withdrawn from sensor during acquisition (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_FFD_FINGER_MISPLACED				Finger was misplaced during acquisition (MorphoSmart&tm; MSO 3x1 FFD only).
	 */
	I DbVerifyAndUpdate(
									US							i_us_Timeout,
									T_MORPHO_FAR				i_us_FAR,
									UL							i_ul_CallbackCmd,
									T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
									PVOID						i_pv_CallbackArgument,
									I							i_i_CoderChoice,
									UL							i_ul_DetectModeChoice,
									UL							i_ul_MatchingStrategy
									);

	/**
	 * @see DbVerifyAndUpdate (compatibility with MORPHO_SDK.dll Version 4.3.3.0)
	 */
	I DbVerifyAndUpdate(
						US							i_us_Timeout,
						T_MORPHO_FAR				i_us_FAR,
						UL							i_ul_CallbackCmd,
						T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
						PVOID						i_pv_CallbackArgument,
						I							i_i_CoderChoice
						);

	/**
	 * @see DbVerifyAndUpdate (compatibility with MORPHO_SDK.dll Version 4.0.3.0)
	 */
	I DbVerifyAndUpdate(	US							i_us_Timeout,
						T_MORPHO_FAR				i_us_FAR,
						UL							i_ul_CallbackCmd,
						T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
						PVOID						i_pv_CallbackArgument);

	/**
	 * This method retrieves the content of a field.
	 *
	 * @param[in] i_ul_FieldIndex
	 * - Field index (0 is used for UserID, first supplementary field is 1).
	 * @param[out] o_ul_LenField
	 * - Data field length.
	 * @param[out] o_puc_DataField
	 * - Data field. This parameter is a pointer
	 * to the internal object. It must not be allocated, released or modified.
	 * Before performing such tasks, the application should make a copy of this data.
	 *
	 * Verify that you have initialized the field with PutField() function before retrieving the field
	 * otherwise the application hangs.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified base doesn't exist.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB	Database is created without C_MORPHO_Database::GetUser.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_FIELD_NOT_FOUND	The field is not found
	 * @retval #MORPHOERR_USER_NOT_FOUND	The user does not exist in the database.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetField(	UL			i_ul_FieldIndex,
				UL	&		o_ul_LenField,
				PUC	&		o_puc_DataField	);

	/**
	 * This method retrieves the content of a filter-type field.
	 *
	 * @param[in] i_ul_FilterFieldIndex
	 * - Field index (0 is used for UserID, first supplementary field is 1).
	 * @param[out] o_ul_LenField
	 * - Data field length.
	 * @param[out] o_puc_DataField
	 * - Data field. This parameter is a pointer
	 * to the internal objet. It must not be allocated, released or modified.
	 * Before performing such tasks, the application should make a copy of this data.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	Specified field is not a filter field.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified base doesn't exist.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB	User is created without C_MORPHO_Database::GetUser.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_FIELD_NOT_FOUND	The field is not found
	 * @retval #MORPHOERR_USER_NOT_FOUND	The user does not exist in the database.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	Filtering is not available on currently connected device.
	 */
	I GetFilterData(	UL			i_ul_FilterFieldIndex,
						UL	&		o_ul_LenField,
						PUC	&		o_puc_DataField	);

	/**
	 * This method is not available yet.
	 *
	 * @param[in] i_ul_FilterFieldIndex
	 * - RFU
	 * @param[in] i_ul_LenField
	 * - RFU
	 * @param[in] i_puc_DataField
	 * - RFU
	 *
	 * @retval #MORPHOERR_NOT_IMPLEMENTED	Method not available yet.
	 */
	I UpdateFilterData(	UL			i_ul_FilterFieldIndex,
						UL			i_ul_LenField,
						PUC			i_puc_DataField	);


	/**
	 * This function captures and enrolls a live finger. The number of fingers can be specified.
	 * The calculated minutiae can also be exported to the host. The minutiae are calculated
	 * after three fingerprint image acquisitions (the user has to put each finger 3 times
	 * on the MorphoSmart&tm; sensor). To obtain the best accuracy, it is strongly recommended
	 * to use the fore, the thumb or the middle fingers. Acquired templates and fields can be
	 * accessed thanks to methods defined in C_MORPHO_TemplateList and C_MORPHO_FieldList classes.
	 * Fingerprint quality and advanced security levels compatibility (MorphoSmart&tm; FINGER VP only)
	 * are only available if export of templates is activated.
	 * C_MORPHO_TemplateList::GetFVPTemplate must be called to retrieve multimodal template.
	 * C_MORPHO_TemplateList::GetTemplate must be called to retrieve fingerprint template.<br>
	 *
	 * @note Image export is activated with C_MORPHO_TemplateList::SetActiveFullImageRetrieving(). <br>If Image export is activated, Image is retrieved with C_MORPHO_TemplateList::GetFullImageRetrieving()
	 *
	 * @warning If @ref SetTemplateUpdateMask has been called with a value different from 0, no new user will be created in the database. Instead, an existing user will
	 * be updated with the newly generated templates, provided @p i_uc_SaveRecord is set to 1. In this situation, the user ID <b>must already</b> exist in the database
	 * to identify the record in the database to update with the generated templates.
	 *
	 * @param[in] i_us_Timeout
	 * - Finger detection timeout in seconds. Its value must be between 0 and 0xFFFF.
	 * 0 corresponds to an infinite timeout.
	 *
	 * @param[in] i_uc_AcquisitionThreshold
	 * - Minimum value of fingerprint quality. This value can be 0 (strongly recommended) or
	 * any value between 20 and 100.
	 *
	 * @param[in] i_uc_AdvancedSecurityLevelsRequired
	 * - Set this value to 1 to get multimodal templates compatible with advanced security levels
	 * (levels greater than Standard), otherwise set this value to 0 (MorphoSmart&tm; FINGER VP only).
	 *
	 * @param[in] i_x_CompressAlgo
	 * - Compression algorithm to be used to compress the fingerprint image. Available algorithms are:
	 *	- #MORPHO_NO_COMPRESS
	 *	- #MORPHO_COMPRESS_V1
	 *	- #MORPHO_COMPRESS_WSQ
	 * - Image export is activated with C_MORPHO_TemplateList::SetActiveFullImageRetrieving().
	 * If Image export is activated, Image is retrieved with C_MORPHO_TemplateList::GetFullImageRetrieving()
	 *
	 * @param[in] i_uc_CompressRate
	 * - Compression rate used by the fingerprint image compression algorithm:
	 *	- useless for #MORPHO_NO_COMPRESS and #MORPHO_COMPRESS_V1 algorithms (must be set to 0).
	 *	- can vary between 2 and 255 for #MORPHO_COMPRESS_WSQ algorithm, usual recommended value is 10.
	 *
	 * @param[in] i_us_ExportMinutiae
	 * - Defines the format of the exported minutiae.
	 *     - Set this value to 0 to exclude the calculated minutiae from the reply.
	 *     - Set this value to 1 to export the minutiae with its default size.
	 * - For \ref MORPHO_PK_COMP fingerprint template format only, this field can be set to a value from 170 (0xAA) to 255 (0xFF) to limit the size of the fingerprint template. If the fingerprint template size is higher than the required value, it is compressed to match the requirement before being included in the reply, otherwise the fingerprint template is included without modification. It means that the fingerprint template size is less or equal to the specified value.
	 * - For \ref MORPHO_PK_ANSI_378, \ref MORPHO_PK_ISO_FMC_CS, \ref MORPHO_PK_ISO_FMC_NS and \ref MORPHO_PK_ISO_FMR fingerprint template formats, this field can be set to a value from 2 (0x02) to 255 (0xFF) to limit the number of PK (minutiae) in the fingerprint template.
	 *
	 * @param[in] i_uc_FingerNumber
	 * - The number of fingers to enroll. This function can enroll 1 or 2 fingers.
	 * Set this value to 0x01 to enroll 1 finger per user.
	 * Set this value to 0x02 to enroll 2 fingers per user.
	 *
	 * @param[in] i_x_TemplateType
	 * - Indicates the template acquisition format. Value can be
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_COMP "MORPHO_PK_COMP" (recommended)
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_COMP_NORM "MORPHO_PK_COMP_NORM"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_MAT "MORPHO_PK_MAT"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_MAT_NORM "MORPHO_PK_MAT_NORM"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_ANSI_378 "MORPHO_PK_ANSI_378"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_MINEX_A "MORPHO_PK_MINEX_A"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_ISO_FMR "MORPHO_PK_ISO_FMR"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_ISO_FMC_NS "MORPHO_PK_ISO_FMC_NS"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_ISO_FMC_CS "MORPHO_PK_ISO_FMC_CS"
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_PK_PKLITE "MORPHO_PK_PKLITE" (only available on MorphoSmart&tm; CBM-E3)
	 *    - @ref T_MORPHO_TYPE_TEMPLATE::MORPHO_NO_PK_FP "MORPHO_NO_PK_FP"
	 *
	 * @param[in] i_x_FVPTemplateType
	 * - Indicates the template acquisition format. Value can be
	 *    - @ref MORPHO_PK_FVP
	 *    - @ref MORPHO_NO_PK_FVP
	 *
	 * @param[in] i_uc_SaveRecord
	 * - Indicates if the template will saved in the data base or not
	 *		- 0 the template will not be saved in the data base
	 *		- 1 the template will be be savaed in the data base
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements.
	 * This mask describes the asynchronous status events that will trig the callback function.
	 * 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want
	 * to receive the command status (move finger up...) and low-resolution images.
	 *
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of the asynchronous status. NULL if not used.
	 *
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 *
	 * @param[in] i_i_CoderChoice
	 * - Contains the biometric coder to use (#MORPHO_MSO_V9_CODER or #MORPHO_MSO_V9_JUV_CODER).
	 * Morpho recommends using #MORPHO_MSO_V9_CODER.
	 * Please refer to the MorphoSmartHostInterface document for details.
	 *
	 * @param[in] i_ul_DetectModeChoice
	 * - Bitmask of the following:
	 *     - #MORPHO_VERIF_DETECT_MODE: more permissive mode than default; MorphoSmart&tm; detects more easily finger presence, but might issue lower quality templates.
	 *     - #MORPHO_ENROLL_DETECT_MODE: strongest detection mode (default mode).
	 *     - #MORPHO_WAKEUP_LED_OFF: (only available on MorphoSmart&tm; MSO FFD) leds are turned off while waiting for a finger (impedance wakeup).
	 *     - #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE: (not available on MorphoSmart&tm; CBM-V) force the finger to cover the top of the capture area to increase quality.
	 *     - #MORPHO_WAKEUP_LED_ON: (only available on MorphoSmart&tm; FINGER VP) leds are turned on while waiting for a finger.
	 *
	 * @param[in] o_x_Template
	 * - Exported Template
	 *
	 * @param[in] i_px_MocParameters
	 * - Match On Card Capture parameters
	 *
	 * @note In parameter i_ul_DetectModeChoice:
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_ENROLL_DETECT_MODE cannot be used together.
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE cannot be used together.
	 *
	 * @retval #MORPHO_OK									The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL							An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER						One or more input parameters are out of range.
	 * @retval #MORPHOERR_INVALID_USER_DATA					The input user data is not valid: bad identifier or wrong size.
	 * @retval #MORPHOERR_TIMEOUT							The finger detection timeout has expired.
	 * @retval #MORPHOERR_BASE_NOT_FOUND					The specified database does not exist.
	 * @retval #MORPHOERR_DB_FULL							The maximum number of users that can be stored in the local database has been reached.
	 * @retval #MORPHOERR_CMDE_ABORTED						Command is canceled.
	 * @retval #MORPHOERR_MEMORY_PC							Not enough memory on tne PC.
	 * @retval #MORPHOERR_SAME_FINGER						The user gave the same finger twice.
	 * @retval #MORPHOERR_PROTOCOLE							Communication protocol error.
	 * @retval #MORPHOERR_INVALID_USER_ID					The user already exists in the database (same UserID).
	 * @retval #MORPHOERR_USER_NOT_FOUND					A template update has been requested through @ref SetTemplateUpdateMask, but the UserID has not been found in the database.
	 * @retval #MORPHOERR_INVALID_PK_FORMAT					Invalid template format
	 * @retval #MORPHOERR_FFD								False Finger Detected.
	 * @retval #MORPHOERR_MOIST_FINGER						The finger can be too moist or the scanner is wet.
	 * @retval #MORPHOERR_LICENSE_MISSING					A required license is missing (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH	Failed to make a multimodal template compatible with advanced security levels (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY	Failed to capture the fingerprint with a quality greater than or equal to the specified threshold.
	 * @retval #MORPHOERR_KEY_NOT_FOUND						The specified key is missing, unable to encrypt biometrics data.
	 * @retval #MORPHOERR_ALREADY_ENROLLED					User has already been enrolled.
	 * @retval #MORPHOERR_NO_HIT							Authentication or Identification failed.
	 */
	I Enroll(
					US							i_us_Timeout,
					UC							i_uc_AcquisitionThreshold,
					UC							i_uc_AdvancedSecurityLevelsRequired,
					T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
					UC							i_uc_CompressRate,
					UC							i_uc_ExportMinutiae,
					UC							i_uc_FingerNumber,
					T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
					T_MORPHO_FVP_TYPE_TEMPLATE	i_x_FVPTemplateType,
				    UC							i_uc_SaveRecord,
					UL							i_ul_CallbackCmd,
					T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
					PVOID						i_pv_CallbackArgument,
					I							i_i_CoderChoice,
					UL							i_ul_DetectModeChoice,
					C_MORPHO_TemplateList &		o_x_Template,
					PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters
			);

	/**
	 * @see Enroll (kept for compatibility with MORPHO_SDK.dll Version 6.4.2.0)
	 */
	I Enroll(
						US							i_us_Timeout,
						UC							i_uc_AcquisitionThreshold,
						UC							i_uc_AdvancedSecurityLevelsRequired,
						T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
						UC							i_uc_CompressRate,
						UC							i_uc_ExportImage,
						UC							i_uc_ExportMinutiae,
						UC							i_uc_FingerNumber,
						T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
						T_MORPHO_FVP_TYPE_TEMPLATE	i_x_FVPTemplateType,
					    UC							i_uc_SaveRecord,
						UL							i_ul_CallbackCmd,
						T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
						PVOID						i_pv_CallbackArgument,
						I							i_i_CoderChoice,
						UL							i_ul_DetectModeChoice,
						C_MORPHO_TemplateList &		o_x_Template,
						T_MORPHO_IMAGE				*o_px_Image,
						PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters
				);

	/**
	 * @see Enroll (kept for compatibility with MORPHO_SDK.dll Version 6.1.3.0)
	 */
	I Enroll(
						US							i_us_Timeout,
						T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
						UC							i_uc_CompressRate,
						UC							i_uc_ExportImage,
						UC							i_uc_ExportMinutiae,
						UC							i_uc_FingerNumber,
						T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
						T_MORPHO_FVP_TYPE_TEMPLATE	i_x_FVPTemplateType,
					    UC							i_uc_SaveRecord,
						UL							i_ul_CallbackCmd,
						T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
						PVOID						i_pv_CallbackArgument,
						I							i_i_CoderChoice,
						UL							i_ul_DetectModeChoice,
						C_MORPHO_TemplateList &		o_x_Template,
						T_MORPHO_IMAGE				*o_px_Image,
						PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters
				);


	/**
	 * @see Enroll (kept for compatibility with MORPHO_SDK.dll Version 5.2.4.0)
	 */
	I Enroll(
				US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice
				);

	/**
	 * @see Enroll (kept for compatibility with MORPHO_SDK.dll Version 4.2.3.2)
	 */
	I Enroll(
				US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				I							i_i_CoderChoice
				);

	/**
	 * @see Enroll (kept for compatibility with MORPHO_SDK.dll Version 4.0.3.0)
	 */
	I Enroll(	US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument);

	/**
	 * This method sets the template update mask for the next @ref Enroll or @ref DbStore.
	 *
	 * @param[in] i_ul_updateMask
	 * - bitmask for fingerprint update for an existing user. Set to 0 for default method behaviour (enroll a new user). Any other value
	 *   will trigger a fingerprint add or replace for an existing user in the database instead of enrolling a new user. Selection of
	 *   finger slot index to add or replace is achieved through bit index set to 1 in the parameter, least significant bit being finger
	 *   slot 0. Therefore, maximum value allowed for the parameter depends on the maximum number of fingers per user. As an example, see
	 *   the available values below for a database with 2 fingers per user:
	 *     - 0x00000000 : default behaviour, a new user will be enrolled
	 *     - 0x00000001 : update first finger
	 *     - 0x00000002 : add (or update) second finger
	 *     - 0x00000003 : update both fingers (or update first finger and add a second one)
	 *
	 * @note When this method is called with a value different than 0, please keep in mind that the index of a generated or provided
	 * template may not match the finger index in which it will eventually be stored in the database. For example, when using value
	 * 0x00000002, only one template is expected, but it will be stored as a second finger in the database.
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER		A finger index greater than the maximum available on the current database has been requested.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB	The user has been created without C_MORPHO_Database::GetUser
	 */
	I SetTemplateUpdateMask(	UL			i_ul_updateMask);


	/**
	 * This function captures a live finger and checks if it matches with the user referred to.
	 *
	 * @param[in] i_us_Timeout
	 * - Finger detection timeout in seconds. Its value must be between 0 and 0xFFFF.
	 * 0 corresponds to an infinite timeout.
	 *
	 * @param[in] i_us_FAR
	 * - This parameter specifies how tight the matching threshold is.
	 * Morpho recommends #MORPHO_FAR_5 (see paragraph #T_MORPHO_FAR).
	 *
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements.
	 * This mask describes the asynchronous status events that will trig the callback function.
	 * 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want
	 * to receive the command status (move finger up...) and low-resolution images.
	 *
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of the asynchronous status. NULL if not used.
	 *
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 *
	 * @param[out] o_pul_MatchingScore
	 * - Contains the result matching score. NULL if not used.
	 * For security reason, the secure MorphoSmart&tm; can not export the matching score
	 * because a "rogue" application can mount an "hillclimbing" attack by sequentially
	 * randomly modifying a sample and retaining only the changes that produce an increase
	 * in the returned score.
	 *
	 * @param[out] o_puc_ExportNumPk
	 * - Contains the result matching PK number. NULL if not used.
	 * If o_puc_ExportNumPk is not NULL, if a false or a too moist finger
	 * is detected (for MSOxx1 only), or if the finger is not authenticated,
	 * the returned matching PK number value is 0xFF.
	 *
	 * @param[in] i_i_CoderChoice
	 * - contains the biometric coder to use (#MORPHO_MSO_V9_CODER or #MORPHO_MSO_V9_JUV_CODER).
	 * Morpho recommends using #MORPHO_MSO_V9_CODER.
	 * Please refer to the MorphoSmartHostInterface document for details.
	 *
	 * @param[in] i_ul_DetectModeChoice
	 * - Bitmask of the following:
	 *     - #MORPHO_VERIF_DETECT_MODE: more permissive mode than default; MorphoSmart&tm; detects more easily finger presence, but might issue lower quality templates.
	 *     - #MORPHO_ENROLL_DETECT_MODE: strongest detection mode (default mode).
	 *     - #MORPHO_WAKEUP_LED_OFF: (only available on MorphoSmart&tm; MSO FFD) leds are turned off while waiting for a finger (impedance wakeup).
	 *     - #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE: (not available on MorphoSmart&tm; CBM-V) force the finger to cover the top of the capture area to increase quality.
	 *     - #MORPHO_WAKEUP_LED_ON: (only available on MorphoSmart&tm; FINGER VP) leds are turned on while waiting for a finger.
	 *
	 * @param[in] i_ul_MatchingStrategy
	 * - Value among of the following:
	 * 		- #MORPHO_STANDARD_MATCHING_STRATEGY: default strategy.
	 * 		- #MORPHO_ADVANCED_MATCHING_STRATEGY: less FRR, but more processing time (not available on MorphoSmart&tm; FINGER VP).
	 *
	 * @note In parameter i_ul_DetectModeChoice:
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_ENROLL_DETECT_MODE cannot be used together.
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE cannot be used together.
	 *
	 * @retval #MORPHO_OK									The comparison was successful.
	 * @retval #MORPHOERR_INTERNAL 							An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER						The matching threshold value or timeout value is out of range or there is no input biometric data.
	 * @retval #MORPHOERR_INVALID_TEMPLATE					The reference template is not valid: bad identifier, corrupted minutiae.
	 * @retval #MORPHOERR_TIMEOUT							The finger detection timeout has expired.
	 * @retval #MORPHOERR_NO_HIT								The function returned a No Hit.
	 * @retval #MORPHOERR_CMDE_ABORTED						Command is canceled.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB					Database is created without C_MORPHO_Database::GetUser.
	 * @retval #MORPHOERR_PROTOCOLE							Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS					Class has been corrupted.
	 * @retval #MORPHOERR_FFD								False Finger Detected.
	 * @retval #MORPHOERR_MOIST_FINGER						The finger can be too moist or the scanner is wet.
	 * @retval #MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH		Fingerprint template cannot be matched in high security level (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN	Finger was misplaced or has been withdrawn from sensor during acquisition (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_FFD_FINGER_MISPLACED				Finger was misplaced during acquisition (MorphoSmart&tm; MSO 3x1 FFD only).
	 * @retval #MORPHOERR_UNAVAILABLE						Functionality is not available on currently connected device.
	 */
	I Verify(
						US							i_us_Timeout,
						T_MORPHO_FAR				i_us_FAR,
						UL							i_ul_CallbackCmd,
						T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
						PVOID						i_pv_CallbackArgument,
						PUL							o_pul_MatchingScore,
						PUC							o_puc_ExportNumPk,
						I							i_i_CoderChoice,
						UL							i_ul_DetectModeChoice,
						UL							i_ul_MatchingStrategy
						);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 4.3.3.0)
	 */
	I Verify(
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_ExportNumPk,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice
				);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 4.2.3.2)
	 */
	I Verify(
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_ExportNumPk,
				I							i_i_CoderChoice
				);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 4.0.3.0)
	 */
	I Verify(
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_ExportNumPk);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 4.0.2.0)
	 */
	I Verify(
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I Verify(
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument);

	/**
	 * This function cancels the live acquisition.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DB	Database is created without C_MORPHO_Database::GetUser.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I CancelLiveAcquisition ( VOID );

    /**
	 * This function retreives the finger templates quality of a user in the database.
	 * @param[in] i_uc_IndexDB
	 * - Index of the database to search in.
	 * @param[in] i_puc_UserId
	 * - ID of the user whose finger's quality must be retrieved.
	 * @param[in] i_uc_UserIDSize
	 * - UserID size.
	 * @param[in] i_ul_UserIndex
	 * - Index of the user whose finger's quality must be retrieved. This parameter is used if only i_puc_UserId is null.
	 * @param[out] o_puc_TemplateQuality
	 * - Finger template quality(ies). Each quality is returned as a byte.
	 * @param[out] o_puc_FinguerNumber
	 * - Number of fingers in the database for the specified user.
 	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL			An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER		One or more input parameters are out of range.
	 * @retval #MORPHOERR_INVALID_USER_ID	The UserID does not exist in the database.
	 * @retval #MORPHOERR_BASE_NOT_FOUND		The database does not exist.
	 * @retval #MORPHOERR_USER_NOT_FOUND		The user does not exist in the database.
	 * @retval #MORPHOERR_UNAVAILABLE		Functionality is not available for currently connected device.
     */
	I GetUserTemplateQuality(	UC	i_uc_IndexDB,
								PUC	i_puc_UserId,
								UC	i_uc_UserIDSize,
								UL	i_ul_UserIndex,
								PUC	o_puc_TemplateQuality,
								PUC	o_puc_FinguerNumber);

	/**
	 * This function specifies whether templates verification will be done or not when a new record
	 * is created (cf CUSer::DBStore method). By default, it is not allowed to add the record of a
	 * user that is already enrolled in the database.
	 *
	 * @param[in] i_b_NoCheckOnTemplate
	 * - Set this parameter to TRUE to add a record without templates verification.
	 * This option is useful to reduce the time taken to fill large databases.
	 * In this case, the database coherence must be previously checked.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I SetNoCheckOnTemplateForDBStore(	BOOL	i_b_NoCheckOnTemplate );

	/**
	 * This function sets a new value for the matching statistics for the user.
	 *
	 * @param[in]  i_uc_IndexDB
	 * - Database index.
	 * @param[in]  i_puc_UserId
	 * - ID of the user for which matching statistics must be updated.
	 * @param[in]  i_uc_UserIDSize
	 * - UserID size.
	 * @param[in]  i_ul_UserIndex
	 * - Index of the user whose matching statistics must be retrieved. This parameter is used if only i_puc_UserId is null.
	 * @param[in]  i_ul_matchingStats
	 * - New value for the user's matching statistics (must be between 0 and 0x00FFFFFF)
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_USER_NOT_FOUND	User index or PIN do not match any existing user in the database.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	Database is created without C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	Matching statistics operations are not available on current device.
	 */
	I SetStats (	UC i_uc_IndexDB,
					PUC i_puc_UserId ,
					UC i_uc_UserIDSize,
					UL i_ul_UserIndex,
					UL i_ul_matchingStats);

	/**
	 * This function retrieves the current matching statistics for the user.
	 *
	 * @param[in]  i_uc_IndexDB
	 * - Database index.
	 * @param[in]  i_puc_UserId
	 * - ID of the user for which matching statistics must be updated.
	 * @param[in]  i_uc_UserIDSize
	 * - UserID size.
	 * @param[in]  i_ul_UserIndex
	 * - Index of the user whose matching statistics must be retrieved. This parameter is used if only i_puc_UserId is null.
	 * @param[out]  o_pul_matchingStats
	 * - The current value for the user's matching statistics.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_USER_NOT_FOUND	User index or PIN do not match any existing user in the database.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	Database is created without C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	Matching statistics operations are not available on current device.
	 */
	I GetStats (	UC i_uc_IndexDB,
					PUC i_puc_UserId ,
					UC i_uc_UserIDSize,
					UL i_ul_UserIndex,
					PUL	o_pul_matchingStats);

	/**
	 * This function allows to set the OTP parameters.
	 *
	 * @param[in] i_us_OTP_Param
	 * - Value of the OTP parameters that defines the behavior of the MorphoSmart&tm; product.
	 * This parameter is managed as a bits array :
	 *    -	bit 0 : if this bit is set, local re-enrollment is allowed, else local re-enrollment
	 *              is  not allowed
	 *    -	bit 1 - bit 15 : NOT USED.
	 * @param[in] i_us_otp_sequence_number_max (16 bits) or i_u64_otp_sequence_number_max (64 bits for HOTP Algo)
	 * - Initial value of the sequence number N.
	 * Note for OTP_HASH_ALGO_SHA1 Algo: In order that the OTP generation time will not be excessive, this sequence number
	 * should not be too high. Indeed, the OTP generation time is proportional to the sequence
	 * number.
	 * @param[in] i_ul_PasswordLen
	 * - Length of the OTP password.
	 * @param[in] i_puc_otp_Password
	 * - OTP password:
	 * This password must be written in the MorphoSmart&tm; to initialize the OTP generation algorithm.
	 * @param[in] i_ul_PinLen
	 * - -1 Re-enrollment pin code length.
	 * @param[in] i_puc_otp_Pin
	 * - -1 Re-enrollment pin code.
	 * If there is no OTP reference template recorded in the MorphoSmart&tm;
	 * (i.e. at the time of the first OTP enrollment), a pin code is needed.
	 * Writing the re-enrollment pin code in the MorphoSmart&tm;, results in erasing all
	 * reference templates and allowing the enrollment of the user.
	 * @param[in] i_ul_OTPUserDataLen
	 * - OTP User Data field length.
	 * @param[in] i_puc_OTPUserData
	 * - OTP User Data field. This field is not modified if the bit OTP_VALID_USER_DATA
	 * is not set in i_us_option_mask.
	 * @param[in] i_us_option_mask
	 * - binary mask indicating the presence of valid values (see the OTP_VALID_XXX constants
	 * in the section 5.2)
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_STATUS	MSO returned an unknown status error
	 * @retval #MORPHOERR_BADPARAMETER	Invalid parameter
	 * @retval #MORPHOERR_OTP_LOCK_SET_PARAM	ILV_OTP_SET_PARAMETERS  Locked
	 * @retval #MORPHOERR_UNAVAILABLE	OTP functionalities not available for current device.
	 */
	I OTP_SetParameters (
							US			i_us_OTP_Param,						//	<opt> the OTP parameters
							US			i_us_otp_sequence_number_max,		//	<opt> the initial N value
							UL			i_ul_PasswordLen,					//	<opt> the password length
							PUC			i_puc_otp_Password,					//	<opt> the password
							UL			i_ul_PinLen,						//	<opt> the PIN length
							PUC			i_puc_otp_Pin,						//	<opt> the PIN (writing this erases the base and ref PK)
							UL			i_ul_OTPUserDataLen,				//	<opt> the OTP User Data length (0 otherwise)
							PUC			i_puc_OTPUserData,					//	<opt> the OTP User Data
							US			i_us_option_mask );					//	bitmask indicating the presence of valid values (see defines OTP_VALID_XXX)

	/**
	 * @see OTP_SetParameters
	 */
	I OTP_SetParameters (
							US			i_us_OTP_Param,						//	<opt> the OTP parameters
							T__UINT64	i_u64_otp_sequence_number_max,		//	<opt> the initial N value
							UL			i_ul_PasswordLen,					//	<opt> the password length
							PUC			i_puc_otp_Password,					//	<opt> the password
							UL			i_ul_PinLen,						//	<opt> the PIN length
							PUC			i_puc_otp_Pin,						//	<opt> the PIN (writing this erases the base and ref PK)
							UL			i_ul_OTPUserDataLen,				//	<opt> the OTP User Data length (0 otherwise)
							PUC			i_puc_OTPUserData,					//	<opt> the OTP User Data
							US			i_us_option_mask );					//	bitmask indicating the presence of valid values (see defines OTP_VALID_XXX)


	/**
	 * 	This function enrolls live fingers of the user in the OTP token in the OTP database.
	 * It also allows associating a User ID and two additional user public data fields.
	 * The user enrolment is performed the following way:
	 * -	If there is no OTP reference template record (i.e. no PK),
	 *      the user must present his pin code.
	 * -	If there is an OTP reference template record AND that the local enrolment is allowed,
	 *      the user   must present the correct fingerprint to be able to re-enroll himself later.
	 *      As a result, the reference templates are these new captured templates.
	 *
	 * @param[in] i_us_Timeout
	 * - Finger detection timeout in seconds. Its value must be between 0 and 0xFFFF.
	 * 0 corresponds to an infinite timeout.
	 * @param[in] i_ul_PinLen
	 * - Length of the re-enrollment pin code.
	 * @param[in] i_puc_Pin
	 * - Re-enrollment pin code.
	 * If there is no OTP reference template recorded in the MorphoSmart&tm;
	 * (i.e. at the time of the first OTP enrollment), a pin code is needed.
	 * Writing the re-enrollment pin code in the MorphoSmart&tm;, results in erasing all
	 * reference templates and allow the enrollment of the user.
	 * @param[in] i_uc_lenUserID
	 * - UserID length. The maximum user length is #MORPHO_USER_ID_MAXLEN bytes.
	 * @param[in] i_puc_UserID
	 * - UserID. This is the unique user identifier in the OTP database.
	 * @param[in] i_uc_lenAddField1
	 * - Additional user data public field 1 length.
	 * @param[in] i_puc_AddField1
	 * - Additional user data public field 1.
	 * @param[in] i_uc_lenAddField2
	 * - Additional user data public field 2 length.
	 * @param[in] i_puc_AddField2
	 * - Additional user data public field 2.
	 * @param[in] i_x_TemplateType
	 * - Indicates the template acquisition format. . The template format should be
	 * #MORPHO_PK_COMP. The other template formats (#MORPHO_PK_COMP_NORM, or #MORPHO_PK_MAT,
	 * or #MORPHO_PK_MAT_NORM) are reserved for compatibility with existing systems or specific usage.
	 * @param[in] i_us_MaxSizeTemplate
	 * - Defines the format of the exported minutiae.
	 * Set this value to 0 to exclude the calculated minutiae from the reply.
	 * Set this value to 1 to export the minutiae with its default size.
	 * For #MORPHO_PK_COMP only, this value can be set from 170 to 255 to compress the template.
	 * The buffer exported will have the size you have chosen at the most,  but could be less.
	 * @param[in] i_x_typEnvelop
	 * - #MORPHO_RAW_TEMPLATE means that we want to generate raw templates without signature
	 * #MORPHO_X984_SIGNED_TEMPLATE means that we want to generate templates with a digital
	 * signature and an X9.84 envelop.
	 * @param[in] i_i_ApplicationDataLen
	 * - Length of i_puc_ApplicationData buffer. Set to 0 if not used.
	 * @param[in] i_puc_ApplicationData
	 * - Application buffer that will be added to the X9.84 envelop. This data is taken
	 * into account only when i_x_typEnvelop parameter is set to #MORPHO_X984_SIGNED_TEMPLATE.
	 * NULL if not used. This parameter is used only by your application.
	 * @param[out] o_ppx_OtpUserTemplateList
	 * - Contains the calculated templates. When #MORPHO_X984_SIGNED_TEMPLATE is used,
	 * the X984_SIGNED_TEMPLATE can be obtained with C_MORPHO_TemplateList::GetX984 and
	 * the raw templates with C_MORPHO_TemplateList::GetTemplate.
	 * @param[in] i_us_option_mask
	 * - binary mask indicating the presence of valid values (see the OTP_VALID_XXX constants
	 * in the section 5.2: Constants table)
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements. This mask describes the
	 * asynchronous status events that will trig the callback function.
	 * 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want
	 * to receive the command status (move finger up...) and low-resolution images.
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of the asynchronous status. NULL if not used.
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_STATUS	MSO returned an unknown status error
	 * @retval #MORPHOERR_BADPARAMETER	Invalid parameter
	 * @retval #MORPHOERR_OTP_NOT_INITIALIZED	All OTP parameters are not initialized
	 * @retval #MORPHOERR_OTP_PIN_NEEDED	The pin code is needed for the first enrolment
	 * @retval #MORPHOERR_OTP_REENROLL_NOT_ALLOWED	The re-enrolment of the user is not allowed
	 * @retval #MORPHOERR_OTP_ENROLL_FAILED	OTP enrollment failed
	 * @retval #MORPHOERR_OTP_IDENT_FAILED	OTP identification failed
	 * @retval #MORPHOERR_OTP_LOCK_ENROLL	ILV_OTP_ENROLL_USER Locked
	 * @retval #MORPHOERR_UNAVAILABLE	OTP functionalities not available for current device.
	 */
	I OTP_EnrollUser (
		US							i_us_Timeout,						//	the operation timeout
		UL							i_ul_PinLen,						//	<opt> the PIN length
		PUC							i_puc_Pin,							//	<opt> the PIN
		UC							i_uc_lenUserID,						//	<opt> the UserID length
		PUC							i_puc_UserID,						//	<opt> the UserID
		UC							i_uc_lenAddField1,					//	<opt> the additionnal field1 length
		PUC							i_puc_AddField1,					//	<opt> the additionnal field1
		UC							i_uc_lenAddField2,					//	<opt> the additionnal field2 length
		PUC							i_puc_AddField2,					//	<opt> the additionnal field2

		T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
		US							i_us_MaxSizeTemplate,
		T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
		I							i_i_ApplicationDataLen,
		PUC							i_puc_ApplicationData,
		C_MORPHO_TemplateList **	o_ppx_OtpUserTemplateList,

		US							i_us_option_mask, 					//	bitmask indicating the presence of valid values (see defines OTP_VALID_XXX)
		UL							i_ul_CallbackCmd,					// i_ul_AsynchronousEvent
		T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
		PVOID						i_pv_CallbackArgument
		);

	/**
	 * This function allows to retrieve the OTP token status : initialization status
	 * (OTP parameter state and value), user enrolment status,
	 * User ID, Additional user data fields.
	 *
	 * @param[out] o_puc_ParamState
	 * - OTP parameters status. This is the binary mask indicating the presence of valid
	 * values filled in OTP_EnrollUser function or in the OTP_Generate function.
	 * @param[out] o_pus_OTP_Param
	 * - OTP parameters value (if filled in the OTP_SetParameters function)
	 * @param[out] o_puc_UserEnrolled
	 * - User enrollment status. If the user is enrolled, this parameter value is 0x01,
	 * else this value is 0x00.
	 * @param[out] o_puc_lenUserID
	 * - UserID length (if filled in the OTP_EnrollUser function).
	 * @param[out] o_ppuc_UserID
	 * - UserID (if filled in the OTP_EnrollUser function).
	 * @param[out] o_puc_lenAddField1
	 * - Additional user data public field 1 length (if filled in the OTP_EnrollUser function).
	 * @param[out] o_ppuc_AddField1
	 * - Additional user data public field 1 (if filled in the OTP_EnrollUser function).
	 * @param[out] o_puc_lenAddField2
	 * - Additional user data public field 2 length (if filled in the OTP_EnrollUser function).
	 * @param[out] o_ppuc_AddField2
	 * - Additional user data public field 2 (if filled in the OTP_EnrollUser function).
	 * @param[out] o_pul_lenOTPUserData
	 * - OTP User Data field length.
	 * @param[out] o_ppuc_OTPUserData
	 * - OTP User Data field (if filled in the OTP_EnrollUser function).
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_OTP_NOT_INITIALIZED	All OTP parameters are not initialized
	 * @retval #MORPHOERR_UNAVAILABLE	OTP functionalities not available for current device.
	 */
	I OTP_GetStatus (													// return values are only valid upon success of the function
		PUC					o_puc_ParamState,							//	the OTP parameters state
		PUS					o_pus_OTP_Param,							//	the OTP Parameters value
		PUC					o_puc_UserEnrolled,							//	indicates if the user is enrolled or not (0x00)
		PUC					o_puc_lenUserID,							//	if set during enroll, contains the UserID length (0 otherwise)
		PUC*				o_ppuc_UserID,								//	if set during enroll, contains the UserID
		PUC					o_puc_lenAddField1,							//	if set during enroll, contains the Additionnal field1 length (0 otherwise)
		PUC*				o_ppuc_AddField1,							//	if set during enroll, contains the Additionnal field1
		PUC					o_puc_lenAddField2,							//	if set during enroll, contains the Additionnal field2 length (0 otherwise)
		PUC*				o_ppuc_AddField2,							//	if set during enroll, contains the Additionnal field2
		PUL					o_pul_lenOTPUserData,						//	if set during enroll, contains the Additionnal field2 length (0 otherwise)
		PUC*				o_ppuc_OTPUserData
		);

	/**
	 * This function generates an OTP (One Time Password) for a fingerprint that has already
	 * been enrolled and saved in the OTP database.
	 *
	 * @param[in] i_us_Timeout
	 * - Finger detection timeout in seconds. Its value must be between 0 and 0xFFFF.
	 * 0 corresponds to an infinite timeout.
	 * @param[in] i_uc_otp_Hash_function
	 * - Hash algorithm used to generate the OTP (see the hash algorithm constants in the section 5.2).
	 * @param[in] i_us_otp_sequence_number (16 bits) or i_u64_otp_sequence_number(64 bits for HOTP Algo)
	 * - Current sequence number N used to generate the OTP. This value is provided by the remote
	 * server and it is only required in this case.
	 * @param[in] i_ul_SeedLen
	 * - Seed length.
	 * @param[in] i_puc_otp_seed
	 * - Seed used to generate the OTP.
	 * @param[in] i_us_option_mask
	 * - binary mask indicating the presence of valid values (see the OTP_VALID_XXX constants
	 * in the section 5.2: Constants table)
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements. This mask describes the
	 * asynchronous status events that will trig the callback function.
	 * 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want
	 * to receive the command status (move finger up...) and low-resolution images.
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of the asynchronous status. NULL if not used.
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 * @param[out] o_puc_lenOTP
	 * - OTP length.
	 * @param[out] o_ppuc_OTP
	 * - One Time Password (OTP).
	 * @param[out] o_puc_lenUserID
	 * - UserID length (if filled in the OTP_EnrollUser function).
	 * @param[out] o_ppuc_UserID
	 * - UserID (if filled in the OTP_EnrollUser function).
	 * @param[out] o_puc_lenAddField1
	 * - Additional user data public field 1 length (if filled in the OTP_EnrollUser function).
	 * @param[out] o_ppuc_AddField1
	 * - Additional user data public field 1 (if filled in the OTP_EnrollUser function).
	 * @param[out] o_puc_lenAddField2
	 * - Additional user data public field 2 length (if filled in the OTP_EnrollUser function).
	 * @param[out] o_ppuc_AddField2
	 * - Additional user data public field 2 (if filled in the OTP_EnrollUser function).
	 * @param[out] o_pul_lenOTPUserData
	 * - OTP User Data field length.
	 * @param[out] o_ppuc_OTPUserData
	 * - OTP User Data field (if filled in the OTP_EnrollUser function).
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_STATUS	MSO returned an unknown status error
	 * @retval #MORPHOERR_BADPARAMETER	Invalid parameter
	 * @retval #MORPHOERR_OTP_NOT_INITIALIZED	All OTP parameters are not initialized
	 * @retval #MORPHOERR_NO_MORE_OTP	No more OTP available (sequence number = 0)
	 * @retval #MORPHOERR_OTP_NO_HIT	OTP authentication or identification failed
	 * @retval #MORPHOERR_OTP_ENROLL_FAILED	OTP enrollment failed
	 * @retval #MORPHOERR_OTP_IDENT_FAILED	OTP identification failed
	 * @retval #MORPHOERR_OTP_LOCK_GEN_OTP	ILV_OTP_GENERATE Locked
	 * @retval #MORPHOERR_UNAVAILABLE	OTP functionalities not available for current device.
	 */
	I OTP_Generate (
		US							i_us_Timeout,						//	the operation timeout
		UC							i_uc_otp_Hash_function,				//	<opt> the HASH algorithm
		US							i_us_otp_sequence_number,			//	<opt> the N value
		UL							i_ul_SeedLen,						//	<opt> the Seed length
		PUC							i_puc_otp_seed,						//	<opt> the Seed
		US							i_us_option_mask, 					//	bitmask indicating the presence of valid values (see defines OTP_VALID_XXX)

		UL							i_ul_CallbackCmd,
		T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
		PVOID						i_pv_CallbackArgument,

		PUC							o_puc_lenOTP,						// the OTP length
		PUC*						o_ppuc_OTP,							// the OTP itself
		PUC							o_puc_lenUserID,					//	if set during enroll, contains the UserID length (0 otherwise)
		PUC*						o_ppuc_UserID,						//	if set during enroll, contains the UserID
		PUC							o_puc_lenAddField1,					//	if set during enroll, contains the Additionnal field1 length (0 otherwise)
		PUC*						o_ppuc_AddField1,					//	if set during enroll, contains the Additionnal field1
		PUC							o_puc_lenAddField2,					//	if set during enroll, contains the Additionnal field2 length (0 otherwise)
		PUC*						o_ppuc_AddField2,					//	if set during enroll, contains the Additionnal field2
		PUL							o_pul_lenOTPUserData,				//	if set during enroll, contains the OTP User Data length (0 otherwise)
		PUC*						o_ppuc_OTPUserData					//	if set during enroll, contains the OTP User Data
		);

	/**
	 * @see OTP_Generate
	 */
	I OTP_Generate (
		US							i_us_Timeout,						//	the operation timeout
		UC							i_uc_otp_Hash_function,				//	<opt> the HASH algorithm
		T__UINT64					i_u64_otp_sequence_number,			//	<opt> the N value
		UL							i_ul_SeedLen,						//	<opt> the Seed length
		PUC							i_puc_otp_seed,						//	<opt> the Seed
		US							i_us_option_mask, 					//	bitmask indicating the presence of valid values (see defines OTP_VALID_XXX)

		UL							i_ul_CallbackCmd,
		T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
		PVOID						i_pv_CallbackArgument,

		PUC							o_puc_lenOTP,						// the OTP length
		PUC*						o_ppuc_OTP,							// the OTP itself
		PUC							o_puc_lenUserID,					//	if set during enroll, contains the UserID length (0 otherwise)
		PUC*						o_ppuc_UserID,						//	if set during enroll, contains the UserID
		PUC							o_puc_lenAddField1,					//	if set during enroll, contains the Additionnal field1 length (0 otherwise)
		PUC*						o_ppuc_AddField1,					//	if set during enroll, contains the Additionnal field1
		PUC							o_puc_lenAddField2,					//	if set during enroll, contains the Additionnal field2 length (0 otherwise)
		PUC*						o_ppuc_AddField2,					//	if set during enroll, contains the Additionnal field2
		PUL							o_pul_lenOTPUserData,				//	if set during enroll, contains the OTP User Data length (0 otherwise)
		PUC*						o_ppuc_OTPUserData					//	if set during enroll, contains the OTP User Data
		);

	/**
	 * This function specifies the number of acquisitions per finger
	 *
	 * @param[in] i_uc_EnrollmentType
	 * Number of acquisitions per finger, possible values are:
	 * - 0x03 : Three (3) acquisitions per finger. The template is generated from a consolidation calculation of three consecutive acquisitions of the same finger.
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER		Invalid parameter
	 */
	I SetEnrollmentType(UC	i_uc_EnrollmentType);

	/**
	 * This function returns the number of acquisitions per finger.
	 *
	 */
	UC GetEnrollmentType();

private:
	I SetUserId(	UC	i_uc_lenUserID,	PUC	i_puc_UserID);
	I DeleteUserId();
	I DeleteUserOTP();
	I SetIndexUser(	UL		i_ul_IndexUser );
	I GetIndexUser(	UL &	o_ul_IndexUser );
	I Reset();

protected:
	I Copy(const C_MORPHO_User& i_px_UserSrc);

private:

	typedef struct
	{
		// OTP Members
		UC					m_uc_OTPParamState;
		US					m_us_OTPParam;
		UC					m_uc_OTPUserEnrolled;
		UC					m_auc_OTP[OTP_STRING_SIZE+1];

		UC					m_uc_lenOtpUserID;
		PUC					m_puc_OtpUserID;

		UL					m_ul_lenOtpUserData;
		PUC					m_puc_OtpUserData;

		UC					m_uc_lenAddField1;
		PUC					m_puc_AddField1;

		UC					m_uc_lenAddField2;
		PUC					m_puc_AddField2;

		C_MORPHO_TemplateList * m_px_OtpUserTemplateList;

	}T_MORPHO_OTP_DATA, *PT_MORPHO_OTP_DATA;

	typedef struct
	{
		C_MORPHO_Database *	m_px_AssociatedDatabase;
		PVOID				m_px_wrapper;

		UL					m_ul_IndexUser;

		UC					m_uc_lenUserID;
		PUC					m_puc_UserID;

		UC					m_uc_lenFilterData;
		PUC					m_puc_FilterData;

		BOOL				m_b_NoCheckOnTemplate;

		PT_MORPHO_OTP_DATA	m_px_OtpData;

		UL					m_ul_pkUpdateMask;

		UC					m_uc_EnrollmentType;

		UC					m_uc_StrategyAcquisitionMode;
	} T_MORPHO_USER,*PT_MORPHO_USER;

	PT_MORPHO_USER m_px_MorphoUser;

	UL	m_ul_magicEnd;
};

#endif	// MORPHO_USER_H
