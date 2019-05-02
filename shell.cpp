/*
 *  ==========================================
 *  Title:  myShellProject
 *  Author: Patryk Wieczorek
 * ==========================================
 */
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<cstdio>
#include<algorithm>
#include<unistd.h>
using namespace std;

int IDnum = 0;
class Directory;
int temporaryIntegerHolder=-1;//This is my fix for problem with file handling.
string command, attribute, name, userName="";

class SystemCompound{
protected:
   string name;
   int ID;
   int type;
   int size;
   string login="";
   string password="";
public:
    void setName(string name){
        this->name = name;
    }
    string getName(){
        return name;
    }
    bool operator<(SystemCompound sc){
        int i=0;
        while(sc.name[i]!='\0' && this->name[i]!='\0'){
            if(name[i]<sc.name[i])
                return true;
            else if(name[i]>sc.name[i])
                return false;
            i++;
        }
        return false;
    }
    void setPassword(){
        if( password == ""){
            cout << "Input password: ";
            getline(cin, password);
        }
        else{
            cout << "You have already set password. Would you like to change it? (y/n)"<<endl;
            while(true){
                    char answer;
                    answer = fgetc(stdin);
                    cin.ignore(256,'\n');
                    if(answer == 'n'){
                        break;
                    }
                    else if(answer =='y'){
                        cout << "Input password: ";
                        getline(cin,password);
                        break;
                    }
                    else
                        cout << "Write y or n"<<endl;
                }
        }
    }
    bool checkPassword(){
        if( password != "" ){
            string inputPass;
            cout << "password: "<<endl;
            getline(cin,inputPass);
            if( inputPass == password)
                return true;
            else{
                cout << "wrong password!"<<endl;
                return false;
            }
        }
        else return true;
    }
    void resetPassword(){
        password="";
    }
};
class File: public SystemCompound{
friend class Directory;
    string text;
public:
    File(){
        type = 0;
        ID = ++IDnum;
    };
};
class Directory: public SystemCompound{
    Directory* parent;
    vector<File> files;
    vector<Directory> directories;
public:
    Directory(string name){
        this->name = name;
        IDnum = ++IDnum;
        parent = NULL;
        type = 1;
    }
    File getFile(int i){
        return files[i];
    }
    void writeText(int i){
        getline(cin,files[i].text);
    }
    string readText(int i){
        return files[i].text;
    }
    void addDir(Directory newDir){
        directories.push_back(newDir);
    }
    void addFile(File newFile){
        files.push_back(newFile);
    }
    void printDirectories(int flag){
        vector<Directory> direcTemp(directories);
        if( attribute != "-a" )
			cout << ". .. ";
        if(flag == 1){
            sort(direcTemp.begin(),direcTemp.begin()+direcTemp.size());
        }
        if(flag == 1);
        for(int i=0;i<directories.size();i++){
            if(flag == 1)cout << direcTemp[i].name<<" ";
            else cout << directories[i].name<<" ";
        }
    }
    void printFiles(int flag){
        vector<File> filesTemp(files);
        if(flag == 1){
            sort(filesTemp.begin(),filesTemp.begin()+filesTemp.size());
        }
        for(int i=0;i<files.size();i++){
            if(flag == 1)cout << filesTemp[i].name<<" ";
            else cout << files[i].name<<" ";
        }
    }
    bool searchForDir(string name){
        for(int i=0;i<directories.size();i++)
            if(directories[i].name == name){
                temporaryIntegerHolder=i;
                return true;
            }
        return false;
    }
    bool searchForFile(string nameSearch){
        for(int i=0;i<files.size();i++)
            if(files[i].name == nameSearch){
                temporaryIntegerHolder = i;
                return true;
            }
        return false;
    }
    int searchForName(string nameCompare){ //Function returns diffrent values to distinguish file from directory
        for(int i=0;i<directories.size();i++)
            if(directories[i].name == nameCompare){
                return 1;
            }
        for(int i=0;i<files.size();i++)
            if(files[i].name == nameCompare){
                return 2;
            }
        return 0;
    }
    Directory* changeDir(string name,int flag){
       if(flag == 0){
		   for(int i=0;i<directories.size();i++){
				if(directories[i].name == name){
					directories[i].parent = this;
					return &directories[i];
				}
		   }
	   }
       else if(flag == 1)
            return parent;
    }
    void rmFile(string name){
        searchForFile(name);
        files.erase(files.begin()+temporaryIntegerHolder);
    }
    void rmDir(string name){
        searchForDir(name);
        directories[temporaryIntegerHolder].files.clear();
        directories.erase( directories.begin() + temporaryIntegerHolder );
    }
    void setFilePassword(int i){
        files[i].setPassword();
    }
    void setDirPassword(int i){
        directories[i].setPassword();
    }
    void resetFilePassword(int i){
        files[i].resetPassword();
    }
    bool checkFilePassword(int i){
        if( files[i].checkPassword())
            return true;
        return false;
    }
        bool checkDirPassword(int i){
        if( directories[i].checkPassword() )
            return true;
        return false;
    }
    void printPath(Directory* subDir){
		
		if(subDir != NULL){
			printPath(subDir->parent);
			//if(subDir!=NULL)
				cout << "/" << subDir->name;
		}
		
	}
}*currentDir = new Directory("rootDir");

bool searchForName(string name){//This function has been made to give intuitive errors.
    if( currentDir->searchForName(name) == 1 ){
        cout << "There already exists a directory with given name!"<<endl;
        return true;
    }
    else if( currentDir->searchForName(name) == 2 ){
        cout << "There already exists a file with given name!"<<endl;
        return true;
    }
    return false;
}

void parseCommand(){ //In this function I get command name, it's attribute and name of folder or file.
    command = "", name = "", attribute = "";
    getline(cin,command);
    stringstream parse(command);
    parse >> command >> name >> attribute;

    if(name[0] == '-'){
        string temp = name;
        name = attribute;
        attribute = temp;
    }
}

int main(){
    while(command != "exit"){
		if(userName!="")
			cout << "["<<userName<<"]";
        cout << "<#>: ";
        parseCommand();

        if( command == "ls" ){
            if(attribute=="-namesort"){
                currentDir->printDirectories(1);
                currentDir->printFiles(1);
            }
            else{
                currentDir->printDirectories(0);
                currentDir->printFiles(0);
            }
            cout << endl;
        }

        else if( command == "cd"){
            if (!currentDir->searchForDir(name)){
                if(name == ".."){
                    if(currentDir->changeDir(name,1)==NULL){
                        cout <<"You are already in root directory, you cannot go deeper."<<endl;
                        continue;
                    }
                    currentDir = currentDir->changeDir(name,1);
                }
                else if(name == ".");
                else
                    cout << "Directory "<< name << " does not exist!"<<endl;
                continue;
            }
            //currentDir->searchForDir(name);
            if( currentDir->checkDirPassword(temporaryIntegerHolder) == false ) continue;
            currentDir = currentDir->changeDir(name,0);
        }

        else if(command == "mkdir"){
            if( searchForName(name) ) continue;
            Directory newDir(name);
            currentDir->addDir(newDir);
        }

        else if( command == "pwd" ){
            //cout << "Current folder: "<< currentDir->getName() <<endl;
            cout << "Current folder: ";
            currentDir->printPath(currentDir);
            cout << endl;
        }

        else if( command == "touch" ){
            if( searchForName(name) ) continue;
            File newFile;
            newFile.setName(name);
            currentDir->addFile(newFile);
        }

        else if( command == "rm" ){
            if( currentDir->searchForName(name) == 1 && attribute !="-rdir")
                cout << "To remove directory add -rdir attribute."<<endl;
            else if( currentDir->searchForName(name) == 1 && attribute =="-rdir")
                currentDir->rmDir(name);
            else if( currentDir->searchForName(name) == 2 )
                currentDir->rmFile(name);
            else cout << "No such file nor directory was found with given name.";
        }

        else if( command == "writeto" ){
            currentDir->searchForFile(name);
            if(!currentDir->checkFilePassword(temporaryIntegerHolder)) continue;
            if(!currentDir->searchForFile(name)){
                cout << "There is no file named "<<name<<" to which you can write to."<<endl;
                continue;
            }
            cout << "Input text: "<<endl;
            currentDir->writeText(temporaryIntegerHolder);
        }

        else if( command == "readfile" ){
            currentDir->searchForFile(name);
            if(!currentDir->checkFilePassword(temporaryIntegerHolder)) continue;
            if(!currentDir->searchForFile(name)){
                cout << "There is no file named "<<name<<" from which you can read from."<<endl;
                continue;
            }
            cout << currentDir->readText(temporaryIntegerHolder)<<endl;
        }

        else if( command == "registerme" ){
            if(userName!=""){
                cout << "You are already registered as a user. Would you like to change your name? (y/n)"<<endl;
                string answer;
                while(true){
                    cin >> answer;
                    cin.ignore(256,'\n');
                    if(answer == 'n'){
                        break;
                    }
                    else if(answer =='y'){
                        cout << "What do you want to be called: ";
                        getline(cin,userName);
                        break;
                    }
                    else
                        cout << "Write y or n"<<endl;
                }
            }
            else{
                cout << "Write your desired name: ";
                getline(cin,userName);
            }
        }

        else if( command == "whoami" ){
            if(userName=="") cout << "You have not set your name yet! Write registerme to choose your name."<<endl;
            else
                cout << "You are "<<userName<<endl;
        }

        else if( command == "setpasswd" ){
                if( currentDir->searchForName(name) == 0){
                    cout << "No such file nor directory was found with given name.";
                    continue;
                }
                else if( currentDir->searchForName(name) == 1 ){
                    currentDir->searchForDir(name);
                    if( attribute == "-r") currentDir->resetPassword();
                    else
                        currentDir->setDirPassword(temporaryIntegerHolder);
                }
                else{
                    currentDir->searchForFile(name);
                    if( attribute == "-r") currentDir->resetFilePassword(temporaryIntegerHolder);
                    else
                        currentDir->setFilePassword(temporaryIntegerHolder);
                }
        }

        else if(command == "man"){
            if(name == "ls"){
                cout << endl;
                cout << "Command ls works similar to the one known from UNIX systems." << endl;
                cout << "The only diffrence is that it always proritizes directories over files." << endl;
                cout << "Avaible attributes are:" << endl;
                cout << "-namesort (lists everything in lexicographical order)" << endl;
                cout << endl;
            }
            else if(name == "cat"){
                cout << endl;
                cout << "Command readfile works like cat command." << endl;
                cout << "Viable syntax is: readfile filename" << endl;
                cout << "Which prints text contents of file filename" << endl;
                cout << endl;
            }
            else if(name == "writeto"){
                cout << endl;
                cout << "Command writeto is used to write text content into a file" << endl;
                cout << "Viable syntax is: writeto filename" << endl;
                cout << "Which writes text into the file filename." << endl;
                cout << endl;
            }
            else if(name == "registerme"){
                cout << endl;
                cout << "Command registerme allows you to choose your unique name." << endl;
                cout << "Your chosen name will be displayed in command prompt." << endl;
                cout << "Currently it has only cosmetical purposes." << endl;
                cout << endl;
            }
            else if(name == "whoami"){
                cout << endl;
                cout << "Command whoami allows you to see your chosen name with command registerme." << endl;
                cout << endl;
            }
            else if(name == "setpasswd"){
                cout << endl;
                cout << "Command setpassword allows you to secure created file/directory with a password"<<endl;
                cout << "Available attributes are:" << endl;
                cout << "-r (removes password from file)";
                cout << "You can only use this command after you registered yourself since your name is tied with password."<<endl;
                cout << endl;
            }
            else cout << "No entry in manual for: "<< name << endl;
        }

        else if( command == "help"){
            cout << "Available commands are: "<<endl;
            cout << "ls cd mkdir pwd rm setpasswd"<<endl;
            cout << "touch writeto cat registerme whoami"<<endl<<endl;
            cout << "There are also manual entries on following commands: "<<endl;
            cout << "writeto readfile ls registerme whoami setpassword"<<endl<<endl;
        }

        else if(command!="exit")
            cout << command <<": not known syntax! write help for available commands"<<endl;
    }
    return 0;
}
