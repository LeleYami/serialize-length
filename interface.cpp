#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TMessage.h"
#include "TBufferFile.h"
#include "TBuffer.h"
#include "TObject.h"
#include "TString.h"
#include "string.h"
#include <iostream>
#include "RootEventData/TBossFullEvent.h"
using namespace std;


int to_TBuffer(char* filename, int entry_id,char* addressOfTBuffer)
	{
		TEvtHeader *event=new TEvtHeader();
		//string name = filename;
		//TBuffer* b;
		TFile *f = new TFile(filename);
		//int temp = atoi(entry_id);
		TChain old_chain("Event");
		old_chain.Add(filename);
		
                TTree* new_tree = old_chain.CloneTree(0);
		old_chain.GetEntry(entry_id);

		new_tree->Fill();
		TMessage mess(kMESS_OBJECT);
		mess.WriteObject(new_tree);
		//int size = mess.Length();
                //mess.WriteObject(event); 
		//TBufferFile b(TBuffer::kWrite,size);
		//TTree::Class()->WriteBuffer(b,new_tree);
                char *tempBuffer=mess.Buffer();
		printf("%s\n",tempBuffer);
                int bufferLength = mess.Length();
		cout<<"the length  of the TOject is:";
                printf("%d\n",bufferLength);
                char *tempBuffer1=new char[bufferLength];
                memcpy(tempBuffer1,tempBuffer,bufferLength);
                printf("%s\n",tempBuffer1);
		printf("%x\n",&tempBuffer1[0]);
		
		addressOfTBuffer = tempBuffer1;
		
                return bufferLength;
		
	}


int main(int argc, char* argv[])
{
        char* arr="abc";
	to_TBuffer(argv[1],argv[2],arr);
	//printf("%x\n",&arr[0]);
	return 0;
} 
