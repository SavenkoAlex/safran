// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_TemplateList.h
 * @brief Definitions of the C_MORPHO_TemplateList class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_TEMPLATE_LIST_H
#define MORPHO_TEMPLATE_LIST_H

#ifndef SWIG
#ifdef MORPHO_SDK
#define MORPHO_API __declspec(dllexport)
#else
#define MORPHO_API __declspec(dllimport)
#endif
#endif

/**
 * @brief Class used to contain templates.
 * This class contains a list of templates. C_MORPHO_User inherits from this class.
 */
class MORPHO_API C_MORPHO_TemplateList
{
private:
	UL	m_ul_magicFirst;

/*********************************************/
public:
	friend class C_MORPHO_Database;
	friend class C_MORPHO_Device;
	friend class C_MORPHO_User;

/** @brief Maximum length of a PK COMP */
	#define MORPHO_PK_MAXLEN		256		// Maximum length of a PK COMP
/** @brief The maximum number of templates in C_MORPHO_TemplateList */
	#define MORPHO_TEMPLATE_MAX		200		// The maximum number of templates in C_MORPHO_TemplateList

	/**
	 * Default constructor
	 */
	C_MORPHO_TemplateList  ();

	/**
	 * Copy constructor
	 * @param[in] i_px_TemplateList source object
	 */
	C_MORPHO_TemplateList  (const C_MORPHO_TemplateList& i_px_TemplateList);

	/**
	 * Default destructor
	 */
	virtual ~C_MORPHO_TemplateList ();

	/**
	 * Assignation operator
	 * @param[in] i_px_TemplateList source object
	 */
	C_MORPHO_TemplateList& operator= (const C_MORPHO_TemplateList& i_px_TemplateList);

	/**
	 * @see PutTemplate (kept for compatibility)
	 */
	I PutTemplate(	T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
					UL						i_ul_lenTemplate,
					PUC						i_puc_dataTemplate,
					UC &					o_uc_indexTemplate);

	/**
	 * Add a template to the list.
	 * The C_MORPHO_TemplateList can contain several raw templates but only one X984 template.
	 *
	 * @param[in] i_uc_typTemplate
	 * - #MORPHO_PK_COMP should be used.
	 * - type of the template. Can be any of these values :
	 *    - #MORPHO_PK_COMP_NORM
	 *    - #MORPHO_PK_MAT
	 *    - #MORPHO_PK_MAT_NORM
	 *    - #MORPHO_PK_ANSI_378
	 *    - #MORPHO_PK_ANSI_378_2009
	 *    - #MORPHO_PK_MINEX_A
	 *    - #MORPHO_PK_ISO_FMR
	 *    - #MORPHO_PK_ISO_FMR_2011
	 *    - #MORPHO_PK_ISO_FMC_NS
	 *    - #MORPHO_PK_ISO_FMC_CS
	 *    - #MORPHO_PK_PKLITE (only available on MorphoSmart&tm; CBM-E3)<br>
	 * are also supported but Morpho recommends
	 * using them only for compatibility with existing systems or specific usage.
	 * @param[in] i_ul_lenTemplate
	 * - Template length.
	 * @param[in] i_puc_dataTemplate
	 * - Template.
	 * @param[in] i_uc_dataIndex
	 * - In case of use of ANSI 378 and ISO FMR, the template can store more than one fingerprint.
	 * This parameter allows selecting one specific fingerprint in the template.
	 * Set to 0xFF, all fingerprints in the template are selected.
	 * @param[in] i_uc_PkFpQuality
	 * - Quality of the fingerprint
	 * @param[out] o_uc_indexTemplate
	 * - Template index in the list (starts from 0).
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER  One or more input parameters are out of range.
	 */
	I PutTemplate(	T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
					UL						i_ul_lenTemplate,
					PUC						i_puc_dataTemplate,
					UC						i_uc_dataIndex,
				    UC						i_uc_PkFpQuality,
					UC &					o_uc_indexTemplate);

	/**
	 * @see PutTemplate (kept for compatibility)
	 */
	I PutTemplate(	T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
					UL						i_ul_lenTemplate,
					PUC						i_puc_dataTemplate,
					UC						i_uc_dataIndex,
					UC &					o_uc_indexTemplate);



    /**
	 * Add a template to the list.
	 * The C_MORPHO_TemplateList can contain several multimodal templates.
	 *
	 * @param[in] i_uc_typTemplate
	 * - #MORPHO_PK_FVP
	 * - #MORPHO_PK_FVP_MATCH.
	 * @param[in] i_ul_lenTemplate
	 * - Template length.
	 * @param[in] i_puc_dataTemplate
	 * - Template.
	 * @param[in] i_uc_PkFVPQuality
	 * - Quality of the fingerprint part of multimodal template.
	 * @param[in] i_uc_ExportAdvancedSecurityLevelsCompatibility
	 * - Advanced security levels compatibility of multimodal template.
	 * @param[out] o_uc_indexTemplate
	 * - Template index in the list (starts from 0).
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	 I PutFVPTemplate(  T_MORPHO_FVP_TYPE_TEMPLATE	i_uc_typTemplate,
					UL						    i_ul_lenTemplate,
					PUC						    i_puc_dataTemplate,
					UC						    i_uc_PkFVPQuality,
					UC						    i_uc_ExportAdvancedSecurityLevelsCompatibility,
					UC &					    o_uc_indexTemplate);


	/**
	 * @see PutFVPTemplate (kept for compatibility)
	 */
	I PutFVPTemplate(  T_MORPHO_FVP_TYPE_TEMPLATE	i_uc_typTemplate,
					UL						    i_ul_lenTemplate,
					PUC						    i_puc_dataTemplate,
					UC &					    o_uc_indexTemplate);

	/**
	 * Set the template to a fake template to indicate that
	 */
	I SetTemplateIsFromSmartCard (
					T_MORPHO_SMARTCARD_WORKFLOW_TYPE	i_x_WorkFlowCardType,	// Work Flow type to read the templates in the Smart Card
					T_MORPHO_TYPE_TEMPLATE				i_x_typTemplate,		// template type stored in the Smart Card
					UC									i_uc_TemplateNbr		// Number of Templates to read in the Smart Card
					);

	/**
	 * @todo Not documented
	 */
	I GetIsTemplateFromSmartCard (
					PT_MORPHO_TEMPLATE_IN_SMARTCARD		&o_px_TemplateInSmartCard
					);

    /**
	 * Retrieves a multimodal template content from the list.
	 *
	 * @param[in] i_uc_indexTemplate
	 * - Template index in the list (starts from 0).
	 * @param[out] o_uc_typTemplate
	 * - Type of the template. Can be any of these values:
	 *    - #MORPHO_PK_FVP
	 *    - #MORPHO_PK_FVP_MATCH.
	 * @param[out] o_ul_lenTemplate
	 * - Template length.
	 * @param[out] o_puc_dataTemplate
	 * - Template buffer.
	 * @param[out] o_uc_PkFVPQuality
	 * - Quality of the fingerprint part of multimodal template.
	 * @param[out] o_uc_AdvancedSecurityLevelsCompatibility
	 * - Advanced security levels compatibility of multimodal template.
 	 * @param[in] i_uc_dataIndex
	 * - Must be set to 0xFF.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	Invalid template index.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	 I GetFVPTemplate(
			    UC									i_uc_indexTemplate,
				T_MORPHO_FVP_TYPE_TEMPLATE &	    o_uc_typTemplate,
				UL	&						        o_ul_lenTemplate,
				PUC	&						        o_puc_dataTemplate,
				UC &								o_uc_PkFVPQuality,
				UC  &								o_uc_AdvancedSecurityLevelsCompatibility,
				UC	&						        i_uc_dataIndex);


	/**
	 * @see GetFVPTemplate (kept for compatibility)
	 */
	 I GetFVPTemplate(	UC							i_uc_indexTemplate,
				T_MORPHO_FVP_TYPE_TEMPLATE &	    o_uc_typTemplate,
				UL	&						        o_ul_lenTemplate,
				PUC	&						        o_puc_dataTemplate,
				UC	&						        i_uc_dataIndex);

	/**
	 * Retrieves a template content from the list.
	 *
	 * @param[in] i_uc_indexTemplate
	 * - Template index in the list (starts from 0).
	 * @param[out] o_uc_typTemplate
	 * - type of the template. Can be any of these values :
	 *    - #MORPHO_PK_COMP
	 *    - #MORPHO_PK_COMP_NORM
	 *    - #MORPHO_PK_MAT
	 *    - #MORPHO_PK_MAT_NORM
	 *    - #MORPHO_PK_ANSI_378
	 *    - #MORPHO_PK_ANSI_378_2009
	 *    - #MORPHO_PK_MINEX_A
	 *    - #MORPHO_PK_ISO_FMR
	 *    - #MORPHO_PK_ISO_FMR_2011
	 *    - #MORPHO_PK_ISO_FMC_NS
	 *    - #MORPHO_PK_ISO_FMC_CS
	 *    - #MORPHO_PK_PKLITE (only available on MorphoSmart&tm; CBM-E3)
	 * @param[out] o_ul_lenTemplate
	 * - Template length.
	 * @param[in] i_uc_dataIndex
	 * - In case of use of ANSI 378 and ISO FMR, the template can store more than one fingerprint.
	 * This parameter allows selecting one specific fingerprint in the template.
	 * Set to 0xFF, all fingerprints in the template are selected.
	 * @param[out] o_uc_PkFpQuality
	 * -  Quality of the fingerprint
	 * @param[out] o_puc_dataTemplate
	 * - Template. NULL if not used.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	Invalid template index.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	 I GetTemplate(	UC						i_uc_indexTemplate,
					T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
					UL	&						o_ul_lenTemplate,
					PUC	&						o_puc_dataTemplate,
					UC	&						o_uc_PkFpQuality,
					UC	&						i_uc_dataIndex);

	 /**
	 * @see GetTemplate (kept for compatibility)
	 */
	 I GetTemplate(	UC							i_uc_indexTemplate,
					T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
					UL	&						o_ul_lenTemplate,
					PUC	&						o_puc_dataTemplate,
					UC	&						i_uc_dataIndex);


	/**
	 * @see GetTemplate (kept for compatibility)
	 */
	I GetTemplate(	UC							i_uc_indexTemplate,
					T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
					UL	&						o_ul_lenTemplate,
					PUC	&						o_puc_dataTemplate);

	/**
	 * Activate Image Export.
	 * @param[in] i_b_fullImageState
	 * - TRUE to activate. Default is FALSE
	 *  @see C_MORPHO_Device::Capture() @see GetActiveFullImageRetrieving() @see GetFullImageRetrieving()
	 */
	VOID SetActiveFullImageRetrieving (bool i_b_fullImageState);

	/**
	 * Check if Image export is activated.
	 *  @retval  TRUE if Image export is activated
	 *  @retval  FALSE if Image export is not activated
	 *
	 *  @see SetActiveFullImageRetrieving
	 */
	bool GetActiveFullImageRetrieving();

	/**
	 * Retrieves Image exported with C_MORPHO_Device::Capture or C_MORPHO_User::Enroll
	 *
	 * @param[in] i_uc_indexImage
	 * - Image index in the list (starts from 0).
	 * @param[out] o_ul_FullImageRetriving
	 * - Length of image retrieved or length of compressed image buffer
	 * @param[out] o_puc_bufFullImageRetriving
	 * - Image or Compressed image buffer
	 *
	 * @retval #MORPHO_OK	Image has been successfully retrieved
	 * @retval #MORPHOERR_BADPARAMETER	Invalid image index, or no image to retrieve.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetFullImageRetrieving 	(
								UC	i_uc_indexImage , // Index de l'empreinte
								UL	&o_ul_FullImageRetriving , // Taille du buffer de retour
								PUC &o_puc_bufFullImageRetriving  // Buffer contenant les données
							);

	/**
	 * Retrieves Image exported with C_MORPHO_Device::Capture or C_MORPHO_User::Enroll
	 *
	 * @param[in] i_uc_indexImage
	 * - Image index in the list (starts from 0).
	 * @param[out] o_ppx_morphoImage
	 * - The image is returned in full resolution.
	 *
	 * @retval #MORPHO_OK	Image has been successfully retrieved
	 * @retval #MORPHOERR_BADPARAMETER	Invalid image index, or no image to retrieve.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetFullImageRetrieving 	(
								UC				i_uc_indexImage , // Index de l'empreinte
								T_MORPHO_IMAGE*	&o_ppx_morphoImage   // struct T_MORPHO_IMAGE
							);

	I PutFullImageRetrieving 	(
								UC	&o_uc_indexImage, // Index de l'empreinte
								UL	i_ul_FullImageRetriving, // Taille du buffer de retour
								PUC i_puc_bufFullImageRetriving // Buffer contenant les données
							);

	VOID SetEtatEncryption(UC i_uc_IsBioDataEncryption);


	/**
	 * Retrieves the application data contained in the X9.84 biometric token.
	 * @attention this function does not verify the X9.84 signature.
	 * The data integrity should have been checked using the C_MORPHO_TemplateList::VerifSignX984() function.
	 *
	 * @param[in] i_ul_X984Size
	 * - Length of the X984 biometric token.
	 * @param[in] i_puc_X984
	 * - Template with a X9.84 envelop. This template can be issued from the C_MORPHO_TemplateList::GetX984() function
	 * for example.
	 * @param[out] o_ul_lenApplicationData
	 * - Length of the application data buffer.
	 * @param[out] o_puc_ApplicationData
	 * - Application data extracted from the X9.84 envelop. NULL if not used.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	The template is not encapsulated within an X9.84 envelope.
	 */
	I ExtractDataX984 (
					UL							i_ul_X984Size,
					PUC							i_puc_X984,
					UL						  &	o_ul_lenApplicationData,
					PUC						  & o_puc_ApplicationData);

	/**
	 * Retrieves the raw template contained in the X9.84 biometric token.
	 * @attention this function does not verify the X9.84 signature.
	 * The data integrity should have been checked using the C_MORPHO_TemplateList::VerifSignX984() function.
	 *
	 * @param[in] i_ul_X984Size
	 * - Length of the X9.84 biometric token buffer.
	 * @param[in] i_puc_X984
	 * - Template with an X9.84 envelope. This template can be issued from the C_MORPHO_TemplateList::GetX984() function
	 * for example.
	 * @param[in] i_uc_indexTemplate
	 * - Template index. 1 to extract the template of the first finger,
	 * 2 to extract the second template …
	 * @param[out] o_uc_typTemplate
	 * - The template acquisition format can. The template format should be #MORPHO_PK_COMP.<br>
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
	 * @param[out] o_ul_lenTemplate
	 * - Length of the template buffer.
	 * @param[out] o_puc_dataTemplate
	 * - The raw template. This buffer is allocated by C_MORPHO_TemplateList::PutTemplate and
	 * released by C_MORPHO_TemplateList destructor.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	The class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	The template is not encapsulated within an X9.84 envelope.
	 * The index i_uc_indexTemplate is unknown.

	 */
	I ExtractTemplateX984 (
							UL							i_ul_X984Size,
							PUC							i_puc_X984,
							UC							i_uc_indexTemplate,
							T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
							UL	&						o_ul_lenTemplate,
							PUC &						o_puc_dataTemplate);

	/**
	 * Retrieves the multimodal template contained in the X9.84 biometric token.
	 * @attention this function does not verify the X9.84 signature.
	 * The data integrity should have been checked using the C_MORPHO_TemplateList::VerifSignX984() function.
	 *
	 * @param[in] i_ul_X984Size
	 * - Length of the X9.84 biometric token buffer.
	 * @param[in] i_puc_X984
	 * - Multimodal template with an X9.84 envelope. This template can be issued from the C_MORPHO_TemplateList::GetFVPX984() function
	 * for example.
	 * @param[in] i_uc_indexTemplate
	 * - Template index. 1 to extract the template of the first finger,
	 * 2 to extract the second template …
	 * @param[out] o_uc_typFVPTemplate
	 * - Type of the template. Can be any of these values:
	 *    - #MORPHO_PK_FVP
	 *    - #MORPHO_PK_FVP_MATCH
	 * @param[out] o_ul_lenTemplate
	 * - Length of the template buffer.
	 * @param[out] o_puc_dataTemplate
	 * - Template buffer. This buffer is allocated by C_MORPHO_TemplateList::PutTemplate and
	 * released by C_MORPHO_TemplateList destructor.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	The class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	The template is not encapsulated within an X9.84 envelope.
	 * The index i_uc_indexTemplate is unknown.

	 */
	I ExtractFVPTemplateX984 (
							UL							i_ul_X984Size,
							PUC							i_puc_X984,
							UC							i_uc_indexTemplate,
							T_MORPHO_FVP_TYPE_TEMPLATE &o_uc_typFVPTemplate,
							UL	&						o_ul_lenTemplate,
							PUC &						o_puc_dataTemplate);

	/**
	 * Retrieves the signer identifier contained in the X9.84 biometric token.
	 * @attention the integrity is not verified.
	 * More information about the X9.84 format can be found in the document MorphoSmartOverview.
	 * The signer identifier is the SHA-1 hash of the signer certificate
	 *
	 * @param[in] i_ul_X984Size
	 * - Length of the X9.84 biometric template buffer.
	 * @param[in] i_puc_X984
	 * - Template with an X9.84 envelope. This template can be obtained from the C_MORPHO_TemplateList::GetX984() function
	 * for example.
	 * @param[out] o_ul_lenSignerId
	 * - Length of the Signer id buffer.
	 * @param[out] o_puc_SignerId
	 * - The signer identifier. This buffer is allocated and released by C_MORPHO_TemplateList
	 * destructor.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range
	 * @retval #MORPHOERR_SECU	Cryptographic errors

	 */
	I ExtractSignerIdX984 (
							UL							i_ul_X984Size,
							PUC							i_puc_X984,
							UL						  &	o_ul_lenSignerId,
							PUC						  & o_puc_SignerId);

	/**
	 * Stores the X984 biometric token in the template list.<br>
	 * The C_MORPHO_TemplateList can have several raw templates but only one X984 template.<br>
	 * The X984 biometric token contains between one to ten templates.<br>
	 * The template format should be #MORPHO_PK_COMP.<br>
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
	 * The X984 biometric token can be accepted and its integrity verified by a secure MorphoSmart&tm;
	 * only.<br>
	 * More information can be found in the document "MorphoSmartHostSystemInterfaceSpecification.pdf".
	 *
	 * @param[in] i_ul_X984Size
	 * - Length of the X9.84 biometric template buffer.
	 * @param[in] i_puc_X984
	 * - Template with an X9.84 envelope.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_ELT_ALREADY_PRESENT	The template is already present in the list.

	 */
	I PutX984(
							UL						i_ul_X984Size,
							PUC						i_puc_X984);
	/**
	 * Get the X984 biometric template in the template list.
	 *
	 * @param[out] o_ul_X984Size
	 * - Length of the X9.84 biometric template buffer.
	 * @param[out] o_puc_X984
	 * - Template with an X9.84 envelope. This referenced buffer is allocated in a private
	 * buffer and is released by C_MORPHO_TemplateList destructor.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.

	 */
	I GetX984(
							UL					&	o_ul_X984Size,
							PUC					&	o_puc_X984);

	/**
	 * Stores the X984 biometric token for multimodal template in the template list.<br>
	 * The C_MORPHO_TemplateList can have several multimodal templates but only one X984 multimodal template.<br>
	 * The X984 biometric token contains between one to ten multimodal templates.<br>
	 * The template format should be #MORPHO_PK_FVP or #MORPHO_PK_FVP_MATCH.<br>
	 * The X984 biometric token can be accepted and its integrity verified by a secure MorphoSmart&tm;
	 * only.<br>
	 * More information can be found in the document "MorphoSmartHostSystemInterfaceSpecification.pdf".
	 *
	 * @param[in] i_ul_fvp_X984Size
	 * - Length of the X9.84 biometric multimodal template buffer.
	 * @param[in] i_puc_fvp_X984
	 * - Multimodal template with an X9.84 envelope.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_ELT_ALREADY_PRESENT	The multimodal template is already present in the list.

	 */
	I PutFVPX984(
							UL					i_ul_fvp_X984Size,
							PUC					i_puc_fvp_X984);
	/**
	 * Get the X984 multimodal template in the template list.
	 *
	 * @param[out] o_ul_fvp_X984Size
	 * - Length of the X9.84 multimodal template buffer.
	 * @param[out] o_puc_fvp_X984
	 * - Multimodal template with an X9.84 envelope. This referenced buffer is allocated in a private
	 * buffer and is released by C_MORPHO_TemplateList destructor.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.

	 */
	I GetFVPX984(
							UL					&	o_ul_fvp_X984Size,
							PUC					&	o_puc_fvp_X984);
	/**
	 * Set the X9.84 template index.
	 *
	 * @param[in] i_uc_PkX984Index
	 * - Set 0 to use all templates, set 1 to use the first template only,
	 * or set 2 to use the second template only.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.

	 */
	I SetPkX984Index(			UC	i_uc_PkX984Index);

	/**
	 * The X9.84 template signature is checked.
	 *
	 * @param[in] i_ul_X984Size
	 * - Length of the X9.84 biometric template buffer.
	 * @param[in] i_puc_X984
	 * - Template with an X9.84 envelope.
	 * @param[in] i_ul_MsoCertificateSize
	 * - Length of the MorphoSmart&tm; certificate buffer.
	 * @param[in] i_puc_MsoCertificate
	 * - the MorphoSmart&tm; certificate buffer.
	 * @param[in] i_pf_VerifSignRSA
	 * - verifies the RSA signature. This argument can be null.
	 * @param[in] i_pf_VerifSignDSA
	 * - verifies the DSA signature. This argument can be null.
	 * @param[out] o_pi_Result
	 * - verification callback function returned status. The example MSOSECU.dll returns TRUE (signature is OK) or FALSE (the signature is false).
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range
	 * @retval #MORPHOERR_SECU	The callback functions returned the errors between -2000 and -2199 (see MSOSECU_Errors.h), or ASN1 errors, or RSA errors, or DSA errors, or callback function RSA is not set, or callback function DSA is not set, or the cryptography algorithm format is unknown.
	 * @retval #MORPHOERR_USER	The callback functions returned the errors between -10000 and -10499.
	 * @retval #MORPHOERR_SIGNER_ID	The certificate identity is not the same than the X984 certificate identity
	 * @retval #MORPHOERR_SIGNER_ID_INVALID	The X984 certificate identity size is different to 20 octets (SHA_1 size)

	 */
	I VerifSignX984 (
							UL									i_ul_X984Size,
							PUC									i_puc_X984,
							UL									i_ul_MsoCertificateSize,
							PUC									i_puc_MsoCertificate,
							T_MORPHO_CALLBACK_VERIF_SIGN		i_pf_VerifSignRSA,
							T_MORPHO_CALLBACK_VERIF_SIGN		i_pf_VerifSignDSA,
							PI									o_pi_Result);

	/**
	 * Get the number of fingerprint templates in C_MORPHO_TemplateList.
	 *
	 * @param[in] o_uc_NbTemplate: the number of template in C_MORPHO_TemplateList.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetNbTemplate(UC &	o_uc_NbTemplate);

	/**
	 * Get the number of multimodal templates in C_MORPHO_TemplateList.
	 *
	 * @param[in] o_uc_NbTemplate: the number of template in C_MORPHO_TemplateList.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetNbFVPTemplate(UC &	o_uc_NbTemplate);

protected:
	I Copy( C_MORPHO_TemplateList& io_px_TemplateListDest, const C_MORPHO_TemplateList& i_px_TemplateListSrc );

private:
	I Reset();

private:


	typedef struct _T_MORPHO_EXPORT_IMAGE_LIST
	{
		_T_MORPHO_EXPORT_IMAGE_LIST		*m_px_next;
		UL								m_ul_lenImage;
		PUC								m_puc_bufImage;
		T_MORPHO_IMAGE					*m_px_morphoImage;

	}T_MORPHO_EXPORT_IMAGE_LIST,*PT_MORPHO_EXPORT_IMAGE_LIST;


	typedef struct _T_MORPHO_TEMPLATE_LIST
	{
		_T_MORPHO_TEMPLATE_LIST		*m_px_next;
		T_MORPHO_TYPE_TEMPLATE		m_uc_typTemplate;
		UL							m_ul_lenTemplate;
		UC							m_uc_PkIndex;
		PUC							m_puc_dataTemplate;
		UC							m_uc_PkFpQuality;

	} T_MORPHO_TEMPLATE_LIST;


	typedef struct _T_MORPHO_FVP_TEMPLATE_LIST
	{
		_T_MORPHO_FVP_TEMPLATE_LIST		*m_px_next;
		T_MORPHO_FVP_TYPE_TEMPLATE		m_uc_typTemplate;
		UL								m_ul_lenTemplate;
		UC								m_uc_FVPIndex;
		PUC								m_puc_dataTemplate;
		UC								m_uc_PkFpQuality;
		UC								m_uc_AdvancedSecurityLevelsCompatibility;

	} T_MORPHO_FVP_TEMPLATE_LIST;

	typedef struct
	{
		UL								m_ul_X984Size;
		PUC								m_puc_X984;

		UL								m_ul_fvp_X984Size;
		PUC								m_puc_fvp_X984;

		UL								m_ul_DataTokenSize;
		PUC								m_puc_DataToken;

		UL								m_ul_lenSignerId;
		PUC								m_puc_SignerId;

		UC								m_uc_nbTemplate;
		UC								m_uc_nbFVPTemplate;
		T_MORPHO_TEMPLATE_LIST			*m_px_TemplateList;
		T_MORPHO_FVP_TEMPLATE_LIST		*m_px_FVPTemplateList;

		UC								m_uc_PkX984Index;


		T_MORPHO_EXPORT_IMAGE_LIST		*m_px_Export_Image_List;
		bool							m_b_ActiveFullImageRetriving;
		PT_MORPHO_TEMPLATE_IN_SMARTCARD	m_px_TemplateInSmartCard;

		UC								m_uc_IsBioDataEncryption;

	} T_MORPHO_TEMPLATELIST,*PT_MORPHO_TEMPLATELIST;

	PT_MORPHO_TEMPLATELIST m_px_MorphoTemplateList;

	UL	m_ul_magicEnd;
};

#endif // MORPHO_TEMPLATE_LIST_H
