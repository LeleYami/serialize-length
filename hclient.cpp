#include "TFile.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TROOT.h"
#include "TTree.h"
#include <iostream>
#include <TObjArray.h>
#include <TLeaf.h>
#include "RootEventData/TBossFullEvent.h"

using namespace std;

void hclient(char *argv,char *argv1)
{

   TEvtHeader *event=new TEvtHeader();

   //gROOT->ProcessLine(".L libthis2.so");

   // Open connection to server
   TSocket *sock = new TSocket("localhost", 51400);

   //wait till we get the start message
   char str[32];
   sock->Recv(str,32);


   //server tells us who we are
   //int idx=!strcmp(str,"go 0") ? 0 : 1;
   int idx;
   if(strcmp(str,"go 0") == 0){
      idx = 0;
    }

   if (idx == 0){
   printf("this is the client:\n");
   
    
   //open the file and get the object
   if(strcmp(argv,"") != 0){
          
          //TFile f("1.dst");
	  TMessage *mess;

	  printf("%s,%s \n",argv,argv1);
	  sock->Send(argv);
	  sock->Send(argv1);

          sock->Recv(mess);
          
          //free(mess);
          //TObject* m= mess->ReadObject(mess->GetClass());
          
          TTree* m= (TTree*)mess->ReadObject(mess->GetClass());

          TBranch* b=m->GetBranch("m_evtRecDTagCol");          
          for(int i=0;i<b->GetEntries();i++){
          	printf("%ld\n",b[i].GetTotalSize());
          }
	  printf("%d\n",b->GetEntries());


          TLeaf* leaf=b->GetLeaf("m_evtRecDTagCol");         
          leaf->Browse(0);

          m->Show(0);
          m->Print();

          
	  sock->Send("Finished");
   }

   //sock->Send("the first client:\n");

  
  }
    // Close the socket
    sock->Close();
  
}

int main(int argc, char **argv){
	hclient(argv[1],argv[2]);
        return 0;
}
