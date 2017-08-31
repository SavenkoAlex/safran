// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_FieldList.h
 * @brief Definitions of the C_MORPHO_FieldList class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_FIELD_LIST_H
#define MORPHO_FIELD_LIST_H

#ifndef SWIG
#ifdef MORPHO_SDK
#define MORPHO_API __declspec(dllexport)
#else
#define MORPHO_API __declspec(dllimport)
#endif
#endif

/**
 * @brief Class containing the list of fields associated to a user.
 * This class contains the list of fields associated to a user.
 * C_MORPHO_User inherits from this class.
 */
class MORPHO_API C_MORPHO_FieldList
{
private:
	UL	m_ul_magicFirst;
public:
	friend class C_MORPHO_Database;
	friend class C_MORPHO_User;


	/**
	 * Default constructor
	 */
	C_MORPHO_FieldList  ();

	/**
	 * Copy constructor
	 * @param [in] i_px_FieldList source object
	 */
	C_MORPHO_FieldList(const C_MORPHO_FieldList& i_px_FieldList);

	/**
	 * Default destructor
	 */
	virtual ~C_MORPHO_FieldList ();

	/**
	 * Assignation operator
	 * @param [in] i_px_FieldList source object
	 */
	C_MORPHO_FieldList& operator= (const C_MORPHO_FieldList& i_px_FieldList);

	/**
	 * Add a field to the list
	 *
	 * @param[in] i_ul_indexField
	 * - Index field. Index can not be 0 because this value is reserved for UserID.
	 * First available index is 1.
	 * @param[in] i_ul_lenField
	 * - length of data to store in the field. When fields are stored in a database record,
	 * only useful length is used, so choosing a lower length than the maximum size
	 * (defined during base creation) will save some space in the database.
	 * @param[in] i_puc_dataField
	 * - Data stored in the field.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER  One or more input parameters are out of range.
	 */
	I PutField(	UL		i_ul_indexField,
				UL		i_ul_lenField,
				PUC		i_puc_dataField);

	/**
	 * Retrieves a field structure definition from the list.
	 *
	 * @param[in] i_ul_indexField
	 * - Field index (index can not be 0 because it is reserved for UserID, first available is 1).
	 * @param[out] o_ul_lenField
	 * - Data length stored in the field.
	 * @param[out] o_puc_dataField
	 * - Data stored in the field. This parameter must not be allocated, released or modified.
	 * Before performing such tasks, the application should make a copy of this data.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_FIELD_NOT_FOUND	Field not found.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetField(	UL		i_ul_indexField,
				UL	&	o_ul_lenField,
				PUC	&	o_puc_dataField);

protected:
	I DeleteField (UL		i_ul_indexField);
	I GetNbField(	UL	&	o_ul_nbField);
	I Copy( C_MORPHO_FieldList& io_px_FieldListDest, const C_MORPHO_FieldList& i_px_FieldListSrc );

private:
	I Reset();

private:
	typedef struct _T_MORPHO_FIELD_LIST
	{
		_T_MORPHO_FIELD_LIST		*m_px_next;
		UL							m_ul_indexField;
		UL							m_ul_lenField;
		PUC							m_puc_dataField;
	} T_MORPHO_FIELD_LIST;

	typedef struct
	{
		UL								m_ul_nbField;
		T_MORPHO_FIELD_LIST				*m_px_FieldList;

	} T_MORPHO_FIELDLIST,*PT_MORPHO_FIELDLIST;

	PT_MORPHO_FIELDLIST	m_px_MorphoFieldList;

	UL	m_ul_magicEnd;
};

#endif // MORPHO_FIELD_LIST_H
