// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <pthread.h>

#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <vector>
#include <getopt.h>
//#include "MORPHOSMART_TestBio.h"
#include "MORPHO_Types.h"
#include "sample_sdk.h"

// If enabled, will use libsdl to display live acquisition feed
//#define ENABLE_DISPLAY

static UL	g_ul_ledEventsID;

static UC		g_uc_ConnectionType = 0; 					
static I		g_i_speed = 115200;      					
static I		g_i_port = 1;
static C		g_c_PortName[INPUT_BUFFER_SIZE];		

#ifndef __arm__
namespace openssl
{
	//#include <openssl/ossl_typ.h>
	#include <openssl/evp.h>
}

void *g_x_LibCrypto = NULL;
typedef int (FCT_RAND_BYTES) ( unsigned char *buf, int num);
typedef const openssl::EVP_CIPHER * (FCT_EVP_DES_EDE3_CBC) (void);
typedef int (FCT_EVP_ENCRYPTINIT) (openssl::EVP_CIPHER_CTX *ctx, const openssl::EVP_CIPHER *cipher, const unsigned char *key,
	const unsigned char *iv);
typedef int (FCT_EVP_ENCRYPTUPDATE) (openssl::EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in,
	int inl);

FCT_RAND_BYTES * 		g_pf_RANDBytes 			= NULL;
FCT_EVP_DES_EDE3_CBC * 	g_pf_EVP_des_ede3_cbc 	= NULL;
FCT_EVP_ENCRYPTINIT *	g_pf_EVP_EncryptInit	= NULL;
FCT_EVP_ENCRYPTUPDATE *	g_pf_EVP_EncryptUpdate 	= NULL;


#define TRIPLE_DES_KEY_LENGTH	24
#define DES_BLOCK_LENGTH		8
#endif //__arm__

#ifdef ENABLE_DISPLAY
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define CORRECT_LEVEL2(a)	if (a == 0x00)	   \
								a = 0x02;      \
							else if (a == 0x40)\
								a = 0x48;      \
							else if (a == 0x80)\
								a = 0xB0;      \
							else if (a == 0xC0)\
								a = 0xFE;      \

#define CORRECT_LEVEL1(a)	if (a == 0x00)     \
								a = 0x00;      \
							else if (a == 0x80)\
								a = 0xFF;      \


SDL_Surface *screen;
SDL_Surface *img;
SDL_Event event;
SDL_Color colors[256];
US g_us_ScreenNbRow;
US g_us_ScreenNbCol;
UC g_uc_IsVideoInit;
UC g_uc_IsSDL_ttfInit;
#endif

#ifndef MORPHO_FIELD_NAME_LEN
#define MORPHO_FIELD_NAME_LEN 6
#endif

#ifdef ENABLE_DISPLAY
/*****************************************************************************/
/*****************************************************************************/
I InitScreen ()
{
	I l_i_i;
	const SDL_VideoInfo *vi;

	/*
	* Initialisation de SDL
	*/
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		fprintf (stderr, "Error SDL: %s\n", SDL_GetError ());
		return -1;
	}
	g_uc_IsVideoInit = 1;
	atexit (SDL_Quit);

	/*
	* initialisation de SDL_ttf: affichage de texte dans une fenetre SDL
	*/
	if (TTF_Init () < 0)
	{
		fprintf (stderr, "Error init SDL_TTF: %s\n", SDL_GetError ());
		return -1;
	}
	g_uc_IsSDL_ttfInit = 1;

	g_us_ScreenNbRow = 420;
	g_us_ScreenNbCol = 420;

	for (l_i_i = 0; l_i_i < 256; l_i_i++)
	{
		colors[l_i_i].r = l_i_i;
		colors[l_i_i].g = l_i_i;
		colors[l_i_i].b = l_i_i;
	}

	screen = SDL_SetVideoMode (g_us_ScreenNbCol,
					g_us_ScreenNbRow + 60,
					8, SDL_RESIZABLE | SDL_HWPALETTE);

	SDL_SetColors (screen, colors, 0, 256);

	vi = SDL_GetVideoInfo ();
	if (vi && vi->wm_available)
		SDL_WM_SetCaption ("Capture Window", "Icone SDL");

	sleep (1);
	return 0;
}

/*****************************************************************************/
/*****************************************************************************/
I CloseScreen ()
{
	if (g_uc_IsSDL_ttfInit == 1)
		TTF_Quit ();

	if (g_uc_IsVideoInit == 1)
	{
		SDL_FreeSurface (screen);
		SDL_Quit ();
	}

	return 0;
}

I ConvertImage(	PUC i_puc_Img,
				US i_us_NbRow,
				US i_us_NbCol,
				UC i_uc_NbBitsPerPixel,
				PUC o_puc_Img8BitsPerPixel)
{
	I l_i_ImageSize;
	PUC l_puc_image8bits;
	I l_i_Cnt;

	l_puc_image8bits = o_puc_Img8BitsPerPixel;

	l_i_ImageSize = i_us_NbRow * i_us_NbCol;
	switch (i_uc_NbBitsPerPixel)
	{
	case 8:
		memcpy (o_puc_Img8BitsPerPixel, i_puc_Img, l_i_ImageSize);
		break;
	case 4:
		for (l_i_Cnt = 0; l_i_Cnt < l_i_ImageSize / 2; l_i_Cnt++)
		{
			l_puc_image8bits[2 * l_i_Cnt] = (i_puc_Img[l_i_Cnt] & 0xF0) + 0x08;
			l_puc_image8bits[2 * l_i_Cnt + 1] =((i_puc_Img[l_i_Cnt] & 0x0F) << 4) + 0x08;
		}
		break;
	case 2:
		for (l_i_Cnt = 0; l_i_Cnt < l_i_ImageSize / 4; l_i_Cnt++)
		{
			l_puc_image8bits[4 * l_i_Cnt] = (i_puc_Img[l_i_Cnt] & 0xC0);
			l_puc_image8bits[4 * l_i_Cnt + 1] = ((i_puc_Img[l_i_Cnt] & 0x30) << 2);
			l_puc_image8bits[4 * l_i_Cnt + 2] = ((i_puc_Img[l_i_Cnt] & 0x0C) << 4);
			l_puc_image8bits[4 * l_i_Cnt + 3] = ((i_puc_Img[l_i_Cnt] & 0x03) << 6);
			CORRECT_LEVEL2 (l_puc_image8bits[4 * l_i_Cnt])
			CORRECT_LEVEL2 (l_puc_image8bits[4 * l_i_Cnt + 1])
			CORRECT_LEVEL2 (l_puc_image8bits[4 * l_i_Cnt + 2])
			CORRECT_LEVEL2 (l_puc_image8bits[4 * l_i_Cnt + 3])
		}
		break;
	case 1:
		for (l_i_Cnt = 0; l_i_Cnt < l_i_ImageSize / 8; l_i_Cnt++)
		{
			l_puc_image8bits[8 * l_i_Cnt] = (UC) (i_puc_Img[l_i_Cnt] & 0x80);
			l_puc_image8bits[8 * l_i_Cnt + 1] = (UC) ((i_puc_Img[l_i_Cnt] & 0x40) << 1);
			l_puc_image8bits[8 * l_i_Cnt + 2] = (UC) ((i_puc_Img[l_i_Cnt] & 0x20) << 2);
			l_puc_image8bits[8 * l_i_Cnt + 3] = (UC) ((i_puc_Img[l_i_Cnt] & 0x10) << 3);
			l_puc_image8bits[8 * l_i_Cnt + 4] = (UC) ((i_puc_Img[l_i_Cnt] & 0x08) << 4);
			l_puc_image8bits[8 * l_i_Cnt + 5] = (UC) ((i_puc_Img[l_i_Cnt] & 0x04) << 5);
			l_puc_image8bits[8 * l_i_Cnt + 6] = (UC) ((i_puc_Img[l_i_Cnt] & 0x02) << 6);
			l_puc_image8bits[8 * l_i_Cnt + 7] = (UC) ((i_puc_Img[l_i_Cnt] & 0x01) << 7);
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt])
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt + 1])
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt + 2])
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt + 3])
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt + 4])
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt + 5])
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt + 6])
			CORRECT_LEVEL1 (l_puc_image8bits[8 * l_i_Cnt + 7])
		}
		break;
	default:
		fprintf (stderr, "Invalid value i_uc_NbBitsPerPixel: %d\n", i_uc_NbBitsPerPixel);
		return -1;
	}

	return 0;
}

I Display_Image (PUC i_puc_Img, US i_us_NbRow, US i_us_NbCol,
		UC i_uc_NbBitsPerPixel)
{
	I l_i_Ret;
	SDL_Rect coords;
	SDL_Rect Rect;
	PUC l_puc_image8bits;

	if ((i_uc_NbBitsPerPixel <= 0) || (i_us_NbRow <= 0) || (i_us_NbCol <= 0))
		return -1;

	l_puc_image8bits = (PUC)malloc (648 * 488);
	l_i_Ret =
		ConvertImage (i_puc_Img, i_us_NbRow, i_us_NbCol,
			i_uc_NbBitsPerPixel, l_puc_image8bits);
	if (l_i_Ret < 0)
		return -1;

	img = SDL_AllocSurface (SDL_SWSURFACE, i_us_NbCol,	// img->w,
							i_us_NbRow,	// img->h,
							8,	// img->format->BitsPerPixel,
							0x00,	// 0xF800, // img->format->Rmask,
							0x00,	// 0x07E0, // img->format->Gmask,
							0x00,	// 0x001F, //img->format->Bmask,
							0x00	// img->format->Amask
		);
	SDL_SetColors (img, colors, 0, 256);

	coords.x = screen->w / 2 - img->w / 2;
	coords.y = g_us_ScreenNbRow / 2 - img->h / 2;
	coords.w = i_us_NbCol;
	coords.h = i_us_NbRow;

	memcpy (img->pixels, (void *) l_puc_image8bits, i_us_NbCol * i_us_NbRow);

	Rect.x = 0;
	Rect.y = 0;
	Rect.w = g_us_ScreenNbCol;
	Rect.h = g_us_ScreenNbRow;
	SDL_FillRect (screen, &Rect, 0);
	SDL_BlitSurface (img, 0, screen, &coords);
	SDL_UpdateRect (screen, Rect.x, Rect.y, Rect.w, Rect.h);
	SDL_FreeSurface (img);

	if (l_puc_image8bits != NULL)
		free (l_puc_image8bits);

	return 0;
}
#endif

#ifndef __arm__
int LoadKsSymmSec( bool & successOfFunction, T_DATA *io_px_data )
{
	successOfFunction = false;

	int l_i_Ret = MORPHO_OK;
	bool l_b_continue = true;

	std::vector<unsigned char> l_vuc_ksCurBin, l_vuc_ksNewBin;
	int l_i_cryptlen = 0;
	unsigned char * l_puc_crypt = NULL;


	//read current Ks key from file
	char l_ac_String[256];

	fprintf (stdout, "---Open 'ks.cur.bin' file -----\n");
	fprintf (stdout, "Enter File name : -> ");
	fgets (l_ac_String, 256, stdin);
	l_ac_String[strlen (l_ac_String) - 1] = 0;	// Suppress '\n'

	std::ifstream ksCurBinFile( l_ac_String, std::ios::binary );

	if( !ksCurBinFile.is_open() )
	{
		fprintf(stdout, "Unable to open 'ks.cur.bin' file\n");
		l_b_continue = false;
	}
	else
	{
		l_vuc_ksCurBin.assign( std::istreambuf_iterator<char>(ksCurBinFile), std::istreambuf_iterator<char>() );
		ksCurBinFile.close();

		if( l_vuc_ksCurBin.size() != TRIPLE_DES_KEY_LENGTH )
		{
			fprintf(stdout, "Error: key in 'ks.cur.bin' file is not 192 bits long\n");
			l_b_continue = false;
		}
	}

	if( l_b_continue )
	{
		//read new Ks key from file
		char l_ac_String[256];

		fprintf (stdout, "---Open 'ks.new.bin' file -----\n");
		fprintf (stdout, "Enter File name : -> ");
		fgets (l_ac_String, 256, stdin);
		l_ac_String[strlen (l_ac_String) - 1] = 0;	// Suppress '\n'

		std::ifstream ksNewBinFile( l_ac_String, std::ios::binary );

		if( !ksNewBinFile.is_open() )
		{
			fprintf(stdout, "Unable to open 'ks.new.bin' file\n");
			l_b_continue = false;
		}
		else
		{
			l_vuc_ksNewBin.assign( std::istreambuf_iterator<char>(ksNewBinFile), std::istreambuf_iterator<char>() );
			ksNewBinFile.close();

			if( l_vuc_ksNewBin.size() != TRIPLE_DES_KEY_LENGTH )
			{
				fprintf(stdout, "Error: key in 'ks.new.bin' file is not 192 bits long\n");
				l_b_continue = false;
			}

		}
	}

	if( l_b_continue )
	{
		//format data for encryption (because we have to encrypt [current-ks||new-ks] with current ks)
		l_vuc_ksNewBin.insert( l_vuc_ksNewBin.begin(), l_vuc_ksCurBin.begin(), l_vuc_ksCurBin.end() );

		l_i_cryptlen = l_vuc_ksNewBin.size();

		l_puc_crypt = (unsigned char *)malloc(l_i_cryptlen);
		if( l_puc_crypt == NULL )
		{
			fprintf(stdout, "Memory allocation error\n");
			l_b_continue = false;
		}
	}

	/************************************/
	/**encrypt new key with current key**/
	/************************************/

	if( l_b_continue )
	{
		//open libcrypto.so
		if( g_x_LibCrypto == NULL )
		{
			g_x_LibCrypto = dlopen("libcrypto.so", RTLD_LAZY);
			if( g_x_LibCrypto == NULL )
			{
				fprintf(stdout, "Unable to open 'libcrypto.so'\n");
				l_b_continue = false;
			}
			else
			{
				//resolve some func symbols of the library
				g_pf_RANDBytes = (FCT_RAND_BYTES *) dlsym(g_x_LibCrypto, "RAND_bytes");
				g_pf_EVP_des_ede3_cbc = (FCT_EVP_DES_EDE3_CBC *) dlsym(g_x_LibCrypto, "EVP_des_ede3_cbc");
				g_pf_EVP_EncryptInit = (FCT_EVP_ENCRYPTINIT *) dlsym(g_x_LibCrypto, "EVP_EncryptInit");
				g_pf_EVP_EncryptUpdate = (FCT_EVP_ENCRYPTUPDATE *) dlsym(g_x_LibCrypto, "EVP_EncryptUpdate");

				if( g_pf_RANDBytes == NULL || g_pf_EVP_des_ede3_cbc == NULL || g_pf_EVP_EncryptInit == NULL
					|| g_pf_EVP_EncryptUpdate == NULL )
				{
					if( g_pf_RANDBytes == NULL )
						fprintf(stdout, "Error while loading 'RAND_bytes' function.\n");

					if( g_pf_EVP_des_ede3_cbc == NULL )
						fprintf(stdout, "Error while loading 'EVP_des_ede3_cbc' function.\n");

					if( g_pf_EVP_EncryptInit == NULL )
						fprintf(stdout, "Error while loading 'EVP_EncryptInit' function.\n");

					if( g_pf_EVP_EncryptUpdate == NULL )
						fprintf(stdout, "Error while loading 'EVP_EncryptUpdate' function.\n");

					dlclose( g_x_LibCrypto );
					g_x_LibCrypto = NULL;

					l_b_continue = false;
					g_pf_RANDBytes 			= NULL;
					g_pf_EVP_des_ede3_cbc 	= NULL;
					g_pf_EVP_EncryptInit 	= NULL;
					g_pf_EVP_EncryptUpdate 	= NULL;
				}
			}
		}
	}


	unsigned char l_auc_iv[DES_BLOCK_LENGTH];

	if( l_b_continue )
	{
		//generate random iv
		if( !g_pf_RANDBytes( l_auc_iv, DES_BLOCK_LENGTH ) )
		{
			fprintf(stdout, "Error OpenSSL for 'RAND_bytes' function\n");
			l_b_continue = false;
		}
	}

	unsigned char * l_puc_plaintext = NULL;
	int l_i_FinalCryptlen = 0;

	if( l_b_continue )
	{
		//encryption is done without padding : last bytes (0 up to 7) may not be encrypted
		//compute length of input data that will be indeed encrypted
		l_i_FinalCryptlen = l_i_cryptlen - ( l_i_cryptlen % DES_BLOCK_LENGTH );

		//copy last plaintext bytes to output buffer
		memcpy( l_puc_crypt + l_i_FinalCryptlen, &l_vuc_ksNewBin.front() + l_i_FinalCryptlen, l_i_cryptlen % DES_BLOCK_LENGTH );

		//allocate temporary buffer for plaintext data
		l_puc_plaintext	= static_cast<unsigned char *>( malloc( l_i_FinalCryptlen ) );
		if( l_puc_plaintext == NULL )
		{
			fprintf(stdout, "Memory allocation error\n");
			l_b_continue = false;
		}
		else
			memcpy( l_puc_plaintext, &l_vuc_ksNewBin.front(), l_i_FinalCryptlen );
	}

	if( l_b_continue )
	{
		//initialize encryption
		openssl::EVP_CIPHER_CTX l_x_ctx = {0};
		g_pf_EVP_EncryptInit( &l_x_ctx, g_pf_EVP_des_ede3_cbc(), &l_vuc_ksCurBin.front(), l_auc_iv );

		//encrypt
		int l_i_encr = 0;
		g_pf_EVP_EncryptUpdate( &l_x_ctx, l_puc_crypt, &l_i_encr, l_puc_plaintext, l_i_FinalCryptlen );

		free( l_puc_plaintext );
	}

	if( l_b_continue )
	{
		//prepare buffer to send: [iv][3DES-CBC(current-ks,[current-ks||new-ks])]
		l_vuc_ksNewBin.assign( l_puc_crypt, l_puc_crypt + l_i_cryptlen );
		l_vuc_ksNewBin.insert( l_vuc_ksNewBin.begin(), l_auc_iv, l_auc_iv + sizeof(l_auc_iv) );
		free(l_puc_crypt);
	}


	/*******************************************/
	/**end of encrypt new key with current key**/
	/*******************************************/



	if( l_b_continue )
	{
		//load new key in device : key is sent in ciphertext
		l_i_Ret = io_px_data->m_x_device.LoadKsSecurely( l_vuc_ksNewBin );

		if( l_i_Ret != MORPHO_OK )
		{
			#if DEBUG
			fprintf(stdout, "Error ret=%d while calling C_MORPHO_Device::LoadKsSecurely() function\n", l_i_Ret);
			#endif
		}
		else
			successOfFunction = true;
	}



	return l_i_Ret;
}


int DisplayKsKcv( T_DATA *io_px_data )
{
	unsigned char l_auc_kcv[ KCV_LEN ] = {0};

	int l_i_Ret = io_px_data->m_x_device.GetKCV( ID_KS, NULL, l_auc_kcv );

	if( l_i_Ret != MORPHO_OK )
	{
		#if DEBUG
		fprintf(stdout, "Error ret=%d while calling C_MORPHO_Device::GetKCV() function\n", l_i_Ret);
		#endif
	}
	else
		fprintf(stdout, "Ks KCV : 0x%02X 0x%02X 0x%02X\n", l_auc_kcv[0], l_auc_kcv[1], l_auc_kcv[2]);

	return l_i_Ret;
}
#endif //__arm__

int htoi(char i_c_char)
{
	switch(i_c_char)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': case 'a': return 10;
	case 'B': case 'b': return 11;
	case 'C': case 'c': return 12;
	case 'D': case 'd': return 13;
	case 'E': case 'e': return 14;
	case 'F': case 'f': return 15;
	default:
		return -1;
	}
}

char itoh(unsigned char i_uc_char)
{
	switch(i_uc_char)
	{
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 10: return 'A';
	case 11: return 'B';
	case 12: return 'C';
	case 13: return 'D';
	case 14: return 'E';
	case 15: return 'F';
	default:
		return '?';
	}
}

int SampleGetTickCount()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_usec/1000 + tv.tv_sec*1000);
}

#ifndef __arm__
/*****************************************************************************/
/*****************************************************************************/
int CommandLoadKs()
{
	char l_ac_string[32];
	int l_i_i = 0;

	fprintf (stdout, "-------------------- Security Mode ----------------\n");
	fprintf (stdout, "\tLoad Ks in Unsecure Mode 				--->(1)\n");
	fprintf (stdout, "\tLoad Ks in Symmetric Secure Mode                --->(2)\n");
	fprintf (stdout, "\tGet Ks KCV 						--->(3)\n");

	fprintf (stdout, "Select -> ");
	while (1)
	{
		if (fgets (l_ac_string, 32, stdin) == NULL)
		{
			fprintf (stdout, "->Error\n");
			return -1;
		}
		if (sscanf (l_ac_string, "%d", &l_i_i) == 1)
		{
			if (l_i_i >= 1 && l_i_i <= 3)
				break;
		}
		fprintf (stdout, "Select -> ");
	}

	return l_i_i;
}
#endif //__arm__

void usage(void)
{
    fprintf(stdout, "---------------- SDK_Sample Options------------------ \n");
    fprintf(stdout, "Connection mode     : -c (Default: USB) \n");
    fprintf(stdout, "---->  -c USB       : Usb Connection \n");
    fprintf(stdout, "---->  -c RS232     : Serial Connection\n");
    fprintf(stdout, "---->  -s <speed>   : Set baud rate to speed \n");
    fprintf(stdout, "---->  -p <port>   : Set port serial Connection \n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Example :\n");
    fprintf(stdout, "----> SDK_Sample -c USB\n");
    fprintf(stdout, "----> SDK_Sample -c RS232 -p 1 -s 115200\n");
    fprintf(stdout, "\n");
}

/**
 * Main menu.
 */
int main(int argc, char *argv[])
{
	C		l_ac_buffer[INPUT_BUFFER_SIZE];
	UC		l_uc_loop = 1;
	I		l_i_command;
	T_DATA	l_x_data;

	I                option;
   	C                *liste_options = "c:s:p:d:h";

	printf("Test MORPHOSMART\n");
	I i;

	// Options
	    while ((option = getopt(argc, argv, liste_options)) != -1)
	    {
		switch (option)
		{
		    case 'c' :
		        if(strcmp(optarg, "RS232") == 0)
		        {
		        	fprintf(stdout, "Use Serial Port\n");
					g_uc_ConnectionType = 1;
					printf("please insert the baudrate \n");
					scanf("%d", &g_i_speed);
					printf("baudrate =%d\n",g_i_speed);
					printf("please insert the serial port \n");
					scanf("%s", &g_c_PortName);
					printf("the port name =%s\n",g_c_PortName);

		        }
		        else if(strcmp(optarg, "USB") == 0)
		        {
		            fprintf(stdout, "Use USB bus\n");
		            g_uc_ConnectionType = 0;
		        }
		        break;
		    case 's' :
			sscanf(optarg, "%d", &g_i_speed);
			fprintf(stdout, "g_i_speed = %s \n",optarg);
			break;
		    case 'p' :
			sscanf(optarg, "%d", &g_i_port);
			fprintf(stdout, "g_i_port = COM%s \n",optarg);
			break;
		    case 'h' :
                	usage();
                	return 0;
		}
	    }

	while (l_uc_loop)
	{
		getUserInput(l_ac_buffer, "\n\nMain menu:\n1 - device operations\n2 - database operations\n3 - user operations\n0 - quit\n");
		if (sscanf (l_ac_buffer, "%d", &l_i_command) == 1)
		{

			switch (l_i_command)
			{
			case 1:
				deviceOperation(&l_x_data);
				break;
			case 2:
				databaseOperation(&l_x_data);

				break;
			case 3:
				userOperation(&l_x_data);
				break;
			case 0:
				l_uc_loop = 0;
				break;
			default:
				fprintf(stdout, "Invalid command\n\n");
				break;
			}
		}
	}

	return 0;
}

/**
 * C_MORPHO_Device interface
 */
I deviceOperation(PT_DATA io_px_data)
{
	I						l_i_Ret;
	UC						l_uc_loop = 1;
	I						l_i_command = 0;
	UL						l_ul_index;
	L						l_l_deviceIndex;
	UL						l_ul_NbUsbDevice;
	PC						l_pc_deviceName;
	PC						l_pc_deviceProperties;
	C_MORPHO_TemplateList	l_x_templates;
	C						l_ac_buffer[INPUT_BUFFER_SIZE];
	C						l_ac_chaine[INPUT_BUFFER_SIZE];
	UL						l_ul_asyncEvents =	MORPHO_CALLBACK_DETECTQUALITY |
												MORPHO_CALLBACK_CODEQUALITY |
												MORPHO_CALLBACK_IMAGE_CMD |
												MORPHO_CALLBACK_COMMAND_CMD |
												MORPHO_CALLBACK_ENROLLMENT_CMD;
	UC						l_i_LED;
	T_MORPHO_COMPRESS_ALGO	l_x_compressAlgo = MORPHO_NO_COMPRESS;
	I						l_i_CompressRate;
	UL						l_ul_nbFingers;

	US						l_us_paramTag;
	I						l_i_intParam;
	PUC						l_puc_stringParam = NULL;
	UL						l_ul_stringParamSize;
//	I						l_i_Param;
	I						l_i_i;
	PUC						l_puc_MsoCertif = NULL;


	while (l_uc_loop)
	{
		getUserInput(l_ac_buffer, "\n\nDevice operation menu:\n"
				"1 - open connection\n"
				"2 - disconnect a device\n"
				"3 - capture\n"
				"4 - get image\n"
				"5 - verify capture against file(s)\n"
				"6 - get quality threshold\n"
				"7 - set quality threshold\n"
				"8 - get LED state\n"
				"9 - set LED state\n"
				"10 - get log size\n"
				"11 - set log size\n"
				"12 - test acquisition cancel\n"
				"13 - convert a template\n"
				"14 - get configuration string\n"
				"15 - set configuration string\n"
				"16 - get configuration integer\n"
				"17 - set configuration integer\n"
				"18 - get log buffer\n"
				"19 - clear log buffer\n"
#ifndef __arm__
				"20 - Load New Key KS in Sensor\n"
#endif
				//"21 - check finger presence\n"
				"0 - back to main menu\n");

		sscanf(l_ac_buffer, "%d", &l_i_command);

		switch (l_i_command)
		{
		/////////////////////////////////////////////
		// List all connected devices and open connection
		/////////////////////////////////////////////
		case 1:
			
			

			if (g_uc_ConnectionType == 0)
			{
				l_i_Ret = io_px_data->m_x_device.InitUsbDevicesNameEnum(&l_ul_NbUsbDevice);
				fprintf(stdout, "InitUsbDevicesNameEnum returned %d\n", l_i_Ret);
				
				if(l_ul_NbUsbDevice)
				{

					fprintf(stdout, "Detected %ld USB devices :\n", l_ul_NbUsbDevice);

					for (l_ul_index = 0; l_ul_index < l_ul_NbUsbDevice; l_ul_index++)
					{
						l_i_Ret = io_px_data->m_x_device.GetUsbDevicesNameEnum(l_ul_index, l_pc_deviceName, l_pc_deviceProperties);
						fprintf(stdout, "%ld - %s - %s\n", l_ul_index, l_pc_deviceName, l_pc_deviceProperties);
					}

					getUserInput(l_ac_buffer, "\nSelect device index to open (-1 to cancel connection): ");
					sscanf(l_ac_buffer, "%d", (int*)&l_l_deviceIndex);

					if (l_l_deviceIndex < -1)
					{
						fprintf(stdout, "Error, invalid index.\n");
					}
					else if (l_l_deviceIndex == -1)
					{
						fprintf(stdout, "Connection aborted.\n");
					}
					else
					{
						l_i_Ret = io_px_data->m_x_device.GetUsbDevicesNameEnum((UL)l_l_deviceIndex, l_pc_deviceName, l_pc_deviceProperties);

						if (l_i_Ret != MORPHO_OK)
							fprintf(stdout, "Invalid selection\n");

						else 
						{
							l_i_Ret = io_px_data->m_x_device.OpenUsbDevice(l_pc_deviceName, 0, NULL);
							// Check connected sensor type
							if(l_i_Ret == MORPHO_OK)
							{
								if (((strcmp(l_pc_deviceProperties, "CBM") == 0)) ||((strcmp(l_pc_deviceProperties, "Sagem MorphoSmart CBM") == 0)))
								{
									io_px_data->m_e_deviceType = DEVICE_CBM;
								}
								else if (strcmp(l_pc_deviceProperties, "CBI") == 0)
								{
									io_px_data->m_e_deviceType = DEVICE_CBI;
								}
								else if (strcmp(l_pc_deviceProperties, "MSI") == 0)
								{
									io_px_data->m_e_deviceType = DEVICE_MSI;
								}
								else if ( (strcmp(l_pc_deviceProperties, "MorphoSmart FINGER VP") == 0) || (strcmp(l_pc_deviceProperties, "MSO FVP") == 0) )
								{
									io_px_data->m_e_deviceType = DEVICE_FVP;
								}
								//TODO: map other devices properties to the correct type accordingly
								else
								{
									io_px_data->m_e_deviceType = DEVICE_UNKNOWN;
								}
							}
						}
							
					}
				}				
				else
					fprintf(stdout, "No device connected\n");

			}
			else
			{	
				printf("Open device RS232 \n");
				//l_i_Ret = io_px_data->m_x_device.OpenDevice(g_i_port, g_i_speed);
				l_i_Ret = io_px_data->m_x_device.OpenDevice(g_c_PortName, g_i_speed);
				printf("return Open device RS232 %d  \n",l_i_Ret);
			}


			if (l_i_Ret == MORPHO_OK) 
			{
				const char *l_pc_defaultPath = "/data/cbi";
				fprintf(stdout, "Connection successfully opened\n");
				
				l_i_Ret = io_px_data->m_x_device.GetDatabase(0, (PC)l_pc_defaultPath, io_px_data->m_x_database);

				if (l_i_Ret == MORPHO_OK)
				{
					UL	l_ul_usedRecords;
					UL	l_ul_freeRecords;
					UL	l_ul_totalRecords;

					fprintf(stdout, "Database found on the device\n");

					io_px_data->m_x_database.GetNbUsedRecord(l_ul_usedRecords);
					io_px_data->m_x_database.GetNbFreeRecord(l_ul_freeRecords);
					io_px_data->m_x_database.GetNbTotalRecord(l_ul_totalRecords);

					fprintf(stdout, "%ld / %ld entries (%ld free).\n", l_ul_usedRecords, l_ul_totalRecords, l_ul_freeRecords);
				}
				else
					fprintf(stdout, "No database found on the device\n");

				UL	l_ul_device_max_users;
				UL	l_ul_device_max_finger;
				l_i_Ret = io_px_data->m_x_database.GetMaxUser(&l_ul_device_max_users, &l_ul_device_max_finger);
				if (l_i_Ret == MORPHO_OK)
				{
					fprintf(stdout, "Device capabilities:\n  Max database users: %ld\n  Max fingers per user: %ld\n\n",
							l_ul_device_max_users,
							l_ul_device_max_finger);
				}
				else
				{
					fprintf(stdout, "Error %d while retrieving device capabilities\n", l_i_Ret);
				}

				// Device malloc test
				{
					PUC	l_puc_test_buffer = NULL;

					if (io_px_data->m_x_device.Malloc((PVOID*)&l_puc_test_buffer, 2048) == MORPHO_OK)
					{
						fprintf(stdout, "Device malloc test - 2kb successfully allocated\n");
						if (io_px_data->m_x_device.Free((PVOID*)&l_puc_test_buffer) == MORPHO_OK)
						{
							fprintf(stdout, "Device free test passed\n");
						}
						else
						{
							fprintf(stdout, "Device free test failed\n");
						}
					}
					else
					{
						fprintf(stdout, "Device malloc test failed\n");
					}
				}

				l_i_Ret = io_px_data->m_x_device.RegisterLEDEvent(&LEDEvent, NULL, &g_ul_ledEventsID);
				if (l_i_Ret == MORPHO_OK)
					fprintf(stdout, "Listening LED events\n");
				else if (l_i_Ret == MORPHOERR_UNAVAILABLE)
					fprintf(stdout, "LED events unavailable on the device\n");
				else
					fprintf(stdout, "Error %d while trying to register LED events callback\n", l_i_Ret);

				//TODO: register FFD callback
				l_i_Ret = io_px_data->m_x_device.RegisterFFDCallback(&FFDEvent);
				if (l_i_Ret == MORPHO_OK)
					fprintf(stdout, "Listening LED events\n");
				else if (l_i_Ret == MORPHOERR_UNAVAILABLE)
					fprintf(stdout, "FFD events unavailable on the device\n");
				else
					fprintf(stdout, "Error %d while trying to register FFD events callback\n", l_i_Ret);
			}
			else
				fprintf(stdout, "Connection error %d\n", l_i_Ret);		
			break;

		/////////////////////////////////////////////
		// Disconnect device
		/////////////////////////////////////////////
		case 2:
			l_i_Ret = io_px_data->m_x_device.UnregisterLEDEvent(g_ul_ledEventsID);
			l_i_Ret = io_px_data->m_x_device.UnregisterFFDCallback();
			l_i_Ret = io_px_data->m_x_device.CloseDevice();

			if (l_i_Ret == MORPHO_OK)
				fprintf(stdout, "Successfully closed device connection\n");
			else
				fprintf(stdout, "Error %d while trying to close connection\n", l_i_Ret);
			break;

		/////////////////////////////////////////////
		// Capture
		/////////////////////////////////////////////
		case 3:
			UC		l_auc_X984Data[1024];
			T_MORPHO_TYPE_TEMPLATE	l_x_TypeTemplate;
			char l_ac_Extension[16];

#ifdef ENABLE_DISPLAY
				InitScreen();
#endif

			l_x_TypeTemplate = getTemplateType();
			getTemplateExtension(l_x_TypeTemplate,l_ac_Extension);

			l_x_templates.SetActiveFullImageRetrieving(TRUE);

			getUserInput(l_ac_buffer, "Choose exported image compression:\n1 - no compression\n2 - WSQ compression\n");
			if (sscanf(l_ac_buffer, "%d", &l_i_Ret) != 1)
			{
				fprintf(stdout, "Invalid input, default to RAW image\n");
			}
			else
			{
				if (l_i_Ret == 2)
				{
					l_x_compressAlgo = MORPHO_COMPRESS_WSQ;

					getUserInput(l_ac_buffer, "Enter exported image compression rate (2-255), leave blank to default to 15:\n");
					if (l_ac_buffer[0] == '\0')
					{
						l_i_CompressRate = 15;
					}
					else if (sscanf(l_ac_buffer, "%d", &l_i_CompressRate) != 1)
					{
						fprintf(stdout, "Invalid input, using default value 15\n");
						l_i_CompressRate = 15;
					}
				}
				else
				{
					if (l_i_Ret != 1)
						fprintf(stdout, "Invalid value, default to no compression\n");
					l_x_compressAlgo = MORPHO_NO_COMPRESS;
					l_i_CompressRate = 0;
				}

				l_x_templates.SetActiveFullImageRetrieving(true);
			}

			getUserInput(l_ac_buffer, "Choose number of fingers:\n");
			if (sscanf(l_ac_buffer, "%ld", &l_ul_nbFingers) == 1)
			{
				if ((l_ul_nbFingers > 3) || (l_ul_nbFingers < 1))
				{
					fprintf(stdout, "Invalid value, default to one capture\n");
					l_ul_nbFingers = 1;
				}
			}
			else
			{
				l_ul_nbFingers = 1;
			}

			l_i_Ret = io_px_data->m_x_device.Capture(
								0,							//US							i_us_Timeout,
								0,							//UC							i_uc_AcquisitionThreshold,
								0,							//UC							i_uc_AdvancedSecurityLevelsRequired,
								(UC)l_ul_nbFingers,				//UC							i_uc_FingerNumber,
								l_x_TypeTemplate,			//T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
								MORPHO_NO_PK_FVP,			//T_MORPHO_FVP_TYPE_TEMPLATE    i_x_FVPTypeTemplate;
								1,							//US							i_us_MaxSizeTemplate,
								0,							//UC							i_uc_enrolType (1),
								l_ul_asyncEvents,			//UL							i_ul_CallbackCmd,
								eventCallback,				//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
								io_px_data,					//PVOID							i_pv_CallbackArgument,
								l_x_templates,				//C_MORPHO_TemplateList &		o_x_Template;
								MORPHO_RAW_TEMPLATE,		//T_MORPHO_TEMPLATE_ENVELOP		i_x_typEnvelop,
								0,							//I								i_i_ApplicationDataLen,
								l_auc_X984Data,				//PUC							i_puc_ApplicationData
								0,							//UC							i_uc_LatentDetection
								MORPHO_DEFAULT_CODER,							//I								i_i_CoderChoice
								MORPHO_ENROLL_DETECT_MODE,	//MORPHO_ENROLL_DETECT_MODE|m_ul_WakeUpMode,	//UL							i_ul_DetectModeChoice
								0,							//UC							i_uc_SaveIndexImage
								NULL,						//PT_MORPHO_MOC_PARAMETERS		i_px_MocParameters
								l_x_compressAlgo,			//T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
								l_i_CompressRate			//UC							i_uc_CompressRate
							);

			// After a successful capture, allow the user to generate and export the fingerprint template
			if (l_i_Ret == MORPHO_OK)
			{
				getUserInput(l_ac_buffer, "\n\nEnter root file name of exported template (leave blank to skip)(extension is automatic):\n");
				if (l_ac_buffer[0] != '\0')
				{
					T_MORPHO_TYPE_TEMPLATE	l_e_templateType;
					UL						l_ul_lenTemplate;
					PUC						l_puc_dataTemplate;
					UC						l_uc_PkFpQuality;
					UC						l_uc_dataIndex = 0xFF;
					FILE					*l_px_outputFile;
					char					l_ac_templateFileName[2048];
					UC						l_uc_index;

					for (l_uc_index = 0; l_uc_index < l_ul_nbFingers; l_uc_index++)
					{
						sprintf(l_ac_templateFileName, "%s_%d%s", l_ac_buffer, l_uc_index, l_ac_Extension);
						l_px_outputFile = fopen(l_ac_templateFileName, "wb");

						if (l_px_outputFile == NULL)
							fprintf(stdout, "Error, unable to create file %s\n", l_ac_templateFileName);
						else
						{
							l_x_templates.GetTemplate(	l_uc_index,//UC  i_uc_indexTemplate,
														l_e_templateType,//T_MORPHO_TYPE_TEMPLATE &  o_uc_typTemplate,
														l_ul_lenTemplate,
														l_puc_dataTemplate,
														l_uc_PkFpQuality,
														l_uc_dataIndex);

							fwrite(l_puc_dataTemplate, 1, l_ul_lenTemplate, l_px_outputFile);
							fclose(l_px_outputFile);
							printf ("File %s saved\n\n",l_ac_templateFileName);
						}
					}
				}

				fprintf(stdout, "Enter path to export image (extension will be automatically appended), leave blank to skip:\n");
				getUserInput(l_ac_buffer, "");
				if (l_ac_buffer[0] != '\0')
				{
					FILE *				exportImage = NULL;
					T_MORPHO_IMAGE *	l_px_image = NULL;
					UC					l_uc_index;
					char				l_ac_imgFileName[2048];

					for (l_uc_index = 0; l_uc_index < l_ul_nbFingers; l_uc_index++)
					{
						l_i_Ret = l_x_templates.GetFullImageRetrieving(l_uc_index, l_px_image);

						if (l_i_Ret == MORPHO_OK)
						{
							if (l_x_compressAlgo == MORPHO_COMPRESS_WSQ)
							{
								sprintf(l_ac_imgFileName, "%s_%d.wsq", l_ac_buffer, l_uc_index);
								exportImage = fopen(l_ac_imgFileName, "wb");
								if (exportImage == NULL)
								{
									fprintf(stdout, "Unable to create file %s\n", l_ac_imgFileName);
								}
								else
								{
									fwrite(l_px_image->m_puc_CompressedImage, 1, l_px_image->m_x_ImageWSQHeader.m_ul_WSQImageSize, exportImage);
								}
								fclose(exportImage);
							}
							else if (l_x_compressAlgo == MORPHO_NO_COMPRESS)
							{
								sprintf(l_ac_imgFileName, "%s_%d_[%dx%d].raw",
										l_ac_buffer,
										l_uc_index,
										l_px_image->m_x_ImageHeader.m_us_NbCol,
										l_px_image->m_x_ImageHeader.m_us_NbRow);
								exportImage = fopen(l_ac_imgFileName, "wb");
								if (exportImage == NULL)
								{
									fprintf(stdout, "Unable to create file %s\n", l_ac_imgFileName);
								}
								else
								{
									fwrite(l_px_image->m_puc_Image, 1, (l_px_image->m_x_ImageHeader.m_us_NbCol * l_px_image->m_x_ImageHeader.m_us_NbRow * l_px_image->m_x_ImageHeader.m_uc_NbBitsPerPixel) / 8, exportImage);
								}
								fclose(exportImage);
							}
						}
					}
				}
			}
			else
				fprintf(stdout, "Error %d while capturing image\n", l_i_Ret);

#ifdef ENABLE_DISPLAY
			CloseScreen();
#endif
			break;

		/////////////////////////////////////////////
		// Get image
		/////////////////////////////////////////////
		case 4:
		{
			T_MORPHO_COMPRESS_ALGO	l_x_compressAlgo = MORPHO_NO_COMPRESS;
			T_MORPHO_IMAGE			l_x_image;

			getUserInput(l_ac_buffer, "Choose exported image compression:\n1 - no compression\n2 - WSQ compression\n");
			if (sscanf(l_ac_buffer, "%d", &l_i_Ret) != 1)
			{
				fprintf(stdout, "Invalid input, default to RAW image\n");
			}
			else
			{
				if (l_i_Ret == 2)
				{
					l_x_compressAlgo = MORPHO_COMPRESS_WSQ;

					getUserInput(l_ac_buffer, "Enter exported image compression rate (2-255), leave blank to default to 15:\n");
					if (l_ac_buffer[0] == '\0')
					{
						l_i_CompressRate = 15;
					}
					else if (sscanf(l_ac_buffer, "%d", &l_i_CompressRate) != 1)
					{
						fprintf(stdout, "Invalid input, using default value 15\n");
						l_i_CompressRate = 15;
					}
				}
				else
				{
					if (l_i_Ret != 1)
					{
						fprintf(stdout, "Invalid value, default to no compression\n");
					}
					l_x_compressAlgo = MORPHO_NO_COMPRESS;
					l_i_CompressRate = 0;
				}
			}

			l_i_Ret = io_px_data->m_x_device.GetImage(
					0,	//US							i_us_Timeout,
					0,//UC							i_uc_AcquisitionThreshold,
					l_x_compressAlgo,//T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
					l_i_CompressRate,//UC                          i_uc_CompressRate,
					l_ul_asyncEvents,//UL							i_ul_CallbackCmd,
					eventCallback,//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
					io_px_data,//PVOID						i_pv_CallbackArgument,
					&l_x_image,//T_MORPHO_IMAGE				*o_px_Image,
					MORPHO_ENROLL_DETECT_MODE,//UC							i_uc_DetectModeChoice,
					LATENT_DETECT_DISABLE//UC							i_uc_LatentDetection
					);

			if (l_i_Ret == MORPHO_OK)
			{
				fprintf(stdout, "Enter path to export image (extension will be automatically appended), leave blank to skip:\n");
				getUserInput(l_ac_buffer, "");
				if (l_ac_buffer[0] != '\0')
				{
					FILE *				exportImage = NULL;
					char				l_ac_imgFileName[2048];

					if (l_x_compressAlgo == MORPHO_COMPRESS_WSQ)
					{
						sprintf(l_ac_imgFileName, "%s.wsq", l_ac_buffer);
						exportImage = fopen(l_ac_imgFileName, "wb");
						if (exportImage == NULL)
						{
							fprintf(stdout, "Unable to create file %s\n", l_ac_imgFileName);
						}
						else
						{
							fwrite(l_x_image.m_puc_CompressedImage, 1, l_x_image.m_x_ImageWSQHeader.m_ul_WSQImageSize, exportImage);
						}
						fclose(exportImage);
					}
					else if (l_x_compressAlgo == MORPHO_NO_COMPRESS)
					{
						sprintf(l_ac_imgFileName, "%s_[%dx%d].raw",
								l_ac_buffer,
								l_x_image.m_x_ImageHeader.m_us_NbCol,
								l_x_image.m_x_ImageHeader.m_us_NbRow);
						exportImage = fopen(l_ac_imgFileName, "wb");
						if (exportImage == NULL)
						{
							fprintf(stdout, "Unable to create file %s\n", l_ac_imgFileName);
						}
						else
						{
							fwrite(l_x_image.m_puc_Image, 1, l_x_image.m_x_ImageHeader.m_us_NbCol * l_x_image.m_x_ImageHeader.m_us_NbRow, exportImage);
						}
						fclose(exportImage);
					}
				}
			}
			else
			{
				fprintf(stdout, "Error %d while performing GetImage\n", l_i_Ret);
			}
		}
			break;

		/////////////////////////////////////////////
		// Verify capture against file(s)
		/////////////////////////////////////////////
		case 5:
		{
			C_MORPHO_TemplateList	l_x_PkRef;
			//UC						l_uc_templateIndex;
			UL						l_ul_matchingScore;
			UC						l_uc_ExportNumPk;
			T_MORPHO_FAR			l_e_matching_threshold = MORPHO_FAR_5;
			I						l_i_matching_threshold;
			US						l_us_timeout;
			I						l_i_coderChoice;

			getUserInput(l_ac_buffer, "Enter reference template file name:\n");

			if (storePkFromFile(l_ac_buffer, l_x_PkRef) == 0) {

				getUserInput(l_ac_buffer, "Enter capture timeout in seconds (0-65535, blank or 0 for no timeout):\n");
				if (sscanf(l_ac_buffer, "%hd", &l_us_timeout) != 1)
				{
					l_us_timeout = 0;
				}
				getUserInput(l_ac_buffer, "Enter matching threshold (0-10, leave blank for default 5):\n");
				if (sscanf(l_ac_buffer, "%d", &l_i_matching_threshold) == 1)
				{
					if ((l_i_matching_threshold >= 0) && (l_i_matching_threshold <= 10))
					{
						l_e_matching_threshold = (T_MORPHO_FAR)l_i_matching_threshold;
					}
				}
#if 0
				getUserInput(l_ac_buffer, "Choose coder (blank or invalid value will default to MORPHO_MSO_V9_CODER):\n"
						"1 - MORPHO_MSO_V9_CODER\n"
						"2 - MORPHO_MSO_V9_JUV_CODER\n"
						"3 - MORPHO_MSO_V9_THIN_FINGER_CODER\n");
				if (sscanf(l_ac_buffer, "%d", &l_i_coderChoice) == 1)
				{
					switch(l_i_coderChoice)
					{
					case 2:
						l_i_coderChoice = MORPHO_MSO_V9_JUV_CODER;
						break;
					case 3:
						l_i_coderChoice = MORPHO_MSO_V9_THIN_FINGER_CODER;
						break;
					default:
						l_i_coderChoice = MORPHO_MSO_V9_CODER;
						break;
					}
				}
				else
				{
					l_i_coderChoice = MORPHO_DEFAULT_CODER;
				}
#else
				l_i_coderChoice = MORPHO_DEFAULT_CODER;
#endif

				l_i_Ret = io_px_data->m_x_device.Verify(
						l_us_timeout,									//i_us_Timeout,
						l_e_matching_threshold,				//T_MORPHO_FAR				i_us_FAR,
						l_x_PkRef,							//C_MORPHO_TemplateList &		i_x_TemplateList,
						l_ul_asyncEvents,					//UL							i_ul_CallbackCmd,
						eventCallback,						//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
						io_px_data,							//PVOID						i_pv_CallbackArgument,
						&l_ul_matchingScore,				//PUL							o_pul_MatchingScore,
						&l_uc_ExportNumPk,					//PUC							o_puc_ExportNumPk,
						l_i_coderChoice,									//I								i_i_CoderChoice,
						MORPHO_VERIF_DETECT_MODE,			//UL							i_ul_DetectModeChoice,
						MORPHO_STANDARD_MATCHING_STRATEGY	//UL							i_ul_MatchingStrategy
				);

				switch(l_i_Ret) {
				case MORPHO_OK:
					fprintf(stdout, "Templates #%d matched\n", l_uc_ExportNumPk);
					break;
				case MORPHOERR_NO_HIT:
					fprintf(stdout, "Templates did not match\n");
					break;
				case MORPHOERR_INVALID_PK_FORMAT:
					fprintf(stdout, "Input template format is not supported\n");
					break;
				default:
					fprintf(stdout, "Error %d while verifying templates\n", l_i_Ret);
					break;
				}
			}
			else
			{
				fprintf(stdout, "Unable to open template file %s\n", l_ac_buffer);
			}
		}
			break;

		/////////////////////////////////////////////
		// Get quality threshold
		/////////////////////////////////////////////
		case 6:
		{
			UL		l_ul_quality = 0;
			l_i_Ret = io_px_data->m_x_device.GetQualityThreshold(&l_ul_quality);
			switch(l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, "Current quality threshold: %lu\n", l_ul_quality);
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Feature unavailable on current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while retrieving quality threshold\n", l_i_Ret);
				break;
			}
		}
			break;

		/////////////////////////////////////////////
		// Set quality threshold
		/////////////////////////////////////////////
		case 7:
		{
			UL	l_ul_newQuality;
			getUserInput(l_ac_buffer, "Enter new quality threshold:\n");
			if (sscanf(l_ac_buffer, "%lu", &l_ul_newQuality) != 1)
			{
				fprintf(stdout, "Invalid value\n");
			}
			else
			{
				l_i_Ret = io_px_data->m_x_device.SetQualityThreshold(l_ul_newQuality);
				switch (l_i_Ret)
				{
				case MORPHO_OK:
					fprintf(stdout, "Quality threshold successfully set to %lu\n", l_ul_newQuality);
					break;
				case MORPHOERR_UNAVAILABLE:
					fprintf(stdout, "Function not available on current device\n");
					break;
				default:
					fprintf(stdout, "Error %d while trying to set new quality threshold\n", l_i_Ret);
					break;
				}
			}
		}
			break;

		/////////////////////////////////////////////
		// Get LED state
		/////////////////////////////////////////////
		case 8:
			l_i_Ret = io_px_data->m_x_device.GetLEDState(&l_i_LED);

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, (l_i_LED)?
						"LED is on\n":
						"LED is off\n");
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "LED state unavailable on current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while trying to retrieve LED state\n", l_i_Ret);
				break;
			}
			break;

		/////////////////////////////////////////////
		// Set LED state
		/////////////////////////////////////////////
		case 9:
		{
			I	l_i_LED = -1;

			do
			{
				getUserInput(l_ac_buffer, "0 - turn LED off\n1 - turn LED on\n");
				if (sscanf(l_ac_buffer, "%d", &l_i_LED) != 1)
					fprintf(stdout, "Invalid input");
			} while ((l_i_LED < 0) && (l_i_LED > 1));

			l_i_Ret = io_px_data->m_x_device.SetLEDState((UC)l_i_LED);

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, "Successfully modified LED state\n");
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "LED state unavailable on current device");
				break;
			default:
				fprintf(stdout, "Error %d while trying to set LED state\n", l_i_Ret);
				break;
			}
		}
			break;

		/////////////////////////////////////////////
		// Get log size
		/////////////////////////////////////////////
		case 10:
		{
			UL	l_ul_logSize = 0;
			l_i_Ret = io_px_data->m_x_device.GetLOGSize(&l_ul_logSize);

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, "Current log size: %lu bytes\n", l_ul_logSize);
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Log size features unavailable on current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while trying to retrieve log size\n", l_i_Ret);
				break;
			}
		}
			break;

		/////////////////////////////////////////////
		// Set log size
		/////////////////////////////////////////////
		case 11:
		{
			UL	l_ul_logSize = 0;

			getUserInput(l_ac_buffer, "Enter new RAM log buffer size in bytes :\n");
			if (sscanf(l_ac_buffer, "%lu", &l_ul_logSize) != 1)
				fprintf(stdout, "Invalid input\n");
			else
			{
				l_i_Ret = io_px_data->m_x_device.SetLOGSize(l_ul_logSize);

				switch (l_i_Ret)
				{
				case MORPHO_OK:
					fprintf(stdout, "Log size successfully set to %lu bytes\n", l_ul_logSize);
					break;
				case MORPHOERR_UNAVAILABLE:
					fprintf(stdout, "Log size features unavailable on current device\n");
					break;
				default:
					fprintf(stdout, "Error %d while trying to set new log size\n", l_i_Ret);
					break;
				}
			}
		}
			break;

		/////////////////////////////////////////////
		// Test acquisition cancel
		/////////////////////////////////////////////
		case 12:
		{
			pthread_t				l_x_cancelThread, l_x_qualityThread;
			pthread_attr_t			l_x_threadParams;
			UL						l_ul_fingers;

			io_px_data->m_x_database.GetMaxUser(NULL, &l_ul_fingers);

			pthread_attr_init(&l_x_threadParams);
			fprintf(stdout, "Launching identification to be cancelled\n");
			pthread_create(&l_x_cancelThread, &l_x_threadParams, cancelThread, (void*)io_px_data);
			pthread_create(&l_x_qualityThread, &l_x_threadParams, qualityThread, (void*)io_px_data);

			l_i_Ret = io_px_data->m_x_device.Capture(
					0,							//US							i_us_Timeout,
					0,							//UC							i_uc_AcquisitionThreshold,
					0,							//UC							i_uc_AdvancedSecurityLevelsRequired,
					1,							//UC							i_uc_FingerNumber,
					MORPHO_PK_COMP,				//T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
					MORPHO_NO_PK_FVP,			//T_MORPHO_FVP_TYPE_TEMPLATE    i_x_FVPTypeTemplate;
					0,						//US							i_us_MaxSizeTemplate,
					0,							//UC							i_uc_enrolType (1),
					l_ul_asyncEvents,			//UL							i_ul_CallbackCmd,
					eventCallback,				//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
					io_px_data,					//PVOID							i_pv_CallbackArgument,
					l_x_templates,				//C_MORPHO_TemplateList &		o_x_Template;
					MORPHO_RAW_TEMPLATE,		//T_MORPHO_TEMPLATE_ENVELOP		i_x_typEnvelop,
					0,							//I								i_i_ApplicationDataLen,
					NULL,				//PUC							i_puc_ApplicationData
					0,							//UC							i_uc_LatentDetection
					MORPHO_DEFAULT_CODER,							//I								i_i_CoderChoice
					MORPHO_ENROLL_DETECT_MODE,	//MORPHO_ENROLL_DETECT_MODE|m_ul_WakeUpMode,	//UL							i_ul_DetectModeChoice
					0,							//UC							i_uc_SaveIndexImage
					NULL,						//PT_MORPHO_MOC_PARAMETERS		i_px_MocParameters
					MORPHO_NO_COMPRESS,			//T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
					0							//UC							i_uc_CompressRate
				);
		}
			break;

		/////////////////////////////////////////////
		// Convert a template
		/////////////////////////////////////////////
		case 13:
		{
			FILE					*l_px_inputTemplateFile = NULL;
			FILE					*l_px_outputTemplateFile = NULL;
			T_MORPHO_TYPE_TEMPLATE	l_x_inputFormat;
			T_MORPHO_TYPE_TEMPLATE	l_x_outputFormat;
			PUC						l_puc_input_buffer = NULL;
			UL						l_ul_input_size;
			PUC						l_puc_output_buffer = NULL;
			UL						l_ul_output_size;
			I						l_i_TypeTemplateChoice;

			// Input template preparation
			getUserInput(l_ac_buffer, "Enter input template file name:\n");
			l_px_inputTemplateFile = fopen(l_ac_buffer, "rb");
			if (l_px_inputTemplateFile == NULL)
			{
				fprintf(stdout, "Unable to open file %s\n", l_ac_buffer);
				break;
			}
			fprintf(stdout, "Select input template format:\n0 - CFV\n1 - BIOSCRYPT\n2 - PKCOMP\n");
			getUserInput(l_ac_buffer, "");
			if (sscanf(l_ac_buffer, "%d", &l_i_TypeTemplateChoice) != 1)
			{
				fprintf(stdout, "Invalid input\n");
				break;
			}

			if ((l_i_TypeTemplateChoice < 0) || (l_i_TypeTemplateChoice > 2))
			{
				fprintf(stdout, "Invalid selection\n");
				break;
			}

			switch(l_i_TypeTemplateChoice)
			{
			case 0: l_x_inputFormat = MORPHO_PK_CFV; break;
			case 1: l_x_inputFormat = MORPHO_PK_BIOSCRYPT; break;
			case 2: l_x_inputFormat = MORPHO_PK_COMP; break;
			}

			// Output template preparation
			getUserInput(l_ac_buffer, "Enter output template file name:\n");
			l_px_outputTemplateFile = fopen(l_ac_buffer, "wb");
			if (l_px_outputTemplateFile == NULL)
			{
				fprintf(stdout, "Unable to create file %s\n", l_ac_buffer);
				fclose(l_px_inputTemplateFile);
				break;
			}
			fprintf(stdout, "Select output template format:\n0 - CFV\n1 - BIOSCRYPT\n2 - PKCOMP\n");
			getUserInput(l_ac_buffer, "");
			if (sscanf(l_ac_buffer, "%d", &l_i_TypeTemplateChoice) != 1)
			{
				fprintf(stdout, "Invalid input\n");
				break;
			}

			if ((l_i_TypeTemplateChoice < 0) || (l_i_TypeTemplateChoice > 2))
			{
				fprintf(stdout, "Invalid selection\n");
				break;
			}

			switch(l_i_TypeTemplateChoice)
			{
			case 0: l_x_outputFormat = MORPHO_PK_CFV; break;
			case 1: l_x_outputFormat = MORPHO_PK_BIOSCRYPT; break;
			case 2: l_x_outputFormat = MORPHO_PK_COMP; break;
			}

			fseek(l_px_inputTemplateFile, 0, SEEK_END);
			l_ul_input_size = ftell(l_px_inputTemplateFile);
			fseek(l_px_inputTemplateFile, 0, SEEK_SET);

			l_puc_input_buffer = (PUC)malloc(l_ul_input_size);
			if (l_puc_input_buffer == NULL)
			{
				fprintf(stdout, "Memory error\n");
				break;
			}

			fread(l_puc_input_buffer, 1, l_ul_input_size, l_px_inputTemplateFile);

			l_i_Ret = io_px_data->m_x_device.ConvertTemplate(l_puc_input_buffer,
														l_ul_input_size,
														l_x_inputFormat,
														l_x_outputFormat,
														&l_puc_output_buffer,
														&l_ul_output_size);

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fwrite(l_puc_output_buffer, 1, l_ul_output_size, l_px_outputTemplateFile);
				if (io_px_data->m_x_device.ReleaseTemplate(&l_puc_output_buffer) != MORPHO_OK)
				{
					fprintf(stdout, "An error occurred while freeing conversion result\n");
				}
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Template conversion unavailable on current device\n");
				break;
			default:
				fprintf(stdout, "Error %d occurred while trying to convert template\n", l_i_Ret);
				break;
			}

			free(l_puc_input_buffer);
			fclose(l_px_inputTemplateFile);
			fclose(l_px_outputTemplateFile);
		}
			break;

		/////////////////////////////////////////////
		// Get configuration string
		/////////////////////////////////////////////
		case 14:
			/*
			getUserInput(l_ac_buffer,	"Select parameter:\n"
										"1 - Window position\n"
										"2 - UI configuration\n"
										"3 - UI reset\n");

			l_us_paramTag = CONFIG_SENSOR_WIN_POSITION_TAG;
			l_i_Ret = io_px_data->m_x_device.GetConfigParam(l_us_paramTag, &l_i_Param);

			switch (l_us_paramTag)
			{
			case 1:
				l_us_paramTag = CONFIG_SENSOR_WIN_POSITION_TAG;
				break;
			case 2:
				l_us_paramTag = CONFIG_UI_CONFIG_TAG;
				break;
			case 3:
				l_us_paramTag = CONFIG_UI_RESET_TAG;
				break;
			default:
				fprintf(stdout, "Invalid entry\n");
				l_us_paramTag = 0xFFFF;
				break;
			}

			if (l_us_paramTag == 0xFFFF)
			{
				break;
			}
			*/

			l_ul_stringParamSize = INPUT_BUFFER_SIZE;
			//l_i_Ret = io_px_data->m_x_device.GetConfigParam(l_us_paramTag, &l_ul_stringParamSize, &l_puc_stringParam);
			l_i_Ret = io_px_data->m_x_device.GetConfigParam(CONFIG_SENSOR_WIN_POSITION_TAG, &l_ul_stringParamSize, &l_puc_stringParam);

			if (l_i_Ret == MORPHO_OK)
			{
				//fprintf(stdout, "Value for parameter %hd: %.*s\n", l_us_paramTag, (int)l_ul_stringParamSize, (char*)l_puc_stringParam);
				fprintf(stdout, "Value for parameter %hd: %d\n", CONFIG_SENSOR_WIN_POSITION_TAG, *((int*)l_puc_stringParam));
			}
			else
			{
				fprintf(stdout, "Error %d while trying to read string parameter\n", l_i_Ret);
			}

			break;

		/////////////////////////////////////////////
		// Set configuration string
		/////////////////////////////////////////////
		case 15:
			getUserInput((char*)l_ac_buffer, "Enter string parameter value:\n");

			l_ul_stringParamSize = sizeof(I);
			sscanf(l_ac_buffer, "%d", &l_i_intParam);

			l_i_Ret = io_px_data->m_x_device.SetConfigParam(CONFIG_SENSOR_WIN_POSITION_TAG, l_ul_stringParamSize, (PUC)(&l_i_intParam));

			if (l_i_Ret == MORPHO_OK)
			{
				fprintf(stdout, "Parameter successfully set\n");
			}
			else
			{
				fprintf(stdout, "Error %d while trying to write string parameter\n", l_i_Ret);
			}
			break;

		/////////////////////////////////////////////
		// Get configuration integer
		/////////////////////////////////////////////
		case 16:
			l_us_paramTag = CONFIG_SENSOR_WIN_POSITION_TAG;

			l_ul_stringParamSize = INPUT_BUFFER_SIZE;
			l_i_Ret = io_px_data->m_x_device.GetConfigParam(l_us_paramTag, &l_i_intParam);

			if (l_i_Ret == MORPHO_OK)
			{
				fprintf(stdout, "Value for parameter %hd: %d\n", l_us_paramTag, l_i_intParam);
			}
			else
			{
				fprintf(stdout, "Error %d while trying to read string parameter\n", l_i_Ret);
			}
			break;

		/////////////////////////////////////////////
		// Set configuration integer
		/////////////////////////////////////////////
		case 17:
			/*
			getUserInput(l_ac_buffer, "Enter parameter tag value:\n");

			if (sscanf(l_ac_buffer, "%hd", &l_us_paramTag) != 1)
			{
				fprintf(stdout, "Invalid entry\n");
				break;
			}
			*/
			l_us_paramTag = CONFIG_SENSOR_WIN_POSITION_TAG;

			getUserInput(l_ac_buffer, "Enter integer parameter value:\n");
			if (sscanf(l_ac_buffer, "%d", &l_i_intParam) != 1)
			{
				fprintf(stdout, "Invalid parameter value\n");
				break;
			}

			l_i_Ret = io_px_data->m_x_device.SetConfigParam(l_us_paramTag, l_i_intParam);

			if (l_i_Ret == MORPHO_OK)
			{
				fprintf(stdout, "Parameter successfully set for parameter tag %hd\n", l_us_paramTag);
			}
			else
			{
				fprintf(stdout, "Error %d while trying to write string parameter\n", l_i_Ret);
			}
			break;

		/////////////////////////////////////////////
		// Get log buffer
		/////////////////////////////////////////////
		case 18:
		{
			PUC	l_puc_logBuffer = NULL;
			UL	l_ul_logSize;
			l_i_Ret = io_px_data->m_x_device.GetLOG(&l_ul_logSize, &l_puc_logBuffer);

			if (l_i_Ret == MORPHO_OK)
			{
				fprintf(stdout, "Log successfully retrieved:\n\n");
				fwrite(l_puc_logBuffer, 1, l_ul_logSize, stdout);

				io_px_data->m_x_device.Free((PVOID*)&l_puc_logBuffer);
			}
			else
			{
				fprintf(stdout, "Error %d while retrieving device log.\n", l_i_Ret);
			}
		}
			break;
		/////////////////////////////////////////////
		// Clear log buffer
		/////////////////////////////////////////////
		case 19:
			l_i_Ret = io_px_data->m_x_device.ClearLog();

			if (l_i_Ret == MORPHO_OK)
			{
				fprintf(stdout, "Log content cleared.\n");
			}
			else
			{
				fprintf(stdout, "Error %d while trying to clear log content.\n", l_i_Ret);
			}
			break;

#ifndef __arm__
		/////////////////add for MSO1300E_Inde////////////////
		case 20:
			switch (l_i_i = CommandLoadKs())
			{
				case 1:
					//Load New Key KS in Sensor
					{
						l_i_Ret = MORPHO_OK;

						//read Ks key from file
						char l_ac_String[256];

						fprintf (stdout, "---Open 'ks.bin' file -----\n");
						fprintf (stdout, "Enter File name : -> ");
						fgets (l_ac_String, 256, stdin);
						l_ac_String[strlen (l_ac_String) - 1] = 0;	// Suppress '\n'

						std::ifstream ksFile( l_ac_String, std::ios::binary );

						if( !ksFile.is_open() )
							fprintf(stdout, "Unable to open 'ks.bin' file\n");
						else
						{
							std::vector<unsigned char> l_x_ks;
							l_x_ks.assign( std::istreambuf_iterator<char>(ksFile), std::istreambuf_iterator<char>() );
							ksFile.close();

							/*//retrieve device certificate (if secure device)
							if( l_puc_MsoCertif == NULL )
								g_x_MorphoDevice.SecuReadCertificate( 0, &l_ul_MsoCertifSize, &l_puc_MsoCertif );
								//index 0 for device certificate*/


							if( l_puc_MsoCertif == NULL )  //non secure device: key is sent in plaintext
							{
								l_i_Ret = io_px_data->m_x_device.LoadKs( l_x_ks );
								if( l_i_Ret != MORPHO_OK )
								{
									#if DEBUG
									fprintf(stdout, "Error ret=%d while calling C_MORPHO_Device::LoadKs() function\n", l_i_Ret);
									#endif
								}
							}
							/*else  //secure device: key is sent in ciphertext
							{
								std::cout << "Please give a filename containing the 'host.key' : -> ";
								std::string l_str_filename;
								std::cin >> l_str_filename;

								std::ifstream secretFile( l_str_filename.c_str(), std::ios::binary );

								if( !secretFile.is_open() )
									fprintf(stdout, "Unable to open the file\n");
								else
								{
									//l_x_ks.assign( std::istreambuf_iterator<char>(ksFile), std::istreambuf_iterator<char>() );
									secretFile.close();
								}
							}*/

							if(l_i_Ret != MORPHO_OK)
								fprintf(stdout, "Load Ks in Unsecure Mode-----> error %d\n", l_i_Ret);
							else
								fprintf(stdout, "Load Ks in Unsecure Mode-----> OK\n");
						}
					}
					break;

				case 2:
					{
						bool successOfFunction;
							l_i_Ret = LoadKsSymmSec( successOfFunction, io_px_data );

						if( l_i_Ret != MORPHO_OK )
							fprintf(stdout, "Load Ks in Symmetric Secure Mode-----> error %d\n", l_i_Ret);
						else
							if( successOfFunction )
								fprintf(stdout, "Load Ks in Symmetric Secure Mode-----> OK\n");
					}
					break;

				case 3:
					l_i_Ret = DisplayKsKcv(io_px_data);

					if( l_i_Ret != MORPHO_OK )
						fprintf(stdout, "DisplayKsKcv-----> error %d\n", l_i_Ret);

					break;
			}
			break;
		/////////////////end add for MSO1300E_Inde////////////
#endif //__arm__

#if 0	// Not implemented yet
		/////////////////////////////////////////////
		// Check finger presence
		/////////////////////////////////////////////
		case 21:
		{
			UC	l_uc_isFingerPresent;
			l_i_Ret = io_px_data->m_x_device.CheckFingerPresence(&l_uc_isFingerPresent);

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, (l_uc_isFingerPresent)?
						"Finger present on device\n":
						"No finger detected on device\n");
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Finger detection unavailable on current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while checking finger presence\n", l_i_Ret);
				break;
			}
		}
			break;
#endif	// Check finger presence

		/////////////////////////////////////////////
		// Back to main menu
		/////////////////////////////////////////////
		case 0:
			l_uc_loop = 0;
			break;
		default:
			fprintf(stdout, "Invalid command\n");
			break;
		}
	}

	return 0;
}

/**
 * C_MORPHO_Database interface
 */
I databaseOperation(PT_DATA io_px_data)
{
	I							l_i_Ret;
	UC							l_uc_loop = 1;
	I							l_i_command = 0;
	UL							l_ul_usedRecords;
	UL							l_ul_freeRecords;
	UL							l_ul_totalRecords;
	UL							l_ul_matchingScore;
	I							l_i_nbFingersToMatch;
	UC							l_uc_maxFinger;
	UC							l_uc_matchedFingerIndex;
	C							l_ac_buffer[INPUT_BUFFER_SIZE];
	I							l_i_searchFieldIndex;
	UC							l_auc_searchData[MAX_FIELD_SIZE];
	UL							l_ul_nbFields;
	UL							l_ul_index;
	UL							l_ul_PINLength;
	PUC							l_puc_PIN = NULL;
	UL							l_ul_fieldLen;
	US							l_us_fieldMaxSize;
	UC							l_ac_fieldName[MORPHO_FIELD_NAME_LEN+1];
	PUC							l_puc_fieldData;
	T_MORPHO_FIELD_ATTRIBUTE	l_e_fieldType;
	UL							l_ul_asyncEvents =	MORPHO_CALLBACK_DETECTQUALITY |
													MORPHO_CALLBACK_CODEQUALITY |
													MORPHO_CALLBACK_IMAGE_CMD |
													MORPHO_CALLBACK_COMMAND_CMD |
													MORPHO_CALLBACK_ENROLLMENT_CMD;
	T_MORPHO_FAR				l_x_matchingThreshold = MORPHO_FAR_5;

	int	T1, T2;

	while (l_uc_loop)
	{
		getUserInput(l_ac_buffer, "\n\nDatabase operation menu:\n"
				"1 - check database entries\n"
				"2 - check user data\n"
				"3 - identify live acquisition\n"
				"4 - clear database\n"
				"5 - delete database\n"
				"6 - create database\n"
				"7 - search a user\n"
				"8 - remove a user\n"
				"9 - resize the database\n"
				"10 - get most frequent users list size\n"
				"11 - set most frequent users list size\n"
				"12 - autoupdate most frequent users list size\n"
				"13 - deactivate most frequent users list\n"
				"14 - get user PIN\n"
				"15 - get user PIN list\n"
				"16 - change database path\n"
				"17 - save matching statistics\n"
				"18 - get user matching statistics\n"
				"19 - set user matching statistics\n"
				"20 - filter database\n"
				"21 - get active filter\n"
				"22 - retrieve all users filter value\n"
				"23 - get base config\n"
				"24 - get user in database\n"
				"0 - back to main menu\n");
		sscanf(l_ac_buffer, "%d", &l_i_command);

		switch (l_i_command)
		{
		/////////////////////////////////////////////
		// Check database entries
		/////////////////////////////////////////////
		case 1:
			T1 = SampleGetTickCount();
			io_px_data->m_x_database.GetNbUsedRecord(l_ul_usedRecords);
			T2 = SampleGetTickCount();
			fprintf(stdout, "Database opened in %dms\n", T2 - T1);

			io_px_data->m_x_database.GetNbFreeRecord(l_ul_freeRecords);
			io_px_data->m_x_database.GetNbTotalRecord(l_ul_totalRecords);

			fprintf(stdout, "%ld / %ld entries (%ld free).\n\nDatabase fields:\n", l_ul_usedRecords, l_ul_totalRecords, l_ul_freeRecords);
			printDatabaseFields(io_px_data->m_x_database, MORPHO_PUBLIC_FIELD, &l_ul_nbFields, NULL);
			printDatabaseFields(io_px_data->m_x_database, MORPHO_PRIVATE_FIELD, &l_ul_nbFields, NULL);
			printDatabaseFields(io_px_data->m_x_database, MORPHO_STAT_FIELD, &l_ul_nbFields, NULL);
			printDatabaseFields(io_px_data->m_x_database, MORPHO_FILTER_FIELD, &l_ul_nbFields, NULL);

			listDatabaseUsers(io_px_data);
			break;

		/////////////////////////////////////////////
		// Check user data
		/////////////////////////////////////////////
		case 2:
			// Get user by index
			if ((io_px_data->m_e_deviceType == DEVICE_CBI) || (io_px_data->m_e_deviceType == DEVICE_MSI))
			{
				getUserInput(l_ac_buffer, "Enter user index:\n");

				if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_index) != 1)
				{
					fprintf(stdout, "Invalid index\n");
					break;
				}

				l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_index, &l_ul_PINLength, &l_puc_PIN);

				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);

					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "User UID '%s' not found\n", l_puc_PIN);
					else
					{
						l_i_Ret = io_px_data->m_x_database.GetNbField(l_ul_nbFields);
						printUserFields(io_px_data->m_x_user, l_ul_nbFields);
					}
					io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
				}
				else
				{
					fprintf(stdout, "Cannot find user index %ld\n", l_ul_index);
				}
			}
			// Get user by PIN
			else
			{
				getUserInput(l_ac_buffer, "Enter user PIN:\n");

				l_i_Ret = io_px_data->m_x_database.GetUser((UL)strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);

				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "User UID '%s' not found\n", l_ac_buffer);
				else
				{
					l_i_Ret = io_px_data->m_x_database.GetNbField(l_ul_nbFields);
					printUserFields(io_px_data->m_x_user, l_ul_nbFields);
				}
				io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
			}
			break;

		/////////////////////////////////////////////
		// Identify live acquisition
		/////////////////////////////////////////////
		case 3:
#ifdef ENABLE_DISPLAY
			InitScreen();
#endif
			getUserInput(l_ac_buffer, "Enter matching threshold (0-10):");
			if (sscanf(l_ac_buffer, "%d", &l_i_nbFingersToMatch) != 1)
			{
				fprintf(stdout, "Invalid input, defaut to 5\n");
				l_x_matchingThreshold = MORPHO_FAR_5;
			}
			else
			{
				if ((l_i_nbFingersToMatch < 0) || (l_i_nbFingersToMatch > 10))
				{
					fprintf(stdout, "Invalid value, default to 5\n");
					l_x_matchingThreshold = MORPHO_FAR_5;
				}
				else
				{
					switch (l_i_nbFingersToMatch)
					{
					case 0:
						l_x_matchingThreshold = MORPHO_FAR_0;
						break;
					case 1:
						l_x_matchingThreshold = MORPHO_FAR_1;
						break;
					case 2:
						l_x_matchingThreshold = MORPHO_FAR_2;
						break;
					case 3:
						l_x_matchingThreshold = MORPHO_FAR_3;
						break;
					case 4:
						l_x_matchingThreshold = MORPHO_FAR_4;
						break;
					case 5:
					default:
						l_x_matchingThreshold = MORPHO_FAR_5;
						break;
					case 6:
						l_x_matchingThreshold = MORPHO_FAR_6;
						break;
					case 7:
						l_x_matchingThreshold = MORPHO_FAR_7;
						break;
					case 8:
						l_x_matchingThreshold = MORPHO_FAR_8;
						break;
					case 9:
						l_x_matchingThreshold = MORPHO_FAR_9;
						break;
					case 10:
						l_x_matchingThreshold = MORPHO_FAR_10;
						break;
					}
				}
			}

			getUserInput(l_ac_buffer, "Enter number of templates to use from the database (skip to use all templates):");
			if (sscanf(l_ac_buffer, "%d", &l_i_nbFingersToMatch) != 1)
			{
				if (l_ac_buffer[0] == '\0')
				{
					io_px_data->m_x_database.GetNbFinger(l_uc_maxFinger);
					l_i_nbFingersToMatch = (I)l_uc_maxFinger;
				}
				else
				{
					fprintf(stdout, "Invalid input.\n");
					break;
				}
			}

			l_i_Ret = io_px_data->m_x_database.Identify(
									5,	//US							i_us_Timeout,
									l_x_matchingThreshold,	//T_MORPHO_FAR				i_us_FAR,
									l_ul_asyncEvents,	//UL							i_ul_CallbackCmd,
									eventCallback,	//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
									io_px_data,	//PVOID						i_pv_CallbackArgument,
									io_px_data->m_x_user,	//C_MORPHO_User &				o_x_User,
									&l_ul_matchingScore,	//PUL							o_pul_MatchingScore,
									&l_uc_matchedFingerIndex,//PUC							o_puc_FingerIndex,
									MORPHO_DEFAULT_CODER,	//I							i_i_CoderChoice,
									MORPHO_ENROLL_DETECT_MODE,	//UL							i_ul_DetectModeChoice,
									MORPHO_STANDARD_MATCHING_STRATEGY,	//UL							i_ul_MatchertModeChoice
									l_i_nbFingersToMatch
							);
			/*
			 * US							i_us_Timeout,
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
			 * */

#ifdef ENABLE_DISPLAY
			CloseScreen();
#endif

			if (l_i_Ret == MORPHO_OK)
			{
				UL	l_ul_fieldLen;
				PUC	l_puc_fieldData;
				UC	l_auc_stringBuffer[MAX_FIELD_SIZE];
				UL	l_ul_stats;
				UC	l_auc_fieldName[MAX_FIELD_NAME_LEN+1];

				fprintf(stdout, "Match found (template %d). User info:\n", l_uc_matchedFingerIndex);

				io_px_data->m_x_user.GetField(0, l_ul_fieldLen, l_puc_fieldData);
				memcpy(l_auc_stringBuffer, l_puc_fieldData, l_ul_fieldLen);
				l_auc_stringBuffer[l_ul_fieldLen] = 0;
				fprintf(stdout, "User id #%s\n", l_auc_stringBuffer);

				l_i_Ret = io_px_data->m_x_user.GetStats(0, l_puc_fieldData, l_ul_fieldLen, 0, &l_ul_stats);
				if (l_i_Ret == MORPHO_OK)
					fprintf(stdout, "Stats: %ld matches\n", l_ul_stats);

				io_px_data->m_x_database.GetNbField(l_ul_nbFields);

				for (l_ul_index = 1; l_ul_index < l_ul_nbFields+1; l_ul_index++)
				{
					memset(l_auc_fieldName, 0, MAX_FIELD_NAME_LEN+1);
					l_i_Ret = io_px_data->m_x_database.GetField(l_ul_index, l_e_fieldType, l_us_fieldMaxSize, l_auc_fieldName);

					if ((l_e_fieldType == MORPHO_PUBLIC_FIELD) || (l_e_fieldType == MORPHO_PRIVATE_FIELD))
					{
						io_px_data->m_x_user.GetField(l_ul_index, l_ul_fieldLen, l_puc_fieldData);
						memcpy(l_auc_stringBuffer, l_puc_fieldData, l_ul_fieldLen);
						l_auc_stringBuffer[l_ul_fieldLen] = 0;
						fprintf(stdout, "Field %ld: %s\n", l_ul_index, l_auc_stringBuffer);
					}
					else if (l_e_fieldType == MORPHO_FILTER_FIELD)
					{
						l_i_Ret = io_px_data->m_x_user.GetFilterData(l_ul_index, l_ul_fieldLen, l_puc_fieldData);
						if (l_i_Ret == MORPHO_OK)
						{
							UL	charIndex;
							fprintf(stdout, "Filter value: 0x");
							for (charIndex = 0; charIndex < l_ul_fieldLen; charIndex++)
							{
								fprintf(stdout, "%02X", l_puc_fieldData[charIndex]);
							}
							fprintf(stdout, "\n");
						}
					}
				}
			}
			else if (l_i_Ret == MORPHOERR_TIMEOUT)
			{
				fprintf(stdout, "Capture timed out\n");
			}
			else
			{
				if (io_px_data->m_e_deviceType == DEVICE_FVP)
				{
					fprintf(stdout, "No match found in database.\n");
				}
				else
				{
					getUserInput(l_ac_buffer, "No match found in database. Enter 'Yes' or 'Y' to retry with advanced matching strategy :\n");
					if ((!strcmp(l_ac_buffer, "Yes")) || (!strcmp(l_ac_buffer, "Y")))
					{
						l_i_Ret = io_px_data->m_x_database.Identify(
															5,	//US							i_us_Timeout,
															MORPHO_FAR_5,	//T_MORPHO_FAR				i_us_FAR,
															l_ul_asyncEvents,	//UL							i_ul_CallbackCmd,
															eventCallback,	//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
															io_px_data,	//PVOID						i_pv_CallbackArgument,
															io_px_data->m_x_user,	//C_MORPHO_User &				o_x_User,
															&l_ul_matchingScore,	//PUL							o_pul_MatchingScore,
															&l_uc_matchedFingerIndex,//PUC							o_puc_FingerIndex,
															MORPHO_DEFAULT_CODER,	//I							i_i_CoderChoice,
															0,	//UL							i_ul_DetectModeChoice,
															MORPHO_ADVANCED_MATCHING_STRATEGY,	//UL							i_ul_MatchertModeChoice
															l_i_nbFingersToMatch
													);

						if (l_i_Ret == MORPHO_OK)
						{
							UL	l_ul_fieldLen;
							PUC	l_puc_fieldData;
							UC	l_auc_stringBuffer[MAX_FIELD_SIZE];

							fprintf(stdout, "Match found. User info:\n");

							io_px_data->m_x_user.GetField(0, l_ul_fieldLen, l_puc_fieldData);
							memcpy(l_auc_stringBuffer, l_puc_fieldData, l_ul_fieldLen);
							l_auc_stringBuffer[l_ul_fieldLen] = 0;
							fprintf(stdout, "User id #%s\n", l_auc_stringBuffer);

							io_px_data->m_x_database.GetNbField(l_ul_nbFields);

							for (l_ul_index = 0; l_ul_index < l_ul_nbFields; l_ul_index++)
							{
								io_px_data->m_x_user.GetField(l_ul_index+1, l_ul_fieldLen, l_puc_fieldData);
								memcpy(l_auc_stringBuffer, l_puc_fieldData, l_ul_fieldLen);
								l_auc_stringBuffer[l_ul_fieldLen] = 0;
								fprintf(stdout, "Field %ld: %s\n", l_ul_index+1, l_auc_stringBuffer);
							}
						}
						else if (l_i_Ret == MORPHOERR_TIMEOUT)
						{
							fprintf(stdout, "Capture timed out\n");
						}
						else
						{
							fprintf(stdout, "No match found after advanced matching attempt.\n");
						}
					}
				}
			}
			break;

		/////////////////////////////////////////////
		// Clear database
		/////////////////////////////////////////////
		case 4:
			l_i_Ret = io_px_data->m_x_database.DbDelete(io_px_data->m_x_database.MORPHO_ERASE_BASE);

			if (l_i_Ret == MORPHO_OK)
				fprintf(stdout, "Database content cleared\n");
			else
				fprintf(stdout, "Error %d while trying to clear database\n", l_i_Ret);
			break;

		/////////////////////////////////////////////
		// Delete databse
		/////////////////////////////////////////////
		case 5:
			l_i_Ret = io_px_data->m_x_database.DbDelete(io_px_data->m_x_database.MORPHO_DESTROY_BASE);

			if (l_i_Ret == MORPHO_OK)
				fprintf(stdout, "Database deleted\n");
			else
				fprintf(stdout, "Error %d while trying to delete database\n", l_i_Ret);
			break;

		/////////////////////////////////////////////
		// Create database
		/////////////////////////////////////////////
		case 6:
		{
			I	l_i_NbFields;
			I	l_i_fieldType;
			I	l_i_fieldIndex;
			UL	l_ul_fieldSize;
			UC l_uc_encrypted = 0;
			UC	l_auc_fieldName[MAX_FIELD_SIZE];

			l_i_Ret = MORPHO_OK;
			if ((io_px_data->m_e_deviceType == DEVICE_CBI) || (io_px_data->m_e_deviceType == DEVICE_MSI))
			{
				getUserInput(l_ac_buffer, "Enter database path: ");

				l_i_Ret = io_px_data->m_x_database.SetPath((PUC)l_ac_buffer);
				if ((l_i_Ret != MORPHO_OK) && (l_i_Ret != MORPHOERR_UNAVAILABLE))
					fprintf(stdout, "Unable to set database path to %s\n", l_ac_buffer);
			}

			if (l_i_Ret == MORPHO_OK)
			{
				getUserInput(l_ac_buffer, "Select number of fields to create in the database: ");
				sscanf(l_ac_buffer, "%d", &l_i_NbFields);

				for (l_i_fieldIndex = 0; l_i_fieldIndex < l_i_NbFields; l_i_fieldIndex++)
				{
					UL	l_ul_newFieldIndex;
					fprintf(stdout, "\nEnter name for field #%d: ", l_i_fieldIndex);
					memset(l_auc_fieldName, 0, MAX_FIELD_NAME_LEN);
					getUserInput(l_ac_buffer, "");
					while (strlen((const char*)l_ac_buffer) > 6)
					{
						fprintf(stdout, "Field name too long (6 characters max allowed), please enter new name:\n");
						getUserInput(l_ac_buffer, "");
					}
					memcpy(l_auc_fieldName, l_ac_buffer, strlen((const char*)l_ac_buffer));

					// Field type selection for CBI/MSI
					if ((io_px_data->m_e_deviceType == DEVICE_CBI) || (io_px_data->m_e_deviceType == DEVICE_MSI))
					{
						getUserInput(l_ac_buffer, "\nSelect field type:\n"
								"0 - Public\n"
								"1 - Private\n"
								"2 - Statistics\n"
								"3 - Filter\n");
						sscanf(l_ac_buffer, "%d", &l_i_fieldType);

						while((l_i_fieldType < 0)||(l_i_fieldType > 3))
						{
							getUserInput(l_ac_buffer, "\nInvalid field type, please choose one of the following:\n"
								"0 - Public\n"
								"1 - Private\n"
								"2 - Statistics\n"
								"3 - Filter\n");
							sscanf(l_ac_buffer, "%d", &l_i_fieldType);
						}
					}
					// Field type selection for CBM, MSO and Finger VP
					else
					{
						getUserInput(l_ac_buffer, "\nSelect field type:\n"
								"0 - Public\n"
								"1 - Private\n");
						sscanf(l_ac_buffer, "%d", &l_i_fieldType);

						while((l_i_fieldType < 0)||(l_i_fieldType > 1))
						{
							getUserInput(l_ac_buffer, "\nInvalid field type, please choose one of the following:\n"
								"0 - Public\n"
								"1 - Private\n");
							sscanf(l_ac_buffer, "%d", &l_i_fieldType);
						}
					}

					getUserInput(l_ac_buffer, "Enter field maximum size in bytes (max=256, skip to default to 64 bytes):\n");
					if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldSize) != 1)
						l_ul_fieldSize = MAX_FIELD_SIZE;
					if (l_ul_fieldSize > 256)
						l_ul_fieldSize = 256;

					switch(l_i_fieldType)
					{
					case 0:
					default:
						l_i_fieldType = MORPHO_PUBLIC_FIELD;
						break;
					case 1:
						l_i_fieldType = MORPHO_PRIVATE_FIELD;
						break;
					case 2:
						l_i_fieldType = MORPHO_STAT_FIELD;
						break;
					case 3:
						l_i_fieldType = MORPHO_FILTER_FIELD;
						break;
					}

					l_i_Ret = io_px_data->m_x_database.PutField(
							(T_MORPHO_FIELD_ATTRIBUTE)l_i_fieldType,
							l_ul_fieldSize,
							l_auc_fieldName,
							l_ul_newFieldIndex);

					if (l_i_Ret != MORPHO_OK)
					{
						fprintf(stdout, "\nError %d while adding field to database structure, aborting database creation.\n", l_i_Ret);
						break;
					}
				}

				if (l_i_Ret == MORPHO_OK)
				{
					UL	l_ul_NbRecord;
					UL	l_ul_NbFingers;
					T_MORPHO_TYPE_TEMPLATE	l_e_templateType = MORPHO_PK_COMP;	// Create a non-normalized database


					getUserInput(l_ac_buffer, "\nSelect maximum number of database record:\n");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_NbRecord);

					getUserInput(l_ac_buffer, "\nSelect maximum number of fingers per user:\n");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_NbFingers);

					do{
						getUserInput(l_ac_buffer, "\nSelect database encryption status:\n"
								"0 - unecrypted database\n"
								"1 - encrypted database\n");
						sscanf(l_ac_buffer, "%d", (PUC)&l_uc_encrypted);
					}while(l_uc_encrypted != 0 && l_uc_encrypted != 1);

					l_i_Ret = io_px_data->m_x_database.DbCreate(
							l_ul_NbRecord,
							(UC)l_ul_NbFingers,
							l_e_templateType,
							0,
							l_uc_encrypted);

					if (l_i_Ret == MORPHO_OK)
						fprintf(stdout, "\nNone encrypted database creation successful\n");
					else
						fprintf(stdout, "\nError %d while creating database\n", l_i_Ret);
				}
			}
		}
			break;

		/////////////////////////////////////////////
		// Search a user
		/////////////////////////////////////////////
		case 7:
			io_px_data->m_x_database.GetNbField(l_ul_nbFields);
			fprintf(stdout, "\nDatabase fields:\n0 - User ID\n");
			for (l_ul_index = 1; l_ul_index < l_ul_nbFields+1; l_ul_index++){
				memset(l_ac_fieldName, 0, MORPHO_FIELD_NAME_LEN+1);
				l_i_Ret = io_px_data->m_x_database.GetField(l_ul_index,
															l_e_fieldType,
															l_us_fieldMaxSize,
															l_ac_fieldName);
				if (l_i_Ret == MORPHO_OK) {
					fprintf(stdout, "%ld - %s\n", l_ul_index, l_ac_fieldName);
				}
			}

			getUserInput(l_ac_buffer, "Select field index on which to perform the query: ");
			sscanf(l_ac_buffer, "%d", &l_i_searchFieldIndex);

			if ((l_i_searchFieldIndex < 0) || ((UL)l_i_searchFieldIndex > l_ul_nbFields+1)) {
				fprintf(stdout, "Invalid field index\n");
				break;
			}

			getUserInput(l_ac_buffer, "\nEnter data to find:\n");
			memset(l_auc_searchData, 0, MAX_FIELD_SIZE);
			memcpy(l_auc_searchData, l_ac_buffer, strlen(l_ac_buffer));
			fprintf(stdout, "\n");

			l_i_Ret = io_px_data->m_x_database.DbQueryFirst(l_i_searchFieldIndex, strlen((const char*)l_auc_searchData), l_auc_searchData, io_px_data->m_x_user);

			if (l_i_Ret == MORPHO_OK) {
				fprintf(stdout, "User found :\n");
				for (l_ul_index = 0; l_ul_index < l_ul_nbFields+1; l_ul_index++){
					l_i_Ret = io_px_data->m_x_user.GetField(l_ul_index, l_ul_fieldLen, l_puc_fieldData);
					if (l_i_Ret == MORPHO_OK) {
						memcpy(l_ac_buffer, l_puc_fieldData, l_ul_fieldLen);
						l_ac_buffer[l_ul_fieldLen] = 0;
						fprintf(stdout, "%s ", l_ac_buffer);
					}

				}
				fprintf(stdout, "\n");

				l_i_Ret = io_px_data->m_x_database.DbQueryNext(io_px_data->m_x_user);
				while(l_i_Ret == MORPHO_OK)
				{
					for (l_ul_index = 0; l_ul_index < l_ul_nbFields+1; l_ul_index++)
					{
						l_i_Ret = io_px_data->m_x_user.GetField(l_ul_index, l_ul_fieldLen, l_puc_fieldData);
						if (l_i_Ret == MORPHO_OK) {
							memcpy(l_ac_buffer, l_puc_fieldData, l_ul_fieldLen);
							l_ac_buffer[l_ul_fieldLen] = 0;
							fprintf(stdout, "%s ", l_ac_buffer);
						}
					}
					fprintf(stdout, "\n");
					l_i_Ret = io_px_data->m_x_database.DbQueryNext(io_px_data->m_x_user);
				}
			}
			else if (l_i_Ret == MORPHOERR_USER_NOT_FOUND)
				fprintf(stdout, "User not found\n");
			else
				fprintf(stdout, "Error %d occurred while searching user\n", l_i_Ret);

			break;

		/////////////////////////////////////////////
		// Remove a user
		/////////////////////////////////////////////
		case 8:
			// Remove user by index
			if ((io_px_data->m_e_deviceType == DEVICE_CBI) || (io_px_data->m_e_deviceType == DEVICE_MSI))
			{
				fprintf(stdout, "Please select user index to remove:\n");
				//listDatabaseUsers(io_px_data);
				getUserInput(l_ac_buffer, "");

				if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_index) != 1)
				{
					fprintf(stdout, "Invalid index value\n");
					break;
				}

				l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_index, &l_ul_PINLength, &l_puc_PIN);

				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);
					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Error: invalid user ID\n");
					else {
						l_i_Ret = io_px_data->m_x_user.DbDelete();
						if (l_i_Ret != MORPHO_OK)
							fprintf(stdout, "Error %d while deleting user\n", l_i_Ret);
						else
							fprintf(stdout, "User %s deleted successfully\n", l_ac_buffer);

						io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
					}
				}
				else
				{
					fprintf(stdout, "Error %d while trying to translate index to PIN\n", l_i_Ret);
				}
			}
			// Remove user by PIN
			else
			{
				getUserInput(l_ac_buffer, "Please select user PIN to remove:\n");

				l_i_Ret = io_px_data->m_x_database.GetUser((UL)strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);
				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Error: invalid user ID\n");
				else {
					l_i_Ret = io_px_data->m_x_user.DbDelete();
					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Error %d while deleting user\n", l_i_Ret);
					else
						fprintf(stdout, "User %s deleted successfully\n", l_ac_buffer);

					io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
				}
			}
			break;

		/////////////////////////////////////////////
		// Resize the database
		/////////////////////////////////////////////
		case 9:
			getUserInput(l_ac_buffer, "Enter new maximum number of records:\n");
			if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_totalRecords) != 1)
				fprintf(stdout, "Invalid value\n");
			else {
				l_i_Ret = io_px_data->m_x_database.ResizeDb(l_ul_totalRecords);

				switch (l_i_Ret) {
				case MORPHO_OK:
					fprintf(stdout, "Database successfully resized\n");
					break;
				case MORPHOERR_UNAVAILABLE:
					fprintf(stdout, "Operation unavailable on current device\n");
					break;
				case MORPHOERR_BADPARAMETER:
					fprintf(stdout, "New database size invalid (probably less than current capacity)\n");
					break;
				default:
					fprintf(stdout, "Error %d while trying to resize database\n", l_i_Ret);
					break;
				}
			}
			break;

		/////////////////////////////////////////////
		// Get most frequent users list size
		/////////////////////////////////////////////
		case 10:
			l_i_Ret = io_px_data->m_x_database.GetMFUSize(&l_ul_totalRecords);

			if (l_i_Ret == MORPHOERR_UNAVAILABLE)
				fprintf(stdout, "Feature unavailable on current device\n");
			else if (l_i_Ret != MORPHO_OK)
				fprintf(stdout, "Error %d while trying to retrieve current MFU list size\n", l_i_Ret);
			else {
				fprintf(stdout, "Current MFU list size: %ld\n", l_ul_totalRecords);
			}
			break;

		/////////////////////////////////////////////
		// Set most frequent users list size
		/////////////////////////////////////////////
		case 11:
			l_i_Ret = io_px_data->m_x_database.GetMFUSize(&l_ul_totalRecords);

			if (l_i_Ret == MORPHOERR_UNAVAILABLE)
				fprintf(stdout, "Feature unavailable on current device\n");
			else if (l_i_Ret != MORPHO_OK)
				fprintf(stdout, "Error %d while trying to retrieve current MFU list size\n", l_i_Ret);
			else {
				fprintf(stdout, "Current MFU list size: %ld\n", l_ul_totalRecords);
				getUserInput(l_ac_buffer, "Enter new MFU size:");
				if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_totalRecords) != 1)
					fprintf(stdout, "Invalid input\n");
				else {
					l_i_Ret = io_px_data->m_x_database.SetMFUSize(l_ul_totalRecords);

					switch (l_i_Ret) {
					case MORPHO_OK:
						fprintf(stdout, "MFU size successfully set to %ld\n", l_ul_totalRecords);
						break;
					default:
						fprintf(stdout, "Error %d while trying to set new MFU size\n", l_i_Ret);
						break;
					}
				}
			}

			break;

		/////////////////////////////////////////////
		// Autoupdate most frequent users list size
		/////////////////////////////////////////////
		case 12:
			l_i_Ret = io_px_data->m_x_database.AutoupdateMFUSize();
			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, "Successfully recomputed MFU size\n");
				l_i_Ret = io_px_data->m_x_database.GetMFUSize(&l_ul_nbFields);
				if (l_i_Ret == MORPHO_OK)
				{
					fprintf(stdout, "New MFU size: %ld\n", l_ul_nbFields);
				}
				else
				{
					fprintf(stdout, "Error %d while retrieving new MFU size\n", l_i_Ret);
				}
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "MFU functionality unavailable on the current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while trying to recompute MFU list size\n", l_i_Ret);
				break;
			}
			break;

		/////////////////////////////////////////////
		// Deactivate most frequent users list size
		/////////////////////////////////////////////
		case 13:
			l_i_Ret = io_px_data->m_x_database.DeactivateMFUGroup();
			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, "Successfully deactivated MFU functionality\n");
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "MFU functionality unavailable on the current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while trying to deactivate MFU list size\n", l_i_Ret);
				break;
			}
			break;

		/////////////////////////////////////////////
		// Get user PIN
		/////////////////////////////////////////////
		case 14:
		{
			UL	l_ul_userIndex;
			PUC	l_puc_userPIN;
			UL	l_ul_userPINLength;
			UC	l_auc_pinBuffer[INPUT_BUFFER_SIZE];

			getUserInput(l_ac_buffer, "Enter user index:\n");
			if (sscanf(l_ac_buffer, "%lu", &l_ul_userIndex) != 1)
				fprintf(stdout, "Invalid entry\n");
			else
			{
				l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_userIndex, &l_ul_userPINLength, &l_puc_userPIN);

				switch (l_i_Ret)
				{
				case MORPHO_OK:
					memcpy(l_auc_pinBuffer, l_puc_userPIN, l_ul_userPINLength);
					l_auc_pinBuffer[l_ul_userPINLength] = '\0';
					fprintf(stdout, "PIN for user %lu : %s\n", l_ul_userIndex, l_auc_pinBuffer);
					l_i_Ret = io_px_data->m_x_database.ReleaseUserPin(&l_puc_userPIN);
					break;
				case MORPHOERR_UNAVAILABLE:
					fprintf(stdout, "Functionality is unavailable for current device\n");
					break;
				default:
					fprintf(stdout, "Error %d while trying to retrieve PIN for user %lu\n", l_i_Ret, l_ul_userIndex);
					break;
				}
			}
		}
			break;

		/////////////////////////////////////////////
		// Get user PIN list
		/////////////////////////////////////////////
		case 15:
		{
			UL	l_ul_nbUsers;
			PUL	l_pul_userPINLength;
			PUC	*l_puc_userPINList;
			UL	start, end, page;

			T1 = SampleGetTickCount();
			l_i_Ret = io_px_data->m_x_database.GetUserPinList(&l_ul_nbUsers, l_pul_userPINLength, l_puc_userPINList);
			T2 = SampleGetTickCount();

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				start = 0;
				end = l_ul_nbUsers;
				page = end;
				fprintf(stdout, "Found %lu users in database in %dms.\n", l_ul_nbUsers, T2 - T1);

				getUserInput(l_ac_buffer, "Display (A)ll users, or index (R)ange?\n");
				if ((l_ac_buffer[0] == 'a') || (l_ac_buffer[0] == 'A'))
				{
					getUserInput(l_ac_buffer, "Select users number to display per page (skip to display all):\n");
					if (sscanf(l_ac_buffer, "%d", (int*)&page) != 1)
						page = l_ul_nbUsers;
				}
				else if ((l_ac_buffer[0] == 'r') || (l_ac_buffer[0] == 'R'))
				{
					getUserInput(l_ac_buffer, "Select user indexes to display (start,end):\n");
					if (sscanf(l_ac_buffer, "%d,%d", (int*)&start, (int*)&end) != 2)
					{
						fprintf(stdout, "Invalid values\n");
						break;
					}
					getUserInput(l_ac_buffer, "Select users number to display per page (skip to display all):\n");
					if (sscanf(l_ac_buffer, "%d", (int*)&page) != 1)
						page = l_ul_nbUsers;
				}
				else
					break;

				fprintf(stdout, "Index\tPIN\n");
				for (l_ul_index = start; l_ul_index < end; l_ul_index++)
				{
					memset(l_ac_buffer, 0, INPUT_BUFFER_SIZE);
					memcpy(l_ac_buffer, l_puc_userPINList[l_ul_index], l_pul_userPINLength[l_ul_index]);
					fprintf(stdout, "%lu\t%s\n", l_ul_index, l_ac_buffer);

					if (!(l_ul_index%page) && l_ul_index)
					{
						fprintf(stdout, "Press enter to display next %ld users (q to skip)\n", page);
						getUserInput(l_ac_buffer, "");
						if ((l_ac_buffer[0] == 'q') || (l_ac_buffer[0] == 'Q'))
							break;
					}
				}
				l_i_Ret = io_px_data->m_x_database.ReleaseUserPinList(l_ul_nbUsers, l_pul_userPINLength, l_puc_userPINList);
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Functionality is unavailable for current device\n");break;
			default:
				fprintf(stdout, "Error %d while trying to retrieve user PIN list\n", l_i_Ret);
				break;
			}
			break;
		}

		/////////////////////////////////////////////
		// Change database path
		/////////////////////////////////////////////
		case 16:
			getUserInput(l_ac_buffer, "Enter new database path:\n");

			T1 = SampleGetTickCount();
			l_i_Ret = io_px_data->m_x_database.SetPath((PUC)l_ac_buffer);
			T2 = SampleGetTickCount();

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, "Path changed successfully (database opened in %dms)\n", T2-T1);
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Unable to change database path on the current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while changing database path\n", l_i_Ret);
				break;
			}
			break;

		/////////////////////////////////////////////
		// save matching statistics
		/////////////////////////////////////////////
		case 17:
			T1 = SampleGetTickCount();
			l_i_Ret = io_px_data->m_x_database.SynchronizeStats();
			T2 = SampleGetTickCount();

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				fprintf(stdout, "Statistics successfully synchronized on storage device in %dms\n", T2 - T1);
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Unable to synchronize matching statistics on the current device\n");
				break;
			default:
				fprintf(stdout, "Error %d while synchronizing matching statistics\n", l_i_Ret);
				break;
			}
			break;

		/////////////////////////////////////////////
		// get user matching statistics
		/////////////////////////////////////////////
		case 18:
			fprintf(stdout, "Please select user index to check:\n");
			getUserInput(l_ac_buffer, "");

			if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_index) != 1)
			{
				fprintf(stdout, "Invalid user index\n");
				break;
			}

			l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_index, &l_ul_PINLength, &l_puc_PIN);

			if (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);
				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Error: invalid user ID\n");
				else {
					l_i_Ret = io_px_data->m_x_user.GetStats(0,
							l_puc_PIN,
							l_ul_PINLength,
							0,
							&l_ul_index);
					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Error %d while retrieving user matching statistics\n", l_i_Ret);
					else
						fprintf(stdout, "%ld matches for user %s\n", l_ul_index, l_ac_buffer);
				}

				io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
			}
			else
			{
				fprintf(stdout, "Error %d while trying to retrieve PIN for user index %ld\n", l_i_Ret, l_ul_index);
			}
			break;

		/////////////////////////////////////////////
		// set user matching statistics
		/////////////////////////////////////////////
		case 19:
		{
			fprintf(stdout, "Please select user index to update:\n");
			getUserInput(l_ac_buffer, "");

			if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_index) != 1)
			{
				fprintf(stdout, "Invalid user index\n");
				break;
			}

			l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_index, &l_ul_PINLength, &l_puc_PIN);

			if (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);
				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Error: invalid user ID\n");
				else {
					getUserInput(l_ac_buffer, "Enter new matching statistics:\n");
					if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_index) != 1)
					{
						fprintf(stdout, "Invalid input");
					}
					else
					{
						l_i_Ret = io_px_data->m_x_user.SetStats(0,
								l_puc_PIN,
								l_ul_PINLength,
								0,
								l_ul_index);

						if (l_i_Ret != MORPHO_OK)
							fprintf(stdout, "Error %d while setting user matching statistics\n", l_i_Ret);
						else
							fprintf(stdout, "%ld matches for user %.*s\n", l_ul_index, (int)l_ul_PINLength, l_puc_PIN);
					}

					io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
				}
			}
			else
			{
				fprintf(stdout, "Error %d while trying to retrieve PIN for user index %ld\n", l_i_Ret, l_ul_index);
			}
		}
			break;


		/////////////////////////////////////////////
		// filter database
		/////////////////////////////////////////////
		case 20:
			l_i_Ret = io_px_data->m_x_database.GetNbField(l_ul_nbFields);

			for (l_ul_index = 1; l_ul_index < l_ul_nbFields+1; l_ul_index++)
			{
				memset(l_auc_searchData, 0, MAX_FIELD_SIZE);
				l_i_Ret = io_px_data->m_x_database.GetField(l_ul_index, l_e_fieldType, l_us_fieldMaxSize, l_auc_searchData);
				if ((l_i_Ret == MORPHO_OK) && (l_e_fieldType == MORPHO_FILTER_FIELD))
				{
					fprintf(stdout, "%s (%ld)\t| ", l_auc_searchData, l_ul_index);
				}
				fprintf(stdout, "\n");
			}

			if (sscanf(l_ac_buffer, "%d", &l_i_searchFieldIndex) == 1)
			{
				getUserInput(l_ac_buffer, "Enter filter value (leave blank to reset filter):\n");

				// Apply filter
				T1 = SampleGetTickCount();
				if (l_ac_buffer[0] != '\0')
				{
					unsigned int charIndex;
					UC	l_auc_binFilter[1024];
					for (charIndex = 0; charIndex < strlen(l_ac_buffer); charIndex++)
					{
						l_auc_binFilter[charIndex] = htoi(l_ac_buffer[charIndex*2]) << 4;
						l_auc_binFilter[charIndex] += htoi(l_ac_buffer[charIndex*2+1]);
					}
					l_i_Ret = io_px_data->m_x_database.SetFilter(l_i_searchFieldIndex, MORPHO_FILTER_TYPE_BINARY, (strlen(l_ac_buffer))/2, (void*)l_auc_binFilter);
				}
				// Reset filter
				else
					l_i_Ret = io_px_data->m_x_database.SetFilter(l_i_searchFieldIndex, MORPHO_FILTER_TYPE_BINARY, 0, NULL);
				T2 = SampleGetTickCount();

				switch (l_i_Ret)
				{
				case MORPHO_OK:
					fprintf(stdout, "Filter successfully applied in %dms\n", T2 - T1);
					break;
				case MORPHOERR_UNAVAILABLE:
					fprintf(stdout, "Filter feature unavailable on current device\n");
					break;
				default:
					fprintf(stdout, "Error %d while applying new filter\n", l_i_Ret);
					break;
				}
			}
			else
				fprintf(stdout, "Invalid input.");
			break;

		/////////////////////////////////////////////
		// Get active filter
		/////////////////////////////////////////////
		case 21:
		{
			UC						l_uc_filterID;
			T_MORPHO_FILTER_TYPE	l_x_filterType;
			UL						l_ul_filterLength;
			PVOID					l_pv_filterData;

			l_i_Ret = io_px_data->m_x_database.GetFilter(l_uc_filterID, l_x_filterType, l_ul_filterLength, l_pv_filterData);

			switch (l_i_Ret)
			{
			case MORPHO_OK:
				if (l_pv_filterData == NULL)
				{
					fprintf(stdout, "Database is not currently filtered\n");
				}
				else
				{
					if (l_x_filterType == MORPHO_FILTER_TYPE_INTEGER)
					{
						fprintf(stdout, "Integer filter: %d\n", *((I*)l_pv_filterData));
					}
					else
					{
						fprintf(stdout, "Binary filter (%ld bytes): ", l_ul_filterLength);
						for (l_ul_index = 0; l_ul_index < l_ul_filterLength; l_ul_index++)
						{
							fprintf(stdout, "%02X ", ((PUC)l_pv_filterData)[l_ul_index]);
						}
						fprintf(stdout, "\n");
					}
				}
				break;
			case MORPHOERR_UNAVAILABLE:
				fprintf(stdout, "Filters unavailable for current device.\n");
				break;
			default:
				fprintf(stdout, "Error %d while trying to retrieve active filter\n", l_i_Ret);
				break;
			}
		}
			break;


		/////////////////////////////////////////////
		// Retrieve all users filter value
		/////////////////////////////////////////////
		case 22:
			printDatabaseFields(io_px_data->m_x_database, MORPHO_FILTER_FIELD, &l_ul_nbFields, NULL);
			getUserInput(l_ac_buffer, "Select filter field index to retrieve:\n");
			if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_index) != 1)
			{
				fprintf(stdout, "Invalid field selection\n");
			}
			else
			{
				UL	l_ul_nbUsers;
				UL	l_ul_byteIndex;
				PUL	l_pul_filterSizes;
				PUC	*l_ppuc_filterData;
				l_i_Ret = io_px_data->m_x_database.GetFilterDataList(l_ul_index, &l_ul_nbUsers, &l_pul_filterSizes, &l_ppuc_filterData);

				switch (l_i_Ret)
				{
				case MORPHO_OK:
					fprintf(stdout, "Filter data:\nIndex\tFilter\n");
					for (l_ul_index = 0; l_ul_index < l_ul_nbUsers; l_ul_index++)
					{
						fprintf(stdout, "%ld\t0x", l_ul_index);
						for(l_ul_byteIndex = 0; l_ul_byteIndex < l_pul_filterSizes[l_ul_index]; l_ul_byteIndex++)
						{
							fprintf(stdout, "%02X", l_ppuc_filterData[l_ul_index][l_ul_byteIndex]);
						}
						fprintf(stdout, "\n");
					}
					l_i_Ret = io_px_data->m_x_database.ReleaseFilterDataList(l_ul_nbUsers, &l_pul_filterSizes, &l_ppuc_filterData);
					if (l_i_Ret != MORPHO_OK)
					{
						fprintf(stdout, "Error %d while releasing memory for filter data\n", l_i_Ret);
					}
					break;
				case MORPHOERR_UNAVAILABLE:
					fprintf(stdout, "Filter data unavailable for currently connected device.\n");
					break;
				default:
					fprintf(stdout, "Error %d while retrieving filter data from the database\n", l_i_Ret);
					break;
				}
			}
			break;
		/////////////////////////////////////////////
		// Get DataBase Config
		/////////////////////////////////////////////
		case 23:
		{
			UC	i_number_finger;
			UL	l_max_record_number;
			UL	l_current_record_number;
			UC	i_encrypted;
			UL	i_ul_fieldIndex;
			::T_MORPHO_FIELD_ATTRIBUTE o_uc_fieldAttribute = ::MORPHO_PUBLIC_FIELD;
			US	o_us_fieldMaxSize;
			UC	o_puc_fieldName[MORPHO_FIELD_NAME_LEN];
			UL	i_ul_numberfield;

			io_px_data->m_x_database.GetNbFinger(i_number_finger);
			fprintf(stdout, "Finger Number : %d \n",i_number_finger);
			io_px_data->m_x_database.GetMaxUser(&l_max_record_number);
			fprintf(stdout, "Max Record Number : %d \n",l_max_record_number);
			io_px_data->m_x_database.GetNbUsedRecord(l_current_record_number);
			fprintf(stdout, "Current Record Number : %d \n",l_current_record_number);

			io_px_data->m_x_database.GetDbEncryptionStatus(i_encrypted);
			fprintf(stdout, "Encrypted database : %s \n",((i_encrypted == 0) ? "No" : "Yes"));

			io_px_data->m_x_database.GetNbField(i_ul_numberfield);
			for(int i = 1; i<= i_ul_numberfield ;i++)
			{
				io_px_data->m_x_database.GetField(i,o_uc_fieldAttribute,o_us_fieldMaxSize,o_puc_fieldName);
				fprintf(stdout, "field number %d : %s \n",i,o_puc_fieldName);
			}
		}
			break;
		/////////////////////////////////////////////
		// Get List User in Database
		/////////////////////////////////////////////
		case 24:
		{
			UC l_auc_FieldIndexDesc[MORPHO_NB_FIELD_MAX_BIT];
			C_MORPHO_UserList l_x_UserList;
   			C_MORPHO_User     *l_px_User;
		    UL l_ul_lenfield;
			UL l_ul_NbField;
		    PUC l_puc_DataField;
		    UC l_auc_buf[3][25];
		    PUC l_ppuc_FieldName[15];
			UL l_ul_NbUsedUser;


			io_px_data->m_x_database.GetNbField(l_ul_NbField);

			l_ppuc_FieldName[0] = (PUC) malloc(25);
			strcpy((PC) (l_ppuc_FieldName[0]), "UserID");
			for(int i=1; i<(I)l_ul_NbField+1; i++)
			{
			    T_MORPHO_FIELD_ATTRIBUTE l_x_FieldAttribute;
			    US l_us_FieldMaxSize;
			    l_ppuc_FieldName[i] = (PUC) malloc(25);
			    memset(l_ppuc_FieldName[i], 0, 25);
			    io_px_data->m_x_database.GetField(i, l_x_FieldAttribute, l_us_FieldMaxSize, l_ppuc_FieldName[i]);
			}

			io_px_data->m_x_database.FillIndexDescriptor(TRUE, 0, l_auc_FieldIndexDesc);
        	io_px_data->m_x_database.FillIndexDescriptor(FALSE, 1, l_auc_FieldIndexDesc);
        	io_px_data->m_x_database.FillIndexDescriptor(FALSE, 2, l_auc_FieldIndexDesc);
        	io_px_data->m_x_database.ReadPublicFields(l_auc_FieldIndexDesc, l_x_UserList);

			l_x_UserList.GetNbUser(l_ul_NbUsedUser);

			if(l_ul_NbUsedUser > 0)
			{
			    for(int i=0; i<(I)l_ul_NbUsedUser; i++)
			    {
				l_x_UserList.GetUser(i, l_px_User);
				for(int j=0; j<3; j++)
				{
				    l_px_User->GetField(j, l_ul_lenfield, l_puc_DataField);
				    // fprintf(stdout, "field : %s \n",l_puc_DataField);
				    if(l_ul_lenfield>0)
				    {
						memcpy(l_auc_buf[j], l_puc_DataField, l_ul_lenfield);
						l_auc_buf[j][l_ul_lenfield] = 0;
				    }
				    else
				    {
						strcpy((PC)l_auc_buf[j], "<none>");
				    }

				    if( (strcmp((PC)l_auc_buf[j], "")==0) || (strcmp((PC)l_auc_buf[j], " ")==0))
					strcpy((PC)l_auc_buf[j], "<none>");
				}

				fprintf(stdout,"%d - %s=%-8s %s=%-8s %s=%-8s \n", i, (PC)l_ppuc_FieldName[0], l_auc_buf[0],(PC)l_ppuc_FieldName[1], l_auc_buf[1],
									(PC)l_ppuc_FieldName[2], l_auc_buf[2]);
			    }
			}

			
			
		}
			break;
		
		/////////////////////////////////////////////
		// Back to main menu
		/////////////////////////////////////////////
		case 0:
			l_uc_loop = 0;
			break;
		default:
			fprintf(stdout, "Invalid command\n");
			break;
		}
	}

	return 0;
}

I userOperation(PT_DATA io_px_data)
{
	I							l_i_Ret;
	UC							l_uc_loop = 1;
	I							l_i_command = 0;
	C_MORPHO_TemplateList		l_x_templateList;
	C							l_ac_buffer[INPUT_BUFFER_SIZE];
	UC							l_auc_buffer[256];
	UL							l_ul_nbField;
	UL							l_ul_fieldIndex;
	UL							l_ul_fieldLen;
	US							l_us_maxSize;
	I							l_i_AcquisitionThreshold;
	UC							l_uc_nbFinger;
	T_MORPHO_IMAGE *			l_px_image = NULL;
	UL							l_ul_asyncEvents =	MORPHO_CALLBACK_DETECTQUALITY |
													MORPHO_CALLBACK_CODEQUALITY |
													MORPHO_CALLBACK_IMAGE_CMD |
													MORPHO_CALLBACK_COMMAND_CMD |
													MORPHO_CALLBACK_ENROLLMENT_CMD;
	UL							l_ul_userIndex;
	UL							l_ul_PINLength;
	PUC							l_puc_PIN = NULL;
	UL							l_ul_NbInsert;
	PUC							l_puc_aud = NULL;
	I							l_i_CompressRate = 2;
	T_MORPHO_COMPRESS_ALGO		l_x_compressAlgo = MORPHO_NO_COMPRESS;
	T_MORPHO_TYPE_TEMPLATE		l_x_TypeTemplate = MORPHO_PK_CFV;

	int T1, T2;

	while (l_uc_loop)
	{
		getUserInput(l_ac_buffer, "\n\nUser operation menu:\n"
				"1 - enroll user\n"
				"2 - add user from template file\n"
				"3 - batch add user from template file\n"
				"4 - display public user information\n"
				"5 - modify public user information\n"
				"6 - modify private user information\n"
				"7 - get user filter data\n"
				//"8 - verify match in database\n"
				"0 - back to main menu\n");
		sscanf(l_ac_buffer, "%d", &l_i_command);

		switch (l_i_command)
		{
		/////////////////////////////////////////////
		// Enroll user
		/////////////////////////////////////////////
		case 1:

			if (fillNewUserFields(io_px_data) != MORPHO_OK)
				break;

			getUserInput(l_ac_buffer, "\nEnter number of fingers:");
			sscanf(l_ac_buffer, "%d", (int*)&l_uc_nbFinger);

			getUserInput(l_ac_buffer, "Enter image quality threshold (20 - 100):");
			sscanf(l_ac_buffer, "%d", &l_i_AcquisitionThreshold);

			while((l_i_AcquisitionThreshold<20) || (l_i_AcquisitionThreshold>100))
			{
				getUserInput(l_ac_buffer, "\nInvalid threshold value, please enter a value between 20 and 100:");
				sscanf(l_ac_buffer, "%d", &l_i_AcquisitionThreshold);
			}

#ifdef ENABLE_DISPLAY
		InitScreen();
#endif
		getUserInput(l_ac_buffer, "Choose exported image compression:\n1 - no compression\n2 - WSQ compression\n");
		if (sscanf(l_ac_buffer, "%d", &l_i_Ret) != 1)
		{
			fprintf(stdout, "Invalid input, default to RAW image\n");
		}
		else
		{
			if (l_i_Ret == 2)
			{
				l_x_compressAlgo = MORPHO_COMPRESS_WSQ;

				getUserInput(l_ac_buffer, "Enter exported image compression rate (2-255), leave blank to default to 15:\n");
				if (l_ac_buffer[0] == '\0')
				{
					l_i_CompressRate = 15;
				}
				else if (sscanf(l_ac_buffer, "%d", &l_i_CompressRate) != 1)
				{
					fprintf(stdout, "Invalid input, using default value 15\n");
					l_i_CompressRate = 15;
				}
			}
			else
			{
				if (l_i_Ret != 1)
					fprintf(stdout, "Invalid value, default to no compression\n");
				l_x_compressAlgo = MORPHO_NO_COMPRESS;
				l_i_CompressRate = 0;
			}
		}

		io_px_data->m_x_device.Malloc((void**)&l_px_image, l_uc_nbFinger * sizeof(T_MORPHO_IMAGE));
		if (l_px_image == NULL)
		{
			fprintf(stdout, "Error: unable to allocate memory for export image container\n");
			break;
		}

		l_x_templateList.SetActiveFullImageRetrieving(TRUE);

		l_i_Ret = io_px_data->m_x_user.Enroll(
								15,//US  i_us_Timeout,
								0,//(UC)  l_i_AcquisitionThreshold,
								0,//UC  i_uc_AdvancedSecurityLevelsRequired,
								l_x_compressAlgo,
								(UC)l_i_CompressRate,//UC  i_uc_CompressRate,
								1,//UC  i_uc_ExportImage,
								1,//UC  i_uc_ExportMinutiae,
								l_uc_nbFinger,//UC  i_uc_FingerNumber,
								MORPHO_PK_COMP,//T_MORPHO_TYPE_TEMPLATE  i_x_TemplateType,
								MORPHO_NO_PK_FVP,//T_MORPHO_FVP_TYPE_TEMPLATE  i_x_FVPTemplateType,
								1,//UC  i_uc_SaveRecord,
								l_ul_asyncEvents,//UL  i_ul_CallbackCmd,
								eventCallback,//T_MORPHO_CALLBACK_FUNCTION  i_pf_Callback,
								io_px_data,//PVOID  i_pv_CallbackArgument,
								MORPHO_DEFAULT_CODER,//I  i_i_CoderChoice,
								MORPHO_ENROLL_DETECT_MODE|MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE,//UL  i_ul_DetectModeChoice,
								l_x_templateList,
								l_px_image,//T_MORPHO_IMAGE *  o_px_Image,
								NULL//PT_MORPHO_MOC_PARAMETERS  i_px_MocParameters
								);

		if (l_i_Ret == MORPHO_OK)
		{
			fprintf(stdout, "Enrollment successful\nEnter path to export image (extension will be automatically appended), leave blank to skip:\n");
			getUserInput(l_ac_buffer, "");
			if (l_ac_buffer[0] != '\0')
			{
				FILE	*exportImage = NULL;
				UC		l_uc_index;
				char	l_ac_exportImageFileName[2048];

				for(l_uc_index = 0; l_uc_index < l_uc_nbFinger; l_uc_index++)
				{
					if (l_x_compressAlgo == MORPHO_COMPRESS_WSQ)
					{
						sprintf(l_ac_exportImageFileName, "%s_%d.wsq", l_ac_buffer, l_uc_index);
						exportImage = fopen(l_ac_exportImageFileName, "wb");
						if (exportImage == NULL)
						{
							fprintf(stdout, "Unable to create file %s\n", l_ac_exportImageFileName);
						}
						else
						{
							fwrite(l_px_image[l_uc_index].m_puc_CompressedImage, 1, l_px_image[l_uc_index].m_x_ImageWSQHeader.m_ul_WSQImageSize, exportImage);
							//io_px_data->m_x_device.Free((void**)&l_px_image [l_uc_index].m_puc_CompressedImage);
						}
						fclose(exportImage);
					}
					else if (l_x_compressAlgo == MORPHO_NO_COMPRESS)
					{
						sprintf(l_ac_exportImageFileName, "%s_%d_[%dx%d].raw",
								l_ac_buffer,
								l_uc_index,
								l_px_image[l_uc_index].m_x_ImageHeader.m_us_NbCol, l_px_image[l_uc_index].m_x_ImageHeader.m_us_NbRow);
						exportImage = fopen(l_ac_exportImageFileName, "wb");
						if (exportImage == NULL)
						{
							fprintf(stdout, "Unable to create file %s\n", l_ac_exportImageFileName);
						}
						else
						{
							fwrite(l_px_image[l_uc_index].m_puc_Image, 1, (l_px_image[l_uc_index].m_x_ImageHeader.m_us_NbCol * l_px_image[l_uc_index].m_x_ImageHeader.m_us_NbRow * l_px_image[l_uc_index].m_x_ImageHeader.m_uc_NbBitsPerPixel) / 8, exportImage);
							//io_px_data->m_x_device.Free((void**)&l_px_image [l_uc_index].m_puc_Image);
						}
						fclose(exportImage);
					}
					else
					{
						fprintf(stdout, "Unsupported compression format %d\n", l_px_image[l_uc_index].m_x_ImageHeader.m_uc_CompressionType);
					}
				}
			}
		}
		else
			fprintf(stdout, "Error %d while enrolling user\n", l_i_Ret);

		if (l_px_image != NULL)
		{
			io_px_data->m_x_device.Free((void**)&l_px_image);
		}

#ifdef ENABLE_DISPLAY
			CloseScreen();
#endif

			break;

		/////////////////////////////////////////////
		// Add user from template file
		/////////////////////////////////////////////
		case 2:
			getUserInput(l_ac_buffer, "Enter input template file name:\n");

			if (l_ac_buffer[0] != '\0') {
				FILE						*l_px_templateFile;
				PUC							l_puc_Pk;
				UL							l_ul_PkSize;
				UC							l_uc_PkIndex;

				l_px_templateFile = fopen(l_ac_buffer, "rb");

				if (l_px_templateFile == NULL)
					fprintf(stdout, "Unable to open file %s\n", l_ac_buffer);
				else {
					fseek(l_px_templateFile, 0, SEEK_END);
					l_ul_PkSize = ftell(l_px_templateFile);
					fseek(l_px_templateFile, 0, SEEK_SET);

					l_puc_Pk = (PUC)malloc(l_ul_PkSize);
					fread(l_puc_Pk, 1, l_ul_PkSize, l_px_templateFile);

					l_x_TypeTemplate = getTemplateType();

					l_i_Ret = fillNewUserFields(io_px_data);

					if (l_i_Ret == MORPHO_OK)
					{
						l_i_Ret = io_px_data->m_x_user.PutTemplate(l_x_TypeTemplate,
														l_ul_PkSize,
														l_puc_Pk,
														l_uc_PkIndex);
					}
					else
						break;

					io_px_data->m_x_user.SetNoCheckOnTemplateForDBStore(TRUE);
					getUserInput(l_ac_buffer, "Enter 'Yes' or 'Y' to prevent double template insertion in the database\n");
					if ( (!strcasecmp(l_ac_buffer, "yes")) || (!strcasecmp(l_ac_buffer, "y")) )
					{
						io_px_data->m_x_user.SetNoCheckOnTemplateForDBStore(FALSE);
					}

					T1 = SampleGetTickCount();
					l_i_Ret = io_px_data->m_x_user.DbStore();
					T2 = SampleGetTickCount();

					fprintf(stdout, "DbStore finished in %dms\n", T2 - T1);

					switch(l_i_Ret) {
					case MORPHO_OK:
						fprintf(stdout, "User successfully added to the database\n");
						break;
					case MORPHOERR_INVALID_USER_ID:
						fprintf(stdout, "User ID already present in database\n");
						break;
					case MORPHOERR_ALREADY_ENROLLED:
						fprintf(stdout, "User already enrolled\n");
						break;
					case MORPHOERR_SAME_FINGER:
						fprintf(stdout, "At least one input template matched against other input templates\n");
						break;
					default:
						fprintf(stdout, "Error %d while trying to add record to database\n", l_i_Ret);
						break;
					}

					free(l_puc_Pk);
					fclose(l_px_templateFile);
				}
			}
			break;

		/////////////////////////////////////////////
		// Batch add user from MORPHO_PK_CFV template file
		/////////////////////////////////////////////
		case 3:
		{
			int timerStore1, timerStore2, timerStoreTotal;

			getUserInput(l_ac_buffer, "Enter input template file name:\n");

			if (l_ac_buffer[0] != '\0') {
				FILE						*l_px_templateFile;
				PUC							l_puc_Pk;
				UL							l_ul_PkSize;
				UC							l_uc_PkIndex;
				T_MORPHO_FIELD_ATTRIBUTE	l_uc_fieldAttribute;
				UC							l_auc_fieldName[MORPHO_FIELD_NAME_LEN+1];

				l_auc_fieldName[MORPHO_FIELD_NAME_LEN] = '\0';

				l_px_templateFile = fopen(l_ac_buffer, "rb");

				if (l_px_templateFile == NULL)
					fprintf(stdout, "Unable to open file %s\n", l_ac_buffer);
				else {
					fseek(l_px_templateFile, 0, SEEK_END);
					l_ul_PkSize = ftell(l_px_templateFile);
					fseek(l_px_templateFile, 0, SEEK_SET);

					l_puc_Pk = (PUC)malloc(l_ul_PkSize);
					fread(l_puc_Pk, 1, l_ul_PkSize, l_px_templateFile);

					l_i_Ret = io_px_data->m_x_database.GetNbField(l_ul_nbField);

					getUserInput(l_ac_buffer, "Enter first new user ID:\n");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_userIndex);

					getUserInput(l_ac_buffer, "Enter number of insertions:\n");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_NbInsert);

					l_puc_aud = (PUC)malloc(l_ul_nbField * INPUT_BUFFER_SIZE);
					memset(l_puc_aud, 0, l_ul_nbField * INPUT_BUFFER_SIZE);

					for (l_ul_fieldIndex = 1; l_ul_fieldIndex < l_ul_nbField+1; l_ul_fieldIndex++) {
						l_i_Ret = io_px_data->m_x_database.GetField(l_ul_fieldIndex, l_uc_fieldAttribute, l_us_maxSize, l_auc_fieldName);
						if (l_uc_fieldAttribute == MORPHO_FILTER_FIELD)
							fprintf(stdout, "%s (hexadecimal value, %d bytes):\n", l_auc_fieldName, l_us_maxSize);
						else
							fprintf(stdout, "%s:\n", l_auc_fieldName);
						getUserInput((char*)&l_puc_aud[(l_ul_fieldIndex-1)*INPUT_BUFFER_SIZE], "");
					}

					T1 = SampleGetTickCount();
					timerStoreTotal = 0;
					while(l_ul_NbInsert--)
					{
						sprintf(l_ac_buffer, "%ld", l_ul_userIndex);

						l_i_Ret = io_px_data->m_x_database.GetUser(strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);

						l_i_Ret = io_px_data->m_x_user.PutTemplate(MORPHO_PK_CFV,
													l_ul_PkSize,
													l_puc_Pk,
													l_uc_PkIndex);

						for (l_ul_fieldIndex = 1; l_ul_fieldIndex < l_ul_nbField+1; l_ul_fieldIndex++) {
							l_i_Ret = io_px_data->m_x_database.GetField(l_ul_fieldIndex, l_uc_fieldAttribute, l_us_maxSize, l_auc_fieldName);
							l_auc_fieldName[MORPHO_FIELD_NAME_LEN] = 0;
							if (l_uc_fieldAttribute == MORPHO_FILTER_FIELD)
							{
								UL	charIndex;

								for (charIndex = 0; charIndex < l_us_maxSize; charIndex++)
								{
									l_auc_buffer[charIndex] = htoi(l_puc_aud[(l_ul_fieldIndex-1)*INPUT_BUFFER_SIZE + charIndex*2]) * 16;
									l_auc_buffer[charIndex] += htoi(l_puc_aud[(l_ul_fieldIndex-1)*INPUT_BUFFER_SIZE + charIndex*2+1]);
								}
								l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_us_maxSize, l_auc_buffer);
							}
							else if (l_uc_fieldAttribute == MORPHO_STAT_FIELD)
							{
								UL	stats;

								sscanf((const char*)&l_puc_aud[(l_ul_fieldIndex-1)*INPUT_BUFFER_SIZE], "%d", (int*)&stats);

								l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, sizeof(UL), (PUC)&stats);
							}
							else
							{
								l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, strlen((const char*)&l_puc_aud[(l_ul_fieldIndex-1)*INPUT_BUFFER_SIZE]), &l_puc_aud[(l_ul_fieldIndex-1)*INPUT_BUFFER_SIZE]);
							}
						}

						timerStore1 = SampleGetTickCount();
						l_i_Ret = io_px_data->m_x_user.DbStore();
						timerStore2 = SampleGetTickCount();
						timerStoreTotal += timerStore2 - timerStore1;

						if (l_i_Ret != MORPHO_OK)
							break;

						l_ul_userIndex++;
					}
					T2 = SampleGetTickCount();

					switch(l_i_Ret) {
					case MORPHO_OK:
						fprintf(stdout, "Users successfully added to the database in %dms\n(DbStore cumulative time: %dms)\n", T2 - T1, timerStoreTotal);
						break;
					case MORPHOERR_INVALID_USER_ID:
						fprintf(stdout, "User ID already present in database\n");
						break;
					case MORPHOERR_ALREADY_ENROLLED:
						fprintf(stdout, "User already enrolled\n");
						break;
					case MORPHOERR_SAME_FINGER:
						fprintf(stdout, "Matching found between input templates\n");
						break;
					default:
						fprintf(stdout, "Error %d while trying to add records to database\n", l_i_Ret);
						break;
					}

					free(l_puc_Pk);
					free(l_puc_aud);
					fclose(l_px_templateFile);
				}
			}
		}
			break;

		/////////////////////////////////////////////
		// Display public user information
		/////////////////////////////////////////////
		case 4:
			// Select user by index for CBI/MSI
			if ((io_px_data->m_e_deviceType == DEVICE_CBI) || (io_px_data->m_e_deviceType == DEVICE_MSI))
			{
				fprintf(stdout, "Select user index to check:\n");
				//listDatabaseUsers(io_px_data);
				getUserInput(l_ac_buffer, "");

				if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_userIndex) != 1)
				{
					fprintf(stdout, "Invalid user index\n");
					break;
				}

				l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_userIndex, &l_ul_PINLength, &l_puc_PIN);

				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);
					//l_i_Ret = io_px_data->m_x_database.GetUser((UC)strlen(l_ac_buffer), (PUC)l_ac_buffer, l_x_user);
					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Invalid user selection\n");
					else {
						printDatabaseFields(io_px_data->m_x_database, MORPHO_PUBLIC_FIELD, &l_ul_nbField, NULL);

						getUserInput(l_ac_buffer, "Select field index to display:");
						sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldIndex);

						l_i_Ret = io_px_data->m_x_user.GetField(l_ul_fieldIndex, l_ul_fieldLen, l_puc_aud);
						if (l_i_Ret == MORPHO_OK)
						{
							fprintf(stdout, "%.*s\n", (int)l_ul_fieldLen, l_puc_aud);
						}
						else
						{
							fprintf(stdout, "Error %d while retrieving content for field %ld\n", l_i_Ret, l_ul_fieldIndex);
						}
					}

					io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
				}
			}
			// Select user by PIN for other devices
			else
			{
				getUserInput(l_ac_buffer, "Select user PIN to check:\n");

				l_i_Ret = io_px_data->m_x_database.GetUser((UL)strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);
				//l_i_Ret = io_px_data->m_x_database.GetUser((UC)strlen(l_ac_buffer), (PUC)l_ac_buffer, l_x_user);
				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Invalid user selection\n");
				else {
					printDatabaseFields(io_px_data->m_x_database, MORPHO_PUBLIC_FIELD, &l_ul_nbField, NULL);

					getUserInput(l_ac_buffer, "Select field index to display:");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldIndex);

					l_i_Ret = io_px_data->m_x_user.GetField(l_ul_fieldIndex, l_ul_fieldLen, l_puc_aud);
					if (l_i_Ret == MORPHO_OK)
					{
						fprintf(stdout, "%.*s\n", (int)l_ul_fieldLen, l_puc_aud);
					}
					else
					{
						fprintf(stdout, "Error %d while retrieving content for field %ld\n", l_i_Ret, l_ul_fieldIndex);
					}
				}
			}
			break;

		/////////////////////////////////////////////
		// Modify public user information
		/////////////////////////////////////////////
		case 5:
			// Select user by index for CBI/MSI devices
			if ((io_px_data->m_e_deviceType == DEVICE_CBI) || (io_px_data->m_e_deviceType == DEVICE_MSI))
			{
				fprintf(stdout, "Select user index to update:\n");
				//listDatabaseUsers(io_px_data);
				getUserInput(l_ac_buffer, "");

				if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_userIndex) != 1)
				{
					fprintf(stdout, "Invalid user index\n");
					break;
				}

				l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_userIndex, &l_ul_PINLength, &l_puc_PIN);

				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);
					//l_i_Ret = io_px_data->m_x_database.GetUser((UC)strlen(l_ac_buffer), (PUC)l_ac_buffer, l_x_user);
					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Invalid user selection\n");
					else {
						printDatabaseFields(io_px_data->m_x_database, MORPHO_PUBLIC_FIELD, &l_ul_nbField, NULL);

						getUserInput(l_ac_buffer, "Select field index to modify:");
						sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldIndex);

						getUserInput(l_ac_buffer, "Enter new field value:\n");
						l_ul_fieldLen = strlen(l_ac_buffer);
						l_ac_buffer[l_ul_fieldLen] = 0;	//Remove \n
						//l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);
						l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);

						if (l_i_Ret != MORPHO_OK)
							fprintf(stdout, "Error: invalid field\n");
						else
						{
							l_i_Ret = io_px_data->m_x_user.DbUpdatePublicFields();
							if (l_i_Ret == MORPHO_OK)
								fprintf(stdout, "Field updated successfully\n");
							else
								fprintf(stdout, "Error %d while trying to update field content\n", l_i_Ret);
						}
					}

					io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
				}
				else
				{
					fprintf(stdout, "Error %d while trying to retrieve PIN for user %ld\n", l_i_Ret, l_ul_userIndex);
				}
			}
			// Select user by PIN on other devices
			else
			{
				getUserInput(l_ac_buffer, "Select user PIN to update:\n");

				l_i_Ret = io_px_data->m_x_database.GetUser((UL)strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);
				//l_i_Ret = io_px_data->m_x_database.GetUser((UC)strlen(l_ac_buffer), (PUC)l_ac_buffer, l_x_user);
				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Invalid user selection\n");
				else {
					printDatabaseFields(io_px_data->m_x_database, MORPHO_PUBLIC_FIELD, &l_ul_nbField, NULL);

					getUserInput(l_ac_buffer, "Select field index to modify:");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldIndex);

					getUserInput(l_ac_buffer, "Enter new field value:\n");
					l_ul_fieldLen = strlen(l_ac_buffer);
					l_ac_buffer[l_ul_fieldLen] = 0;	//Remove \n
					//l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);
					l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);

					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Error: invalid field\n");
					else
					{
						l_i_Ret = io_px_data->m_x_user.DbUpdatePublicFields();
						if (l_i_Ret == MORPHO_OK)
							fprintf(stdout, "Field updated successfully\n");
						else
							fprintf(stdout, "Error %d while trying to update field content\n", l_i_Ret);
					}
				}
			}
			break;

		/////////////////////////////////////////////
		// Modify private user information
		/////////////////////////////////////////////
		case 6:
			// Select user by index on CBI/MSI
			if ((io_px_data->m_e_deviceType == DEVICE_CBI) || (io_px_data->m_e_deviceType == DEVICE_MSI))
			{
				fprintf(stdout, "Select user to update:\n");
				//listDatabaseUsers(io_px_data);
				getUserInput(l_ac_buffer, "");

				if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_userIndex) != 1)
				{
					fprintf(stdout, "Invalid user index\n");
					break;
				}

				l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_userIndex, &l_ul_PINLength, &l_puc_PIN);

				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);
					//l_i_Ret = io_px_data->m_x_database.GetUser((UC)strlen(l_ac_buffer), (PUC)l_ac_buffer, l_x_user);
					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Invalid user selection\n");
					else {
						printDatabaseFields(io_px_data->m_x_database, MORPHO_PRIVATE_FIELD, &l_ul_nbField, NULL);

						getUserInput(l_ac_buffer, "Select field index to modify:");
						sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldIndex);

						getUserInput(l_ac_buffer, "Enter new field value:\n");
						l_ul_fieldLen = strlen(l_ac_buffer);
						l_ac_buffer[l_ul_fieldLen] = 0;	//Remove \n
						//l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);
						l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);

						if (l_i_Ret != MORPHO_OK)
							fprintf(stdout, "Error: invalid field\n");
						else
						{
							l_i_Ret = io_px_data->m_x_user.DbVerifyAndUpdate(
																		0,	//US							i_us_Timeout,
																		MORPHO_FAR_5,	//T_MORPHO_FAR				i_us_FAR,
																		l_ul_asyncEvents,	//UL							i_ul_CallbackCmd,
																		eventCallback,	//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
																		io_px_data,	//PVOID						i_pv_CallbackArgument,
																		MORPHO_DEFAULT_CODER,	//I							i_i_CoderChoice,
																		0,	//UL							i_ul_DetectModeChoice,
																		MORPHO_STANDARD_MATCHING_STRATEGY	//UL							i_ul_MatchertModeChoice
																		);
							if (l_i_Ret == MORPHO_OK)
								fprintf(stdout, "Field updated successfully\n");
							else if (l_i_Ret == MORPHOERR_NO_HIT)
								fprintf(stdout, "Failed to authenticate user to update private data\n");
							else
								fprintf(stdout, "Error %d while trying to update field content\n", l_i_Ret);
						}
					}
					io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
				}
				else
				{
					fprintf(stdout, "Error %d while trying to retrieve PIN for user index %ld\n", l_i_Ret, l_ul_userIndex);
				}
			}
			// Select user by PIN for other devices
			else
			{
				getUserInput(l_ac_buffer, "Select user PIN to update:\n");
				//listDatabaseUsers(io_px_data);

				l_i_Ret = io_px_data->m_x_database.GetUser((UL)strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);

				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Invalid user selection\n");
				else {
					printDatabaseFields(io_px_data->m_x_database, MORPHO_PRIVATE_FIELD, &l_ul_nbField, NULL);

					getUserInput(l_ac_buffer, "Select field index to modify:");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldIndex);

					getUserInput(l_ac_buffer, "Enter new field value:\n");
					l_ul_fieldLen = strlen(l_ac_buffer);
					l_ac_buffer[l_ul_fieldLen] = 0;	//Remove \n
					//l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);
					l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_ul_fieldLen, (PUC)l_ac_buffer);

					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Error: invalid field\n");
					else
					{
						l_i_Ret = io_px_data->m_x_user.DbVerifyAndUpdate(
																	0,	//US							i_us_Timeout,
																	MORPHO_FAR_5,	//T_MORPHO_FAR				i_us_FAR,
																	l_ul_asyncEvents,	//UL							i_ul_CallbackCmd,
																	eventCallback,	//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
																	io_px_data,	//PVOID						i_pv_CallbackArgument,
																	MORPHO_DEFAULT_CODER,	//I							i_i_CoderChoice,
																	0,	//UL							i_ul_DetectModeChoice,
																	MORPHO_STANDARD_MATCHING_STRATEGY	//UL							i_ul_MatchertModeChoice
																	);
						if (l_i_Ret == MORPHO_OK)
							fprintf(stdout, "Field updated successfully\n");
						else if (l_i_Ret == MORPHOERR_NO_HIT)
							fprintf(stdout, "Failed to authenticate user to update private data\n");
						else
							fprintf(stdout, "Error %d while trying to update field content\n", l_i_Ret);
					}
				}
			}
			break;

		/////////////////////////////////////////////
		// Get user filter data
		/////////////////////////////////////////////
		case 7:
			fprintf(stdout, "Select user:\n");
			//listDatabaseUsers(io_px_data);
			getUserInput(l_ac_buffer, "");

			if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_userIndex) != 1)
			{
				fprintf(stdout, "Invalid user index\n");
				break;
			}

			l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_userIndex, &l_ul_PINLength, &l_puc_PIN);

			if (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = io_px_data->m_x_database.GetUser(l_ul_PINLength, l_puc_PIN, io_px_data->m_x_user);
				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Invalid user selection\n");
				else {
					printDatabaseFields(io_px_data->m_x_database, MORPHO_FILTER_FIELD, &l_ul_nbField, NULL);

					getUserInput(l_ac_buffer, "Select filter index to display:");
					sscanf(l_ac_buffer, "%d", (int*)&l_ul_fieldIndex);

					l_i_Ret = io_px_data->m_x_user.GetFilterData(l_ul_fieldIndex, l_ul_fieldLen, l_puc_aud);

					if (l_i_Ret != MORPHO_OK)
						fprintf(stdout, "Error: invalid field\n");

					// Display hexadecimal filter value
					else
					{
						UL	charIndex;
						fprintf(stdout, "Filter value: 0x");
						for (charIndex = 0; charIndex < l_ul_fieldLen; charIndex++)
						{
							fprintf(stdout, "%02X", l_puc_aud[charIndex]);
						}
						fprintf(stdout, "\n");
					}
				}
				io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
			}
			else if (l_i_Ret == MORPHOERR_UNAVAILABLE)
			{
				fprintf(stdout, "Filtering functions unavailable for current devices\n");
			}
			else
			{
				fprintf(stdout, "Error %d while trying to retrieve PIN for user index %ld\n", l_i_Ret, l_ul_userIndex);
			}
			break;

#if 0 // Not implemented yet
		/////////////////////////////////////////////
		// Verify match in database
		/////////////////////////////////////////////
		case 8:
		{
			C_MORPHO_TemplateList	l_x_pkList;
			PUC						l_puc_userID;
			UL						l_ul_matchingScore;

			fprintf(stdout, "Select user index to verify:\n");
			//listDatabaseUsers(io_px_data);
			getUserInput(l_ac_buffer, "");

			if (sscanf(l_ac_buffer, "%d", (int*)&l_ul_userIndex) != 1)
			{
				fprintf(stdout, "Invalid user index\n");
				break;
			}

			l_i_Ret = io_px_data->m_x_database.GetUserPin(l_ul_userIndex, &l_ul_PINLength, &l_puc_PIN);

			if (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = io_px_data->m_x_database.GetUser((UC)strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);
				if (l_i_Ret != MORPHO_OK)
					fprintf(stdout, "Invalid user selection\n");
				else {
					getUserInput(l_ac_buffer, "Enter template file name:\n");
					if (storePkFromFile(l_ac_buffer, l_x_pkList) == MORPHO_OK)
					{
						io_px_data->m_x_user.GetField(0, l_ul_fieldLen, l_puc_userID);
						l_i_Ret = io_px_data->m_x_user.VerifyMatch_Db(MORPHO_FAR_5, l_x_pkList, l_puc_userID, l_ul_fieldLen, 0, &l_ul_matchingScore);

						switch(l_i_Ret)
						{
						case MORPHO_OK:
							fprintf(stdout, "Template file %s successfully verified against user %s in database, matching score: %lu\n", l_ac_buffer, l_puc_userID, l_ul_matchingScore);
							break;
						case MORPHOERR_UNAVAILABLE:
							fprintf(stdout, "Feature unavailable on current device\n");
							break;
						default:
							fprintf(stdout, "Error %d while trying to verify template file %s with user %s\n", l_i_Ret, l_ac_buffer, l_puc_userID);
						}
					}
				}
				io_px_data->m_x_database.ReleaseUserPin(&l_puc_PIN);
			}
			else
			{
				fprintf(stdout, "Error %d while trying to retrieve PIN for user %ld\n", l_i_Ret, l_ul_userIndex);
			}
		}
			break;
#endif	// VerifyMatch_Db

		/////////////////////////////////////////////
		// Back to main menu
		/////////////////////////////////////////////
		case 0:
			l_uc_loop = 0;
			break;
		default:
			fprintf(stdout, "Invalid command\n");
			break;
		}
	}

	return 0;
}

I listDatabaseUsers(PT_DATA io_px_data)
{
	I							l_i_Ret;
	UL							l_ul_nbField;
	UL							l_ul_nbUsers;
	UL							l_ul_userIndex;
	//UC							l_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX_BIT];
	C_MORPHO_UserList			l_x_UserList;
	C_MORPHO_User				*l_px_user;

	int TRead1, TRead2, TPrint1, TPrint2;

	//l_i_Ret = io_px_data->m_x_database.FillIndexDescriptor(TRUE, 0, l_auc_FieldIndexDescriptor);

	TRead1 = SampleGetTickCount();
	printDatabaseFields(io_px_data->m_x_database, MORPHO_PUBLIC_FIELD, &l_ul_nbField, &l_x_UserList);
	TRead2 = SampleGetTickCount();

	/*
	l_i_Ret = io_px_data->m_x_database.ReadPublicFields(l_auc_FieldIndexDescriptor, l_x_UserList);
	*/

	l_i_Ret = l_x_UserList.GetNbUser(l_ul_nbUsers);

	TPrint1 = SampleGetTickCount();
	for (l_ul_userIndex = 0; l_ul_userIndex < l_ul_nbUsers; l_ul_userIndex++)
	{
		l_x_UserList.GetUser(l_ul_userIndex, l_px_user);
		printUserFields(*l_px_user, l_ul_nbField);
	}
	TPrint2 = SampleGetTickCount();

	fprintf(stdout, "ReadPublicFields: %dms\nprintUserFields: %dms\n", TRead2 - TRead1, TPrint2 - TPrint1);

	return l_i_Ret;
}

I printDatabaseFields(C_MORPHO_Database &i_x_database, T_MORPHO_FIELD_ATTRIBUTE i_e_fieldType, PUL o_pul_nbFields, C_MORPHO_UserList *o_px_UserList)
{
	I							l_i_Ret;
	UL							l_ul_index;
	UL							l_ul_nbField;
	T_MORPHO_FIELD_ATTRIBUTE	l_e_attribute;
	US							l_us_fieldLen;
	UC							l_auc_fieldName[MORPHO_FIELD_NAME_LEN+1];
	UC							l_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX_BIT];

	l_i_Ret = i_x_database.GetNbField(l_ul_nbField);

	if (o_pul_nbFields != NULL)
		*o_pul_nbFields = 0;

	l_i_Ret = i_x_database.FillIndexDescriptor(TRUE, 0, l_auc_FieldIndexDescriptor);

	if (i_e_fieldType == MORPHO_PUBLIC_FIELD)
	{
		fprintf(stdout, "PIN (0)\t| ");

		if (o_pul_nbFields != NULL)
			(*o_pul_nbFields)++;
	}

	for (l_ul_index = 1; l_ul_index < l_ul_nbField+1; l_ul_index++)
	{
		memset(l_auc_fieldName, 0, MORPHO_FIELD_NAME_LEN+1);
		l_i_Ret = i_x_database.GetField(l_ul_index, l_e_attribute, l_us_fieldLen, l_auc_fieldName);

		if ((l_i_Ret == MORPHO_OK)&&(l_e_attribute == i_e_fieldType))
		{
			fprintf(stdout, "%s (%ld)", l_auc_fieldName, l_ul_index);

			l_i_Ret = i_x_database.FillIndexDescriptor(FALSE, l_ul_index, l_auc_FieldIndexDescriptor);

			if (o_pul_nbFields != NULL)
			{
				(*o_pul_nbFields)++;
			}
		}
		else if (l_i_Ret != MORPHO_OK)
			fprintf(stdout, "Error %d while retrieving user field %ld\n", l_i_Ret, l_ul_index);

		if (l_ul_index < l_ul_nbField)
			fprintf(stdout, "\t| ");
		else
			fprintf(stdout, "\n");
	}

	if (o_px_UserList != NULL)
	{
		l_i_Ret = i_x_database.ReadPublicFields(l_auc_FieldIndexDescriptor, *o_px_UserList);
	}

	return l_i_Ret;
}

I printUserFields (C_MORPHO_User &i_x_user, UL i_ul_nbFields)
{
	I		l_i_Ret;
	UL		l_ul_index;
	UL		l_ul_fieldLen;
	PUC		l_puc_fieldData;
	UC		l_auc_stringBuffer[MAX_FIELD_SIZE];

	for (l_ul_index = 0; l_ul_index < i_ul_nbFields+1; l_ul_index++) {
		l_i_Ret = i_x_user.GetField(l_ul_index, l_ul_fieldLen, l_puc_fieldData);
		if (l_i_Ret == MORPHO_OK)
		{
			memcpy(l_auc_stringBuffer, l_puc_fieldData, l_ul_fieldLen);
			l_auc_stringBuffer[l_ul_fieldLen] = 0;
			fprintf(stdout, "%s", l_auc_stringBuffer);
		}
		if (l_ul_index == i_ul_nbFields)
			fprintf(stdout, "\n");
		else
			fprintf(stdout, "\t| ");
	}

	return 0;
}

I storePkFromFile(const char *i_pc_fileName, C_MORPHO_TemplateList &io_x_template)
{
	I						l_i_Ret = MORPHOERR_INTERNAL;
	FILE					*l_px_file;
	UL						l_ul_PkSize;
	PUC						l_puc_PkBuf;
	UC						l_uc_templateIndex;
	T_MORPHO_TYPE_TEMPLATE				l_x_TypeTemplate;

	l_px_file = fopen(i_pc_fileName, "rb");

	if (l_px_file == NULL)
	{
		fprintf(stdout, "Unable to open file %s\n", i_pc_fileName);
		l_i_Ret = MORPHOERR_BADPARAMETER;
	}
	else
	{
		fseek(l_px_file, 0, SEEK_END);
		l_ul_PkSize = ftell(l_px_file);
		fseek(l_px_file, 0, SEEK_SET);

		l_puc_PkBuf = (PUC)malloc(l_ul_PkSize);
		fread(l_puc_PkBuf, 1, l_ul_PkSize, l_px_file);

		l_x_TypeTemplate = getTemplateType();

		l_i_Ret = io_x_template.PutTemplate(l_x_TypeTemplate,
											l_ul_PkSize,
											l_puc_PkBuf,
											0,	//UC  i_uc_PkFpQuality,
											l_uc_templateIndex);
		free(l_puc_PkBuf);
	}
	return l_i_Ret;
}

void getTemplateExtension(T_MORPHO_TYPE_TEMPLATE i_x_TypeTemplate,PC o_pc_extension)
{
	if(o_pc_extension == NULL)
	{
		fprintf(stdout, "Error getTemplateExtension : parameter o_pc_extension is NULL\n");
		return;
	}

	switch (i_x_TypeTemplate)
	{
		case MORPHO_PK_COMP:
			strcpy (o_pc_extension, ".pkcomp");
			break;
		case MORPHO_PK_COMP_NORM:
			strcpy (o_pc_extension, ".pkcn");
			break;
		case MORPHO_PK_MAT_NORM:
			strcpy (o_pc_extension, ".pkmn");
			break;
		case MORPHO_PK_ANSI_378_2009:
			strcpy (o_pc_extension, ".ansi-fmr-2009");
			break;
		case MORPHO_PK_ISO_FMR_2011:
			strcpy (o_pc_extension, ".iso-fmr-2011");
			break;
		case MORPHO_PK_MAT:
			strcpy (o_pc_extension, ".pkmat");
			break;
		case MORPHO_PK_ANSI_378:
			strcpy (o_pc_extension, ".ansi-fmr");
			break;
		case MORPHO_PK_MINEX_A:
			strcpy (o_pc_extension, ".minex-a");
			break;
		case MORPHO_PK_ISO_FMR:
			strcpy (o_pc_extension, ".iso-fmr");
			break;
		case MORPHO_PK_ISO_FMC_NS:
			strcpy (o_pc_extension, ".iso-fmc-ns");
			break;
		case MORPHO_PK_ISO_FMC_CS:
			strcpy (o_pc_extension, ".iso-fmc-cs");
			break;
		case MORPHO_PK_DIN_V66400_CS:
			strcpy (o_pc_extension, ".din-cs");
			break;
		case MORPHO_PK_DIN_V66400_CS_AA:
			strcpy (o_pc_extension, ".din-cs-aa");
			break;
		case MORPHO_PK_PKLITE:
			strcpy (o_pc_extension, ".pkl");
			break;
		case MORPHO_PK_ISO_FMC_CS_AA:
			strcpy (o_pc_extension, ".iso-fmc-cs-aa");
			break;
		default:
			strcpy (o_pc_extension, ".cfv");
			break;
	}
}

T_MORPHO_TYPE_TEMPLATE getTemplateType()
{
	C						l_ac_buffer[INPUT_BUFFER_SIZE];
	I						l_i_TypeTemplateChoice;
	T_MORPHO_TYPE_TEMPLATE	l_x_TypeTemplate = MORPHO_PK_CFV;

	getUserInput(l_ac_buffer, "\n\nTemplate type menu:\n"
								"1  - PK_COMP\n"
								"2  - PK_COMP_NORM\n"
								"3  - PK_MAT\n"
								"4  - PK_MAT_NORM\n"
								"5  - PK_ANSI_378\n"
								"6  - PK_ANSI_378_2009\n"
								"7  - PK_MINEX_A\n"
								"8  - PK_ISO_FMR\n"
								"9  - PK_ISO_FMR_2011\n"
								"10 - PK_ISO_FMC_NS\n"
								"11 - PK_ISO_FMC_CS\n"
								"12 - PK_DIN_V66400_CS\n"
								"13 - PK_DIN_V66400_CS_AA\n"
								"14 - PK_ISO_FMC_CS_AA\n"
								"0  - PKLITE\n"
								"Any other - PK_CFV\n");
	sscanf(l_ac_buffer, "%d", &l_i_TypeTemplateChoice);

	switch (l_i_TypeTemplateChoice )
	{
		case 1:
			l_x_TypeTemplate = MORPHO_PK_COMP;
			break;
		case 2:
			l_x_TypeTemplate = MORPHO_PK_COMP_NORM;
			break;
		case 3:
			l_x_TypeTemplate = MORPHO_PK_MAT;
			break;
		case 4:
			l_x_TypeTemplate = MORPHO_PK_MAT_NORM;
			break;
		case 5:
			l_x_TypeTemplate = MORPHO_PK_ANSI_378;
			break;
		case 6:
			l_x_TypeTemplate = MORPHO_PK_ANSI_378_2009;
			break;
		case 7:
			l_x_TypeTemplate = MORPHO_PK_MINEX_A;
			break;
		case 8:
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR;
			break;
		case 9:
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR_2011;
			break;
		case 10:
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_NS;
			break;
		case 11:
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS;
			break;
		case 12:
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS;
			break;
		case 13:
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS_AA;
			break;
		case 14:
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS_AA;
			break;
		case 0:
			l_x_TypeTemplate = MORPHO_PK_PKLITE;
			break;
		default:
			l_x_TypeTemplate = MORPHO_PK_CFV;
			break;
	}

	return l_x_TypeTemplate;
}

I getUserInput(char o_auc_buffer[INPUT_BUFFER_SIZE], const char*message)
{
	fprintf(stdout, "%s", message);
	fgets(o_auc_buffer, INPUT_BUFFER_SIZE, stdin);
	o_auc_buffer[strlen(o_auc_buffer) - 1] = '\0';	// Remove trailing \n

	return 0;
}

I fillNewUserFields(PT_DATA	io_px_data)
{
	I							l_i_Ret;
	UL							l_ul_nbField, l_ul_fieldIndex, l_ul_pkUpdateMask;
	C							l_ac_buffer[INPUT_BUFFER_SIZE];
	US							l_us_maxSize;
	T_MORPHO_FIELD_ATTRIBUTE	l_uc_fieldAttribute;
	UC							l_auc_fieldName[MORPHO_FIELD_NAME_LEN+1];
	UC							l_auc_buffer[256];
	I							l_i_charIndex;

	l_i_Ret = io_px_data->m_x_database.GetNbField(l_ul_nbField);

	getUserInput(l_ac_buffer, "Enter user ID:\n");

	l_i_Ret = io_px_data->m_x_database.GetUser(strlen(l_ac_buffer), (PUC)l_ac_buffer, io_px_data->m_x_user);

	l_i_Ret = io_px_data->m_x_user.SetTemplateUpdateMask(0);

	getUserInput(l_ac_buffer, "Update existing fingerprint? ('y' or 'yes', skip for default behaviour and create a new record in the database)\n");

	if ((strcasecmp(l_ac_buffer, "y") == 0) || (strcasecmp(l_ac_buffer, "yes") == 0))
	{
		getUserInput(l_ac_buffer, "Select finger index to update:\n"
				"0 - cancel update and create a new record (user ID must not be already in database)\n"
				"1 - update first finger\n"
				"2 - add or update second finger\n"
				"3 - update both fingers\n");

		if (sscanf(l_ac_buffer, "%ld", &l_ul_pkUpdateMask) == 1)
		{
			l_i_Ret = io_px_data->m_x_user.SetTemplateUpdateMask(l_ul_pkUpdateMask);
			if (l_i_Ret != MORPHO_OK)
			{
				fprintf(stdout, "Invalid value, ignoring template update and create a new record\n");
			}
		}
		else
		{
			fprintf(stdout, "Invalid entry, attempt to create a new record\n");
		}
	}

	for (l_ul_fieldIndex = 1; l_ul_fieldIndex < l_ul_nbField+1; l_ul_fieldIndex++) {
		l_i_Ret = io_px_data->m_x_database.GetField(l_ul_fieldIndex, l_uc_fieldAttribute, l_us_maxSize, l_auc_fieldName);
		l_auc_fieldName[MORPHO_FIELD_NAME_LEN] = 0;
		if (l_uc_fieldAttribute == MORPHO_FILTER_FIELD)
		{
			fprintf(stdout, "%s (enter filter value in hexadecimal, %d bytes):\n", l_auc_fieldName, l_us_maxSize);
			getUserInput(l_ac_buffer, "");
			while (strlen(l_ac_buffer) != 2*l_us_maxSize)
			{
				fprintf(stdout, "Inavlid input, please enter hexadecimal filter value for %d bytes:\n", l_us_maxSize);
				getUserInput(l_ac_buffer, "");
			}

			for (l_i_charIndex = 0; l_i_charIndex < l_us_maxSize; l_i_charIndex++)
			{
				l_auc_buffer[l_i_charIndex] = htoi(l_ac_buffer[l_i_charIndex*2]) << 4;
				l_auc_buffer[l_i_charIndex] += htoi(l_ac_buffer[l_i_charIndex*2+1]);
			}
			l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, l_us_maxSize, l_auc_buffer);
		}
		else if (l_uc_fieldAttribute == MORPHO_STAT_FIELD)
		{
			UL	stats;
			fprintf(stdout, "%s (matching statistics):\n", l_auc_fieldName);
			getUserInput(l_ac_buffer, "");
			while (sscanf(l_ac_buffer, "%d", (int*)&stats) != 1)
			{
				fprintf(stdout, "Invalid value, please enter a positive integer:\n");
				getUserInput(l_ac_buffer, "");
			}
			l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, sizeof(UL), (PUC)&stats);
		}
		else
		{
			fprintf(stdout, "%s:\n", l_auc_fieldName);
			getUserInput(l_ac_buffer, "");
			l_i_Ret = io_px_data->m_x_user.PutField(l_ul_fieldIndex, strlen(l_ac_buffer), (PUC)l_ac_buffer);
		}
	}

	return l_i_Ret;
}

I eventCallback(
		PVOID						i_pv_context,
		T_MORPHO_CALLBACK_COMMAND	i_i_command,
		PVOID						i_pv_param)
{
	PUC								l_puc_EnrollmentCmd;

	if (i_i_command == MORPHO_CALLBACK_COMMAND_CMD)
	{
		switch(*(PI)i_pv_param)
		{
		case MORPHO_MOVE_NO_FINGER:
			break;
		case MORPHO_REMOVE_FINGER:
			fprintf(stdout, "Remove finger\n");
			break;
		case MORPHO_LATENT:
			fprintf(stdout, "Latent detected\n");
			break;
		case MORPHO_MOVE_FINGER_UP:
			fprintf(stdout, "Move finger up\n");
			break;
		case MORPHO_MOVE_FINGER_DOWN:
			fprintf(stdout, "Move finger down\n");
			break;
		case MORPHO_MOVE_FINGER_LEFT:
			fprintf(stdout, "Move finger left\n");
			break;
		case MORPHO_MOVE_FINGER_RIGHT:
			fprintf(stdout, "Move finger right\n");
			break;
		case MORPHO_PRESS_FINGER_HARDER:
			fprintf(stdout, "Increase finger pressure\n");
			break;
		case MORPHO_FINGER_OK:
			fprintf(stdout, "Acquisition OK\n");
			break;
		case MORPHOERR_FFD:
			fprintf(stdout, "False finger detected\n");
			break;
		case MORPHOERR_MOIST_FINGER:
			fprintf(stdout, "Finger too moist\n");
			break;
		case MORPHO_FINGER_MISPLACED:
			fprintf(stdout, "Bad finger placement\n");
			break;
		case MORPHO_FINGER_DETECTED:
			break;
		case MORPHO_LIVE_OK:
			fprintf(stdout, "Finger detected, processing ...\n");
			break;
		default:
			break;
		}
	}
	else if (i_i_command == MORPHO_CALLBACK_ENROLLMENT_CMD)
	{
		l_puc_EnrollmentCmd = (PUC)i_pv_param;

		switch (l_puc_EnrollmentCmd[2])
		{
		case 1:
			fprintf(stdout, "PLACE Finger #%d\nCAPTURE %d/%d\n",l_puc_EnrollmentCmd[0],l_puc_EnrollmentCmd[2],l_puc_EnrollmentCmd[3]);
			break;

		case 2:
			fprintf(stdout, "Finger #%d AGAIN\nCAPTURE %d/%d\n",l_puc_EnrollmentCmd[0],l_puc_EnrollmentCmd[2],l_puc_EnrollmentCmd[3]);
			break;

		case 3:
			fprintf(stdout, "Finger #%d AGAIN\nCAPTURE %d/%d\n",l_puc_EnrollmentCmd[0],l_puc_EnrollmentCmd[2],l_puc_EnrollmentCmd[3]);
			break;
		}
	}
#ifdef ENABLE_DISPLAY
	else if (i_i_command == MORPHO_CALLBACK_IMAGE_CMD)
	{
		memcpy (&l_x_ImageStructure.m_x_ImageHeader,
			(T_MORPHO_IMAGE_HEADER *) i_pv_param,
			sizeof (T_MORPHO_IMAGE_HEADER));
		l_x_ImageStructure.m_puc_Image =
		(PUC) i_pv_param + sizeof (T_MORPHO_IMAGE_HEADER);

		Display_Image (l_x_ImageStructure.m_puc_Image,
			l_x_ImageStructure.m_x_ImageHeader.m_us_NbRow,
			l_x_ImageStructure.m_x_ImageHeader.m_us_NbCol,
			l_x_ImageStructure.m_x_ImageHeader.m_uc_NbBitsPerPixel);
		SDL_PollEvent (&event);
		if (event.type == SDL_QUIT)
			l_px_data->m_x_device.CancelLiveAcquisition();

	}
#endif

	return 0;
}

int LEDEvent(
	const void *param,
	int state)
{
	if (state)
		fprintf(stdout, "*** LED turned ON\n");
	else
		fprintf(stdout, "*** LED turned OFF\n");

	return 0;
}

int FFDEvent(int* ffdState)
{
	return 0;
}

void *cancelThread(void *userParam)
{
	I		l_i_ret;
	PT_DATA	l_px_data = (PT_DATA)userParam;

	sleep(5);

	l_i_ret = l_px_data->m_x_device.CancelLiveAcquisition();
	fprintf(stdout, "Cancel live acquisition returned %d\n", l_i_ret);

	return NULL;
}

void *qualityThread(void *userParam)
{
	I		l_i_ret;
	PT_DATA	l_px_data = (PT_DATA)userParam;
	UL		l_ul_quality;

	sleep(1);

	l_i_ret = l_px_data->m_x_device.GetQualityThreshold(&l_ul_quality);
	if (l_i_ret == MORPHO_OK)
		fprintf(stdout, "Current quality: %ld\n", l_ul_quality);
	else {
		fprintf(stdout, "Error %d while retrieving quality threshold\n", l_i_ret);
		return NULL;
	}

	l_i_ret = l_px_data->m_x_device.SetQualityThreshold(85);

	l_i_ret = l_px_data->m_x_device.GetQualityThreshold(&l_ul_quality);
	if (l_i_ret == MORPHO_OK)
		fprintf(stdout, "New quality: %ld\n", l_ul_quality);
	else {
		fprintf(stdout, "Error %d while retrieving quality threshold\n", l_i_ret);
		return NULL;
	}

	return NULL;
}

