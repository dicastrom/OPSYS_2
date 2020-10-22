#include <unistd.h>
int main(){

		access("/usr",F_OK);
		//checks for the existance of the file "usr" 7 times, will be 7 syscalls more than the empty.c file

		access("/usr",F_OK);
		access("/usr",F_OK);
		access("/usr",F_OK);
		access("/usr",F_OK);
		access("/usr",F_OK);
		access("/usr",F_OK);

	return 0;
}
