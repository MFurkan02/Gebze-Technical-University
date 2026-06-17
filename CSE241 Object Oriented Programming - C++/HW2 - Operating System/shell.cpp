#include "shell.h"

namespace FurkanOS{


string SoftLinkedFile::get_name(){
        return name;
}

void SoftLinkedFile::CreationTime() {
            // Get current time
            time_t currentTime = time(nullptr);

            // Convert the current time to the local time
            struct tm* timeInfo = localtime(&currentTime);

            char buffer[20];  // Buffer to store the formatted time
            strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeInfo);

            creation_time=string(buffer);
}

string SoftLinkedFile::getCreationTime(){
    return creation_time;
} 

void RegularFile::rm(string file_name){
            Directory temp;
            try{
                int found = 0;
                for (auto it = temp.objects().begin(); it != temp.objects().end(); ++it) {
                    if (dynamic_cast<RegularFile*>(*it) && dynamic_cast<RegularFile*>(*it)->get_name() == file_name) {
                        cout << "File is found!\n";
                        found = 1;
                        delete *it;  // deleting the object before erasing
                        temp.objects().erase(it);
                        break;  // exit the loop once the element is found and erased
                    }
                }

            if (!found) {
                throw invalid_argument("File to be deleted not found! Please type the file name again if it exists for buffer errors!\n");
            }
        }
        catch(exception& e){
                cerr << "Exception caught! ->" << e.what() << endl; 
        }
} // Deletes file in the current directory

void RegularFile::cp(string source,string destination){
            Directory temp;
            size_t slash = source.find_last_of('/');
            string src_file= source.substr(slash+1);
            cout << src_file <<endl;
            int src,dst;
            try{
                int found = 0;
                for (auto it = temp.objects().begin(); it != temp.objects().end(); ++it) {
                    if (dynamic_cast<RegularFile*>(*it) && dynamic_cast<RegularFile*>(*it)->get_name() == src_file) {
                        cout << "found!\n";
                        found = 1;
                        auto pos=temp.objects().begin(); // creating an iterator 
                        advance(pos,dst); // and advancing it to the position to insert which is current directory.
                        temp.objects().insert(pos,*it); // inserting(copying) the source file to the destination which is '.'(current).
                        break;  // exit the loop once the element is found and erased
                    }
                }

                if (!found) {
                throw invalid_argument("Directory or file to be copied not found!\nSource input must be in the form of /etc/filename starting with /etc/ and destination is always the current directory '.'\n")  ;
                }
            }
            catch(exception& e){
                cerr << "Exception caught! ->" << e.what() << endl; 
            }
        
            
        } // Copies the source file to the destination file 

void RegularFile::cat(string file_name){
            Directory temp;
            for(const auto& elem : temp.objects()){ // iterating through
                if(dynamic_cast<RegularFile*>(elem)){  // if it is a file
                    if(dynamic_cast<RegularFile*>(elem)->get_name()==file_name) // and it is the file we are looking for
                        cout << dynamic_cast<RegularFile*>(elem)->get_content() << endl ; // type the file contents on the screen.
                }
            }
        } // Types the file contents on the screen


void RegularFile::set_name(string new_name){

        name=new_name;
}

void RegularFile::set_type(char ntype){
        type=ntype;
}
        
void RegularFile::set_time(string ntime){
        creation_time=ntime;
}

void RegularFile::set_byte(string byte){
    bytes=byte;
}

void RegularFile::set_content(string cont){
    content=cont;
}

string RegularFile::get_byte() const{
    return bytes;
}

string RegularFile::get_content() const{
    return content;
}

char RegularFile::get_type() const {
    return type;
}

string RegularFile::get_name() const {
    return name;
}

void RegularFile::CreationTime() {
            // Get current time
            time_t currentTime = time(nullptr);

            // Convert the current time to the local time
            struct tm* timeInfo = localtime(&currentTime);

            char buffer[20];  // Buffer to store the formatted time
            strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeInfo);

            creation_time=string(buffer);
}

string RegularFile::getCreationTime() const {
    return creation_time;
}



void Directory::ls_recursive(int root){ 
        if(dynamic_cast<Directory*>(obj[curr]) && root==0){  // to check if we are in the main directory
            chk++; // to check and print the root directory.
            cout << dynamic_cast<Directory*>(obj[curr])->type  << " " <<  dynamic_cast<Directory*>(obj[curr])->get_name() << "\t" << dynamic_cast<Directory*>(obj[curr])->getCreationTime() << endl; // printing the other files with their types,names and creation times. 
        }else if( chk<2  && dynamic_cast<RegularFile*>(obj[curr]) ){ // to print only the files in the current directory.
            cout << dynamic_cast<RegularFile*>(obj[curr])->get_type() << " " << dynamic_cast<RegularFile*>(obj[curr])->get_name() << "\t" << dynamic_cast<RegularFile*>(obj[curr])->getCreationTime() << "\t" << dynamic_cast<RegularFile*>(obj[curr])->get_byte() << endl;
        }
        curr++;
        if(curr==obj.size()){
            return;
        }
        ls_recursive(root);
}

void Directory::ls(string x){
            int check=0;
            // first printing the current directory like D . time.
            cout << type << " .\t" << dynamic_cast<Directory*>(obj[curr])->getCreationTime() << endl; // first printing our current directory with its type and creation time.
            int root=curr; // to get the first curr index to check if it is the root directory.
            if(curr>0){
                for(int i=curr-1;i>=0;i--){
                    if(dynamic_cast<Directory*>(obj[i])){
                        // printing the previous directory like D .. time if there is one.
                        cout << type << " ..\t" << dynamic_cast<Directory*>(obj[i])->getCreationTime() << endl; //  printing '..' for the previous directory with its type and creation time if there is a previous directory.
                        break;
                    }
                }
            }
            if(x=="-R"){ // if it is recursive 
                chk=0;
                ls_recursive(root);
                curr=root;
                return;
            }     
            for(int i=curr ; i< obj.size() ; ++i){ // printing the all directory and files in the current directory. 
                    if(dynamic_cast<Directory*>(obj[i]) && !curr){
                        check++; // to check and print the root directory.
                        cout << dynamic_cast<Directory*>(obj[i])->type  << " " <<  dynamic_cast<Directory*>(obj[i])->get_name() << "\t" << dynamic_cast<Directory*>(obj[i])->getCreationTime() << endl; // printing the other files with their types,names and creation times. 
                    }else if(check<2 && dynamic_cast<RegularFile*>(obj[i]) ){ // to print only the files in the current directory.
                       cout << dynamic_cast<RegularFile*>(obj[i])->get_type() << " " << dynamic_cast<RegularFile*>(obj[i])->get_name() << "\t" << dynamic_cast<RegularFile*>(obj[i])->getCreationTime() << "\t" << dynamic_cast<RegularFile*>(obj[i])->get_byte() << endl;
                    }
            }
            

}   // Lists the contents of the current directory -- R is for recursive listing 

void Directory::mkdir(string directory_name){ // insert if not in main directory
            Directory* ndir = new Directory(directory_name) ;
            if(curr==0){ // if root directory add directory to end. 
                obj.push_back(dynamic_cast<File*>(ndir)); // and advancing it to the position to insert which is current directory.
            }
            else{ // if not add inside the curr directory
                obj.insert(obj.begin()+curr+1,dynamic_cast<File*>(ndir)); // inserting(copying) the source file to the destination which is '.'(current).
            }
} // Makes or removes a directory

void Directory::rmdir(string directory_name){
            try{
                int found = 0;
                for (auto it = obj.begin(); it != obj.end(); ++it) {
                    if (dynamic_cast<Directory*>(*it) && dynamic_cast<Directory*>(*it)->get_name() == directory_name) {
                        found = 1;
                        delete *it;  // Don't forget to delete the object before erasing
                        obj.erase(it);
                        break;  // exit the loop once the element is found and erased
                    }
                }

        if (!found) {
            throw invalid_argument("Directory to be deleted not found! Please type the directory name again if it exists for buffer errors!\n");
        }
            }
            catch(exception& e){
                cerr << "Exception caught! ->" << e.what() << endl; 
            }
}  // Removes a directory   

void Directory::rm(string file_name){
             try{
                int found = 0;
                for (auto it = obj.begin(); it != obj.end(); ++it) { // iterating through the files.
                    if (dynamic_cast<RegularFile*>(*it) && dynamic_cast<RegularFile*>(*it)->get_name() == file_name) { // if it is a file and name of it is our file.
                        found = 1;
                        delete *it;  // deleting the object before erasing
                        obj.erase(it);
                        break;  // exit the loop once the element is found and erased
                    }
                }

        if (!found) {
            throw invalid_argument("File to be deleted not found! Please type the file name again if it exists for buffer errors!\n");
        }
            }
            catch(exception& e){
                cerr << "Exception caught! ->" << e.what() << endl; 
            }
} // Deletes file in the current directory

void Directory::cp(string source,string destination){
            size_t slash = source.find_last_of('/');
            string src_file= source.substr(slash+1);
            int found = 0;
            int index=-1;
            try{
                for (auto it = obj.begin(); it != obj.end(); ++it) { // iterating through the directories.
                    index++;
                    if (dynamic_cast<RegularFile*>(*it) && dynamic_cast<RegularFile*>(*it)->get_name() == src_file) {
                        found = 1;
                        obj.insert(obj.begin()+curr+1,*it); // inserting(copying) the source file to the destination which is '.'(current).
                        break;  // exit the loop once the element is found and erased
                    }
                    else if (dynamic_cast<Directory*>(*it) && dynamic_cast<Directory*>(*it)->get_name() == src_file) {
                        found = 1;
                        //auto pos=obj.begin(); // creating an iterator 
                        //advance(pos,curr); // and advancing it to the position to insert which is current directory.
                        obj.insert(obj.begin()+curr+1,*it); // inserting(copying) the source file to the destination which is '.'(current).
                        break;  // exit the loop once the element is found and erased
                    }
                }
                if (!found) {
                  throw invalid_argument("Directory or file to be copied not found!\nSource input must be in the form of /etc/filename starting with /etc/ and destination is always the current directory '.'\n")  ;
                }
            }
            catch(exception& e){
                cerr << "Exception caught! ->" << e.what() << endl; 
            }
} // Copies the source file to the destination file 

void Directory::link(string source,string destination){
            size_t slash = source.find_last_of('/'); // to get the file name out of the slash addresses.
            string src_file= source.substr(slash+1);

            size_t slash2 = destination.find_last_of('/');
            string dest_file= destination.substr(slash2+1); // to get the file name out of the slash addresses.
            int src=-1;
            int dst=-1;

            int msrc,mdst; // to get the index of the src and dest files.
            try{
                int found = 0;
                for (auto it = obj.begin(); it != obj.end(); ++it) {
                    src++;
                    dst++;
                    if (dynamic_cast<RegularFile*>(*it) && dynamic_cast<RegularFile*>(*it)->get_name() == src_file) {
                        found = 1;
                        msrc=src; // getting the source index of the file in the system.
                        
                    }
                    else if (dynamic_cast<RegularFile*>(*it) && dynamic_cast<RegularFile*>(*it)->get_name() == dest_file) {
                        found = 1;
                        mdst=dst; // getting the destination index of the file in the system.
                    }
                }
                dynamic_cast<RegularFile*>(obj[mdst])->set_byte(dynamic_cast<RegularFile*>(obj[msrc])->get_byte());
                dynamic_cast<RegularFile*>(obj[mdst])->set_content(dynamic_cast<RegularFile*>(obj[msrc])->get_content());
                shared_ptr<SoftLinkedFile> dest_file = make_shared<SoftLinkedFile>(src_file);

                if (!found) {
                throw invalid_argument("Directory or file to be copied not found!\nSource input must be in the form of /etc/filename starting with /etc/ and destination is always the current directory '.'\n")  ;
                }
            }
            catch(exception& e){
                cerr << "Exception caught! ->" << e.what() << endl; 
            }

}  // Makes a soft link between the source file and the destination file

void Directory::cd(string directory_name){
             try{
                int found=0; 
                if(directory_name==".." && curr>0){ // if '..' is selected will be gone to the prev directory. 
                    for(int i=curr-1;i>=0;i--){ // finding the prev directory by going back
                        if(dynamic_cast<Directory*>(obj[i]))
                            curr=i;
                    }
                }
                else{ // if it is not '..' and directory name is entered.
                    for(int i=0 ; i<obj.size() ; ++i ){
                        if(dynamic_cast<Directory*>(obj[i])){
                            if(dynamic_cast<Directory*>(obj[i])->get_name()==directory_name){
                                curr=i; // changing to current index to the index of the found directory.
                                found=1; // found for error catch
                                break;
                            }
                        }
                    }
                    if(!found){ // if not found throw.
                        throw invalid_argument("The directory you are trying to reach does not exist! Please type the directory name again if it exists for buffer errors!\n");
                    }
                }
            }
            catch(exception& e){
                cerr << "Exception caught! -> " << e.what() << endl; 
            }
}  // Changes the current directory 

void Directory::cat(string file_name){
    try{
    int found=0;
            for(const auto& elem : obj){ // iterating through
                if(dynamic_cast<RegularFile*>(elem)){  // if it is a file
                    if(dynamic_cast<RegularFile*>(elem)->get_name()==file_name){ // and it is the file we are looking for
                        found=1;
                        cout << dynamic_cast<RegularFile*>(elem)->get_content() << endl ; // type the file contents on the screen.
                        break;
                    }
                }    
            }
            if(!found)
                throw invalid_argument("File not found!\n");
    }
    catch(exception& e){
                cerr << "Exception caught! -> " << e.what() << endl; 
            }
} // Types the file contents on the screen

string Directory::get_name(){
        return name;
}

void Directory::CreationTime() {
            // Get current time
            time_t currentTime = time(nullptr);

            // Convert the current time to the local time
            struct tm* timeInfo = localtime(&currentTime);

            char buffer[20];  // Buffer to store the formatted time
            strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeInfo);

            creation_time=string(buffer);
}

string Directory::getCreationTime(){
    return creation_time;
}




void OS_Shell() {
    //Directory main("main");
    Directory * main = new Directory;
    main->OS_disk();
    string input;
    cout << "Enter myShell to start the simple operating system!" << endl;
    cin >> input;
    int err=0;
    if (input == "myShell") { // if input is 'myShell' OS will begin.
        cout << "Welcome to the simple operating system! Enter 'exit' to exit from the operating system!" << endl;
        cout << "Available commands are ls, mkdir,rmdir,rm, cp, link, cd, and exit." << endl;
        while(1){// till exit is entered.
            if(err!=0)
                cout << "> ";
            getline(cin, input);  // getting input line from user.
            istringstream iss(input); // extracting the input to words to get the name from the command ex: mkdir 'name'.
            string command;
            iss >> command;
            
            if(command=="exit"){
                break;
            }

            if (command == "ls") {
                string name; // extracting the 2nd word from input to check if it is recursive listing(-R).
                iss >> name; 
                if(name=="-R"){
                    cout << "recursive listing" << endl;
                    //cout << type << " .\t" << dynamic_cast<Directory*>(obj[curr])->getCreationTime() << endl; // first printing our current directory with its type and creation time.
                    main->ls(name);
                }
                else if(name.empty()){
                    main->ls(name);
                }
                else{
                    cout << "Enter ls -R for recursive, only ls for normal listing!" << endl;
                }
            } else if (command == "mkdir") {
                string name;
                iss >> name;
                main->mkdir(name);
            } else if (command == "rmdir") {
                string name;
                iss >> name;
                main->rmdir(name);
            } else if (command == "cd") {
                string name;
                iss >> name;
                main->cd(name);
            } else if (command == "rm") {
                string name;
                iss >> name;
                main->rm(name);
            } else if (command == "cp") {
                string source;
                iss >> source;
                string dest;
                iss >> dest;
                main->cp(source,dest);
            } else if (command == "link") {
                string source;
                iss >> source;
                string dest;
                iss >> dest;
                main->link(source,dest);
            } else if (command == "cat") {
                string name;
                iss >> name;
                main->cat(name);}
                else if(command=="\n" || command==" "){}
            else {
                if(err!=0)
                    cout << "Please enter a valid command!" << endl;
            }
            err++;
        } 
    }
    else{
        cout << "Invalid input! Program terminated please input 'myShell' for program to start" << endl;
    }
    main->write_disk();
}

void Directory::OS_disk() {
    try {
        ifstream disk("FurkanOS.txt");
        if (!disk.is_open()) {
            throw runtime_error("Error! File does not exist!\n");
        }
        int ind=0;
        string line;
        while (getline(disk, line)) {
            
            istringstream iss(line);
            char type;
            iss >> type;
            if(type=='D'){
                cout <<  "";
                Directory* temp = new Directory();
                temp->type=type;
                iss >> temp->name;
                getline(iss, temp->creation_time);
                obj.push_back(dynamic_cast<File*>(temp));
            }
            else if(type=='F'){
                cout <<  "";
                RegularFile* fl= new RegularFile();
                fl->set_type(type); 
                string nname;
                iss >> nname;
                fl->set_name(nname);
                string month,day,hour;
                iss >> month >> day >> hour;
                string time=month + " " + day + " " +  hour;
                fl->set_time(time);
                string bytes,content;
                iss >> bytes;
                getline(iss,content);
                fl->set_byte(bytes);
                fl->set_content(content);
                obj.push_back(dynamic_cast<File*>(fl));       
            }
        }
        disk.close();
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
    
    // to print all files and directories from the disk.
    /*for (const auto entry : obj) {
        
        if(dynamic_cast<Directory*>(entry)){
          cout <<  dynamic_cast<Directory*>(entry)->type << dynamic_cast<Directory*>(entry)->name << dynamic_cast<Directory*>(entry)->creation_time << endl;
        }else if(dynamic_cast<RegularFile*>(entry)){
          cout <<  dynamic_cast<RegularFile*>(entry)->get_type() << dynamic_cast<RegularFile*>(entry)->get_name() << dynamic_cast<RegularFile*>(entry)->getCreationTime() << dynamic_cast<RegularFile*>(entry)->get_byte() <<dynamic_cast<RegularFile*>(entry)->get_content() << endl;
        }
    }*/
}

void Directory::write_disk() { // to save the directory and files of my OS.
    try {
        ofstream disk("FurkanOS.txt");
        if (!disk.is_open()) {
            throw runtime_error("Error! File does not exist!\n");
        }
        for(int i=0;i<obj.size();++i){ // writing the types,names and creation times of the files.
            auto elem=obj[i];
                if(dynamic_cast<RegularFile*>(obj[i])){
                    disk << dynamic_cast<RegularFile*>(obj[i])->get_type() << " " << dynamic_cast<RegularFile*>(obj[i])->get_name() << "\t" << dynamic_cast<RegularFile*>(obj[i])->getCreationTime() << "\t" << dynamic_cast<RegularFile*>(obj[i])->get_byte() << dynamic_cast<RegularFile*>(obj[i])->get_content() << endl;
                }
                if(dynamic_cast<Directory*>(obj[i])){
                    disk << dynamic_cast<Directory*>(obj[i])->type << " " <<  dynamic_cast<Directory*>(obj[i])->get_name() << "\t" << dynamic_cast<Directory*>(obj[i])->getCreationTime() << endl;
                }
        }
        disk.close();
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
}


}

int main() {
    FurkanOS::OS_Shell();
    return 0;
}
