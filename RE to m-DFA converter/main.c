
/*
Formal Languages and Automata
CS322 Fall 2015
project 2
Regular expression to m-DFA converter
Author : Sangguk Lee
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define BUFSIZE 10000
//#define READ 1  //  1로 설정하면 콘솔 입력, 0으로 설정하면 "input.txt"로 입력

FILE *readfp;

char buf[BUFSIZE];

void getbuf(){
    fgets(buf,BUFSIZE,readfp);
    if(readfp!=stdin)
        printf("%s",buf);
    if(buf[strlen(buf)-1]==10)
        buf[strlen(buf)-1]=0;
}

int getint(){
    getbuf();
    int i=strlen(buf)-1,n=0,t=1;
    if(i==-1)
        return -1;
    for(;i>=0;i--){
        if(buf[i]<48 || buf[i]>57){
            printf("0 이상의 정수 숫자만 입력하여주세요.\n");
            return -1;
        }
        n+=(buf[i]-48)*t;
        t*=10;
    }
    return n;
}

int getsint(char* query){
    printf("%s\n",query);
    int n;
    do{
        printf(">> ");
        n=getint();
    }while(n==-1);
    return n;
}

typedef struct _state{
    char* name;
    int accepting;
    struct _state* other;
} state;

typedef struct _statelist{
    state* state;
    struct _statelist* nextstate;
} statelist;

int eqstatelist(statelist* a, statelist* b){
    statelist *templist, *templist2;
    for(templist=a; templist!=NULL; templist=templist->nextstate){
        for(templist2=b; templist2!=NULL; templist2=templist2->nextstate){
            if(templist->state==templist2->state)
                break;
        }
        if(templist2==NULL)
            return 0;
    }
    for(templist=b; templist!=NULL; templist=templist->nextstate){
        for(templist2=a; templist2!=NULL; templist2=templist2->nextstate){
            if(templist->state==templist2->state)
                break;
        }
        if(templist2==NULL)
            return 0;
    }
    return 1;
}

typedef struct _state_DFA{
    statelist* element;
    int accepting;
    struct _state_DFA* other;
} state_DFA;

typedef struct _input{
    char *name;
    struct _input* other;
} input;

typedef struct _function{
    state_DFA *currentstate,*nextstate;
    input* input;
    struct _function* otherstate;
    struct _function* otherinput;
} function;

typedef struct _efunction{
    state* currentstate;
    int nextstatenum;
    state** nextstate;
    input* input;
    int epsilon;   //  1 for epsilon input , 0 for other input
    struct _efunction* otherstate;
    struct _efunction* otherinput;
} efunction;

typedef struct _DFA{
    state_DFA* Q;
    input* S;
    function* d;
    state_DFA* q0;
} DFA;

typedef struct _MDFA{
    int statenum;
    input* S;
    int inputnum;
    int** function; //  state * input(by orer) -> state
    int q0;
    int* accepting;
} MDFA;

typedef struct _eNFA{
    state* Q;
    input* S;
    efunction* d;
    state* q0;
} eNFA;

int getstateorder(eNFA* N, state* Q){
    int n=0;
    state* tempQ=N->Q;
    while(tempQ!=Q){
        tempQ=tempQ->other;
        n++;
    }
    //printf("\norder2\n");
    return n;
}

int getstatenum(eNFA* N){
    return getstateorder(N,NULL);
}

int getstateorder_DFA(DFA* N, state_DFA* Q){
    int n=0;
    state_DFA* tempQ=N->Q;
    while(tempQ!=Q){
        tempQ=tempQ->other;
        n++;
    }
    return n;
}

int getstatenum_DFA(DFA* N){
    return getstateorder_DFA(N,NULL);
}

state* findstate_eNFA(eNFA* D, char* name){
    state* temp=D->Q;
    if(temp==NULL)
        return NULL;
    while(strcmp(temp->name,name)){
        temp=temp->other;
        if(temp==NULL)
            return NULL;
    }
    return temp;
}

state_DFA* findstate(DFA* D, statelist* list){
    state_DFA* temp=D->Q;
    if(temp==NULL)
        return NULL;
    while(!eqstatelist(temp->element,list)){
        temp=temp->other;
        if(temp==NULL)
            return NULL;
    }
    return temp;
}

state_DFA* findstatebyorder(DFA* D, int order){
    state_DFA* trav=D->Q;
    int i;
    for(i=0; i<order; i++)
        trav=trav->other;
    return trav;
}

function* findfunction(DFA* D, state_DFA* currentstate, char* symbol){
    function* trav=D->d;
    while(trav->currentstate!=currentstate){
        trav=trav->otherstate;
        if(trav==NULL)
            return NULL;
    }
    while(strcmp(trav->input->name,symbol)){
        trav=trav->otherinput;
        if(trav==NULL)
            return NULL;
    }
    return trav;
}

efunction* findfunction_eNFA(eNFA* D, state* currentstate, int epsilon, input* input){
    efunction* trav=D->d;
    while(trav->currentstate!=currentstate){
        trav=trav->otherstate;
        if(trav==NULL)
            return NULL;
    }
    if(epsilon){
        while(trav->epsilon!=1){
            trav=trav->otherinput;
            if(trav==NULL)
                return NULL;
        }
        return trav;
    }
    while(trav->input!=input){
        trav=trav->otherinput;
        if(trav==NULL)
            return NULL;
    }
    return trav;
}

statelist* setfunction_eNFA(eNFA* D, statelist* currentstate, input* input){
    statelist *trav=currentstate, *list=NULL, *temp;
    int i;
    char* check=(char*)calloc(getstatenum(D)*sizeof(char),sizeof(char));
    while(trav!=NULL){
        efunction* tempfunc=findfunction_eNFA(D,trav->state,0,input);
        for(i=0;i<tempfunc->nextstatenum;i++){
            check[getstateorder(D,tempfunc->nextstate[i])]=1;
        }
        trav=trav->nextstate;
    }
    state* tempQ=D->Q;
    for(i=0;i<getstatenum(D);i++){
        if(i>0)
            tempQ=tempQ->other;
        if(check[i]){
            temp=(statelist*)malloc(sizeof(statelist));
            temp->state=tempQ;
            temp->nextstate=list;
            list=temp;
        }
    }
    return list;
}

input* findinput(eNFA* N, char* x){
    input* trav=N->S;
    while(trav!=NULL){
        if(!strcmp(trav->name,x))
            return trav;
        trav=trav->other;
    }
    return trav;
}

void e_closure_help(eNFA* N, state* Q, char* visited){
    visited[getstateorder(N,Q)]=1;
    efunction* efunc=findfunction_eNFA(N,Q,1,NULL);
    int i;
    for(i=0; i<efunc->nextstatenum; i++){
        if(!visited[getstateorder(N,efunc->nextstate[i])])
            e_closure_help(N,efunc->nextstate[i],visited);
    }
}

statelist* e_closure(eNFA* N, state* Q){
    int i, statenum=getstatenum(N);
    char* visited=(char*)calloc(statenum*sizeof(char),sizeof(char));
    e_closure_help(N,Q,visited);
    statelist* list=(statelist*)malloc(sizeof(statelist));
    list->state=Q;
    list->nextstate=NULL;
    statelist* templist=list;
    state* tempQ=N->Q;
    for(i=0;i<statenum;i++){
        if(i>0)
            tempQ=tempQ->other;
        if(i==getstateorder(N,Q))
            continue;
        if(visited[i]){
            templist->nextstate=(statelist*)malloc(sizeof(statelist));
            templist=templist->nextstate;
            templist->state=tempQ;
            templist->nextstate=NULL;
        }
    }
    return list;
}

void printstatelist(statelist* state){
    printf("[");
    statelist* tempstate=state;
    if(tempstate!=NULL){
        printf("%s",tempstate->state->name);
        tempstate=tempstate->nextstate;
    }
    while(tempstate!=NULL){
        printf(", %s",tempstate->state->name);
        tempstate=tempstate->nextstate;
    }
    printf("]");
}

void printstate_DFA(state_DFA* state){
    printstatelist(state->element);
}

statelist* set_e_closure(eNFA* N, statelist* from){
    statelist *trav=from, *list=NULL, *temp;
    int i;
    char* check=(char*)calloc(getstatenum(N)*sizeof(char),sizeof(char));
    while(trav!=NULL){
        statelist* templist=e_closure(N,trav->state);
        for(; templist!=NULL; templist=templist->nextstate){
            getstateorder(N,templist->state);
            check[getstateorder(N,templist->state)]=1;
        }
        trav=trav->nextstate;
    }
    state* tempQ=N->Q;
    for(i=0;i<getstatenum(N);i++){
        if(i>0)
            tempQ=tempQ->other;
        if(check[i]){
            temp=(statelist*)malloc(sizeof(statelist));
            temp->state=tempQ;
            temp->nextstate=list;
            list=temp;
        }
    }
    return list;
}

DFA* createDFA(){
    DFA* D=(DFA*)malloc(sizeof(DFA));
    D->Q=NULL;
    D->S=NULL;
    D->d=NULL;
    D->q0=NULL;
    return D;
}

eNFA* createeNFA(){
    eNFA* D=(eNFA*)malloc(sizeof(eNFA));
    D->Q=NULL;
    D->S=NULL;
    D->d=NULL;
    D->q0=NULL;
    return D;
}

void addstate(DFA* D, statelist* list){
    if(D->Q==NULL){
        D->Q=(state_DFA*)malloc(sizeof(state_DFA));
        D->Q->element=list;
        D->Q->other=NULL;
        D->Q->accepting=0;
        return;
    }
    state_DFA* trav=D->Q;
    while(trav->other!=NULL)
        trav=trav->other;
    trav->other=(state_DFA*)malloc(sizeof(state_DFA));
    trav->other->element=list;
    trav->other->other=NULL;
    trav->other->accepting=0;
}

int addstate_eNFA(eNFA* D, char* name){
    if(D->Q==NULL){
        D->Q=(state*)malloc(sizeof(state));
        D->Q->name=(char*)malloc(strlen(name)+1);
        strcpy(D->Q->name,name);
        D->Q->other=NULL;
        D->Q->accepting=0;
        return 1;
    }
    state* trav=D->Q;
    int i=1;
    if(!strcmp(trav->name,name)){
        printf("State 1이 그 이름을 이미 가지고 있습니다. 다른 이름을 사용하세요.\n");
        return 0;
    }
    while(trav->other!=NULL){
        trav=trav->other;
        i++;
        if(!strcmp(trav->name,name)){
            printf("State %d이 그 이름을 이미 가지고 있습니다. 다른 이름을 사용하세요.\n",i);
            return 0;
        }
    }
    trav->other=(state*)malloc(sizeof(state));
    trav->other->name=(char*)malloc(strlen(name)+1);
    strcpy(trav->other->name,name);
    trav->other->other=NULL;
    trav->other->accepting=0;
    return 1;
}

int addinput_eNFA(eNFA* D, char* name){
    if(D->S==NULL){
        D->S=(input*)malloc(sizeof(input));
        D->S->name=(char*)malloc(strlen(name)+1);
        strcpy(D->S->name,name);
        D->S->other=NULL;
        return 1;
    }
    input* trav=D->S;
    int i=1;
    if(!strcmp(trav->name,name))    //  이미 존재
        return 0;
    while(trav->other!=NULL){
        trav=trav->other;
        i++;
        if(!strcmp(trav->name,name))    //  이미 존재
            return 0;
    }
    trav->other=(input*)malloc(sizeof(input));
    trav->other->name=(char*)malloc(strlen(name)+1);
    strcpy(trav->other->name,name);
    trav->other->other=NULL;
    return 1;
}

DFA* eNFAtoDFA(eNFA* N){
    DFA* D=(DFA*)malloc(sizeof(DFA));
    D->Q=NULL;
    addstate(D,e_closure(N,N->q0));
    D->S=N->S;  //  input symbols은 같음
    state_DFA* tempQ;   //  transition function
    D->d=NULL;
    input* tempS;
    function *tempd1=D->d, *tempd2, *pre1=NULL, *pre2;
    for(tempQ=D->Q ; tempQ!=NULL ; tempQ=tempQ->other){
        tempd1=(function*)malloc(sizeof(function));
        if(D->d==NULL)
            D->d=tempd1;
        if(pre1!=NULL)
            pre1->otherstate=tempd1;
        tempd2=tempd1;
        pre2=NULL;
        for(tempS=D->S ; tempS!=NULL ; tempS=tempS->other){
            if(tempd2==NULL)
                tempd2=(function*)malloc(sizeof(function));
            if(pre2!=NULL)
                pre2->otherinput=tempd2;
            tempd2->currentstate=tempQ;
            tempd2->input=tempS;
            statelist* templist=set_e_closure(N,setfunction_eNFA(N,tempQ->element,tempS));
            if(findstate(D,templist)==NULL)
                addstate(D,templist);
            tempd2->nextstate=(findstate(D,templist));
            tempd2->otherstate=NULL;
            tempd2->otherinput=NULL;
            pre2=tempd2;
            tempd2=tempd2->otherinput;
        }
        pre1=tempd1;
        tempd1=tempd1->otherstate;
    }
    D->q0=findstate(D,e_closure(N,N->q0));  //  initial state
    for(tempQ=D->Q; tempQ!=NULL; tempQ=tempQ->other){   //  final states
        statelist* trav;
        for(trav=tempQ->element; trav!=NULL; trav=trav->nextstate){
            if(trav->state->accepting){
                tempQ->accepting=1;
                break;
            }
        }
    }
    return D;
}

void reachable(DFA* D, state_DFA* currentQ, char* check, int turn){
    if(turn<0)
        return;
    if(check[getstateorder_DFA(D,currentQ)]==1)
        return;
    check[getstateorder_DFA(D,currentQ)]=1;
    input* trav=D->S;
    for(; trav!=NULL; trav=trav->other)
        reachable(D,findfunction(D,currentQ,trav->name)->nextstate, check, turn-1);
}

MDFA* minimalizeDFA(DFA* D){
    MDFA* M=(MDFA*)malloc(sizeof(MDFA));
    int statenum=getstatenum_DFA(D);
    char* check=(char*)calloc(statenum*sizeof(char),sizeof(char));
    reachable(D,D->q0,check,statenum);
    int i, j=0, k;
    for(i=statenum-1; i>=0; i--){
        if(!check[i])
            statenum--; //  unreached state를 제외한 statenum
    }
    char* link=(char*)malloc(statenum*sizeof(char));
    for(i=0; i<statenum; i++){
        while(!check[j])    //  if unreached
            j++;
        link[i]=j++;
    }
    char** table=(char**)malloc(statenum*sizeof(char*));    //  table filling algorithm에 쓰임 1 for distinguishable
    for(i=0; i<statenum; i++)  //  final state를 구별하는 basis
        table[i]=(char*)calloc(i*sizeof(char),sizeof(char));
    int finished=1;
    for(i=1; i<statenum; i++){  //  final state를 구별하는 basis
        for(j=0; j<i; j++){
            if(findstatebyorder(D,link[i])->accepting!=findstatebyorder(D,link[j])->accepting){
                table[i][j]=1;
                finished=0;
            }
        }
    }
    input* trav;
    while(!(finished++)){   //  table filling loop
        for(i=1; i<statenum; i++){
            for(j=0; j<i; j++){
                for(trav=D->S; trav!=NULL; trav=trav->other){
                    //printf("5");
                    int state1=getstateorder_DFA(D,findfunction(D,findstatebyorder(D,link[i]),trav->name)->nextstate);
                    int state2=getstateorder_DFA(D,findfunction(D,findstatebyorder(D,link[j]),trav->name)->nextstate);
                    int s1,s2;
                    for(s1=0; link[s1]!=state1; s1++);
                    for(s2=0; link[s2]!=state2; s2++);
                    if(s1>s2 && table[s1][s2] && table[i][j]==0){
                        table[i][j]=1;
                        finished=0;
                        break;
                    }
                    if(s2>s1 && table[s2][s1] && table[i][j]==0){
                        table[i][j]=1;
                        finished=0;
                        break;
                    }
                }
            }
        }
    }
    int* group=(int*)malloc(statenum*sizeof(int));
    for(i=0; i<statenum; i++)
        group[i]=i;
    for(finished=0; finished++==0; ){   //  table filling loop
        for(i=1; i<statenum; i++){
            for(j=0; j<i; j++){
                if(!table[i][j]){   //  if equivalent
                    for(k=0; k<statenum; k++){
                        if(group[k]==i)
                            group[k]=j;
                    }
                    finished=0;
                    table[i][j]=1;  //  알고리즘 최적화를 위해 distinguishable하다고 적어줌
                }
            }
        }
    }
    int min, max;
    for(i=0; i<statenum; i++){  //  group 정리
        min=statenum;
        for(j=0; j<statenum; j++){
            if(group[j]>=i && group[j]<min)
                min=group[j];
        }
        if(min==statenum)
            break;
        for(j=0; j<statenum; j++){
            if(group[j]==min)
                group[j]=i;
        }
        max=i;
    }
    M->statenum=max+1;
    M->S=D->S;
    M->inputnum=0;
    for(trav=M->S; trav!=NULL; trav=trav->other)
        M->inputnum++;
    M->function=(int**)malloc(M->statenum*sizeof(int*));
    for(i=0; i<M->statenum; i++)
        M->function[i]=(int*)malloc(M->inputnum*sizeof(int));
    for(i=0; i<M->statenum; i++){  //  transition function
        //printf("i = %d , statenum = %d \n",i,statenum);
        for(k=0; group[k]!=i; k++);
        trav=M->S;
        for(j=0; j<M->inputnum; j++){
            int state=getstateorder_DFA(D,findfunction(D,findstatebyorder(D,link[k]),trav->name)->nextstate), s;
            for(s=0; link[s]!=state; s++);
            M->function[i][j]=group[s];
            trav=trav->other;
        }
    }
    i=getstateorder_DFA(D,D->q0); //  initial state
    for(j=0; link[j]!=i; j++);
    M->q0=group[j];
    M->accepting=(int*)malloc(statenum*sizeof(int));
    for(i=0; i<M->statenum; i++){  //  final states
        for(j=0; group[j]!=i; j++);
        M->accepting[i]=findstatebyorder(D,link[j])->accepting;
    }
    return M;
}

void printMDFA(MDFA* D){
    FILE* outto=fopen("output.txt", "w");
    printf("\n#################mDFA#################\n");
    fprintf(outto, "#################mDFA#################\n");
    printf("States : [");
    fprintf(outto, "States : [");
    if(D->statenum>0){
        printf("0");
        fprintf(outto, "0");
    }
    int i,j;
    for(i=1; i<D->statenum; i++){
        printf(", %d",i);
        fprintf(outto, ", %d",i);
    }
    printf("]\nVocabulary : [");
    fprintf(outto, "]\nVocabulary : [");
    input* tempS=D->S;
    if(tempS!=NULL){
        printf("%s",tempS->name);
        fprintf(outto, "%s",tempS->name);
        tempS=tempS->other;
    }
    while(tempS!=NULL){
        printf(", %s",tempS->name);
        fprintf(outto, ", %s",tempS->name);
        tempS=tempS->other;
    }
    printf("]\nState Transition Functions :");
    fprintf(outto, "]\nState Transition Functions :");
    for(i=0; i<D->statenum; i++){
        input* trav=D->S;
        for(j=0; j<D->inputnum; j++){
            printf("\n(%d, '%s') => %d", i, trav->name, D->function[i][j]);
            fprintf(outto, "\n(%d, '%s') => %d", i, trav->name, D->function[i][j]);
            trav=trav->other;
        }
    }
    printf("\nInitial State : %d\nFinal State : [", D->q0);
    fprintf(outto, "\nInitial State : %d\nFinal State : [", D->q0);
    for(i=0; i<D->statenum; i++){
        if(D->accepting[i]){
            printf("%d",i++);
            fprintf(outto, "%d",i-1);
            break;
        }
    }
    for(; i<D->statenum; i++){
        if(D->accepting[i]){
            printf(", %d",i);
            fprintf(outto, ", %d",i);
        }
    }
    printf("]\n\n동일 경로 내의 output.txt에도 출력되었습니다.\n");
    fclose(outto);
}

int isinL(MDFA* D,char* x){
    int state=D->q0, i, j;
    input* trav;
    for(i=0;i<strlen(x);i++){
        char a[5]={0};
        a[0]=x[i];
        j=0;
        for(trav=D->S; trav!=NULL; trav=trav->other){
            if(!strcmp(trav->name, a))
                break;
            j++;
        }
        if(trav==NULL)
            return -1;
        state=D->function[state][j];
    }
    if(D->accepting[state])
        return 1;
    return 0;
}

void evaluation(MDFA* D){
    printf("확인할 문자열을 입력해주세요.\n>> ");
    getbuf();
    int n=isinL(D,buf);
    if(n==-1)
        printf("Input symbol이 아닌 문자가 포함되어있습니다.\n");
    else if(n==0)
        printf("아니요\n");
    else if(n==1)
        printf("네\n");
}

// 여기부터 본프로젝트 2에서 추가

char* PARS;

typedef struct _pre_eNFA{
    int start, end;
} peNFA;

peNFA* makepe(int start, int end){
    peNFA* ret=(peNFA*)malloc(sizeof(peNFA));
    ret->start=start;
    ret->end=end;
    return ret;
}

typedef struct _pestack{
    peNFA* pe;
    struct _pestack *up, *down;
} pestack;

pestack* stack;
pestack* petop;

void stack_init(){
    stack=(pestack*)malloc(sizeof(pestack));
    stack->pe=NULL;
    input* aa;
    stack->up=NULL;
    stack->down=NULL;
    petop=stack;
}

void push(peNFA* pe){
    petop->up=(pestack*)malloc(sizeof(pestack));
    petop->up->down=petop;
    petop=petop->up;
    petop->pe=pe;
    petop->up=NULL;
}

peNFA* pop(){
    peNFA* ret=petop->pe;
    petop=petop->down;
    free(petop->up);
    petop->up=NULL;
    return ret;
}

char* inttostr(int n){
    int b=0;
    char* ret=(char*)calloc(11*sizeof(char),sizeof(char));
    if(n==0){
        ret[0]=48;
        return ret;
    }
    while(n>0){
        ret[b++]=n%10+48;
        n/=10;
    }
    return ret;
}

eNFA* PARStoeNFA(char* post){
    stack_init();
    int statenum=0, i, n;
    char* input=(char*)calloc(sizeof(char)*2*strlen(post),sizeof(char));
    int* tostate=(int*)malloc(sizeof(int)*2*strlen(post));
    int* tostate2=(int*)malloc(sizeof(int)*2*strlen(post));
    for(i=0;i<2*strlen(post);i++){
        tostate[i]=-1;
        tostate2[i]=-1;
    }

    for(i=0; post[i]; i++){ //  stack을 이용하여 postfix인 PARS 결과로 간단한 eNFA 생성
        peNFA* newpe;
        if(post[i]==36 || (post[i]>=48 && post[i]<=57) || (post[i]>=65 && post[i]<=90) || (post[i]>=97 && post[i]<=122)){  //  symbols
            newpe=makepe(statenum, statenum+1);
            input[statenum]=post[i];
            tostate[statenum]=statenum+1;
            statenum+=2;
        }
        else if(post[i]=='*'){  //  closure
            peNFA *oldpe=pop();
            newpe=makepe(statenum,statenum+1);
            input[statenum]='$';
            tostate[statenum]=oldpe->start;
            tostate2[statenum]=statenum+1;
            if(!input[oldpe->end]){
                input[oldpe->end]='$';
                tostate[oldpe->end]=statenum+1;
            }
            else    //  oldpe->end state는 이미 input epsilon인 transition을 하나 가지고 있음
                tostate2[oldpe->end]=statenum+1;
            input[statenum+1]='$';
            tostate[statenum+1]=statenum;
            free(oldpe);
            statenum+=2;
        }
        else if(post[i]=='+'){  //  union
            peNFA *oldpe1=pop(), *oldpe2=pop();
            newpe=makepe(statenum,statenum+1);
            input[statenum]='$';
            tostate[statenum]=oldpe1->start;
            tostate2[statenum]=oldpe2->start;
            if(!input[oldpe1->end]){
                input[oldpe1->end]='$';
                tostate[oldpe1->end]=statenum+1;
            }
            else    //  oldpe1->end state는 이미 input epsilon인 transition을 하나 가지고 있음
                tostate2[oldpe1->end]=statenum+1;
            if(!input[oldpe2->end]){
                input[oldpe2->end]='$';
                tostate[oldpe2->end]=statenum+1;
            }
            else    //  oldpe2->end state는 이미 input epsilon인 transition을 하나 가지고 있음
                tostate2[oldpe2->end]=statenum+1;
            free(oldpe1);
            free(oldpe2);
            statenum+=2;
        }
        else if(post[i]==','){  //  concatenation
            peNFA *oldpe2=pop(), *oldpe1=pop();
            newpe=makepe(oldpe1->start,oldpe2->end);
            if(!input[oldpe1->end]){
                input[oldpe1->end]='$';
                tostate[oldpe1->end]=oldpe2->start;
            }
            else    //  oldpe1->end state는 이미 input epsilon인 transition을 하나 가지고 있음
                tostate2[oldpe1->end]=oldpe2->start;
            free(oldpe1);
            free(oldpe2);
        }
        else if(post[i]=='.')   //  parentheses
            continue;
        else{
            printf("parser error\n");
            exit(0);
        }
        push(newpe);
    }

    //  여기부터 본프로젝트 2-1에 쓰인 eNFA 형식으로 전환

    eNFA* D=createeNFA();
    for(i=0;i<statenum;i++)
        addstate_eNFA(D,inttostr(i));

    for(i=0;i<statenum;i++){
        if(input[i] && input[i]!='$'){
            char* in=(char*)calloc(2*sizeof(char),sizeof(char));
            in[0]=input[i];
            addinput_eNFA(D,in);
        }
    }
    state* tempQ;
    struct _input *tempS;
    efunction *tempd1=D->d, *tempd2, *pre1=NULL, *pre2;
    int curstate=0;
    for(tempQ=D->Q ; tempQ!=NULL ; tempQ=tempQ->other){
        tempd1=(efunction*)malloc(sizeof(efunction));
        if(D->d==NULL)
            D->d=tempd1;
        if(pre1!=NULL)
            pre1->otherstate=tempd1;
        tempd2=tempd1;
        tempd2->currentstate=tempQ;
        tempd2->input=findinput(D,"$");
        tempd2->otherstate=NULL;
        tempd2->otherinput=NULL;
        tempd2->epsilon=1;
        if(input[curstate]!='$')
            n=0;
        else if(tostate2[curstate]==-1)
            n=1;
        else
            n=2;
        tempd2->nextstatenum=n;
        if(n>0){
            tempd2->nextstate=(state**)malloc(n*sizeof(state*));
            tempd2->nextstate[0]=findstate_eNFA(D,inttostr(tostate[curstate]));
            if(n==2)
                tempd2->nextstate[1]=findstate_eNFA(D,inttostr(tostate2[curstate]));
        }
        else
            tempd2->nextstate=NULL;
        pre2=tempd2;
        for(tempS=D->S ; tempS!=NULL ; tempS=tempS->other){
            tempd2=(efunction*)malloc(sizeof(efunction));
            pre2->otherinput=tempd2;
            tempd2->currentstate=tempQ;
            tempd2->input=tempS;
            tempd2->otherstate=NULL;
            tempd2->otherinput=NULL;
            tempd2->epsilon=0;
            if(input[curstate]==tempS->name[0])
                n=1;
            else
                n=0;
            tempd2->nextstatenum=n;
            if(n==1){
                tempd2->nextstate=(state**)malloc(sizeof(state*));
                tempd2->nextstate[0]=findstate_eNFA(D,inttostr(tostate[curstate]));
            }
            else
                tempd2->nextstate=NULL;
            pre2=tempd2;
            tempd2=tempd2->otherinput;
        }
        pre1=tempd1;
        tempd1=tempd1->otherstate;
        curstate++;
    }
    peNFA* lastpe=pop();
    D->q0=findstate_eNFA(D,inttostr(lastpe->start));
    findstate_eNFA(D,inttostr(lastpe->end))->accepting=1;
    return D;
}

void evalRE(){
    printf("Regular expression을 입력해주세요.\n");
    yyparse();
    eNFA* N=PARStoeNFA(PARS);
    DFA* D=eNFAtoDFA(N);
    MDFA* M=minimalizeDFA(D);
    printMDFA(M);
    int i=1;
    printf("\nm-DFA의 Language에 포함되는지 확인할 문자열을 넣겠습니까? [y,n]\n");
    while(1){
        printf(">> ");
        getbuf();
        if(!strcmp(buf,"y"))
            break;
        else if(!strcmp(buf,"n")){
            i=0;
            break;
        }
        else if(buf[0]!=0){
            printf("'y' 또는 'n'를 입력해주세요.\n");
            printf("m-DFA에 문자열을 넣겠습니까? [y,n]\n");
        }
    }
    while(i){
        evaluation(M);
        printf("\n다른 문자열도 확인하겠습니까? [y,n]\n");
        while(1){
            printf(">> ");
            getbuf();
            if(!strcmp(buf,"y"))
                break;
            else if(!strcmp(buf,"n")){
                i=0;
                break;
            }
            else if(buf[0]!=0){
                printf("'y' 또는 'n'를 입력해주세요.\n");
                printf("다른 문자열도 확인하겠습니까? [y,n]\n");
            }
        }
    }
}

int main(){
    printf("Regular expression to m-DFA converter입니다.\n만든이 : KAIST 전산학부 14학번 이상국\n\n");
    readfp=stdin;
    PARS=(char*)calloc(sizeof(char)*BUFSIZE,sizeof(char));
    int i=1;
    while(i){
        evalRE();
        printf("\n다른 Regular expression도 변환하겠습니까? [y,n]\n");
        while(1){
            printf(">> ");
            getbuf();
            if(!strcmp(buf,"y")){
                PARS[0]=0;
                break;
            }
            else if(!strcmp(buf,"n")){
                i=0;
                break;
            }
            else if(buf[0]!=0){
                printf("'y' 또는 'n'를 입력해주세요.\n");
                printf("다른 Regular expression도 변환하겠습니까? [y,n]\n");
            }
        }
    }
    printf("\n엔터 키를 누르면 Regular expression to m-DFA converter를 종료합니다.\n");
    char a=getchar();
    return 0;
}
