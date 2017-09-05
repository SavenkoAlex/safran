// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file MORPHO_Device.h
 * @brief Definition of the C_MORPHO_Device class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_DEVICE_H
#define MORPHO_DEVICE_H

#if defined (__GNUC__)
#undef min
#undef max
#endif

#include <vector>
#include <string>

#ifdef _ANDROID_USB
#include "jni.h"
#endif
#ifndef SWIG
#ifdef MORPHO_SDK
#define MORPHO_API __declspec(dllexport)
#else
#define MORPHO_API __declspec(dllimport)
#endif
#endif

#define IHM_BUFFER_SIZE		4*1024	// Maximum Size of CSV File

/**
 * @brief Class communicating with the MorphoSmart&tm;.
 * This class performs operations related to a physical MorphoSmart&tm;.
 * The internal variable members are filled after a correct connection to the MorphoSmart&tm;
 * (after a successful OpenDevice() call).
 */
class MORPHO_API C_MORPHO_Device
{
private:
	UL	m_ul_magicFirst;

	/** @intern String that describes the last received config
	*/
	PUC m_puc_LastReceivedConfig;

public:

	/** @brief Device connection type */
	enum T_DEVICE_CONNECTION_TYPE{
		COM_TYPE_NOT_SET = 0,	/**< No connection opened */
		RS232_COM_TYPE = 1,		/**< Serial connection */
		USB_COM_TYPE = 2,		/**< USB connection */
		USER_COM_TYPE = 3		/**<  */
	};

	/** @brief Maximum number of databases */
	#define MORPHO_NB_DATABASE_MAX				5		// maximum database occurences
	/** @brief MorphoSmart&tm; general descriptor maximum length */
	#define MORPHO_DESCRIPTOR_MAXLEN	1024		// MSO product descriptor maximum length
	/** @brief MorphoSmart&tm; product descriptor maximum length */
	#define MORPHO_PRODUCT_DESCRIPTOR_MAXLEN	MORPHO_DESCRIPTOR_MAXLEN		// MSO product descriptor maximum length
	/** @brief MorphoSmart&tm; software descriptor maximum length */
	#define MORPHO_SOFTWARE_DESCRIPTOR_MAXLEN	MORPHO_DESCRIPTOR_MAXLEN		// MSO software descriptor maximum length
	/** @brief MorphoSmart&tm; sensor descriptor maximum length */
	#define MORPHO_SENSOR_DESCRIPTOR_MAXLEN		MORPHO_DESCRIPTOR_MAXLEN		// MSO sensor descriptor maximum length

	// members

	/** @brief String that describes the product reference
	 *  @attention these fields contain strings in a human readable format, and subject to changes from one version to another. They should be used for user information only, and no automatic data retrieval should be performed on these values.
	 */
	PUC	m_puc_ProductDescriptor;
	/** @brief String that describes the software release
	 *  @attention these fields contain strings in a human readable format, and subject to changes from one version to another. They should be used for user information only, and no automatic data retrieval should be performed on these values.
	 */
	PUC	m_puc_SoftwareDescriptor;
	/** @brief String that describes the optical sensor
	 *  @attention these fields contain strings in a human readable format, and subject to changes from one version to another. They should be used for user information only, and no automatic data retrieval should be performed on these values.
	 */
	PUC	m_puc_SensorDescriptor;

	/**
	 * Default constructor
	 */
	C_MORPHO_Device ();

	/**
	 * Copy constructor
	 * @param[in] i_px_MorphoDevice source Object
	 */
	C_MORPHO_Device(const C_MORPHO_Device& i_px_MorphoDevice);

	/**
	 * Default destructor
	 */
	virtual ~C_MORPHO_Device ();

	/**
	 * Assignation operator
	 * @param[in] i_px_MorphoDevice source Object
	 */
	C_MORPHO_Device& operator= (const C_MORPHO_Device& i_px_MorphoDevice);

	/**
	 * This function returns the identifier of the connection to a specific MorphoSmart&tm;
	 * of a C_MORPHO_Device object. The returned value is NULL at the creation of a
	 * C_MORPHO_Device object, before the connection to the MorphoSmart&tm; or after the
	 * closing of the connection to the MorphoSmart&tm;. If the connection failed,
	 * the returned value remains NULL.
	 *
	 * @cond MORPHO_IMPLEMENTATION_NOTE
	 * @note - @ref IMPLEMENTATION_NOTE_GETHANDLE
	 * @endcond
	 *
	 * @retval #MORPHO_HANDLE Identifier of the connexion to the MorphoSmart&tm;
	 */
	MORPHO_HANDLE GetHandle(VOID);
	/**
	 * This function returns the type of  the connection to a specific MorphoSmart&tm;
	 * of a C_MORPHO_Device object. The returned value is NULL at the creation of a
	 * C_MORPHO_Device object, before the connection to the MorphoSmart&tm; or after the
	 * closing of the connection to the MorphoSmart&tm;. If the connection failed,
	 * the returned value remains NULL.
	 *
	 * @retval #USB_COM_TYPE	USB connection
	 * @retval #RS232_COM_TYPE	Serial connection
	 * @retval #COM_TYPE_NOT_SET	No connection opened
	 */
	UL	GetComType(VOID);

	/**
	 * This function opens the MorphoSmart&tm; communication link and tests it
	 * with a basic exchange with the MorphoSmart&tm;. This method updates
	 * C_MORPHO_Device::m_puc_ProductDescriptor, C_MORPHO_Device::m_puc_SoftwareDescriptor and C_MORPHO_Device::m_puc_SensorDescriptor
	 * fields. Speed rate is managed by the SDK (115200 bps on a serial COM).
	 *
	 * @param[in]  i_i_SerialPortNumber
	 *	- -1: 	USB connection (MSO300)
	 *	- 0: 	Auto detection mode. A command is sent to all available serial COM ports. The detection process is stopped with the first MorphoSmart&tm; found. We do not recommend this mode because it may cause problem with some other plug-in peripherals, and this mode is slower than the direct option.
	 *	- >0: 	Serial COM port number (MSO200): 1 means COM1, 2 means COM2, ...
	 *
	 * @param[in] i_i_BaudRate
	 * - The default value is 115200 bauds.
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CONNECT	Cannot connect to the MorphoSmart&tm;.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_NO_SERVER	The Morpho MorphoSmart Service Provider Usb Server is stopped or not installed.
	 */
	I OpenDevice (
				I   i_i_SerialPortNumber,
				I	i_i_BaudRate = 115200
				);


	/**
		 * This function opens the MorphoSmart&tm; communication link and tests it
		 * with a basic exchange with the MorphoSmart&tm;. This method updates
		 * C_MORPHO_Device::m_puc_ProductDescriptor, C_MORPHO_Device::m_puc_SoftwareDescriptor and C_MORPHO_Device::m_puc_SensorDescriptor
		 * fields.
		 *
		 * @param[in]  i_pc_SerialPortName
		 *	- Serial port name COM1, COM2, COMx for Windows and tty%s for Linux ttyS1 ttyUSB0 ,etc ...
		 *
		 * @param[in] i_i_BaudRate
		 * - Baudrate of the communication.
		 * @retval #MORPHO_OK	The execution of the function was successful.
		 * @retval #MORPHOERR_CONNECT	Cannot connect to the MorphoSmart&tm;.
		 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
		 * @retval #MORPHOERR_NO_SERVER	The Morpho MorphoSmart Service Provider Usb Server is stopped or not installed.
		 * @retval #MORPHOERR_BADPARAMETER  One or more input parameters are out of range.
		 */
		I OpenDevice (
					PC   i_pc_SerialPortName,
					I	i_i_BaudRate
					);

	/**
	 * This function sends the information required to open a communication
	 * (RS232, USB or user communication)
	 *
	 * @param[in]  i_pf_CallbackComOpen
	 * - User function called to replace SPRS232_Open or SPUSB_Open.
	 * @param[in]  i_pf_CallbackComSend
	 * - User function called to replace SPRS232_Send or SPUSB_Send,
	 * @param[in]  i_pf_CallbackComReceive
	 * - User function called to replace SPRS232_Receive or SPUSB_Receive,
	 * @param[in]  i_pf_CallbackComReceiveFree
	 * - User function called to replace SPRS232_ ReceiveFree or SPUSB_ ReceiveFree,
	 * @param[in]  i_pf_CallbackComClose
	 * - User function called to replace SPRS232_Close or SPUSB_Close,
	 * @param[in]  i_pc_String
	 * - User ComOpen function parameter,
	 * @param[in]  i_ul_Param
	 * - User ComOpen function parameter,
	 * @param[in,out]  io_pv_Param
	 * - User ComOpen function parameter.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CONNECT	Connection with the MSO failed.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_USER	The function has returned an error, defined and implemented by the user.
	 * @retval #MORPHOERR_NO_SERVER	The Morpho MorphoSmart Service Provider Usb Server is stopped or not installed.
	 */
	I OpenDevice (
			T_MORPHO_CALLBACK_COM_OPEN			i_pf_CallbackComOpen,
			T_MORPHO_CALLBACK_COM_SEND			i_pf_CallbackComSend,
			T_MORPHO_CALLBACK_COM_RECEIVE		i_pf_CallbackComReceive,
			T_MORPHO_CALLBACK_COM_RECEIVE_FREE	i_pf_CallbackComReceiveFree,
			T_MORPHO_CALLBACK_COM_CLOSE			i_pf_CallbackComClose,
			PC									i_pc_String,
			UL									i_ul_Param,
			PVOID								io_pv_Param
			);



	/**
	 * This function enumerates the number of connected USB MorphoSmart&tm; devices.<br>
	 * It builds an internal list which is composed of USB MorphoSmart&tm; name and
	 * returns the number of plugged device. The MorphoSmart&tm; name is the product
	 * number concatenated with the serial number.<br>
	 * To get the name of the device that are currently plugged to the PC,
	 * call the GetUsbDevicesNameEnum function.
	 *
	 * @param[out]  o_pul_NbUsbDevice
	 * - The number of the USB MorphoSmart&tm; devices plugged to the host.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL	USB errors.
	 * @retval #MORPHOERR_CONNECT	No libraries were found to communicate with a device.
	 */
	#ifdef _ANDROID_USB
		I InitUsbDevicesNameEnum( PUL	o_pul_NbUsbDevice, JNIEnv* i_env);
	#else
		I InitUsbDevicesNameEnum( PUL	o_pul_NbUsbDevice);
	#endif

	/**
	 * InitUsbDevicesNameEnum must be called before using this function.<br>
	 * This function returns a string containing the name of the device.
	 *
	 * @param[in] i_ul_Index
	 * - This index is between 0 and the number of connected device returned by the function InitUsbDevicesNameEnum.
	 * @param[out]  o_pc_MsoName
	 * - A string containing the name of the MSO (product number-serial number).
	 * @param[out]  o_pc_MsoProperties
	 * - A string containing the type of the MSO (MSO100, MSO300...) and other information.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	The index is unknown.
	 */
	I GetUsbDevicesNameEnum (
							UL		i_ul_Index,
							PC	&	o_pc_MsoName,
							PC	&	o_pc_MsoProperties
							);

	/**
	 * This function opens the USB communication link and tests it with a basic exchange
	 * with the MorphoSmart&tm;. This method updates C_MORPHO_Device::m_puc_ProductDescriptor,
	 * C_MORPHO_Device::m_puc_SoftwareDescriptor and C_MORPHO_Device::m_puc_SensorDescriptor fields.
	 *
	 * @param[in] i_pc_MsoName
	 * - A string containing the name of the MSO (product number-serial number), returned by the GetUsbDevicesNameEnum function.
	 * @param[in] i_ul_TimeOut
	 * - Timeout in millisecond.
	 * @param[in,out] io_pv_param
	 * - Must be set to NULL.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CONNECT	Cannot connect to the MorphoSmart&tm;.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_USB_DEVICE_NAME_UNKNOWN	The specified USB device is not plugged
	 * @retval #MORPHOERR_NO_SERVER	The Morpho MorphoSmart Service Provider Usb Server is stopped or not installed.
	 * @retval #MORPHOERR_BADPARAMETER  One or more input parameters are out of range.
	 */
	I OpenUsbDevice (
					PC								i_pc_MsoName,
					UL								i_ul_TimeOut,
					PVOID							io_pv_param
					);
#ifdef _ANDROID_USB
	/**
	 * This function opens the USB communication link and tests it with a basic exchange
	 * with the MorphoSmart&tm;. This method updates C_MORPHO_Device::m_puc_ProductDescriptor,
	 * C_MORPHO_Device::m_puc_SoftwareDescriptor and C_MORPHO_Device::m_puc_SensorDescriptor fields.
	 *
	 * @param[in] i_i_BusNumber
	 * - Morpho usb device bus.
	 * @param[in] i_i_DeviceAddress
	 * - Morpho usb device address.
	 * @param[in] i_i_FileDescriptor
	 * - Morpho usb device file descriptor.
	 * @param[in] i_ul_TimeOut
	 * - Timeout in millisecond.
	 * @param[in,out] io_pv_param
	 * - Must be set to NULL.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CONNECT	Cannot connect to the MorphoSmart&tm;.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_USB_DEVICE_NAME_UNKNOWN	The specified USB device is not plugged
	 * @retval #MORPHOERR_NO_SERVER	The Morpho MorphoSmart Service Provider Usb Server is stopped or not installed.
	 * @retval #MORPHOERR_BADPARAMETER  One or more input parameters are out of range.
	 */
	I OpenUsbDeviceFD (
					I 								i_i_BusNumber,
					I 								i_i_DeviceAddress,
					I 								i_i_FileDescriptor,
					UL								i_ul_TimeOut,
					PVOID							io_pv_param
					);
	/**
	 * OpenUsbDeviceFD must be called before using this function.<br>
	 * This function returns a string containing the name of the device.
	 *
	 * @param[out]  o_pc_MsoName
	 * - A string containing the name of the MSO (product number-serial number).
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL	Could not retrieve device name .
	 */
	I GetOpenedUsbDeviceName (
							PC	&	o_pc_MsoName
							);

#endif //_ANDROID_USB
					


	/**
	 * This function sends the information required to open a communication
	 * (RS232, USB or user communication)
	 *
	 * @param[in] i_ul_Timeout
	 * - set timeout.
	 * @param[in] i_puc_Data
	 * - Buffer to send
	 * @param[in] i_ul_DataSize
	 * - Size of the buffer to send.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_PROTOCOLE	RS232 or USB error
	 * @retval #MORPHOERR_INTERNAL	Internal error
	 * @retval #MORPHOERR_USER	User error
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 */
	I ComSend (
					UL		i_ul_Timeout,
					PUC		i_puc_Data,
					UL		i_ul_DataSize);

	/**
	 * This function receives the information required to open a communication
	 * (RS232, USB or user communication)
	 *
	 * @param[in] i_ul_Timeout
	 * - set timeout.
	 * @param[in,out] io_ppuc_Data
	 * - This buffer is allocated by the Receive callback Function
	 * @param[out] o_pul_DataSize
	 * - size of buffer.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_PROTOCOLE	RS232 or USB error
	 * @retval #MORPHOERR_INTERNAL	Internal error
	 * @retval #MORPHOERR_USER	User error
	 */
	I ComReceive (
					UL		i_ul_Timeout,
					PUC*	io_ppuc_Data,
					PUL		o_pul_DataSize);

	/**
	 * This function closes the MorphoSmart&tm; communication link.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful
	 * @retval #MORPHOERR_CLOSE_COM	Can not close the MorphoSmart&tm; communication link
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_USER	User error
	 */
	I CloseDevice ( VOID );

#ifdef ENABLE_SECU_ILV

	/**
	 * This function is reserved for MorphoSmart&tm; S configured with "offered security protocol".
	 * We remind that with "offered security protocol" the MorphoSmart&tm; makes the communication
	 * answers safe on the host request. A safe frame includes a digital signature computed on:
	 * - the hash of the host request
	 * - an anti-replay random number (generated by the Host)
	 * - the MorphoSmart&tm; answer frame
	 *
	 * No encryption is performed.<br>
	 * All commands sent between OfferedSecuOpen() and OfferedSecuClose() have the security
	 * encapsulation. Otherwise, commands do not have the security encapsulation.<br>
	 * This function registers the callback functions in global variables. There is no secured
	 * protocol exchange.<br>
	 * On the first call of this function, the MorphoSmart&tm; certificate is read from
	 * MorphoSmart&tm; and is stored in a global variable.<br>
	 * During the following exchanges with MorphoSmart&tm; , the i_pf_CallbackVerifOfferedSecurity
	 * Signature verifies the digital signature of the frame with the MorphoSmart&tm; certificate.
	 * The MorphoSmart&tm; certificate must be checked. The i_pf_CallbackVerifCertif can do it but
	 * other possibility can be used: The i_pf_CallbackVerifOfferedSecuritySignature can
	 * systematically verify the MorphoSmart&tm; certificate.
	 *
	 * @param[in] i_pf_CallbackGenRandom
	 * - callback that generates random numbers. This callback is stored in a global variable
	 * and is used in the following exchanges with MorphoSmart&tm; for anti-replay random generation.
	 * @param[in] i_pf_CallbackVerifOfferedSecuritySignature
	 * - callback that verifies the asymmetric digital signature of the protocol frames.
	 * This callback is stored in a global variable and is used in the following exchanges with
	 * MorphoSmart&tm; to verify the MorphoSmart&tm; received frame.
	 * @param[in] i_pf_CallbackVerifCertif
	 * - callback that verifies the MorphoSmart&tm; certificate.
	*
	 * @retval #MORPHO_OK	Success.
	 * @retval #MORPHOERR_BADPARAMETER	A callback is set to NULL.
	 * @retval #MORPHOERR_SECU	Security error.
	 * @retval #MORPHOERR_CERTIF_INVALID	The certificate is not valid.
	 */
	I OfferedSecuOpen (
								T_MORPHO_CALLBACK_RAND						i_pf_CallbackGenRandom,
								T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN	i_pf_CallbackVerifOfferedSecuritySignature,
								T_MORPHO_CALLBACK_VERIF_CERTIF				i_pf_CallbackVerifCertif
								);

	//compatibility with MORPHO_SDK.dll Version 2.0
	/**
	 * OfferedSecuOpen (compatibility with MORPHO_SDK.dll Version 2.0)
	 * See OfferedSecuOpen
	 */
	I OfferedSecuOpen (
								T_MORPHO_CALLBACK_RAND						i_pf_CallbackGenRandom,
								T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN	i_pf_CallbackVerifOfferedSecuritySignature
								);

	/**
	 * This function is reserved for MorphoSmart&tm; S, configured with "offered security protocol".<br>
	 * This function releases all registered callbacks with OfferedSecuOpen() function.
	 * After this call, frames are not any more secured. The MorphoSmart&tm; certificate is not
	 * released, use C_MORPHO_Device::CloseDevice to do it.
	 *
	 * @retval #MORPHO_OK	Success.
	 */
	I OfferedSecuClose( VOID );

	/**
	 * This function is reserved for MorphoSmart&tm; S, configured with "tunneling protocol".<br>
	 * We remind that with "tunneling protocol" the MorphoSmart&tm; and the Host make all
	 * communication frames (request and responses) secure: Host and MorphoSmart&tm; exchange
	 * symmetric keys during a mutual authentication. Then all frames have an anti-replay
	 * counter, a signature and are encrypted.<br>
	 * All commands sent between TunnelingOpen() and TunnelingClose() have the security
	 * encapsulation.
	 *
	 * @param[in] i_pf_CallbackTripleDesSign
	 * - callback that implements triple DES EDE3 CBC signature algorithm.
	 * This algorithm is used for to insure integrity of the frames issued by the host.
	 * @param[in] i_pf_CallbackTripleDesCrypt
	 * - callback that implements triple DES EDE3 CBC encryption algorithm.
	 * This algorithm is used for to insure confidentiality of the frames issued by the host.
	 * @param[in] i_pf_CallbackTripleDesVerifSign
	 * - callback that that implements triple DES EDE3 CBC signature verification algorithm.
	 * This algorithm is used for to insure integrity of the frames issued by the MorphoSmart&tm;.
	 * @param[in] i_pf_CallbackTripleDesDecrypt
	 * - callback that implements triple DES EDE3 CBC encryption algorithm.
	 * This algorithm is used for to insure confidentiality of the frames issued by
	 * the MorphoSmart&tm;.
	 * @param[in] i_pf_CallbackTunnelingInit1
	 * - callback that performs the first step of the mutual authentication.
	 * The MorphoSmart&tm; certificate argument must be checked
	 * @param[in] i_pf_CallbackTunnelingInit2
	 * - callback that performs the second step of the mutual authentication.
	 * @param[in] i_ul_HostCertificateSize
	 * - host X509 certificate size.
	 * @param[in] i_puc_HostCertificateSize
	 * - host X509 certificate, coded in DER.
	 *
	 * @retval #MORPHO_OK	The matching was successful.
	 * @retval #MORPHOERR_BADPARAMETER	A callback is set to NULL.
	 * @retval #MORPHOERR_SECU	Security error.
	 */
	I TunnelingOpen (
					T_MORPHO_CALLBACK_TRIPLE_DES_SIGN		i_pf_CallbackTripleDesSign,
					T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT		i_pf_CallbackTripleDesCrypt,
					T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN	i_pf_CallbackTripleDesVerifSign,
					T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT	i_pf_CallbackTripleDesDecrypt,
					T_MORPHO_CALLBACK_TUNNELING_INIT1		i_pf_CallbackTunnelingInit1,
					T_MORPHO_CALLBACK_TUNNELING_INIT2		i_pf_CallbackTunnelingInit2,
					UL										i_ul_HostCertificateSize,
					PUC										i_puc_HostCertificateSize
					);

	/**
	 * This function is reserved for MorphoSmart&tm; S, configured with "tunneling protocol".
	 * All callbacks registered with TunnelingOpen() are released.
	 *
	 * @retval #MORPHO_OK	The matching was successful.
	*/
	I TunnelingClose ( VOID);

	/**
	 * This function reads any certificate that is part of MorphoSmart&tm; certification path
	 * (from MorphoSmart&tm; certificate till ROOT certificate).
	 *
	 * @param[in] i_uc_index
	 * - Certificate index to read. 0 is MorphoSmart&tm; certificate.
	 * N is certificate issuer of certificate N-1…
	 * @param[out] o_pul_CertifSize
	 * - X509 certificate size.
	 * @param[out] o_ppuc_Certif
	 * - X509 certificate encoded in DER.
	 *
	 * @retval #MORPHO_OK	The matching was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval
	 */
	I SecuReadCertificate (
								UC				i_uc_index,
								PUL				o_pul_CertifSize,
								PUC*			o_ppuc_Certif
								);

	/**
	 * This function stores a certificate in the MorphoSmart&tm;. This command must be
	 * used in complex PKI architectures where the MorphoSmart&tm; and the Host do not have
	 * the same issuer.
	 *
	 *  @attention The MorphoSmart&tm; can store 30 certificates in its certificate area, including Host System certificates, and its own certificate certification path (the deeper the MorphoSmart&tm; certification path is, the less place it remains for Host System certificates). Once there is no more room, the oldest Host System certificate is replaced.
	 *
	 * @param[in] i_ul_CertifSize
	 * - X509 certificate size.
	 * @param[in] i_puc_Certif
	 * - X509 certificate encoded in DER.
	 *
	 * @retval #MORPHO_OK	The matching was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I SecuStoCertif (
								UL			i_ul_CertifSize,
								PUC			i_puc_Certif
								);

	/**
	 * This function stores a PKCS#12 token in the MorphoSmart&tm;. The PKCS#12 token
	 * contains MorphoSmart&tm; private key, MorphoSmart&tm; certificate, and all certificates
	 * that belong to the certification path.<br>
	 * Note: When new keys are successfully loaded, all previous keys are discarded.
	 *
	 * @param[in] i_ul_TokenPKCS12Size
	 * - PKCS#12 token size.
	 * @param[in] i_puc_TokenPKCS12
	 * - X509 certificate encoded in DER.
	 * @param[in] i_i_AntiReplayCnt
	 * - Anti-replay counter. This counter ensures that an old PKCS#12 token will be not
	 * anymore loaded.
	*
	 * @retval #MORPHO_OK	The matching was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I SecuStoPkcs12 (
								UL			i_ul_TokenPKCS12Size,
								PUC			i_puc_TokenPKCS12,
								I			i_i_AntiReplayCnt
								);
#endif

	/**
	 * This function retrieves MorphoSmart&tm; configuration.
	 *
	 * @param[out] o_auc_SecuSerialNumber
	 * - MorphoSmart&tm; serial number.
	 * @param[out] o_puc_SecuConfig
	 * - binary mask that indicates security options managed by the MorphoSmart&tm;.
	 * - Binary mask is defined as follow:
	 *    - b8	b7	b6	b5	b4	b3	b2	b1	Biometric Subtype Mask
	 *    - Mask = 0x00	No security options.
	 *    - b1 = 1	The MorphoSmart&tm; uses the "tunneling" protocol.
	 *    - b2 = 1	The MorphoSmart&tm; uses the "offered security" protocol.
	 *    - b3 = 1	The MorphoSmart&tm; uses only templates with a digital signature (within an X9.84 envelop).
	 *    - b4 = 1	The MorphoSmart&tm; only accept signed firmware packages during retrofit.
	 *    - b5 = 1	The MorphoSmart&tm; can not return the matching score.
	 *    - b6-b7-b8	RFU
	 * - Note:
	 *    - b1 and b2 are exclusive
	 *    - If b4 is not set, the MorphoSmart&tm; accepts templates with or without a digital signature.
	 * @param[out] o_pus_SecuFar
	 * - Limit of the threshold accepted by the MorphoSmart&tm;. For example, if the value is
	 * set to #MORPHO_FAR_5, the MorphoSmart&tm; will accept a matching command with a more restrictive
	 * FAR (#MORPHO_FAR_6 …), but will reject a matching command with a less restrictive
	 * FAR (#MORPHO_FAR_4 …).
	 * @param[out] o_pul_SecuMinMSL
	 * - Minimum Multimodal Security Level (MorphoSmart&tm; FINGER VP only):
	 *    - #MULTIMODAL_SECURITY_STANDARD : Low False Multimodal Detection security level.
	 *    - #MULTIMODAL_SECURITY_MEDIUM : Medium False Multimodal Detection security level.
	 *    - #MULTIMODAL_SECURITY_HIGH : High False Multimodal Detection security level.
	 * @retval #MORPHO_OK	The matching was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */

	I GetSecuConfig (
								UC				o_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN+1],
								PUC				o_puc_SecuConfig,
								PT_MORPHO_FAR	o_pus_SecuFar,
								PUL				o_pul_SecuMinMSL
								);

	/**
	 * @see GetSecuConfig (kept for compatibility with MORPHO_SDK.dll Version 6.4)
	 */
	I GetSecuConfig (
								UC				o_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN+1],
								PUC				o_puc_SecuConfig,
								PT_MORPHO_FAR	o_pus_SecuFar
								);

	/**
	 * This function retrieves FFD logs.<br>
	 * Log data received from the terminal are formatted and concatenated into
	 * a output buffer separated by CR/LF.<br>
	 * This output buffer is sized according to the number of received events
	 * and should be released using the \ref C_MORPHO_Device::ReleaseFFDLogs method.
	 *
	 * @param[out] o_pul_loglength
	 * - Total log length
	 * @param[out] o_ppuc_log
	 * - Concatenated log lines
	 *
	 * @retval #MORPHO_OK	The retrieve was ok.
	 * @retval MSO_BAD_PARAMETER	Wrong device
	 * @retval COM_ERR_NOT_OPEN	Communication is not initiated
	 * @retval COM_ERR	Error during communication
	 * @retval COM_RECEIVE_ILV_INVALID	Unknown sent ILV
	 * @retval COM_STOP	Communication stopped
	 * @retval COM_ERROR_SYNCHRO_I	Unexpected returned ILV
	 */
	I GetFFDLogs ( PUL o_pul_loglength, PUC *o_ppuc_log );

	/**
	 * This function releases FFD log buffer allocated by \ref C_MORPHO_Device::GetFFDLogs method.
	 *
	 * @param[in] i_puc_log
	 * - Log line buffer to release
	 *
	 * @retval #MORPHO_OK	The release was ok.
	 */
	I ReleaseFFDLogs ( PUC i_puc_log );

	/**
	 * When the biometric terminal returns an internal error, the SDK returns
	 * #MORPHOERR_INTERNAL. GetInternalError() retrieves this internal error.
	 * This internal error may help to diagnose internal problems and should
	 * be sent to Morpho for further analysis.
	 *
	 * @retval Internal biometric terminal error.
	 */
	I GetInternalError ( VOID );

	/**
	 * This function returns a database instance.
	 *
	 * @cond MORPHO_IMPLEMENTATION_NOTE
	 * @note - @ref IMPLEMENTATION_NOTE_GETDATABASE
	 * @endcond
	 *
	 * @param[in] i_uc_DatabaseIndex
	 * - Database index (first index is 0).
	 * @param[in] i_pc_default_path
	 * - Must be NULL. (Reserved for future use).
	 * @param[out] o_x_dataBase
	 * - Database instance. See C_MORPHO_Database class.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BASE_NOT_FOUND	Database file is not present on the device, or on the
	 * 									specified path. Retrieved object can still be used to
	 * 									create a new database.
	 * @retval #MORPHOERR_BADPARAMETER	Index is higher than (#MORPHO_NB_DATABASE_MAX-1).
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I GetDatabase( UC i_uc_DatabaseIndex, PC i_pc_default_path, C_MORPHO_Database & o_x_dataBase );

	/*!
	 * @see GetDatabase (kept for compatibility with MORPHO_SDK.dll Version 4.3.3.0)
	 */
	I GetDatabase( UC i_uc_DatabaseIndex, C_MORPHO_Database & o_x_dataBase );

	/**
	 * This function captures a live finger and checks if it matches with the user referred
	 * to. The maximum number of reference templates is 20.
	 *
	 * @cond MORPHO_IMPLEMENTATION_NOTE
	 * @note - @ref IMPLEMENTATION_NOTE_VERIFY
	 * @endcond
	 *
	 * @param[in] i_us_Timeout
	 * - Finger detection timeout in seconds. Its value must be between 0 and 0xFFFF.
	 * 0 corresponds to an infinite timeout.
	 *
	 * @param[in] i_us_FAR
	 * - This parameter specifies how tight the matching threshold is.
	 * Morpho recommends #MORPHO_FAR_5 (see #T_MORPHO_FAR).
	 *
	 * @param[in] i_x_TemplateList
	 * - List of candidate's templates to match. The number of templates must be
	 * less or equal to 20. The template format should be #MORPHO_PK_COMP or #MORPHO_PK_FVP (MorphoSmart&tm; FINGER VP only).<br>
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
	 * - Only a secure MorphoSmart&tm; accept the X984 biometric token and verify the integrity.
	 *
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements. This mask describes the
	 * asynchronous status events that will trig the callback function.
	 * Set this parameter to 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want
	 * to receive the command status (move finger more left, remove finger,...) and
	 * low-resolution images.
	 *
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of asynchronous status. NULL if not used.
	 *
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 *
	 * @param[out] o_pul_MatchingScore
	 * - Contains the result matching score. NULL if not used.
	 * For security reason, the secure MorphoSmart&tm; cannot export the matching score
	 * because a "rogue" application can mount an "hillclimbing" attack by sequentially randomly modifying a sample and retaining only the changes that produce an increase in the returned score.
	 *
	 * @param[out] o_puc_ExportNumPk
	 * - Contains the result matching PK number. NULL if not used.
	 * If o_puc_ExportNumPk is not NULL, if a false or a too moist finger is detected
	 * (for MSOxx1 only), or if the finger is not authenticated, the returned matching PK number
	 * value is 0xFF.
	 *
	 * @param[in] i_i_CoderChoice
	 * - contains the biometric coder to use (#MORPHO_MSO_V9_CODER
	 * or #MORPHO_MSO_V9_JUV_CODER). Morpho recommends using #MORPHO_MSO_V9_CODER.
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
	 * @retval #MORPHOERR_INTERNAL 							An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER						The matching threshold value or timeout value is out of range or there is no biometric input data.
	 * @retval #MORPHOERR_INVALID_TEMPLATE					The reference template is not valid: bad identifier, corrupted minutiae.
	 * @retval #MORPHOERR_TIMEOUT							The finger detection timeout has expired.
	 * @retval #MORPHOERR_NO_HIT								The matching returned a No Hit.
	 * @retval #MORPHOERR_CMDE_ABORTED						Command is canceled.
	 * @retval #MORPHOERR_NO_REGISTERED_TEMPLATE				Template list is empty.
	 * @retval #MORPHOERR_MIXED_TEMPLATE						Template list contains templates with mixed formats.
	 * @retval #MORPHOERR_MEMORY_PC							Not enough memory on the PC.
	 * @retval #MORPHOERR_PROTOCOLE							Communication protocol error.
	 * @retval #MORPHOERR_CORRUPTED_CLASS					Class has been corrupted.
	 * @retval #MORPHOERR_FFD								False Finger Detected.
	 * @retval #MORPHOERR_MOIST_FINGER						The finger can be too moist or the scanner is wet.
	 * @retval #MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH		Fingerprint template cannot be matched in high security level (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_LICENSE_MISSING					A required license is missing (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN	Finger was misplaced or has been withdrawn from sensor during acquisition (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_FFD_FINGER_MISPLACED				Finger was misplaced during acquisition (MorphoSmart&tm; MSO 3x1 FFD only).
	 */
	I Verify(	US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				C_MORPHO_TemplateList &		i_x_TemplateList,
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
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I Verify(	US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				C_MORPHO_TemplateList &		i_x_TemplateList,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_ExportNumPk,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UL							i_ul_MatchingStrategy,
				UC							i_uc_databaseIndex
			);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 4.3.3.0)
	 */
	I Verify(	US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				C_MORPHO_TemplateList &		i_x_TemplateList,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_ExportNumPk,
				I							i_i_CoderChoice,
				UL							i_ul_DetecModeChoice
				);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 4.2.3.2)
	 */
	I Verify(		US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				C_MORPHO_TemplateList &		i_x_TemplateList,
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
	I Verify(		US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				C_MORPHO_TemplateList &		i_x_TemplateList,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore,
				PUC							o_puc_ExportNumPk);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 3.1.2.0)
	 */
	I Verify(		US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				C_MORPHO_TemplateList &		i_x_TemplateList,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				PUL							o_pul_MatchingScore);

	/**
	 * @see Verify (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I Verify(		US							i_us_Timeout,
				T_MORPHO_FAR				i_us_FAR,
				C_MORPHO_TemplateList &		i_x_TemplateList,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument);

	/**
	 * This function checks if one template of the reference template list matches
	 * with the unique template of the search template list.<br>
	 * The maximum number of reference templates is 20.<br>
	 * The maximum number of search template is 1.
	 *
	 * @param[in] i_us_FAR
	 * - This parameter specifies how tight the matching threshold is.
	 * Morpho recommends #MORPHO_FAR_5 (see paragraph #T_MORPHO_FAR).
	 * @param[in] i_x_TemplateList1
	 * - Search template list. This list must include one unique template.
	 * The template format should be #MORPHO_PK_COMP or #MORPHO_PK_FVP_MATCH (MorphoSmart&tm; FINGER VP only).<br>
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
	 * - Only a secure MorphoSmart&tm; accept the X984 biometric token and verify the integrity.
	 *
	 * @param[in] i_x_TemplateList2
	 * - Reference templates list. The number of templates must be lower than 20.
	 * The template format should be #MORPHO_PK_COMP or #MORPHO_PK_FVP (MorphoSmart&tm; FINGER VP only).<br>
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
	 * - Only secure MorphoSmart&tm; accept the X984 biometric token and verify the integrity.
	 *
	 * @param[out] o_pul_MatchingScore
	 * - Contains the result matching score. NULL if not used.
	 * For security reason, the secure MorphoSmart&tm; cannot export the matching score
	 * because a "rogue" application can mount a "hillclimbing" attack by sequentially
	 * randomly modifying a sample and retaining only the changes that produce an increase
	 * in the returned score.
	 *
	 * @retval #MORPHO_OK	The function was successful.
	 * @retval #MORPHOERR_INTERNAL 	An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	The matching threshold value or timeout value is out of range or there is no biometric input data.
	 * @retval #MORPHOERR_INVALID_TEMPLATE	One template is not valid: bad identifier, corrupted minutiae.
	 * @retval #MORPHOERR_NO_HIT	There are no common fingers between the two users.
	 * @retval #MORPHOERR_CMDE_ABORTED	Command is canceled.
	 * @retval #MORPHOERR_NO_REGISTERED_TEMPLATE	At least one template list is empty.
	 * @retval #MORPHOERR_MIXED_TEMPLATE	Template list contains templates with mixed formats.
	 * @retval #MORPHOERR_MEMORY_PC	Not enough memory on the PC.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_LICENSE_MISSING	A required license is missing (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_INVALID_PK_FORMAT	Invalid template format
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I VerifyMatch(	T_MORPHO_FAR			i_us_FAR,
					C_MORPHO_TemplateList &	i_x_TemplateList1,
					C_MORPHO_TemplateList &	i_x_TemplateList2,
					PUL						o_pul_MatchingScore);

	/**
	 * @see VerifyMatch (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I VerifyMatch(	T_MORPHO_FAR			i_us_FAR,
					C_MORPHO_TemplateList &	i_x_TemplateList1,
					C_MORPHO_TemplateList &	i_x_TemplateList2);


	/**
	 * This function captures live fingers and extracts their templates. The number of fingers
	 * can be specified. The template is calculated after one or three fingerprint image
	 * acquisitions (the user has to put each finger 1 or 3 times on the sensor).<br>
	 * We strongly recommend getting 3 images for enrollment purpose, and 1 image for
	 * verification purpose. An enrollment based on 3 images will increase the system accuracy.
	 * To obtain the best accuracy, it is strongly recommended to use the fore,
	 * the thumb or the middle fingers.<br>
	 * Fingerprint quality and advanced security levels compatibility (MorphoSmart&tm; FINGER VP only)
	 * are only available if export of templates is activated.<br>
	 * - C_MORPHO_TemplateList::GetFVPTemplate must be called to retreive multimodal template.<br>
	 * - C_MORPHO_TemplateList::GetTemplate must be called to retreive fingerprint template.<br>
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
	 * @param[in] i_uc_FingerNumber
	 * - The number of fingers to enroll. This function can enroll 1 or 2 fingers.
	 *     - Set this value to 1 to enroll 1 finger per user.
	 *     - Set this value to 2 to enroll 2 fingers per user.
	 *
	 * @param[in] i_x_TemplateType
	 * - Indicates the template acquisition format. The template format should be #MORPHO_PK_COMP.
	 * The other template formats:
	 *     - #MORPHO_PK_COMP_NORM
	 *     - #MORPHO_PK_MAT
	 *     - #MORPHO_PK_MAT_NORM
	 *     - #MORPHO_PK_ANSI_378
	 *     - #MORPHO_PK_MINEX_A
	 *     - #MORPHO_PK_ISO_FMR
	 *     - #MORPHO_PK_ISO_FMC_NS
	 *     - #MORPHO_PK_ISO_FMC_CS
	 *     - #MORPHO_PK_PKLITE (only available on MorphoSmart&tm; CBM-E3)<br>
	 * are also supported, but Morpho recommends using them only for compatibility with existing systems or specific usage.<br>
	 * On MorphoSmart&tm; FINGER VP, put \ref T_MORPHO_TYPE_TEMPLATE::MORPHO_NO_PK_FP "MORPHO_NO_PK_FP".
	 *
	 * @param[in] i_x_TemplateFVPType
	 * - Indicates the multimodal template acquisition format. The template format should be #MORPHO_PK_FVP.
	 *
	 * @param[in] i_us_MaxSizeTemplate
	 * - Defines the format of the exported minutiae.
	 *    - Set this value to 0 to exclude the calculated minutiae from the reply.
	 *    - Set this value to 1 to export the minutiae with its default size.
	 * - For #MORPHO_PK_COMP only, this value can be set from 170 to 255 to compress the template.<br>
	 * The buffer exported will have the size you have chosen at the most, but could be less.
	 *
	 * @param[in] i_uc_enrolType
	 * - Specifies the number of fingerprint image acquisitions. Allowed values are 0, 1 and 3.<br>
	 * We strongly recommend setting this value to 0 (default value) or 3 for enrollment purpose
	 * to increase the system performances: in this case, the template is generated from a
	 * consolidation calculation of three consecutive acquisitions of the same fingerprint.<br>
	 * It is also possible to set this value to 1 for verification purpose. In this case,
	 * it is not possible to save the record in the internal database: in this case,
	 * the template is generated from one single fingerprint acquisition.<br>
	 * On MorphoSmart&tm; FINGER VP, the value 1 is deprecated.
	 *
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements.
	 * This mask describes the asynchronous status events that will trig the callback function.
	 * Set to 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want to
	 * receivecommand status (move finger up...) and low-resolution images.
	 *
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of asynchronous status. NULL if not used.
	 *
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 *
	 * @param[out] o_x_Template
	 * - Contains the calculated templates. o_x_Template is reset before being used.
	 * When #MORPHO_X984_SIGNED_TEMPLATE is used, the X984_SIGNED_TEMPLATE can be obtained
	 * with C_MORPHO_TemplateList::GetX984 and the raw templates with
	 * C_MORPHO_TemplateList::GetTemplate.
	 *
	 * @param[in] i_x_typEnvelop
	 * - #MORPHO_RAW_TEMPLATE means that we want to generate raw templates without signature
	 * #MORPHO_X984_SIGNED_TEMPLATE means that we want to generate templates with a digital
	 * signature and an X9.84 envelop.
	 *
	 * @param[in] i_i_ApplicationDataLen
	 * - Length of i_puc_ApplicationData buffer. Set to 0 if not used.
	 *
	 * @param[in] i_puc_ApplicationData
	 * - Application buffer that will be added to the X9.84 envelop. This data is taken into
	 * account only when i_x_typEnvelop parameter is set to #MORPHO_X984_SIGNED_TEMPLATE. NULL
	 * if not used. This parameter is used only by your application.
	 *
	 * @param[in] i_uc_LatentDetection
	 * - Set to a #LATENT_DETECT_DISABLE value to disable the latent detection mechanism: the default settings
	 * - Set to a #LATENT_DETECT_ENABLE value to enable the latent detection mechanism.
	 * This should be enabled when capturing the template for verification performed on
	 * the PC (i.e. when i_uc_enrolType is set to 1).
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
	 * @param[in] i_uc_SaveIndexImage
	 * - Reserved to Morpho use only, please specify the default value 0x00.
	 *
	 * @param[in] i_px_MocParameters
	 * - Structure defining a match on Smart Card Template. NULL if not used.
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
	 *	- can vary between 2 and 255 for #MORPHO_COMPRESS_WSQ algorithm, usual recommended value is 15.
	 *
	 * @note In parameter i_ul_DetectModeChoice:
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_ENROLL_DETECT_MODE cannot be used together.
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE cannot be used together.
	 *
	 * @retval #MORPHO_OK									The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL							An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER						One or more input parameters are out of range.
	 * @retval #MORPHOERR_TIMEOUT							The finger detection timeout has expired.
	 * @retval #MORPHOERR_MEMORY_PC							Not enough memory on the PC.
	 * @retval #MORPHOERR_SAME_FINGER						User gave the same finger twice.
	 * @retval #MORPHOERR_PROTOCOLE							Communication protocol error.
	 * @retval #MORPHOERR_CMDE_ABORTED						C_MORPHO_Device::CancelLiveAcquisition has been called before capture has been completed.
	 * @retval #MORPHOERR_INVALID_PK_FORMAT					Invalid template format
	 * @retval #MORPHOERR_CORRUPTED_CLASS					Class has been corrupted.
	 * @retval #MORPHOERR_FFD								False Finger Detected
	 * @retval #MORPHOERR_MOIST_FINGER						The finger can be too moist or the scanner is wet.
	 * @retval #MORPHOERR_LICENSE_MISSING					A required license is missing (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH	Failed to make a multimodal template compatible with advanced security levels (MorphoSmart&tm; FINGER VP only).
	 * @retval #MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY	Failed to capture the fingerprint with a quality greater than or equal to the specified threshold.
	 * @retval #MORPHOERR_KEY_NOT_FOUND						The specified key is missing, unable to encrypt biometrics data.
	 */

	I Capture(	US							i_us_Timeout,
				UC							i_uc_AcquisitionThreshold,
				UC							i_uc_AdvancedSecurityLevelsRequired,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				T_MORPHO_FVP_TYPE_TEMPLATE	i_x_TemplateFVPType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UC							i_uc_SaveIndexImage,
				PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters,
				T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
				UC							i_uc_CompressRate
				);

	I Capture(	US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				T_MORPHO_FVP_TYPE_TEMPLATE	i_x_TemplateFVPType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UC							i_uc_SaveIndexImage,
				PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters,
				T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
				UC							i_uc_CompressRate
				);


	I Capture(	US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UC							i_uc_SaveIndexImage,
				PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters,
				T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
				UC							i_uc_CompressRate
				);


	I Capture(
				US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UC							i_uc_SaveIndexImage,
				PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters
				);

	/**
	 * @see Capture (kept for compatibility with MORPHO_SDK.dll Version 5.1.3.0)
	 */
	I Capture(
				US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice,
				UC							i_uc_SaveIndexImage
				);

	/**
	 * @see Capture (kept for compatibility with MORPHO_SDK.dll Version 4.3.3.0)
	 */
	I Capture(
				US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection,
				I							i_i_CoderChoice,
				UL							i_ul_DetectModeChoice
				);

	/**
	 * @see Capture (kept for compatibility with MORPHO_SDK.dll Version 4.2.3.2)
	 */
	I Capture(
				US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection,
				I							i_i_CoderChoice
				);

	/**
	 * @see Capture (kept for compatibility with MORPHO_SDK.dll Version 4.0.3.0)
	 */
	I Capture(	US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen,
				PUC							i_puc_ApplicationData,
				UC							i_uc_LatentDetection
				);

	/**
	 * @see Capture (kept for compatibility with MORPHO_SDK.dll Version 2.0)
	 */
	I Capture(	US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
				I							i_i_ApplicationDataLen=0,
				PUC							i_puc_ApplicationData=NULL);

	/**
	 * @see Capture (kept for compatibility with MORPHO_SDK.dll Version 1.2)
	 */
	I Capture(	US							i_us_Timeout,
				UC							i_uc_FingerNumber,
				T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				US							i_us_MaxSizeTemplate,
				UC							i_uc_enrolType,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				C_MORPHO_TemplateList &		o_x_Template);

	/**
	 * This function converts a template buffer to another template format.
	 * TODO: available formats for conversion
	 *
	 * @param[in] i_puc_input_template
	 * - Template buffer to be converted.
	 *
	 * @param[in] i_ul_input_template_size
	 * - Size, in bytes, of the input template buffer.
	 *
	 * @param[in] i_x_input_format
	 * - Template format of the input buffer.
	 *
	 * @param[in] i_x_output_format
	 * - Desired output template format.
	 *
	 * @param[out] o_ppuc_output_template
	 * - If the conversion is successful, will point to the converted template buffer. When this buffer is not used
	 * anymore, release it through the @ref ReleaseTemplate method. If the conversion has failed, this will point to
	 * NULL.
	 *
	 * @param[out] o_pul_output_template_size
	 * - Size, in bytes, of the converted buffer if successful.
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL			An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER		One or more input parameters are out of range.
	 * @retval #MORPHOERR_MEMORY_PC			Not enough memory on the PC.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I ConvertTemplate(
				PUC						i_puc_input_template,
				UL						i_ul_input_template_size,
				T_MORPHO_TYPE_TEMPLATE	i_x_input_format,
				T_MORPHO_TYPE_TEMPLATE	i_x_output_format,
				PUC *					o_ppuc_output_template,
				PUL						o_pul_output_template_size
				);

	/**
	 * This function releases memory allocated by a previous successful call to @ref Convert method.
	 *
	 * @param[in,out] io_ppuc_template
	 * - Pointer to the converted buffer returned by a successful call to @ref Convert method.
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL			An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER		One or more input parameters are out of range.
	 * @retval #MORPHOERR_MEMORY_PC			Not enough memory on the PC.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I ReleaseTemplate(
				PUC *		io_ppuc_template
				);

	/**
	 * This function captures live fingers and extracts its image in full resolution.
	 * To obtain the best accuracy, it is strongly recommended to use the fore,
	 * the thumb or the middle fingers. C_MORPHO_Device::GetImage() is a fingerprints acquisition.<br>
	 * This function performs the following process:
	 * -	Lighting of the MorphoSmart&tm; sensor : the MorphoSmart&tm; terminal starts the reception of images which come from the MorphoSmart&tm; sensor,
	 * -	Searching of a fingerprint within each received image,
	 * -	Evaluation of the fingerprint quality until obtaining a sufficient quality,
	 * -	False finger detection (if applicable, i.e. for MSOxx1),
	 * -	Emission of the image that contains the sufficient fingerprint quality.
	 *
	 * For a well use of this function, you had better to set the timeout in order that the user
	 * could position his finger: we recommend a timeout value between 15 and 60 seconds,
	 * a 30 seconds timeout would be ideal.
	 *
	 * @param[in] i_us_Timeout
	 * - Finger detection timeout in seconds. Its value must be between 0 and 0xFFFF.
	 * 0 corresponds to an infinite timeout.
	 *
	 * @param[in] i_uc_AcquisitionThreshold
	 * - Minimum value of fingerprint quality. This value can be 0 (strongly recommended) or
	 * any value between 20 and 100.
	 *
	 * @param[in] i_x_CompressAlgo
	 * - Compression algorithm to be used to compress the fingerprint image. Available algorithms are:
	 *	- #MORPHO_NO_COMPRESS
	 *	- #MORPHO_COMPRESS_V1
	 *	- #MORPHO_COMPRESS_WSQ
	 *
	 * @param[in] i_uc_CompressRate
	 * - Compression rate used by the fingerprint image compression algorithm:
	 *	- useless for #MORPHO_NO_COMPRESS and #MORPHO_COMPRESS_V1 algorithms (must be set to 0).
	 *	- can vary between 2 and 255 for #MORPHO_COMPRESS_WSQ algorithm, usual recommended value is 15.
	 *
	 * @param[in] i_ul_CallbackCmd
	 * - Binary mask with #T_MORPHO_CALLBACK_COMMAND elements.
	 * This mask describes the asynchronous status events that will trig the callback function.
	 * 0 if you do not want any asynchronous status to be received.
	 * For example #MORPHO_CALLBACK_COMMAND_CMD | #MORPHO_CALLBACK_IMAGE_CMD means we want
	 * to receive command status (move finger up...) and low-resolution images.
	 *
	 * @param[in] i_pf_Callback
	 * - User function called on the reception of asynchronous status. NULL if not used.
	 *
	 * @param[in] i_pv_CallbackArgument
	 * - Argument that will be passed to the callback function.
	 *
	 * @param[out] o_px_Image
	 * - The image is returned in full resolution.
	 *
	 * @param[in] i_uc_DetectModeChoice
	 * - Bitmask of the following:
	 *     - #MORPHO_VERIF_DETECT_MODE: more permissive mode than default; MorphoSmart&tm; detects more easily finger presence, but might issue lower quality templates.
	 *     - #MORPHO_ENROLL_DETECT_MODE: strongest detection mode (default mode).
	 *     - #MORPHO_WAKEUP_LED_OFF: (only available on MorphoSmart&tm; MSO FFD) leds are turned off while waiting for a finger (impedance wakeup).
	 *     - #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE: (not available on MorphoSmart&tm; CBM-V) force the finger to cover the top of the capture area to increase quality.
	 *     - #MORPHO_WAKEUP_LED_ON: (only available on MorphoSmart&tm; FINGER VP) leds are turned on while waiting for a finger.
	 *
	 * @param[in] i_uc_LatentDetection
	 * - Set to a #LATENT_DETECT_DISABLE value to disable the latent detection mechanism: the default settings
	 * - Set to a #LATENT_DETECT_ENABLE value to enable the latent detection mechanism.
	 * This should be enabled when capturing the template for verification performed on
	 * the PC (i.e. when i_uc_enrolType is set to 1).
	 *
	 * @note In parameter i_ul_DetectModeChoice:
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_ENROLL_DETECT_MODE cannot be used together.
	 * - #MORPHO_VERIF_DETECT_MODE and #MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE cannot be used together.
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INTERNAL			An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER		One or more input parameters are out of range.
	 * @retval #MORPHOERR_TIMEOUT			The finger detection timeout has expired.
	 * @retval #MORPHOERR_MEMORY_PC			Not enough memory on the PC.
	 * @retval #MORPHOERR_PROTOCOLE			Communication protocol error.
	 * @retval #MORPHOERR_BAD_COMPRESSION	Invalid compression type.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_FFD				False Finger Detected.
	 * @retval #MORPHOERR_MOIST_FINGER		The finger can be too moist or the scanner is wet.
	 * @retval #MORPHOERR_LICENSE_MISSING	A required license is missing (MorphoSmart&tm; FINGER VP only).
	 */
	I GetImage(
							US							i_us_Timeout,
							UC							i_uc_AcquisitionThreshold,
							T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
							UC							i_uc_CompressRate,
							UL							i_ul_CallbackCmd,
							T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
							PVOID						i_pv_CallbackArgument,
							T_MORPHO_IMAGE				*o_px_Image,
							UC							i_uc_DetectModeChoice,
							UC							i_uc_LatentDetection
							);
/**
	 * @see GetImage (kept for compatibility with MORPHO_SDK.dll Version 5.0.0.0)
	 */

	I GetImage(
							US							i_us_Timeout,
							T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
							UC							i_uc_CompressRate,
							UL							i_ul_CallbackCmd,
							T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
							PVOID						i_pv_CallbackArgument,
							T_MORPHO_IMAGE				*o_px_Image,
							UC							i_uc_DetectModeChoice,
							UC							i_uc_LatentDetection
							);
	/**
	 * @see GetImage (kept for compatibility with MORPHO_SDK.dll Version 5.0.0.0)
	 */
	I GetImage(	US			i_us_Timeout,
							T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
							UL							i_ul_CallbackCmd,
							T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
							PVOID						i_pv_CallbackArgument,
							T_MORPHO_IMAGE				*o_px_Image,
							UC							i_uc_DetectModeChoice,
							UC							i_uc_LatentDetection
							);

	/**
	 * @see GetImage (kept for compatibility with MORPHO_SDK.dll Version 4.3.3.0)
	 */
	I GetImage(
				US							i_us_Timeout,
				T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				T_MORPHO_IMAGE				*o_px_Image,
				UC							i_uc_DetectModeChoice);

	/**
	 * @see GetImage (kept for compatibility with MORPHO_SDK.dll Version 4.0.3.0)
	 */
	I GetImage(	US							i_us_Timeout,
				T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
				UL							i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				PVOID						i_pv_CallbackArgument,
				T_MORPHO_IMAGE				*o_px_Image);

	/**
	 * For Morpho internal use only:
	 * This function reconstructs the new PKFVP from the Morphokit: PKFVP + PKCOMP.
	 *
	 * @param[in] i_x_TemplateListFVP
	 * - Reference templates list. The template format should be #MORPHO_PK_FVP.
	 * @param[in] i_x_TemplateListPKcomp
	 * - Reference templates list. The template format should be #MORPHO_PK_COMP.
	 * @param[out] o_x_TemplateListFVP
	 * - Reference templates list. The new output template format should be #MORPHO_PK_FVP.
	 */
	I BuildPkFVP(	C_MORPHO_TemplateList &	i_x_TemplateListFVP,
					C_MORPHO_TemplateList &	i_x_TemplateListPKcomp,
					C_MORPHO_TemplateList &	o_x_TemplateListFVP);


	/**
	 * This function returns a descriptor in the form of a string.
	 *
	 * @param[in] i_uc_DescFormat
	 * - Required descriptor. Available descriptors are:
	 *	- #BINDESC_VERSION
	 *	- #BINDESC_MAX_USER
	 *	- #BINDESC_MAX_DB
	 *	- #BINDESC_SOFT
	 *	- #BINDESC_FLASH_SIZE
	 *	- #BINDESC_PRODUCT_NAME
	 *	- #BINDESC_PID
	 *	- #BINDESC_SN
	 *	- #BINDESC_OEM_PID
	 *	- #BINDESC_OEM_SN
	 *	- #BINDESC_SENSOR_ID
	 *	- #BINDESC_SENSOR_SN
	 *	- #BINDESC_LICENSES
	 *	- #BINDESC_CUSTOM_DESCRIPTOR
	 * @param[out] o_pc_BinDesc
	 * - String buffer that will contain the required descriptor.
	 * @param[out] io_pul_SizeOfBinDesc
	 * - Size of string buffer.
	 *
 	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_CLASS		The class has been destroyed.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER		String buffer is invalid or too small.
	 */
	I GetDescriptorBin(   UC i_uc_DescFormat,
						  PC o_pc_BinDesc,
						  PUL io_pul_SizeOfBinDesc);

	/**
	 * This function returns a descriptor in the form of an integer.
	 *
	 * @param[in] i_uc_DescFormat
	 * - Required descriptor. Available descriptors are:
	 *	- #BINDESC_MAX_USER
	 *	- #BINDESC_MAX_DB
	 *	- #BINDESC_FLASH_SIZE
	 * @param[out] o_pul_BinDesc
	 * - Integer that will contain the required descriptor value.
	 *
 	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_CLASS		The class has been destroyed.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER		o_pul_BinDesc is an invalid pointer.
	 */
	I GetDescriptorBin (	UC  i_uc_DescFormat,
							PUL o_pul_BinDesc);

	/**
	 * This function is dedicated to multimodal template conversion with MorphoSmart&tm; FINGER VP.
	 * It converts an out of date multimodal template generated under FVP v01.03 or older firmware releases to a template
	 * compatible with FVP v01.04 and later firmware releases.
	 *
	 * @param[in] i_x_InTemplateListpk
	 * - Reference templates list containing the multimodal template to convert.
	 * @param[out] o_x_OutTemplateListpk
	 * - Reference templates list containing the converted multimodal template compatible with the latest firmware release.
	 */
	I ConvertPkFVP(	C_MORPHO_TemplateList &	 	i_x_InTemplateListpk,
					C_MORPHO_TemplateList &	 	o_x_OutTemplateListpk );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I SetQualityThreshold ( UL	i_ul_quality);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetQualityThreshold ( PUL	o_pul_quality);

	/**
	 * This function cancels a finger acquisition.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I CancelLiveAcquisition ( VOID );

	/**
	 * This function sets the security level for finger acquisition functions.
	 * This level can be retrieved at any moment with #GetSecurityLevel.
	 *
	 * @param[in] i_i_SecurityLevel
	 * - Possible values:
	 *	- MorphoSmart&tm; MSO 3x1 FFD:
	 *		- #FFD_SECURITY_LEVEL_DEFAULT_HOST
	 *		- #FFD_SECURITY_LEVEL_LOW_HOST
	 *		- #FFD_SECURITY_LEVEL_MEDIUM_HOST
	 *		- #FFD_SECURITY_LEVEL_HIGH_HOST
	 *	- MorphoSmart&tm; CBM-E2(since v13.01):
	 *		- #FFD_SECURITY_LEVEL_DEFAULT_HOST
	 *		- #FFD_SECURITY_LEVEL_LOW_HOST
	 *		- #FFD_SECURITY_LEVEL_MEDIUM_HOST
	 *		- #FFD_SECURITY_LEVEL_HIGH_HOST
	 *		- #FFD_SECURITY_LEVEL_NONE_HOST
	 *	- MorphoSmart&tm; FINGER VP:
	 *		- #MULTIMODAL_SECURITY_STANDARD
	 *		- #MULTIMODAL_SECURITY_MEDIUM
	 *		- #MULTIMODAL_SECURITY_HIGH
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_CLASS		The class has been destroyed
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I SetSecurityLevel ( I	i_i_SecurityLevel );

	/**
	 * @see SetSecurityLevel (kept for compatibility with MORPHO_SDK.dll Version 6.3.0.0)
	 */
	I Set_FFD_SecurityLevel ( I	i_i_FFD_SecurityLevel );

	/**
	 * This function returns the security level set with #SetSecurityLevel.
	 *
	 * @param[out] o_pi_SecurityLevel
	 * - Possible values:
	 *	- MorphoSmart&tm; MSO 3x1 FFD:
	 *		- #FFD_SECURITY_LEVEL_LOW_HOST
	 *		- #FFD_SECURITY_LEVEL_MEDIUM_HOST
	 *		- #FFD_SECURITY_LEVEL_HIGH_HOST
	 *	- MorphoSmart&tm; CBM-E2(since v13.01):
	 *		- #FFD_SECURITY_LEVEL_LOW_HOST
	 *		- #FFD_SECURITY_LEVEL_MEDIUM_HOST
	 *		- #FFD_SECURITY_LEVEL_HIGH_HOST
	 *		- #FFD_SECURITY_LEVEL_NONE_HOST
	 *	- MorphoSmart&tm; FINGER VP:
	 *		- #MULTIMODAL_SECURITY_STANDARD
	 *		- #MULTIMODAL_SECURITY_MEDIUM
	 *		- #MULTIMODAL_SECURITY_HIGH
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_CLASS		The class has been destroyed
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_BADPARAMETER		o_pi_SecurityLevel is an invalid pointer.
	 */
	I GetSecurityLevel ( PI	o_pi_SecurityLevel );

	/**
	 * @see GetSecurityLevel (kept for compatibility with MORPHO_SDK.dll Version 6.3.0.0)
	 */
	I Get_FFD_SecurityLevel ( PI	o_pi_FFD_SecurityLevel );


	/*
	 * @todo Not Documented
	 */
	I LoadMasterKeys (	PUC		i_puc_MasterKey1,
						PUC		i_puc_MasterKey3);

	/**
	 * This function sends to the MorphoSmart&tm; the parameters to define a Match on Smart Card
	 *
	 * @param[in] i_px_MocApduParameters
	 * - the parameters  for the MOC APDU
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_CLASS	The class has been destroyed
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I SetMocApdu (
		PT_MORPHO_MOC_APDU_PARAMETERS i_px_MocApduParameters
		);

	/**
	 * This function loads a new Key in the MorphoSmart&tm;, this key is used to ciffer the Template passed to the Smart Card in a Match on Card process
	 *	It uses the compatible key load solution
	 * @param[in] i_puc_Kenc_Ciffered_by_KencTrans
	 * - the key Kenc ciffered by KencTrans used to ciffer the Template
	 * @param[in] i_ul_Kenc_Ciffered_by_KencTrans_Len
	 * - the previous parameter lenght
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_CLASS	The class has been destroyed
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I LoadMocKey
	(
			PUC		i_puc_Kenc_Ciffered_by_KencTrans,
			UL		i_ul_Kenc_Ciffered_by_KencTrans_Len
	);

	/**
	 * This function loads a new Key in the MorphoSmart&tm;, this key is used to ciffer the Template passed to the Smart Card in a Match on Card process
	 *	It uses the secured key load solution
	 *
	 * @param[in] i_puc_Kenc_Ciffered_by_Certificate
	 * - Defines the Kenc Ciffered by the MSO Certificate.
	 * @param[in] i_ul_Kenc_Ciffered_by_Certificate_Len
	 * - the previous parameter lenght.
	 *
	 * @param[in] i_puc_Kenc_Ciffered_by_Certificate_Signature
	 * - Defines the Signature of the previous parameter, the Kenc Ciffered by the MSO Certificate.
	 * @param[in] i_ul_Kenc_Ciffered_by_Certificate_Signature_Len
	 * - the previous parameter lenght.
	 *
	 * @param[in] i_puc_HostCertificate
	 * - The host certificate encoded in DER format used for the signature.
	 * @param[in] i_ul_HostCertificateSize
	 * - the previous parameter lenght.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_INVALID_CLASS	The class has been destroyed
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I LoadMocKey
	(
			PUC		i_puc_Kenc_Ciffered_by_Certificate,
			UL		i_ul_Kenc_Ciffered_by_Certificate_Len,
			PUC		i_puc_Kenc_Ciffered_by_Certificate_Signature,
			UL		i_ul_Kenc_Ciffered_by_Certificate_Signature_Len,
			PUC		i_puc_HostCertificate,
			UL		i_ul_HostCertificateSize
	);

	/**
	 * This function retrieves the KCV of a key saved in the MorphoSmart&tm;.
	 *
	 * @param[in] i_uc_KeyID		Identifier of the key:
	 *								- @ref ID_KENC
	 *								- @ref ID_KSECRET
	 *								- @ref ID_KS
	 * @param[in] i_puc_SecretID	Identifier of the locking secret when @b i_uc_KeyID =
	 *								@ref ID_KSECRET (set to NULL otherwise).
	 * @param[out] o_auc_KCV		KCV of the key.
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER		One or more input parameters are out of range.
	 * @retval #MORPHOERR_KEY_NOT_FOUND		The specified key is missing, unable to compute KCV.
	 * @retval #MORPHOERR_INVALID_CLASS		The class has been destroyed
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE		No device connected, or functionality not available on current device.
	 */
	I GetKCV (
			UC		i_uc_KeyID,
			PUC		i_puc_SecretID,
			UC		o_auc_KCV[KCV_LEN]
	);
	

	#ifdef _ANDROID_USB
	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	 I SetJNIEnv(JNIEnv* i_env);
	#endif
#if 0
	/*
	 * @see Identify (kept for compatibility)
	 */
	I Identify(
				US								i_us_Timeout,
				T_MORPHO_FAR					i_us_FAR,
				C_MORPHO_TemplateList &			i_x_TemplateList,
				UL								i_ul_CallbackCmd,
				T_MORPHO_CALLBACK_FUNCTION		i_pf_Callback,
				PVOID							i_pv_CallbackArgument,
				PUL								o_pul_MatchingScore,
				T_MORPHO_TYPE_TEMPLATE			i_x_TemplateType,
				C_MORPHO_TemplateList &			o_x_Template,
				T_MORPHO_TEMPLATE_ENVELOP		i_x_typEnvelop,
				I								i_i_ApplicationDataLen,
				PUC								i_puc_ApplicationData,
				UC &							o_uc_indexTemplate);

#endif

	/**
	 * This function requests the terminal to start the unlock process, and to return
	 * the value of the seed to be used for the next step of the unlock process.<br>
	 * The unlocking process is detailed in the MorphoSmartHostSystemInterfaceSpecification.pdf
	 * document.
	 *
	 * @param[out] o_puc_Seed
	 * - Contains the seed send by the MorphoSmart&tm;.
	 * @param[out] o_pul_SeedSize
	 * - Contains the seed size send by the MorphoSmart&tm;.
	 *
	 * @retval #MORPHO_OK 	The function was successful.
	 * @retval #MORPHOERR_INTERNAL	An error occurred during the execution of the function.
	 * @retval #MORPHOERR_DEVICE_NOT_LOCK	The device is not locked.
	 * @retval #MORPHOERR_PROTOCOLE	Communication protocol error.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I GetUnlockSeed(
				PUC		&o_puc_Seed,
				PUL		o_pul_SeedSize);

	/**
	 * This function unlocks the MorphoSmart&tm;. The host transmits the Seed retrieved by a
	 * prior call to the GetUnlockSeed function, ciphered  with the Secret corresponding to
	 * the SecretID that it owns. These two parameters are given by Morpho to the customer,
	 * upon his request.
	 *
	 * @param[in] i_puc_SecretID
	 * - Contains the Identification of the Secret used to cipher the seed.
	 * @param[in] i_ul_SecretIDSize
	 * - Contains the SecretID size.
	 * @param[in] i_puc_CipheredSeed
	 * - Contains the ciphered seed by the Secret according to the SecretID.
	 * @param[in] i_ul_CipheredSeedSize
	 * - Contains the seed ciphered size.
	 *
	 * @retval #MORPHO_OK 	The function was successful.
	 * @retval #MORPHOERR_INTERNAL 	An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I Unlock(
				PUC		i_puc_SecretID,
				UL		i_ul_SecretIDSize,
				PUC		i_puc_CipheredSeed,
				UL		i_ul_CipheredSeedSize);
	/**
	 * This function reboot the MorphoSmart&tm; software.
	 *
	 * @retval #MORPHO_OK 	The function was successful.
	 * @retval #MORPHOERR_INTERNAL 	An error occurred during the execution of the function.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 *
	 * \attention This function should not be used when a biometric command is underway.
	 * @note The use of RebootSoft function should use the following sequence to be robust in all environments :<br>
	 *      Step 1 - Call RebootSoft<br>
	 *      Step 2 - Wait for reboot software (the waiting time is approximately 3 seconds)<br>
	 *      on a Linux environment, you should add the step : Call the function C_MORPHO_Device::OpenDevice to reconnect with the sensor.
	 *
	 * <h3>Example usage:</h3>
	 * @code
	 *
	 * C_MORPHO_Device     l_x_MorphoSmart;
	 * I                   l_i_Ret;
	 *
	 * // reboot MorphoSmart software
	 * l_i_Ret = l_x_MorphoSmart.RebootSoft();
	 *
	 * if (l_i_Ret == MORPHO_OK)
	 * {
	 *      //Suspends the execution of the current thread until (3 second).
	 *      Sleep(3000);
	 *
	 *      //Opens the MorphoSmart communication link
	 *      l_i_Ret = l_x_MorphoSmart.OpenDevice(-1);
	 * }
	 *
	 * @endcode
	 */
	I RebootSoft() ;
	/**
	 * 	This function modifies the value of the specified parameter of the MorphoSmart&tm; device
	 * 	configuration. The modification is ignored until the MorphoSmart&tm; device is restarted.<br>
	 * 	A power off of the MorphoSmart&tm; device does not restore the default values: the modified
	 * 	values are saved.
	 *
	 * @param[in] i_us_tag
	 * - Parameter Identifier :
	 * 		- #CONFIG_SENSOR_WIN_POSITION_TAG: Sensor Window Position (only available for MorphoSmart&tm; CBM). Accepted values are 0, 1, 2, 3, 4, 5, 6 or 7.
	 * @param[in] i_i_paramValue
	 * - Parameter Value.
	 *
	 * @retval #MORPHO_OK 				The function was successful.
	 * @retval #MORPHOERR_INTERNAL 		An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I SetConfigParam(US i_us_tag, I i_i_paramValue);

	/**
	 * 	This function modifies the value of the specified parameter of the MorphoSmart&tm; device
	 * 	configuration. The modification is ignored until the MorphoSmart&tm; device is restarted.<br>
	 * 	A power off of the MorphoSmart&tm; device does not restore the default values: the modified
	 * 	values are saved.
	 *
	 * @param[in] i_us_tag
	 * - Parameter Identifier :
	 * 		- #CONFIG_SENSOR_WIN_POSITION_TAG: Sensor Window Position (only available for MorphoSmart&tm; CBM). Accepted values are 0, 1, 2, 3, 4, 5, 6 or 7.
	 *		- #CONFIG_UI_CONFIG_TAG: UI Configuration (only available for MorphoSmart&tm; FINGER VP)
	 *		- #CONFIG_UI_RESET_TAG: UI Reset (only available for MorphoSmart&tm; FINGER VP)
	 * @param[in] i_ul_size
	 * - Parameter Size.
	 * @param[in] i_puc_paramValue
	 * - Parameter Value.
	 *
	 * @retval #MORPHO_OK 				The function was successful.
	 * @retval #MORPHOERR_INTERNAL 		An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I SetConfigParam(	US i_us_tag, UL i_ul_size, PUC i_puc_paramValue);

	/**
	 * 	This function retrieves the value of a specified parameter of MorphoSmart&tm; device configuration.
	 *
	 * @param[in] i_us_tag
	 * - Parameter Identifier :
	 * 		- #CONFIG_SENSOR_WIN_POSITION_TAG: Sensor Window Position (only available for MorphoSmart&tm; CBM).
	 * @param[in, out] io_pul_paramValue
	 * - Parameter Value.
	 *
	 * @retval #MORPHO_OK 				The function was successful.
	 * @retval #MORPHOERR_INTERNAL 		An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I GetConfigParam(US i_us_tag, PI io_pul_paramValue);

	/**
	 * 	This function retrieves the value of a specified parameter of MorphoSmart&tm; device configuration.
	 *
	 * @param[in] i_us_tag
	 * - Parameter Identifier :
	 * 		- #CONFIG_SENSOR_WIN_POSITION_TAG: Sensor Window Position (only available for MorphoSmart&tm; CBM).
	 *		- #CONFIG_UI_CONFIG_TAG: UI Configuration (only available for MorphoSmart&tm; FINGER VP)
	 *		- #CONFIG_UI_RESET_TAG: UI Reset (only available for MorphoSmart&tm; FINGER VP)
	 * @param[in, out] io_pul_size
	 * - Parameter Size. The size can be NULL
	 * @param[in, out] io_ppuc_paramValue
	 * - Parameter Value. The paramValue can be NULL
	 *
	 * @retval #MORPHO_OK 				The function was successful.
	 * @retval #MORPHOERR_INTERNAL 		An error occurred during the execution of the function.
	 * @retval #MORPHOERR_BADPARAMETER	One or more input parameters are out of range.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I GetConfigParam(US i_us_tag, PUL io_pul_size, PUC* io_ppuc_paramValue);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I RegisterLEDEvent(
			//T_MORPHO_CALLBACK_FUNCTION	i_pf_callbackLED,
			int		(*callback)(const void *, int),
			PVOID	i_pv_param,
			PUL		o_pul_id
			);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I UnregisterLEDEvent( UL	i_ul_id );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I RegisterFFDCallback( int(*callback)(int*) );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I UnregisterFFDCallback( VOID );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I SetLOGSize( UL	i_ul_LogSize );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetLOGSize( PUL	o_pul_LogSize );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetLOG (	PUL	o_pul_LogSize,
				PUC	*o_ppuc_LogData);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I ClearLog( VOID );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I RegisterLogCallback (	void(*i_pf_callback)(char *),
			const void *i_pv_context);

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I UnregisterLogCallback ( VOID );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I CheckFingerPresence( PUC		o_puc_FingerPresent );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I SetLEDState(	UC	 	i_uc_LEDState );

 	/**
	 * @cond MORPHO_INTERNAL_USE
	 * \SAGEMUSEONLY
	 * @endcond
	 */
	I GetLEDState(	PUC	 	o_puc_LEDState );

	/**
	 * Allocates memory using device memory management modules. All data allocated through this
	 * method must be released by @ref Free method.
	 *
	 * @param[out] o_ppuc_buffer
	 * - Pointer to the newly allocated zone if successful.
	 * @param[in]  i_ul_size
	 * - Size in bytes to allocate. Must be greater than 0.
	 *
	 * @retval #MORPHO_OK				The function was successful
	 * @retval #MORPHOERR_MEMORY_PC 		The device is out of memory.
	 * @retval #MORPHOERR_BADPARAMETER	Provided pointer is NULL, or size allocation is 0.
	 */
	I Malloc(	PVOID	* 	o_ppuc_buffer, UL	i_ul_size );

	/**
	 * Releases memory previously allocated by @ref Malloc method.
	 *
	 * @param[in,out] io_ppuc_buffer
	 * - Memory to deallocate.
	 *
	 * @retval #MORPHO_OK				The function was successful
	 * @retval #MORPHOERR_MEMORY_PC 		The device is out of memory.
	 * @retval #MORPHOERR_BADPARAMETER	Provided pointer is NULL.
	 */
	I Free(	PVOID	* 	io_ppuc_buffer );

	/**
	 * This method loads a new symmetric key K<sub>S</sub> in the MorphoSmart&tm; using the
	 * unsecure mode: the new key is sent to device in plaintext. This mode is only available for
	 * standard devices (i.e. not secure).
	 *
	 * K<sub>S</sub> is used by the MorphoSmart&tm; to encrypt the exported biometric data at
	 * enrollment, see @ref EnableDataEncryption.
	 *
	 * @param[in] i_vuc_key	New symmetric key K<sub>S</sub> in plaintext.
	 *
	 * @deprecated left for compatibility, do not use in new projects
	 *
	 * @retval #MORPHO_OK
	 * @retval #MORPHOERR_BADPARAMETER
	 * @retval #MORPHOERR_INVALID_CLASS
	 * @retval #MORPHOERR_CORRUPTED_CLASS
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	int LoadKs(const std::vector<unsigned char> & i_vuc_key);

	/**
	 * This method loads a new symmetric key K<sub>S</sub> in the MorphoSmart&tm; using the
	 * unsecure mode: the new key is sent to device in plaintext. This mode is only available for
	 * standard devices (i.e. not secure).
	 *
	 * K<sub>S</sub> is used by the MorphoSmart&tm; to encrypt the exported biometric data at
	 * enrollment, see @ref EnableDataEncryption.
	 *
	 * @param[in] i_puc_key	New  symmetric key K<sub>S</sub> in plaintext.
	 * @param[in] i_i_key_size Size of symmetric key K<sub>S</sub> in plaintext.
	 *
	 * @retval #MORPHO_OK
	 * @retval #MORPHOERR_BADPARAMETER
	 * @retval #MORPHOERR_INVALID_CLASS
	 * @retval #MORPHOERR_CORRUPTED_CLASS
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I LoadKs(const PUC i_puc_key, I i_i_key_size);

	/**
	 * This method loads a new symmetric key K<sub>S</sub> in the MorphoSmart&tm; using the
	 * symmetric secure mode: the new key is sent to device in ciphertext, encrypted with the
	 * current key. This mode is available for both standard and secure devices. It is dedicated to
	 * key renewal, i.e. there must already be a key in the device.
	 *
	 * K<sub>S</sub> is used by the MorphoSmart&tm; to encrypt the exported biometric data at
	 * enrollment, see @ref EnableDataEncryption.
	 *
	 *
	 * @deprecated left for compatibility, do not use in new projects
	 *
	 * @param[in] i_vuc_key	New symmetric key K<sub>S</sub> encrypted with the current key to be
	 *						renewed using 3DES CBC algorithm as follows: [iv][Current K<sub>S</sub>
	 *						|| New K<sub>S</sub>]<sub>Current K<sub>S</sub></sub>.
	 *
	 * @retval #MORPHO_OK
	 * @retval #MORPHOERR_BADPARAMETER
	 * @retval #MORPHOERR_INVALID_CLASS
	 * @retval #MORPHOERR_CORRUPTED_CLASS
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	int LoadKsSecurely(const std::vector<unsigned char> & i_vuc_key);

	/**
	 * This method loads a new symmetric key K<sub>S</sub> in the MorphoSmart&tm; using the
	 * symmetric secure mode: the new key is sent to device in ciphertext, encrypted with the
	 * current key. This mode is available for both standard and secure devices. It is dedicated to
	 * key renewal, i.e. there must already be a key in the device.
	 *
	 * K<sub>S</sub> is used by the MorphoSmart&tm; to encrypt the exported biometric data at
	 * enrollment, see @ref EnableDataEncryption.
	 *
	 * @param[in] i_puc_key	New symmetric key K<sub>S</sub> encrypted with the current key to be
	 *						renewed using 3DES CBC algorithm as follows: [iv][Current K<sub>S</sub>
	 *						|| New K<sub>S</sub>]<sub>Current K<sub>S</sub></sub>.
 	 * @param[in] i_i_key_size Size of symmetric key K<sub>S</sub>
	 * @retval #MORPHO_OK
	 * @retval #MORPHOERR_BADPARAMETER
	 * @retval #MORPHOERR_INVALID_CLASS
	 * @retval #MORPHOERR_CORRUPTED_CLASS
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I LoadKsSecurely(const PUC i_puc_key, I i_i_key_size);
	/**
	 * This method loads a new symmetric key K<sub>S</sub> in the MorphoSmart&tm; using the
	 * asymmetric secure mode: the new key is sent in ciphertext, encrypted with device certificate
	 * and signed with host private key. This mode is only available for secure devices.
	 *
	 * K<sub>S</sub> is used by the MorphoSmart&tm; to encrypt the exported biometric data at
	 * enrollment, see @ref EnableDataEncryption.
	 *
	 *
	 * @deprecated left for compatibility, do not use in new projects
	 *
	 * @param[in] i_vuc_key					Symmetric key K<sub>S</sub> in ciphertext.
	 * @param[in] i_vuc_signature			Signature of the encrypted key.
	 * @param[in] i_vuc_host_certificate	Host certificate (encoded in DER format).
	 *
	 * @retval #MORPHO_OK
	 * @retval #MORPHOERR_BADPARAMETER
	 * @retval #MORPHOERR_INVALID_CLASS
	 * @retval #MORPHOERR_CORRUPTED_CLASS
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	int LoadKsSecurely
	(
		const std::vector<unsigned char> & i_vuc_key,
		const std::vector<unsigned char> & i_vuc_signature,
		const std::vector<unsigned char> & i_vuc_host_certificate
	);

	/**
	 * This method loads a new symmetric key K<sub>S</sub> in the MorphoSmart&tm; using the
	 * asymmetric secure mode: the new key is sent in ciphertext, encrypted with device certificate
	 * and signed with host private key. This mode is only available for secure devices.
	 *
	 * K<sub>S</sub> is used by the MorphoSmart&tm; to encrypt the exported biometric data at
	 * enrollment, see @ref EnableDataEncryption.
	 *
	 * @param[in] i_puc_key					Symmetric key K<sub>S</sub> in ciphertext.
	 * @param[in] i_i_key_size              Size of symmetric key K<sub>S</sub> in ciphertext.
	 * @param[in] i_puc_signature			Signature of the encrypted key.
	 * @param[in] i_i_signature_size        Size of signature of the encrypted key.
	 * @param[in] i_puc_host_certificate	Host certificate (encoded in DER format).
	 * @param[in] i_i_host_certificate_size Size of host certificate.
	 *
	 * @retval #MORPHO_OK
	 * @retval #MORPHOERR_BADPARAMETER
	 * @retval #MORPHOERR_INVALID_CLASS
	 * @retval #MORPHOERR_CORRUPTED_CLASS
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I LoadKsSecurely
	(
		const PUC  i_puc_key,
		I i_i_key_size,
		const PUC i_puc_signature,
		I i_i_signature_size,
		const PUC i_puc_host_certificate,
		I i_i_host_certificate_size
	);
	/**
	 * This method enables or disables the encryption of exported biometric data and sets the diversification data to be concatenated to image/template before encryption.
	 * @param[in] i_b_IsDataEncryption			This variable is used to indicate the activation or deactivation encryption.
	 * @param[in] i_ul_DiversificationDataSize	Diversification data size.
	 * @param[in] i_puc_DiversificationData		Diversification data to be concatenated to image/template before encryption.
	 *
	 * @retval #MORPHO_OK					The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER		One or more input parameters are out of range.
	 * @retval #MORPHOERR_INVALID_CLASS		The class has been destroyed
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 * @retval #MORPHOERR_UNAVAILABLE	No device connected, or functionality not available on current device.
	 */
	I EnableDataEncryption(
							bool		i_b_IsDataEncryption,
							UL			i_ul_DiversificationDataSize,
							PUC			i_puc_DiversificationData);

	/**
	 * This method indicates if exported biometric data encryption is activated and returns diversification data.
	 * Exported biometric data encryption is enabled/disabled with @ref EnableDataEncryption.
	 * @param[out] o_pul_DiversificationDataSize	Size of diversification data buffer.
	 * @param[out] o_ppuc_DiversificationData		Pointer to diversification data buffer (must be freed by the caller).
	 *
	 * @retval True		Exported biometric data encryption is activated
	 * @retval False	Exported biometric data encryption is not activated
	 */
	bool IsDataEncryptionEnabled(
									PUL		o_pul_DiversificationDataSize,
									PUC*	o_ppuc_DiversificationData);

	/**
	 * \SAGEMUSEONLY
	 */
	I EnableCS(bool	i_b_IsCSEnabled);

	/**
	 * \SAGEMUSEONLY
	 */
	bool IsCSEnabled();

	/**
	 * \SAGEMUSEONLY
	 */
	INSTANCE_LIB GetInstanceMSOCS();

	/**
	 * \SAGEMUSEONLY
	 */
	I GetSensorType();


	// Log
	/**
	 * Modify the logging mode (the modification is effective only in RAM memory and it is not reported in the configuration file).
	 * @param[in] i_x_Mode		the new logging mode.
	 *
	 * @retval MORPHO_OK	          the modification succeeded
	 * @retval MORPHOERR_BADPARAMETER invalid parameter
	 * @retval MORPHOERR_UNAVAILABLE  log unavailable
	 */
	I SetLoggingMode(T_MORPHO_LOG_Mode i_x_Mode);

	/**
	 * Modify the logging level of a group (the modification is effective only in RAM memory and it is not reported in the configuration file).
	 * @param[in] i_ui_GroupId  identifier of the group.
	 * @param[in] i_x_Level     new logging level of the group; MORPHO_LOG_NOLOG for disabling the logging for the group.
	 *
	 * @retval MORPHO_OK	          the modification succeeded
	 * @retval MORPHOERR_BADPARAMETER invalid parameter
	 * @retval MORPHOERR_UNAVAILABLE  log unavailable
	 */
	I SetLoggingLevelOfGroup(UI                 i_ui_GroupId,
							 T_MORPHO_LOG_Level i_x_Level);


	/**
	 *
	 * This function sets the Strategy Acquisition Mode : Five different modes are supported.
	 *
	 * @param[in] i_uc_StrategyAcquisitionMode
	 * - Possible values:
	 *		- #MORPHO_ACQ_EXPERT_MODE  (Default Value)
	 *		- #MORPHO_ACQ_UNIVERSAL_FAST_MODE
	 *		- #MORPHO_ACQ_UNIVERSAL_ACCURATE_MODE
	 *		- #MORPHO_ACQ_FULL_MULTIMODAL_MODE
	 *		- #MORPHO_ACQ_ANTI_SPOOFING_MODE
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER		Invalid parameter
	 */
	I SetStrategyAcquisitionMode(UC	i_uc_StrategyAcquisitionMode );

	/**
	 *
	 * This function returns the Acquisition Mode set by SetStrategyAcquisitionMode.
	 * - Possible values:
	 *		- #MORPHO_ACQ_EXPERT_MODE
	 *		- #MORPHO_ACQ_UNIVERSAL_FAST_MODE
	 *		- #MORPHO_ACQ_UNIVERSAL_ACCURATE_MODE
	 *		- #MORPHO_ACQ_FULL_MULTIMODAL_MODE
	 *		- #MORPHO_ACQ_ANTI_SPOOFING_MODE
	 */
	UC GetStrategyAcquisitionMode();
private:
	// Operations
	I GetDescriptor();

	I LoadSensorType();

	// Bug correction: FVP-634
	// Flag pour savoir si OpenDevice a été appelée
	I									m_i_isOpen;
	// OpenDevice N�1
	I									m_i_serialPortNumber;
	I									m_i_BaudeRate;
	// OpenDevice N�2
	T_MORPHO_CALLBACK_COM_OPEN			m_pf_CallbackComOpen;
	T_MORPHO_CALLBACK_COM_SEND			m_pf_CallbackComSend;
	T_MORPHO_CALLBACK_COM_RECEIVE		m_pf_CallbackComReceive;
	T_MORPHO_CALLBACK_COM_RECEIVE_FREE	m_pf_CallbackComReceiveFree;
	T_MORPHO_CALLBACK_COM_CLOSE			m_pf_CallbackComClose;
	PC									m_pc_String;
	UL									m_ul_Param;
	// OpenDevice N�3
	C									m_ac_MsoName[64];
	UL									m_ul_TimeOut;


	VOID Certificatefree();
	VOID ReleaseEnumDevice();
	VOID ReleaseUsbDeviceName();
	#ifdef _ANDROID_USB
		I InitUsbDevicesEnum (PUL	o_pul_NbDevicePlug, JNIEnv* i_env);
	#else
		I InitUsbDevicesEnum (PUL	o_pul_NbDevicePlug);
	#endif
private:

	#define MORPHO_USB_DEVICE_MAX				100		// The USB driver can't support more than 100 USB devices

	typedef struct
	{
		MORPHO_HANDLE	m_h_Mso100Handle;

		PVOID 			m_px_wrapper;

		PUC				m_puc_imageAndHeader;
		UL				m_ul_imageAndHeaderSize;

		PUC				m_puc_RawImageAndHeader;
		UL				m_ul_RawImageAndHeaderSize;

		PUC				m_puc_Certif;
		UL				m_ul_CertifSize;

		PUC				m_puc_ReceiveData;
		UL				m_ul_ReceiveDataSize;

		PVOID			m_ax_UsbDevice [MORPHO_USB_DEVICE_MAX];

		UL				m_ul_NbUsbDevice;
		PC				m_apc_UsbDeviceName [MORPHO_USB_DEVICE_MAX];
		PC				m_apc_UsbDeviceFriendlyName [MORPHO_USB_DEVICE_MAX];
		UL				m_ul_NbUsbDeviceName;

		UL				m_ul_ComType;

		I				m_i_FFD_SecurityLevel;

		UC				m_auc_Seed[LOCK_ALEA_SIZE];

		UL				m_ul_DiversificationDataSize;
		PUC				m_puc_DiversificationData;
		INSTANCE_LIB	m_x_LibMSOCS;
		I				m_i_SensorType;

		bool			m_b_IsCSEnabled;
		bool			m_b_IsDataEncryption;
		UC			    m_uc_StrategyAcquisitionMode; //FVP 1.10
		PC				m_pc_serialPortName;

	} T_MORPHO_DEVICE,*PT_MORPHO_DEVICE;

	PT_MORPHO_DEVICE	m_px_MorphoDevice;

	UL	m_ul_magicEnd;
};


#endif // MORPHO_DEVICE_H
