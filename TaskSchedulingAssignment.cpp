/* *********************************************************************
*	Author  : Imteaj, Md. Nabid
*	ID      : 11-19998-3
*	Topic   : Implemention of Task Scheduling Algorithms
*	Course  : Operating System [ E ]
*	Faculty : HOSSAIN, BAYZID ASHIK
*	Date    : Monday, April 1, 2013.
*	Lang    : C/C++
*   Compiler: CodeBlocks 12.11
*   Ref(s). : [1] Computer System Concepts - Silberchatz, Galvin, Gagne
*				  Seventh Edition.
*			  [2] http://programming-technique.blogspot.com/
*				2011/09/how-to-resize-console-window-using-c.html
*			  [3] http://www.cplusplus.com/reference/
*			  [4] www.google.com
*   >> Class Project, CSE_AIUB.
*************************************************************************/
#pragma comment(linker, "/STACK:16777216")
#pragma warning(disable:4786)

#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>

using namespace std;
/*
const int SIZ = 100005;
const int INF = (1<<30);
const double PI = 2*acos(0.0);
const double EPS = 1e-11;
*/

#define sq(x) ((x)*(x))
#define mset(x,n) memset((x),(n),sizeof((x)))

/// debug
/// Courtesy: Smilitude
#define D(x) if(1) cout << __LINE__ << " " << #x" = " << (x) << endl
#define D2(x,y) if(1) cout << __LINE__ << " " << #x" = " << (x) \
		<< ", " << #y" = " << (y) << endl
#define pfl(x) cout << (x) << endl
#define pf(x) cout << (x)
#define pfln() cout << endl
#define pfs() cout << " "
#define READ(f) freopen( f, "r", stdin )
#define WRITE(f) freopen( f, "w", stdout )
#define Sys() system( "pause" )

#define REP(i,n) for( int i=0,_e(n); i<_e; i++ )
#define FOR(i,a,b) for( int i=(a),_e(b); i<=_e; i++ )

//template<class T> T BitLen( T a ){ return (1+(floor(log(a)))); }// Note: return bit length of a number [alternate ceil(1+log(a));]
//template<class T>inline T MAX( T a, T b ){ return a>b ? a:b; } // Note: returns maximum of a and b
template<class T>inline T MIN( T a, T b ){ return a<b ? a:b; } // Note: returns minimum of a and b
//template<class T>inline T GCD( T a, T b ){ if(a<0)return GCD(-a,b); if(b<0)return GCD(a,-b);while(b){ b^=a^=b^=a%=b; }return a; } // Note: returns GCD of a and b
//template<class T>inline T LCM( T a, T b ){ if(a<0)return LCM(-a,b); if(b<0)return LCM(a,-b);return a/GCD(a,b)*b; } // Note: returns LCM of a and b
//template<class T>inline void SWAP( T &a, T &b ) { a = a^b; b = a^b; a = a^b; } // Note: swaps a to b, *Not for Double
//inline void SWAP(double &a, double &b) { double c = a; a = b; b = c;} // Note: swaps a to b *Double
//template<class T, class T1>T Reverse( T1 A[], T i, T j ) { for(i,j-=1; i<j; i++, j--) SWAP(A[i], A[j]); return 0;} // Note: char array reverse form i to j
//template<class T, class T1>T Reverse( T1 &A, T i, T j ) { for(i,j-=1; i<j; i++, j--) SWAP(A[i], A[j]); return 0;} // Note: string reverse
//template< class T >inline T fAbs( T a ) { return a<0 ? (a*(-1)) : a; }

int resizeConsole( ) {
	/// ref. [2]
	HANDLE wHnd;    // Handle to write to the console.
	HANDLE rHnd;    // Handle to read from the console.
	// Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    // Change the window title:
    SetConsoleTitle("Implemention of Task Scheduling Algorithms");
    // Set up the required window size:
    SMALL_RECT windowSize = {0, 0, 105, 105};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);
    // Change the console window size:
        // Create a COORD to hold the buffer size:
    COORD bufferSize = {10, 10};
    SetConsoleScreenBufferSize(wHnd, bufferSize);
    // Exit
    return 0;
}

struct Task{
	int processId;
	int brustTime;
	int arrivalTime;
	int priority;
	int arrivalq;
	Task(){ processId = brustTime = arrivalq = arrivalTime = priority = 0; };
	bool operator()( Task a, Task b ) { // default
		return a.arrivalq < b.arrivalq;
	}
}cmp;
vector< Task > VT;

bool cmp1( Task a, Task b ) { // sjfs non preemtive
	if( a.brustTime == b.brustTime ) return a.processId < b.processId;
	return a.brustTime < b.brustTime;
}

bool cmp2( Task a, Task b ) { // sjfs preemtive
	return a.arrivalTime < b.arrivalTime; // distinct arrival time
}

bool cmp3( Task a, Task b ) { // priority
	if( a.priority == b.priority ) return a.processId < b.processId;
	return a.priority < b.priority;
}

void ganttChart( ) {
	char ch;
	int nOP = VT.size(), i, j = nOP, tmp, k = 0, totalBrustTime = 0;
	for( i=0; i<nOP; i++ ) totalBrustTime += VT[i].brustTime;
	char arr[500];
	mset( arr, 0 );
	char tt[10];
	for( j=0, k=0; j<nOP; j++ ) {
		arr[k++] = (char)179;
		int tmp = VT[j].brustTime / 2;
		for( i=0; i<tmp; i++ ) arr[k++] = ' ';
		arr[k++] = 'P';
		tmp = VT[j].processId;
		int res = 1, rem, p=0, t = tmp;
		while( res != 0 ) {
			res = t / 10;
			rem = t % 10;
			t = res;
			tt[p++] = rem + '0';
		}
		for( i=p-1; i>=0; i-- ) arr[k++] = tt[i];
		for( i=0; i<tmp; i++ ) arr[k++] = ' ';
	}
	arr[k++] = (char)179;
	cout << "G A N T T - C H A R T" << endl;
	for( i=0; i<k; i++ ) {  // upper part
		if( i == 0 ) cout << (char)218;
		else if( i == k-1 ) cout << (char)191;
		else {
			if( arr[i] == (char)179 ) cout << (char)194;
			else cout << (char)196;
		}
	}
	cout << endl;
	cout << arr << endl;   // mid part
	for( i=0; i<k; i++ ) { // lower part
		if( i == 0 ) cout << (char)192;
		else if( i == k-1 ) cout << (char)217;
		else {
			if( arr[i] == (char)179 ) cout << (char)193;
			else cout << (char)196;
		}
	}
	cout << endl;
	char ar[500];
	mset( ar, 0 );
	for( i=1; i<k; i++ ) ar[i] = ' ';
	tmp = 0;
	ar[0] = '0';
	int wait[500], top=0;
	wait[top++] = tmp;
	for( i=1, j=0; i<k; i++ ) {
		if( arr[i] == (char)179 ) {
			tmp += VT[j].brustTime;
			wait[top++] = tmp;
			int res = 1, rem, p=0, t = tmp; mset(tt,0);
			while( res != 0 ) {
				res = t /10;
				rem = t % 10;
				t = res;
				tt[p++] = rem +  '0';
			}
			j++;
			for( p--; p>=0; p-- ) ar[i++] = tt[p];
		}
	}
	cout << ar << endl;
	// average waiting time
	top--; // ignoring last one
	cout << "Average waiting time:" << endl;
	cout << "= ( " << 0;
	for( i=1; i<top; i++ ) {
		cout << "+"<< wait[i];
		wait[i] += wait[i-1];
	}
	cout << " ) / " << nOP << endl;
	cout << "= " << (double)(wait[top-1] / (double)nOP);
	cout << endl;
}

int FCFS( ) {
	cout << "First-Come First-Served(FCFS) Scheduling" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Number of Processes?: " << endl;
	int nOP; // number of processes
	cin >> nOP;
	int i; Task a; char presss;
	VT.clear();
	cout << "Process   Brust Time" << endl;
	cout << "-------   ----------" << endl;
	for( i=0; i<nOP; i++ ) {
			a.processId = i+1;
			cout << "  P" << i+1 << "          ";
			cin >> a.brustTime;
			cout << endl;
			VT.push_back(a);
	}
	cout << "Do you want to set arrival queue? Y/N: ";
	cin >> presss;
	if( presss=='Y' || presss=='y' ) {
		cout << "Process   Arrival Queue" << endl;
		cout << "-------   -------------" << endl;
		for( i=0; i<nOP; i++ ) {
			cout << "  P" << i+1 << "             ";
			cin >> VT[i].arrivalq;
			cout << endl;
		}
		sort( VT.begin(), VT.end(), cmp );
		ganttChart( );
	}
	else if( presss=='n' || presss=='N' ) {
		ganttChart( );
	}
	return 0;

}

int preemtive_SJFS( ) {
	vector< Task > Q; // ready queue
	vector< Task > V; // temporary vector
	vector< Task > RP; // ready to print
	int timer = 0;
	sort( VT.begin(), VT.end(), cmp2 );
	int i = 0, j = 0, k = 0, p = 0, nOP = VT.size();
	while( k < nOP ) {
		if( VT[k].arrivalTime == timer ) Q.push_back( VT[k] ), k++;
		if( Q.size() ) {
			sort( Q.begin(), Q.end(), cmp1 );
			Task tmp = Q[0];
			tmp.brustTime = 1;
			Q[0].brustTime -= 1; // unit 1 is done
			RP.push_back( tmp );
		}
		timer++;
	}
	while( Q.size() ) { // final arranging
		sort( Q.begin(), Q.end(), cmp1 );
		if( Q[0].brustTime <= 0 ) { Q.erase( Q.begin()+0 ); continue; } // erasing 0th element
		Task tmp = Q[0];
		tmp.brustTime = 1;
		Q[0].brustTime -= 1; // unit 1 is done
		RP.push_back( tmp );
	}
	i = 1;
	while( i < RP.size() ) {
		if( RP[i].processId == RP[i-1].processId ) {
			RP[i-1].brustTime += 1;
			RP.erase( RP.begin()+i ); // erasing ith element
			continue;
		}
		i++;
	}
	VT.clear();
	while( RP.size() ) {
			VT.push_back( RP.front() );
			RP.erase( RP.begin()+0 );
	}
	return 0;
}

int SJFS( ) {
	cout << "SJFS" << endl;
	cout << "Shortest-Job First(SJF) Scheduling" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Number of Processes?: " << endl;
	int nOP; // number of processes
	cin >> nOP;
	int i; Task a; char presss;
	VT.clear();
	cout << "Process   Brust Time" << endl;
	cout << "-------   ----------" << endl;
	for( i=0; i<nOP; i++ ) {
			a.processId = i+1;
			cout << "  P" << i+1 << "          ";
			cin >> a.brustTime;
			cout << endl;
			VT.push_back(a);
	}
	cout << "Do you want to set arrival time? Y/N: ";
	cin >> presss;
	if( presss=='Y' || presss=='y' ) {
		cout << "Process   Arrival Time" << endl;
		cout << "-------   -------------" << endl;
		for( i=0; i<nOP; i++ ) {
			cout << "  P" << i+1 << "            ";
			cin >> VT[i].arrivalTime;
			cout << endl;
		}
		//sort( VT.begin(), VT.end(), cmp1 );
		// preemtive
		cout << "Default: Preemtive" << endl;
		preemtive_SJFS( );
		ganttChart( );
	}
	else if( presss=='n' || presss=='N' ) {
		sort( VT.begin(), VT.end(), cmp1 );
		// default non preemtive
		cout << "Default: Non-preemtive" << endl;
		ganttChart( );
	}
	return 0;
}

int PriS( ) {
	cout << "Priority Scheduling" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Number of Processes?: ";
	int nOP; // number of processes
	cin >> nOP;
	int i; Task a; char presss;
	VT.clear();
	cout << "Process   Brust Time" << endl;
	cout << "-------   ----------" << endl;
	for( i=0; i<nOP; i++ ) {
			a.processId = i+1;
			cout << "  P" << i+1 << "          ";
			cin >> a.brustTime;
			cout << endl;
			VT.push_back(a);
	}
	cout << "Process   Priority" << endl;
	cout << "-------   --------" << endl;
	for( i=0; i<nOP; i++ ) {
			cout << "  P" << i+1 << "          ";
			cin >> VT[i].priority;
			cout << endl;
	}
	sort( VT.begin(), VT.end(), cmp3 );
	ganttChart( );
	return 0;
}

int non_preemtive_RRS( ) {
	int tm_qntm = (1<<31)-1, i=0, len = VT.size(); // unit milisecond
	for( i=0; i<len; i++ ) tm_qntm = MIN( tm_qntm, VT[i].brustTime );
	tm_qntm = rand() % tm_qntm + 1; // random quantum
	cout << "Random Time Quantum: " << tm_qntm << endl;
	vector< Task > RP;
	i=0;
	while( VT.size() ) {
		i %= VT.size();
		if( VT[i].brustTime <= 0 ) { VT.erase( VT.begin()+i ); continue; }
		Task a = VT[i];
		if( a.brustTime <= tm_qntm ) {
			RP.push_back( a );
			VT[i].brustTime -= tm_qntm;
		}
		else {
			a.brustTime = tm_qntm;
			VT[i].brustTime -= tm_qntm;
			RP.push_back( a );
		}
		i++;
	}
	VT.clear();
	while( RP.size() ) {
			VT.push_back( RP.front() );
			RP.erase( RP.begin()+0 );
	}
	return 0;
}

int RRS( ) {
	cout << "Round-Robin Scheduling" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Number of Processes?: ";
	int nOP; // number of processes
	cin >> nOP;
	int i; Task a; char presss;
	VT.clear();
	cout << "Process   Brust Time" << endl;
	cout << "-------   ----------" << endl;
	for( i=0; i<nOP; i++ ) {
			a.processId = i+1;
			cout << "  P" << i+1 << "          ";
			cin >> a.brustTime;
			cout << endl;
			VT.push_back(a);
	}
	non_preemtive_RRS( );
	ganttChart( );
	return 0;
}

void boxDrawing( ) {
	int i;
	char ch;
	/// box drawing ------------------------------------------------------------------------
	ch = (char)201; cout << ch; /// upper left corner
	for(i=0, ch=(char)205; i<=51; i++) cout << ch;
	ch = (char)187; cout << ch << endl; /// upper right corner

	ch = (char)186;
	cout << ch << "                       M E N U                      " << ch << endl;

	ch = (char)204; cout << ch;
	for(i=0, ch=(char)205; i<=6; i++) cout << ch;
	ch = (char)203; cout << ch;
	for(i=0, ch=(char)205; i<=43; i++) cout << ch;
	ch = (char)185; cout << ch << endl;

	ch = (char)186;
	cout << ch << "Buttons" << ch << '\t' << "         O b j e c t i v e           " << ch << endl;
	ch = (char)204; cout << ch;
	for(i=0, ch=(char)205; i<=6; i++) cout << ch;
	ch = (char)206; cout << ch;
	for(i=0, ch=(char)205; i<=43; i++) cout << ch;
	ch = (char)185; cout << ch << endl; ch = (char)186;
	cout << ch << "   1   " << ch << "   First-Come First-Served(FCFS) Scheduling " << ch << endl;
	ch = (char)204; cout << ch;
	for(i=0, ch=(char)205; i<=6; i++) cout << ch;
	ch = (char)206; cout << ch;
	for(i=0, ch=(char)205; i<=43; i++) cout << ch;
	ch = (char)185; cout << ch << endl;ch = (char)186;
	cout << ch << "   2   " << ch  << "   Shortest Job First(SJF) Scheduling       " << ch << endl;
	ch = (char)204; cout << ch;
	for(i=0, ch=(char)205; i<=6; i++) cout << ch;
	ch = (char)206; cout << ch;
	for(i=0, ch=(char)205; i<=43; i++) cout << ch;
	ch = (char)185; cout << ch << endl; ch = (char)186;
	cout << ch << "   3   " << ch << "   Priority Scheduling                      " << ch << endl;
	ch = (char)204; cout << ch;
	for(i=0, ch=(char)205; i<=6; i++) cout << ch;
	ch = (char)206; cout << ch;
	for(i=0, ch=(char)205; i<=43; i++) cout << ch;
	ch = (char)185; cout << ch << endl; ch = (char)186;
	cout << ch << "   4   " << ch << "   Round Robin(RR) Scheduling               " << ch << endl;
	ch = (char)204; cout << ch;
	for(i=0, ch=(char)205; i<=6; i++) cout << ch;
	ch = (char)206; cout << ch;
	for(i=0, ch=(char)205; i<=43; i++) cout << ch;
	ch = (char)185; cout << ch << endl; ch = (char)186;
	cout << ch << "   0   " << ch << "   Terminate                                " << ch << endl;

	ch = (char)200; cout << ch; /// lower left corner
	for(i=0, ch=(char)205; i<=6; i++) cout << ch;
	ch = (char)202; cout << ch;
	for(i=0, ch=(char)205; i<=43; i++) cout << ch;
	ch = (char)188; cout << ch << endl; /// lower right corner
   /// box complete -------------------------------------------------------------------------
}

bool mainMenu( ) {
	boxDrawing( );
	int button;
	char ch;
	cout << "Your Choice: ";
	cin >> button;
	if( !(button >= 0 && button  <= 4) ) {
			cerr << "Wrong Choice!" << endl;
			cout << "Continue? Y/N: ";
			cin >> ch;
			if( ch=='Y'||ch=='y' ) return true;
			else if( ch=='N'||ch=='n' ) return false;
	}
	if( button == 0 ) {
		cout << "Program Terminting." << endl;
		return false;
	}
	switch( button ) {
		case 1: FCFS(); break;
		case 2: SJFS(); break;
		case 3: PriS(); break;
		case 4: RRS();  break;
	}

	return true;
}

int main( ){
	//#define DeBug
	#ifdef DeBug
		READ( "in.txt" );
		WRITE( "out.txt" );
	#endif
	resizeConsole( ); // ref. [2]
	cout << "Implemention of Task Scheduling Algorithms" << endl;
	cout << "Time: " << __TIME__ << endl;
	cout << "Date: " << __DATE__ << endl;
	cout << "==========================================" << endl;
	while( true ) {
		if( !mainMenu( ) ) break;
	}
	return 0;
}
