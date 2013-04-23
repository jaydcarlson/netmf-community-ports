//empty

#include <TinyCLR_Interop.h>
typedef int (*USER_FUNCTION)(void*, int *, unsigned char *);

//int g;
INT32 InvokeRLP(UINT32 address, void* param0, int* param1, unsigned char* param2)
{
	//int (*Pfunc)(void*, int *, unsigned char *) = NULL;

	USER_FUNCTION Pfunc = (USER_FUNCTION)address;

	return Pfunc(param0, param1, param2);
}

//
//use in other file
// INT32 retVal = 0; 
//
//	address = Get_address(bbb);
//		
//	return InvokeRLP(adress, param0, param1, param2);
