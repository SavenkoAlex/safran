// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_UserList.h
 * @brief Definition of the C_MORPHO_UserList class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_USERLIST_H
#define MORPHO_USERLIST_H

#ifndef SWIG
#ifdef MORPHO_SDK
#define MORPHO_API __declspec(dllexport)
#else
#define MORPHO_API __declspec(dllimport)
#endif
#endif

/**
 * @brief Class containing a list of users
 * This class contains the list of users.
 * Warning: this class is not linked to the internal database.
 * When a record is added or removed from the database, this object is not updated.
 */
class MORPHO_API C_MORPHO_UserList
{
private:
	UL	m_ul_magicFirst;
public:
	friend class C_MORPHO_Database;

	/**
	 * Default constructor
	 */
	C_MORPHO_UserList();

	/**
	 * Copy constructor
	 * @param[in] i_px_UserList source Object
	 */
	C_MORPHO_UserList(const C_MORPHO_UserList& i_px_UserList);

	/**
	 * Default destructor
	 */
	virtual ~C_MORPHO_UserList();

	/**
	 * Assignation operator
	 * @param[in] i_px_UserList source Object
	 */
	C_MORPHO_UserList& operator= (const C_MORPHO_UserList& i_px_UserList);

	/**
	 * Get a user instance from the list.
	 *
	 * @param[in] i_ul_UserIndex
	 * - User index (first index is 0 ...).
	 * @param[out] o_px_User
	 * - Retrieved user.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_USER_NOT_FOUND	The user index is higher than the last user recorded in the list.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetUser		(	UL					i_ul_UserIndex,
					C_MORPHO_User* &	o_px_User);
	/**
	 * Retrieves the number of user instances contained in the list.
	 *
	 * @param[out] o_ul_NbUser
	 * - Number of user instances contained in the list instance.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetNbUser	(	UL &				o_ul_NbUser);
private:
	I AddUser		(	C_MORPHO_User	&	i_x_User);
	I Reset();
protected:
	I Copy(const C_MORPHO_UserList& i_px_UserListSrc);

private:
	typedef struct _T_MORPHO_USER_LIST
	{
		_T_MORPHO_USER_LIST			*m_px_next;
		C_MORPHO_User				*m_px_User;
	} T_MORPHO_USER_LIST;

	typedef struct
	{
		UL								m_ul_nbUser;
		T_MORPHO_USER_LIST				*m_px_UserList;
	} T_MORPHO_USERLIST,*PT_MORPHO_USERLIST;

	PT_MORPHO_USERLIST m_px_MorphoUserList;

	UL	m_ul_magicEnd;
};

#endif // MORPHO_USERLIST_H
