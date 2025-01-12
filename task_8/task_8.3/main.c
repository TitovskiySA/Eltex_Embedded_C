#include <stdio.h>
#include <dlfcn.h>

int main(){
	void* handle = dlopen("./libs/libapp.so", RTLD_LAZY);
	if (!handle){
		fprintf(stderr, "ERROR: %s\n", dlerror());
		return 1;
	}

	void (*hello_foo)(void) = dlsym(handle, "say_hello");

	char* error;
	if ((error == dlerror()) != NULL){
		fprintf(stderr, "error: %s\n", error);
		dlclose(handle);
		return 1;
	}

	hello_foo();
	dlclose(handle);
	return 0;
}
