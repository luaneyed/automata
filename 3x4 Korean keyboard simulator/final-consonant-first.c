/*
Formal Languages and Automata
CS322 Fall 2015
main project 3
3×4 한글자판을 위한 한글 받침우선 모아쓰기 simulator
Author : Sangguk Lee
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>

#define BUFSIZE 10000

typedef int state;
typedef struct _hangul{
    wchar_t letter; //  글자
    wchar_t cho;    //  초성
    wchar_t jung;   //  중성
    wchar_t jung1;  //  예비중성
    wchar_t jong;   //  종성
    wchar_t jong1;  //  부분종성1
    wchar_t jong2;  //  부분종성2
} hangul;
char buf[BUFSIZE];

void getbuf(){
    fgets(buf,BUFSIZE,stdin);
    if(strlen(buf)>0)
        buf[strlen(buf)-1]=0;
}

int combination(hangul* output, int k){
    output[k].letter=44032;
    int num=output[k].jung;
    if(num==0)
        num=output[k].jung1;
    num-=12623;
    if(num<0){
        output[k].letter=num=output[k].cho;
        return 0;
    }
    output[k].letter+=num*28;
    num=output[k].cho-12593;
    if(num>19)
        num-=1;
    if(num>15)
        num-=7;
    if(num>5)
        num-=2;
    if(num>2)
        num-=1;
    output[k].letter+=num*21*28;
    num=output[k].jong;
    if(num==0){
        num=output[k].jong1;
    }
    if(num>0)
        num-=12592;
    if(num>24)
        num-=1;
    if(num>18)
        num-=1;
    if(num>7)
        num-=1;
    output[k].letter+=num;
    return 1;
}

void print(hangul* output, int k){
    int i=0;
    for(;i<=k;i++)
        wprintf(L"%c",output[i].letter);
}

state automata(state* S, char uni, hangul* output, int* j, int* k){
    if(uni==';'){
        combination(output,*k);
        if(output[*k].letter!=0){
            output[*k].letter=0;
            output[*k].cho=0;
            output[*k].jung=0;
            output[*k].jung1=0;
            output[*k].jong=0;
            output[*k].jong1=0;
            output[*k].jong2=0;
        }
        else if(*k>0){
            (*k)--;
            output[*k].letter=0;
            output[*k].cho=0;
            output[*k].jung=0;
            output[*k].jung1=0;
            output[*k].jong=0;
            output[*k].jong1=0;
            output[*k].jong2=0;
        }
        return 0;
    }
    if(S[*j]==0){   //  initial state
        if(uni=='1'){   //  ㄱ
            output[*k].cho=12593;
            return 1;
        }
        if(uni=='2'){   //  ㄴ
            output[*k].cho=12596;
            return 2;
        }
        if(uni=='q'){   //  ㄹ
            output[*k].cho=12601;
            return 4;
        }
        if(uni=='w'){   //  ㅁ
            output[*k].cho=12609;
            return 2;
        }
        if(uni=='a'){   //  ㅅ
            output[*k].cho=12613;
            return 5;
        }
        if(uni=='s'){   //  ㅇ
            output[*k].cho=12615;
            return 6;
        }
        return -1;  //  잘못된 문자열
    }
    if(S[*j]==1){   //  ㄱ, ㄷ, ㅂ, ㅈ (획추가 한 번 가능, 쌍자음 가능)
        if(uni=='z'){  //  획추가
            if(output[*k].cho==12593)
                output[*k].cho=12619;
            else if(output[*k].cho==12599)
                output[*k].cho=12620;
            else if(output[*k].cho==12610)
                output[*k].cho=12621;
            else if(output[*k].cho==12616)
                output[*k].cho=12618;
            return 4;
        }
        if(uni=='c'){  //  쌍자음
            if(output[*k].cho==12593)
                output[*k].cho=12594;
            else if(output[*k].cho==12599)
                output[*k].cho=12600;
            else if(output[*k].cho==12610)
                output[*k].cho=12611;
            else if(output[*k].cho==12616)
                output[*k].cho=12617;
            return 4;
        }
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==2){   //  ㄴ, ㅁ
        if(uni=='z'){  //  획추가
            if(output[*k].cho==12596)
                output[*k].cho=12599;
            else if(output[*k].cho==12609)
                output[*k].cho=12610;
            return 1;
        }
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==3)   //  dead state
        return -1;  //  혹시라도 도달하면~
    if(S[*j]==4){   //  자음 무조건 끝
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==5){   //  ㅅ
        if(uni=='z'){  //  획추가
            output[*k].cho=12616;
            return 1;
        }
        if(uni=='c'){  //  쌍자음
            output[*k].cho=12614;
            return 4;
        }
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==6){   //  ㅇ
        if(uni=='z'){  //  획추가
            output[*k].cho=12622;
            return 1;
        }
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==7){   //  ㅏ
        if(uni=='1'){  //  ㄱ
            output[*k].jong1=12593;
            return 11;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong1=12596;
            return 12;
        }
        if(uni=='z'){  //  획추가
            output[*k].jung1=12625;
            return 10;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong1=12601;
            return 13;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12609;
            return 14;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong=12613;
            return 15;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12615;
            return 16;
        }
        if(uni=='3'){  //  ㅏㅓ
            output[*k].jung1=12627;
            return 17;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12624;
            output[*k].jung1=0;
            return 8;
        }
        return -1;
    }
    if(S[*j]==8){   //  모음 무조건 끝
        if(uni=='1'){  //  ㄱ
            output[*k].jong1=12593;
            return 11;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong1=12596;
            return 12;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong1=12601;
            return 13;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12609;
            return 14;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong=12613;
            return 15;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12615;
            return 16;
        }
        return -1;
    }
    if(S[*j]==9){   //  ㅗ
        if(uni=='1'){  //  ㄱ
            output[*k].jong1=12593;
            return 11;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong1=12596;
            return 12;
        }
        if(uni=='z'){  //  획추가
            output[*k].jung=12635;
            output[*k].jung1=0;
            return 8;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong1=12601;
            return 13;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12609;
            return 14;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong=12613;
            return 15;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12615;
            return 16;
        }
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12632;
            return 10;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12634;
            output[*k].jung1=0;
            return 8;
        }
        if(uni=='e'){  //  ㅗㅜ
            output[*k].jung1=12636;
            return 18;
        }
        return -1;
    }
    if(S[*j]==10){   // ㅡ, ㅑ, ㅕ, ㅝ, ㅘ (모음추가는 ㅣ만 가능)
        if(uni=='1'){  //  ㄱ
            output[*k].jong1=12593;
            return 11;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong1=12596;
            return 12;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong1=12601;
            return 13;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12609;
            return 14;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong=12613;
            return 15;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12615;
            return 16;
        }
        if(uni=='d'){  //  ㅣ
            if(output[*k].jung1==12641) //  ㅡ
                output[*k].jung=12642;
            else if(output[*k].jung1==12625) //  ㅑ
                output[*k].jung=12626;
            else if(output[*k].jung1==12629) //  ㅕ
                output[*k].jung=12630;
            else if(output[*k].jung1==12637) //  ㅝ
                output[*k].jung=12638;
            else if(output[*k].jung1==12632) //  ㅘ
                output[*k].jung=12633;
            output[*k].jung1=0;
            return 8;
        }
        return -1;
    }
    if(S[*j]==11){   // 초중 + ㄱ
        if(uni=='1'){  //  ㄱ
            output[*k].jong=12593;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong=12593;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong=12619;
            output[*k].jong1=0;
            return 19;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong=12593;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12593;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong2=12613;
            output[*k].jong=12595;
            return 20;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12593;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12615;
            return 6;
        }
        if(uni=='c'){  //  쌍자음
            output[*k].jong=12594;
            output[*k].jong1=0;
            return 19;
        }
        output[*k].jong1=0;
        combination(output,(*k)++);
        output[*k].cho=12593;
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==12){   // 초중 + ㄴ
        if(uni=='1'){  //  ㄱ
            output[*k].jong=12596;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong=12596;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong=12599;
            output[*k].jong1=0;
            return 21;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong=12596;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12596;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 22;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 23;
        }
        output[*k].jong1=0;
        combination(output,(*k)++);
        output[*k].cho=12596;
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==13){   // 초중 + ㄹ
        if(uni=='1'){  //  ㄱ
            output[*k].jong2=12593;
            output[*k].jong=12602;
            return 24;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 25;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong=12601;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong2=12609;
            output[*k].jong=12603;
            return 26;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong2=12613;
            output[*k].jong=12605;
            return 20;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 23;
        }
        output[*k].jong1=0;
        combination(output,(*k)++);
        output[*k].cho=12601;
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==14){   // 초중 + ㅁ
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong1=12610;
            output[*k].jong=0;
            return 27;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        output[*k].jong=0;
        combination(output,(*k)++);
        output[*k].cho=12609;
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==15){   // 초중 + ㅅ
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong=12616;
            return 21;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        if(uni=='c'){  //  쌍자음
            output[*k].jong=12614;
            return 19;
        }
        output[*k].jong=0;
        combination(output,(*k)++);
        output[*k].cho=12613;
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==16){   // 초중 + ㅇ
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong=12622;
            return 19;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        output[*k].jong=0;
        combination(output,(*k)++);
        output[*k].cho=12615;
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==17){   //  ㅓ
        if(uni=='1'){  //  ㄱ
            output[*k].jong1=12593;
            return 11;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong1=12596;
            return 12;
        }
        if(uni=='z'){  //  획추가
            output[*k].jung1=12629;
            return 10;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong1=12601;
            return 13;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12609;
            return 14;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong=12613;
            return 15;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12615;
            return 16;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12628;
            output[*k].jung1=0;
            return 8;
        }
        return -1;
    }
    if(S[*j]==18){   //  ㅜ
        if(uni=='1'){  //  ㄱ
            output[*k].jong1=12593;
            return 11;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong1=12596;
            return 12;
        }
        if(uni=='z'){  //  획추가
            output[*k].jung=12640;
            output[*k].jung1=0;
            return 8;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong1=12601;
            return 13;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12609;
            return 14;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong=12613;
            return 15;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12615;
            return 16;
        }
        if(uni=='3'){  //  ㅏㅓ
            output[*k].jung1=12637;
            return 10;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12639;
            output[*k].jung1=0;
            return 8;
        }
        return -1;
    }
    if(S[*j]==19){   // 초중 + (받침 추가 불가, 획추가 or 쌍자음 불가)
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        if(output[*k].jong==12607)   //  ㄿ
            output[++(*k)].cho=12621;
        else if(output[*k].jong==12606)  //  ㄾ
            output[++(*k)].cho=12620;
        else if(output[*k].jong==12598)  //  ㄶ
            output[++(*k)].cho=12622;
        else if(output[*k].jong==12608)  //  ㅀ
            output[++(*k)].cho=12622;
        else
            output[++(*k)].cho=output[*k-1].jong;
        output[*k-1].jong2=0;
        output[*k-1].jong=output[*k-1].jong1;
        output[*k-1].jong1=0;
        combination(output,*k-1);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==20){   // 초중 + ㄳ, ㄽ, ㅄ
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong2=0;
            output[*k].jong=output[*k].jong1;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12616;
            return 1;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        if(uni=='c'){  //   쌍자음
            output[*k].jong2=0;
            output[*k].jong=output[*k].jong1;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12614;
            return 4;
        }
        output[++(*k)].cho=12613;
        output[*k-1].jong2=0;
        output[*k-1].jong=output[*k-1].jong1;
        output[*k-1].jong1=0;
        combination(output,*k-1);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==21){   // 초중 + ㄷ, ㅈ, ㄼ
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            if(output[*k].jong==12599)   //  ㄷ
                output[*k].jong=12620;
            else if(output[*k].jong==12616)   //  ㅈ
                output[*k].jong=12618;
            else if(output[*k].jong==12604){   //  ㄼ
                output[*k].jong2=12621;
                output[*k].jong=12607;
            }
            return 19;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        if(uni=='c'){  //   쌍자음
            if(output[*k].jong==12599){   //  ㄷ
                output[*k].jong=0;
                combination(output,*k);
                output[++(*k)].cho=12600;
            }
            else if(output[*k].jong==12616){   //  ㅈ
                output[*k].jong=0;
                combination(output,*k);
                output[++(*k)].cho=12617;
            }
            else if(output[*k].jong==12604){   //  ㄼ
                output[*k].jong1=0;
                output[*k].jong2=0;
                output[*k].jong=12601;
                combination(output,*k);
                output[++(*k)].cho=12611;
            }
            return 4;
        }
        if(output[*k].jong==12604)   //  ㄼ
            output[++(*k)].cho=12610;
        else
            output[++(*k)].cho=output[*k-1].jong;
        output[*k-1].jong2=0;
        output[*k-1].jong=output[*k-1].jong1;
        output[*k-1].jong1=0;
        combination(output,*k-1);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==22){   // 초중 + ㄴㅅ
        if(uni=='z'){  //  획추가
            output[*k].cho=0;
            combination(output,(*k)--);
            output[*k].jong2=12616;
            output[*k].jong=12597;
            return 24;
        }
        output[*k-1].jong=12596;
        output[*k-1].jong1=0;
        combination(output,*k-1);
        if(uni=='c'){  //   쌍자음
            output[*k].cho=12614;
            return 4;
        }
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==23){   // 초중 + ㄴㅇ, ㄹㅇ
        if(uni=='z'){  //  획추가
            output[*k].cho=0;
            combination(output,(*k)--);
            output[*k].jong2=12622;
            if(output[*k].jong1==12596)
                output[*k].jong=12598;
            else if(output[*k].jong1==12601)
                output[*k].jong=12608;
            return 19;
        }
        output[*k-1].jong=output[*k-1].jong1;
        output[*k-1].jong1=0;
        combination(output,*k-1);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==24){   // 초중 + ㄺ, ㄵ
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            if(output[*k].jong==12602)   //  ㄺ
                output[*k+1].cho=12619;
            else if(output[*k].jong==12597)   //  ㄵ
                output[*k+1].cho=12618;
            output[*k].jong2=0;
            output[*k].jong=output[*k].jong1;
            output[*k].jong1=0;
            combination(output,(*k)++);
            return 4;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        if(uni=='c'){  //  쌍자음
            if(output[*k].jong==12602)   //  ㄺ
                output[*k+1].cho=12594;
            else if(output[*k].jong==12597)   //  ㄵ
                output[*k+1].cho=12617;
            output[*k].jong2=0;
            output[*k].jong=output[*k].jong1;
            output[*k].jong1=0;
            combination(output,(*k)++);
            return 4;
        }
        output[*k+1].cho=output[*k].jong2;
        output[*k].jong2=0;
        output[*k].jong=output[*k].jong1;
        output[*k].jong1=0;
        combination(output,(*k)++);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==25){   // 초중 + ㄹㄴ
        if(uni=='z'){  //  획추가
            output[*k].cho=12599;
            return 28;
        }
        output[*k-1].jong=output[*k-1].jong1;
        output[*k-1].jong1=0;
        combination(output,*k-1);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==26){   // 초중 + ㄻ
        if(uni=='1'){  //  ㄱ
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong2=12610;
            output[*k].jong=12604;
            return 21;
        }
        if(uni=='q'){  //  ㄹ
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[++(*k)].cho=12613;
            return 5;
        }
        if(uni=='s'){  //  ㅇ
            output[++(*k)].cho=12615;
            return 6;
        }
        output[*k+1].cho=12609;
        output[*k].jong2=0;
        output[*k].jong=12601;
        output[*k].jong1=0;
        combination(output,(*k)++);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==27){   // 초중 + ㅂ
        if(uni=='1'){  //  ㄱ
            output[*k].jong=12610;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12593;
            return 1;
        }
        if(uni=='2'){  //  ㄴ
            output[*k].jong=12610;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12596;
            return 2;
        }
        if(uni=='z'){  //  획추가
            output[*k].jong=12621;
            output[*k].jong1=0;
            return 21;
        }
        if(uni=='q'){  //  ㄹ
            output[*k].jong=12610;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12601;
            return 4;
        }
        if(uni=='w'){  //  ㅁ
            output[*k].jong=12610;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12609;
            return 2;
        }
        if(uni=='a'){  //  ㅅ
            output[*k].jong2=12613;
            output[*k].jong=12612;
            return 20;
        }
        if(uni=='s'){  //  ㅇ
            output[*k].jong=12610;
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12615;
            return 6;
        }
        if(uni=='c'){  //  쌍자음
            output[*k].jong1=0;
            combination(output,*k);
            output[++(*k)].cho=12611;
            return 4;
        }
        output[*k].jong1=0;
        combination(output,(*k)++);
        output[*k].cho=12610;
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
    if(S[*j]==28){   // 초중 + ㄹㄷ
        if(uni=='z'){  //  획추가
            output[*k].cho=0;
            combination(output,(*k)--);
            output[*k].jong2=12620;
            output[*k].jong=12606;
            return 19;
        }
        if(uni=='c'){  //  쌍자음
            output[*k-1].jong=output[*k-1].jong1;
            output[*k-1].jong1=0;
            combination(output,*k-1);
            output[*k].cho=12600;
            return 4;
        }
        output[*k-1].jong=output[*k-1].jong1;
        output[*k-1].jong1=0;
        combination(output,*k-1);
        if(uni=='3'){  //  ㅏ
            output[*k].jung1=12623;
            return 7;
        }
        if(uni=='d'){  //  ㅣ
            output[*k].jung=12643;
            return 8;
        }
        if(uni=='e'){  //  ㅗ
            output[*k].jung1=12631;
            return 9;
        }
        if(uni=='x'){  //  ㅡ
            output[*k].jung1=12641;
            return 10;
        }
        return -1;
    }
}

int getinput(){
    getbuf();
    int i=-1,j=0,k=0;    //  j는 output, k는 S의 argument
    hangul* output=(hangul*)malloc(strlen(buf)*sizeof(hangul));
    state* S=(state*)malloc((strlen(buf)+1)*sizeof(state));
    for(i=0;i<strlen(buf);i++){
        output[i].letter=0;
        output[i].cho=0;
        output[i].jung=0;
        output[i].jung1=0;
        output[i].jong=0;
        output[i].jong1=0;
        output[i].jong2=0;
        S[i]=0;
    }
    for(i=0;i<strlen(buf);i++){
        state s=automata(S,buf[i],output,&j,&k);
        combination(output,k);
        print(output,k);
        printf(" ");
        if(s==-1)
            return 2;   //  완벽하지 않은 문자열
        if(buf[i]==';'){
            if(j>0)
                j--;
            S[j]=s;
        }
        else
            S[++j]=s;
    }
    free(output);
    return 0;
}

int again(int i){
    if(i)
        printf("'y' 또는 'n'를 입력해주세요.\n>> ");
    getbuf();
    if(!strcmp(buf,"y"))
        return 1;
    if(!strcmp(buf,"n"))
        return 0;
    again(1);
}

int main(void){
    _wsetlocale(LC_ALL, L"Korean");
    printf("3×4 한글자판을 위한 한글 받침우선 모아쓰기 simulator입니다.\n만든이 : KAIST 전산학부 14학번 이상국\n");
    int i;
    do{
        printf("문자열을 입력해주세요.\n>> ");
        i=getinput();
        if(i==1){
            printf("\n한글이나 backspace를 나타내지 않는 문자가 포함되어 있습니다. 다시 입력하겠습니까?[y/n]\n>> ");
            if(again(0)==1)
                i=0;
        }
        else if(i==2){
            printf("\n올바르지 않은 문자열입니다. 다시 입력하겠습니까?[y/n]\n>> ");
            if(again(0)==1)
                i=0;
        }
        else{
            printf("\n더 입력할 문자열이 있습니까?[y/n]\n>> ");
            if(again(0)==0)
                i=1;
        }
    }while(!i);
    return 0;
}
