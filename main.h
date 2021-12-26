//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include <Menus.hpp>
#include "deque.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TBevel *Bevel1;
        TImage *Fon;
        TImage *Tmp;
        TPaintBox *GScr;
        TImage *Pics;
        TImage *NextBall;
        TLabel *Label1;
        TLabel *Label2;
        TTimer *Timer2;
        TImage *Tmp2;
        TImage *Tmp3;
        TMainMenu *MainMenu1;
        TImage *Image1;
        TLabel *Label5;
        TBevel *Bevel4;
        TLabel *Label8;
        TValueListEditor *ValueListEditor1;
        TLabel *Label3;
        TLabel *Label4;
        TImage *NextBall2;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N15;
        TMenuItem *Lines1;
        TMenuItem *Color1;
        TMenuItem *Memory1;
        TMenuItem *Click1;
        TMenuItem *Swaper1;
        TMenuItem *N16;
        TMenuItem *N17;
        TMenuItem *x2;
        TMenuItem *N41;
        TMenuItem *N51;
        TMenuItem *N61;
        TLabel *Label6;
        TMenuItem *N3;
        TImage *NextBall3;
        TMenuItem *River1;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *BiginJapan1;
        TMenuItem *Knullakukk1;
        TMenuItem *AXK81;
        TLabel *Label7;
        TLabel *Label9;
        TLabel *Label10;
        TMenuItem *N4;
        TMenuItem *N7;
        TMenuItem *N8;
        void __fastcall GScrMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall Label8Click(TObject *Sender);
        void __fastcall Label5Click(TObject *Sender);
        void __fastcall N10Click(TObject *Sender);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall N6Click(TObject *Sender);
        void __fastcall x2Click(TObject *Sender);
        void __fastcall Lines1Click(TObject *Sender);
        void __fastcall Color1Click(TObject *Sender);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall N16Click(TObject *Sender);
        void __fastcall River1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall AXK81Click(TObject *Sender);
        void __fastcall Knullakukk1Click(TObject *Sender);
        void __fastcall BiginJapan1Click(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Label10Click(TObject *Sender);
private:	// User declarations
        void __fastcall LineD(int x,int y,int x1,int y1,TImage *Fon);
        void __fastcall CreateNewBalls(void);
        void __fastcall DrawInHole(int x,int y,int cl);
        void __fastcall DrawInHole_R(int x,int y,int cl);
        void __fastcall DrawInHole_M(int x,int y,int cl);
        void __fastcall DrawInHole_C(int x,int y,int cl);
        void __fastcall FindNewBalls(void);
        int  __fastcall CheckForLines(int x,int y,int xp,int yp);
        bool __fastcall CheckingHole(int x,int y);
        void __fastcall FreshTbl(void);
        void __fastcall SlowDestr(int x1,int y1);
        void __fastcall SlowVis(int x1,int y1);
        void __fastcall FindPath(int xx,int yy,int colr);
        void __fastcall FindNewBalls_Cl(void);
        bool __fastcall CheckingHole_Cl(deque<int> x_k,deque<int> y_k);
        void __fastcall CreateNewFigure(void);
        void __fastcall FromNewToReal(void);
        void __fastcall FromRealToTemp(void);
        void __fastcall FromTempTo(bool wher);
        void __fastcall DrawInN(TObject *Send);
        void __fastcall RotFig(bool wher);
        bool __fastcall CheckOnStopDown(void);
        bool __fastcall CheckOnStopLeft(void);
        bool __fastcall CheckOnStopRight(void);
        void __fastcall LoadFJPG(AnsiString astr);
public:		// User declarations
        int mz[9][9];
        int mzz[18][18];
        int col[3],cols[3],x_old,y_old,zx,zy,wf,hf;
        bool mzzz[20][10],rf[4][4],nf[4][4],tf[7][7];
        enum {cBlue,cGray,cGreen,cPurple,cRed,cYellow};
        void __fastcall WhenIdle(TObject *Sender, bool &Done);
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
