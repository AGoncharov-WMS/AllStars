//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "jpeg.hpp"
#include <stack>
#include "inifiles.hpp"
#include <algorithm>
#include "bass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

HSTREAM strs[4];
HMUSIC mods[3];
extern bool fpt=false,sn=true;
extern int score=0,xt=0,yt=0,knn=0,all_num=0,stp=1,nom_mod=0;
class cell
{
 public:
        int cell_x;
        int cell_y;
 private:
};
cell tmp_cell;
//---------------------------------------------------------------------------

std::stack<cell> path;
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadFJPG(AnsiString astr)
{
 TJPEGImage *examp=new TJPEGImage;
 TMemoryStream *MemStream=new TMemoryStream ;
 Graphics::TBitmap * Bitmap = new Graphics::TBitmap;
 MemStream->LoadFromFile(astr);
 MemStream->Seek(0,0);
 examp->LoadFromStream(MemStream);
 Bitmap->Assign(examp);
 Pics->Picture->Assign(Bitmap);
 delete MemStream;
 delete examp;
 delete Bitmap;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Tetris
//---------------------------------------------------------------------------
void __fastcall TForm1::RotFig(bool wher)
{
 int nb=max(wf,hf),x_x=x_old/26,y_y=y_old/26,kn,dob=0;
 kn=y_old-y_y*26;
 kn==0?dob=0:dob=1;
 y_y=y_y+4-nb;
 if (x_x+nb>10)
  return;
 for (int i=y_y;i<y_y+nb+dob;i++)
  for (int j=x_x;j<x_x+nb;j++)
   if (mzzz[i][j])
    return;
 bool k[7][7];
 for (int i=0;i<7;i++)
  for (int j=0;j<7;j++)
  {
   if(wher)
    k[i][j]=tf[j][6-i];
   else
    k[i][j]=tf[6-j][i];
  }
 for(int i=0;i<7;i++)
  for(int j=0;j<7;j++)
   tf[i][j]=k[i][j];
 FromTempTo(false);
 DrawInN(Tmp);
 swap(wf,hf);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CreateNewFigure(void)
{
 int xt=3,yt=3,k=0;
 randomize();
 for (int i=0;i<7;i++)
  for (int j=0;j<7;j++)
   tf[i][j]=false;
 do
 {
  if (tf[xt][yt]==false)
  {
   tf[xt][yt]=true;
   k++;
  }
  switch(random(4))
  {
   case 0: xt++; break;
   case 1: xt--; break;
   case 2: yt++; break;
   case 3: yt--; break;
  }
 }
 while (k<4);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawInN(TObject *Send)
{
 for (int i=0;i<4;i++)
  for (int j=0;j<4;j++)
  {
   if ((nf[i][j] && ((TImage *)Send)->Name=="NextBall3") || (rf[i][j] && ((TImage *)Send)->Name=="Tmp") )
    ((TImage *)Send)->Canvas->CopyRect(Rect(j*26,i*26,j*26+26,i*26+26),Pics->Canvas,Rect(0,0,26,26));
   else
    ((TImage *)Send)->Canvas->CopyRect(Rect(j*26,i*26,j*26+26,i*26+26),Pics->Canvas,Rect(4,4,5,5));
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FromNewToReal(void)
{
 for (int i=0;i<4;i++)
  for (int j=0;j<4;j++)
   rf[i][j]=nf[i][j];
 for (int j=3;j>=0;j--)
  for (int i=0;i<4;i++)
   if (rf[i][j])
   {
    wf=j+1;
    goto sdg;
   }
sdg:
 for (int i=0;i<4;i++)
  for (int j=0;j<4;j++)
   if (rf[i][j])
   {
    hf=3-i+1;
    goto sdg2;
   }
sdg2:
 Tmp->Picture=NextBall3->Picture;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FromRealToTemp(void)
{
 for (int i=0;i<7;i++)
  for (int j=0;j<7;j++)
   tf[i][j]=false;
 for (int i=0;i<4;i++)
  for (int j=0;j<4;j++)
   tf[i+1][j+1]=rf[i][j];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FromTempTo(bool wher)
{
 int nx,ny,nx1,ny1;
 for (int i=6;i>0;i--)
  for (int j=0;j<7;j++)
   if (tf[i][j]==true)
   {
    ny=i;
    nx=j;
    goto mzxc1;
   }
mzxc1:
 for (int j=0;j<7;j++)
  for (int i=6;i>0;i--)
   if (tf[i][j]==true)
   {
    nx1=j;
    ny1=i;
    goto mzxc2;
   }
mzxc2:
 if (nx1<nx)
  nx=nx1;
 if (ny1>ny)
  ny=ny1;
 nx1=nx;
 ny1=ny;
 for (int i=3;i>=0;i--,ny--,nx=nx1)
  for (int j=0;j<4;j++)
  {
   if (wher)
   {
    if (nx<7 && ny>=0)
     nf[i][j]=tf[ny][nx];
    else
     nf[i][j]=false;
   }
   else
   {
    if (nx<7 && ny>=0)
     rf[i][j]=tf[ny][nx];
    else
     rf[i][j]=false;
   }
   nx++;
  }
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::CheckOnStopDown(void)
{
 int x_x=x_old/26,y_y=(y_old+104)/26;
 for (int j=3;j>=0;j--,y_y--)
  for (int i=x_x,nk=0;i<x_x+4;i++,nk++)
   if (y_y>=0 && mzzz[y_y][i] && rf[j][nk])
    return true;
 return false;
}
//---------------------------------------------------------------------------

bool __fastcall TForm1::CheckOnStopLeft(void)
{
 int x_x=x_old/26,y_y=y_old/26,kn;
 x_x--;
 if (x_x<0)
  return true;
 kn=y_old-y_y*26;
 for (int j=0;j<4;j++)
  if ((rf[0][j] && mzzz[y_y+0][x_x+j]) ||
      (rf[0][j] && mzzz[y_y+1][x_x+j] && kn!=0) ||
      (rf[1][j] && mzzz[y_y+1][x_x+j]) ||
      (rf[1][j] && mzzz[y_y+2][x_x+j] && kn!=0) ||
      (rf[2][j] && mzzz[y_y+2][x_x+j]) ||
      (rf[2][j] && mzzz[y_y+3][x_x+j] && kn!=0) ||
      (rf[3][j] && mzzz[y_y+3][x_x+j]) ||
      (rf[3][j] && mzzz[y_y+4][x_x+j] && kn!=0)
     )
     return true;
 return false;
}
//---------------------------------------------------------------------------

bool __fastcall TForm1::CheckOnStopRight(void)
{
 int x_x=x_old/26,y_y=y_old/26,kn;
 if (x_x>9-wf)
  return true;
 kn=y_old-y_y*26;
 for (int j=x_x+wf-1,nb=wf-1;j>=x_x;j--,nb--)
  if ((rf[0][nb] && mzzz[y_y+0][j+1]) ||
      (rf[0][nb] && mzzz[y_y+1][j+1] && kn!=0) ||
      (rf[1][nb] && mzzz[y_y+1][j+1]) ||
      (rf[1][nb] && mzzz[y_y+2][j+1] && kn!=0) ||
      (rf[2][nb] && mzzz[y_y+2][j+1]) ||
      (rf[2][nb] && mzzz[y_y+3][j+1] && kn!=0) ||
      (rf[3][nb] && mzzz[y_y+3][j+1]) ||
      (rf[3][nb] && mzzz[y_y+4][j+1] && kn!=0)
     )
     return true; 
 return false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Lines Color
//---------------------------------------------------------------------------
// Передаются координаты точек которые необходимо проверить
bool __fastcall TForm1::CheckingHole_Cl(deque<int> x_k,deque<int> y_k)
{
 deque<int> x_p(0);
 deque<int> y_p(0);
 deque<int> xx_p(0);
 bool rez=false;
 int x,y;
cik:
  while (!x_k.empty())
  {
   x=x_k.front();
   y=y_k.front();
   int xx,yy,k,pk=1,xp,yp;
// Цикл по 4 направлениям света
   for (int j=0;j<4;j++)
   {
    switch (j)
    {
     case 0: xp=-1; yp=0; break;
     case 1: xp=-1; yp=-1; break;
     case 2: xp=0; yp=-1; break;
     case 3: xp=1; yp=-1; break;
    }
    xx=x; yy=y; k=1;
    for (int i=0;i<2;i++)
    {
     xx=x+xp;
     yy=y+yp;
     while (mzz[xx][yy]==mzz[x][y] && xx>=0 && yy>=0 && xx<=8 && yy<=13)
     {
      x_p.push_front(xx);
      y_p.push_front(yy);
      k++;
      xx+=xp;
      yy+=yp;
     }
     xp=-xp;
     yp=-yp;
    }
    if (k>=3)
    {
     score+=k;
     Label6->Caption="Результат: "+IntToStr(score);
     if (score>GScr->Tag)
     {
      GScr->Tag+=150;
      if (Timer2->Interval-1>0 && Timer2->Tag==1)
       Timer2->Interval--;
      else
      {
       Timer2->Interval=15;
       if (Timer2->Tag+1<=10)
        Timer2->Tag++;
       else
        Timer2->Tag=10;
      }
     }
     pk+=k;
    }
    if (k==2)
    {
     x_p.pop_front();
     y_p.pop_front();
    }
    k=1;
   }
   if (pk>=3)
   {
    rez=true;
    x_p.push_front(x);
    y_p.push_front(y);
   }
   x_k.pop_front();
   y_k.pop_front();
  }
// Удаление линий
  while (!x_p.empty())
  {
   mzz[x_p.front()][y_p.front()]=-1;
   BASS_ChannelStop(strs[3]);
   BASS_ChannelStop(strs[1]);
   if (sn && N6->Checked)
    BASS_ChannelPlay(strs[1],false);
   if (count(xx_p.begin(),xx_p.end(),x_p.front())==0)
    xx_p.push_front(x_p.front());
   x_p.pop_front();
   y_p.pop_front();
  }
// Сдвижка массива
  while (!xx_p.empty())
  {
   for (int j=12;j>=0;j--)
    if (mzz[xx_p.front()][j]!=-1)
    {
     int jj=j+1;
     while (mzz[xx_p.front()][jj]==-1)
     {
      swap(mzz[xx_p.front()][jj-1],mzz[xx_p.front()][jj]);
      if (jj+1>13)
       break;
      else
       jj++;
     }
    }
   for (int j=0;j<14;j++)
   {
    if (mzz[xx_p.front()][j]!=-1)
    {
     x_k.push_front(xx_p.front());
     y_k.push_front(j);
    }
   }
   xx_p.pop_front();
  }
// Если была сдвижка, то возобновить проверку на линии
  if (!x_k.empty())
   goto cik;
 return rez;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FindNewBalls_Cl(void)
{
 Tmp3->Canvas->CopyMode=cmSrcCopy;
 NextBall2->Canvas->CopyMode=cmSrcCopy;
 randomize();
 for (int i=0;i<3;i++)
 {
  col[i]=random(5);
  Tmp3->Canvas->CopyRect(Rect(0,0,32,32),Pics->Canvas,Rect(col[i]*32,0,col[i]*32+32,32));
  NextBall2->Canvas->CopyRect(Rect(0,i*32,32,i*32+32),Tmp3->Canvas,Rect(0,0,32,32));
 }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Lines
//---------------------------------------------------------------------------
void __fastcall TForm1::FindPath(int xx,int yy,int colr)
{
 tmp_cell.cell_x=xx;
 tmp_cell.cell_y=yy;
 path.push(tmp_cell);
 mzz[xx][yy]=-99;
 if (mzz[xx-1][yy]==colr && xx-1>=0)
  FindPath(xx-1,yy,colr);
 if (mzz[xx][yy-1]==colr && yy-1>=0)
  FindPath(xx,yy-1,colr);
 if (mzz[xx+1][yy]==colr && xx+1<18)
  FindPath(xx+1,yy,colr);
 if (mzz[xx][yy+1]==colr && yy+1<18)
  FindPath(xx,yy+1,colr);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LineD(int x,int y,int x1,int y1,TImage *Fon)
{
 Fon->Canvas->MoveTo(x,y);
 Fon->Canvas->LineTo(x1,y1);
}
//---------------------------------------------------------------------------

int __fastcall TForm1::CheckForLines(int x,int y,int xp,int yp)
{
 int xx=x,yy=y,k=1;
 for (int i=0;i<4;i++)
 {
  if (i>1 && k<5)
   break;
  xx=x+xp;
  yy=y+yp;
  while (mz[xx][yy]==mz[x][y])
  {
   if (i==0 || i==1)
    k++;
   else
   {
    SlowDestr(xx,yy);
    mz[xx][yy]=-1;
    DrawInHole(xx,yy,mz[xx][yy]);
   }
   xx+=xp;
   yy+=yp;
  }
  xp=-xp;
  yp=-yp;
 }
 if (k>=5)
  return k;
 else
  return 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawInHole(int x,int y,int cl)
{
 Tmp->Canvas->CopyMode=cmSrcCopy;
 int zxx,zyy;
 if (x==xt && y==yt && Timer2->Enabled==true)
 {
  zxx=zx;
  zyy=zy;
 }
 else
 {
  zxx=zyy=0;
 }
 switch (cl)
 {
  case cBlue:   Tmp->Canvas->CopyRect(Rect(0+zxx,0+zyy,55-zxx,55-zyy),Pics->Canvas,Rect(2,0,57,55));     break;
  case cGray:   Tmp->Canvas->CopyRect(Rect(0+zxx,0+zyy,55-zxx,55-zyy),Pics->Canvas,Rect(56,0,111,55));   break;
  case cGreen:  Tmp->Canvas->CopyRect(Rect(0+zxx,0+zyy,55-zxx,55-zyy),Pics->Canvas,Rect(110,0,165,55));  break;
  case cPurple: Tmp->Canvas->CopyRect(Rect(0+zxx,0+zyy,55-zxx,55-zyy),Pics->Canvas,Rect(164,0,219,55));   break;
  case cRed:    Tmp->Canvas->CopyRect(Rect(0+zxx,0+zyy,55-zxx,55-zyy),Pics->Canvas,Rect(218,0,273,55));  break;
  case cYellow: Tmp->Canvas->CopyRect(Rect(0+zxx,0+zyy,55-zxx,55-zyy),Pics->Canvas,Rect(272,0,327,55)); break;
  default:      Tmp->Canvas->CopyMode=cmBlackness;
                Tmp->Canvas->CopyRect(Rect(0,0,55,55),Pics->Canvas,Rect(0,0,1,1)); break;
 }
 if (x!=77)
 {
  GScr->Canvas->CopyMode=cmSrcCopy;
  GScr->Canvas->CopyRect(Rect(x*60+3,y*60+3,x*60+58,y*60+58),Tmp->Canvas,Rect(0,0,55,55));
 }
 else
 {
  NextBall->Canvas->CopyMode=cmSrcCopy;
  if (y==0)
   NextBall->Canvas->CopyRect(Rect(0,0,55,55),Tmp->Canvas,Rect(0,0,55,55));
  if (y==1)
   NextBall->Canvas->CopyRect(Rect(55,0,110,55),Tmp->Canvas,Rect(0,0,55,55));
  if (y==2)
   NextBall->Canvas->CopyRect(Rect(110,0,165,55),Tmp->Canvas,Rect(0,0,55,55));
 }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Memory
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawInHole_M(int x,int y,int cl)
{
 Tmp->Canvas->CopyMode=cmSrcCopy;
 switch (cl)
 {
  case -1:   Tmp->Canvas->CopyRect(Rect(0,0,55,55),Image1->Canvas,Rect(0,0,55,55));     break;
  case -10:  Tmp->Canvas->CopyMode=cmBlackness;
             Tmp->Canvas->CopyRect(Rect(0,0,55,55),Image1->Canvas,Rect(0,0,1,1)); break;
  default:   Tmp->Canvas->CopyRect(Rect(0,0,55,55),Pics->Canvas,Rect(mz[x][y]*55,0,mz[x][y]*55+55,55));   break;
 }
 GScr->Canvas->CopyMode=cmSrcCopy;
 GScr->Canvas->CopyRect(Rect(x*60+3,y*60+3,x*60+58,y*60+58),Tmp->Canvas,Rect(0,0,55,55));
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  River
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawInHole_R(int x,int y,int cl)
{
 Tmp->Canvas->CopyMode=cmSrcCopy;
 switch (cl)
 {
  case -1:   Tmp->Canvas->CopyMode=cmBlackness;
             Tmp->Canvas->CopyRect(Rect(0,0,39,39),Image1->Canvas,Rect(0,0,1,1)); break;
  default:   Tmp->Canvas->CopyRect(Rect(0,0,39,39),Pics->Canvas,Rect(mzz[x][y]*39,0,mzz[x][y]*39+39,39));   break;
 }
 GScr->Canvas->CopyMode=cmSrcCopy;
 GScr->Canvas->CopyRect(Rect(x*39-3,y*39+75,x*39+39-3,y*39+39+75),Tmp->Canvas,Rect(0,0,39,39));
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Click
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawInHole_C(int x,int y,int cl)
{
 Tmp->Canvas->CopyMode=cmSrcCopy;
 switch (cl)
 {
  case -10:  Tmp->Canvas->CopyMode=cmBlackness;
             Tmp->Canvas->CopyRect(Rect(0,0,30,30),Image1->Canvas,Rect(0,0,1,1)); break;
  default:   Tmp->Canvas->CopyRect(Rect(0,0,30,30),Pics->Canvas,Rect(mzz[x][y]*30,0,mzz[x][y]*30+30,30));   break;
 }
 GScr->Canvas->CopyMode=cmSrcCopy;
 GScr->Canvas->CopyRect(Rect(x*30,y*30,x*30+30,y*30+30),Tmp->Canvas,Rect(0,0,30,30));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FreshTbl(void)
{
 AnsiString nm;
 int n;
 if (Form1->Tag==1 || Form1->Tag==4 || Form1->Tag==5)
 {
  n=1;
  while (score<StrToInt(ValueListEditor1->Cells[1][n]))
   n++;
 }
 if (Form1->Tag==2 || Form1->Tag==3)
 {
  n=1;
  while (Label3->Tag>StrToInt(ValueListEditor1->Cells[1][n]))
   n++;
 }
 InputQuery("Вы попали в таблицу рекордов!","Введите имя:",nm);
 for (int i=1;i<=nm.Length();i++)
  if (nm[i]=='=')
   nm[i]='_';
 TStringList* nms=new TStringList();
 nms->AddStrings(ValueListEditor1->Strings);
 if (Form1->Tag==1 || Form1->Tag==4  || Form1->Tag==5)
  nm=nm+"="+IntToStr(score);
 if (Form1->Tag==2 || Form1->Tag==3)
  nm=nm+"="+Label3->Caption;
 nms->Insert(n-1,nm);
 nms->Delete(10);
 ValueListEditor1->Strings=nms;
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
 TIniFile *ini;
 ini = new TIniFile(ssm+"table.ini");
 for (int i=1;i<=10;i++)
 {
  switch (Form1->Tag)
  {
   case 1: ini->WriteString( "Lines",  "Rank"+IntToStr(i), nms->Strings[i-1]); break;
   case 2:
           if (knn==4)
            ini->WriteString( "Memory",  "Rank"+IntToStr(i), nms->Strings[i-1]);
           else
            ini->WriteString( "Memory2",  "Rank"+IntToStr(i), nms->Strings[i-1]);
           break;
   case 3:
           if (knn==4)
            ini->WriteString( "Click",  "Rank"+IntToStr(i), nms->Strings[i-1]);
           if (knn==5)
            ini->WriteString( "Click2",  "Rank"+IntToStr(i), nms->Strings[i-1]);
           if (knn==6)
            ini->WriteString( "Click3",  "Rank"+IntToStr(i), nms->Strings[i-1]);
           break;
   case 4: ini->WriteString( "Color",  "Rank"+IntToStr(i), nms->Strings[i-1]); break;
   case 5: ini->WriteString( "Tetris",  "Rank"+IntToStr(i), nms->Strings[i-1]); break;
  }
 }
 delete ini;
 ValueListEditor1->Refresh();
 delete nms;
 score=0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Lines
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateNewBalls(void)
{
 randomize();
 int x,y;
 int endgm=0;
 for (int i=0;i<9;i++)
  for (int j=0;j<9;j++)
   if (mz[i][j]==-1)
    endgm++;
 if (endgm<4)
 {
  if (score>StrToInt(ValueListEditor1->Cells[1][10]))
   FreshTbl();
  else
   ShowMessage(" В таблицу ты не попал :-(");
  GScr->Enabled=false;  
  return;
 }
 int x_m[3],y_m[3];
 for (int i=0;i<3;i++)
 {
  x=random(9);
  y=random(9);
  if (mz[x][y]<0)
  {
   mz[x][y]=col[i];
   SlowVis(x,y);
   DrawInHole(x,y,mz[x][y]);
   x_m[i]=x;
   y_m[i]=y;
  }
  else
   i--;
 }
 for (int i=0;i<3;i++)
  if (CheckingHole(x_m[i],y_m[i]))
  {
   if (sn && N6->Checked)
    BASS_ChannelPlay(strs[0],false);
   SlowDestr(x_m[i],y_m[i]);
   mz[x_m[i]][y_m[i]]=-1;
   DrawInHole(x_m[i],y_m[i],mz[x_m[i]][y_m[i]]);
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Lines Color
//---------------------------------------------------------------------------
void __fastcall TForm1::FindNewBalls(void)
{
 randomize();
 for (int i=0;i<3;i++)
 {
  col[i]=random(6);
  DrawInHole(77,i,col[i]);
 }
}
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
 FreeLibrary(GetModuleHandle("OleAut32"));
 FreeLibrary(GetModuleHandle("Ole32"));
 randomize();
 Form1->Color=clBlack;
 Fon->Canvas->Pen->Width=1;
 Fon->Canvas->Brush->Bitmap=NULL;
 Fon->Canvas->Brush->Color=clBlack;
 Fon->Canvas->FillRect(Rect(0,0,Fon->Width,Fon->Height));
 Fon->Canvas->Pen->Color=clBlack;
 Fon->Canvas->Rectangle(0,0,Fon->Width,Fon->Height);
 Fon->Canvas->Pen->Color=clSilver;
 int x=Fon->Width/9;
 for (int i=0;i<8;i++)
 {
  LineD(x,0,x,Fon->Height,Fon);
  LineD(0,x,Fon->Width,x,Fon);
  x+=Fon->Width/9;
 }
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
 AnsiString ssm2=ssm+"allstars.ini";
 if (!FileExists(ssm2))
 {
  TIniFile *ini;
  ini = new TIniFile(ssm+"allstars.ini");
  ini->WriteInteger( "Options", "PlaySound", 1 );
  ini->WriteInteger( "Options", "PlayMusic", 1 );
  N6->Checked=true;
  N2->Checked=true;
  delete ini;
 }
 else
 {
  TIniFile *ini;
  ini = new TIniFile(ssm+"allstars.ini");
  N6->Checked=ini->ReadInteger( "Options", "PlaySound", 1 );
  N2->Checked=ini->ReadInteger( "Options", "PlayMusic", 1 );
  delete ini;
 }
 if (!FileExists(ssm+"table.ini"))
 {
  TIniFile *ini2;
  ini2 = new TIniFile(ssm+"table.ini");
  for (int i=1;i<=10;i++)
  {
   ini2->WriteString( "Lines",  "Rank"+IntToStr(i), "Stranger=2");
   ini2->WriteString( "Color",  "Rank"+IntToStr(i), "Stranger=2");
   ini2->WriteString( "Tetris",  "Rank"+IntToStr(i), "Stranger=2");
   ini2->WriteString( "River",  "Rank"+IntToStr(i), "Stranger=2");
   ini2->WriteString( "Memory",  "Rank"+IntToStr(i), "Stranger=9999");
   ini2->WriteString( "Memory2",  "Rank"+IntToStr(i), "Stranger=9999");
   ini2->WriteString( "Click",  "Rank"+IntToStr(i), "Stranger=900");
   ini2->WriteString( "Click2",  "Rank"+IntToStr(i), "Stranger=900");
   ini2->WriteString( "Click3",  "Rank"+IntToStr(i), "Stranger=900");
   ini2->WriteString( "Swaper",  "Rank"+IntToStr(i), "Stranger=2");
   ini2->WriteString( "Swaper2",  "Rank"+IntToStr(i), "Stranger=2");
  }
  delete ini2;
 }
 for (int i=0;i<9;i++)
  for (int j=0;j<9;j++)
   mz[i][j]=-1;
 Application->OnIdle = &WhenIdle;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WhenIdle(TObject *Sender, bool &Done)
{
 Application->ProcessMessages();
 Done = false;
 if (Form1->Tag==1)
 {
  for (int i=0;i<9;i++)
   for (int j=0;j<9;j++)
    DrawInHole(i,j,mz[i][j]);
 }
 if (Form1->Tag==2)
 {
  for (int i=0;i<9;i++)
   for (int j=0;j<9;j++)
    DrawInHole_M(i,j,mzz[i][j]);
 }
 if (Form1->Tag==3)
 {
  for (int i=0;i<18;i++)
   for (int j=0;j<18;j++)
    DrawInHole_C(i,j,mzz[i][j]);
 }
 if (Form1->Tag==4)
 {
  Tmp->Canvas->CopyMode=cmBlackness;
  Tmp->Canvas->CopyRect(Rect(0,0,288,448),Pics->Canvas,Rect(0,0,1,1));
  Tmp->Canvas->CopyMode=cmSrcCopy;
  for (int i=0;i<9;i++)
   for (int j=0;j<14;j++)
    if (mzz[i][j]>=0)
     Tmp->Canvas->CopyRect(Rect(i*32,j*32,i*32+32,j*32+32),Pics->Canvas,Rect(mzz[i][j]*32,0,mzz[i][j]*32+32,32));
  for (int i=0;i<3;i++)
   Tmp->Canvas->CopyRect(Rect(x_old,y_old+i*32,x_old+32,y_old+32+i*32),Pics->Canvas,Rect(cols[i]*32,0,cols[i]*32+32,32));
  GScr->Canvas->CopyMode=cmSrcCopy;
  GScr->Canvas->Pen->Color=clBlue;
  GScr->Canvas->Pen->Width=6;
  GScr->Canvas->Brush->Style=bsClear;
  GScr->Canvas->RoundRect(123,40,418,495,10,10);
  GScr->Canvas->CopyRect(Rect(126,43,414,491),Tmp->Canvas,Rect(0,0,288,448));
 }
 if (Form1->Tag==5)
 {
  Tmp3->Canvas->CopyMode=cmBlackness;
  Tmp3->Canvas->CopyRect(Rect(0,0,260,520),Tmp->Canvas,Rect(0,0,260,520));
  Tmp3->Canvas->CopyMode=cmSrcCopy;
  Tmp3->Canvas->CopyRect(Rect(x_old,y_old,x_old+104,y_old+104),Tmp->Canvas,Rect(0,0,104,104));
  for (int i=0;i<20;i++)
   for (int j=0;j<10;j++)
   {
    if (mzzz[i][j])
     Tmp3->Canvas->CopyRect(Rect(j*26,i*26,j*26+26,i*26+26),Pics->Canvas,Rect(26,0,52,26));
   }
  GScr->Canvas->Pen->Color=clBlue;
  GScr->Canvas->Pen->Width=6;
  GScr->Canvas->Brush->Style=bsClear;
  GScr->Canvas->RoundRect(137,7,404,534,10,10);
  GScr->Canvas->CopyRect(Rect(140,10,400,530),Tmp3->Canvas,Rect(0,0,260,520));
 }
 if (Form1->Tag==6)
 {
  for (int i=0;i<14;i++)
   for (int j=0;j<9;j++)
    DrawInHole_R(i,j,mzz[i][j]);
 }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Lines
//---------------------------------------------------------------------------
bool __fastcall TForm1::CheckingHole(int x,int y)
{
 int k=CheckForLines(x,y,-1,0)+CheckForLines(x,y,-1,-1)+
       CheckForLines(x,y,0,-1)+CheckForLines(x,y,1,-1);
 if (k>=5)
 {
  score+=((k-5)*2+5);
  Label6->Caption="Результат: "+IntToStr(score);
  return true;
 }
 else
  return false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SlowVis(int x1,int y1)
{
 int x=27;
 int y=27;
 int zx=27;
 int zy=27;
 while (x>0)
 {
  Tmp2->Canvas->CopyMode=cmBlackness;
  Tmp2->Canvas->CopyRect(Rect(0,0,55,55),Pics->Canvas,Rect(0,0,1,1));
  Tmp2->Canvas->CopyMode=cmSrcCopy;
  switch (mz[x1][y1])
  {
   case cBlue:   Tmp2->Canvas->CopyRect(Rect(x--,y--,zx++,zy++),Pics->Canvas,Rect(2,0,57,55));     break;
   case cGray:   Tmp2->Canvas->CopyRect(Rect(x--,y--,zx++,zy++),Pics->Canvas,Rect(56,0,111,55));   break;
   case cGreen:  Tmp2->Canvas->CopyRect(Rect(x--,y--,zx++,zy++),Pics->Canvas,Rect(110,0,165,55));  break;
   case cPurple: Tmp2->Canvas->CopyRect(Rect(x--,y--,zx++,zy++),Pics->Canvas,Rect(164,0,219,55));   break;
   case cRed:    Tmp2->Canvas->CopyRect(Rect(x--,y--,zx++,zy++),Pics->Canvas,Rect(218,0,273,55));  break;
   case cYellow: Tmp2->Canvas->CopyRect(Rect(x--,y--,zx++,zy++),Pics->Canvas,Rect(272,0,327,55)); break;
  }
  GScr->Canvas->CopyMode=cmSrcCopy;
  GScr->Canvas->CopyRect(Rect(x1*60+3,y1*60+3,x1*60+58,y1*60+58),Tmp2->Canvas,Rect(0,0,55,55));
  Sleep(1);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SlowDestr(int x1,int y1)
{
 int x=0;
 int y=0;
 int zx=55;
 int zy=55;
 while (x<zx)
 {
  Tmp2->Canvas->CopyMode=cmBlackness;
  Tmp2->Canvas->CopyRect(Rect(0,0,55,55),Pics->Canvas,Rect(0,0,1,1));
  Tmp2->Canvas->CopyMode=cmSrcCopy;
  switch (mz[x1][y1])
  {
   case cBlue:   Tmp2->Canvas->CopyRect(Rect(x++,y++,zx--,zy--),Pics->Canvas,Rect(2,0,57,55));     break;
   case cGray:   Tmp2->Canvas->CopyRect(Rect(x++,y++,zx--,zy--),Pics->Canvas,Rect(56,0,111,55));   break;
   case cGreen:  Tmp2->Canvas->CopyRect(Rect(x++,y++,zx--,zy--),Pics->Canvas,Rect(110,0,165,55));  break;
   case cPurple: Tmp2->Canvas->CopyRect(Rect(x++,y++,zx--,zy--),Pics->Canvas,Rect(164,0,219,55));   break;
   case cRed:    Tmp2->Canvas->CopyRect(Rect(x++,y++,zx--,zy--),Pics->Canvas,Rect(218,0,273,55));  break;
   case cYellow: Tmp2->Canvas->CopyRect(Rect(x++,y++,zx--,zy--),Pics->Canvas,Rect(272,0,327,55)); break;
  }
  GScr->Canvas->CopyMode=cmSrcCopy;
  GScr->Canvas->CopyRect(Rect(x1*60+3,y1*60+3,x1*60+58,y1*60+58),Tmp2->Canvas,Rect(0,0,55,55));
  Sleep(1);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GScrMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if (Form1->Tag==1)
 {
  deque<int> x_p(0);
  deque<int> y_p(0);
  int xx,yy,proh;
  if (mz[X/60][Y/60]>=0)
  {
   for (int i=0;i<9;i++)
    for (int j=0;j<9;j++)
     mzz[i][j]=mz[i][j];
   x_p.push_back(X/60);
   y_p.push_back(Y/60);
   if (fpt==true)
    DrawInHole(x_old,y_old,mz[x_old][y_old]);
   x_old=xt=X/60;
   y_old=yt=Y/60;
   Timer2->Enabled=true;
   mzz[X/60][Y/60]=-10;
   while (!x_p.empty())
   {
    xx=x_p.front();
    yy=y_p.front();
    proh=mzz[x_p.front()][y_p.front()];
    if (mzz[xx-1][yy]==-1 && xx-1>=0)
    {
     mzz[xx-1][yy]=proh-1;
     x_p.push_back(xx-1);
     y_p.push_back(yy);
    }
    if (mzz[xx+1][yy]==-1 && xx+1<9)
    {
     mzz[xx+1][yy]=proh-1;
     x_p.push_back(xx+1);
     y_p.push_back(yy);
    }
    if (mzz[xx][yy-1]==-1 && yy-1>=0)
    {
     mzz[xx][yy-1]=proh-1;
     x_p.push_back(xx);
     y_p.push_back(yy-1);
    }
    if (mzz[xx][yy+1]==-1 && yy+1<9)
    {
     mzz[xx][yy+1]=proh-1;
     x_p.push_back(xx);
     y_p.push_back(yy+1);
    }
    x_p.pop_front();
    y_p.pop_front();
   }
   fpt=true;
   return;
  }
  xx=X/60;
  yy=Y/60;
  if (fpt==true)
  {
   if (mzz[xx][yy]==-1)
   {
    if (sn && N6->Checked)
     BASS_ChannelPlay(strs[2],false);
    return;
   }
   if (mzz[xx][yy]<-10)
   {
    Timer2->Enabled=false;
    zx=zy=0;
    SlowDestr(x_old,y_old);
    mz[xx][yy]=mz[x_old][y_old];
    mz[x_old][y_old]=-1;
    DrawInHole(x_old,y_old,-1);
    SlowVis(xx,yy);
    DrawInHole(xx,yy,mz[xx][yy]);
    fpt=false;
    if (CheckingHole(xx,yy))
    {
     if (sn && N6->Checked)
      BASS_ChannelPlay(strs[0],false);
     SlowDestr(xx,yy);
     mz[xx][yy]=-1;
     DrawInHole(xx,yy,-1);
    }
    else
    {
     CreateNewBalls();
     FindNewBalls();
    }
   }
  }
 }
 if (Form1->Tag==2)
 {
  int xkl=X/60,ykl=Y/60;
  if (mz[xkl][ykl]>=0)
  {
   mzz[xkl][ykl]=mz[xkl][ykl];
   if (x_old==-1)
   {
    BASS_ChannelStop(strs[1]);
    if (sn && N6->Checked)
     BASS_ChannelPlay(strs[1],false);
    x_old=X/60;
    y_old=Y/60;
    return;
   }
   else
   {
    if (x_old==xkl && y_old==ykl)
     return;
    BASS_ChannelStop(strs[1]);
    if (sn && N6->Checked)
     BASS_ChannelPlay(strs[1],false);
    DrawInHole_M(xkl,ykl,mzz[xkl][ykl]);
    Sleep(500);
    if (mz[x_old][y_old]==mz[xkl][ykl])
    {
     mz[x_old][y_old]=mz[xkl][ykl]=-10;
     mzz[x_old][y_old]=mzz[xkl][ykl]=-10;
     all_num-=2;
     BASS_ChannelStop(strs[1]);
     if (sn && N6->Checked)
      BASS_ChannelPlay(strs[0],false);
     if (all_num==0)
     {
      Timer2->Enabled=false;
      if (Label3->Tag<StrToInt(ValueListEditor1->Cells[1][10]))
       FreshTbl();
      else
       ShowMessage("В таблицу ты не попал :-(");
      GScr->Enabled=false;
      return;
     }
    }
    else
    {
     mzz[x_old][y_old]=mzz[xkl][ykl]=-1;
    }
    x_old=y_old=-1;
   }
  }
 }
 if (Form1->Tag==3)
 {
  if (mzz[X/30][Y/30]==-10)
   return;
  deque<int> i_p(0);
  deque<int> ii_p(0);
  x_old=mzz[X/30][Y/30];
  FindPath(X/30,Y/30,x_old);
  if (path.size()>1)
   while (!path.empty())
   {
    tmp_cell=path.top();
    path.pop();
    mzz[tmp_cell.cell_x][tmp_cell.cell_y]=-10;
    Label3->Tag--;
    Label3->Caption=Label3->Tag;
    if (count(i_p.begin(),i_p.end(),tmp_cell.cell_x)==0)
     i_p.push_front(tmp_cell.cell_x);
   }
  else
  {
   tmp_cell=path.top();
   path.pop();
   mzz[tmp_cell.cell_x][tmp_cell.cell_y]=x_old;
  }
  while (!i_p.empty())
  {
   BASS_ChannelStop(strs[1]);
   if (sn && N6->Checked)
    BASS_ChannelPlay(strs[1],false);
   for (int j=16;j>=0;j--)
    if (mzz[i_p.front()][j]!=-10)
    {
     int jj=j+1;
     while (mzz[i_p.front()][jj]==-10)
     {
      swap(mzz[i_p.front()][jj-1],mzz[i_p.front()][jj]);
//      Sleep(1);
//      DrawInHole_C(i_p.front(),jj-1,mzz[i_p.front()][jj-1]);
//      DrawInHole_C(i_p.front(),jj,mzz[i_p.front()][jj]);
      if (jj+1>17)
       break;
      else
       jj++;
     }
    }
   for (int j=0;j<=18;j++)
   {
    if (j==18)
    {
     ii_p.push_front(i_p.front());
     break;
    }
    if (mzz[i_p.front()][j]!=-10)
     break;
   }
   i_p.pop_front();
  }
  sort(ii_p.begin(),ii_p.end());
  while (!ii_p.empty())
  {
   for (int i=ii_p.back();i<17;i++)
    for (int j=0;j<18;j++)
     swap(mzz[i][j],mzz[i+1][j]);
   ii_p.pop_back();
  }
  y_old=0;
  for (int i=0;i<18;i++)
   for (int j=17;j>=0;j--)
    if (mzz[i][j]!=-10)
    {
     x_old=mzz[i][j];
     FindPath(i,j,x_old);
     if (path.size()>1)
      y_old=1;
     while (!path.empty())
     {
      tmp_cell=path.top();
      path.pop();
      mzz[tmp_cell.cell_x][tmp_cell.cell_y]=x_old;
     }
     if (y_old==1)
      return;
    }
  if (y_old==0)
  {
   if (Label3->Tag<StrToInt(ValueListEditor1->Cells[1][10]))
    FreshTbl();
   else
    ShowMessage(" В таблицу ты не попал :-(");
   GScr->Enabled=false;
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
 if (Form1->Tag==1)
 {
  Timer2->Interval=200;
  if (Timer2->Tag==0)
  {
   zx=0;
   zy=0;
   Timer2->Tag=1;
  }
  else
  {
   zx=4;
   zy=4;
   Timer2->Tag=0;
  }
 }
 if (Form1->Tag==2)
 {
  Timer2->Interval=1000;
  Label3->Caption=Label3->Tag++;
 }
 if (Form1->Tag==4)
 {
  deque<int> x_k(0);
  deque<int> y_k(0);
  int yy_y;
  if (y_old+106>=448 || mzz[x_old/32][(y_old+106)/32]!=-1)
  {
   Timer2->Enabled=false;
   if (sn && N6->Checked)
    BASS_ChannelPlay(strs[3],false);
   if (y_old>=0)
    {
    yy_y=(y_old+15)/32;
    mzz[x_old/32][yy_y+2]=cols[2];
    mzz[x_old/32][yy_y+1]=cols[1];
    mzz[x_old/32][yy_y]=cols[0];
    x_k.push_front(x_old/32);
    x_k.push_front(x_old/32);
    x_k.push_front(x_old/32);
    y_k.push_front(yy_y);
    y_k.push_front(yy_y+1);
    y_k.push_front(yy_y+2);
   }
   if (y_old>=-42 && y_old<0)
   {
    mzz[x_old/32][1]=cols[2];
    mzz[x_old/32][0]=cols[1];
    x_k.push_front(x_old/32);
    x_k.push_front(x_old/32);
    y_k.push_front(0);
    y_k.push_front(1);
   }
   if (y_old>=-74 && y_old<-42)
   {
    mzz[x_old/32][0]=cols[2];
    x_k.push_front(x_old/32);
    y_k.push_front(0);
   }
   CheckingHole_Cl(x_k,y_k);
   cols[0]=col[0];
   cols[1]=col[1];
   cols[2]=col[2];
   FindNewBalls_Cl();
   x_old=128;
   y_old=-106;
   stp=Timer2->Tag;
   Timer2->Enabled=true;
  }
  if (y_old+stp+106>=448 || mzz[x_old/32][(y_old+stp+106)/32]!=-1)
   stp=1;
  if (y_old==-106 && mzz[x_old/32][(y_old+stp+106)/32]!=-1)
  {
   Timer2->Enabled=false;
   if (score>StrToInt(ValueListEditor1->Cells[1][10]))
    FreshTbl();
   else
    ShowMessage(" В таблицу ты не попал :-(");
  }
  y_old+=stp;
 }
 if (Form1->Tag==5)
 {
  if (y_old+104>=520 || CheckOnStopDown())
  {
   Timer2->Enabled=false;
   if (y_old<0)
   {
    if (score>StrToInt(ValueListEditor1->Cells[1][10]))
     FreshTbl();
    else
     ShowMessage("Ты проиграл :(");
    return;
   }
   int x_x=x_old/26,y_y=(y_old+10)/26;
   for (int i=y_y,nkx=0;i<y_y+4;i++,nkx++)
    for (int j=x_x,nky=0;j<x_x+4;j++,nky++)
    {
     if (rf[nkx][nky])
      mzzz[i][j]=rf[nkx][nky];
    }
   deque<int> vs(0);
   for (int i=0,k=0;i<20;i++,k=0)
   {
    for (int j=0;j<10;j++)
     if (mzzz[i][j])
      k++;
    if (k==10)
     vs.push_back(i);
   }
   if (vs.size()!=0)
   {
    for (unsigned int i=0;i<vs.size();i++)
     for (int j=0;j<10;j++)
      mzzz[vs[i]][j]=false;
    score=score+vs.size()*10;
    Label6->Caption="Результат: "+IntToStr(score);
    for (unsigned int i=0;i<vs.size();i++)
     for (int j=0;j<10;j++)
      for (int n=vs[i];n>0;n--)
       mzzz[n][j]=mzzz[n-1][j];
    BASS_ChannelStop(strs[3]);
    BASS_ChannelStop(strs[1]);
    if (sn && N6->Checked)
     BASS_ChannelPlay(strs[1],false);
   }
   else
   {
    BASS_ChannelStop(strs[3]);
    BASS_ChannelStop(strs[1]);
    if (sn && N6->Checked)
     BASS_ChannelPlay(strs[3],false);
   }
   x_old=78;
   y_old=-104;
   stp=Timer2->Tag;
   FromNewToReal();
   CreateNewFigure();
   FromTempTo(true);
   DrawInN(NextBall3);
   FromRealToTemp();
   Timer2->Enabled=true;
  }
  y_old+=stp;
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label8Click(TObject *Sender)
{
 ShellExecute(Application->Handle, NULL, "mailto:wms@nm.ru", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label5Click(TObject *Sender)
{
 ShellExecute(Application->Handle, NULL, "www.wms.nm.ru", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N10Click(TObject *Sender)
{
 if (((TMenuItem *)Sender)->Name=="N41")
  knn=4;
 if (((TMenuItem *)Sender)->Name=="N51")
  knn=5;
 if (((TMenuItem *)Sender)->Name=="N61")
  knn=6;
 Form1->Tag=3;
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
// Pics->Picture->LoadFromFile(ssm+"Pics\\SmallStars.bmp");
 LoadFJPG(ssm+"Pics\\SmallStars.jpg");
 Fon->Visible=false;
 GScr->Visible=true;
 Timer2->Enabled=false;
 NextBall->Visible=false;
 NextBall2->Visible=false;
 NextBall3->Visible=false;
 Label1->Visible=false;
 Label3->Visible=true;
 Label4->Visible=true;
 Label2->Visible=true;
 Timer2->Enabled=false;
 GScr->Enabled=true;
 Label6->Visible=false;
 Label3->Tag=324;
 Label3->Caption="324";
 Label4->Caption="шт.";
 TIniFile *ini;
 ini = new TIniFile(ssm+"table.ini");
 ValueListEditor1->Strings->Clear();
 for (int i=1;i<11;i++)
 {
  if (knn==4)
   ValueListEditor1->Strings->Add(ini->ReadString( "Click", "Rank"+IntToStr(i), "Stranger=900" ));
  if (knn==5)
   ValueListEditor1->Strings->Add(ini->ReadString( "Click2", "Rank"+IntToStr(i), "Stranger=900" ));
  if (knn==6)
   ValueListEditor1->Strings->Add(ini->ReadString( "Click3", "Rank"+IntToStr(i), "Stranger=900" ));
 }
 delete ini;
 ValueListEditor1->Visible=true;
 for (int i=0;i<18;i++)
  for (int j=0;j<18;j++)
   mzz[i][j]=random(knn);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 int s;
 s=(y_old+106)/32;
 if (Key==VK_RIGHT)
 {
  if (Form1->Tag==4)
   if (x_old+32<=256 && mzz[(x_old+33)/32][s-2]<0 && mzz[(x_old+33)/32][s-1]<0 && mzz[(x_old+33)/32][s]<0)
    x_old+=32;
  if (Form1->Tag==5)
   if (!CheckOnStopRight())
    x_old+=26;
 }
 if (Key==VK_SPACE)
  stp=10;
 if (Key==VK_LEFT)
 {
  if (Form1->Tag==4)
   if (x_old-32>=0 && mzz[(x_old-30)/32][s-2]<0 && mzz[(x_old-30)/32][s-1]<0 && mzz[(x_old-30)/32][s]<0)
    x_old-=32;
  if (Form1->Tag==5)
   if (!CheckOnStopLeft())
    x_old-=26;
 }
 if (Key==VK_DOWN)
 {
  if (Form1->Tag==4)
  {
   s=cols[2];
   cols[2]=cols[1];
   cols[1]=cols[0];
   cols[0]=s;
  }
  if (Form1->Tag==5)
   RotFig(true);
 }
 if (Key==VK_UP)
 {
  if (Form1->Tag==4)
  {
   s=cols[0];
   cols[0]=cols[1];
   cols[1]=cols[2];
   cols[2]=s;
  }
  if (Form1->Tag==5)
   RotFig(false);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N6Click(TObject *Sender)
{
 N6->Checked=!N6->Checked;
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
 TIniFile *ini;
 ini = new TIniFile(ssm+"allstars.ini");
 ini->WriteInteger("Options","PlaySound",N6->Checked);
 delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::x2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Lines1Click(TObject *Sender)
{
 Form1->Tag=1;
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
// Pics->Picture->LoadFromFile(ssm+"Pics\\BigStar.bmp");
 LoadFJPG(ssm+"Pics\\BigStar.jpg");
 Label1->Visible=true;
 NextBall2->Visible=false;
 NextBall3->Visible=false;
 NextBall->Visible=true;
 Fon->Visible=true;
 GScr->Visible=true;
 Timer2->Enabled=false;
 Label3->Visible=false;
 Label4->Visible=false;
 Label2->Visible=true;
 Timer2->Enabled=false;
 Label6->Caption="Результат:";
 Label6->Visible=true;
 TIniFile *ini;
 ini = new TIniFile(ssm+"table.ini");
 ValueListEditor1->Strings->Clear();
 for (int i=1;i<11;i++)
  ValueListEditor1->Strings->Add(ini->ReadString( "Lines", "Rank"+IntToStr(i), "Stranger=10" ));
 delete ini;
 ValueListEditor1->Visible=true;
 for (int i=0;i<9;i++)
  for (int j=0;j<9;j++)
  {
   mz[i][j]=-1;
   DrawInHole(i,j,-1);
  }
 col[0]=random(6);
 col[1]=random(6);
 col[2]=random(6);
 CreateNewBalls();
 FindNewBalls();
 score=0;
 GScr->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Color1Click(TObject *Sender)
{
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
// Pics->Picture->LoadFromFile(ssm+"Pics\\BStars.bmp");
 LoadFJPG(ssm+"Pics\\BStars.jpg");
 GScr->Canvas->CopyMode=cmBlackness;
 GScr->Canvas->CopyRect(Rect(0,0,540,540),Pics->Canvas,Rect(0,0,1,1));
 GScr->Canvas->CopyMode=cmSrcCopy;
 Form1->Tag=4;
 Fon->Visible=false;
 Timer2->Enabled=false;
 Label3->Visible=false;
 Label4->Visible=false;
 Label2->Visible=true;
 Label6->Caption="Результат:";
 Label6->Visible=true;
 TIniFile *ini;
 ini = new TIniFile(ssm+"table.ini");
 ValueListEditor1->Strings->Clear();
 for (int i=1;i<11;i++)
  ValueListEditor1->Strings->Add(ini->ReadString( "Color", "Rank"+IntToStr(i), "Stranger=2" ));
 delete ini;
 ValueListEditor1->Visible=true;
 Label1->Visible=true;
 NextBall->Visible=false;
 NextBall2->Visible=true;
 NextBall3->Visible=false;
 for (int i=0;i<9;i++)
  for (int j=0;j<14;j++)
   mzz[i][j]=-1;
 cols[0]=random(5);
 cols[1]=random(5);
 cols[2]=random(5);
 FindNewBalls_Cl();
 score=0;
 x_old=128;
 y_old=-106;
 Timer2->Tag=1;
 GScr->Tag=150;
 stp=Timer2->Tag;
 Timer2->Interval=10;
 Timer2->Enabled=true;
 GScr->Enabled=true;
 GScr->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3Click(TObject *Sender)
{
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
// Pics->Picture->LoadFromFile(ssm+"Pics\\ForTetr.bmp");
 LoadFJPG(ssm+"Pics\\ForTetr.jpg");
 GScr->Canvas->CopyMode=cmBlackness;
 GScr->Canvas->CopyRect(Rect(0,0,540,540),Pics->Canvas,Rect(0,0,1,1));
 GScr->Canvas->CopyMode=cmSrcCopy;
 Form1->Tag=5;
 Fon->Visible=false;
 Timer2->Enabled=false;
 Label3->Visible=false;
 Label4->Visible=false;
 Label2->Visible=true;
 Label6->Caption="Результат:";
 Label6->Visible=true;
 TIniFile *ini;
 ini = new TIniFile(ssm+"table.ini");
 ValueListEditor1->Strings->Clear();
 for (int i=1;i<11;i++)
  ValueListEditor1->Strings->Add(ini->ReadString( "Tetris", "Rank"+IntToStr(i), "Stranger=2" ));
 delete ini;
 ValueListEditor1->Visible=true;
 Label1->Visible=true;
 NextBall->Visible=false;
 NextBall2->Visible=false;
 NextBall3->Visible=true;

 CreateNewFigure();
 FromTempTo(true);
 DrawInN(NextBall3);
 FromNewToReal();
 FromRealToTemp();
 GScr->Visible=true;
 for (int i=0;i<20;i++)
  for (int j=0;j<10;j++)
  {
   mzzz[i][j]=false;
//   Tmp->Canvas->CopyRect(Rect(i*26,j*26,i*26+26,j*26+26),Pics->Canvas,Rect(4,4,5,5));
  }
 score=0;
 Label6->Caption="Результат: "+IntToStr(score);
 x_old=78;
 y_old=-104;
 GScr->Enabled=true;
 Timer2->Tag=1;
 Timer2->Interval=10;
 Timer2->Enabled=true;
 stp=Timer2->Tag;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N16Click(TObject *Sender)
{
 int nk,ek;
 if (((TMenuItem *)Sender)->Name=="N16")
 {
  knn=4;
  all_num=36;
  nk=0;
  ek=18;
 }
 if (((TMenuItem *)Sender)->Name=="N17")
 {
  knn=8;
  all_num=72;
  nk=12;
  ek=48;
 }
 Form1->Tag=2;
 x_old=y_old=-1;
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
// Pics->Picture->LoadFromFile(ssm+"Pics\\Cars.bmp");
 LoadFJPG(ssm+"Pics\\Cars.jpg");
 Fon->Visible=true;
 GScr->Visible=true;
 Timer2->Enabled=false;
 NextBall2->Visible=false;
 NextBall->Visible=false;
 NextBall3->Visible=false;
 Label1->Visible=false;
 Label3->Visible=true;
 Label4->Visible=true;
 Label2->Visible=true;
 Timer2->Enabled=true;
 Label6->Visible=false;
 Label3->Tag=0;
 Label3->Caption="0";
 Label4->Caption="сек.";
 TIniFile *ini;
 ini = new TIniFile(ssm+"table.ini");
 ValueListEditor1->Strings->Clear();
 for (int i=1;i<11;i++)
 {
  if (knn==4)
   ValueListEditor1->Strings->Add(ini->ReadString( "Memory", "Rank"+IntToStr(i), "Stranger=9999" ));
  else
   ValueListEditor1->Strings->Add(ini->ReadString( "Memory2", "Rank"+IntToStr(i), "Stranger=9999" ));
 }
 delete ini;
 ValueListEditor1->Visible=true;
 for (int i=0;i<9;i++)
  for (int j=0;j<9;j++)
   mz[i][j]=mzz[i][j]=-10;
 for (int i=0;i<2;i++)
 {
  randomize();
  for (int j=nk,ixv,jyv;j<ek;j++)
  {
zx_1:
   ixv=random(9);
   jyv=random(knn);
   if (mz[ixv][jyv]==-10)
   {
    mz[ixv][jyv]=j;
    mzz[ixv][jyv]=-1;
   }
   else
    goto zx_1;
  }
 }
 GScr->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::River1Click(TObject *Sender)
{
 Form1->Tag=6;
 x_old=y_old=-1;
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
 Pics->Picture->LoadFromFile(ssm+"Pics\\CarsR.bmp");
 Fon->Visible=false;
 GScr->Visible=true;
 Timer2->Enabled=false;
 NextBall2->Visible=false;
 NextBall->Visible=false;
 NextBall3->Visible=false;
 Label1->Visible=false;
 Label3->Visible=true;
 Label4->Visible=true;
 Label2->Visible=true;
 Timer2->Enabled=true;
 Label6->Visible=false;
 Label3->Tag=0;
 Label3->Caption="0";
 Label4->Caption="сек.";
 TIniFile *ini;
 ini = new TIniFile(ssm+"table.ini");
 ValueListEditor1->Strings->Clear();
 for (int i=1;i<11;i++)
  ValueListEditor1->Strings->Add(ini->ReadString( "River", "Rank"+IntToStr(i), "Stranger=9999" ));
 delete ini;
 ValueListEditor1->Visible=true;
 for (int i=0;i<14;i++)
  for (int j=0;j<9;j++)
   mzz[i][j]=-1;
 for (int i=0;i<4;i++)
 {
  randomize();
  for (int j=0,ixv,jyv;j<21;j++)
  {
zx_1:
   ixv=random(12)+1;
   jyv=random(7)+1;
   if (mzz[ixv][jyv]==-1)
    mzz[ixv][jyv]=j;
   else
    goto zx_1;
  }
 }
 GScr->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
 if (BASS_GetVersion()!=DWORD(MAKELONG(2,1)))
  sn=false;
 if (!BASS_Init(1, 44100, 0, Handle, NULL))
  sn=false;
 AnsiString astr=ExtractFilePath(Application->ExeName);
 if (sn)
 {
  strs[0]=BASS_StreamCreateFile(false,(astr+"\\sounds\\shoot.mp3").c_str(),0,0,0);
  strs[1]=BASS_StreamCreateFile(false,(astr+"\\sounds\\slash.mp3").c_str(),0,0,0);
  strs[2]=BASS_StreamCreateFile(false,(astr+"\\sounds\\nopath.mp3").c_str(),0,0,0);
  strs[3]=BASS_StreamCreateFile(false,(astr+"\\sounds\\break.mp3").c_str(),0,0,0);
  mods[0]=BASS_MusicLoad(false,(astr+"\\music\\knulakuk.mod").c_str(),0,0,BASS_MUSIC_RAMP|BASS_MUSIC_POSRESET|BASS_SAMPLE_LOOP,0);
  mods[1]=BASS_MusicLoad(false,(astr+"\\music\\bignjap.mod").c_str(),0,0,BASS_MUSIC_RAMP|BASS_MUSIC_POSRESET|BASS_SAMPLE_LOOP,0);
  mods[2]=BASS_MusicLoad(false,(astr+"\\music\\axk8.mod").c_str(),0,0,BASS_MUSIC_RAMP|BASS_MUSIC_POSRESET|BASS_SAMPLE_LOOP,0);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
 for (int a=0;a<4;a++)
  BASS_StreamFree(strs[a]);
 for (int a=0;a<3;a++)
  BASS_MusicFree(mods[a]);
 BASS_Free();
 Action=caFree;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AXK81Click(TObject *Sender)
{
 BASS_ChannelStop(mods[nom_mod]);
 nom_mod=2;
 if (sn && N2->Checked)
  BASS_ChannelPlay(mods[2],true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Knullakukk1Click(TObject *Sender)
{
 BASS_ChannelStop(mods[nom_mod]);
 nom_mod=0;
 if (sn && N2->Checked)
  BASS_ChannelPlay(mods[0],true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BiginJapan1Click(TObject *Sender)
{
 BASS_ChannelStop(mods[nom_mod]);
 nom_mod=1;
 if (sn && N2->Checked)
  BASS_ChannelPlay(mods[1],true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2Click(TObject *Sender)
{
 N2->Checked=!N2->Checked;
 if (!N2->Checked)
  BASS_ChannelStop(mods[nom_mod]);
 AnsiString ssm=ExtractFilePath(Application->ExeName)+"\\";
 TIniFile *ini;
 ini = new TIniFile(ssm+"allstars.ini");
 ini->WriteInteger("Options","PlayMusic",N2->Checked);
 delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormActivate(TObject *Sender)
{
 if (sn && N2->Checked)
  BASS_ChannelPlay(mods[0],true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label10Click(TObject *Sender)
{
 ShellExecute(Application->Handle, NULL, "www.un4seen.com", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

