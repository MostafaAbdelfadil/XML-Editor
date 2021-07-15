#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QStack>
#include <QVector>
#include <QQueue>
#include <QFile>
#include <QTextBrowser>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString first_tag = "#text";
QString text_tag = '"' + first_tag + '"';
QString space(int n)
    {
      QString space = "    ",st;
          for (int i = 0; i < n; i++)
          {
              st += space;
          }
          return st;
    }
QString space1(int n)
    {
      QString space = " ",st;
          for (int i = 0; i < n; i++)
          {
              st += space;
          }
          return st;
    }
int find(QString s,char c)
{
    for(int i=0;i<s.length();i++)
    {
        if(s[i]==c)
            return i;
    }
    return -1;
}
int rfind(QString s,char c)
{
    for(int i=s.length()-1;i>=0;i--)
    {
        if(s[i]==c)
            return i;
    }
    return -1;
}
int FindFirstAngle(QString s)
{
    for (int j = 0; j < s.length(); j++)
    {
        if (s[j] != ' ')
            return j;
    }
}
int Num_Of_Angle(QString s)
{
    int count = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '<')
            count++;
    }
    return count;
}
QVector<QString> tokens(QString s)
{
    QVector<QString> a;
    QVector<int> spaces;
    int index = find(s,'>');
    int count = 0;
    for (int i = 0;i < index + 1; i++)
    {
        if (s[i] == '"')
        {
            count++;
            if (count == 2)
                count = 0;
        }
        if (s[i] == ' ' && count == 0)
            spaces.push_back(i);
    }
    if (spaces.size() > 0)
    {
        a.push_back((s.mid(0, spaces[0])) + '>');
        for (int i = 0; i < spaces.size(); i++)
        {
            if (i < spaces.size() - 1)
                a.push_back(s.mid(spaces[i] + 1, spaces[i + 1] - spaces[i] - 1));
            else
                a.push_back(s.mid(spaces[i] + 1, index - spaces[i] - 1));
        }
    }
    else
        a.push_back(s.mid(0, index + 1));


    return a;
}
struct attribute
{
    QString name;
    QString value;
    int level;
};
class node
{
    QString tag;   //tag with attributes
    QString name;  //tag without attributes
    QString JsonName;
    bool JsonMark;
    QVector<attribute> attrs;
    int level;
    node* parent;
    QVector<node*> childs;

public:
    node() { level = 0; JsonMark = false; parent = NULL; }
    node(QString);
    void set_tag(QString g) { tag = g; }
    QString get_tag() { return tag; }
    void set_name(QString n) { name = n; JsonName = '"' + name.mid(1, name.length() - 2) + '"'; }
    QString get_name() { return name; }
    void set_JsonName(QString j) { JsonName = '"' + j + '"'; }
    QString get_JsonName() { return JsonName; }
    void set_Mark(bool m) { JsonMark = m; }
    bool get_Mark() { return JsonMark; }
    void set_attributes(QVector<QString>);
    QVector<attribute> get_attributes() { return attrs; }
    void set_lvl(int l) { level = l; if (attrs.size() > 0) { for (int i = 0; i < attrs.size(); i++) { attrs[i].level = l + 1; } } }
    int get_lvl() { return level; }
    void set_parent(node* p) { parent = p; }
    node* get_parent() { return parent; }
    void set_child(node* ch) { childs.push_back(ch); childs.back()->set_parent(this); }
    QVector<node*> get_childs() { return childs; }
};
node::node(QString t)
{
    name = t;
    JsonName = '"' + name.mid(1, name.length() - 2) + '"';
    JsonMark = false;
    level = 0;
    parent = NULL;
}
void node::set_attributes(QVector<QString> toks)
{
    for (int i = 1; i < toks.size(); i++)
    {
        attribute tr;
        int equal = find(toks[i],'=');
        tr.name = '"' + toks[i].mid(0, equal) + '"';
        tr.value = toks[i].mid(equal + 1);
        attrs.push_back(tr);
    }
}

class Tree
{
    node* root;
QQueue<QString>ve ;
public:
    Tree() { root = NULL; }
    Tree(QString FileLoc) { root = NULL;


                              }
    //Tree() { out.close(); }
    bool Empty(){ return (root == NULL); }
    node* get_root() { return root; }
    QVector<QVector<node*>> similar(QVector<node*>);
    void insert(QString);
    void print_attributes(node*);
    void print_minify(node*);
    void print_Json(node*);
    void print_minify(QString output);
    void print_Json(QString output);

};

QVector<QVector<node*>> Tree::similar(QVector<node*> same)
{
    QVector<QVector<node*>> a;
    for (int i = 0; i < same.size(); i++)
    {
        QVector<node*> temp;
        if (!same[i]->get_Mark())
        {
            temp.push_back(same[i]);
            same[i]->set_Mark(true);
        }
        for (int j = i + 1; j < same.size(); j++)
        {
            if ((same[i]->get_name() == same[j]->get_name()) && (!same[j]->get_Mark()))
            {
                temp.push_back(same[j]);
                same[j]->set_Mark(true);
            }
        }
        if (temp.size() > 0)
            a.push_back(temp);
    }
    for (int i = 0; i < same.size(); i++)
        same[i]->set_Mark(false);
    return a;
}

void Tree::insert(QString FileLoc)
{
     QFile file(FileLoc);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
  QTextStream in(&file);
    QStack<QString> tags;
    QStack<int> lvls;
    node* p = NULL;

    while (!in.atEnd())
    {
        QString t;
        t = in.readLine();
        int n = FindFirstAngle(t);

        t = t.mid(n);
        QVector<QString> toks;
        if (t[0] == '<') //not text
            toks = tokens(t);

        if (Empty())
        {
            root = new node(toks[0]);
            p = root;
            root->set_tag(t);
            p->set_attributes(toks);
            tags.push(toks[0]);
            lvls.push(0);
            root->set_lvl(0); continue;
        }

        if (t[0] != '<') //text
        {
            node* ch = new node();
            ch->set_tag(t);
            ch->set_JsonName(t);
            p->set_child(ch);
            ch->set_lvl(ch->get_parent()->get_lvl() + 1);
        }
        else
        {
            if (t[1] == '/') //closing tag
            {
                tags.pop(); lvls.pop();
                p = p->get_parent();
            }
            else //openning tag
            {
                if (Num_Of_Angle(t) == 1) //several lines tag
                {
                    tags.push(toks[0]);
                    lvls.push(lvls.top() + 1);
                    node* ch = new node(toks[0]);
                    p->set_child(ch);
                    p = ch;
                    p->set_tag(t);
                    p->set_attributes(toks);
                    p->set_lvl(lvls.top());
                }
                else if (Num_Of_Angle(t) > 1) //same line tag
                {
                    int index = find(t,'>');
                    int index1 = rfind(t,'<');
                    tags.push(toks[0]);
                    lvls.push(lvls.top() + 1);
                    node* ch = new node(toks[0]);
                    p->set_child(ch);
                    p = ch;
                    p->set_tag(t.mid(0, index + 1));
                    p->set_attributes(toks);
                    p->set_lvl(lvls.top());

                    //text in same line tag
                    node* text = new node();
                    text->set_tag(t.mid(index + 1, index1 - index - 1));
                    text->set_JsonName(text->get_tag());
                    p->set_child(text);
                    text->set_lvl(text->get_parent()->get_lvl() + 1);
                    tags.pop(); lvls.pop();
                    p = p->get_parent();
                }
            }
        }

    }
    file.close();
}

void Tree::print_attributes(node* n)
{
    for (int i = 0; i < n->get_attributes().size(); i++) //printing attributes
    {
       ve.enqueue(space(n->get_attributes()[i].level + 1) + n->get_attributes()[i].name
            + ": " + n->get_attributes()[i].value + "," + '\n');
    }
}
void Tree::print_minify(node* rootptr)
{
    if (rootptr->get_tag()[0] == '<')
    {
        ve.enqueue(rootptr->get_tag());
        int size = rootptr->get_childs().size();
        for (int i = 0; i < size; i++)
            print_minify(rootptr->get_childs()[i]);

        ve.enqueue("</" + rootptr->get_name().mid(1));
    }
    else{
        ve.enqueue(rootptr->get_tag());
    }
}
void Tree::print_minify(QString output)
{
    QFile file1(output);
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text))
           return;
    QTextStream out(&file1);
print_minify(root);
    int size=ve.size();
   for (int i = 0; i < size; i++)
    {
        out << ve.front();
        ve.dequeue();

    }
   file1.close();
    }




void Tree::print_Json(node* rootptr)
{
    QVector<QVector<node*>> SimChilds;
    if (rootptr->get_tag()[0] == '<')
    {
        if (rootptr->get_childs()[0]->get_tag()[0] == '<')
            SimChilds = similar(rootptr->get_childs());
    }

    if (rootptr->get_parent() == NULL) //Tree root
    {
       ve.enqueue("{\n");
        ve.enqueue(space(rootptr->get_lvl() + 1) + rootptr->get_JsonName() + ":");
        if (rootptr->get_childs().size() > 0 || rootptr->get_attributes().size() > 0)
        {
            ve.enqueue( " {\n");
            print_attributes(rootptr);

            for (int i = 0; i < SimChilds.size(); i++)
            {
                if (SimChilds[i].size() > 1)  //array json
                {
                    for (int k = 0; k < SimChilds[i].size(); k++)  //similars marked
                    {
                        SimChilds[i][k]->set_Mark(true);
                    }
                    ve.enqueue(space(SimChilds[i][0]->get_lvl() + 1)+ SimChilds[i][0]->get_JsonName() + ": [\n");
                    for (int j = 0; j < SimChilds[i].size(); j++)
                    {
                        if (SimChilds[i][j]->get_childs()[0]->get_tag()[0] != '<' && SimChilds[i][j]->get_attributes().size() == 0)
                        {
                            print_Json(SimChilds[i][j]);
                        }
                        else if (SimChilds[i][j]->get_childs()[0]->get_tag()[0] != '<' && SimChilds[i][j]->get_attributes().size() > 0)
                        {
                            print_Json(SimChilds[i][j]);
                        }
                        else
                        {
                            ve.enqueue(space(SimChilds[i][j]->get_lvl() + 2) + "{\n");
                            print_Json(SimChilds[i][j]);
                        }
                        if (SimChilds[i][j]->get_childs()[0]->get_tag()[0] != '<' )
                        {
                            if (j == SimChilds[i].size() - 1) //last element
                                ve.enqueue("\n");
                            else
                                ve.enqueue(",\n");
                        }
                        else
                        {
                            if (j == SimChilds[i].size() - 1)
                                ve.enqueue(space(SimChilds[i][j]->get_lvl() + 2) + "}\n");
                            else
                                ve.enqueue(space(SimChilds[i][j]->get_lvl() + 2) + "},\n");

                        }
                    }
                    if (SimChilds[i][0]->get_childs()[0]->get_tag()[0] != '<' && SimChilds[i][0]->get_attributes().size() == 0)
                        ve.enqueue(space(SimChilds[i][0]->get_lvl() + 1) + "]\n");
                    else
                        ve.enqueue(space(SimChilds[i][0]->get_lvl() + 1) + "]\n");

                }
                else  //normal json
                {
                    print_Json(SimChilds[i][0]);
                }
            }
            ve.enqueue(space(rootptr->get_lvl() + 1) + "}\n");
        }
         ve.enqueue( "}");
    }

    else if (rootptr->get_tag()[0] != '<') //Text node
    {
        if (rootptr->get_parent()->get_attributes().size() > 0)
        {
            ve.enqueue(space(rootptr->get_lvl() + 1) + text_tag + ": " + rootptr->get_JsonName() + '\n');
        }
        else if (rootptr->get_parent()->get_Mark() == true)
        {
            ve.enqueue(space(rootptr->get_lvl() + 1) + rootptr->get_JsonName());
        }
        else
        {
            ve.enqueue(rootptr->get_JsonName() + ",\n");
        }

    }

    else if (rootptr->get_childs()[0]->get_tag()[0] != '<') //leaf node
    {
        if (!rootptr->get_Mark())
        {
            ve.enqueue(space(rootptr->get_lvl() + 1) + rootptr->get_JsonName() + ":");
        }
        //rootptr->set_Mark(true);
        if (rootptr->get_attributes().size() > 0)  //leaf node with attributes
        {
            if (!rootptr->get_Mark())
            {
                ve.enqueue(" {\n");
            }
            else  //leaf but array
                ve.enqueue(space(rootptr->get_lvl() + 2) + "{\n");

            print_attributes(rootptr);
            for (int i = 0; i < rootptr->get_childs().size(); i++)
            {
                print_Json(rootptr->get_childs()[i]);
            }
            if (!rootptr->get_Mark())
                ve.enqueue(space(rootptr->get_lvl() + 2) + "},\n");
            else
                ve.enqueue(space(rootptr->get_lvl() + 2) + "}");
        }
        else  //leaf node without attributes
        {
            ve.enqueue(" ");
            for (int i = 0; i < rootptr->get_childs().size(); i++)
            {
                print_Json(rootptr->get_childs()[i]);
            }
        }

    }
    //node with tags childs
    else
    {
        if (!rootptr->get_Mark())
        {
            ve.enqueue( space(rootptr->get_lvl() + 1) + rootptr->get_JsonName() + ":");
            if (rootptr->get_childs().size() > 0 || rootptr->get_attributes().size() > 0)
            {
                ve.enqueue( " {\n");
                print_attributes(rootptr);
            }
        }
        else
            print_attributes(rootptr);

        for (int i = 0; i < SimChilds.size(); i++)
        {
            if (SimChilds[i].size() > 1)  //array json
            {
                for (int k = 0; k < SimChilds[i].size(); k++)  //similars marked
                {
                    SimChilds[i][k]->set_Mark(true);
                }

                ve.enqueue( space(SimChilds[i][0]->get_lvl() + 1) + SimChilds[i][0]->get_JsonName() + ": [\n");
                for (int j = 0; j < SimChilds[i].size(); j++)
                {
                    if (SimChilds[i][j]->get_childs()[0]->get_tag()[0] != '<' && SimChilds[i][j]->get_attributes().size() == 0)
                    {
                        print_Json(SimChilds[i][j]);
                    }
                    else if (SimChilds[i][j]->get_childs()[0]->get_tag()[0] != '<' && SimChilds[i][j]->get_attributes().size() > 0)
                    {
                        print_Json(SimChilds[i][j]);
                    }
                    else
                    {
                         ve.enqueue(space(SimChilds[i][j]->get_lvl() + 2) + "{\n");
                        print_Json(SimChilds[i][j]);
                    }
                    if (SimChilds[i][j]->get_childs()[0]->get_tag()[0] != '<' )
                    {
                        if (j == SimChilds[i].size() - 1) //last element
                            ve.enqueue("\n");
                        else
                            ve.enqueue(",\n");
                    }
                    else
                    {
                        if (j == SimChilds[i].size() - 1)
                            ve.enqueue(space(SimChilds[i][j]->get_lvl() + 2) + "}\n");
                        else
                            ve.enqueue( space(SimChilds[i][j]->get_lvl() + 2) + "},\n");

                    }
                }

                if (SimChilds[i][0]->get_childs()[0]->get_tag()[0] != '<' && SimChilds[i][0]->get_attributes().size() == 0)
                    ve.enqueue(space(SimChilds[i][0]->get_lvl() + 1) + "]\n");
                else
                {
                    if (i == SimChilds.size() - 1) //last array
                    {
                        ve.enqueue(space(SimChilds[i][0]->get_lvl() + 1) + "]\n");
                    }
                    else
                        ve.enqueue( space(SimChilds[i][0]->get_lvl() + 1) + "],\n");
                }
            }
            else  //normal json
            {
                print_Json(SimChilds[i][0]);
            }
        }
        if (!rootptr->get_Mark())
            ve.enqueue( space(rootptr->get_lvl() + 1) +"}\n");
    }
}

void Tree::print_Json(QString output)
{
    QFile file1(output);
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text))
           return;
    QTextStream out(&file1);
print_Json(root);
    int size=ve.size();
   for (int i = 0; i < size; i++)
    {
        out << ve.front();
        ve.dequeue();

    }
   file1.close();
    }

int spa(QString s)
{
    char sp = ' ';
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != sp)
        {
                      return i;
        }
    }
    return -1;
}
void format(QString loc,QString output) {
    QFile file(loc);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;
 QTextStream in(&file);

    QStack<QString>s;
    QQueue<QString>num;
    QString f;
    int nu = 0;
    while (!in.atEnd())
    {
        f = in.readLine();

        int open_tag_second=find(f,'<')+1;
        if (s.empty())
                {
                 f=f.mid(spa(f));
                    num.enqueue(f);
                    int index1 = find(f,' ');
                    if (index1 != -1)
                        f = f.mid(1, index1 - 1) + '>';
                    else
                        f = f.mid(1);
                    s.push(f);
                }
            else
            {
                if (f[spa(f)] != '<')
                {
                    f=f.mid(spa(f));
                    num.enqueue(space(nu + 1) + f);
                }
                else {
                    int index = find(f,'>');
                    if (index != f.length() - 1) {
                        f=f.mid(spa(f));
                        num.enqueue(space(nu + 1) + f);
                    }
                    else {
                        QString co = s.top(), ff;
                        ff = f.mid(open_tag_second+1);
                        if (co == ff&&find(f,'/')!=-1)
                        {

                            ff = space(nu) + "</" + ff;
                            num.enqueue(ff);
                            s.pop();
                            nu--;
                        }
                        else {
                            nu++;
                            f=f.mid(spa(f));
                            num.enqueue(space(nu) + f);
                            int index1 = find(f,' ');
                            if (index1 != -1)
                                f = f.mid(1, index1 - 1) + '>';
                            else
                                f = f.mid(1);
                            s.push(f);
                        }
                    }
                }

            }


    }
    file.close();
    QFile file1(output);
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text))
           return;
    QTextStream out(&file1);

    int size=num.size();
    for (int i = 0; i < size; i++)
    {
        out << num.front();
        if(i!=(size-1))
            out<<'\n';
        num.dequeue();
    }

    file1.close();
}



void consistency(QString FileLocIn,QString output) {

    QFile file(FileLocIn);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;
     QTextStream in(&file);
    QFile file1(output);
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text))
           return;
    QTextStream out(&file1);
    QString err;
    err=output.mid(0,rfind(output,'/')+1)+"Error.txt";
    QFile file2(err);
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
           return;
    QTextStream out2(&file2);
    QStack<QString>s, temp;
    QStack<int>ss;
    QQueue<QString>num, error;
    QString f, sp;
    int u = 0;

    while (!in.atEnd())
    {

        f = in.readLine();

        if (spa(f) != 0 && u == 0)
            u = 1;
        if (s.empty())
        {
            num.enqueue(f);
            error.enqueue(f);
            ss.push(spa(f));
            int index1 = find(f,' ');
            if (index1 != -1)
                f = f.mid(1, index1 - 1) + '>';
            else
                f = f.mid(find(f,'<') + 1);
            s.push(f);
        }
        else
        {
            if (f[spa(f)] != '<')
            {
                num.enqueue(f);
                error.enqueue(f);
            }
            else {
                int index = find(f,'>');
                if (index != f.length() - 1) {
                    if ((ss.top() == spa(f) && u == 1))
                    {
                        num.enqueue(space1(ss.top()) + "</" + s.top());
                        error.enqueue(space1(ss.top()) + "         <-------");
                        s.pop();
                        ss.pop();

                    }
                    if (u == 0 && num.back()[0] != '<')
                    {
                        num.enqueue(space1(ss.top()) + "</" + s.top());
                        error.enqueue(space1(ss.top()) + "         <-------");
                        s.pop();
                        ss.pop();
                    }
                    int ind = rfind(f,'>');
                    if (ind == f.length() - 1) {
                        QString d, d1, r;
                        r = f.mid(0, spa(f));
                        f = f.mid(spa(f));
                        int tt = find(f,' ');
                        if (tt != -1 && tt < find(f,'>')) {
                            d = f.mid(find(f,'<') + 1, find(f,' ') - find(f,'<') - 1);
                            d1 = f.mid(rfind(f,'<') + 2, f.length() - 3 - rfind(f,'<'));
                        }
                        else
                        {
                            d = f.mid(find(f,'<') + 1, find(f,'>') - find(f,'<') - 1);
                            d1 = f.mid(rfind(f,'<') + 2, f.length() - 3 - rfind(f,'<'));
                        }
                        if (d == d1) {
                            num.enqueue(r + f);
                            error.enqueue(r + f);
                        }
                        else
                        {
                            error.enqueue(r + f + "  <--------");

                            f = f.mid(find(f,'<'), rfind(f,'<') - find(f,'<'));
                            num.enqueue(r + f + "</" + d + ">");
                        }
                    }
                    else
                    {
                        QString r = f.mid(0, spa(f));
                        f = f.mid(spa(f));
                        int tt = find(f,' ');
                        if (tt != -1 && tt < find(f,'>'))
                            num.enqueue(r + f + "</" + f.mid(find(f,'<') + 1, (find(f,' ') - find(f,'<') - 1)) + ">");
                        else
                            num.enqueue(r + f + "</" + f.mid(find(f,'<') + 1, (find(f,'>') - find(f,'<'))));
                        error.enqueue(r + f + "  <--------");
                    }
                }
                else {
                    QString co = s.top(), ff;
                    sp = f.mid(0, find(f,'<'));
                    ff = f.mid(find(f,'<') + 2);
                    if (co == ff)
                    {
                        ff = sp + "</" + ff;
                        num.enqueue(ff);
                        error.enqueue(ff);
                        s.pop();
                        ss.pop();

                    }
                    else {
                        int y = 0, yy = 0;
                        if (u == 1 && num.back()[spa(num.back())] != '<')
                        {
                            QString co = s.top(), ff;
                            sp = f.mid(0, find(f,'<'));
                            ff = f.mid(find(f,'<') + 2);
                            if (co == ff) {
                                ff = sp + "</" + ff;
                                num.enqueue(ff);
                                error.enqueue(ff);
                                s.pop();
                                ss.pop();
                            }
                            else {
                                num.enqueue(space1(ss.top()) + "</" + s.top());
                                error.enqueue(space1(ss.top()) + "         <-------");
                                s.pop();
                                ss.pop();
                            }
                        }
                        else {

                            while (!s.empty() && find(f,'/') != -1 && (find(f,'/') - find(f,'<') == 1))
                            {
                                QString co = s.top(), ff1;
                                ff1 = f.mid(find(f,'<') + 2);
                                if (co == ff1)
                                {
                                    y = 1;
                                    break;
                                }
                                y++;
                                temp.push(s.top());
                                s.pop();

                            }


                            while (!temp.empty()) {
                                s.push(temp.top());
                                temp.pop();
                            }
                            if ((ss.top() >= spa(f) && u == 1) && (find(f,'/') == -1 || (find(f,'/') - find(f,'<') > 1)))
                                y = 1;
                            if (y > 1)
                            {
                                f = f.mid(spa(f));
                                num.enqueue(space1(ss.top()) + "</" + s.top());
                                error.enqueue(space1(ss.top()) + f + "   <---------");
                                s.pop();
                                ss.pop();
                            }
                            yy = 0;
                            if (((find(f,'/') != -1 && (find(f,'/') - find(f,'<') == 1)) || (ss.top() >= spa(f) && u == 1)) && y == 1)
                            {
                                yy = 1;
                                if (find(f,'/') != -1 && (find(f,'/') - find(f,'<') == 1)) {
                                    int c = 0;
                                    while (c == 0)
                                    {
                                        ff = f.mid(find(f,'<') + 2);
                                        if (ff == s.top())
                                        {
                                            c = 1;
                                            num.enqueue(space1(ss.top()) + "</" + s.top());
                                            error.enqueue(space1(ss.top())+"</" + s.top());
                                            s.pop();
                                            ss.pop();


                                        }
                                        else
                                        {
                                            num.enqueue(space1(ss.top()) + "</" + s.top());
                                            error.enqueue(space1(ss.top()) + "     <-------");
                                            s.pop();
                                            ss.pop();

                                        }
                                    }

                                }
                                else if (ss.top() >= spa(f))
                                {

                                    if (ss.top() > spa(f)) {
                                        while (ss.top() != spa(f))
                                        {
                                            num.enqueue(space1(ss.top()) + "</" + s.top());
                                            error.enqueue(ss.top() + "         <-------");
                                            s.pop();
                                            ss.pop();

                                        }
                                    }
                                    if (ss.top() == spa(f))
                                    {
                                        num.enqueue(space1(ss.top()) + "</" + s.top());
                                        error.enqueue(ss.top() + "         <-------");
                                        s.pop();
                                        ss.pop();

                                    }
                                    num.enqueue(f);
                                    error.enqueue(f);
                                    ss.push(spa(f));
                                    ff = f.mid(find(f,'<') + 1);

                                    int index1 = find(ff,' ');
                                    if (index1 != -1)
                                        f = f.mid(find(f,'<') + 1, index1) + '>';
                                    else
                                        f = f.mid(find(f,'<') + 1);
                                    s.push(f);

                                }

                            }
                        }
                        if ((find(f,'/') == -1 || (find(f,'/') - find(f,'<') > 1)) && yy == 0) {
                            num.enqueue(f);
                            error.enqueue(f);
                            ss.push(spa(f));
                            ff = f.mid(find(f,'<') + 1);
                            int index1 = find(ff,' ');
                            if (index1 != -1)
                                f = ff.mid(0, index1) + '>';
                            else
                                f = f.mid(find(f,'<') + 1);
                            s.push(f);
                        }
                    }
                }
            }

        }


    }
    if (!s.empty())
    {
        while (!s.empty())
        {
            num.enqueue(space1(ss.top()) + "</" + s.top());
            error.enqueue(space1(ss.top()) + "         <-------");
            s.pop();
            ss.pop();

        }
    }
    int size = num.size();

    for (int i = 0; i < size; i++)
    {
        out << num.front();
        if(i!=(size-1))
            out<<'\n';
        out2 << error.front() << '\n';
        num.dequeue();
        error.dequeue();
    }

file.close();
file1.close();
file2.close();
}

void MainWindow::on_run_clicked()
{
    Tree t;

    QString s=ui->in->text();
    QString temp=s;
    QString e=ui->out->text();

   t.insert(s);

    if(ui->Consistency->isChecked()){
        consistency(s,e);
        s=e;
        QString err=e.mid(0,rfind(e,'/')+1)+"Error.txt";
       QFile file3(err);
           if (!file3.open(QIODevice::ReadOnly | QIODevice::Text))
                  return;
         QTextStream in2(&file3);
         while (!in2.atEnd())
         {
             QString c = in2.readLine();
             ui->error->append(c);

         }
    }
      if(ui->format->isChecked()){
      format(s,e);
      s=e;
      }

      if(ui->minify->isChecked()){

          t.print_minify(e);
          s=e;
      }
      if(ui->json->isChecked()){

        t.print_Json(e);
        s=e;
      }
     QFile file(e);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
               return;
      QTextStream in(&file);
      QFile file1(temp);
      if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
             return;
    QTextStream in1(&file1);
      QString f;
      QString f1;
      ui->output->setText("");
      ui->input->setText("");
      while (!in1.atEnd())
      {
         f1 = in1.readLine();
          ui->input->append(f1);
      }
      while (!in.atEnd())
      {
          f = in.readLine();
          ui->output->append(f);

      }
}

