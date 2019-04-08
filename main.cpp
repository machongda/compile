#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define	norw 11            //norw-1���ؼ���(1->(norw-1)�Ǳ�����)
#define	al 20            //��Ĺؼ��ֵĳ���
#define ID norw //��
#define INT norw+1 //����
#define COMMA norw+2  //����
#define ENDF norw+3   //���
#define COLON norw+4  //ð��
#define SEMIC norw+5  //�ֺ�
#define ADD norw+6  //�Ӻ�
#define MINUS norw+7 //����
#define MULTI norw+8 //�˺�
#define EVALU norw+9 //��ֵ��
#define LE norw+10 //С�ڵ���
#define NE norw+11 //������
#define LT norw+12 //С��
#define EQ norw+13 //����
#define GE norw+14 //���ڵ���
#define GT norw+15 //����
#define DEVIDE norw+16//����
char TOKEN[20];                      //�ַ������������δ�ŵ��ʴ��ĵĸ����ַ�

extern int lookup(char *);           //��TOKEN�ַ����鱣���ֱ�
extern void report_error(char);      //��������еĴʷ�����
bool isalpha(char);                  //�жϽ����ַ��Ƿ�Ϊ��ĸ
bool isalnum(char);                  //�жϽ����ַ��Ƿ�Ϊ��ĸ��������
bool isdigit(char);                  //�жϽ����ַ��Ƿ�Ϊ����

FILE* fin;
FILE* fout;

void scanner()
{
    //�ʷ�������������򣬶�������ı��ļ����дʷ�����
    char ch;
    int i,c;
    int len;
    int error=0;                         //��¼�ļ��дʷ�����ĸ���
    ch=fgetc(fin);                        //�������ļ��ж�ȡһ���ַ�
    while(ch!=EOF)
    {
        //���������ļ����յ��ַ������ļ�������ʱ��ִ��ѭ��
        if(isalpha(ch))
        {
            //����������ļ����յĵ�һ���ַ�����ĸ
            TOKEN[0]=ch;
            ch=fgetc(fin);
            i=1;
            while(isalnum(ch))
            {
                TOKEN[i]=ch;
                i++;
                ch=fgetc(fin);
            }
            TOKEN[i]='\0';
            c=lookup(TOKEN);                   //�鱣���ֱ�
            if(c==0)
            {
                fprintf(fout,"(%d,%s)\n", ID,TOKEN);    //�����ʶ��
            }
            else
                fprintf(fout,"(%d,%s)\n", c,TOKEN);                //������յ���Ϊ������
        }

        if(isdigit(ch))                     //����������ļ����յĵ�һ���ַ�������
        {
            TOKEN[0]=ch;
            ch=fgetc(fin);
            i=1;
            while(isdigit(ch)||ch=='.')
            {
                //�ӵڶ��������ַ���ʼ����������ʱ��ִ��ѭ��������С���������ĸʱ����ѭ��
                if(ch=='.')
                {
                    TOKEN[i]=ch;
                    i++;
                    break;
                }

                TOKEN[i]=ch;
                i++;
                ch=fgetc(fin); //�ظ������ַ���ֱ�����յ������ֻ���С����

            }
            if(ch=='.') //������С����������ѭ����С����������������
            {
                ch=fgetc(fin);//ȡ��С������һ����
                while(isdigit(ch))
                {
                    //С��������������
                    TOKEN[i]=ch;
                    i++;
                    ch=fgetc(fin); //�ظ������ַ���ֱ�����յ�������
                }
            }
 printf("(%d,%s)\n", INT, TOKEN);
            if(isalpha(ch)||ch=='.')//���������ĸ��С����ͱ���
            {
                //�����������ĸ,error
                TOKEN[i]=ch;
                i++;
                TOKEN[i]='\0';
                printf("%s is error\n", TOKEN);
                error++;
            }
            else
            {



                fseek(fin,-1,1);//ָ�����,ĩβ���������
                TOKEN[i]='\0';
                i++;
                fprintf(fout,"(%d,%s)\n", INT, TOKEN);                      //������յ���Ϊ����
            }
        }
        else            //����������ļ����յĵ�һ���ַ��Ȳ�����ĸ�ֲ�������
            switch(ch)
            {
            //�������յ��ķ����ַ����з��࣬��ȡһ��һ��
            case':':
                ch=fgetc(fin);
                if(ch=='=')
                    fprintf(fout,"(%d,:=)\n", EVALU);           //������շ���Ϊ��ֵ��
                else
                {
                    fseek(fin,-1,1);                        //�ļ������ַ�����һ���ַ�
                    fprintf(fout,"(%d,':')\n", COLON);                     //���ð��
                }
                break;
            case',':
                fprintf(fout,"(%d,',')\n", COMMA);
                break;                   //�������
            case'.':
                fprintf(fout,"(%d,'.')\n", ENDF);
                break;                    //������
            case';':
                fprintf(fout,"(%d,'.')\n", SEMIC);
                break;                   //����ֺ�
            case'+':
                fprintf(fout,"(%d,'+')\n", ADD);
                break;                     //����Ӻ�
            case'-':
                fprintf(fout,"(%d,'-')\n", MINUS);
                break;                   //�������
            case'*':
                fprintf(fout,"(%d,'*')\n", MULTI);
                break;                   //����˺�
            case'<':
                ch=fgetc(fin);
                if(ch=='=')
                    fprintf(fout,"(%d,'<=')\n", LE);             //���С�ڻ���ں�
                else if(ch=='>')
                    fprintf(fout,"(%d,'<>')\n", NE);        //��������ں�
                else
                {
                    fseek(fin,-1,1);
                    fprintf(fout,"(%d,'<')\n", LT);;                 //���С�ں�
                }
                break;
            case'=':
                fprintf(fout,"(%d,'=')\n", EQ);
                break;                      //������ں�
            case'>':
                ch=fgetc(fin);
                if(ch=='=')
                    fprintf(fout,"(%d,'>=')\n", GE);             //������ڻ���ں�
                else
                {
                    fseek(fin,-1,1);
                    fprintf(fout,"(%d,'>')\n", GT);                     //������ں�
                }
                break;
            case'/':
                TOKEN[0]=ch;
                ch=fgetc(fin);
                if(ch=='/')
                {
                    TOKEN[1]='/';
                    TOKEN[2]='\0';
                    //fprintf(fout,"(%d,%s)\n", NOTE,TOKEN);
                    ch=fgetc(fin);
                    TOKEN[0]=ch;
                    i=1;
                    ch=fgetc(fin);
                    while(ch!='\n')
                    {
                        TOKEN[i]=ch;
                        i++;
                        ch=fgetc(fin);
                    }
                    TOKEN[i]='\0';
                    //fprintf(fout,"(%d,%s)\n",CONTENT,TOKEN);
                    break;
                }
                else
                {
                    fseek(fin,-1,1);
                    fprintf(fout,"(%d,'/')\n", DEVIDE);
                }
                break;
            case'{':
                TOKEN[0]=ch;
                ch=fgetc(fin);
                len=1;
                while(ch!='}')
                {
                    if(ch==EOF)
                        break;
                    ch=fgetc(fin);
                    len++;
                }
                if(ch=='}')
                {
                    break;
                }
                if(ch==EOF)
                {
                    TOKEN[1]='\0';
                    printf("%s is error\n",TOKEN);
                    error++;
                    fseek(fin,-len,1);
                    break;
                }
            case'}':
                TOKEN[0]=ch;
                TOKEN[1]='\0';
                error++;
                printf("%s is error\n",TOKEN);
                break;
            case' ':
                break;
            case'\n':
                break;
            case'\t':
                break;
            case EOF:
                break;
            default:
                printf("%c is error\n", ch);             //���շ������ַ����򱨸�ʷ�����
                error++;
                break;
            }
        ch=fgetc(fin);                                          //�������ļ��ж�ȡ��һ�����ʣ�ֱ���ļ�����
    }//whileѭ������
    printf("������%d���ʷ�����!",error);
    return;
}

int lookup(char *token)
{
    int j;
    char word[norw][al];
    strcpy(&(word[1][0]), "begin");
    strcpy(&(word[2][0]), "end");
    strcpy(&(word[3][0]), "var");
    strcpy(&(word[4][0]), "integer");
    strcpy(&(word[5][0]), "while");
    strcpy(&(word[6][0]), "do");
    strcpy(&(word[7][0]), "if");
    strcpy(&(word[8][0]), "then");
    strcpy(&(word[9][0]), "procedure");
    strcpy(&(word[10][0]), "else");

    for(j=1; j<=norw-1; j++)
        if(stricmp(token,word[j])==0)
            return j; //��TOKEN�ַ����鱣���ֱ����鵽���ر����������
    return 0;                                              //TOKEN���Ǳ����֣�����0
}


/*judge the type of the input char:alpha,num,digit*/
bool isalpha(char c)
{
    //�жϽ����ַ��Ƿ�Ϊ��ĸ
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
        return 1;
    else
        return 0;

}
bool isalnum(char c)
{
    //�жϽ����ַ��Ƿ�Ϊ��ĸ��������
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))
        return 1;
    else
        return 0;
}

bool isdigit(char c)
{
    //�жϽ����ַ��Ƿ�Ϊ����
    if(c>='0'&&c<='9')
        return 1;
    else
        return 0;
}


int main()
{
    char filename[20];
    printf("�������ļ���:");
    scanf("%s",filename);
    if((fin=fopen(filename,"r"))==NULL) //��Ҫ��ȡ���ı��ļ�
    {
        printf("���ܴ��ļ�.\n");
        exit(0);
    }

    printf("�����뱣�����������ļ���:");
    scanf("%s",filename);
    if((fout=fopen(filename,"w"))==NULL)
    {
        printf("���ܴ��ļ�.\n");
        exit(0);
    }

    scanner();    //���ôʷ���������
    getchar();
    getchar();

    fclose(fin);
    fclose(fout);
    return 0;
}
