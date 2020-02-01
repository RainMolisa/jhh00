#include "ACode.h"
#include "AMonitor.h"
#include "ASource.h"
#include<iostream>
#include<conio.h>
using namespace std;
int main(int argc, char *argv[]){
	char * version="!HVER!TJStrikeGundam2: 1.6.0 [SJY 01/06/07]";
	try{
		if (InitHTK(argc,argv,version)<0){
			ReportErrors("Main",0);
			return -1;
		}
		//void CreateHMMSet(HMMSet *hset, MemHeap *heap, Boolean allowTMods);
		APacket p;
		AStringData *sd;
		AObsData *od;
		ABuffer scv("scv");
		ABuffer Drone("Drone");
		ASource Zealot("Zealot",&scv);
		ACode Marine("Marine",&scv,&Drone);
		AMonitor OverLoad;
		OverLoad.AddComponent(&Zealot);
		OverLoad.AddComponent(&Marine);
		OverLoad.Start();
		Zealot.Start();
		Marine.Start();
		//AHmms hset("shanghai");
		char buf1;
		bool co=true;
		cout<<"点击y键继续:";
		do{
			buf1=getche();
		}while(buf1!='y');
		cout<<endl;
		/* the basic sequence needed to create a HMM set is
      CreateHMMSet {AddMMF} (MakeHMMSet | MakeOneHMM) LoadHMMSet
*/
		HMMSet* hset=new HMMSet;
		while(co)
		{
			cout<<"说“上海”（按enter键训练,按n开始识别实验）"<<endl;
			Zealot.SendMessageA("start()");
			buf1=getche();//?
			Zealot.SendMessageA("stop()");
			switch(buf1){
			case 13:
				//hset=new AHmms("shanghai");
			/*	Marine.SetCodeHMMSet(&hset);*/
				CreateHMMSet(hset,&(Marine.ReturnMemHeap()),TRUE);
				MakeOneHMM(hset,"./HMMModel/OneModel.txt");
				SaveInOneFile(hset,"./HMMModel/OneModel.txt");
				break;
			case 'n':
				p=Drone.GetPacket();
				od=(AObsData*)p.GetData();
				//od->data;是Observation
				/*if(hset.CheckCompatible(&(od->data))){
					cout<<"No"<<endl;
				}else{
					cout<<"Yes"<<endl;
				}*/
				break;
			default:
				co=false;
				break;
			}
		}
		Zealot.Join();
		Marine.Join();
		Zealot.Terminate();
		Marine.Terminate();
		OverLoad.Terminate();
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