#include <stdlib.h>
#include <stdio.h>
#include "Define.h"
#include <conio.h>
//��ʼ������ַ�������һ����Ԫ���洢$�ַ�
void InitString(P_String S){
	S->length = 0;
	S->head = (P_Char)malloc(sizeof(CharNode));
	S->head->next = NULL;
	S->head->data = '$';
	S->next = NULL;
}

void ReInitString(P_String S){
	P_Char cur = S->head,pre = NULL;
	while(cur){
		pre = cur;
		cur = cur->next;
		free(pre);
		pre = NULL;
	}
	InitString(S);
}
void DestroyString(P_String PS){
	ReInitString(PS);
	free(PS->head);
	PS->head = NULL;
}
//�����ַ�����S����Ϊ��ʼ��״̬���ַ���
void ScanString(P_String S){
	char input;
	P_Char cur = S->head;
	while(input = getchar(),input != '\n'){
		cur->data = input;
		cur->next = (P_Char)malloc(sizeof(CharNode));
		cur = cur->next;
		cur->data = '$';
		cur->next = NULL;
		S->length ++;
	}
}

Status PrintString(String S){
	if(!S.length) return ERROR;
	P_Char cur = S.head;
	while(cur->data != '$' && cur->data != '#'){
		printf("%c",cur->data);
		cur = cur->next;
	}
	return OK;
}
//�����ַ���,S2����Ϊ�ѳ�ʼ�����ַ���
void CopyString(String S1,P_String S2){
	P_Char cur1 = S1.head,cur2 = S2->head;
	while(cur1->data != '$'){
		cur2->data = cur1->data;
		cur1 = cur1->next;
		cur2->next = (P_Char)malloc(sizeof(CharNode));
		cur2 = cur2->next;
		cur2->data = '$';
		cur2->next = NULL;
	}
	S2->length = S1.length;
}
//�������ļ����ַ�����ʽ�洢
//(*S)����Ϊ��ʼ��״̬���ַ���
void FileString(P_String S,FILE *fin){
	P_Char cur = S->head;
	char c;
	while(c = fgetc(fin),c != '$'){
		if(c == '\n') cur->data = ',';
		else if(c=='{'||c=='}'||c=='['||c==']'||c==' '||c=='	') continue;
		else cur->data = c;
		cur->next = (P_Char)malloc(sizeof(CharNode));
		cur->next->data = '$';
		cur->next->next = NULL;
		cur = cur->next;
		S->length ++;
	}
}

void InitDate(Date *D){
	D->day = 0;
	D->month = 0;
	D->year = 0;
}

Status InitTreeNode(P_member PM){
	PM->firstchild = NULL;
	PM->nextsibling = NULL;
	PM->parent = NULL;
	InitDate(&(PM->birthdate));
	InitDate(&(PM->datedeath));
	InitString(&(PM->name));
	InitString(&(PM->parter));
	InitString(&(PM->profession));
	PM->children = 0;
	return OK;
}
//obj����Ϊ��ʼ������String����
//��ʼ״̬(*cur)��ָ�����ַ����ĵ�һ���ַ�
//�������ַ������ӿ�ʼʱcur��ָ�ַ���breakpoint��ǰһ���ַ������ݸ������ַ���obj
//��ʹ(*cur)ָ����һ�����ַ����ĵ�һ���ַ�
Status SplitString(P_String obj,P_Char *cur,char breakpoint){
	if(obj == NULL){
		while((*cur)->data != breakpoint)
			(*cur) = (*cur)->next;
	}
	else{
		P_Char cur_obj = obj->head;
		obj->length = 0;
		while((*cur)->data !=breakpoint &&(*cur)->data !='$'){
			cur_obj->data = (*cur)->data;
			cur_obj->next = (P_Char)malloc(sizeof(CharNode));
			cur_obj = cur_obj->next;
			cur_obj->next = NULL;
			cur_obj->data = '$';
			obj->length ++;
			(*cur) = (*cur)->next;
		}
	}
	if((*cur)->data !='$'){
		do{//�������������breakpoint�����
			(*cur) = (*cur)->next;
		}while((*cur)->data == breakpoint);
	}
	return OK;
}
//n����10��time�η�
int N10(int time,int n){
	for(int i = 0;i < time;i++) n *= 10;
	return n;
}

//���ַ���ת��Ϊ������curָ�����ַ����ĵ�һλ��i��ʾ���ֵĵ�iλ
int TransForm(String S){
	int sum = 0;
	P_Char cur = S.head;
	int i = S.length;
	for(i;i > 0;i--){
		sum += N10(i-1,cur->data-48);
		cur = cur->next;
	}
	return sum;
}		

void Dater(String S,Date *D){
	P_Char cur = S.head;
	for(int i = 4;i > 0;i--){
		D->year += N10(i-1,cur->data-48);
		cur = cur->next;
	}
	P_Char cur2 = cur->next->next;
	for(i = 2;i > 0;i--){
		D->month += N10(i-1,cur->data-48);
		D->day += N10(i-1,cur2->data-48);
		cur = cur->next;
		cur2 = cur2->next;
	}
}
//(*cur)ָ����(*PM)Ϊ���������ַ�����λ�����ַ���������	
Status GnrtTree(P_member PM,P_member Parent,P_Char *cur){
	PM->parent = Parent;
	SplitString(&(PM->name),cur,',');
	if((*cur)->data == '1') PM->sex = male;
	else PM->sex = female;
	*cur = (*cur)->next->next;
	SplitString(&(PM->parter),cur,',');
	String S2;
	InitString(&S2);
	SplitString(&S2,cur,',');
	Dater(S2,&(PM->birthdate));
	ReInitString(&S2);
	SplitString(&S2,cur,',');
	Dater(S2,&(PM->datedeath));
	SplitString(&(PM->profession),cur,',');
	ReInitString(&S2);
	SplitString(&S2,cur,',');               
	PM->children = TransForm(S2);
	P_member kid = PM;
	for(int i = 0;i < PM->children;i++){
		if(i == 0){
			PM->firstchild = (P_member)malloc(sizeof(Member));
			kid = PM->firstchild;
		}
		else{
			kid->nextsibling = (P_member)malloc(sizeof(Member));
			kid = kid->nextsibling;
		}
		InitTreeNode(kid);
		GnrtTree(kid,PM,cur);
	}
	return OK;
}	
//��Stringת��Ϊ˳��洢��char[n]
//�õ����ַ�����ԭ����һ����Ԫ���ڴ洢'\0'
void TransString(String S,char **s){
	*s = (char *)malloc(sizeof(char)*(S.length+1));
	P_Char cur = S.head;
	int i = 0;
	for(i;i < S.length;i++){
		(*s)[i] = cur->data;
		cur = cur->next;
	}(*s)[i] = '\0';
}

void SaveString(String S,FILE **fout){
	char *s = NULL;
	TransString(S,&s);
	fwrite(s,sizeof(char),S.length+1,*fout);
}

Status SaveData(Member M,FILE **fout){
	SaveString(M.name,fout);
	fwrite(&(M.sex),sizeof(Info),1,*fout);
	SaveString(M.parter,fout);
	SaveString(M.profession,fout);
	fwrite(&(M.birthdate),sizeof(int),3,*fout);
	fwrite(&(M.datedeath),sizeof(int),3,*fout);
	fwrite(&(M.children),sizeof(int),1,*fout);
//	fwrite(&(M.parent),sizeof(P_member),3,*fout);�ô�������һ��������Ϊ�����bug
	P_member kid = M.firstchild;
	for(int i = 0;i < M.children;i++){
		SaveData(*kid,fout);
		kid = kid->nextsibling;
	}
	return OK;
}

void LoadString(P_String P_S,FILE **fin){
	InitString(P_S);
	P_Char cur = P_S->head;
	char c;
	fread(&c,sizeof(char),1,*fin);
	while(c != '\0'){	
		cur->data = c;
		cur->next = (P_Char)malloc(sizeof(CharNode));
		cur = cur->next;
		cur->data = '$';
		cur->next = NULL;
		P_S->length++;
		fread(&c,sizeof(char),1,*fin);
	}
}
//���뱣��ļ�������Ϣ���������ļ�����PM�������ں������ʼ�����Ľڵ�
Status LoadData(P_member PM,FILE **fin,P_member Parent){
	PM->parent = Parent;
	LoadString(&(PM->name),fin);
//	PrintString(PM->name);
	fread(&(PM->sex),sizeof(Info),1,*fin);
	LoadString(&(PM->parter),fin);
	LoadString(&(PM->profession),fin);
	fread(&(PM->birthdate),sizeof(int),3,*fin);
	fread(&(PM->datedeath),sizeof(int),3,*fin);
	fread(&(PM->children),sizeof(int),1,*fin);
//	fread(&(PM->parent),sizeof(P_member),3,*fin);
	P_member kid = PM;
	for(int i = 0;i < PM->children;i++){
		if(i == 0){
			kid->firstchild = (P_member)malloc(sizeof(Member));
			kid = kid->firstchild;
		}
		else{
			kid->nextsibling = (P_member)malloc(sizeof(Member));
			kid = kid->nextsibling;
		}
		InitTreeNode(kid);		
		LoadData(kid,fin,PM);
	}
	return OK;
}
//��ʽ����
void format(int counter){
	for(int i = 0;i < counter;i++){
		printf(" ");
	}
}
//�����M��ʼ�ļ�������������Ϣ
//�û�����ʱcounter����Ϊ0
Status PrintInfo(Member M,int counter){
	format(counter);
	PrintString(M.name);
	printf("{\n");

	format(counter+2);
	if(M.sex == 1) printf("��, ");
	else printf("Ů, ");

	printf("��ż:");
	if(PrintString(M.parter) ==ERROR) printf("��");
	printf("\n");
	format(counter+2);
	
	printf("%d.%d.%d",M.birthdate.year
					 ,M.birthdate.month
					 ,M.birthdate.day);
	printf("~");
	printf("%d.%d.%d",M.datedeath.year
					 ,M.datedeath.month
					 ,M.datedeath.day);
	printf("\n");

	format(counter+2);
	printf("ְҵ:");
	if(PrintString(M.profession) ==ERROR) printf("��");
	printf("\n");

	format(counter+2);
	printf("��%d������[\n",M.children);
    if(M.children){
		counter += 4;
		P_member cur = M.firstchild;
		while(cur){
			PrintInfo(*cur,counter);
			cur = cur->nextsibling;
		}
		counter -= 4;
	}
	format(counter+2);
	printf("]\n");
	format(counter);
	printf("}\n");
	return OK;
}

Status InitStack(Stack *S){
	S->length = 0;
	S->bottom = (NodePosition)malloc(sizeof(Node));
	S->top = S->bottom;
	S->top->final = ERROR;
	S->top->next = NULL;
	S->top->pre = NULL;
	return OK;
}

Status Push(Stack *S,Status final){
	S->length++;
	S->top->next = (NodePosition)malloc(sizeof(Node));
	NodePosition pre = S->top;
	S->top = S->top->next;
	S->top->final = final;
	S->top->next = NULL;
	S->top->pre = pre;
	return OK;
}

Status Pop(Stack *S){
	S->top = S->top->pre;
	free(S->top->next);
	S->top->next = NULL;
	S->length--;
	return OK;
}

Status DestroyStack(Stack *S){
	for(int i =0;i <S->length;i++) Pop(S);
	free(S->bottom);
	S->bottom = NULL;
	S->top = NULL;
	S->length = 0;
	return OK;
}
//�ַ���ʾģʽ�´�ӡ���νṹ,final��ʶM�Ƿ�Ϊ��parent�����һ��kid
//�û�����ʱfinal������OTHERS
Status PrintTree(Member M,Status final,Stack *S){
	NodePosition cur = S->bottom->next;
	P_member cp = NULL;
	int i = 1;
	while(i<=S->length){
		if(cur->final == TRUE) printf("  ");
		else if(cur->final == FALSE) printf("��");
		if(i == S->length){
			if(final == TRUE) printf("��");
			else printf("��");
		}
		cur = cur->next;
		i++;
	}
	PrintString(M.name); printf("\n");
	Push(S,final);
	final = FALSE;
	for(i = 1;i <= M.children;i++){
		if(i == 1) cp = M.firstchild;
		else cp = cp->nextsibling;
		if(i == M.children) final = TRUE;
		PrintTree(*cp,final,S);
	}
	Pop(S);
	return OK;
}
//
void DeleteTree(P_member del){
	if(del){
		DeleteTree(del->firstchild);
		del->firstchild = NULL;
		DeleteTree(del->nextsibling);
		del->nextsibling = NULL;
		free(del);
		del = NULL;
	}
}
//�ж������ַ����Ƿ���ͬ
Status StringEqual(String S1,String S2){
	if(S1.length != S2.length) return FALSE;
	P_Char cur1 = S1.head,cur2 = S2.head;
	while(cur1->data == cur2->data && cur1->data != '$'){
		cur1 = cur1->next;
		cur2 = cur2->next;
	}
	if(cur1->data == '$') return TRUE;
	return FALSE;
}
//�ж�����D�Ƿ������ָ���ķ�Χ��
//��KeyInfo�е�datedeath��birthday�ֱ��ʾ����ƥ�䷶Χ������
Status Within(Member KeyInfo,Date D){
//	printf("%d %d %d ~ %d %d %d :%d %d %d\n",KeyInfo.birthdate.year,KeyInfo.birthdate.month,KeyInfo.birthdate.day,KeyInfo.datedeath.year,KeyInfo.datedeath.month,KeyInfo.datedeath.day,D.year,D.month,D.day);
	if(D.year <= KeyInfo.datedeath.year && D.year >= KeyInfo.birthdate.year){
		if(D.year < KeyInfo.datedeath.year && D.year > KeyInfo.birthdate.year) return TRUE;
		else if(D.year == KeyInfo.birthdate.year){
			if(D.month > KeyInfo.birthdate.month) return TRUE;
			else if(D.month == KeyInfo.birthdate.month){
				if(D.day >= KeyInfo.birthdate.day) return TRUE;
			}
		}
		else if(D.year == KeyInfo.datedeath.year){
			if(D.month < KeyInfo.datedeath.month) return TRUE;
			else if(D.month == KeyInfo.datedeath.month){
				if(D.day <= KeyInfo.datedeath.day) return TRUE;
			}
		}
	}
	return FALSE;
}

Status InitRsltList(RsltList *Rslt,OprtnType SrchType){
	Rslt->SrchType = SrchType;
	Rslt->length = 0;
	Rslt->head = (P_RsltNode)malloc(sizeof(RsltNode));
	Rslt->head->next = NULL;
	Rslt->head->data = NULL;
	Rslt->tail = Rslt->head;
	return OK;
}
//���������һ���洢��ԪΪ��
Status Rslt_AddNode(RsltList *Rslt,P_member Data){
	Rslt->tail->data = Data;
	Rslt->tail->next = (P_RsltNode)malloc(sizeof(RsltNode));
	Rslt->tail = Rslt->tail->next;
	Rslt->tail->next = NULL;
	Rslt->tail->data = NULL;
	Rslt->length ++;
	return OK;
}

void PrintRList(RsltList RL){
	P_RsltNode cur = RL.head;
	while(cur->next){
		PrintString(cur->data->name);
		printf(" ");
		cur = cur->next;
	}
}

void DestroyRsltList(RsltList *Rslt){
	P_RsltNode dstr = NULL,cur = Rslt->head;
	do{
		dstr = cur;
		cur = cur->next;
		free(dstr);
		dstr = NULL;
	}while(cur);
	Rslt->tail = NULL;
	Rslt->length = 0;
}
//��ָ������Ϣ����������������ƥ��Ľڵ�ĵ�ַ�洢��RL��
//counter���ڼ�������Ƿ��н��
Status Search(P_member Root,OprtnType ST,Member KeyInfo,RsltList *RL,int *counter){
	if(Root){
		switch(ST){
		 case NAME:{
			 if(StringEqual(KeyInfo.name,Root->name) == TRUE){
				 (*counter)++;
				 Rslt_AddNode(RL,Root);
			 }
			 Search(Root->firstchild,NAME,KeyInfo,RL,counter);
			 Search(Root->nextsibling,NAME,KeyInfo,RL,counter);
			 break;
		 }
		 case PROFESSION:{
			 if(StringEqual(KeyInfo.profession,Root->profession) == TRUE){
				 (*counter)++;
				 Rslt_AddNode(RL,Root);
			 }
			 Search(Root->firstchild,PROFESSION,KeyInfo,RL,counter);
			 Search(Root->nextsibling,PROFESSION,KeyInfo,RL,counter);
			 break;
		 }
		 case SEX:{
			 if(KeyInfo.sex == Root->sex){
				 (*counter)++;
				 Rslt_AddNode(RL,Root);
			 }
			 Search(Root->firstchild,SEX,KeyInfo,RL,counter);
			 Search(Root->nextsibling,SEX,KeyInfo,RL,counter);
			 break;
		 }
		 case PARTER:{
			 //PrintString(Root->parter);printf("\n");
			 if(StringEqual(KeyInfo.parter,Root->parter) ==TRUE){
				 (*counter)++;
				 Rslt_AddNode(RL,Root);
			 }
			 Search(Root->firstchild,PARTER,KeyInfo,RL,counter);
			 Search(Root->nextsibling,PARTER,KeyInfo,RL,counter);
			 break;
			 }
		 case BIRTH:{
		//	 printf("%d %d %d ~ %d %d %d\n",KeyInfo.birthdate.year,KeyInfo.birthdate.month,KeyInfo.birthdate.day,KeyInfo.datedeath.year,KeyInfo.datedeath.month,KeyInfo.datedeath.day);
			 if(Within(KeyInfo,Root->birthdate) == TRUE){
				 (*counter)++;
				 Rslt_AddNode(RL,Root);
			 }
			 Search(Root->firstchild,BIRTH,KeyInfo,RL,counter);
			 Search(Root->nextsibling,BIRTH,KeyInfo,RL,counter);
			 break;
		 }
		 case DEATH:{
		//	 printf("%d %d %d ~ %d %d %d\n",KeyInfo.birthdate.year,KeyInfo.birthdate.month,KeyInfo.birthdate.day,KeyInfo.datedeath.year,KeyInfo.datedeath.month,KeyInfo.datedeath.day);
		   	 if(Within(KeyInfo,Root->datedeath) == TRUE){
				 Rslt_AddNode(RL,Root);
				 (*counter)++;
			 }
			 Search(Root->firstchild,DEATH,KeyInfo,RL,counter);
			 Search(Root->nextsibling,DEATH,KeyInfo,RL,counter);
			 break;
		 }
		}//switch(ST)
		if(*counter ==0) return ERROR;
		return OK;
	}//if(Root)
	return ERROR;
}

void Modify(P_member obj,Member InstdInfo,OprtnType OT){
	switch(OT){
	case NAME:{
		ReInitString(&obj->name);
		CopyString(InstdInfo.name,&obj->name);
		break;
		}
	case PROFESSION:{
		ReInitString(&obj->profession);
		CopyString(InstdInfo.profession,&obj->profession);
		break;
		}
	case SEX:{
		obj->sex = InstdInfo.sex;
		break;
		}
	case PARTER:{
		ReInitString(&obj->parter);
		CopyString(InstdInfo.parter,&obj->parter);
		break;
		}
	case BIRTH:{
		obj->birthdate.year = InstdInfo.birthdate.year;
		obj->birthdate.month = InstdInfo.birthdate.month;
		obj->birthdate.day = InstdInfo.birthdate.day;
		break;
		}
	case DEATH:{
		obj->datedeath.year = InstdInfo.datedeath.year;
		obj->datedeath.month = InstdInfo.datedeath.month;
		obj->datedeath.day = InstdInfo.datedeath.day;
		break;
		}
	case DEL:{
		if(obj->parent){//�Ǽ�������
			obj->parent->children--;
			if(obj ==obj->parent->firstchild){
				obj->parent->firstchild =obj->nextsibling;
			}
			else{
				P_member cur =obj->parent->firstchild;
				while(cur->nextsibling != obj) cur = cur->nextsibling;
				cur->nextsibling = obj->nextsibling;
			}
		}
		DeleteTree(obj->firstchild);
		free(obj);
		break;
		}
	default: printf("OT error!\n");
	}//switch(OT)
}
//���������ʼ��
P_String InitSet(){
	FILE *fin = fopen("set.ini","r");
	String S;
	InitString(&S);
	FileString(&S,fin);
	fclose(fin);
	P_String rtrn= (P_String)malloc(sizeof(String) *18);//�ַ���������
	P_Char cur = S.head;
	SplitString(NULL,&cur,',');
	for(int i = 0;i <19;i++) InitString(rtrn +i);
	i = 0;
	for(i;i <5;i++){
		SplitString(rtrn +i,&cur,':');
		SplitString(NULL,&cur,',');
	}/**/
	for(i;i <13;i++){
		SplitString(rtrn +i,&cur,'<');
		SplitString(NULL,&cur,',');
	}	
	SplitString(NULL,&cur,',');
	for(i;i <19;i++){
		SplitString(rtrn +i,&cur,':');
		SplitString(NULL,&cur,',');
	}
	ReInitString(&S);
	free(S.head);
	S.head = NULL;
	return rtrn;
}

Explnt Explaint(P_String PS,String S){
	for(int i = 0;i <19;i++){
		if(StringEqual(S,PS[i]) == TRUE) return (Explnt)i;
	}
	return (Explnt)100;//�˴����ڴ�����typedef������������һ��bug
}					   //ԭ�ȵ����Ϊreturn error
//int i_info[4]
//i_info[0]���Գ�Ա��
//i_info[1]Ů�Գ�Ա��
//i_info[2]�ѹʳ�Ա��
//i_info[3]�ѹʳ�Ա����ϼ�
void DBG(String S){//Debug
	P_Char cur =S.head;
	while(cur->data != '$') cur =cur->next;
	cur =cur->next;
	PrintString(S);printf(":%d\n",(int)(cur->data));
}
void Statistic(int *i_info,P_String *S_P,Member M){//PrintString(M.name);
	switch(M.sex){
		case male:i_info[0]++;break;
		case female:i_info[1]++;break;
	}
	if(*S_P ==NULL){
		(*S_P) =(P_String)malloc(sizeof(String));
		InitString(*S_P);
		(*S_P)->next =NULL;
		CopyString(M.profession,(*S_P));
		P_Char cur =(*S_P)->head;
		while(cur->data != '$') cur =cur->next;
		cur->next = (P_Char)malloc(sizeof(CharNode));
		cur =cur->next;
		cur->data = 1;//���ַ����Ľ�β���һ���ڵ��¼��ְҵ����
//		printf("DBG ");DBG(**S_P);
		cur->next =NULL;//printf(" NULL OK!\n");
	}
	else{
		int flag =0;
		P_String cur =(*S_P);//printf("DBG ");DBG(*cur);
		while(cur){//printf("DBG ");DBG(*cur);
			if(StringEqual(*cur,M.profession) ==TRUE){//printf(" flag");
//printf("DBG ");DBG(*cur);
				flag =1;//printf("DBG ");DBG(*cur);
				P_Char c_cur =cur->head;//printf("DBG ");DBG(*cur);
				while(c_cur->data != '$') c_cur =c_cur->next;//printf("DBG ");DBG(*cur);
		//	printf("DBG ");DBG(*cur);
				c_cur =c_cur->next;	//	PrintString(*cur);printf(": %d(����ǰ)\n",(int)(c_cur->data));
				c_cur->data =((int)(c_cur->data))+1;//��ְҵ������һ
//printf("DBG ");DBG(*cur);
				c_cur->next =NULL;//printf(": OK!\n");
				break;
			}
			cur =cur->next;
		}
		cur =(*S_P);
		while(cur->next) cur =cur->next;
		if(!flag){//printf(": !flag");
			cur->next =(P_String)malloc(sizeof(String));
			cur =cur->next;
			InitString(cur);
			cur->next =NULL;
			CopyString(M.profession,cur);
			P_Char c_cur =cur->head;
			while(c_cur->data != '$') c_cur =c_cur->next;
			c_cur->next = (P_Char)malloc(sizeof(CharNode));
			c_cur =c_cur->next;
			c_cur->data = 1;//���ַ����Ľ�β���һ���ڵ��¼��ְҵ����
//	PrintString(*cur);printf(": %d\n",(int)(c_cur->data));
			c_cur->next =NULL;//printf(": OK!\n");
		}
	}
	if(M.datedeath.month){//�ó�Ա�ѹ�
		i_info[2]++;
		i_info[3] += M.datedeath.year -M.birthdate.year;
		if(M.datedeath.month <M.birthdate.month) i_info[3]--;
	}//getch();
	if(M.firstchild) Statistic(i_info,S_P,*(M.firstchild));//getch();
	if(M.nextsibling) Statistic(i_info,S_P,*(M.nextsibling));//PrintString(M.name);printf(" done!\n");
}
/*
void KeyWordForSearch(P_member Root,RsltList *RL){
	printf("1.name 2.profession 3.birthday 4.datedeath 5.sex\n");
	OprtnType ST;
	printf("Search By:");
	scanf("%d",&ST);
	InitRsltList(RL,ST);
	Member KeyInfo;
	InitTreeNode(&KeyInfo);
	int counter =0;
	if(ST == NAME || ST == PROFESSION){
		getchar();//����������е�'\n'�ַ�ʹ�������ScanString��������������
		switch(ST){
		case NAME:{
			printf("input name:");
			ScanString(&KeyInfo.name);
			Search(Root,NAME,KeyInfo,RL,&counter);
			break;
			}
		case PROFESSION:{
			printf("input profession:");
			ScanString(&KeyInfo.profession);
			Search(Root,PROFESSION,KeyInfo,RL,&counter);
			}
		}//switch(ST)
		ReInitString(&KeyInfo.name);
	}
	else if(ST == BIRTH || ST == DEATH){
		printf("input the rank of date:XXXX XX XX\n");
		printf("from :");
		scanf("%d%d%d",&KeyInfo.birthdate.year
					  ,&KeyInfo.birthdate.month
					  ,&KeyInfo.birthdate.day
		);
		printf("to :");
		scanf("%d%d%d",&KeyInfo.datedeath.year
					  ,&KeyInfo.datedeath.month
					  ,&KeyInfo.datedeath.day
		);
		switch(ST){
		case BIRTH: Search(Root,BIRTH,KeyInfo,RL,&counter);break;
		case DEATH: Search(Root,DEATH,KeyInfo,RL,&counter);
		}
	}
	else{
		Info sex = male;
		printf("1.male 2.female:");
		scanf("%d",&KeyInfo.sex);
		Search(Root,SEX,KeyInfo,RL,&counter);
	}
}
*/