typedef enum {OK,ERROR,TRUE,FALSE,OTHERS} Status;
typedef enum {SRCHINFO,NAME,PROFESSION,SEX,PARTER,
			  BIRTH,DEATH,DEL} OprtnType;
typedef enum {INFO,male,female} Info;
typedef enum {clr,quit,statistic,all,tree,info,addkid,load,
			  delmmbr,imprt,save,srch,mdf,error} Explnt;

//�ַ������ݽṹ��$��ʶ���һ���洢��Ԫ��length������$
typedef struct CharNode{
	char data;
	struct CharNode *next;
}CharNode,*P_Char;

typedef struct String{
	P_Char head;
	int length;
	struct String *next;
}String,*P_String;

//���������ݽṹ
typedef struct{//δ֪�����ڵ�ȫ������Ϊ0
	int year,month,day;
}Date;

typedef struct member{
	String name;
	Info sex;
	String parter,profession;
	Date birthdate,datedeath;
	int children;//��Ů��
	struct member *parent, 
				  *firstchild,
				  *nextsibling;
}Member,*P_member,*Root;

//����ջ�ṹ���ڽ����������Խ��Ѻõĸ�ʽ�������ʾ����
typedef struct Node{
	Status final;//�ж��Ƿ�Ϊ��˫�׵����һ�����ӽ��
	struct Node *next,*pre;
}Node,*NodePosition;

typedef struct Stack{
	int length;
	NodePosition bottom,top;
}Stack;

//���½ṹ���ڴ洢�����õ�ƥ�������Ϣ�ļ��������
typedef struct RsltNode{
	P_member data;
	struct RsltNode *next;
}RsltNode,*P_RsltNode;

typedef struct RsltList{
	OprtnType SrchType;
	int length;
	P_RsltNode head,tail;
	struct RsltList *next;
}RsltList,*P_RsltList;
/*
typedef struct PrsrvdList{
	int length;
	P_RsltList head,tail;
}PrsrvdList,*P_PrsrvdList;
*/