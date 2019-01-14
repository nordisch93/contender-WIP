#include<iostream>
#include<string>
#include<cstring>
#include<stdexcept>

using namespace std;

class Contender{

public:
    int mainloop(){
        cout << "mainloop\n";
//         while(true){
//             
//             
//             
//         }
    };
};

enum CMD {HELP = 0, NEW, OPEN};

class Command {
private:
        CMD cmd;
        char* filename;
    
public:
    
    Command(int argc,  char* argv[]) {
      if (argc < 2) {
          throw std::invalid_argument("Not enough arguments!");
      }
      if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help"))
          cmd = HELP;
      else if (!strcmp(argv[1], "-n") || !strcmp(argv[1], "-new")) {
          cmd = NEW;
          if (argc < 3) {
            throw std::invalid_argument("Not enough arguments!");
          }
          filename = argv[2];
      }
      else if (!strcmp(argv[1], "-o") || !strcmp(argv[1], "-open")) {
          cmd = OPEN;
          if (argc < 3) {
            throw std::invalid_argument("Not enough arguments!");
          }
          filename = argv[2];
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
             createFile(filename);
             return cnt.mainloop();
             break;
         case OPEN:
             openFile(filename);
             return cnt.mainloop();
             break;
        }        
    }
    static void printHelp() {
        cout <<  "This is very helpful text!\n";        
    }
    
    void createFile(char* filename) {        
        return;
    }
    
    void openFile(char* filename) {
        return;
    }
    
};

class ExceptionHandler{

public:
        int HandleError(std::invalid_argument ex) {
         cout <<  "Bad argument. For help type \"-h\"";   
        }    
};

int main(int argc, char* argv[]){
    try {
        Command cmd(argc, argv);
        return cmd.execute();        
    }
    catch (const std::invalid_argument& ex) {
        ExceptionHandler handler;
        return handler.HandleError(ex);
      }
    return 0;
}

