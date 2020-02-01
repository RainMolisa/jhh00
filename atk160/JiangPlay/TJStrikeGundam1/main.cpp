
#include "AIO.h"
#include "AMonitor.h"
#include<iostream>
#include<string>
using namespace std;
int main(int argc, char *argv[]){
	char * version="!HVER!TJStrikeGundam1: 1.6.0 [SJY 01/06/07]";
	string muta;
	try
	{
		if (InitHTK(argc,argv,version)<0){
			ReportErrors("Main",0);
			exit(-1);
		}
		ABuffer kl("kl");
		AIO Marauder("Marauder",&kl);
		AMonitor OverLoad;
		OverLoad.AddComponent(&Marauder);
		OverLoad.Start();
		Marauder.Start();
		while(1){
			cin>>muta;
			if(muta=="end"){
				Marauder.SendMessage("closedown()");
				Marauder.SendMessage("terminate()");
				Marauder.Join();
				OverLoad.Terminate();
				return 0;
			}
			muta="tell(\""+muta;
			muta=muta+"\")";
			Marauder.SendMessage(muta);
		}
	}
	catch (ATK_Error e)
	{ 
		ReportErrors("ATK",e.i); 
	}
    catch (HTK_Error e)
	{ 
		ReportErrors("HTK",e.i); 
	}
	return 0;
}