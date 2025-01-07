/* 
Name : Naveen ps
Date : 20-11-24
Title : LSB-Steganography
		(Programme to encode secret file data to source image file by creating new image file with option '-e' and decode data from new image file to new text file)
Sample execution :
----For Encoding----
Input :- ./a.out -e beautiful.bmp secret.txt pk.bmp
Output :-
Selected encoding
Read and validate function is successfully executed
Files are opened successfully
width = 1024
height = 768
Check capacity function successfully done
Copying image header successful
Info : Encoding magic string Done
Info : Encoding secret file extn size is success
Info : Encoding secret file extn success
Info : Encoding secret file size is success
Info : Encoding secret file data is success
Info : copy remaining data is success
INFO: Encoding Done Successfully

----For Decoding----
Input :- ./a.out -d pk.bmp output.txt
Output :-
Selected decoding
Read and validate function is successfully executed
INFO: Decoding Procedure Started
INFO: Opening required files
INFO: Opened pk.bmp
INFO: Done
INFO: Decoding Magic String Signature
INFO: Done
INFO: Decoding Output File Extension
INFO: Done
INFO: Opended output.txt
INFO: Done
INFO: Done. Opened all required files
INFO: Done
INFO: Decoding File Size
INFO: Done
INFO: Decoding File Data
INFO: Done
INFO: Decoding Done Successfully
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
  //Validate the argc count
  if (argc >= 3)
  {
	// Function call for check operation type
	int operation = check_operation_type(argv);
	
	if (operation == e_encode)
	{
		printf("Selected encoding\n");
		EncodeInfo encode_info;		// Declare structure variable
		// Read and validate encode arguments 
		if (read_and_validate_encode_args(argv, &encode_info) == e_success)
		{
			printf("Read and validate function is successfully executed\n");
			// Function call for encoding
			if (do_encoding(&encode_info) == e_success)
			{
				fclose(encode_info.fptr_src_image);				//_
				fclose(encode_info.fptr_secret);				//_|-After encoding, close the opened files	
				fclose(encode_info.fptr_stego_image);			//
				printf("INFO: Encoding Done Successfully \n");
			}
			else
			{
				printf("Do encoding function failure");
				return 1;
			}
		}
		else
		{
			printf("Read and validate function is failure\n");
			return -1;
		}
	}
	else if (operation == e_decode)			//check value of "operation"
	{
		printf("Selected decoding\n");
		DecodeInfo decInfo;					// // Declare structure variables
		// Read and validate decode arguments 
		if (read_and_validate_decode_args(argv, &decInfo) == e_success)
		{
			printf("Read and validate function is successfully executed\n");
			 // Function call for do decoding
			if (do_decoding(&decInfo) == e_success)
			{
				printf("INFO: Decoding Done Successfully \n");
				fclose(decInfo.fptr_stego_image);
				fclose(decInfo.fptr_output_file);
			}
			else
			{
				printf("Do decoding function failure");
				return 1;
			}
		}
		else
		{
			printf("Read and validate function is failure\n");
			return -1;
		}
	}
	else
		printf("Operation is unvalid\nPass for\nEncoding: ./a.out -e beautiful.bmp secret.txt pk.bmp\nDecoding: ./a.out -d pk.bmp output.txt\n");
  }
  else
		printf("Error : Kindly pass for\nEncoding: 4 arguments\nDecoding: 3 arguments\n");
	return 0;
}

OperationType check_operation_type(char *argv[])		//function defination to check if selected encoding or decoding
{
	if (strcmp(argv[1], "-e") == 0)
	{
		return e_encode;
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
		return e_decode;
	}
	else
		return e_unsupported;			
}




