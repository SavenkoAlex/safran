// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_FieldDescriptor.h
 * @brief Definitions of the C_MORPHO_FieldDescriptor class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_FIELD_DESCRIPTOR_H
#define MORPHO_FIELD_DESCRIPTOR_H

#ifndef SWIG
#ifdef MORPHO_SDK
#define MORPHO_API __declspec(dllexport)
#else
#define MORPHO_API __declspec(dllimport)
#endif
#endif

// types

/**
 * @brief Class used to describe the field structure of the internal database.
 * This class describes the field structure of the internal database.
 * This class is built as a list of field descriptors. This class is an abstract class
 * that should not be instantiated. C_MORPHO_Database inherits from this class.
 */
class MORPHO_API C_MORPHO_FieldDescriptor
{
private:
	UL	m_ul_magicFirst;
public:
	friend class C_MORPHO_Database;

	/** @brief Size of a field name in the local database */
	#define MORPHO_FIELD_NAME_LEN			6		// Size of a field name in the local database

	/**
	 * Default constructor
	 */
	C_MORPHO_FieldDescriptor();

	/**
	 * Copy constructor
	 * @param[in] i_px_FieldDescSrc source Object
	 */
	C_MORPHO_FieldDescriptor(const C_MORPHO_FieldDescriptor& i_px_FieldDescSrc);

	/**
	 * Default destructor
	 */
	virtual ~C_MORPHO_FieldDescriptor();

	/**
	 * Assignation operator
	 * @param[in] i_px_FieldDescSrc source Object
	 */
	C_MORPHO_FieldDescriptor& operator= (const C_MORPHO_FieldDescriptor& i_px_FieldDescSrc);

	/**
	 * Add a field structure definition to the list.
	 *
	 * @param[in] i_uc_fieldAttribute
	 * - Field attribute.
	 * @param[in] i_us_fieldMaxSize
	 * - Field maximum size.
	 * @param[in] i_puc_fieldName
	 * - Field friendly name.
	 * @param[out] o_ul_fieldIndex
	 * - Field index.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I PutField(	T_MORPHO_FIELD_ATTRIBUTE	i_uc_fieldAttribute,
				US							i_us_fieldMaxSize,
				UC							i_puc_fieldName[MORPHO_FIELD_NAME_LEN],
				UL &						o_ul_fieldIndex);

	/**
	 * Retrieves the number of field structure definitions stored in
	 * C_MORPHO_FieldDescriptor instance.
	 *
	 * @retval Number of field structure definitions stored in the list.
	 */
	UL	GetNbField( VOID );

	/**
	 * Retrieves a field structure definition from the list.
	 *
	 * @param[in] i_ul_fieldIndex
	 * - Field index. First supplementary field is 1 (0 is reserved for UserID is not allowed).
	 * @param[out] o_uc_fieldAttribute
	 * - Field attribute.
	 * @param[out] o_us_fieldMaxSize
	 * - Field maximum size.
	 * @param[out] o_puc_fieldName
	 * - Field friendly name.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	Invalid field index (0 or higher than field number).
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetField(	UL							i_ul_fieldIndex,
				T_MORPHO_FIELD_ATTRIBUTE &	o_uc_fieldAttribute,
				US	&						o_us_fieldMaxSize,
				UC							o_puc_fieldName[MORPHO_FIELD_NAME_LEN]);

protected:
	I PurgeAllFields( VOID );

private:
	I Copy( C_MORPHO_FieldDescriptor& io_px_FieldDescDest, const C_MORPHO_FieldDescriptor& i_px_FieldDescSrc );

private:
	typedef struct _T_MORPHO_FIELD_LIST {
		_T_MORPHO_FIELD_LIST		*m_px_next;
		UL							m_ul_indexField;
		T_MORPHO_FIELD_ATTRIBUTE	m_uc_fieldAttribute;
		US							m_us_fieldMaxSize;
		UC							m_puc_fieldName[MORPHO_FIELD_NAME_LEN];
	} T_MORPHO_FIELD_LIST;

	typedef struct
	{
		UL								m_ul_nbField;
		T_MORPHO_FIELD_LIST				*m_px_FieldList;

	} T_MORPHO_FIELDDESCRIPTOR,*PT_MORPHO_FIELDDESCRIPTOR;

	PT_MORPHO_FIELDDESCRIPTOR	m_px_MorphoFieldDescriptor;

	UL	m_ul_magicEnd;
};

#endif // MORPHO_FIELD_DESCRIPTOR_H
