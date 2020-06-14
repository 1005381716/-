typedef enum {OK,ERROR,TRUE,FALSE,OTHERS} Status;
typedef enum {SRCHINFO,NAME,PROFESSION,SEX,PARTER,
			  BIRTH,DEATH,DEL} OprtnType;
typedef enum {INFO,male,female} Info;
typedef enum {clr,quit,statistic,all,tree,info,addkid,load,
			  delmmbr,imprt,save,srch,mdf,error} Explnt;

//字符串数据结构，$标识最后一个存储单元，length不包括$
typedef struct CharNode{
	char data;
	struct CharNode *next;
}CharNode,*P_Char;

typedef struct String{
	P_Char head;
	int length;
	struct String *next;
}String,*P_String;

//家族树数据结构
typedef struct{//未知的日期的全部数据为0
	int year,month,day;
}Date;

typedef struct member{
	String name;
	Info sex;
	String parter,profession;
	Date birthdate,datedeath;
	int children;//子女数
	struct member *parent, 
				  *firstchild,
				  *nextsibling;
}Member,*P_member,*Root;

//以下栈结构用于将家族树“以较友好的格式输出（显示）”
typedef struct Node{
	Status final;//判断是否为其双亲的最后一个孩子结点
	struct Node *next,*pre;
}Node,*NodePosition;

typedef struct Stack{
	int length;
	NodePosition bottom,top;
}Stack;

//以下结构用于存储搜索得的匹配相关信息的家族树结点
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