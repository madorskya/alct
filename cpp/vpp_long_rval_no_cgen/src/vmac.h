/**
Verilog++ SP.
\author A. Madorsky
*/
// Copyright (c) 2002, Alexander Madorsky, University of Florida/Physics. All rights reserved.


#ifndef _VMAC_H_FILE_
#define _VMAC_H_FILE_

extern globcontrol glc;

#ifdef VGEN
	#define For(par1, par2, par3)  glc.setprintassign(0); cout << glc.getmargin() << "for (" << flush; cout << (par1).getname() << "; " << flush; glc.setprintassign(0); cout << (par2).getname() << "; " << flush; glc.setprintassign(0); cout << (par3).getname(); cout << ") " << flush; glc.setprintassign(1); glc.enablemargin(0);
#else
	#define For(par1, par2, par3) for ((par1); ((par2).getbool()); (par3))
#endif

#ifdef VGEN
	#define If(par) glc.setprintassign(0); cout << glc.getmargin() << "if (" << flush; cout << (par).getname(); cout << ") " << flush; glc.setprintassign(1);  glc.enablemargin(0);
#else
	#define If(par) if ((par).getbool())
#endif

#ifdef VGEN
	#define Else cout << glc.getmargin() << "else " << flush;  glc.enablemargin(0);
#else
	#define Else else
#endif

#define begin vbegin()
#ifdef VGEN
	#define vbegin() glc.enablemargin(1); cout << "\n" << glc.getmargin() << "begin\n" << flush; glc.Indent();
#else
	#define vbegin() { glc.alwayspush();
#endif

#define end vend()
#ifdef VGEN
	#define vend() glc.Outdent(); cout << glc.getmargin() << "end\n" << flush;
#else
	#define vend() glc.alwayspop(); }
#endif

#ifdef VGEN
	#define always(par) cout << glc.getmargin() << "always @(" << (par).getorname() << ") " << flush;  glc.enablemargin(0);
#else
	#define always(par) glc.alwaysstart(); if ((par).getchange())
#endif

#ifdef VGEN
	#define assign cout << glc.getmargin() << "assign " << flush; glc.enablemargin(0);
#else
	#define assign
#endif

#ifdef VGEN
	#define deassign(par) cout << glc.getmargin() << "deassign " << flush; cout << (par).getname() << ";\n" << flush; glc.enablemargin(1);
#else
	#define deassign(par) ;
#endif

#ifdef VGEN
	#define begincase(par) glc.setprintassign(0); cout << glc.getmargin() << "case (" << flush; cout << (par).getcatname() << flush; cout << ")\n" << flush; glc.setprintassign(1); glc.Indent();
#else
#define begincase(par) pushswitch((par)); if (0) {}
#endif

#define endcase vendcase()
#ifdef VGEN
	#define vendcase() glc.Outdent(); cout << glc.getmargin() << "endcase\n" << flush;
#else
	#define vendcase() popswitch();
#endif

#ifdef VGEN
	#define case1(par)	 \
						cout << glc.getmargin(); \
						glc.setprintassign(0); cout << ((Signal)(par)).getcatname(); \
						cout << " : " << flush; glc.setprintassign(1);  glc.enablemargin(0);
#else
	#define case1(par) else if ((getswitch() == (par)).getbool())
#endif

#ifdef VGEN
	#define case2(par1, par2) \
						glc.setprintassign(0); \
						cout << glc.getmargin(); \
						glc.setprintassign(0); cout << ((Signal)(par1)).getcatname(); cout << ", "; \
						glc.setprintassign(0); cout << ((Signal)(par2)).getcatname();\
						cout << " : " << flush; glc.setprintassign(1);  glc.enablemargin(0);
#else
	#define case2(par1, par2) else if ( (getswitch() == (par1)).getbool() || (getswitch() == (par2)).getbool())
#endif

#ifdef VGEN
	#define case3(par1, par2, par3) \
						glc.setprintassign(0); \
						cout << glc.getmargin(); \
						glc.setprintassign(0); cout << ((Signal)(par1)).getcatname(); cout << ", "; \
						glc.setprintassign(0); cout << ((Signal)(par2)).getcatname(); cout << ", "; \
						glc.setprintassign(0); cout << ((Signal)(par3)).getcatname(); \
						cout << " : " << flush; glc.setprintassign(1);  glc.enablemargin(0);
#else
	#define case3(par1, par2, par3) else if ( (getswitch() == (par1)).getbool() || (getswitch() == (par2)).getbool() || (getswitch() == (par3)).getbool())
#endif


#define Default vdefault()
#ifdef VGEN
	#define vdefault() cout << glc.getmargin(); cout << "default : " << flush;  glc.enablemargin(0);
#else
	#define vdefault() else
#endif

#define beginmodule vbeginmodule();
#define endmodule vendmodule();

#define beginfunction vbeginfunction();
#define endfunction vendfunction(); return (result);

#if (__GNUC__==2)||defined(_MSC_VER)
#define or ||
#endif
#define makereg this

#define endperiod glc.ResetEvents();

#ifdef VGEN
	#define comment(par) if (glc.getFileOpen()) {cout << glc.getmargin(); cout << (par) << "\n" << flush;} else glc.AddComment((string)(par));
#else
	#define comment(par)
#endif

#ifdef VGEN
	#define printv(par) cout << (par) << flush;
#else
	#define printv(par)
#endif


#ifdef VGEN
	#define initio glc.setparent(this); glc.setFileOpen(0);
#else
	#define initio glc.setparent(this);
#endif

// these macros are different for all possible types of initializations,
// because not all compilers yet support ISO C99 variadic macros (macros with variable number of parameters)
// At the moment of writing, only GNU C++ compiler supports them.
#define Reg(cl)                cl.reg(#cl)
#define Reg_(cl, h, l)         cl.reg(h, l, #cl)
#define Reg__(cl, h, l, t, b)  cl.reg(h, l, t, b, #cl);

#define Wire(cl)               cl.wire(#cl)
#define Wire_(cl, h, l)        cl.wire(h, l, #cl)
#define Wire__(cl, h, l, t, b)    for (int __wi__ = b; __wi__ <= t; __wi__++) cl[__wi__].wire(h, l, #cl, __wi__)

#define Input(cl)              cl.input(#cl)
#define Input_(cl, h, l)       cl.input(h, l, #cl)

#define Output(cl)             cl.output(#cl)
#define Output_(cl, h, l)      cl.output(h, l, #cl)

#define OutReg(cl)             cl.output(#cl, makereg)
#define OutReg_(cl, h, l)      cl.output(h, l, #cl, makereg)

#define Inout(cl)              cl.inout(#cl)
#define Inout_(cl, h, l)       cl.inout(h, l, #cl)

#define Module(md)			   md.init(#md, #md)
#define Module_(md, fn)		   md.init(#md, #fn)

#define cns(b,v) glc.constant(b,v)

#endif

