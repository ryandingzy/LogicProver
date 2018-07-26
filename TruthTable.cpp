#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <iostream>
#include <stack>
#include <map>
using namespace std;
string epr, epr1, epr2;
string pdnf, pcnf; //pdnf析取, pcnf合取
map <char, bool> mp; //哈希表
stack <char> st;    //用于得到后缀表达式
stack <int> dst;    //用于计算表达式的真值
int val[26];
int res, num;

bool CanIn(char fir, char sec) //能否入栈
{
    int i, o;
    switch(fir)    //设置栈内，栈外元素优先级
    {  
        case '#': i = 0; break;  
        case '(': i = 1; break;  
        case '~': i = 3; break;  
        case '>': i = 5; break;  
        case '|': i = 7; break;  
        case '&': i = 9; break;  
        case '!': i = 11; break;  
        case ')': i = 12; break;  
    }  
    switch(sec)  
    {  
        case '#': o = 0; break;  
        case '(': o = 12; break;  
        case '~': o = 2; break;  
        case '>': o = 4; break;  
        case '|': o = 6; break;  
        case '&': o = 8; break;  
        case '!': o = 10; break;  
        case ')': o = 1; break;  
    }  
    if(i < o)  
        return true;  
    else  
        return false;  
}

void to_suffix()      //中缀表达式转后缀表达式
{
    while(st.size())    //清空栈
        st.pop();
    string tmp = "";    //中间变量，用来存后缀表达式
    st.push('#');       //'#'为栈底标记
    int len = epr.length();
    for(int i = 0; i < len; i++)
    {
        if(mp[epr[i]])          //如果当前是字母，则直接加到后缀表达式中
        {
            tmp = tmp + epr[i];
            continue;
        }
        if(CanIn(st.top(), epr[i])) //拿当前的字符和栈顶元素相比，若栈顶元素优先级小于栈外优先级，则入栈
            st.push(epr[i]);
        else if(epr[i] == ')')  //处理括号嵌套
        {
            while(st.top() != '(')
            {
                tmp = tmp + st.top();
                st.pop();
            }
            st.pop();
        }
        else        //如果栈顶元素优先级大于栈外元素优先级，则将元素出栈，直到栈顶元素优先级小于栈外优先级
        {
            do
            {
                tmp = tmp + st.top();
                st.pop();
            }while(!CanIn(st.top(), epr[i]));
            st.push(epr[i]);
        }
    }
    while(st.top() != '#')   //把剩余元素出栈
    {  
        tmp = tmp + st.top();     
        st.pop();  
    }  
    st.pop();               
    epr = tmp;     //此时tmp为得到的后缀表达式，赋值给epr
    //cout << "epr = " << epr << endl;
}

void Or(int a, int b)     //或运算
{
    res = a + b;
    res = res > 1 ? 1 : res;
    dst.push(res);
}

void And(int a, int b)     //与运算
{
    res = a * b;
    dst.push(res);
}

void Not()          //非运算
{
    int a = dst.top();
    dst.pop();
    res = a == 1 ? 0 : 1;
    dst.push(res);
}   

void If(int a, int b)    //条件运算
{
    res = (b == 1 && a == 0) ? 0 : 1;
    dst.push(res);
}

void Iif(int a, int b)  //双条件运算
{
    res = (a == b) ? 1 : 0;
    dst.push(res);
}

void cal()      //计算真值
{
    while(dst.size())   //清空栈dst
        dst.pop();
    int len = epr.length();
    int a, b;
    for(int i = 0; i < len; i++)  
    {
        if(mp[epr[i]])
        {
            for(int j = 0; j < 26; j++)
            {
                if(epr[i] == (j + 'A'))
                {
                    dst.push(val[j]);
                    break;
                }
            }
            continue;
        }  
        if(epr[i] != '!')
        {
            a = dst.top();
            dst.pop();
            b = dst.top();
            dst.pop();
        } 
        switch(epr[i])  
        {  
            case '|': 
                Or(a, b); 
                break;  
            case '&': 
                And(a, b); 
                break;  
            case '!': 
                Not(); 
                break;  
            case '~': 
                Iif(a, b); 
                break;  
            case '>': 
                If(a, b); 
                break;  
        }   
    }  
}

void Input1()  //输入一个表达式
{
    epr = "";
    printf("请输入两个真值表达式: (! 否定), (| 析取), (& 合取), (> 条件), (~ 双条件)\n");
    cin >> epr; 
}

void preprea() //处理表达式
{
    num = 0;
    mp.clear();  //清空map
    int len = epr.length();
    for(int i = 0; i < len; i++)
    {
        if(epr[i] >= 'A' && epr[i] <= 'Z')  
        {
            if(mp[epr[i]] == false)
            {
                mp[epr[i]] = true;  //记录那些字母在表达式中
                num ++;     //num表示表达式中有几种字母
            }
        }
    }
}

void Input2()   //输入两个表达式
{
    epr = "";
    epr1 = "";
    epr2 = "";
    printf("请输入两个真值表达式: (! 否定), (| 析取), (& 合取), (> 条件), (~ 双条件)\n");
    printf("请输入第一个\n");
    cin >> epr1;
    printf("请输入第二个\n");
    cin >> epr2;
}

//idx是字母的标号，cnt表示已经处理的字母的个数，p标记是否输出真值表
void DFS(int idx, int cnt, int p)   //深度优先搜索得到真值表和主范式
{
    if(cnt == num)
    {
        cal();
        if(res == 1)
        {
            pdnf += "(";
            int c = 0;
            for(int i = 0; i < 26; i++)
            {
                char t[1];
                t[0] = i + 'A';
                string ch(t);
                if(mp[ch[0]])
                {
                    c ++;
                    if(val[i] == 1)
                        pdnf += ch;
                    else
                        pdnf += "!" + ch; 
                    if(c != num)
                        pdnf += "&";
                }
            }
            pdnf += ")";
            pdnf += "|";
        }
        else
        {
            pcnf += "(";
            int c = 0; //记录字母个数
            for(int i = 0; i < 26; i++)
            {
                char t[1];
                t[0] = i + 'A';
                string ch(t);
                if(mp[ch[0]]) //判断字母是否存在
                {
                    c ++;
                    if(val[i] == 0)
                        pcnf += ch;
                    else
                        pcnf += "!" + ch; 
                    if(c != num)
                        pcnf += "|";
                }
            }
            pcnf += ")";
            pcnf += "&";
        }
        if(!p)
        {
            for(int i = 0; i < 26; i++)
                if(mp[i + 'A'])
                    printf("%d\t", val[i]);
            printf("%d\n", res);
        }
        return;
    }
    int idxx = 0;
    for(int i = idx; i < 26; i++)
    {
        if(mp[i + 'A'])
        {
            idxx = i;
            break;
        }
    }
    val[idxx] = 1;
    DFS(idxx + 1, cnt + 1, p);
    val[idxx] = 0;
    DFS(idxx + 1, cnt + 1, p);
    return;
}

void Print(bool p)  //输出真值表和主范式, 若p==0，则输出真值表
{
    if(!p)
    {
        for(int i = 0; i < 26; i++)
            if(mp[i + 'A'])
                printf("%c\t", i + 'A');
        printf("res\n");
    }
    int idx = 0;  //idx用来记录0 - 26分别于A - Z对应
    for(int i = 0; i < 26; i++)
    {
        if(mp[i + 'A'])
        {
            idx = i;
            break;
        }
    }
    val[idx] = 1;
    DFS(idx + 1, 1, p);
    val[idx] = 0;
    DFS(idx + 1, 1, p);
}

void output_table()    //输出真值表接口函数
{
    Input1();
    preprea();
    to_suffix();
    printf("真值表为:\n");
    Print(0);
}

void output_pcnf()    //输出主合取范式接口函数  
{
    Input1();
    preprea();
    to_suffix();
    pcnf = ""; //初始化
    Print(1);  
    int len = pcnf.length();
    if(len == 0)
        printf("永真式\n");
    else
    {
        pcnf.erase(pcnf.length() - 1);  
        cout << "主合取范式：" << pcnf << endl << endl;  
    }
}

void output_pdnf()  //输出主析取范式接口函数  
{
    Input1();
    preprea();
    to_suffix();
    pdnf = "";
    Print(1);
    int len = pdnf.length();
    if(len == 0)
        printf("永假式\n");
    else
    {
        pdnf.erase(pdnf.length() - 1);  
        cout << "主析取范式：" << pdnf << endl << endl;  
    }
}

//判断两个表达式的关系
//原理: 命题A与命题B若等价则(A<->B)永真,又永真式无主合取范式
//     命题A与命题B若存在蕴含关系(如A蕴含B)则(A->B)永真,又永真式无主合取范式
void judge_two()    
{
    bool flag1 = false, flag2 = false;
    Input2();
    epr = "(" + epr1 + ")" + "~" + "(" + epr2 + ")";
    preprea();
    to_suffix();
    pcnf = "";
    Print(1);
    int len = pcnf.length();
    if(len == 0)
    {
        flag1 = true;
        printf("\n两命题公式等价\n");
    }
    if(!flag1)
    {
        epr = "(" + epr1 + ")" + ">" + "(" + epr2 + ")";
        preprea();
        to_suffix();
        pcnf = "";
        Print(1);
        len = pcnf.length();
        if(len == 0)
        {
            flag2 = true;
            cout << endl << epr1 << endl;
            printf("蕴含\n");
            cout << epr2 << endl;
        }

        epr = "(" + epr2 + ")" + ">" + "(" + epr1 + ")";
        preprea();
        to_suffix();
        pcnf = "";
        Print(1);
        len = pcnf.length();
        if(len == 0)
        {
            flag2 = true;
            cout << endl << epr2 << endl;
            printf("蕴含\n");
            cout << epr1 << endl;
        }
    }
    if(!flag1 && !flag2)
        printf("两命题公式既不等价又不存在蕴含关系\n");
}

void solve() //解决逻辑问题
{
    printf("问题描述:\n");
    printf("一对夫妻带着他们的一个孩子在路上碰到一个朋友.\n");
    printf("朋友问孩子:\"你是男孩还是女孩?\"朋友没听清孩子的回答.\n");
    printf("孩子父母中某一个说,我孩子回答的是\"我是男孩\",另一个接着说:\"这孩子撒谎,她是女孩.\"\n");
    printf("这家人中男性从不说谎,而女性从来不连续说两句真话,也不连续说两句假话.\n");
    printf("试问这小孩性别,以及谁是其父亲,谁是其母亲？\n\n");
    printf("解决如下, 设:\n\n");
    printf("P表示命题:第一个说话的是父亲\n");
    printf("Q表示命题:第一个人说的话为真\n");
    printf("R表示命题:第二个人说的第一句为真\n");
    printf("S表示命题:第二个人说的第二句为真\n");
    printf("根据已知矛盾信息,我们可以得到以下几种矛盾情况\n");
    printf("1. P&!Q        (即第一个说话的是父亲,父亲说谎)\n");
    printf("2. P&Q         (即第一个说话的是父亲,父亲没说谎,此时若母亲两句话都真或都假\n");
    printf("                则与已知矛盾,若一真一假,则均与父亲所说的矛盾)\n");
    printf("3. !P&(!(R&S)) (即第二个说话的是父亲,两句中有假话)\n");
    printf("4. !P&Q        (即第一个说话的是母亲,且母亲说的是真的,则因为父亲不说假话)\n");
    printf("                ,因此父亲的话为真与母亲的话矛盾\n");
    printf("得到矛盾的四种情况,把他们或在一起得到表达式:\n");
    printf("(P&!Q)|(P&Q)|(!P&(!(R&S)))|(!P&Q)\n");
    printf("求解该表达式的真值表:\n");
    epr = "(P&!Q)|(P&Q)|(!P&(!(R&S)))|(!P&Q)";
    preprea();
    to_suffix();
    Print(0);
    printf("\n从真值表可以看出只有0 0 1 1的时候没有矛盾\n");
    printf("即母亲先说且说谎,父亲后说没说谎,于是可得到答案:\n\n");
    printf("第一个说话的是母亲\n");
    printf("第二个说话的是父亲\n");
    printf("孩子是女孩\n");
}

void Menu() //菜单
{
    printf("\n欢饮使用\n");
    printf("请选择操作:\n");
    printf("1 - 输出真值表\n");
    printf("2 - 输出主合取范式\n");
    printf("3 - 输出主析取范式\n");
    printf("4 - 比较两命题公式是否等价或蕴含\n");
    printf("5 - 解决题目中的逻辑问题\n");
    printf("6 - 退出\n");
}

int main()
{
    Menu();
    while(true)
    {
        char tp[2]; //选择操作
        scanf("%s", tp);
        if(tp[0] > '6' || tp[0] < '1')
        {
            printf("错误输入\n");
            continue;
        }
        if(tp[0] == '1')
            output_table();
        else if(tp[0] == '2')
            output_pcnf();
        else if(tp[0] == '3')
            output_pdnf();
        else if(tp[0] == '4')
            judge_two();
        else if(tp[0] == '5')
            solve();
        else if(tp[0] == '6')
        {
            printf("谢谢使用\n");
            break;
        }   
        Menu();
    }
}
