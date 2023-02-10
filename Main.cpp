#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <io.h>

namespace Generate
{
	using namespace std; 
	
	const int MOD=1e9+7;
	const int inf=1e9+7;
	const double eps=1e-6;
	const int MAXW=10050;
	const int MAXH=5050;
	
	int W,H,thi,BGR,BGG,BGB;
	int piced[MAXH][MAXW];
	int r[MAXH][MAXW],addr[MAXH][MAXW];
	int g[MAXH][MAXW],addg[MAXH][MAXW];
	int b[MAXH][MAXW],addb[MAXH][MAXW];
	
	int StNum;
	struct Station
	{
		int Dis,Pos,id;
		bool operator < (const Station &M) const { return Dis<M.Dis; }
	}St[500];
	bool StReorder(Station x, Station y) { return x.id<=y.id; }
	
	int TrNum;
	struct Train
	{
		int R,G,B;
		vector<pair<int,int> > Plan;
	}Tr[500];
	
	int TimeX=inf,TimeY=-inf;
	inline int GetX(int ti) { return W/20+(W-2*(W/20))*(ti-TimeX)/(TimeY-TimeX); }
	inline int GetY(int id) { return St[id].Pos; }
	int H1[500][2000],H2[500][2000];
	
	void ReadData()
	{
		cin>>W>>H>>thi;
		cin>>BGR>>BGG>>BGB;
		for(int i=1; i<=W; i++)
			for(int j=1; j<=H; j++)
			r[j][i]=g[j][i]=b[j][i]=255;
		cin>>StNum;
		for(int i=1; i<=StNum; i++)
		{
			cin>>St[i].Dis;
			St[i].id=i;
		}	
		sort(St+1, St+1+StNum);
		for(int i=1; i<=StNum; i++)
			St[i].Pos= H/10+(H-2*(H/10))*(St[i].Dis-St[1].Dis)/(St[StNum].Dis-St[1].Dis);
		sort(St+1, St+1+StNum, StReorder);
		
		cin>>TrNum;
		for(int i=1; i<=TrNum; i++)
		{
			cin>>Tr[i].R>>Tr[i].G>>Tr[i].B;
			int CurN; cin>>CurN;
			while(CurN--)
			{
				int StId,StTime;
				cin>>StId>>StTime;
				Tr[i].Plan.push_back(make_pair(StId,StTime));
				TimeX=min(TimeX,StTime);
				TimeY=max(TimeY,StTime);
			}
		}
	}
	
	void DrawGrid()
	{
		for(int i=1; i<=StNum; i++)
			for(int j=GetX(TimeX); j<=GetX(TimeY); j++)
				r[GetY(i)][j]=BGR,g[GetY(i)][j]=BGG,b[GetY(i)][j]=BGB;
		
		int Up=GetY(1), Down=GetY(StNum);
		int L=TimeX/60+1, R=TimeY/60;
		for(int h=L; h<=R; h++)
		{
			int curTi=h*60;
			for(int j=Up; j<=Down; j++)
				r[j][GetX(curTi)]=BGR,g[j][GetX(curTi)]=BGG,b[j][GetX(curTi)]=BGB;
		}
	}
	
	inline void DrawLine(int x1,int y1,int x2,int y2,int R,int G,int B)
	{
		if(x1==x2)
		{
			if(y1>y2) swap(y1,y2);
			for(int j=y1; j<=y2; j++) r[x1][j]=R,g[x1][j]=G,b[x1][j]=B;
			return ;
		}
		if(x1>x2) swap(x1,x2),swap(y1,y2);
		if(y1==y2)
		{
			for(int i=x1; i<=x2; i++) r[i][y1]=R,g[i][y1]=G,b[i][y1]=B;
			return ;
		}
		memset(piced,0,sizeof(piced));
		double k=(y2-y1+0.0)/(x2-x1+0.0);
		double b1=y1-0.0-k*(x1-0.0);
		for(int i=x1; i<=x2; i++)
		{
			int Down=eps+(i-1)*k+b1;
			int Up=eps+i*k+b1;
			if(Down>Up) swap(Down,Up);
			for(int j=max(Down, min(y1,y2)); j<=Up; j++)
				r[i][j]=R,g[i][j]=G,b[i][j]=B,piced[i][j]=1;
		}
	}
	void DrawTr(vector<pair<int,int> > t,int R,int G,int B)
	{
		int n=t.size();
		for(int i=1; i<n; i++)
		{
			if(t[i].first==t[i-1].first)
			{
				bool C1=GetY(t[i-2].first)>GetY(t[i-1].first),
					 C2=GetY(t[i].first)>GetY(t[i+1].first);
				if(C1==C2)
					DrawLine(GetY(t[i-1].first),GetX(t[i-1].second),GetY(t[i].first),GetX(t[i].second),R,G,B);
				else if(C1)
				{
					int MAX=-inf;
					for(int cur=t[i-1].second; cur<=t[i].second; cur++)
						H1[t[i].first][cur]++,MAX=max(MAX,H1[t[i].first][cur]);
					int OffSet=-1*(MAX*H/50+H/200);
					DrawLine(GetY(t[i-1].first)+OffSet,GetX(t[i-1].second),GetY(t[i].first)+OffSet,GetX(t[i].second),R,G,B);
					DrawLine(GetY(t[i-1].first),GetX(t[i-1].second),GetY(t[i-1].first)+OffSet,GetX(t[i-1].second),R,G,B);
					DrawLine(GetY(t[i].first),GetX(t[i].second),GetY(t[i].first)+OffSet,GetX(t[i].second),R,G,B);
				}
				else if(C2)
				{
					int MAX=-inf;
					for(int cur=t[i-1].second; cur<=t[i].second; cur++)
						H2[t[i].first][cur]++,MAX=max(MAX,H2[t[i].first][cur]);
					int OffSet=MAX*H/50+H/200;
					DrawLine(GetY(t[i-1].first)+OffSet,GetX(t[i-1].second),GetY(t[i].first)+OffSet,GetX(t[i].second),R,G,B);
					DrawLine(GetY(t[i-1].first),GetX(t[i-1].second),GetY(t[i-1].first)+OffSet,GetX(t[i-1].second),R,G,B);
					DrawLine(GetY(t[i].first),GetX(t[i].second),GetY(t[i].first)+OffSet,GetX(t[i].second),R,G,B);
				}
			}
			else
			{
				DrawLine(GetY(t[i-1].first),GetX(t[i-1].second),GetY(t[i].first),GetX(t[i].second),R,G,B);
			}
		}
	}
	
	void WriteBMP(char* img, const char* filename)
	{
	    int L= (W*3+3)/4*4;
	    int bmi[]= { L*H+54,0,54,40,W,H,1|3*8<<16,0,L*H,0,0,100,0 };
	    FILE *fp= fopen(filename,"wb");
	    fprintf(fp,"BM");
	    fwrite(&bmi,52,1,fp);
	    fwrite(img,1,L*H,fp);
	    fclose(fp);
	}
	char img[MAXW*MAXH*3];
	void Convert()
	{
		for(int i=0; i<MAXH; i++)
			for(int j=0; j<MAXW; j++)
				addr[i][j]=addg[i][j]=addb[i][j]=255;
		
		for(int i=1; i<=H; i++)
			for(int j=1; j<=W; j++)
			{
				for(int dx=-thi; dx<=thi; dx++)
					for(int dy=abs(dx)-thi; dy<=thi-abs(dx); dy++)
					addr[i+dx][j+dy]=min(addr[i+dx][j+dy],r[i][j]),
					addg[i+dx][j+dy]=min(addg[i+dx][j+dy],g[i][j]),
					addb[i+dx][j+dy]=min(addb[i+dx][j+dy],b[i][j]);
			}
		for(int i=1; i<=H; i++)
			for(int j=1; j<=W; j++)
			r[i][j]=addr[i][j],g[i][j]=addg[i][j],b[i][j]=addb[i][j];
		
		memset(img,0,sizeof(img));
		for(int i=1; i<=W; i++)
			for(int j=1; j<=H; j++)
			{
				int Base=((H-j)*W+i)*3;
				img[Base]=b[j][i],img[Base+1]=g[j][i],img[Base+2]=r[j][i];
			}
		WriteBMP(img,"Result.bmp");
	}
	
	void Main()
	{
		freopen("DataGen.txt","r",stdin);
	//	freopen(".out","w",stdout);
		ReadData();
		DrawGrid();
		for(int i=1; i<=TrNum; i++) DrawTr(Tr[i].Plan,Tr[i].R,Tr[i].G,Tr[i].B);
		Convert();
		freopen("CON","r",stdin);
	}
}

namespace InfoLog
{
	#define STDOUT 1
	
	const int MOD=1e9+7;
	const int inf=1e9+7;
	
	using namespace std;
	
	void LoadLink();
	void SaveLink();
	
	void ReLogStation();
	void AddStation();
	void DelStation();
	void ShowStation();
	void LogStation();
	
	void OutTime(int t);
	int GetTime();
	void ShowPrecise(int TrID);
	
	void AddTrain();
	void DelTrain();
	void CheckPrecise();
	void ShowTrain();
	void LogTrain();
	
	void AddLink();
	void ShowLink();
	void DelLink();
	void ResetLink();
	void LinkTrain();
	
	void GenTrain();
	void SaveAs();
	void Welcome();
	
	bool isFileExists_fopen(string& name)
	{
	    if(FILE *file=fopen(name.c_str(), "r"))
		{
	        fclose(file);
	        return true;
	    }
		else
	        return false;  
	}
	
	int Choose(int ItemNum,string Item[])
	{
		for(int i=0; i<ItemNum; i++)
			cout<<"【"<<(char)('A'+i)<<"】"<<Item[i]<<" ";
		cout<<endl;
		while(1)
		{
			char tmp=getch();
			if(tmp>='A' && tmp<='A'+ItemNum-1) return tmp-'A'+1;
			if(tmp>='a' && tmp<='a'+ItemNum-1) return tmp-'a'+1;
		}
	}
	int ChooseYN()
	{
		cout<<"【Y】是 【N】否"<<endl;
		while(1)
		{
			char tmp=getch();
			if(tmp=='Y' || tmp=='y') return 1;
			if(tmp=='N' || tmp=='n') return 0;
		}
	}
	
	int TrNum;
	struct Train
	{
		string ID;
		vector<pair<string,int> > Plan;
		bool used;
		int Del;
	}Tr[500];
	set<string> AllTrain;
	int StNum;
	struct Station
	{
		string Name;
		string Code;
		int Dis;
		bool operator < (const Station &M) const { return Dis<M.Dis; }
	}St[500];
	map<string,int> Codemp;
	void Load()
	{
		for(int i=0; i<500; i++)
			{ Tr[i].ID=""; Tr[i].Plan.clear(); Tr[i].used=0; Tr[i].Del=0; }
		AllTrain.clear(); Codemp.clear();
		
		string FileName="DataLog.txt";
		if(!isFileExists_fopen(FileName))
			return ;
		freopen("DataLog.txt","r",stdin);
		
		cin>>StNum;
		for(int i=1; i<=StNum;i++)
			cin>>St[i].Name>>St[i].Dis>>St[i].Code;
		sort(St+1,St+1+StNum);
		for(int i=1; i<=StNum; i++)
			Codemp[St[i].Code]=i;
			
		cin>>TrNum;
		for(int i=1; i<=TrNum; i++)
		{
			cin>>Tr[i].ID>>Tr[i].used;
			int curStNum; cin>>curStNum;
			for(int j=1; j<=curStNum; j++)
			{
				string curCode; int curTi; cin>>curCode>>curTi;
				Tr[i].Plan.push_back(make_pair(curCode,curTi));
			}
			AllTrain.insert(Tr[i].ID);
		}
		
		freopen("CON","r",stdin);
	}
	void Save()
	{
		int oldstdout=dup(STDOUT);
		freopen("DataLog.txt","w",stdout);
		
		cout<<StNum<<endl;
		for(int i=1; i<=StNum; i++)
			cout<<St[i].Name<<" "<<St[i].Dis<<" "<<St[i].Code<<endl;
		
		cout<<AllTrain.size()<<endl;
		for(int i=1; i<=TrNum; i++)
		{
			if(Tr[i].Del) continue;
			cout<<Tr[i].ID<<" "<<Tr[i].used<<endl;
			cout<<Tr[i].Plan.size()<<endl;
			for(auto u:Tr[i].Plan)
				cout<<u.first<<" "<<u.second<<endl;
		}
			
		freopen("CON","w",stdout);
		dup2(oldstdout,STDOUT);
	}
	int LiNum;
	struct Link
	{
		int R,G,B;
		vector<string> Path;
	}Li[500];
	void LoadLink()
	{
		for(int i=0; i<500; i++)
			Li[i].Path.clear();
		
		string FileName="DataLink.txt";
		if(!isFileExists_fopen(FileName))
			return ;
		
		freopen("DataLink.txt","r",stdin);
		cin>>LiNum;
		for(int i=1;i<=LiNum;i++)
		{
			cin>>Li[i].R>>Li[i].G>>Li[i].B;
			string tmp;
			while(cin>>tmp)
			{
				if(tmp=="END") break;
				Li[i].Path.push_back(tmp);
			}
		}
		freopen("CON","r",stdin);
	}
	void SaveLink()
	{
		int oldstdout=dup(STDOUT);
		freopen("DataLink.txt","w",stdout);
		cout<<LiNum<<endl;
		for(int i=1; i<=LiNum; i++)
		{
			cout<<Li[i].R<<" "<<Li[i].G<<" "<<Li[i].B<<endl;
			for(auto u:Li[i].Path) cout<<u<<" ";
			cout<<"END"<<endl;
		}
		freopen("CON","w",stdout);
		dup2(oldstdout,STDOUT);
	}
	
	inline string Formalize(string s)
	{
		for(int i=0; i<(int)s.size(); i++)
			if(s[i]>='a' && s[i]<='z') s[i]-='a'-'A';
		return s;
	}
	
	void ReLogStation()
	{
		system("cls");
		for(int i=1; i<500; i++)
			St[i].Dis=0,St[i].Name="";
		Codemp.clear();
		cout<<"请输入站点数量：";
		cin>>StNum;
		cout<<"请依次输入各站点名称、里程、拼音码"<<endl;
		
		map<string,int> allCode; allCode.clear();
		for(int i=1; i<=StNum; i++)
		{
			cin>>St[i].Name>>St[i].Dis>>St[i].Code;
			St[i].Name=Formalize(St[i].Name);
			St[i].Code=Formalize(St[i].Code);
			allCode[St[i].Code]++;
		}
		sort(St+1,St+1+StNum);
		bool ok=1;
		for(auto it:allCode)
		{
			if((it.second)>=2)
			{
				cout<<"【"<<(it.first)<<"】";
				ok=0;
			}
		}
		if(!ok)
		{
			cout<<endl<<"重复的拼音码！按任意键重新录入"<<endl;
			system("pause");
			ReLogStation();
			return ;
		}
		
		for(int i=1; i<=StNum; i++)
			Codemp[St[i].Code]=i;
		
		cout<<"录入完成！按任意键返回上一级"<<endl;
		system("pause");
		LogStation();
	}
	void AddStation()
	{
		system("cls");
		cout<<"请输入站点名称、里程、拼音码："<<endl;
		string Name,Code; int Dis;
		cin>>Name>>Dis>>Code; Code=Formalize(Code);
		if(Codemp[Code])
		{
			cout<<"拼音码【"<<Code<<"】已被【"<<Codemp[Code]<<"】站使用！"<<endl<<endl;
			string AddStationFail[2]={"重新添加","返回上级"};
			int id=Choose(2,AddStationFail);
			if(id==1) AddStation();
			else if(id==2) LogStation();
			return ;
		}
		St[++StNum].Code=Code;
		St[StNum].Dis=Dis;
		St[StNum].Name=Name;
		sort(St+1,St+1+StNum);
		for(int i=1; i<=StNum; i++)
			Codemp[St[i].Code]=i;
		
		cout<<endl<<"添加完成！"<<endl;
		string AddStationSuc[2]={"继续添加","返回上级"};
		int id=Choose(2,AddStationSuc);
		if(id==1) AddStation();
		else if(id==2) LogStation(); 
	}
	void DelStation()
	{
		system("cls");
		
		if(!StNum)
		{
			cout<<"没有站点！按任意键返回上一级"<<endl;
			system("pause");
			LogStation();
			return ;
		}
		for(int i=1; i<=StNum; i++)
			cout<<"【"<<St[i].Name<<"】"<<St[i].Dis<<" "<<St[i].Code<<endl;
		
		cout<<endl<<"请输入待删除车站的拼音码：";
		string Code; cin>>Code; Code=Formalize(Code);
		bool Fail=0;
		set<int> FailID; FailID.clear();
		if(!Codemp[Code])
		{
			cout<<"不存在的车站：【"<<Code<<"】！"<<endl<<endl;
			Fail=1;
		}
		else
		{
			for(int i=1; i<=TrNum; i++)
			{
				for(auto it:Tr[i].Plan)
				if(it.first==Code) FailID.insert(i);
			}
			if(FailID.size())
			{
				cout<<"【"<<St[Codemp[Code]].Name<<"】站有：";
				for(auto TrID:FailID)
					cout<<"【"<<Tr[TrID].ID<<"】";
				cout<<"次列车停靠，无法删除该站点！"<<endl<<endl;
				Fail=1; 
			}
		}
		if(Fail)
		{
			string DelStationFail[2]={"重新删除","返回上级"};
			int id=Choose(2,DelStationFail);
			if(id==1) DelStation();
			else if(id==2) LogStation();
			return ; 
		}
		int StID=Codemp[Code];
		Codemp[Code]=0;
		St[StID].Code="",St[StID].Name="",St[StID].Dis=inf;
		sort(St+1,St+1+StNum); StNum--;
		for(int i=1; i<=StNum; i++)
			Codemp[St[i].Code]=i;
		 
		cout<<"删除成功！"<<endl;
		string DelStationSuc[2]={"继续删除","返回上级"};
		int id=Choose(2,DelStationSuc);
		if(id==1) DelStation();
		else if(id==2) LogStation(); 
	}
	void ShowStation()
	{
		cout<<endl;
		if(!StNum)
		{
			cout<<"没有站点！按任意键返回上一级"<<endl;
			system("pause");
			LogStation();
			return ;
		}
		for(int i=1; i<=StNum; i++)
			cout<<"【"<<St[i].Name<<"】"<<St[i].Dis<<" "<<St[i].Code<<endl;
		cout<<"已显示所有站点信息！按任意键返回上一级"<<endl;
		system("pause");
		LogStation();
	}
	void LogStation()
	{
		system("cls");
		string LogStationChoose[5]={"重新录入站点","增加站点","删除站点","查看所有站点信息","保存并返回上一级"};
		int id=Choose(5,LogStationChoose);
		if(id==1) ReLogStation();
		else if(id==2) AddStation();
		else if(id==3) DelStation();
		else if(id==4) ShowStation();
		else if(id==5) { Save(); Welcome(); }
	}
	
	void OutTime(int t)
	{
		int AddDay=t/1440;
		if(AddDay) cout<<"(+"<<AddDay<<")";
		t%=1440;
		if(t/60<10) cout<<"0"; cout<<t/60;
		cout<<":";
		if(t%60<10) cout<<"0"; cout<<t%60;
	}
	int GetTime()
	{
		int ti=0;
		string s; cin>>s; int sn=s.size();
		if(sn>4)
			ti=1440*atoi(s.substr(0,sn-5).c_str());
		ti+=(s[sn-1]-'0')+(s[sn-2]-'0')*10+(s[sn-3]-'0')*60+(s[sn-4]-'0')*600;
		return ti;
	}
	void ShowPrecise(int TrID)
	{
		cout<<"车次号【"<<Tr[TrID].ID<<"】"<<endl;
		for(int i=0; i<(int)Tr[TrID].Plan.size(); i+=2)
		{
			cout<<St[Codemp[Tr[TrID].Plan[i].first]].Name<<" ";
			if(i==0) { cout<<"--:--"; i--; }
			else OutTime(Tr[TrID].Plan[i].second);
			cout<<" ";
			if(i==(int)Tr[TrID].Plan.size()-1) cout<<"--:--";
			else OutTime(Tr[TrID].Plan[i+1].second);
			cout<<endl;
		}
	}
	
	void AddTrain()
	{
		system("Cls");
		cout<<"请输入车次编号、站点数、各站拼音码及到发时间，首末站各填写发/到时间即可。输入HELP查看示例"<<endl;
		string curID; cin>>curID; curID=Formalize(curID);
		if(curID=="HELP")
		{
			cout<<endl<<"例如：插入10:00在上海虹桥发车、20:00到达南京南、21:05在南京南发车、次日2:00到达北京南的T1次列车，应该输入以下内容："<<endl;
			cout<<"T1 3"<<endl<<"SHQ 1000"<<endl<<"NJN 2000 2105"<<endl<<"BJN 1-0200"<<endl<<endl;
			cout<<"所有输入的字母不区分大小写"<<endl<<endl;
			
			cout<<"请输入车次编号、站点数及各站到发时间，首末站各填写发/到时间即可。"<<endl;
			cin>>curID; curID=Formalize(curID);
		}
		if(AllTrain.find(curID)!=AllTrain.end())
		{
			cout<<"车次已存在!"<<endl<<endl;
			string AddTrainChoose[2]={"重新添加车次","返回上一级"};
			int id=Choose(2,AddTrainChoose);
			if(id==1)
				AddTrain();
			else if(id==2)
				LogTrain();
			return ;
		}
		AllTrain.insert(curID);
		Tr[++TrNum].ID=curID;
		Tr[TrNum].Del=Tr[TrNum].used=0; Tr[TrNum].Plan.clear();
		int curStNum; cin>>curStNum;
		for(int i=1; i<=curStNum; i++)
		{
			string curSt; int curTi;
			cin>>curSt; curSt=Formalize(curSt); curTi=GetTime();
			int StID=0;
			for(int j=1; j<=StNum; j++)
				if(St[j].Code==curSt) StID=j;
			if(!StID)
			{
				Tr[TrNum].ID=""; Tr[TrNum].Plan.clear(); Tr[TrNum].Del=Tr[TrNum].used=0; TrNum--;
				cout<<"不存在的拼音码【"<<curSt<<"】！"<<endl<<endl;
				string StNotFound[2]={"重新添加车次","返回上级"};
				int id=Choose(2,StNotFound);
				if(id==1) AddTrain();
				else if(id==2) LogTrain();
				return ;
			}
			Tr[TrNum].Plan.push_back(make_pair(St[StID].Code,curTi));
			if(i>1 && i<curStNum)
				{ curTi=GetTime(); Tr[TrNum].Plan.push_back(make_pair(St[StID].Code,curTi)); }
		}
		
		cout<<"【"<<Tr[TrNum].ID<<"】次信息登记成功！"<<endl<<endl;
		string AddTrainSuc[2]={"继续添加车次","返回上一级"};
		int id=Choose(2,AddTrainSuc);
		if(id==1)
			AddTrain();
		else if(id==2)
			LogTrain();
	}
	void DelTrain()
	{
		system("cls");
		if(AllTrain.empty())
		{
			cout<<"没有车次！按任意键返回上级"<<endl;
			system("pause");
			LogTrain();
			return ;
		}
		cout<<"已录入的车次："; 
		for(auto i:AllTrain)
			cout<<"【"<<i<<"】 ";
		cout<<endl<<endl;
		cout<<"请输入要删除的车次：";
		string curID; cin>>curID; curID=Formalize(curID);
		if(AllTrain.find(curID)!=AllTrain.end())
		{
			int TrID=0;
			for(int i=1; i<=TrNum; i++)
				if(!Tr[i].Del && Tr[i].ID==curID) TrID=i;	
			
			ShowPrecise(TrID);
			cout<<endl<<"确认要删除该车次吗？"<<endl;
			int id=ChooseYN();
			if(id==0)
			{
				string DelTrainFail[2]={"重新删除车次","返回上一级"};
				int id1=Choose(2,DelTrainFail);
				if(id1==1)
					DelTrain();
				else if(id1==2)
					LogTrain();
			}
			else if(id==1)
			{
				AllTrain.erase(AllTrain.find(curID));
				Tr[TrID].Del=1;
				cout<<endl<<"删除成功！"<<endl;
				string DelTrainSuc[2]={"继续删除车次","返回上一级"};
				int id2=Choose(2,DelTrainSuc);
				if(id2==1)
					DelTrain();
				else if(id2==2)
					LogTrain();
			}
		}
		else
		{
			cout<<"未找到该车次！"<<endl<<endl;
			string DelTrainFail[2]={"重新删除车次","返回上一级"};
			int id=Choose(2,DelTrainFail);
			if(id==1)
				DelTrain();
			else if(id==2)
				LogTrain();
		}
	}
	void CheckPrecise()
	{
		cout<<endl<<"请输入要查看的车次：";
		string curID; cin>>curID; curID=Formalize(curID);
		if(AllTrain.find(curID)==AllTrain.end())
		{
			cout<<"不存在的车次，是否返回上级？"<<endl<<endl;
			int id=ChooseYN();
			if(id==0)
				CheckPrecise();
			else if(id==1)
				ShowTrain();
		}
		else
		{
			int TrID=0;
			for(int i=1; i<=TrNum; i++)
				if(!Tr[i].Del && Tr[i].ID==curID) TrID=i;
			ShowPrecise(TrID);
			string CheckPreciseChoose[2]={"继续查询车次","返回上级"};
			int id=Choose(2,CheckPreciseChoose);
			if(id==1)
				CheckPrecise();
			else if(id==2)
				ShowTrain();
		}
	}
	void ShowTrain()
	{
		system("cls");
		if(AllTrain.empty())
		{
			cout<<"没有车次！按任意键返回上级"<<endl;
			system("pause");
			LogTrain();
			return ;
		}
		for(auto i:AllTrain)
			cout<<"【"<<i<<"】 ";
		cout<<endl;
		string ShowTrainChoose[2]={"查看具体车次","返回上级"};
		int id=Choose(2,ShowTrainChoose);
		if(id==1)
			CheckPrecise();
		else if(id==2)
			LogTrain();
	}
	void LogTrain()
	{
		system("cls");
		string LogTrainChoose[4]={"添加车次","删除车次","查看车次","保存并返回上级"};
		int id=Choose(4,LogTrainChoose);
		
		if(id==1) AddTrain();
		else if(id==2) DelTrain();
		else if(id==3) ShowTrain();
		else if(id==4) { Save(); Welcome(); }
	}
	
	void AddLink()
	{
		cout<<endl;
		vector<string> newLink;
		cout<<"请依次输入交路中的车次，以ADD衔接相邻两日，以END结束："<<endl;
		vector<string> LinkID;
		string tmp;
		while(cin>>tmp)
		{
			tmp=Formalize(tmp); 
			if(tmp=="END") break;
			LinkID.push_back(tmp);
		}
		int ADDcur=0, ADDcnt[500]={0};
		for(auto curID:LinkID)
		{
			if(curID=="ADD")
			{
				ADDcur++;
				continue;
			}
			int TrID=0;
			for(int i=1; i<=TrNum; i++)
				if(Tr[i].ID==curID) TrID=i;
			if(!TrID || Tr[TrID].used)
			{
				if(!TrID) cout<<"不存在的车次：【"<<curID<<"】！"<<endl<<endl;
				else cout<<"已有交路的车次：【"<<curID<<"】！"<<endl<<endl;
				string AddLinkFail[2]={"重新添加","返回上级"};
				int id=Choose(2,AddLinkFail);
				if(id==1) AddLink();
				else if(id==2) LinkTrain();
				return ;
			}
			ADDcnt[TrID]=ADDcur;
			newLink.push_back(curID);
		}
		for(int i=1; i<(int)newLink.size(); i++)
		{
			string curID=newLink[i],preID=newLink[i-1];
			int curTrID,preTrID;
			for(int j=1; j<=TrNum; j++)
				if(Tr[j].ID==curID) curTrID=j;
				else if(Tr[j].ID==preID) preTrID=j;
				
			if(Tr[preTrID].Plan[(int)Tr[preTrID].Plan.size()-1].first!=Tr[curTrID].Plan[0].first)
			{
				cout<<"添加失败！【"<<preID<<"】与【"<<curID<<"】首末站不同无法衔接"<<endl<<endl;
				string AddLinkFail[2]={"重新添加","返回上级"};
				int id=Choose(2,AddLinkFail);
				if(id==1) AddLink();
				else if(id==2) LinkTrain();
				return ;
			}
			if(Tr[preTrID].Plan[(int)Tr[preTrID].Plan.size()-1].second+ADDcnt[preTrID]*1440 > Tr[curTrID].Plan[0].second+ADDcnt[curTrID]*1440)
			{
				cout<<"添加失败！【"<<preID<<"】与【"<<curID<<"】首末站时间倒置无法衔接"<<endl<<endl;
				string AddLinkFail[2]={"重新添加","返回上级"};
				int id=Choose(2,AddLinkFail);
				if(id==1) AddLink();
				else if(id==2) LinkTrain();
				return ;
			}
		}
		for(int i=0; i<(int)newLink.size(); i++)
		{
			string curID=newLink[i];
			int TrID=0;
			for(int j=1; j<=TrNum; j++)
				if(Tr[j].ID==curID) TrID=j;
			Tr[TrID].used=1;
		}
		LiNum++;
		for(auto curID:LinkID)
			Li[LiNum].Path.push_back(curID);
		cout<<"是否为该交路指定颜色？（默认黑色）"<<endl<<endl;
		int id1=ChooseYN();
		if(id1==1)
		{
			cout<<"请分别输入该交路图像的RGB值：（0~255）";
			cin>>Li[LiNum].R>>Li[LiNum].G>>Li[LiNum].B;
		}
		cout<<"添加成功！"<<endl<<endl;
		string AddLinkFail[2]={"继续串联","返回上级"};
		int id=Choose(2,AddLinkFail);
		if(id==1) AddLink();
		else if(id==2) LinkTrain();
		return ;
	}
	void DisplayLink()
	{
		cout<<endl;
		for(int i=1; i<=LiNum; i++)
		{
			cout<<"交路"<<i<<"：";
			for(auto u:Li[i].Path)
				cout<<"【"<<u<<"】";
			cout<<endl;
		}
		cout<<"无交路车次：";
		for(int i=1; i<=TrNum; i++)
			if(!Tr[i].used) cout<<"【"<<Tr[i].ID<<"】";
		cout<<endl<<endl;
	}
	void ShowLink()
	{
		DisplayLink();
		cout<<"按任意键返回上一级"<<endl;
		system("pause");
		LinkTrain();
	}
	void DelLink()
	{
		system("cls");
		DisplayLink();
		cout<<"请在一行内输入所有需要删除的交路编号："<<endl;
		string InputLink; cin>>InputLink;
		stringstream ss; ss<<InputLink;
		int curLID; set<int> toDel;
		while(ss>>curLID)
		{
			if(curLID>LiNum || curLID<=0)
			{
				cout<<"不存在：交路"<<curLID<<"！"<<endl<<endl;
				string DelLinkFail[2]={"重新删除","返回上级"};
				int id=Choose(2,DelLinkFail);
				if(id==1) DelLink();
				else if(id==2) LinkTrain();
				return ;
			}
			toDel.insert(curLID);
		}
		for(auto LID:toDel)
		{
			for(int i=0; i<(int)Li[LID].Path.size(); i++)
			{
				int TrID=0;
				for(int j=1; j<=TrNum; j++)
					if(Tr[j].ID==Li[LID].Path[i]) TrID=j;
				Tr[TrID].used=0;
			}
		}
		int nLiNum=0;
		for(int i=1; i<=LiNum; i++)
		{
			if(toDel.find(i)!=toDel.end()) continue;
			Li[++nLiNum]=Li[i];
		}
		LiNum=nLiNum;
		SaveLink();
		LoadLink();
		cout<<"删除成功！"<<endl<<endl;
		string DelLinkSuc[2]={"继续删除","返回上级"};
		int id2=Choose(2,DelLinkSuc);
		if(id2==1) DelLink();
		else if(id2==2) LinkTrain();
	}
	void ResetLink()
	{
		cout<<endl;
		cout<<"确认取消所有交路？"<<endl<<endl;
		int id=ChooseYN();
		if(id==1)
		{
			LiNum=0;
			for(int i=0; i<500; i++)
				Li[i].Path.clear();
			for(int i=0; i<500; i++)
				Tr[i].used=0,Li[i].R=Li[i].G=Li[i].B=0;
			system("cls");
			cout<<"取消成功！"<<endl; 
		}
		cout<<"按任意键返回上一级"<<endl;
		system("pause");
		LinkTrain();
	}
	void LinkTrain()
	{
		system("cls");
		string LinkTrainChoose[5]={"增加交路","查看当前交路","删除交路","清空所有交路","保存并返回上一级"};
		int id=Choose(5,LinkTrainChoose);
		if(id==1) AddLink();
		else if(id==2) ShowLink();
		else if(id==3) DelLink();
		else if(id==4) ResetLink();
		else if(id==5) { SaveLink(); Save(); Welcome(); }
	}
	
	void GenTrain()
	{
		int W,H,thi,BGR,BGG,BGB;
		
		system("cls");
		cout<<"请依次输入交路图的高度、宽度、线条粗细：";
		cin>>H>>W>>thi;
		cout<<"请依次输入背景网格的颜色RGB值：（0~255）";
		cin>>BGR>>BGG>>BGB; 
		
		int oldstdout=dup(STDOUT);
		freopen("DataGen.txt","w",stdout);
		cout<<W<<" "<<H<<" "<<thi<<endl;
		cout<<BGR<<" "<<BGG<<" "<<BGB<<endl<<endl;
		
		cout<<StNum<<endl;
		for(int i=1; i<=StNum; i++)
			cout<<St[i].Dis<<" ";
		cout<<endl<<endl;
		
		cout<<LiNum<<endl<<endl;
		for(int i=1; i<=LiNum; i++)
		{
			cout<<Li[i].R<<" "<<Li[i].G<<" "<<Li[i].B<<endl;
			
			vector<pair<int,int> > List;
			int ADDcur=0;
			for(auto curID:Li[i].Path)
			{
				if(curID=="ADD")
				{
					ADDcur++;
					continue;
				}
				int TrID=0;
				for(int j=1; j<=TrNum; j++)
					if(Tr[j].ID==curID) TrID=j;
				for(int j=0; j<(int)Tr[TrID].Plan.size(); j++)
					List.push_back(make_pair(Codemp[Tr[TrID].Plan[j].first],Tr[TrID].Plan[j].second+ADDcur*1440));
			}
			
			cout<<(int)List.size()<<endl;
			for(auto tmp:List)
				cout<<tmp.first<<" "<<tmp.second<<endl;
			cout<<endl;
		}
		freopen("CON","w",stdout);
		dup2(oldstdout,STDOUT);
		
		long GenSt=clock();
		cout<<endl<<"交路图生成中..."<<endl;
		Generate::Main();
		long GenEd=clock();
		
		cout<<"交路图生成成功！耗时"<<GenEd-GenSt<<"ms"<<endl<<endl;
		cout<<"是否关闭系统？"<<endl;
		int id=ChooseYN();
		if(id==0) Welcome();
		else exit(0);
	}
	void SaveAs()
	{
		system("cls");
		cout<<"请输入文件夹名：";
		string FolderName; cin>>FolderName;
		system(("md "+FolderName).c_str());
		system(("copy DataLog.txt "+FolderName).c_str());
		system(("copy DataLink.txt "+FolderName).c_str());
		cout<<endl<<"另存完成！按任意键返回上一级"<<endl;
		system("pause");
		Welcome();
	}
	void Welcome()
	{
		system("title 交路图生成系统 by I_Am_Danny");
		system("cls");
		
		cout<<"加载中..."<<endl;
		long Ti_LoadBefore=clock();
		Load(); LoadLink();
		long Ti_LoadAfter=clock();
		cout<<"加载完毕，耗时"<<Ti_LoadAfter-Ti_LoadBefore<<"ms"<<endl<<endl;
		
		cout<<"欢迎使用交路图生成系统！"<<endl<<endl;
		string WelcomeChoose[6]={"站点操作","车次操作","交路操作","导出交路图","另存当前数据","退出系统"};
		int id=Choose(6,WelcomeChoose);
		
		if(id==1) LogStation();
		else if(id==2) LogTrain();
		else if(id==3) LinkTrain();
		else if(id==4) GenTrain();
		else if(id==5) SaveAs();
		else if(id==6) exit(0);
	}
}

signed main()
{
	InfoLog::Welcome();
	return 0;
}
