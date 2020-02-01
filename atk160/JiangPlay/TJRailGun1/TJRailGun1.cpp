// TJRailGun1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "ASource.h"
#include "AMonitor.h"
#include "ACode.h"
#include "ARec.h"
#include "AComponent.h"
#include<conio.h>
#include <string>
#include <iomanip>
using namespace std;
int main(int argc, char* argv[])
{
	int i;
	char * version="!HVER!TJRailGun1: 1.0.0 [SJY 08/02/13]";
	if (InitHTK(argc,argv,version)<0){
		ReportErrors("Main",0);
		system("pause");
		exit(-1);
	}
	ABuffer auChan("auChan");
	ABuffer feChan("feChan");
	ABuffer ansChan("ansChan");
	ARMan rman;
	ASource ain("AIn",&auChan);
	ACode acode("ACode",&auChan,&feChan);
	ARec arec("ARec",&feChan,&ansChan,&rman);
	AHmms *hset;
	ADict *dict;
	AGram *ggrm;
	ResourceGroup *rg;
	AMonitor amon;
	try {
		
		hset=new AHmms("HmmSet");//
		dict=new ADict("ADict");//
		ggrm=new AGram("GGram","global.net");//
		rman.StoreHMMs(hset);
		rman.StoreDict(dict);
		rman.StoreGram(ggrm);
		amon.AddComponent(&ain);
		amon.AddComponent(&acode);
		amon.AddComponent(&arec);
		rg=rman.NewGroup("yn");
		rg->AddHMMs(hset);
		rg->AddDict(dict);
		rg->AddGram(ggrm);
		amon.Start();
		ain.Start();
		acode.Start();
		arec.Start();
//		system("pause");
		arec.SendMessage("usegrp(yn)");
//		arec.SendMessage("start()");
		char buf1;
		APacket p;
		APhraseData *pd;
		while(1){
			buf1=getche();
			if(buf1==13){
				ain.SendMessage("start()");
				arec.SendMessage("start()");
				/*p=ansChan.GetPacket();
				pd=(APhraseData*)p.GetData();
				pd->Show();*/
				
				do{
                    /*AObsData *od = acode.GetSpecimen();
					if (hset->CheckCompatible(&(od->data))){
						cout<<"true"<<endl;
					}*/
					p=ansChan.GetPacket();
					cout<<"GetKind:"<<p.GetKind()<<endl;
					
					pd=(APhraseData*)p.GetData();
					if(pd->ptype==PhraseType::Word_PT){
						cout<<setw(5)<<pd->word<<":"<<setw(10)<<pd->confidence<<setw(10)<<pd->score<<setw(10)<<pd->tag<<endl;
						//pd->Show();
					}/*else{
						cout<<pd->ptype<<endl;
					}*/
				}while(pd->ptype!=PhraseType::End_PT);
				while(1){
					buf1=getche();
					if(buf1==13){
						ain.SendMessage("stop()");
						arec.SendMessage("stop()");
						break;
					}
				}
			}
		}
//		arec.SendMessage("stop()");
	}
	catch (ATK_Error e){
		int n = HRErrorCount();
		printf("ATK Error %d\n",e.i);
		for (int i=1; i<=n; i++)
			printf("  %d. %s\n",i,HRErrorGetMess(i));
	}
	catch (HTK_Error e){
		int n = HRErrorCount();
		printf("HTK Error %d\n",e.i);
		for (int i=1; i<=n; i++)
			printf("  %d. %s\n",i,HRErrorGetMess(i));
	}
	ain.Join();
	acode.Join();
	arec.Join();
	amon.Terminate();
	system("pause");
	return 0;
}

