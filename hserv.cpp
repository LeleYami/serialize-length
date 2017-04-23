#include "TFile.h"
#include "TServerSocket.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TMonitor.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TBufferFile.h"
#include "TBuffer.h"
#include "TObject.h"
#include "TString.h"


void hserv() 
{
   //TEvtHeader *event=new TEvtHeader();

   time_t nowTime1;
   time(&nowTime1);
   printf("At now time is: %s\n",ctime(&nowTime1));

   //gROOT->ProcessLine(".L libthis2.so");
   TServerSocket *ss = new TServerSocket(51400, kTRUE);

   // Accept a connection and return a full-duplex communication socket.
   TSocket *s0 = ss->Accept();

   //tell the clients to start
   s0->Send("go 0");

   //Close the server socket
   //ss->Close();

   TMonitor *mon=new TMonitor;
   
   mon->Add(s0);
  
   TSocket *s;
   s = mon->Select();
   char name[64];

   while (1) {
       TMessage *mess;
     
       s->Recv(mess);

       if(mess->What() == kMESS_STRING ){
          	char str[64];

          	mess->ReadString(str,64);

	  if(strcmp(str,"Finished")==0){
          	mon->Remove(s);
          	if (mon->GetActive() == 0) {
            		printf("No more active clients... waitting\n");

   			// Accept a connection and return a full-duplex communication socket.
   			TSocket *s1 = ss->Accept();

   			//tell the clients to start
   			s1->Send("go 0");
   			
			mon->Add(s1);
			s = mon->Select();
          		}
	  }else{
          	//printf("%s   \n", str);
		int tag=0;
		for(int i=0;i<strlen(str);i++)
			if(str[i] <= '0'||str[i] >= '9')
				{tag=1;break;}
		if(tag==1)
			strcpy(name,str);
		else{
		//printf("%s   \n",name);
  
		int temp=atoi(str);
		//printf("%d\n",temp);
                TChain old_chain("Event");
		old_chain.Add(name);
		
                TTree* new_tree = old_chain.CloneTree(0);
		old_chain.GetEntry(0);

          		
                        new_tree->Fill();

          		//cout<<event->getEventId()<<endl;
      			TMessage mess(kMESS_OBJECT);
			mess.WriteObject(new_tree);
                        //mess.WriteObject(event);
                        char *tempBuffer=mess.Buffer();
			printf("%s\n",tempBuffer);
                        /*Int_t hdrlen = 2*sizeof(UInt_t);
                        printf("%d\n",hdrlen);*/
                        int bufferLength = mess.Length();
                        printf("%d\n",bufferLength);
                        char *tempBuffer1=new char[bufferLength];
                        memcpy(tempBuffer1,tempBuffer,bufferLength);
                        printf("%s\n",tempBuffer1);


          		//create TMessage to save the object			
                        new_tree->Print();
                        new_tree->Show(0);
                        //event->Print();
			s->Send(mess);
                        //s->Send(tempBuffer1);  
                        // send message
     			
	    }
	  }
       }

       delete mess;


       if(s == s0){
          printf("Client 1: bytes recv = %d, bytes sent = %d\n", s0->GetBytesRecv(),s0->GetBytesSent());
       }


       // Close the socket.
       

       time_t nowTime2;
       time(&nowTime2);
       printf("At now time is: %s\n",ctime(&nowTime2));
       }
     s->Close();
     ss->Close();

  }

int main(int argc, char **argv){
	hserv();
}
