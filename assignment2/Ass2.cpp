#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/wait.h>
#include <stdio.h>
#include <cstdlib>
#include <fcntl.h>

using namespace std;

int main(){
	while(1){
		vector<string> comm;
		string command,piece,type;
		pid_t x;
		int status;
		stringstream stream;
		// Prompt on terminal
		cout << "\nSelet one of the following:\nA. Run an internal command.\nB. Run an external command.\nC. Run an external command by redirecting standard input from a file.\nD. Run an external command by redirecting standard output to a file.\nE. Run an external command in background.\nF. Run several external commands in pipe mode.\nG. Quit the shell.\n";
        getline(cin, type);
        cin.clear();
        // Check for quit
        if (type.compare("G") == 0)
            break;

        cout <<"G38shell$ ";

		// Read input command from terminal
		getline(cin,command);
		stream.str(command);

        if(type.compare("B") == 0 or type.compare("E") == 0 or type.compare("A") == 0){
		    // Split the input command
            while(stream >> piece)
                comm.push_back(piece);

            char** args = new char*[comm.size()+1];
            for(int i = 0; i < comm.size(); i++){
                args[i] = new char[comm[i].length()+1];
                comm[i].copy(args[i],comm[i].length());
                args[i][comm[i].length()] = '\0';
            }
            args[comm.size()] = NULL;
            
            if(type.compare("B") == 0 or type.compare("E") == 0){
                x = fork();
                if(x == 0){
                    execvp(args[0],args);
                    _exit(0);
                }
            }
            else{
                if(comm[0].compare("chdir") == 0 or comm[0].compare("cd") == 0){
                    chdir(args[1]);
                }
                else if(comm[0].compare("mkdir") == 0 ){
                    //mkdir(args[1]);
                }
            }
        }
        else if(type.compare("D") == 0){
                int count,i,file_desc,num = 0;
                string filename,actual_command;            
                string temp = stream.str();
                stringstream actual_stream;
                for(i = 0; i < temp.length() - 1; i++){
                	if(temp[i] == '>' && temp[i+1] != '>'){
                		filename = temp.substr(i+1 , temp.length() - i - 1);
                		num = 1;
                		break;
                	}
                	else if(temp[i] == '>' && temp[i+1] == '>'){
                		filename = temp.substr(i+2 , temp.length() - i - 2);
                		num = 2;
                		break;
                	}
                }
                //parsing and trimming spaces
                actual_command = temp.substr(0,i);
                stream.str(filename);
                stream >> filename;
                actual_stream.str(actual_command);
	        	while(actual_stream >> piece) comm.push_back(piece);               	
				char** args = new char*[comm.size()+1];
	            for(int i = 0; i < comm.size(); i++){
	                args[i] = new char[comm[i].length()+1];
	                comm[i].copy(args[i],comm[i].length());
	                args[i][comm[i].length()] = '\0';
	            }
	            args[comm.size()] = NULL;      
	            //child process to execute command
				x = fork();
                if(x == 0){
                    close(1); 
					if (num == 1)file_desc = open(filename.c_str(), O_WRONLY |  O_CREAT , S_IRWXU);
					else file_desc = open(filename.c_str(), O_WRONLY | O_APPEND |  O_CREAT , S_IRWXU);
					dup(file_desc);
					execvp(args[0],args);
					_exit(0);
                }				 
        }

        else if(type.compare("C") == 0){
                int count;
                string filename,actual_command;
                string temp = stream.str();
                stringstream actual_stream;
                int i = temp.find('<');
				filename = temp.substr(i+1 , temp.length() - i - 1);
     			actual_command = temp.substr(0,i);
                stream.str(filename);
                stream >> filename;
                actual_stream.str(actual_command);
	        	while(actual_stream >> piece) comm.push_back(piece);               	
				char** args = new char*[comm.size()+1];
	            for(int i = 0; i < comm.size(); i++){
	                args[i] = new char[comm[i].length()+1];
	                comm[i].copy(args[i],comm[i].length());
	                args[i][comm[i].length()] = '\0';
	            }
	            args[comm.size()] = NULL;    

				x = fork();
                if(x == 0){
                    close(0); 
					int file_desc;
					file_desc = open(filename.c_str(), O_RDONLY , S_IRWXU);
					dup(file_desc);
					execvp(args[0],args);
					_exit(0);
                }				 
        }

       /* else if(type.compare("F") == 0)
        {
        	string temp = stream.str();
        	vector <string> command;
        	int first = 0;

            for(int i = 0; i < temp.length() - 1; i++)
            {
            	if(temp[i] == '|')
            	{
            		command.push_back(temp.substr( first , i - first ));
            		first = i + 1;
            	}
            }

            for(int j= 0; j < command.size(); j++)
            {
            	stringstream stream;
            	while(stream.str(command[j]) >> piece)
                comm.push_back(piece);

	            char** args = new char*[comm.size()+1];
	            for(int i = 0; i < comm.size(); i++)
	            {
	                args[i] = new char[comm[i].length()+1];
	                comm[i].copy(args[i],comm[i].length());
	                args[i][comm[i].length()] = '\0';
	            }
	            args[comm.size()] = NULL;

	            X = fork();

	            if(X == 0)
	            {
	            	
	            }

	            else
	            {
	            	continue;
	            }
            }

        }*/

        if (type.compare("B") == 0 or type.compare("A") == 0 or type.compare("D") == 0 or type.compare("C") == 0 ){
            waitpid(-1,&status,0);
        }
	}
	return 0;
}
