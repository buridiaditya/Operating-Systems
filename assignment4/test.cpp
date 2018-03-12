#include <iostream>
#include "myfs.h"

using namespace std;

int main(){
    int status;
    if((status = create_myfs(10,512)) == -1)
        cerr << "Error creating filesystem\n";
    copy_pc2myfs("myfs.h","myfs.h");
    copy_pc2myfs("test.cpp","test.cpp");
    ls_myfs();
    return 0;
}
