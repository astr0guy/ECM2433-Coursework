#include "ca.h"
#include "msString.h"

extern void printBytes (void* ptr, unsigned short int numBytes)
{
	printf("Starting at memory address %p:\n", ptr);
	/*initialise iterator*/
	unsigned short int i;
	for (i=0;i<numBytes;i++)
	{
	/*Prints number & byte value, then moves the pointer forward by 1*/
		printf("%i:\t%d\n",i+1,*(unsigned char*)ptr);
		ptr = ptr + 1;
	}
}


extern void fileFlip (char *input, char *output)
{
	/*Initializes input and output file pointers*/
	FILE *ifp;
	FILE *ofp;
	char ch;
	/*opens a read-only and write-only files respectively, and throws errors if that's not possible*/
	ifp = fopen(input, "r");
	if (ifp == NULL)
	{
		fprintf(stderr, "Failed to open file %s\n", input);
		exit(-1);
	}
	ofp = fopen(output, "w");
	if (ofp == NULL)
	{
		fprintf(stderr,"Failed to open file %s\n", output);
		exit(-1);
	}
	/*Finds the end of the input file*/
	fseek(ifp, 0, SEEK_END);
	/*Reads each character in the input file in reverse order, and outputs them to the output file.*/
	while (fseek(ifp, -2, SEEK_CUR) == 0)
	{
		ch = fgetc(ifp);
		fputc(ch, ofp);
	}
	/*Flushes the output stream to ensure completion, and closes both file streams*/
	fflush(ofp);
	fclose(ifp);
	fclose(ofp);
}


extern msString msSetString (char *input)
{	
	msString ptr;
	/*Attempts to allocate memory for the msString, calls an error if this is not possible*/
	if(!(ptr = malloc(sizeof(long int) + strlen(input) * sizeof(char))))
	{
		msError("Out of memory; failed to allocate new msString variable");
	}
	/*Writes the string length at the assigned pointer location*/
	*(long int*)ptr = (long int)strlen(input);
	/*Copies the string value, sans null terminator, at the following memory addresses*/
	memcpy(ptr+sizeof(long int),input,strlen(input));
	/*Returns the pointer demarcating the msString*/
	return ptr;
}

extern char *msGetString (msString input)
{
	
	long int len = msLength(input);
	char *output;
	/*Attempts to allocate memory for the string representation of an msString, throws an error if that's not possible
 * 	NOTE: this means that msGetString cannot be directly accessed, instead needing to be stored in variables that must later be freed*/
	if (!(output = (char*)malloc(len+1)))
	{
		msError("Out of memory; failed to extract string from msString");
	}
	/*Copies data from msString memory located AFTER the stored length, as many bytes as the length suggests*/
	memcpy(output, (char *)input + sizeof(long int), len);
	/*Adds a null terminator, for compliance to ANSI C standards*/
	strcat(output, "\0");
	return output;
}

extern void msCopy (msString *dest,msString src)
{
	/*Attempts to allocate the amount of memory taken by the source msString, throws an error if that's not possible*/
	if(!(*dest = (char*)malloc(sizeof(long int) + msLength(src) * sizeof(char))))
	{
		msError("Out of memory; failed to copy an msString variable");
	}
	/*Copies the memory bytw by byte according to the length of the source msString*/
	memcpy(*dest,src,msLength(src) + sizeof(long int));
}

extern void msConcatenate (msString *dest,msString src)
{
	/*Creates variables for the length of the source and destination msStrings, and another for the total of the two*/
	long int srcLen = msLength(src);
	long int destLen = msLength(*dest);
	long int newLen = destLen + srcLen;
	/*Attempts to reallocate the memory of the destination msString to be the size of the total length of the concatenated msStrings,
 * 	throws an error if that's not possible*/
	if(!(*dest = realloc(*dest,sizeof(long int) + newLen)))
	{
		msError("Out of memory; failed to reallocate memory for msString concatenation");
	}
	/*Creates a variable to store the string value of the source string*/
	char* srcVal = msGetString(src);
	/*Copies the total length to the new concatenated msString*/
	memcpy(*dest,&newLen,sizeof(long int));
	/*Skips over the bytes containing the original string in the destination msString, and then adds the string from the source string to te subsequent bbytes*/
	memcpy(*dest + sizeof(long int) + destLen,srcVal,srcLen);
	free(srcVal);
}

extern long int msLength (msString input)
{
	return *(long int*)input;
}

extern int msCompare (msString subj1,msString subj2)
{	
	/*Returns 1 (representing false) if the lengths of the msStrings are not the same*/
	if(msLength(subj1) != msLength(subj2))
	{
		return 1;
	}
	/*Creates variables to store the string values of the 2 msStrings to be compared*/
	char *subj1Val = msGetString(subj1);
	char *subj2Val = msGetString(subj2);
	/*Compares the two strings stored by the msStrings, returns a 1 (representing false) if they don't match*/
	if(strcmp(subj1Val,subj2Val) != 0)
	{
		free(subj1Val);
		free(subj2Val);
		return 1;
	}
	/*Returns a 0 (representing true) if neither of the previous condidtions were met, since this means the msStrings are identical*/
	free(subj1Val);
	free(subj2Val);
	return 0; 
}

extern int msCompareString (msString subj1, char *subj2)
{
	/*Returns 1 (representing false) if the length of the string and the msStrings are not the same*/
	if(msLength(subj1) != strlen(subj2))
	{
		return 1;
	}
	/*Creates variables to store the string value of the msString to be compared*/
	char *subj1Val = msGetString(subj1);
	/*Compares the string and the string stored in the msString, returns a 1 (representing false) if they don't match*/
	if(strcmp(msGetString(subj1),subj2) !=0)
	{
		free(subj1);
		return 1;
	}
	/*Returns a 0 (representing true) if neither of the previous conditions were met, since this means the string and the string stored in the msString are identical*/
	free(subj1);
	return 0;
}

static void msError (char *errMsg)
{
	fprintf(stderr, "%s",errMsg);
	exit(-1);
}
