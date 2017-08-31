// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_Database.h
 * @brief Definition of the C_MORPHO_Database class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_DATABASE_H
#define MORPHO_DATABASE_H

#ifndef SWIG
#ifdef MORPHO_SDK
#define MORPHO_API __declspec(dllexport)
#else
#define MORPHO_API __declspec(dllimport)
#endif
#endif

#ifdef MORPHO_SDK_EXT
#include"MORPHO_Types_Ext.h"
#endif

class C_MORPHO_Device;

/**
 * @brief Class performing operations on the embedded database
 * This class performs operations related to the biometric device local database.
 * As a database is always associated to a device, C_MORPHO_Database has to be instantiated
 * with C_MORPHO_Device::GetDatabase().<br>
 * Fields are described thanks to inherited C_MORPHO_FieldDescriptor class.
 */
class MORPHO_API C_MORPHO_Database: public C_MORPHO_FieldDescriptor
{
private:
	UL	m_ul_magicFirst;
public:
	friend class C_MORPHO_Device;
	friend class C_MORPHO_User;

	// constants

	/** @brief Minimum number of records in the database */
	#define MORPHO_NB_RECORD_MIN			1							// Minimum number of records in the database
	/** @brief Maximum number of fields in a record. For a MorphoSmart&tm; CBM it is 16 */
	#define	MORPHO_NB_FIELD_MAX				20							// Maximum field number in a record
	/** @brief Number of bytes necessary to address all field numbers */
	#define	MORPHO_NB_FIELD_MAX_BIT			(1+MORPHO_NB_FIELD_MAX/8)	// Number of bytes necessary to address all field numbers
	/** @brief Maximum field length */
	#define MORPHO_LEN_FIELD_MAX			128							// Maximum field lenght
	/** @brief Maximum number of fingers enrolled per user */
	#define MORPHO_FINGER_MAX				2							// Maximum number of enrolled fingers by user
	/** @brief Minimum number of fingers enrolled per user */
	#define MORPHO_FINGER_MIN				1							// Minimum number of enrolled fingers by user

	/** @brief Identify the type of database deletion */
	typedef enum {
		MORPHO_ERASE_BASE,		/**< This option erases all records in a local database */
		MORPHO_DESTROY_BASE		/**< This function deletes the local database */
	} T_MORPHO_TYPE_DELETION;

	/**
	 * Default constructor
	 */
	C_MORPHO_Database ();

	/**
	 * Copy constructor
	 * @param[in] i_px_MorphoDatabase source Object
	 */
	C_MORPHO_Database(const C_MORPHO_Database& i_px_MorphoDatabase);

	/**
	 * Default destructor
	 */
	virtual ~C_MORPHO_Database ();

	/**
	 * Assignation operator
	 * @param[in] i_px_MorphoDatabase source Object
	 */
	C_MORPHO_Database& operator= (const C_MORPHO_Database& i_px_MorphoDatabase);

	/**
	 * This function creates a biometric database in flash memory. Then, the database internal
	 * structure has to be defined (see @ref C_MORPHO_FieldDescriptor for more details). Once
	 * a database is created, its internal structure can not be modified.
	 *
	 * @cond MORPHO_IMPLEMENTATION_NOTE
	 * @note - @ref IMPLEMENTATION_NOTE_DBCREATE
	 * @endcond
	 *
	 * @param[in]  i_ul_NbRecord
	 * - Number of records reserved in the database (i.e. maximum number of persons enrolled).
	 * @param[in]  i_uc_NbFinger
	 * - Maximum number of fingerprints which can be stored in each record of the database (1, 2, 10 or 20).
	 *   At record generation, the terminal allows the Host System to save less fingerprints per user than
	 *   specified in the Create Database command (i.e. 1 in case of 2 fingerprints maximum per record).<br>
	 *   In case of MorphoSmart&tm; FINGER VP, the maximum number of fingerprints per record cannot be more than two.
	 * - Only for MorphoSmart&tm; MSO: Two other combinations of the i_ul_NbRecord and i_uc_NbFinger parameters are also possible:
	 *     - If i_ul_NbRecord value is set to 100, i_uc_NbFinger value must be strictly set to 10,
	 *     - If i_ul_NbRecord value is set to 50, i_uc_NbFinger value must be strictly set to 20.
	 * @param[in]  i_x_TemplateType
	 * - Determines whether the database shall be normalized. Allowed values are:
	 *     - #MORPHO_PK_COMP: default database format
	 *     - #MORPHO_PK_COMP_NORM: normalized database format
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_ALREADY_EXISTS 	This database already exists.
	 * @retval #MORPHOERR_MEMORY_DEVICE	Not enough memory for the creation of a database in the MSO.
	 * @retval #MORPHOERR_BADPARAMETER
	 * -	Wrong number of fingers.
	 * -	No UserID found in array i_px_FieldDescriptorArray.
	 * -	Database path is not set or invalid, call @ref SetPath before calling this method, and specify an existing folder with valid read/write access rights.
	 * @retval #MORPHOERR_OUT_OF_FIELD	The number of additional fields is greater than #MORPHO_NB_FIELD_MAX.
	 * @retval #MORPHOERR_FIELD_INVALID	Additional field name length is greater than #MORPHO_FIELD_NAME_LEN
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_INVALID_PK_FORMAT	Incorrect value for i_x_TemplateType.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I DbCreate(	UL							i_ul_NbRecord,
				UC							i_uc_NbFinger,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType);


	/**
	 * This function creates a biometric encrypted or unencrypted database in flash memory. Then, the database internal
	 * structure has to be defined (see @ref C_MORPHO_FieldDescriptor for more details). Once
	 * a database is created, its internal structure can not be modified.
	 *
	 * @param[in]  i_ul_NbRecord
	 * - Number of records reserved in the database (i.e. maximum number of persons enrolled).
	 * @param[in]  i_uc_NbFinger
	 * - Maximum number of fingerprints which can be stored in each record of the database (1, 2, 10 or 20).
	 *   At record generation, the terminal allows the Host System to save less fingerprints per user than
	 *   specified in the Create Database command (i.e. 1 in case of 2 fingerprints maximum per record).<br>
	 *   In case of MorphoSmart&tm; FINGER VP, the maximum number of fingerprints per record cannot be more than two.
	 * - Only for MorphoSmart&tm; MSO: Two other combinations of the i_ul_NbRecord and i_uc_NbFinger parameters are also possible:
	 *     - If i_ul_NbRecord value is set to 100, i_uc_NbFinger value must be strictly set to 10,
	 *     - If i_ul_NbRecord value is set to 50, i_uc_NbFinger value must be strictly set to 20.
	 * @param[in]  i_x_TemplateType
	 * - Determines whether the database shall be normalized. Allowed values are:
	 *     - #MORPHO_PK_COMP: default database format
	 *     - #MORPHO_PK_COMP_NORM: normalized database format
	 *
 	 * @param[in]  i_i_DatabaseIndex
	 * - Database index
	 *
 	 * @param[in]  o_uc_encryptDB
	 * - It takes only two values:
	 *		- 0 : unencrypted database
	 *		- 1 : encrypted database
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_ALREADY_EXISTS 	This database already exists.
	 * @retval #MORPHOERR_MEMORY_DEVICE	Not enough memory for the creation of a database in the MSO.
	 * @retval #MORPHOERR_BADPARAMETER
	 * -	Wrong number of fingers.
	 * -	No UserID found in array i_px_FieldDescriptorArray.
	 * -	Database path is not set or invalid, call @ref SetPath before calling this method, and specify an existing folder with valid read/write access rights.
	 * @retval #MORPHOERR_OUT_OF_FIELD		The number of additional fields is greater than #MORPHO_NB_FIELD_MAX.
	 * @retval #MORPHOERR_FIELD_INVALID		Additional field name length is greater than #MORPHO_FIELD_NAME_LEN
	 * @retval #MORPHOERR_MEMORY_PC	Not		enough memory on the PC.
	 * @retval #MORPHOERR_PROTOCOLE			Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_INVALID_PK_FORMAT	Incorrect value for i_x_TemplateType.
	 * @retval #MORPHOERR_UNAVAILABLE		The associated device object has not successfully opened a connection.
	 */

	I DbCreate(	UL							i_ul_NbRecord,
				UC							i_uc_NbFinger,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				UC							i_i_DatabaseIndex,
				UC							i_uc_encryptDB = 0);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I SetPath(	PUC				i_puc_dbPath);

	/**
	 * This function erases all records in the local database or deletes the local database.
	 *
	 * @param[in]  i_uc_DeletionType:
	 * - #MORPHO_ERASE_BASE: This option erases all records in a local database without
	 * deleting the database.
	 * - #MORPHO_DESTROY_BASE: This option deletes the local database.
	 * - Example
	 *      - I	l_i_Ret ;
	 *      - I	l_i_Ret=Database.DbDelete(#MORPHO_ERASE_BASE);
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I DbDelete(	T_MORPHO_TYPE_DELETION		i_uc_DeletionType );

	/**
	 * This function returns the number of registered records in the local database.
	 *
	 * @param[out]  o_ul_nbUsedRecord
	 * - number of assigned records in the local database.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetNbUsedRecord(	UL &	o_ul_nbUsedRecord );

	/**
	 * This function returns the number of free records in the local database.
	 *
	 * @param[out]  o_ul_nbFreeRecord
	 * - number of free records in the local database.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetNbFreeRecord(	UL &	o_ul_nbFreeRecord );

	/**
	 * This function returns the total number of records reserved in the local database.
	 *
	 * @param[out]  o_ul_nbTotalRecord
	 * - total number of records reserved in the local database.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetNbTotalRecord(	UL &	o_ul_nbTotalRecord );

	/**
	 * This function returns the maximum number of templates per person in the database.
	 *
	 * @param[out]  o_uc_NbFinger
	 * - Maximum number of templates per person in the database (from 1 to #MORPHO_FINGER_MAX).
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetNbFinger(UC	& o_uc_NbFinger);

	/**
	 * This function returns the database status. It tells whether the database is encrypted or not.
	 *
	 * @param[out]  o_uc_encryptDB
	 * - It takes only two values:
	 *		- 0 : unencrypted database
	 *		- 1 : encrypted database
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetDbEncryptionStatus(UC	&o_uc_encryptDB);

	/**
	 * @todo Not Documented
	 */
	I GetFormatPK(T_MORPHO_TYPE_TEMPLATE	& o_x_TemplateType);

	/**
	 *	This function retrieves the number of additional user data fields in the database, excluding the mandatory UID field.
	 *
	 * @param[out]  o_ul_NbField
	 * - Number of additional fields in the database. 0 means only UID is present in the database structure.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetNbField( UL	& o_ul_NbField );

	/**
	 * This method overloads the one inherited from C_MORPHO_FieldDescriptor.<br>
	 * This function returns one-field characteristics.
	 *
	 * @param[in]  i_ul_fieldIndex
	 * - Field index to retrieve characteristics of. 0 is used for the UserID. First supplementary field index is 1.
	 * @param[out]  o_uc_fieldAttribute
	 * - Field attribute (#MORPHO_PUBLIC_FIELD, #MORPHO_PRIVATE_FIELD).
	 * @param[out]  o_us_fieldMaxSize
	 * - Field maximum size.
	 * @param[out]  o_puc_fieldName
	 * - Field name.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_FIELD_NOT_FOUND	Field not found.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetField(	UL							i_ul_fieldIndex,
				T_MORPHO_FIELD_ATTRIBUTE &	o_uc_fieldAttribute,
				US	&						o_us_fieldMaxSize,
				UC							o_puc_fieldName[MORPHO_FIELD_NAME_LEN]);

	/**
	 * This function fills ReadPublicFields() bit array.
	 *
	 * @param[in]  i_b_Erase
	 * - TRUE (value 1) if io_auc_FieldIndexDescriptor needs to be erased.
	 * Set this parameter to TRUE (value 1) for the first call, and then
	 * to FALSE (value 0) for the next ones.
	 * @param[in]  i_i_index
	 * - Field index that should be read (0 = UserID, 1 = first optional field...).
	 * @param[in,out]  io_auc_FieldIndexDescriptor
	 * - Bit array that represents fields that must be retrieved with ReadPublicFields().
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	l_i_index is higher than the maximum number of fields.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I FillIndexDescriptor(	BOOL	i_b_Erase,
							UC		i_i_index,
							UC		io_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX_BIT]);	// binary mask with all desired fields

	/**
	 * This function retrieves a list of public fields from all database records.
	 *
	 * @param[in]  i_auc_FieldIndexDescriptor
	 * - Bit array that represents fields that must be retrieved. Bit 0 is reserved for
	 * the UserID (unique identifier) field.
	 * - Example: 10100000 00000001 00000000 00000000 designates field #0 (UserID),
	 * field #2 and field #15. To avoid errors, it is recommended to fill this parameter
	 * with FillIndexDescriptor() method.
	 * @param[out]  o_x_UserList
	 * - Users list. Field content is retrieved by GetField() method (see C_MORPHO_FieldList).
	 *
	 * @retval #MORPHO_OK						The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL				An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND			There is no Database corresponding to the Identifier specified in the Request.
	 * @retval #MORPHOERR_PROTOCOLE				Communication protocol error.
	 * @retval #MORPHOERR_FIELD_INVALID			One or more desired fields are not found.
	 * @retval #MORPHOERR_USER_NOT_FOUND			User does not exist.
	 * @retval #MORPHOERR_CORRUPTED_CLASS		Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER			One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE			The associated device object has not successfully opened a connection.
	 */
	I ReadPublicFields (	UC					i_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX_BIT],	// binary mask with all desired fields
						C_MORPHO_UserList &	o_x_UserList);

	/**
	 * This function retrieves the first user instance that matches a searched buffer in a specified
	 * field. Private fields can not be retrieved. If multiple instances are present,
	 * the first one is retrieved. This method is successful if one user field exactly matches
	 * the searched pattern (same size, same content).
	 *
	 * @param[in]  i_ul_FieldIndex
	 * - The search is performed with this field index (0 means UserID, 1 means first supplementary field ...).
	 * @param[in]  i_ul_lenDataToFind
	 * - Length of the pattern to search.
	 * @param[in]  i_puc_DataToFind
	 * - Pattern to search.
	 * @param[out]  o_x_User
	 * - First user found with a matching field.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_FIELD_INVALID	Requested field for the search does not exist or does not have proper access rights.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	There is no Database corresponding to the Identifier specified in the Request.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_USER_NOT_FOUND	No record contains the searched data.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I DbQueryFirst(	UL					i_ul_FieldIndex,
					UL					i_ul_lenDataToFind,
					PUC					i_puc_DataToFind,
					C_MORPHO_User &		o_x_User);

	/**
	 * This function must be called after DbQueryFirst(). This function retrieves the next user
	 * instance that matches the searched buffer. This method is successful if one user field
	 * exactly matches the searched pattern (same size, same content).
	 *
	 * @param[out]  o_x_User:  Next user found with a matching field.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	There is no Database corresponding to the Identifier specified in the Request.
	 * @retval #MORPHOERR_BADPARAMETER	C_MORPHO_Database::DbQueryFirst has not previously been called.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_USER_NOT_FOUND	No record contains the searched data.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_NOCALLTO_DBQUERRYFIRST	You have to call C_MORPHO_Database::DbQueryFirst to initialize the querry
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I DbQueryNext(	C_MORPHO_User &		o_x_User);

#ifdef MORPHO_SDK_EXT
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User,
				PUL							o_pul_MatchingScore,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UL							i_ul_ModeChoice,
				PT_EXPORT_IDENT_TIMERS		o_px_IdentTimers,
				PUC							o_puc_Acq_Instrumentations
				);
#endif

	/**
	 * This function identifies a live finger against the local database, and returns
	 * the associated user.
	 *
	 * @cond MORPHO_IMPLEMENTATION_NOTE
	 * @note - @ref IMPLEMENTATION_NOTE_IDENTIFY
	 * @endcond
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
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements. This mask describes the asynchronous
	 * status events that will trig the callback function.
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
	 * @param[out] o_x_User
	 * - C_MORPHO_User instance of the user retrieved. Field contents are read with GetField() method.
	 *
	 * @param[out] o_pul_MatchingScore
	 * - Contains the result matching score. NULL if not used.<br>
	 * For security reason, the secure MorphoSmart&tm; cannot export the matching score
	 * because a "rogue" application can mount an "hillclimbing" attack by sequentially
	 * randomly modifying a sample and retaining only the changes that produce an increase
	 * in the returned score.
	 *
	 * @param[out] o_puc_FingerIndex
	 * - Must be NULL. (Reserved for future use).
	 *
	 * @param[in] i_i_CoderChoice
	 * - Contains the biometric coder to use (#MORPHO_MSO_V9_CODER or #MORPHO_MSO_V9_JUV_CODER).
	 * Morpho recommends using #MORPHO_MSO_V9_CODER.<br>
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
	 * @param[in] i_i_NbFingersToMatch
	 * - Number of templates to use for each user in the database for matching against the acquisition. Using a value
	 * lower than the actual number of templates stored in the database will reduce processing time, but may yield
	 * @ref #MORPHOERR_NO_HIT if the user does not use one of the preferred finger for the acquisition. In this case,
	 * this method shall be called again using a higher value, usually the maximum number of fingers in the database.
	 *
	 * @note In parameter i_ul_DetectModeChoice:
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_ENROLL_DETECT_MODE cannot be used together.
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE cannot be used together.
	 *
	 * @retval #MORPHO_OK									The matching was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE				C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 							An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER						The matching threshold value or timeout value are out of the range.
	 * @retval #MORPHOERR_TIMEOUT							The finger detection timeout has expired.
	 * @retval #MORPHOERR_BASE_NOT_FOUND						The specified database doesn't exist.
	 * @retval #MORPHOERR_NO_HIT								The function returned a No Hit.
	 * @retval #MORPHOERR_CMDE_ABORTED						Command is canceled.
	 * @retval #MORPHOERR_PROTOCOLE							Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS					Class has been corrupted.
	 * @retval #MORPHOERR_FFD								False Finger Detected.
	 * @retval #MORPHOERR_MOIST_FINGER						The finger can be too moist or the scanner is wet.
	 * @retval #MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN	Finger was misplaced or has been withdrawn from sensor during acquisition (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_FFD_FINGER_MISPLACED				Finger was misplaced during acquisition (MorphoSmart&tm; MSO 3x1 FFD only).
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_FingerIndex,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UL							i_ul_MatchingStrategy,
				I							i_i_NbFingersToMatch
				);

	/**
	 * @see Identify (kept for compatibility with MORPHO_SDK.dll Version 4.3.1.0)
	 */
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_FingerIndex,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UL							i_ul_MatchingStrategy
				);

	/**
	 * @see Identify (kept for compatibility with MORPHO_SDK.dll Version 4.3.1.0)
	 */
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User,
				PUL							o_pul_MatchingScore,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UL							i_ul_MatchingStrategy
				);

	/**
	 * @see Identify (kept for compatibility with MORPHO_SDK.dll Version 4.3.1.0)
	 */
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User,
				PUL							o_pul_MatchingScore,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice
				);

	/**
	 * @see Identify (kept for compatibility with MORPHO_SDK.dll Version 4.3.0.0)
	 */
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User,
				PUL							o_pul_MatchingScore,
				I							i_i_CoderChoice
				);

	/**
	 * @see Identify (kept for compatibility)
	 */
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User,
				PUL							o_pul_MatchingScore
				);

	/**
	 * @see Identify (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I Identify (
				US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_User &				o_x_User);

	/**
	 * This function identifies one unique template against the local database.<br>
	 * This function is successful if one of the searched templates matches a reference
	 * template in the local database.<br>
	 * The maximum number of search template is 1.
	 *
	 * @param[in]  i_us_FAR
	 * - This parameter specifies how tight the matching threshold is.
	 * Morpho recommends #MORPHO_FAR_5 (see paragraph #T_MORPHO_FAR).
	 * @param[in]  i_x_TemplateList
	 * - Candidate template list. This list must include one unique template.<br>
	 * The template format should be #MORPHO_PK_COMP or #MORPHO_PK_FVP_MATCH (only available on MorphoSmart&tm; FINGER VP).<br>
	 * The other template formats:
	 *     - #MORPHO_PK_COMP_NORM
	 *     - #MORPHO_PK_MAT
	 *     - #MORPHO_PK_MAT_NORM
	 *     - #MORPHO_PK_ANSI_378
	 *     - #MORPHO_PK_ANSI_378_2009
	 *     - #MORPHO_PK_MINEX_A
	 *     - #MORPHO_PK_ISO_FMR
	 *     - #MORPHO_PK_ISO_FMR_2011
	 *     - #MORPHO_PK_ISO_FMC_NS
	 *     - #MORPHO_PK_ISO_FMC_CS
	 *     - #MORPHO_PK_PKLITE (only available on MorphoSmart&tm; CBM-E3)<br>
	 * are also supported, but Morpho recommends using them only for compatibility with existing systems or specific usage.<br>
	 * Only secure MorphoSmart&tm; accept the X984 biometric token and verify the integrity.
	 * @param[out]  o_x_User
	 * - C_MORPHO_User instance of the user retrieved. Field contents are read by GetField() method.
	 * @param[out]  o_pul_MatchingScore
	 * - Contains the result matching score. NULL if not used.
	 * @param[out]	o_puc_FingerIndex
	 * - Index of the matched finger. NULL if not used. Will be set to 255 if unsupported by the
	 * current device.<br>
	 * For security reason, the secure MorphoSmart&tm; can not export the matching score
	 * because a "rogue" application can mount an "hillclimbing" attack by sequentially
	 * randomly modifying a sample and retaining only the changes that produce an increase
	 * in the returned score.
	 *
	 * @retval #MORPHO_OK	The matching was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_INTERNAL 	An internal error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	The matching threshold value or timeout value are out of range.
	 * @retval #MORPHOERR_INVALID_TEMPLATE	The reference minutiae are not valid (corrupted minutiae).
	 * @retval #MORPHOERR_BASE_NOT_FOUND	The specified database doesn't exist.
	 * @retval #MORPHOERR_NO_HIT	The function returned a No Hit.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_LICENSE_MISSING	A required license is missing (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_INVALID_PK_FORMAT	Invalid template format
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection, or current device does not
	 * 									allow identification of template files.
	 */
	I IdentifyMatch (	T_MORPHO_FAR			i_us_FAR,
					C_MORPHO_TemplateList &	i_x_TemplateList,
					C_MORPHO_User &			o_x_User,
					PUL						o_pul_MatchingScore,
					PUC						o_puc_FingerIndex);

	/**
	 * @see IdentifyMatch (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I IdentifyMatch (	T_MORPHO_FAR			i_us_FAR,
					C_MORPHO_TemplateList &	i_x_TemplateList,
					C_MORPHO_User &			o_x_User,
					PUL						o_pul_MatchingScore);

	/**
	 * @see IdentifyMatch (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I IdentifyMatch (	T_MORPHO_FAR			i_us_FAR,
					C_MORPHO_TemplateList &	i_x_TemplateList,
					C_MORPHO_User &			o_x_User);

	/**
	 * This function cancels the live acquisition.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BASE_NOT_FOUND  The specified database doesn't exist.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I CancelLiveAcquisition ( VOID );

	/**
	 * This function returns a user instance.
	 *
	 * @param[in]  i_uc_lenUserID
	 * - UserID length. The maximum user length is #MORPHO_USER_ID_MAXLEN bytes.
	 * @param[in]  i_puc_UserID
	 * - UserID (unique identifier).
	 * @param[out]  o_x_User
	 * - User instance.  NULL if not used. See C_MORPHO_User class.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetUser (	UC				i_uc_lenUserID,
				PUC				i_puc_UserID,
				C_MORPHO_User & o_x_User );

	/**
	 * This function retreives the Maximum number of records per database allowed by the configuration.
	 * @param[out] o_pus_MaxUser
	 * - return the Maximum number of records per database.
	 * @param[out] o_pus_MaxFinger
	 * - return the Maximum number of fingers that can be enrolled per user (#MORPHO_FINGER_MIN to #MORPHO_FINGER_MAX)
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetMaxUser(PUL o_pul_MaxUser, PUL o_pul_MaxFinger);

	/**
	 * @see GetMaxUser (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I GetMaxUser(PUL o_pul_MaxUser);

	/**
	 * This function retreives Maximum number of database allowed by the configuration.
	 * @param[out] o_pus_MaxDataBase
	 * - return the Maximum number of database.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_NO_ASSOCIATED_DEVICE	C_MORPHO_Database object has not been initialized by calling @ref C_MORPHO_Device::GetDatabase.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	The associated device object has not successfully opened a connection.
	 */
	I GetMaxDataBase(PUS o_pus_MaxDataBase);

	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I ResizeDb (	UL				i_ul_newSize );

	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I SetMFUSize (	UL				i_ul_MFUSize );

	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetMFUSize (	PUL				o_ul_MFUSize );

	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I AutoupdateMFUSize (	VOID );

	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I DeactivateMFUGroup (	VOID );

	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I SynchronizeStats (	VOID );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I SetFilter (	UC						i_uc_filterID,
					T_MORPHO_FILTER_TYPE	i_x_filterType,
					UL						i_ul_filterDataLength,
					PVOID					i_pv_filterData );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetFilter (	UC &					o_uc_filterID,
					T_MORPHO_FILTER_TYPE &	o_x_filterType,
					UL & 					o_ul_filterDataLength,
					PVOID &					o_pv_filterData );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetFilterDataList (	UL				i_ul_filterFieldIndex,
							PUL				o_pul_nbUsers,
							PUL*			o_ppul_filterDataLength,
							PUC**			o_pppuc_filterDataList);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I ReleaseFilterDataList (	UL				i_ul_nbUsers,
								PUL*			io_ppul_filterDataLength,
								PUC**			io_pppuc_filterDataList);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetUserPin (	UL				i_ul_userIndex,
					PUL				o_pul_userPinLength,
					PUC				*o_ppuc_userPin);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I ReleaseUserPin (	PUC				*io_ppuc_userPin);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetUserPinList (	PUL				o_pul_nbUsers,
						PUL				&o_pul_userPinLength,
						PUC*			&o_ppuc_userPinList);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I ReleaseUserPinList (	UL				i_ul_nbUsers,
							PUL				&io_pul_userPinLength,
							PUC*			&io_ppuc_userPinList);

protected:
	I GetIndex(UC	&	o_uc_databaseIndex);
	I Copy( C_MORPHO_Database& io_px_MorphoDatabaseDest, const C_MORPHO_Database& i_px_MorphoDatabaseSrc );
private:
	I GetBaseConfig(VOID);
	I GetBaseConfig(UC i_uc_requestEncyrptionStatus);
	I FindUser(	UL					i_ul_fieldIndex,
				UL					i_ul_lenDataToFind,
				PUC					i_puc_DataToFind,
				UL &				io_ul_userIndexOffset,
				C_MORPHO_User &		o_x_User);
	I Reset();
	I Erase();
	I Destroy();
private:

	typedef struct
	{
		UL						m_ul_nbUsedRecord;
		UL						m_ul_nbFreeRecord;

		UL						m_ul_FindIndexUser;
		UL						m_ul_FindFieldIndex;
		UL						m_ul_lenDataToFind;

		T_MORPHO_TYPE_TEMPLATE	m_x_TemplateType;

		PUC						m_puc_DataToFind;

		PUC						m_puc_databasePath;
		PVOID					m_px_wrapper;
		I						m_i_connectedDevice;

		PVOID					m_px_filterData;
		UL						m_ul_filterDataLength;

		C_MORPHO_Device *		m_px_AssociatedDevice;
		UC						m_uc_DatabaseIndex;
		UC						m_uc_DeviceType;

		UC						m_uc_NbFinger;
		UC						m_uc_encryptedDB;
		UC						m_uc_EmbeddedBaseSynchro;
	} T_MORPHO_DATABASE,*PT_MORPHO_DATABASE;

	PT_MORPHO_DATABASE	m_px_MorphoDatabase;

	UL	m_ul_magicEnd;
};

#endif // MORPHO_DATABASE_H
