//===========================================================================
// stlrProfile.h : data types and definitions related to initialization files
//                 (OS2.INI and SMARTWIN.INI)
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_PROFILE_H_
   #define _STLR_PROFILE_H_

// app string in OS2.INI ----------------------------------------------------
#define OS2INIAPPL     "Smart Windows"   // nome applicaz in OS2.INI

// keys used for storing exceptions -----------------------------------------
#define SZEXC_GENERAL            "EXC_GENERAL"
#define SZEXC_MAINWIN            "EXC_WINDOWS"
#define SZEXC_TBARGEN            "EXC_TBARGEN"
#define SZEXC_TBARHIDE           "EXC_TBARHIDE"
#define SZEXC_WINROLL            "EXC_WINROLL"
#define SZEXC_MAXWIN             "EXC_MAXWIN"
#define SZEXC_SZMOVE             "EXC_SZMOVE"
#define SZEXC_TXTCTL             "EXC_TXTCTL"
#define SZEXC_VIOWIN             "EXC_VIOWIN"
#define SZEXC_MOUSE              "EXC_MOUSE"

#define SZPRO_OPTIONS            "options"
   #define SZPRO_GLOBAL             "global"
   // names of the bitmap files
   #define SZPRO_BMPATBAR           "BMPATBAR"
   #define SZPRO_BMPITBAR           "BMPITBAR"
   #define SZPRO_BMPSMENU           "BMPSMENU"
   #define SZPRO_BMPCLOSE           "BMPCLOSE"
   #define SZPRO_BMPHIDE            "BMPHIDE"
   #define SZPRO_BMPMIN             "BMPMIN"
   #define SZPRO_BMPREST            "BMPREST"
   #define SZPRO_BMPMAX             "BMPMAX"
   #define SZPRO_BMPROLL            "BMPROLL"
   #define SZPRO_BMPUNROLL          "BMPUNROLL"
   #define SZPRO_BMPHTBAR           "BMPHTBAR"
   #define SZPRO_BMPUNHTBAR         "BMPUNHTBAR"
   #define ASZPRO_BMPFILES \
      {SZPRO_BMPATBAR, SZPRO_BMPITBAR, SZPRO_BMPSMENU, \
       SZPRO_BMPCLOSE, SZPRO_BMPHIDE, SZPRO_BMPMIN, \
       SZPRO_BMPREST, SZPRO_BMPMAX, SZPRO_BMPROLL, \
       SZPRO_BMPUNROLL, SZPRO_BMPHTBAR, SZPRO_BMPUNHTBAR}

   // bitmap data
   #define SZPRO_BMPATBARDATA       "BMPATBARDATA"
   #define SZPRO_BMPITBARDATA       "BMPITBARDATA"
   #define SZPRO_BMPSMENUDATA       "BMPSMENUDATA"
   #define SZPRO_BMPCLOSEDATA       "BMPCLOSEDATA"
   #define SZPRO_BMPHIDEDATA        "BMPHIDEDATA"
   #define SZPRO_BMPMINDATA         "BMPMINDATA"
   #define SZPRO_BMPRESTDATA        "BMPRESTDATA"
   #define SZPRO_BMPMAXDATA         "BMPMAXDATA"
   #define SZPRO_BMPROLLDATA        "BMPROLLDATA"
   #define SZPRO_BMPUNROLLDATA      "BMPUNROLLDATA"
   #define SZPRO_BMPHTBARDATA       "BMPHTBARDATA"
   #define SZPRO_BMPUNHTBARDATA     "BMPUNHTBARDATA"
   #define ASZPRO_BMPDATA \
      {SZPRO_BMPATBARDATA, SZPRO_BMPITBARDATA, SZPRO_BMPSMENUDATA,\
       SZPRO_BMPCLOSEDATA, SZPRO_BMPHIDEDATA, SZPRO_BMPMINDATA,\
       SZPRO_BMPRESTDATA, SZPRO_BMPMAXDATA, SZPRO_BMPROLLDATA,\
       SZPRO_BMPUNROLLDATA, SZPRO_BMPHTBARDATA, SZPRO_BMPUNHTBARDATA}

   #define SZPRO_VERSION            "version"

#define SZPRO_SIZEPOS            "sizepos"
   #define SZPRO_PREF               "preferences"
   #define SZPRO_MOUKBD             "moukbdDlg"
   #define SZPRO_BMPSEL             "bmpSelDlg"



#endif // #ifndef _STLR_PROFILE_H_

