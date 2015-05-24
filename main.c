#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>

//全局变量
int check_login_success = 0;//在标题显示用户名称会用到
char user_name[20];//在标题显示用户名称会用到

//定义系统用户信息结构
struct sysuser
{
    char username[20];
    char password[8];
};

//定义图书信息结构
struct book
{
    int number;     //编号
    char name[30];  //书名
    char category[10];  //类别
    int lent;       //是否借出（已借为1，未借为0）
    int year;       //借出时间——年
    int month;      //借出时间——月
    int day;        //借出时间——日
    struct book *next;
};


void save_to_file(FILE *fp, struct book *head);
struct book *search_by_number(struct book *head, int number);
int search_by_category(struct book *head, char *category);

//主标题
//测试成功
void print_main_title()
{
    time_t time_login;
    struct tm *p;
    time(&time_login);
    p = gmtime(&time_login);

    if(check_login_success == 0)
    {
        printf("--------------------------------------------------------------------------------");
        printf("欢迎您，亲爱的用户！\n");
        printf("                                                                                ");
        printf("                           欢迎使用图书管理系统v2.2                             ");
        printf("                                                                                ");
        printf("                        现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
        printf("                                                                                ");
        printf("                                                                                ");
        printf("--------------------------------------------------------------------------------");
    }
    else
    {
        printf("--------------------------------------------------------------------------------");
        printf("欢迎您，%s！\n",user_name);
        printf("                                                                                ");
        printf("                           欢迎使用图书管理系统v2.2                             ");
        printf("                                                                                ");
        printf("                        现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year, p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
        printf("                                                                                ");
        printf("                                                                                ");
        printf("--------------------------------------------------------------------------------");
    }

}

struct book *insert_list(struct book *head, struct book *element)
{
    struct book *p;
    p = head;
    while(p -> next != NULL)
        p = p -> next;
    p -> next = element;
    element -> next = NULL;
    return head;
};

//注册账号
//测试通过！不要修改
int create_user()
{
    struct sysuser su;
    FILE *fp;
    fp = fopen("user.txt", "a");
    if (fp == NULL) fopen("user.txt", "w");
create_loop:
    system("CLS");
    //print_main_title();
    printf("\n");
    printf("===================================注册新用户===================================");
    printf("\n");
    printf("                                    请注意：\n");
    printf("    账号的长度为20位，可使用英文，汉字或数字，一个汉字字符为两位，不允许空格\n");
    printf("         密码的长度为8位，仅允许使用英文字符或数字，不允许空格\n\n");
    printf("                  为保护您的隐私，输入的密码不会显示在屏幕上\n\n");
    printf("                  请输入新的账号，按回车结束：");
    scanf("%s", su.username);
    printf("\n");
    printf("                  请输入新的密码，按回车结束：");
    //scanf("%s", su.password);
    int i=0;
    while(0 < i < 8&&(su.password[i]=getch())!='\r')
    {
        if(su.password[i]=='\b')
        {
            if(i <= 0);
            else i--;
        }
        else
        {
            i++;
            if (i > 8)
            {
                printf("\n             注意：已经达到8位字符上限，本次键入的字符将无效！\n");
                i = i - 1;
            }
        }
    }
    su.password[i]='\0';

    i = 0;
    char check_password[8];
    for(i = 0; i < 8; i++)
        check_password[i] = '\0';
    i = 0;
    printf("\n\n                  请再输入一次密码，按回车结束：");
    while(0 < i < 8&&(check_password[i]=getch())!='\r')
    {
        if(check_password[i]=='\b')
        {
            if(i <= 0);
            else i--;
        }
        else
        {
            i++;
            if (i > 8)
            {
                printf("\n             注意：已经达到8位字符上限，本次键入的字符将无效！\n");
                i = i - 1;
            }
        }
    }
    check_password[i]='\0';
    int check = 1;
    if (strcmp(check_password, su.password) == 0);
    else
    {
        printf("\n\n                  两次密码输入不一致！请重新注册...\n");
        Sleep(2000);
        goto create_loop;
    }

    fprintf(fp, "%s %s", su.username, su.password);
    fprintf(fp,"\n");
    fclose(fp);
    printf("\n\n");
    printf("                  注册成功！\n\n");
    printf("                  请记住您的用户名：%s\n", su.username);
    printf("                  密码：********\n");
    printf("                  按任意键返回登录菜单...");
    char ch;
    ch = getch();
    system("cls");
    return 1;
}

//定义登录
//测试通过！不要修改
int login()
{
    struct sysuser su;
    char ch;
    int check = 0;
    system("CLS");
    print_main_title();
    printf("\n\n");
    printf("                       请输入账号，按回车键结束：");
    scanf("%s", su.username);
    printf("\n");
    printf("                       请输入密码，按回车键结束：\n");
    printf("                       该密码不会显示在屏幕上:");
    //scanf("%s", su.password);
    int i=0;
    while(0 < i < 8&&(su.password[i]=getch())!='\r')
    {
        if(su.password[i]=='\b')
        {
            if(i <= 0);
            else i--;
        }
        else
        {
            i++;
            if (i > 8)
            {
                printf("\n             注意：已经达到8位字符上限，本次键入的字符将无效！\n");
                i = i - 1;
            }
        }
    }
    su.password[i]='\0';

    if(checkUserValid(&su) == 1)
    {
        strcpy(user_name, su.username);
        check = 1;
        printf("\n\n\n");
        printf("                       登录成功！\n");
        printf("\n");
        printf("                       按任意键进入主菜单...");
        ch = getch();
        system("cls");
    }

    else
    {
        printf("\n\n\n");
        printf("                       该用户不存在或密码错误，请核对后重新登录或注册！\n");
        printf("                       如需帮助，请与管理员联系\n");
        printf("                       按任意键返回主菜单...");
        ch = getch();
        system("cls");
    }

    return check;
}

//定义检查用户合法性
//测试通过！不要修改
int checkUserValid(struct sysuser *user)
{

    //打开存储用户账号密码的文件
    FILE *user_file_p;

    user_file_p = fopen("user.txt","r");
    if (user_file_p == NULL) user_file_p = fopen("user.txt","w");

    char usr[30], usr_f[30], pwd[10];

    int check = 0;

    //连接生成准备与文件校验的用户密码字符串
    strcpy(usr, user -> username);
    strcpy(pwd, user -> password);

    //将用户、空格、密码、\n构成新字符串usr，用于在文件中检查匹配
    strcat(usr, " ");
    strcat(usr, pwd);
    strcat(usr, "\n");

    //从文件中读入用户信息数据，遍历判断是否存在
    while(feof(user_file_p) == 0)
    {
        fgets(usr_f, 30, user_file_p);
        if(strcmp(usr, usr_f) == 0)
        {
            check = 1;
            break;
        }
    }

    //关闭文件
    if(fclose(user_file_p))
    {
        printf("Can not close the file!\n");
        exit(0);
    }

    return check;
}

//检查文件是否为空
//空返回0，非空返回1
//测试通过！不要修改
int check_void_file()
{
    FILE *fp;
    fp = fopen("library.txt", "r+");
    char ch;
    ch = fgetc(fp);
    fclose(fp);
    if (ch == EOF) return 0;
    return 1;
}

//从文件中读取数据至链表
//测试通过！不要修改
struct book *make_linklist()
{
    struct book *head;
    head = (struct book *)malloc(sizeof(struct book));
    head->next = NULL;
    FILE *fp;
    fp = fopen("library.txt", "r+");
    if (fp == NULL)
        fp = fopen("library.txt", "w+");
    if (check_void_file() == 0)
    {
        //printf("图书数据库为空！");
        return head;
    }
    struct book *p;
    p = head;
    char ch;
    while(!feof(fp))
    {
        struct book *new_book_init;
        new_book_init = (struct book *)malloc(sizeof(struct book));
        fscanf(fp, "%d", &new_book_init->number);
        ch = fgetc(fp);
        fscanf(fp, "%[^\n]%*c", new_book_init->name);
        fscanf(fp, "%[^\n]%*c", new_book_init->category);
        fscanf(fp, "%d", &new_book_init->lent);
        ch = fgetc(fp);
        fscanf(fp, "%d", &new_book_init->year);
        ch = fgetc(fp);
        fscanf(fp, "%d", &new_book_init->month);
        ch = fgetc(fp);
        fscanf(fp, "%d", &new_book_init->day);
        ch = fgetc(fp);
        new_book_init->next = NULL;

        p->next = new_book_init;
        p = p->next;
    }
    fclose(fp);
    return head;
}


//覆盖链表到文件，将清空原文件的数据
void override_to_file(struct book *head)
{
    FILE *fp;
    fp = fopen("library.txt", "w");
    struct book *p;
    p = head->next;
    while(p->next != NULL)
    {
        fprintf(fp, "%d ", p->number);
        fprintf(fp, "%s\n", p->name);
        fprintf(fp, "%s\n", p->category);
        fprintf(fp, "%d ", p->lent);
        fprintf(fp, "%d ", p->year);
        fprintf(fp, "%d ", p->month);
        fprintf(fp, "%d", p->day);
        fprintf(fp, "\n");
        p = p->next;
    }
    fclose(fp);
}


//通过编号查找
//测试通过，不要修改！
struct book *search_by_number(struct book *head, int number)
{
    struct book *p;
    p = head->next;
    if (p == NULL)
        return NULL;
    while(p -> next != NULL)
    {
        if (p -> number == number)
            return p;
        p = p->next;
    }
    //return NULL;
}

//按照名字查找
//测试成功。不要修改！
struct book *search_by_name(struct book *head, char *name)
{
    struct book *p;
    p = head->next;
    if (p == NULL)
        return NULL;
    while(p != NULL)
    {
        if (strcmp(name, p->name) == 0)
            return p;
        p = p->next;
    }
    //return NULL;
}

//通过输入分类，输出该分类所有的书
//测试成功！
int search_by_category(struct book *head, char *category)
{
    struct book *p;
    p = head->next;
    int flag = 0;
    while(p -> next != NULL)
    {
        if (strcmp(category, p->category) == 0)
        {
            print_booknode(p);
            flag = 1;
        }
        p = p->next;
    }
    if(flag == 0)
    {
        printf("没有找到该分类的书！\n");
        printf("按任意键返回主菜单...\n");
        getch();
        return 0;
    }
    printf("按任意键返回主菜单...\n");
    getch();
    return 0;

}

//输出该书的所有信息
//测试成功！

void print_booklist_title()
{
    printf("您查找的书的资料是：\n");
    printf("--------------------------------------------------------------------------------");
    printf("编号                书名                          分类        借出时间       \n");
    printf("--------------------------------------------------------------------------------");
}

void print_booknode(struct book *t)
{

    if(t != NULL)
    {
        printf("%-20d%-30s%-10s",t->number, t->name, t->category);
        if(t->lent == 1)
        {
            printf("  %d年%d月%d日\n",t->year,t->month,t->day);
        }
        else printf("  在库\n");
    }
    else
    {
        printf("没有找到该书！\n");
        printf("按任意键返回主菜单...\n");
        getch();
    }
}


//delete book
//测试成功！
struct book *delete_book(struct book *head, struct book *p)
{
    printf("                           警告！该操作不可恢复！\n\n");
    printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
    printf("是否继续？\n");
    printf("y：是  任意键：否\n");
    printf("请输入对应选项，按回车继续：");
    char ch;
    scanf("%c", &ch);
    if (ch == 'y')
    {
        struct book *q;
        q = head;
        while(q->next != p)
        {
            q = q->next;
        }
        q->next = p->next;
        free(p);
        return head;
    }
    else
        return head;

}

//输入新的书
//测试通过！不要修改
int input_new_book(struct book *head)
{
    FILE *fp;
    struct book newbook;
    struct book *p;
    p = head;
    if (p -> next == NULL)
    {
        printf("空链表");
    }
input_loop:
    fp = fopen("library.txt", "a+");
    system("ClS");
    printf("\n");
    printf("===================================录入新图书===================================");
    printf("\n");
input_number_loop:
    printf("                        请输入新书的编号：");
    scanf("%d",&newbook.number);
    while (p->next != NULL)
    {
        if (newbook.number == p->number)
        {
            printf("\n                        该书已存在！请核对后重新输入\n                        ");
            system("PAUSE");
            printf("\n");
            goto input_number_loop;
        }
        p = p->next;
    }
    p = head;
    printf("\n");
input_name_loop:
    printf("                        请输入新书的名字：");
    fflush(stdin);
    scanf("%[^'\n']",newbook.name);
    while (p->next != NULL)
    {
        if (strcmp(p->name, newbook.name) == 0)
        {
            printf("\n                        该书已存在！请核对后重新输入\n                        ");
            system("PAUSE");
            printf("\n");
            goto input_name_loop;
        }
        p = p->next;
    }
    p = head;
    fflush(stdin);
    printf("\n");
input_category_loop:
    printf("                        请输入新书的分类：");
    scanf("%[^'\n']",newbook.category);
    while (p->next != NULL)
    {
        if (strcmp(p->category, newbook.category) == 0)
        {
            printf("                        该书已存在！请核对后重新输入\n                        ");
            system("PAUSE");
            printf("\n");
            goto input_name_loop;
        }
        p = p->next;
    }
    p = head;

    newbook.lent = 0;
    newbook.year = 0;
    newbook.month = 0;
    newbook.day = 0;
    newbook.next = NULL;
    fprintf(fp,"%d ",newbook.number);
    fprintf(fp,"%s\n",newbook.name);
    fprintf(fp,"%s\n",newbook.category);
    fprintf(fp,"%d %d %d %d\n",newbook.lent,newbook.year,newbook.month,newbook.day);
    fclose(fp);
    printf("\n                        录入成功！\n\n");
    printf("                        您录入的书本资料如下：\n");
    printf("                        编号：%d\n", newbook.number);
    printf("                        书名：%s\n", newbook.name);
    printf("                        分类：%s\n", newbook.category);
    printf("\n");
    printf("                        是否继续录入？ y：是  n：否 ");
    fflush(stdin);
    char ch;
    scanf("%c", &ch);
    if (ch == 'y')
        goto input_loop;
    else
    {
        printf("\n\n即将在3秒后返回主菜单...");
        Sleep(3000);
        return 1;
    }
}

//输出所有书
//测试成功！
int print_all_book(struct book *head)
{
    system("CLS");
    printf("\n");
    printf("================================显示所有书本====================================");
    printf("\n");
    char ch;
    struct book *p;
    p = head->next;
    if (p == NULL)
    {
        printf("\n数据库中没有书本！\n\n");
        printf("                  按任意键返回...");
        ch = getch();
        return 0;
    }
    print_booklist_title();
    do
    {
        print_booknode(p);
        p = p->next;
    }
    while(p->next != NULL);
    printf("\n按任意键返回...");
    ch = getch();
    return 1;
}


struct book *modify_book(struct book *head, struct book *t)
{
    int i;
    char name_temp[30];
    for(i = 0; i < 30; i++)
        name_temp[i] = '\0';
    i = 0;

    char cate_temp[10];
    for(i = 0; i < 10; i++)
        cate_temp[i] = '\0';
    i = 0;

    int num_temp = 0;
    fflush(stdin);
    //system("CLS");
    //printf("==================================修改图书======================================");
    //printf("                           警告！该操作不可恢复！\n\n");
    //printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
    printf("1、修改名称\n");
    printf("2、修改编号\n");
    printf("3、修改分类\n");
    //printf("4、显示所有书本\n");
    printf("4、什么也不做\n\n");
    printf("请输入选项前的编号，按回车进入：");
    scanf("%d", &i);
    switch(i)
    {
    case 1:
        printf("名称需在30个字符以内，一个汉字为两个字符\n");
        printf("请输入新的名称：");
        fflush(stdin);
        gets(name_temp);
        strcpy(t->name, name_temp);
        printf("修改成功！");
        system("PAUSE");
        return head;
    case 2:
        printf("编号需全为数字，且在20位以内\n");
        printf("请输入新的编号：");
        fflush(stdin);
        scanf("%d", t->number);
        printf("修改成功！");
        system("PAUSE");
        return head;
    case 3:
        printf("分类需在10个字符以内，一个汉字为两个字符\n");
        printf("请输入新的分类：");
        fflush(stdin);
        gets(cate_temp);
        strcpy(t->category, cate_temp);
        printf("修改成功！");
        system("PAUSE");
        return head;
    case 4:
        return head;
    default:
        return head;
    }
}


//主函数
int main()
{

    //登录
    char login_temp;
loop:
    print_main_title();
    printf("\n\n");
    printf("                               1、注册新用户\n\n");
    printf("                               2、登录\n\n");
    printf("                               3、关于\n\n");
    printf("                               4、退出\n\n");
    printf("\n\n");
    printf("                      请输入选项前的编号，按回车键结束：");
    scanf("%c",&login_temp);
    int login_check_temp = 1;
    char login_exit_temp;
    switch(login_temp)
    {
    case '1':
        create_user();
        goto loop;
    case '2':
        login_check_temp = login();
        if(login_check_temp == 0)goto loop;
        break;
    case '3':
        goto loop;
    case '4':
        printf("\n");
        printf("                      您确定要退出吗？\n");
        printf("                      按任意键退出，按n返回\n");
        login_exit_temp = getch();
        if(login_exit_temp == 'n')
        {
            system("CLS");
            goto loop;
        }
        else exit(0);
    default:
        system("CLS");
        goto loop;
    }
    //成功登录则修改标志，以改变标题显示
    check_login_success = 1;

    struct book *head;
    int search_select, admin_select, borrow_select, back_select, modify_select, admin_delete_select;
    char menu_select;
    char ch;
    char str_tmp[100];
    while(1)
    {
        head = make_linklist();
main_loop:
        system("CLS");
        print_main_title();
        printf("\n");
        printf("                                1、借书\n\n");
        printf("                                2、还书\n\n");
        printf("                                3、查找\n\n");
        printf("                                4、显示所有书本\n\n");
        printf("                                5、管理员选项\n\n");
        printf("                                6、退出\n");
        printf("\n");
        printf("                       请输入要使用功能的编号，按回车进入：");

        scanf("%c", &menu_select);

        struct book *t;
        int i;
        int temp = 0;
        char name_temp[30];
        for(i = 0; i < 30; i++) name_temp[i] = 0;
        char category_temp[10];
        for(i = 0; i < 10; i++) category_temp[i] = 0;

        time_t timep;
        struct tm *time_p;
        time(&timep);
        time_p = gmtime(&timep);

        switch(menu_select)
        {
        case '1'://借书
borrow_loop:
            system("CLS");
            print_main_title();
            printf("\n\n");
            printf("                        系统需要定位您要借的书：\n\n");
            printf("                        1、请在此输入书的名字\n\n");
            printf("                        2、请在此输入书的编号\n\n");
            printf("                        3、返回\n\n");
            printf("                        请输入对应选项的编号，按回车进入：");
            scanf("%d", &borrow_select);
            switch(borrow_select)
            {
            case 1:
                printf("请输入书名,按回车结束：");
                scanf("%s", name_temp);
                t = search_by_name(head, name_temp);
                //time_t timep;

                if (t != NULL)
                {
                    system("CLS");
                    print_booklist_title();
                    print_booknode(t);
                    if(t->lent == 0)
                    {
                        printf("您确定要借这本书吗？1：是 2：否");
                        scanf("%d", &temp);
                        if(temp == 1)
                        {
                            t->lent = 1;
                            t->year = 1900 + time_p->tm_year;
                            t->month = time_p->tm_mon;
                            t->day = time_p->tm_mday;
                            //print_booknode(t);
                            printf("借出成功！");
                            override_to_file(head);
                        }
                    }
                    //else break;
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                else
                {
                    printf("没有找到该书！\n");
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                break;
            case 2:
                printf("请输入书的编号，按回车结束：");
                scanf("%d", &temp);
                t = search_by_number(head, temp);
                //time_t timep;

                if (t != NULL)
                {
                    print_booknode(t);
                    if(t->lent == 0)
                    {
                        printf("您确定要借这本书吗？1：是 2：否");
                        scanf("%d", &temp);
                        if(temp == 1)
                        {
                            t->lent = 1;
                            t->year = 1900 + time_p->tm_year;
                            t->month = time_p->tm_mon;
                            t->day = time_p->tm_mday;
                            print_booknode(t);
                            printf("借出成功！");
                            override_to_file(head);
                        }
                    }
                    //else break;
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                else
                {
                    printf("没有找到该书！\n");
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                break;
            case 3:
                break;
            default:
                gets(str_tmp);
                goto borrow_loop;
            }
            break;
        case '2'://还书
            system("CLS");
            print_main_title();
            printf("\n\n");
            printf("                        系统需要定位您要还的书：\n\n");
            printf("                        1、请在此输入书的名字\n\n");
            printf("                        2、请在此输入书的编号\n\n");
            printf("                        3、返回上一级\n\n");
            printf("                        请输入对应功能前的编号，按回车继续：");

            scanf("%d", &back_select);

            switch(back_select)
            {
            case 1://书名
                break;
            case 2://编号
                break;
            default:
                break;
            }

            break;
        case '3'://查找

            system("CLS");
            print_main_title();
            printf("\n\n");
            printf("                        1、根据名字查找\n");
            printf("                        2、根据编号查找\n");
            printf("                        3、根据分类查找\n");
            printf("                        4、根据借出时间查找\n");
            printf("                        5、返回\n");
            printf("\n\n");
            printf("                        请输入对应功能的编号，按回车进入：");
            scanf("%d", &search_select);
            switch(search_select)
            {
            case 1://根据名字
                printf("请输入要查找的书名,按回车结束：");
                scanf("%s", name_temp);
                t = search_by_name(head, name_temp);
                if (t != NULL)
                {
                    print_booknode(t);
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                else
                {
                    printf("没有找到该书！\n");
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                break;
            case 2://根据编号
                //测试成功，不要修改！
                printf("请输入要查找的书的编号,按回车结束：");
                scanf("%d", &temp);
                t = search_by_number(head, temp);
                if (t != NULL)
                {
                    print_booknode(t);
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                else
                {
                    printf("没有找到该书！\n");
                    printf("按任意键返回主菜单...\n");
                    getch();
                    break;
                }
                break;
            case 3://根据分类
                printf("请输入要查找的分类,按回车结束：");
                scanf("%s", category_temp);
                search_by_category(head, category_temp);
                break;
            case 4://根据借出时间
                break;
            default://返回上一级
                break;
            }
            break;

        case '4':
            print_all_book(head);
            break;
        case '5'://管理员
            system("CLS");
            print_main_title();
            printf("\n\n");
            printf("                        1、录入新书到数据库\n\n");
            printf("                        2、修改已有的书的数据\n\n");
            printf("                        3、从数据库中删除书\n\n");
            printf("                        4、返回上一级\n");
            printf("\n\n");
            printf("                        请输入对应功能的编号，按回车进入：");
            scanf("%d", &admin_select);
            switch(admin_select)
            {

            case 1://录入
                input_new_book(head);
                break;

            case 2://修改
modify_loop:
                system("CLS");
                printf("\n");
                printf("====================================修改图书====================================");
                printf("\n");
                printf("                           警告！该操作不可恢复！\n\n");
                printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
                printf("\n\n");
                printf("在这里您可以修改数据库中书的编号、名称、分类和借出时间\n");
                printf("系统需定位您要修改数据的书：\n");
                printf("1、输入名称查找\n");
                printf("2、输入编号查找\n");
                printf("3、列出所有书本\n");
                printf("4、返回\n");
                printf("请输入对应功能前的编号，按回车继续：");
                scanf("%d", &modify_select);
                switch(modify_select)
                {
                case 1://输入名称
                    fflush(stdin);
                    printf("请输入要查找的书的名称，按回车结束：");
                    scanf("%d", &temp);
                    t = search_by_number(head, temp);
                    if (t == NULL)
                    {
                        printf("没有找到该书！请核对后再重新输入\n");
                        printf("按任意键返回主菜单...\n");
                        getch();
                        break;
                    }
                    else
                    {
                        head = modify_book(head, t);
                        override_to_file(head);
                        break;
                    }
                    break;
                case 2://输入编号
                    break;
                case 3:
                    print_all_book(head);
                    goto modify_loop;
                    break;
                case 4:
                    break;
                default:
                    break;
                }
                break;
                //对t进行修改

            case 3://删除
                system("CLS");
                printf("\n");
                printf("====================================删除图书====================================");
                printf("\n");
                printf("                           警告！该操作不可恢复！\n\n");
                printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
                printf("\n\n");
                printf("\n                           警告！该操作不可恢复！\n\n");
                printf("                        系统需定位您要删除的书：\n\n");
                printf("                        1、根据名称查找\n\n");
                printf("                        2、根据编号查找\n\n");
                printf("                        3、返回\n\n");
                printf("                        请输入对应选项的编号，按回车继续：");
                scanf("%d", &admin_delete_select);
                switch(admin_delete_select)
                {
                case 1://按名称删除
                    system("CLS");
                    printf("                           警告！该操作不可恢复！\n\n");
                    printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
                    //printf("如需返回上一级，请输入引号内的“@”字符后按回车继续\n\n");
                    printf("请输入要删除的书的名称，按回车结束：");
                    scanf("%s", name_temp);
                    /*
                    if (name_temp[0] == '@')
                    {
                        printf("\n\n即将返回主菜单\n");
                        system("PAUSE");
                        break;
                    }
                    */
                    t = search_by_name(head, name_temp);
                    if (t != NULL)
                    {
                        head = delete_book(head, t);
                        override_to_file(head);
                    }
                    else
                    {
                        printf("没有找到此书！\n");
                        system("PAUSE");
                    }
                    break;
                case 2://按编号删除
                    system("CLS");
                    printf("                           警告！该操作不可恢复！\n\n");
                    printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
                    //printf("如需返回上一级，请输入引号内的“@”字符后按回车继续\n\n");
                    printf("请输入要删除的书的名称，按回车结束：");
                    scanf("%s", name_temp);
                    /*
                    if (name_temp[0] == '@')
                    {
                        printf("\n\n即将返回主菜单\n");
                        system("PAUSE");
                        break;
                    }
                    */
                    scanf("%d", &temp);
                    t = search_by_number(head, temp);
                    if (t != NULL)
                    {
                        head = delete_book(head, t);
                        override_to_file(head);
                    }
                    else
                    {
                        printf("没有找到此书！\n");
                        system("PAUSE");
                    }
                    break;
                case 3:
                    break;
                }
            default:
                break;
            }
            break;
        case '6'://退出
        {
            printf("\n");
            printf("                      您确定要退出吗？\n");
            printf("                      按任意键退出，按n返回\n");
            login_exit_temp = getch();
            if(login_exit_temp == 'n')
            {
                system("CLS");
                goto main_loop;
            }
            else return 1;
        }

        default:
            goto main_loop;
        }
    }
}


