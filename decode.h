#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include "common.h"

/*
 * Structure to store information required for
 * decoding stego image to new file
 * Info about output and intermediate data is 
 * also stored
 */

typedef struct  _DecodeInfo
{
	/* Encoded stego image info */
	char *stego_image_fname;
	FILE *fptr_stego_image;
	uint image_data_size;

	/* Decoded output file info */
	char *output_file_name;
	FILE *fptr_output_file;
	char *d_extn_secret_file;
	char *magic_data;

}	DecodeInfo;


/* Decoding function prototypes */

/* Read and validation Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointer for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Performs Decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get the magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode data from image */
Status decode_data_from_image(int size, FILE *fptr_stego_image, DecodeInfo *decInfo);

/* Decode byte from lsb */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode file extention size */
Status decode_output_file_extn_size(int, FILE *);

/* Decode size from lsb */
Status decode_size_from_lsb (char *buffer, int *size);

/* Decode secret file extn */
Status decode_secret_file_extn (char *file_ext, DecodeInfo *decInfo) ;

/* Decode extension data from image */
Status decode_extension_data_from_image ( int size, FILE *fptr_d_src_image, DecodeInfo *decInfo);

/* Decode file extenstion */
Status decode_output_file_extn(uint extn_size, DecodeInfo *decInfo);

/* Decode file size */
Status decode_file_size(int, DecodeInfo *decInfo);

/* Decoding file data */
Status decode_data_to_file(DecodeInfo *decInfo);

/* Decode a array of LSB into byte */
Status decode_lsb_to_byte(char *decode_data, char *image_data);

#endif
