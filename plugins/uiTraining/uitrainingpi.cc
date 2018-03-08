/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : A.H. Bril
 * DATE     : NOv 2003
-*/

#include "uimenu.h"
#include "uimsg.h"
#include "uiodmenumgr.h"
#include "uitrainingwell.h"

#include "filepath.h"
#include "ioman.h"
#include "ioobj.h"
#include "oddirs.h"
#include "ptrman.h"
#include "survinfo.h"

#include "odplugin.h"



mDefODPluginInfo(uiTraining)
{
    mDefineStaticLocalObject( PluginInfo, retpi,(
	"Training plugin",
	"GERMI",
	"dGb",
	"1.0",
	"Shows some simple plugin development basics."
	    "\nCan be loaded into od_main only.") );
    return &retpi;
}


class uiTrainingMgr :  public CallBacker
{ mODTextTranslationClass(uiTrainingMgr);
public:
			uiTrainingMgr(uiODMain*);

    uiODMain*		appl_;
    void		doWells(CallBacker*);
};


uiTrainingMgr::uiTrainingMgr( uiODMain* a )
	: appl_(a)
{
    uiMenu* mnu = new uiMenu( appl_, tr("Training Tools") );
    mnu->insertItem( new uiAction(m3Dots(tr("Well Tools")),
					mCB(this,uiTrainingMgr,doWells)) );
    appl_->menuMgr().toolsMnu()->insertItem( mnu );
}


void uiTrainingMgr::doWells( CallBacker* )
{
    uiTrainingWell dlg( appl_ );
    dlg.go();
}


mDefODInitPlugin(uiTraining)
{
    mDefineStaticLocalObject( PtrMan<uiTrainingMgr>, theinst_, = 0 );
    if ( theinst_ ) return 0;

    theinst_ = new uiTrainingMgr( ODMainWin() );
    if ( !theinst_ )
	return "Cannot instantiate Training plugin";

    return 0;
}
