#include<iostream>
#include<string>
#include<cstring>
#include<stdexcept>
#include<sstream>
#include"contact.hpp"
//#include"sqlite3.h"

using namespace std;
//using namespace Contact;

class Contender{

private:
    bool fileIsOpen = false;
    bool fileHasChanged = false;

public:
    int openFile(char* Filename) {

        fileIsOpen = true;
        fileHasChanged = false;
        return 0;
    };

    int closeFile() {
        if (fileHasChanged)
        {
            cout <<  "There are unsaved changes. Exit anyways? Y/N\n";
            while (true)
            {
                char input = getchar();
                if (input == 'Y')
                {
                    // close FILE
                    fileIsOpen = false;
                }
                else if (input == 'N')
                    break;
            }
        }
        else {
            // close FILE
            fileIsOpen = false;
        }
        return 0;
    };
    int saveFile(char* filename) {
        fileHasChanged = false;
        return 0;
    };

    int mainloop(char* filename){
        cout << "mainloop\n";
        string name = "Ander";
        string firstName = "Alex";
        
        Contact contact = Contact(name, firstName);
        cout << contact.printContact();
//         while(true){
//             
//             
//             
//         }
        return 0;
    };
};

enum CMD {ERROR = -1, HELP = 0, NEW, OPEN, SAVE, CLOSE};
enum SCMD {S_HELP = 0, S_NEW, S_OPEN};
enum ERROR_CODES {ERROR_TOO_FEW_ARGUMENTS, ERROR_FILE_NOT_FOUND,  ERROR_CANT_OPEN_FILE};


class ExceptionHandler{

public:
        int HandleError(std::invalid_argument e) {
         cout <<  "Bad argument. For help type \"-h\"\n";   
         return 1;
        }
        int HandleError(std::system_error e){
            cout << "An error occurered with errorcode " <<e.code() << " meaning " <<e.what();
            return 1;
        }
        int HandleError() {
         cout <<  "Unknown Error. Exiting...\n";   
         return 1;
        }
};

class MainloopHandler {
private:
    CMD cmd;
    
public:
    
    MainloopHandler(int cmd, int argc, char** argv){
        switch (cmd) {
         case ERROR:
             throw std::invalid_argument("Not a valid mainloop argument");
             break;
         case HELP:
             cout <<  "Another very helpful message";
             break;
         case NEW:
             
             break;
         case OPEN:
             
             break;
         case SAVE:
             
             break;
         case CLOSE:
             
             break;
        }        
    }
    
    
    int execute(){
        switch (cmd) {

         case HELP:
             cout <<  "Another very helpful message";
             break;
         case NEW:
             
             break;
         case OPEN:
             
             break;
         case SAVE:
             
             break;
         case CLOSE:
             
             break;
         default:
             throw std::invalid_argument("Unknown command");
             break;             
        }
    }
};

class StartupHandler {
private:
        SCMD cmd;
        char* filename;

public:

    StartupHandler(int argc,  char* argv[]) {
      if (argc < 2) {
          throw std::invalid_argument("Not enough arguments!");
      }
      if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
          cmd = S_HELP;
      else if (!strcmp(argv[1], "-n") || !strcmp(argv[1], "--new")) {
          cmd = S_NEW;
          if (argc < 3) {
            throw std::invalid_argument("Not enough arguments!");
          }
          filename = argv[2];
      }
      else if (!strcmp(argv[1], "-o") || !strcmp(argv[1], "--open")) {
          cmd = S_OPEN;
          if (argc < 3) {
            throw std::invalid_argument("Not enough arguments!");
          }
          filename = argv[2];
      }
      else
      {
          throw std::invalid_argument("Invalid command");
      }
    };

    int execute() {
        Contender cnt;
        switch (cmd) {
         case HELP:
            printHelp();
            return 0;
             break;
         case NEW:
             try{
                 // create file
                 if (!cnt.openFile(filename)) {
                     throw std::system_error();
                 }
             }
             catch (std::system_error& ex){
                 ExceptionHandler handler;
                 return handler.HandleError();
             }
             catch ( ... ) {
                 ExceptionHandler handler;
                 return handler.HandleError();
             }
             return cnt.mainloop(filename);
             break;
         case OPEN:
             return cnt.mainloop(filename);
             break;
        }
        return 0;
    }
    static void printHelp() {
        cout <<  "This is very helpful text!\n";
    }
};

int main(int argc, char* argv[]){
    try {
        StartupHandler cmd(argc, argv);
        return cmd.execute();
    }
    catch (const std::invalid_argument& e) {
        ExceptionHandler handler;
        return handler.HandleError(e);
    }
    catch (const std::system_error& e){
        ExceptionHandler handler;
        return handler.HandleError(e);
    }
    catch ( ... ) {
                 ExceptionHandler handler;
                 return handler.HandleError();
    }
    return 0;
}
