#include "function.h"
Status CheckPrmtr(String OrderString,int num_prmtr){//�����������Ƿ��㹻
	int check =0;
	P_Char cur =OrderString.head;
	while(cur->next){
		if(cur->data ==' ') check++;
		cur = cur->next;
	}
	if(check <num_prmtr){
		printf("ȱ�ٱ�Ҫ����!\n");
		return ERROR;
	}
	else return OK;
}

int Imprted(P_member Root){//����Ƿ��м�����
	if(!Root || !Root->name.length){
		printf("��δ���������!\n");
		return 0;
	}
	else return 1;
}

int main(){
	P_member Root =NULL;
	P_String set = InitSet();//����set.ini�ļ���ʼ��������ұ�
	String	get_order,order_type;
	InitString(&get_order);
	InitString(&order_type);
	while(1){
		ReInitString(&get_order);
		ReInitString(&order_type);
		P_Char cur = get_order.head;
		printf("CMD$:");
		ScanString(&get_order);
		if(get_order.length == 0) continue;
		SplitString(&order_type,&cur,' ');
		switch(Explaint(set,order_type)){
		case clr:{
			system("cls");	
			break;
		}
		case quit:goto EXIT;
		case statistic:{
			if(!Imprted(Root)) break;
			int i_info[4];
			for(int i =0;i <4;i++) i_info[i] =0;
			P_String S_prfssn =NULL;
			Statistic(i_info,&S_prfssn,*Root);
			float average =(float)i_info[3]/(float)i_info[2];//printf("[3]:%d [2]:%d\n",i_info[3],i_info[2]);
			printf("����:%d�� Ů��:%d�� �ѹʳ�Աƽ������:%.1f\n",
					i_info[0],i_info[1],average);
			printf("ְҵ�ֲ����:\n");
			P_String cur =S_prfssn;
			while(cur){
				PrintString(*cur);
				P_Char c_cur = cur->head;
				while(c_cur->data != '$') c_cur =c_cur->next;
				c_cur = c_cur->next;
				printf(": %d��\n",(int)c_cur->data);
				cur =cur->next;
			}
			cur =S_prfssn;
			while(cur){
				P_String pre =cur;
				cur =cur->next;
				DestroyString(pre);
			}
			break;
		}//case statistic
		case all:{
			if(!Imprted(Root)) break;
			PrintInfo(*Root,0);
			break;
		}//case all
		case tree:{
			if(!Imprted(Root)) break;
			Stack S;
			InitStack(&S);
			PrintTree(*Root,OTHERS,&S);
			DestroyStack(&S);
			break;
		}//case tree
		case info:{
			if(!Imprted(Root)) break;
			if(CheckPrmtr(get_order,1) ==ERROR)break;
			RsltList RL;
			InitRsltList(&RL,NAME);
			Member KeyWord;
			InitTreeNode(&KeyWord);
			SplitString(&KeyWord.name,&cur,' ');
			int counter =0;
			if(Search(Root,NAME,KeyWord,&RL,&counter) ==ERROR){
				printf("�����޹�!\n");
				break;
			}
			P_member M =RL.head->data;
			if(PrintString(M->name) ==ERROR) printf("��");
			printf("\n");
			if(M->sex == 1) printf("��, ");
			else printf("Ů, ");
			printf("��ż:");
			if(PrintString(M->parter) ==ERROR) printf("��");
			printf("\n");
			printf("%d.%d.%d",M->birthdate.year
							 ,M->birthdate.month
							 ,M->birthdate.day);
			printf("~");
			printf("%d.%d.%d",M->datedeath.year
							 ,M->datedeath.month
							 ,M->datedeath.day);
			printf("\n");
			printf("ְҵ:");
			PrintString(M->profession);
			printf("\n");
			DestroyString(&KeyWord.name);
			DestroyRsltList(&RL);
			break;
		}
		case addkid:{
			if(!Imprted(Root)) break;
			if(CheckPrmtr(get_order,2) ==ERROR)break;
			RsltList RL;
			InitRsltList(&RL,NAME);
			Member KeyWord;
			InitTreeNode(&KeyWord);
			SplitString(&KeyWord.name,&cur,' ');
			int counter =0;
			if(Search(Root,NAME,KeyWord,&RL,&counter) ==ERROR){
				printf("�����޹�!\n");
				break;
			}
			String FileName;
			InitString(&FileName);
			SplitString(&FileName,&cur,'$');
			char *fn = NULL;
			TransString(FileName,&fn);
			FILE *fin = fopen(fn,"r");
			if(!fin){
				printf("�����Ҳ������ļ�!\n");
				break;
			}
			P_member parent =RL.head->data;
			P_member newkid =NULL;
			if(parent->children){
				P_member lastkid =parent->firstchild;
				while(lastkid->nextsibling) lastkid =lastkid->nextsibling;
			    lastkid->nextsibling =(P_member)malloc(sizeof(Member));
				newkid =lastkid->nextsibling;
			}
			else{
				parent->firstchild =(P_member)malloc(sizeof(Member));
				newkid =parent->firstchild;
			}
			InitTreeNode(newkid);
			parent->children++;

			String FT;
			InitString(&FT);
			FileString(&FT,fin);
			P_Char *cur =&FT.head;
			GnrtTree(newkid,newkid->parent,cur);		
		//	LoadData(newkid,&fin,parent);
			DestroyRsltList(&RL);
			DestroyString(&FileName);
			DestroyString(&KeyWord.name);
			DestroyString(&FT);
			fclose(fin);
			break;
		}//case addkid:
  		case load:{
			if(CheckPrmtr(get_order,1) ==ERROR) break;
			Root =(P_member)malloc(sizeof(Member));
			InitTreeNode(Root);
			String FileName;
			InitString(&FileName);
			SplitString(&FileName,&cur,'$');
			char *fn = NULL;
			TransString(FileName,&fn);
			FILE *fin = fopen(fn,"r");
			if(!fin){
				printf("�����Ҳ������ļ�!\n");
				break;
			}
			LoadData(Root,&fin,NULL);
			fclose(fin);
			DestroyString(&FileName);
			break;
		}//case load
		case delmmbr:{
			if(!Imprted(Root)) break;
			if(CheckPrmtr(get_order,1) ==ERROR) break;
			Member KeyWord;
			InitTreeNode(&KeyWord);
			SplitString(&KeyWord.name,&cur,' ');
			RsltList RL;
			InitRsltList(&RL,NAME);
			int counter =0;
			if(Search(Root,NAME,KeyWord,&RL,&counter) ==ERROR){
				printf("�����޹�!\n");
				break;
			}
			Modify(RL.head->data,KeyWord,DEL);
			if(RL.head->data == Root) Root =NULL;
			DestroyString(&KeyWord.name);
			DestroyRsltList(&RL);
			break;
		}
   		case imprt:{
			if(CheckPrmtr(get_order,1) ==ERROR) break;
			Root =(P_member)malloc(sizeof(Member));
			InitTreeNode(Root);
			String FileName;
			InitString(&FileName);
			SplitString(&FileName,&cur,'$');
			char *fn = NULL;
			TransString(FileName,&fn);
			FILE *fin = fopen(fn,"r");
			if(!fin){
				printf("�����Ҳ������ļ�!\n");
				break;
			}
			String FS;
			InitString(&FS);
			FileString(&FS,fin);
			P_Char cur = FS.head;
			GnrtTree(Root,NULL,&cur);
			DestroyString(&FileName);
			DestroyString(&FS);
			break;
		}//case imprt
		case save:{
			if(!Imprted(Root)) break;
			if(CheckPrmtr(get_order,1) ==ERROR) break;
			String FileName;
			InitString(&FileName);
			SplitString(&FileName,&cur,'$');
			char *fn = NULL;
			TransString(FileName,&fn);
			FILE *fout = fopen(fn,"a");
			SaveData(*Root,&fout);
			fclose(fout);
			DestroyString(&FileName);
			break;
		}//case save
		case srch:{
			if(!Imprted(Root)) break;
			if(CheckPrmtr(get_order,2) ==ERROR) break;
			String ST;
			InitString(&ST);
			SplitString(&ST,&cur,' ');
			OprtnType SrchType = (OprtnType)(Explaint(set,ST) -12);
			RsltList RL;
			InitRsltList(&RL,SrchType);
			Member KeyInfo;	 
			InitTreeNode(&KeyInfo);
			switch(SrchType){
			case PROFESSION:{
				SplitString(&KeyInfo.profession,&cur,'$');
				break;
			}
			case SEX:{
				KeyInfo.sex = (Info)(cur->data -48);
				break;
			}
			case PARTER:{
				SplitString(&KeyInfo.parter,&cur,'$');
				break;
			}
			case BIRTH:{
				if(CheckPrmtr(get_order,7) ==ERROR) break;
				String date[6];
				for(int i =0;i <6;i++){
					InitString(date +i);
					SplitString(date +i,&cur,' ');
				}
				KeyInfo.birthdate.year = TransForm(date[0]);
				KeyInfo.birthdate.month = TransForm(date[1]);
				KeyInfo.birthdate.day = TransForm(date[2]);
				KeyInfo.datedeath.year = TransForm(date[3]);
				KeyInfo.datedeath.month = TransForm(date[4]);
				KeyInfo.datedeath.day = TransForm(date[5]);
				for(i =0;i <6;i++) DestroyString(date +i);
				break;
			}
			case DEATH:{
				if(CheckPrmtr(get_order,7) ==ERROR) break;
				String date[6];
				for(int i =0;i <6;i++){
					InitString(date +i);
					SplitString(date +i,&cur,' ');
				}
				KeyInfo.birthdate.year = TransForm(date[0]);
				KeyInfo.birthdate.month = TransForm(date[1]);
				KeyInfo.birthdate.day = TransForm(date[2]);
				KeyInfo.datedeath.year = TransForm(date[3]);
				KeyInfo.datedeath.month = TransForm(date[4]);
				KeyInfo.datedeath.day = TransForm(date[5]);
				for(i =0;i <6;i++) DestroyString(date +i);
				break;	
			}
			default:printf("��������!\n");
			}//switch(SrchType)
			int counter =0;
			if(Search(Root,SrchType,KeyInfo,&RL,&counter) ==ERROR){
				printf("�����޹�!\n");
			}
			else{
				PrintRList(RL);
				printf("\n");
			}
			DestroyRsltList(&RL);
			DestroyString(&ST);
			break;
		}//case srch
		case mdf:{
			if(!Imprted(Root)) break;
			if(CheckPrmtr(get_order,3) ==ERROR) break;
			RsltList SystmSRL;//�洢ϵͳ���ҽ��
			InitRsltList(&SystmSRL,NAME);
			String MdfyItem;
			Member KeyWord;//���ڲ��������
			Member InstdInfo;
			InitTreeNode(&KeyWord);
			InitTreeNode(&InstdInfo);
			InitString(&MdfyItem);
			SplitString(&KeyWord.name,&cur,' ');
			SplitString(&MdfyItem,&cur,' ');
			OprtnType OT = (OprtnType)(Explaint(set,MdfyItem) -12);
			int counter = 0;
			if(Search(Root,NAME,KeyWord,&SystmSRL,&counter) == ERROR){
				printf("�����޹�!\n");
				break;
			}
			Status flag =TRUE;//�����ж��û������item�����Ƿ���ȷ
			switch(OT){
			case NAME:{
				SplitString(&InstdInfo.name,&cur,'$');
				break;
			}//case NAME
			case PROFESSION:{
				SplitString(&InstdInfo.profession,&cur,'$');
				break;
			}//case PROFESSION
			case SEX:{
				InstdInfo.sex = (Info)(cur->data -48);
				break;
			}//case SEX
			case PARTER:{
				SplitString(&InstdInfo.parter,&cur,'$');
				break;
			}//case PARTER
			case BIRTH:{
				if(CheckPrmtr(get_order,5) ==ERROR) break;
				String date[3];
				//for(int i =0;i <6;i++){ һ����Ȥ��bug����ַ
				for(int i =0;i <3;i++){//bug�ĺ�����·�375��
					InitString(date +i);
					SplitString(date +i,&cur,' ');
				}//printf("OT3: %d\n",OT);��bug�ὫOT��0
				InstdInfo.birthdate.year = TransForm(date[0]);
				InstdInfo.birthdate.month = TransForm(date[1]);
				InstdInfo.birthdate.day = TransForm(date[2]);
				for(i =0;i <3;i++) DestroyString(date +i);
				break;
			}//case BITRH
			case DEATH:{
				if(CheckPrmtr(get_order,5) ==ERROR) break;
				String date[3];
				for(int i =0;i <3;i++){
					InitString(date +i);
					SplitString(date +i,&cur,' ');
				}
				InstdInfo.datedeath.year = TransForm(date[0]);
				InstdInfo.datedeath.month = TransForm(date[1]);
				InstdInfo.datedeath.day = TransForm(date[2]);
				for(i =0;i <3;i++) DestroyString(date +i);
				break;
			}//case DEATH
			default:{
				flag =FALSE;
				printf("��������!\n");
			}//default
			}//switch(OT)
			if(flag ==FALSE) break;
			Modify(SystmSRL.head->data,InstdInfo,OT);//���´˴��޷���������
			DestroyRsltList(&SystmSRL);
			DestroyString(&MdfyItem);
			break;
		}//case mdf
		default:printf("��Ч������!\n");
  		}//switch(Explaint(set,order_type))
	}//while(1)
EXIT:
	return 0;
}
