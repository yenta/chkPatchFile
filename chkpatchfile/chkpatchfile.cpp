// chkpatchfile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string> 
#include <vector>
#include <algorithm>

using namespace std;


typedef struct item {
    string fileName;
	string language;
	int SEGTT_num;
    int p4_changelist_num;
	string submitted_by;
    int fileGroup;
    string Type;
    string destination_path;
    string source_path;
}item;
typedef struct patchFileItem {
    int fileGroup;
    string Type;
    string path;
	string fileName;
    string source_path;
	string comment;
}pFileItem;

void WriteFile(string,std::vector<struct item>);
string lowerCase(std::string);
int _tmain(int argc, _TCHAR* argv[])
{
    char txtName[]="./file/test.txt";
	fstream txt;
	string line;
	std::vector<item> itemlist;
	bool needToUpdate;

	txt.open(txtName,ios::in);

	if(txt.fail()){ //if failed to open,fp=0
		cout<<"Fail to open File:" << txtName<<endl;
		exit(1);
	}
	if(txt.is_open()){
		
		while(getline(txt,line)){ //new line
			needToUpdate=true; 
            istringstream iss(line);
			//start find "verified" 
			if(line.find("verified",0)==string::npos){
				if(line.empty()){
                  needToUpdate=false; 
				  break;	
				} 
			  //this line has no "verified" ,store the line
			  item readTmp;
			  while(iss>> readTmp.fileName 
				       >> readTmp.language 
					   >> readTmp.SEGTT_num 
					   >> readTmp.p4_changelist_num 
					   >> readTmp.submitted_by 
					   >> readTmp.fileGroup 
					   >> readTmp.Type 
					   >> readTmp.destination_path 
					   >> readTmp.source_path)
					   itemlist.push_back(readTmp);	
			           //end read line

			} 
			else{
			  // this line has "verified"
			  needToUpdate=false;
			  itemlist.clear();
			} 		
		}			
	}
    txt.close();//close file

	if(needToUpdate){

		cout << "----need to add new item----"<<endl;
		std::vector<item>::iterator it=itemlist.begin();
		for ( ; it != itemlist.end(); it++ ){
			cout<< "File:" <<it->fileName << "\t||FileGroup:"<<it->fileGroup<<endl;
			
		}
		string flist[3]={"./file/a.txt","./file/b.txt","./file/c.txt"};
		//for(int i=0;i<3;i++) WriteFile(flist[i],itemlist);
		WriteFile("./file/a.txt",itemlist);
		
	}
	else cout<<"----verified----"<<endl;

	
	system("pause");
	return 0;
}
void WriteFile(string filePath,std::vector<item> itemlist){
   ifstream rtxt;
   ofstream wtxt;
   string line;
   std::vector<pFileItem> pFileItemlist;
   rtxt.open(filePath.c_str()); //start read patchFilelist
   cout<< "-------------------"<<endl;
   cout<< "Work on File:" << filePath << "..." <<endl;
   if(rtxt.fail()){ //if failed to open,fp=0
		cout<<"Fail to open File:" << filePath.c_str() <<endl;
		exit(1);
	}
   
   if(rtxt.is_open()){
		getline(rtxt,line);//skip first line

		while(getline(rtxt,line)){ //new line
	
              if(line.empty()) break;
              istringstream iss(line);
			  pFileItem readTmp;
			  while(iss>> readTmp.fileGroup 
					   >> readTmp.Type 
					   >> readTmp.path
					   >> readTmp.fileName
					   >> readTmp.source_path
					   )
			  {
				      //cout<<"Tmp:"<<readTmp.fileGroup <<endl;
                      pFileItemlist.push_back(readTmp);
			  }
					   	
			           //end read line		
		}	
	}
    rtxt.close();//close read file
	std::vector<pFileItem>::iterator piit=pFileItemlist.begin();

    cout<<"Start matching..."<<endl;
	std::vector<item>::iterator iit=itemlist.begin();
	int group=0;
	
	for(;iit!=itemlist.end();iit++){
	  std::vector<pFileItem>::iterator piit=pFileItemlist.begin();
	  bool fileMatchGroup=false;
	  cout<<"----------------------"<<endl;
	  cout<<"Now Use:"<<iit->fileName<< "  ||FileGroup:"<<iit->fileGroup<<endl;
      for(;piit!=pFileItemlist.end();piit++){

		  
		  if((piit->fileGroup == iit->fileGroup)&&
			 (lowerCase(piit->fileName)  == lowerCase(iit->fileName))&&
			 (piit->path  == iit->destination_path)&&
			 (piit->source_path  == iit->source_path)&&
			 (piit->Type  == iit->Type)){
				 cout<<"Find SameFile:"<<piit->fileGroup<<"||"<<piit->fileName<<endl;
		         break;
		  }  //Same File 

		  if(group!=piit->fileGroup && fileMatchGroup==false && iit->fileGroup>=group){     //Next Group
		    group=piit->fileGroup;
			if(piit->fileGroup == iit->fileGroup){
			  cout<<"Find SameGroup:"<<group<<"||"<<iit->fileName<<endl;
			  fileMatchGroup=true;
			} 
		  } 
		  else if(group!=piit->fileGroup && fileMatchGroup==true && iit->fileGroup>=group){
			  cout<<"Find SameGroup and need to insert:"<<group<<"||"<<piit->fileName<<endl;
		    //insert
			  pFileItem tmp;
			  tmp.fileGroup  =iit->fileGroup;
			  tmp.fileName   =iit->fileName;
			  tmp.path       =iit->destination_path;
			  tmp.source_path=iit->source_path;
			  tmp.Type       =iit->Type;
			  tmp.comment    ="";
			  pFileItemlist.insert(piit,tmp);
			  break;
		  }
		  else if(iit->fileGroup<group||piit==(pFileItemlist.end()-1)){
			  cout<<"Need to add file:" << iit->fileName <<" to new group:"<<iit->fileGroup<<endl;
			  pFileItem tmp;
			  tmp.fileGroup  =iit->fileGroup;
			  tmp.fileName   =iit->fileName;
			  tmp.path       =iit->destination_path;
			  tmp.source_path=iit->source_path;
			  tmp.Type       =iit->Type;
			  tmp.comment    ="";
			  pFileItemlist.insert(piit+1,tmp);
			  break;
		  }	  
	  }
	}
    cout<<"Finished matching..."<<endl;
    cout<<"Start to Write File..."<<endl;
    wtxt.open(filePath.c_str());//Start Write File
	if(wtxt.fail()){ //if failed to open,fp=0
		cout<<"Fail to open File:" << filePath.c_str() <<endl;
		exit(1);
	}
   
   if(wtxt.is_open()){
		wtxt<<"File Group\tType\tPath\tFlies\tSource\tComment"<<endl;
		std::vector<pFileItem>::iterator piit=pFileItemlist.begin();
        for(;piit!=pFileItemlist.end();piit++)
			wtxt<< piit->fileGroup  <<"\t"
			    << piit->Type       <<"\t"
				<< piit->path       <<"\t"
				<< piit->fileName   <<"\t"
				<< piit->source_path<<endl;
	}
    wtxt.close();//close write file
	cout<<"Write File End"<<endl;
}

string lowerCase(std::string data){
// convert string to back to lower case
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}
