#include<stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include "common.h"
#include <stdlib.h>

/* Function Definiation */

/* Get File names and validate
 * Input: Stego Image file name and Output file name
 * Output: Stores the above files names
 * Return Value: e-success or e_failure, on file errors
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
		if (strcmp(strstr(argv[2],"."), ".bmp") == 0)
        {
			decInfo->stego_image_fname = argv[2];
        }
        else
        {
            return e_failure;
        }
		if (argv[3] != NULL)
		{
			decInfo->output_file_name = argv[3];
		}
		else
		{
			decInfo->output_file_name = "output.txt";
		}
		return e_success;
}

/* Get File pointers for i/p and o/p files
 * Inputs: Stego Image file, Output file
 * Ouput: File pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_decode_files(DecodeInfo *decInfo)
{
	static int open = 0;
	if (open == 0)
	{
		open++;
		// Open Stego Image file
		decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

		// Error handling
		if (decInfo->fptr_stego_image == NULL)
		{
			perror("fopen");
			fprintf(stderr, "Error: Unable to open file %s\n", decInfo->stego_image_fname);
			return e_failure;
		}
		else
		{
			printf("INFO: Opened %s\n", decInfo->stego_image_fname);
		}
	}
	else 
	{
		// Open Output file
		decInfo->fptr_output_file = fopen(decInfo->output_file_name, "w");
		// Error handling
		if (decInfo->fptr_output_file == NULL)
		{
			perror("fopen");
			fprintf(stderr, "Error: Unable to open file %s\n", decInfo->output_file_name);
			return e_failure;
		}
		else
		{
			printf("INFO: Opended %s\n", decInfo->output_file_name);
		}
	}
	return e_success;
}

/* Decoding stego image to another file
 * Input: File info of stego image and output file
 * Output: Decoded message copied in output file
 * Return: e_success or e_failure, on file error
 */
Status do_decoding(DecodeInfo *decInfo)
{
	printf("INFO: Decoding Procedure Started \n");
	printf("INFO: Opening required files\n");
	
	// Open the required files
	if (open_decode_files(decInfo) == e_success)
	{
		printf("INFO: Done\n");
		uint raster_data;
		fseek(decInfo->fptr_stego_image, 10L, SEEK_SET);
		fread(&raster_data, sizeof(int), 1, decInfo->fptr_stego_image);
		fseek(decInfo->fptr_stego_image, raster_data, SEEK_SET);
		printf("INFO: Decoding Magic String Signature\n");
		
		// Decoding magic string signature
		if (decode_magic_string(decInfo) == e_success)
		{
			printf("INFO: Done\n");
			
			// Decoding Output File Extenstion Size
			printf("INFO: Decoding Output File Extension\n");
			if (decode_output_file_extn_size(strlen(".txt"), decInfo->fptr_stego_image) == e_success)
			{
				printf("INFO: Done\n");

				// Check for output file provided or not
			
					if (open_decode_files(decInfo) == e_success)
					{
						printf("INFO: Done\n");
					}
					else
					{
						printf("open_decode_files function failed\n");
						return e_failure;
					}
				printf("INFO: Done. Opened all required files\n");
				
				if (decode_secret_file_extn(decInfo->d_extn_secret_file, decInfo) == e_success)
				{
					printf("INFO: Done\n");
				}
				else
				{
					printf("decode_secret_file_extn finction failed\n");
				}
				// Decoding File Size
				printf("INFO: Decoding File Size\n");
				if (decode_file_size(decInfo->image_data_size, decInfo) == e_success)
				{
					printf("INFO: Done\n");

					// Decoding File Data
					printf("INFO: Decoding File Data\n");
					if (decode_data_to_file(decInfo) == e_failure)
					{
						printf("decode_data_to_file() function failed\n");
						return e_failure;
					}
				}
				else
				{
					printf("decode_file_size() function failed\n");
					return e_failure;
				}
			}
			else
			{
				printf("decode_output_file_extn_size() function failed\n");
				return e_failure;
			}
		}
		else
		{
			printf("decode_magic_string() function failed\n");
			return e_failure;
		}
	}
	else
	{
		printf("open_decode_files() function failed\n");
		return e_failure;
	}
	printf("INFO: Done\n");
	return e_success;
}

/* Decodes the magic string from stego image
 * Input: 	Magic string and File info of input and output
 * Output:	Proceed decoing if magic string if found or else stop decoding
 * Return:	e_success or e_failure
 */
Status decode_magic_string(DecodeInfo *decInfo)
{
	fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    int i = strlen(MAGIC_STRING);

    decInfo->magic_data = malloc(strlen(MAGIC_STRING) + 1);
    decode_data_from_image(strlen(MAGIC_STRING), decInfo->fptr_stego_image, decInfo);

    decInfo->magic_data[i] = '\0';

    if (strcmp(decInfo->magic_data, MAGIC_STRING) == 0)
        return e_success;
    else
        return e_failure;

}

// Function definition for decoding data fom image
Status decode_data_from_image(int size, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    int i;
    char str[8];
    for (i = 0; i < size; i++)
    {
        fread(str, 8, sizeof(char), fptr_stego_image);
        decode_byte_from_lsb(&decInfo->magic_data[i], str);
    }
    return e_success;
}

// Function definition for decode byte from lsb
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    int bit = 7;
    unsigned char ch = 0x00;
    for (int i = 0; i < 8; i++)
    {
        ch = ((image_buffer[i] & 0x01) << bit--) | ch;
    }
    *data = ch;
    return e_success;
}
/* Decode file extenstion size from stego image
 * Input: File info of stego image and output file
 * Output: Decode the extenstion size from stego image 
 * Return: e_success or e_failure
 */
Status decode_output_file_extn_size(int size, FILE *fptr_stego_image)
{
	char str[32];
    int length;

    fread(str, 32, sizeof(char), fptr_stego_image);
    decode_size_from_lsb(str, &length);

    if (length == size)
        return e_success;
    else
        return e_failure;
}

// Function definition decode size from lsb
Status decode_size_from_lsb(char *buffer, int *size)
{
    int j = 31;
    int num = 0x00;
    for (int i = 0; i < 32; i++)
    {
        num = ((buffer[i] & 0x01) << j--) | num;
    }
    *size = num;
}

/* Decode file extenstion from stego image 
 * Input: Extenstion Size and File info of stego image
 * Return: e_success or e_failure
 */

// Function definition for decode secret file extn
Status decode_secret_file_extn(char *file_ext, DecodeInfo *decInfo)
{
    file_ext = ".txt";
    int i = strlen(file_ext);
    decInfo->d_extn_secret_file = malloc(i + 1);
    decode_extension_data_from_image(strlen(file_ext), decInfo->fptr_stego_image, decInfo);

    decInfo->d_extn_secret_file[i] = '\0';

    if (strcmp(decInfo->d_extn_secret_file, file_ext) == 0)
        return e_success;
    else
        return e_failure;
}

// Function definition decode extension data from image
Status decode_extension_data_from_image(int size, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        fread(decInfo->stego_image_fname, 8, 1, fptr_stego_image);
        decode_byte_from_lsb(&decInfo->d_extn_secret_file[i], decInfo->stego_image_fname);
    }
    return e_success;
}


/* Decode file size from stego image
 * Input: FILE info of stego image and output file
 * Return: e_success or e_failure
 */
Status decode_file_size(int file_size, DecodeInfo *decInfo)
{
    char str[32];
    fread(str, 32, sizeof(char), decInfo->fptr_stego_image);
    decode_size_from_lsb(str, &file_size);
    decInfo->image_data_size = file_size;

    return e_success;
}
	

/* Decode file data from stego image
 * Input: FILE info of stego image and output decode file
 * Return: e_success or e_failure
 */
Status decode_data_to_file(DecodeInfo *decInfo)
{
	 char ch;
    for (int i = 0; i < decInfo->image_data_size; i++)
    {
        fread (decInfo->stego_image_fname, 8, sizeof(char), decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, decInfo->stego_image_fname);
        fputc(ch, decInfo->fptr_output_file);
    }
    return e_success;
}

/* Get lsb bit from stego image byte 
 * Input: Image_data array and decode_data character
 * Return: e_success or e_failure
 */
Status decode_lsb_to_byte(char *decode_data, char *image_data)
{
	decode_data[0] = 0;
	for (uint i = 0; i < MAX_IMAGE_BUF_SIZE; i++)
	{
		decode_data[0] <<= 1;
		decode_data[0] |=  (image_data[i] & 0x01);
	}
	return e_success;
}

