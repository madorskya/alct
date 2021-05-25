/**
Verilog++ SP.
\author A. Madorsky
*/

// Copyright (c) 2002, Alexander Madorsky, University of Florida/Physics. All rights reserved.

#ifndef _VLIB_H_FILE_
#define _VLIB_H_FILE_

#include <iostream>
#include <fstream>
#ifdef VGEN
	#include <sstream>
#endif
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef RVALS
//#define RVALS 21 // maximum number of bits in the reg/wire, divided by 32. Define this in your program if you need variables longer than 32-bit.
#endif

#define MAXARG 200

//typedef unsigned rval;
//#define Sizeofrval sizeof(rval)



#define Sizeofrval (RVALS*sizeof(unsigned))
#define UMSB (1 << (sizeof(unsigned)*8-1))

class rval
{

public:	
	// some of the operators are not "fair" yet, they will process only word 0 from the value.
	rval();
	rval(unsigned i);
//	rval(int i);
	void operator=(rval arg);
	rval operator+(rval arg);
	rval operator-(rval arg);
	rval operator*(rval arg);
	rval operator/(rval arg);
	rval operator%(rval arg);

	rval operator^(rval arg);
	rval operator&(rval arg);
	rval operator|(rval arg);

	bool operator&&(rval arg);
	bool operator||(rval arg);

	bool operator< (rval arg);
	bool operator> (rval arg);
	bool operator<=(rval arg);
	bool operator>=(rval arg);
	bool operator==(rval arg);
	bool operator!=(rval arg);

	bool operator! ();
	rval operator~ ();

	void lsh();
	void rsh();

	rval operator<<(rval arg);
	rval operator>>(rval arg);

	unsigned r[RVALS];
};

using namespace std;

class module;

enum SignalMode 
{
	mnone = 0, 
	mreg, 
	mwire, 
	minput, 
	moutput, 
	minout, 
	mnum, 
	mtemp
};

class Signal
{
public:
	Signal ();
	Signal (rval);
	Signal (int);
	Signal (const char*);
	Signal (int bits, rval value);
#ifdef VGEN
//	Signal (Signal& arg);
#endif
	~Signal ();
	void create();
	void init (int, int, const char*);
	void init(const char* rname){init(0, 0, rname);};
	void init(Signal* shost, int h, int l, const char* rname);
	void makemask(int hpar, int lpar);
#ifdef VGEN
	string& getname(){return name;};
	string& getorname(){return orname;};
	string& getcatname();
	void setname (string& rname){name = rname;};
	void setorname (string& rname){orname = rname;};
	void setbrackets(const char* l, const char* r){lb = l; rb = r;};
#endif
	rval getr(){return r;};
	int getint(){return r.r[0];};
	int getl(){return l;};
	int geth(){return h;};
	rval getmask(){return mask;};
	void setr(rval rv){r = rv & mask;};
	void setrc(rval rv){rc = rv & mask;};
	void sethlmask(int high, int low, rval imask){h = high; l = low; mask = imask;};
//	bool getbool(){return (r != 0);};
	bool getbool(){return (getval() != 0);};
	int getposedge(){return pedge;};
	int getnegedge(){return nedge;};
	int getchange(){return change;};
	void setchange(int ch){change = ch;};
	void setposedge(int ch){pedge = ch;};
	void setnegedge(int ch){nedge = ch;};
	void setalwaysn(int n) {alwaysn = (n == -1) ? 0 : n;};
	int  getalwaysn(){return alwaysn;};
	void setprintable(int p){printable = p;};
	rval getval();

	Signal set(Signal);
	Signal asgn(Signal);
	Signal operator=  (Signal);

	Signal operator+  (Signal);
	Signal operator-  (Signal);
	Signal operator*  (Signal);
	Signal operator/  (Signal);
	Signal operator%  (Signal);
	Signal operator^  (Signal);
	Signal operator>> (Signal);
	Signal operator<< (Signal);
	Signal operator&  (Signal);
	Signal operator&& (Signal);
	Signal operator|  (Signal);
	Signal operator|| (Signal);

	Signal operator<  (Signal);
	Signal operator>  (Signal);
	Signal operator<= (Signal);
	Signal operator>= (Signal);
	Signal operator== (Signal);
	Signal operator!= (Signal);

	Signal operator!  ();
	Signal operator~  ();
	Signal* operator&  ();
	Signal operator++  (){return *this = *this + 1;};
	Signal operator--  (){return *this = *this - 1;};
	Signal operator++  (int){return *this = *this + 1;};		// postfix operators require some work (different names for operations and assignment printing?)
	Signal operator--  (int){return *this = *this - 1;};

	friend Signal ror (Signal);
	friend Signal rxor (Signal);
	friend Signal rand (Signal);


	Signal operator()(Signal, Signal);
	Signal operator()(Signal);

	Signal operator,(Signal);

	friend ostream &operator<<(ostream &stream, Signal s);

	void input (int, int, const char*);
	void input (const char* rname){input(0, 0, rname);};

	void output (int, int, const char*);
	void output (const char* rname){output(0, 0, rname);};
	void output (int high, int low, const char* rname, module* parent);
	void output (const char* rname, module* parent);

	void inout (int, int, const char*);
	void inout (const char* rname){inout(0, 0, rname);};

	void reg (int, int, const char*);
	void initreg (int, int, const char*);
	void reg (const char* rname){reg(0, 0, rname);};

	void wire (int, int, const char*);
	void wire (int, int, const char*, int);
	void wire (const char* rname){wire(0, 0, rname);};


protected:
	rval r; // value holder
	rval rc; // current value holder
	int h, l;  // high and low bit numbers
	int hostl; // low bit of host
	rval mask; // bit mask
#ifdef VGEN
	string name;
	string orname;
	string lb, rb; // to distinguish between expression and variable
	string obname;
	string catname;
#endif
	Signal* host; // this Signal is a portion of the host
	Signal *ca1;
	Signal *ca2; // arguments of the comma operator
	int pedge, nedge, change;
	Signal* source; // pointer to source object
	Signal* outhost;
	Signal* outreg;
	int alwaysn;
	int inited;
	int printable;
	int mode; // none, reg, wire, input, output, inout
};


class parameter : public Signal
{
public:
	parameter (const char* rname, Signal arg);
	operator int(){return r.r[0];};
protected:
	void init (int, int, const char*);
	void operator= (Signal arg);
};



class memory
{
public: 
	memory (){r = NULL;};
	~memory ();
	void reg (int, int, int, int, const char*);
	void reg (int nup, int ndown, const char* rname) {reg (0, 0, nup, ndown, rname);};
#ifdef VGEN
	Signal operator[] (Signal);
#else
	Signal& operator[] (Signal);
#endif

protected:
#ifdef VGEN
	string name;
#endif
	Signal* r;
	int up, down;

};


class module
{
public:
	module ();
	virtual ~module ();
	void create();
	void vendmodule();
	void vbeginmodule();

	virtual void operator()(){};
	
	void init (const char*, const char*);
	void init (const char*, const char*, module* fixt);
	void init (const char*, const char*, int);
	Signal posedge (Signal);
	Signal negedge (Signal);
	void pushswitch (Signal arg){switcharg[switchn] = arg; switchn++;};
	Signal getswitch (){return switcharg[switchn - 1];};
	void popswitch () {switchn--;}
	Signal* AddOutReg(Signal arg);
	Signal ifelse(Signal, Signal, Signal);
#ifdef VGEN
	void PrintHeader();
#endif
protected:
#ifdef VGEN
	string name;
	string instname;
	streambuf *outbuf;
	ofstream vfile;
#endif
	int OuterIndPos;
	Signal switcharg[10];
	int switchn;
	int oldenmarg;
	Signal* outreg[1000];
	int outregn;
	void (*runperiod)();
	module* tfixt;

};


class function : public module
{
public:
	function(){};
	void init (int, int, const char*);
	void vendfunction();
	void vbeginfunction();
	void makemask(int hpar, int lpar);
protected:
#ifdef VGEN
	string retname;
#endif
	int h, l;
	rval mask;
	int OldChange;
	Signal result;
};
  
#define MAXSTIM 10000

class globcontrol
{
public:
	globcontrol();
#ifdef VGEN
	void AddParameter(string ln) {pars[npar] = ln; npar++;};
	void AddDeclarator(string ln){decls[ndecl] = ln; ndecl++;};
	void AddComment(string ln){ln += "\n"; decls[ndecl] = ln; ndecl++;};
	void AddIO(string ln);
	void Print ();
	string& PrintIO(bool col);
	void Indent(){indpos++; PrepMargin();};
	void Outdent(){indpos--; PrepMargin();};
	string& getmargin(){int t = nomargin; nomargin = 0; return (t) ? zeromargin : margin;};
	int getpos(){return indpos;};
	void setpos(int pos){indpos = pos; PrepMargin();};
	void setprintassign(int y){pa = y;};
	int  printassign(){return pa;};
	void enablemargin (int i){nomargin = !i;};
	int  getenablemargin (){return !nomargin;};
	void setFileOpen(int fo){VFileOpen = fo;};
	int  getFileOpen(){return VFileOpen;};
#endif
	void alwaysstart() {alwayscnt = 0; alwaysn++; if (alwaysn == 0 || alwaysn == -1) alwaysn = 1;};	
	void alwayspush() {if (alwayscnt >= 0) alwayscnt++;};
	void alwayspop()  {if (alwayscnt >  0) alwayscnt--; if (alwayscnt == 0) alwayscnt = -1;};
	int  alwaysget() {return (alwayscnt > 0);};
	int  getalwaysn() {return (alwayscnt > 0) ? alwaysn : (-1);};

	void setchange(int i);
	int  getchange(){return change;};
	void setparent(module *rparent){parent = rparent;};
	module* getparent(){return parent;};
	void setfunction(int i){functiondecl = i;};
	int  getfunction(){int t = functiondecl; functiondecl = 0; return t;};
	int getce(){return ce;};
	int setce(int c){ce = c; return 1;};
	char* constant(int bits, char* val);
	char* constant(int bits, int val);

protected:
#ifdef VGEN
	string pars[MAXARG];
	int npar;

	string decls[MAXARG];
	int ndecl;

	string dios[MAXARG];
	int ndio;

	int indpos;
	string margin, zeromargin;
	void PrepMargin();
	int nomargin;

	int pa;
	string outln;
#endif
	int functiondecl;

	int alwayscnt;
	int alwaysn;

	int change;
	module* parent;

	int VFileOpen; // shows if the comments can be written into the file, or they need to be added to declarators
	int ce; // clock can be processed
	char constring[RVALS*32+32]; // string for text variables

};

#ifdef VGEN
	#define beginsystem
	#define endsystem  exit(0);
#else
	#define beginsystem glc.setce(0); do { glc.setchange(0);
	#define endsystem   } while (glc.getchange() ? 1 : glc.getce() ? 0 : glc.setce(1));
#endif



#endif
