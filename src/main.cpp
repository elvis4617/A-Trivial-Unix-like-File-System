#include "FileSystem.cpp"

int main(int argc, char *argv[]){

  if (argc == 1 ){
    fprintf(stderr,"usage: %s <FileName> \n",argv[0]);
    exit(0);
  }

  myFileSystem f ((char *)"disk0"); // create disk object
  char line[100]; //store line from textfile
  char* cmd[3]; //store each part of command
  int cmd_cnt; //count for command
  char* input; //temp variable to get each part from command
  

  char buff[1024];
  //fill dummy buffer with 1's
  for (int i = 0; i < 1024; i++){
    buff[i] = '1';
  }

  ifstream testfile (argv[1]); 
  if (testfile.is_open()){
    //get each line in textfile
    while(testfile.getline(line,100)){
      //split line into parts
      cmd_cnt = 0;
      input = strtok(line," ");
      while(input != NULL){
        cmd[cmd_cnt] = input;
        cmd_cnt++;
        input = strtok(NULL," ");
      }

      //process each part from cmd
      switch (*cmd[0]){
        case 'C':
        {
          f.create_file((char*)cmd[1],atoi(cmd[2]));
          break;
        }
        case 'D':
        {
          f.delete_file((char*)cmd[1]);
          break;
        }
        case 'L':
        {
          f.ls();
          break;
        }
        case 'R':
        {
          f.read((char*)cmd[1],atoi(cmd[2]),buff);
          break;
        }
        case 'W':
        {
          f.write((char*)cmd[1],atoi(cmd[2]),buff);
          break;
        }
        default:
        {
          printf("%s\n",cmd[0]);
          break;
        }
      }//end switch
    }
  }
  return 0;
}