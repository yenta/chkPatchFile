// chkpatchfile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string> 
#include <vector>

using namespace std;


typedef struct item {
    string fileName;
	string language;
	int SEGTT_num;
    int p4_changelist_num;
	string submitted_by;
    int fileGroup;
    char Type;
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
		for(int i=0;i<3;i++) WriteFile(flist[i],itemlist);
		
		
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
					   >> readTmp.comment)
					   pFileItemlist.push_back(readTmp);	
			           //end read line		
		}	
	}
    rtxt.close();//close read file


	//std::vector<item>::iterator iit=itemlist.begin();
	//int group=1;
	//for(;iit!=itemlist.end();iit++){
	//  std::vector<pFileItem>::iterator piit=pFileItemlist.begin();
 //     for(;piit!=pFileItemlist.end();piit++){
	//	  bool fileMatch=false;
	//	  
	//	  if((piit->fileGroup == iit->fileGroup)&&
	//		 (piit->fileName  == iit->fileName)&&
	//		 (piit->path  == iit->destination_path)&&
	//		 (piit->source_path  == iit->source_path)&&
	//		 (piit->Type  == iit->Type)) break; //Same File 
	//	  if(group!=piit->fileGroup && fileMatch=false;){
	//	    group=piit->fileGroup;
 //           
	//	  } 
	//	  
	//  }
	//}


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
				<< piit->source_path<<"\t" 
				<< piit->comment    <<endl;
	}
    wtxt.close();//close write file
}


