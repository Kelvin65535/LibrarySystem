#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>

int license_get = 0;

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

struct time
{
    int year;
    int month;
    int day;
};

int login();
int checkUserValid(struct sysuser *user);
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

    printf("================================================================================");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                           欢迎使用图书管理系统v1.1                             ");
    printf("                                   国人原创                                     ");
    printf("                      现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year, p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
    printf("                                                                                ");
    printf("                                                                                ");
    printf("================================================================================");
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
create_loop:
    fp = fopen("user.txt", "a");
    if (fp == NULL) fopen("user.txt", "w");
    system("CLS");
    print_main_title();
    printf("\n\n");
    printf("                  请输入新的账号，按回车结束\n");
    printf("账号的长度为");
    scanf("%s", su.username);
    printf("\n");
    printf("                  请输入新的密码，按回车结束\n");
    printf("         密码的长度为8位，仅允许使用英文字符或数字，不允许空格：");
    //scanf("%s", su.password);
    int i=0;
    while(i < 8&&(su.password[i]=getch())!='\r')
    {
        if(su.password[i]=='\b')
        {
            i--;
        }
        else
        {
            i++;
        }
    }
    su.password[i]='\0';
    fprintf(fp, "%s %s", su.username, su.password);
    fprintf(fp,"\n");
    fclose(fp);
    printf("                  注册成功！\n\n");
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
    while(i < 8&&(su.password[i]=getch())!='\r')
    {
        if(su.password[i]=='\b')
        {
            i--;
        }
        else
        {
            i++;
        }
    }
    su.password[i]='\0';
    //printf("\n你的密码是:%s",su.password);


    if(checkUserValid(&su) == 1)
    {
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
        printf("                       该用户不存在，请核对后重新输入或注册！\n");
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
    while(p -> next != NULL)
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
    printf("编号                书名                          分类        借出时间       \n");
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

//输入新的书
//测试通过！不要修改
int input_new_book()
{
    FILE *fp;
    fp = fopen("library.txt", "a+");
    struct book newbook;
    printf("请输入新书的编号：");
    scanf("%d",&newbook.number);
    printf("请输入新书的名字：");
    fflush(stdin);
    scanf("%[^'\n']",newbook.name);
    fflush(stdin);
    printf("请输入新书的分类：");
    scanf("%[^'\n']",newbook.category);
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
    printf("录入成功！即将返回主菜单...\n");
    return 1;
}

//输出所有书
//测试成功！
int print_all_book(struct book *head)
{
    system("CLS");
    char ch;
    struct book *p;
    p = head->next;
    if (p == NULL)
    {
        printf("数据库中没有书本！\n");
        printf("                  按任意键返回主菜单...");
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
    printf("                  按任意键返回主菜单...");
    ch = getch();
    return 1;
}

int give_license()
{
    char license[54];
    strcpy(license, "9ad3eb1a654e073ba4369d25701de71b4cbf5d23bc98222531114");
    char user_license[54];
    int i;
    for(i = 0; i < 54; i++)
        user_license[i] == '\0';
    printf("请输入激活码，按回车继续：\n");
    fflush(stdin);
    gets(user_license);
    int check = 1;
    check = strcmp(license, user_license);
    if (check == 0){
        license_get = 1;
        return 1;
    }
    else return 0;
}

//主函数
int main()
{

    //登录
    char login_temp;
loop:
    print_main_title();
    printf("\n\n");
    printf("                        1、注册新用户\n");
    printf("                        2、登录\n");
    printf("                        3、关于/跳过登录\n");
    printf("                        4、退出\n");
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
        break;
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
    //成功登录则继续执行程序

    struct book *head;
    int search_select, admin_select, borrow_select, modify_select, admin_delete_select;
    char menu_select;
    char ch;
    char str_tmp[100];
    while(1)
    {
        head = make_linklist();
main_loop:
        system("CLS");
        print_main_title();
        printf("\n\n");
        printf("                        1、借书\n");
        printf("                        2、还书\n");
        printf("                        3、查找\n");
        printf("                        4、显示所有书本\n");
        printf("                        5、管理员选项\n");
        printf("                        6、退出\n");
        printf("                        7、授权\n");
        printf("\n");
        printf("                   请输入要使用功能的编号，按回车进入：");

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
            printf("                        1、根据名字查找\n\n");
            printf("                        2、根据编号查找\n\n");
            printf("                        3、返回\n\n");
            printf("                        请输入对应功能的编号，按回车进入：");
            scanf("%d", &borrow_select);
            switch(borrow_select)
            {
            case 1:
                printf("请输入要查找的书名,按回车结束：");
                scanf("%s", name_temp);
                t = search_by_name(head, name_temp);
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
            case 2:
                printf("请输入要查找的编号，按回车结束：");
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
            if (license_get == 0){
            printf("对不起，该功能尚未开放!\n\n");
            printf("亲爱的用户你好，您现在使用的是免费版本，仅包含完全版的部分功能。\n该功能需要购买获得授权以继续使用，购买金额为10元。\n");

            printf("请把金额转账到支付宝账号1057703657@qq.com中，作者收到转账会自动向您发送激活码，请在收到后进入主菜单的授权选项，输入激活码，即可解锁全部功能！\n");
            printf("一次解锁，终身免费！\n");
            printf("您的支持是开发者莫大的动力！\n");
            system("PAUSE");
            }

            else
            {
                printf("骗你的哈哈哈哈哈哈\nby 叶嘉永\n");
                system("PAUSE");
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
            printf("                        1、录入新书到数据库\n");
            printf("                        2、修改已有的书的数据\n");
            printf("                        3、从数据库中删除书\n");
            printf("                        4、返回上一级\n");
            printf("\n\n");
            printf("                        请输入对应功能的编号，按回车进入：");
            scanf("%d", &admin_select);
            switch(admin_select)
            {
            case 1://录入
                input_new_book();
                break;
            case 2://修改
                system("CLS");
                print_main_title();
                printf("\n\n");
                printf("在这里您可以修改数据库中书的编号、名称、分类和借出时间\n");
                printf("系统需定位您要修改数据的书：\n");
                printf("1、输入名称查找\n");
                printf("2、输入编号查找\n");
                scanf("%d", &modify_select);
                switch(modify_select)
                {
                case 1://输入名称
                    printf("请输入要查找的书的编号,按回车结束：");
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
                        printf("请输入新书的名称：");
                        printf("名称需在30个字符以内，一个汉字为两个字符\n");
                        gets(name_temp);
                        strcpy(t->name, name_temp);
                        override_to_file(head);
                        printf("修改成功！");
                        print_booklist_title();
                        print_booknode(t);
                    }
                    break;
                case 2://输入编号
                    break;
                default:
                    break;
                }

            //对t进行修改

            case 3://删除
                system("CLS");
                print_main_title();
                printf("                        1、名称\n\n");
                printf("                        2、编号\n\n");
                printf("                        3、返回\n\n");
                scanf("%d", &admin_delete_select);
                switch(admin_delete_select)
                {
                case 1://按名称删除
                    printf("请输入要删除的书的名称，按回车结束：");
                    scanf("%s", name_temp);
                    t = search_by_name(head, name_temp);
                    head = delete_book(head, t);
                    override_to_file(head);
                case 2://按编号删除
                    printf("请输入要删除的书的编号，按回车结束：");
                    scanf("%d", &temp);
                    t = search_by_number(head, temp);
                    head = delete_book(head, t);
                    override_to_file(head);
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

        case '7':
            {
                system("CLS");
                int check;
                give_license();
                if (license_get == 1)
                    printf("授权成功！请尽情使用该软件吧\n");
                else
                    printf("授权错误！请核对激活码是否正确并重新输入\n");
                system("PAUSE");
            }

        default:
            goto main_loop;
        }
    }
}


/*

{
    head = make_linklist();
    system("CLS");

    //输出菜单


    scanf("%d", &menu_select);
    int y;
    int m;
    int d;

    struct book *t;
    int i;
    int temp = 0;
    char name_temp[30];
    for(i = 0; i < 30; i++) name_temp[i] = 0;
    char category_temp[10];
    for(i = 0; i < 10; i++) name_temp[i] = 0;

    switch(menu_select)
    {
    case 1:
        input_new_book();
        break;
    case 2:
        printf("请输入书的名称，按回车结束：");
        scanf("%s", name_temp);
        t = search_by_name(head, name_temp);
        t->lent = 1;
        //printf("")
        break;
    case 3:
        printf("注意！此操作不可恢复！\n");
        printf("1、按照编号删除\n2、按照名称删除\n3、返回\n请输入：");
        scanf("%d", &temp);
        switch(temp)
        {
        case 1:
            printf("请输入要删除的书的编号，按回车结束：");
            scanf("%d", &temp);
            t = search_by_number(head, temp);
            head = delete_book(head, t);
            override_to_file(head);
            break;
        case 2:
            printf("请输入要查找的书的名称，按回车结束：");
            scanf("%s", name_temp);
            t = search_by_name(head, name_temp);
            head = delete_book(head, t);
            override_to_file(head);
        default:
            break;
        }
        break;
    case 4:
        //测试成功！
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
        break;
    case 5:
        printf("请输入要查找的分类,按回车结束：");
        scanf("%s", category_temp);
        search_by_category(head, category_temp);
        break;
    case 6:
        //测试成功，不要修改！
        printf("请输入要查找的书的编号,按回车结束：");
        scanf("%d", &temp);
        t = search_by_number(head, temp);
        print_booknode(t);
        printf("按任意键返回主菜单...\n");
        getch();
        break;
    case 7:
        break;
    case 8:
        printf("1、列出所有书籍\n2、按编号顺序重新整理图书\n3、覆盖链表到文件\n4、获取当前日期");
        scanf("%d",&temp);
        switch(temp)
        {
        case 1:
            print_all_book(head);
            break;
        case 3:
            override_to_file(head);
            break;
            //case 4:
            //_getsystime
        }
        break;
    case 9:
        return 1;
    }
}
*/
