#ifndef SHELL_H
#define SHELL_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>

using namespace std;

namespace FurkanOS {

class File{
    public :
        File() {}

        virtual void ls(string x) =0 ; // Lists the contents of the current directory -- R is for recursive listing 
        
        virtual void mkdir(string directory_name) = 0 ; // Makes a directory

        virtual void rmdir(string directory_name)=0; // Removes a directory
        
        virtual void rm(string file_name) = 0; // Deletes file in the current directory
        
        virtual void cp(string source,string destination) = 0; // Copies the source file to the destination file 
        
        virtual void link(string source,string destination) = 0; // Makes a soft link between the source file and the destination file
        
        virtual void cd(string path_new_directory) = 0; // Changes the current directory 
        
        virtual void cat(string file_name) = 0; // Types the file contents on the screen
        
        virtual ~File(){}
};


class SoftLinkedFile : public File {
    public :
        SoftLinkedFile(string sfl_name) : name(sfl_name) {
            CreationTime();
            getCreationTime();
        } 

        // SoftLinkedFile(const SoftLinkedFile& other); // copy constructor

        void ls(string x){

        } // Lists the contents of the current directory -- R is for recursive listing 

        void mkdir(string directory_name){
            
        }// Makes or removes a directory

        void rmdir(string directory_name){
            
        }// Removes a directory   

        void rm(string file_name){ // Deletes file in the current directory
        
        }
        void cp(string source,string destination){ // Copies the source file to the destination file 
        
        }

        void link(string source,string destination){
           
        }// Makes a soft link between the source file and the destination file

        void cd(string path_new_directory){
            
        } // Changes the current directory 

        void cat(string file_name){
            
        } // Types the file contents on the screen

        string get_name();

        void CreationTime();

        string getCreationTime(); 

        ~SoftLinkedFile() {}

    private:
        string name;
        string creation_time;
        string bytes;
        char type;    
}; 

class RegularFile : public File {
    public :
        RegularFile() {
            CreationTime();
            getCreationTime();
        };

        void ls(string x){

        }// Lists the contents of the current directory -- R is for recursive listing 

        void mkdir(string directory_name){

        } // Makes or removes a directory

        void rmdir(string directory_name){

        } // Removes a directory   

        void rm(string file_name);// Deletes file in the current directory

        void cp(string source,string destination); // Copies the source file to the destination file 

        void link(string source,string destination){

        } // Makes a soft link between the source file and the destination file

        void cd(string path_new_directory){

        } // Changes the current directory 

        void cat(string file_name);// Types the file contents on the screen

        string get_name() const ;

        void CreationTime() ;

        string getCreationTime() const; 

        void set_type(char type);

        void set_name(string new_name);

        void set_time(string ntime);

        void set_byte(string byte);

        void set_content(string cont);

        string get_byte() const ;

        string get_content() const;

        char get_type() const ;

        ~RegularFile() {}

    private:
        string name;
        string creation_time;
        string bytes;
        string content;
        char type;

}; 

int chk=0;

class Directory : public File {
    public :
        Directory(string dname="") : name(dname) {
            curr=0;
            CreationTime();
            getCreationTime();
        }

        void ls_recursive(int root);

        void ls(string x);// Lists the contents of the current directory -- R is for recursive listing 

        void mkdir(string directory_name); // Makes or removes a directory

        void rmdir(string directory_name); // Removes a directory   

        void rm(string file_name); // Deletes file in the current directory

        void cp(string source,string destination); // Copies the source file to the destination file 

        void link(string source,string destination);  // Makes a soft link between the source file and the destination file

        void cd(string directory_name); // Changes the current directory 

        void cat(string file_name); // Types the file contents on the screen

        string get_name();

        void CreationTime();

        string getCreationTime(); 

        void OS_disk();

        void write_disk();

        vector<File*> objects() const { return obj ;}
        
        ~Directory(){
            for(int i=0;i<obj.size();++i) 
                delete obj[i] ;
        }

    private:
        int curr;
        string name;
        string creation_time;
        vector <File*> obj;
        char type='D';

};

}


#endif 
