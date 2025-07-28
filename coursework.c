#include "ca.h"
#include "msString.h"

int main (int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Improper number of inputs. Please try again with two inputs.");
		return 1;
	}
	/*Setup for Question 1*/
	void*somePointer;
	/*Creates an array of arbitrary adjacent char values to predictably test the values stored in adjacent bytes*/
	unsigned char characters[4] = {10, 20, 30, 140};
	/*Sets a pointer to the beginning of the array */
	somePointer = &characters[0];
	printBytes(somePointer, 4);

	/*Setup for Question 2*/
	fileFlip(argv[1], argv[2]);
	printf("%s has been reversed and printed to %s.\n",argv[1],argv[2]);
	/*Setup for Question 3*/
	msString	ms	= msSetString("Hello");
	msString	ms2	= msSetString(" World!");
	msString	mscopy	= NULL;

	/*Creates a string variable to store the string stored in the msString ms so that it may later be freed to avoid memory leakage*/
	char *msVal = msGetString(ms);

	printf("String |%s| is %d characters long (%p).\n",
		msVal,msLength(ms),ms);
	
	/*Frees the string variable to avoid memory leakage*/
	free(msVal);	

	msCopy(&mscopy,ms);

	/*Creates a string variable to store the string stored by the msString mscopy so that it may later be freed to avoid memory leakage*/
	char *mscopyVal1 = msGetString(mscopy);

	printf("Copied string |%s| is %d characters long (%p).\n",
		mscopyVal1,msLength(mscopy),mscopy);

	/*Frees the string variable to avoid memory leakage*/
	free(mscopyVal1);

	printf("Compare ms with mscopy: %d\n",msCompare(ms,mscopy));
	printf("Compare ms with ms2   : %d\n",msCompare(ms,ms2));
	printf("Compare ms with Hello : %d\n",msCompareString(ms,"Hello"));
	printf("Compare ms with HelloX: %d\n",msCompareString(ms,"HelloX"));
	printf("Compare ms with Hella : %d\n",msCompareString(ms,"Hella"));;	

	msConcatenate(&mscopy,ms2);

	/*Creates a string variable to store the string stored by the updated msString mscopy so that it may later be freed to avoid memory leakage*/
	char *mscopyVal2 = msGetString(mscopy);
	
	printf("Concatenated string |%s| is %d characters long (%p).\n",
		mscopyVal2,msLength(mscopy),mscopy);
	
	/*Frees the string variable along with the msString to avoid memory leakage and allow a graceful exit*/
	free(mscopyVal2);
	free(mscopy);
	free(ms2);
	free(ms);
	return 0;
}
