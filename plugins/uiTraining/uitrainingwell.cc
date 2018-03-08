/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : R.K. Singh
 * DATE     : June 2007
-*/


#include "uitrainingwell.h"
#include "uiioobjsel.h"
#include "uilistbox.h"
#include "uimsg.h"
#include "ioobj.h"
#include "ioman.h"
#include "welldata.h"
#include "wellio.h"
#include "welllog.h"
#include "welllogset.h"
#include "wellman.h"
#include "welltransl.h"


uiTrainingWell::uiTrainingWell( uiParent* p )
	: uiDialog( p, Setup( tr("List Logs"),
			      tr("Just listing the logs"),
			      mNoHelpKey ) )
{
    const IOObjContext ctxt = mIOObjContext(Well);
    wellselfld_ = new uiIOObjSel( this, ctxt );
    wellselfld_->selectionDone.notify( mCB(this,uiTrainingWell,inpChg) );

    uiListBox::Setup su( OD::ChooseOnlyOne, tr("Select Input Log") );
    logfld_ = new uiListBox( this, su );
    logfld_->setHSzPol( uiObject::Wide );
    logfld_->attach( alignedBelow, wellselfld_ );

    inpChg( 0 );
}


uiTrainingWell::~uiTrainingWell()
{
}


void uiTrainingWell::inpChg( CallBacker* cb )
{
    logfld_->setEmpty();
    const IOObj* selobj = wellselfld_->ioobj( !cb );
    if ( !selobj )
	return;

    RefMan<Well::Data> wd = Well::MGR().get( selobj->key() );
    if ( !wd )
    {
	uiMSG().error( tr("Cannot get data for the selected well") );
	return;
    }

    const Well::LogSet& logs = wd->logs();
    for ( int idx=0; idx<logs.size(); idx++ )
	logfld_->addItem( logs.getLog(idx).name() );
}


bool uiTrainingWell::acceptOK( CallBacker* )
{
    return true;
}
