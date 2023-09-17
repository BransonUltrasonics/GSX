/* includes */

#include <stdio.h>
extern void  PCMain();


/******************************************************************************
* \brief  - main
*			first function to be called during the execution on the program.
* \param  - argc  number of arguments ,  * argv[]	 array of arguments
*
*
* \return  - INT
*
******************************************************************************/
int main (
    int	   argc,	/* number of arguments */
    char * argv[]	/* array of arguments */
    ) {

	PCMain();
    return 0;
}
