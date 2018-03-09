/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : R.K. Singh
 * DATE     : June 2007
-*/


#include "uitrainingwell.h"
#include "uiaxishandler.h"
#include "uiioobjsel.h"
#include "uilistbox.h"
#include "uimsg.h"
#include "uiseissel.h"
#include "uifunctiondisplay.h"

#include "ioobj.h"
#include "ioman.h"
#include "seistrc.h"
#include "seistrctr.h"
#include "seisread.h"
#include "seisselectionimpl.h"
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
    wellselfld_->selectionDone.notify( mCB(this,uiTrainingWell,wellChg) );

    uiListBox::Setup su( OD::ChooseOnlyOne, tr("Select Input Log") );
    logfld_ = new uiListBox( this, su );
    logfld_->setHSzPol( uiObject::Wide );
    logfld_->selectionChanged.notify( mCB(this,uiTrainingWell,logChg) );
    logfld_->attach( alignedBelow, wellselfld_ );

    const IOObjContext sctxt = mIOObjContext(SeisTrc);
    const uiSeisSel::Setup ssu( false, false );
    seisselfld_ = new uiSeisSel( this, sctxt, ssu );
    seisselfld_->selectionDone.notify( mCB(this,uiTrainingWell,seisChg) );
    seisselfld_->attach( alignedBelow, logfld_ );

    uiFunctionDisplay::Setup logfsu;
    logdisp_ = new uiFunctionDisplay( this, logfsu );
    logdisp_->setPrefHeight( 400 );
    logdisp_->setPrefWidth( 100 );
    logdisp_->attach( alignedBelow, seisselfld_ );

    uiFunctionDisplay::Setup trcfsu;
    trcdisp_ = new uiFunctionDisplay( this, trcfsu );
    trcdisp_->setPrefHeight( 400 );
    trcdisp_->setPrefWidth( 100 );
    trcdisp_->attach( ensureBelow, seisselfld_ );
    trcdisp_->attach( rightOf, logdisp_ );

    postFinalise().notify( mCB(this,uiTrainingWell,postFinaliseCB) );
}


void uiTrainingWell::postFinaliseCB( CallBacker* )
{
    wellChg( 0 );
}


uiTrainingWell::~uiTrainingWell()
{
}


void uiTrainingWell::wellChg( CallBacker* cb )
{
    logfld_->setEmpty();
    const IOObj* selobj = wellselfld_->ioobj( !cb );
    if ( !selobj )
	return;

    wd_ = Well::MGR().get( selobj->key() );
    if ( !wd_ )
    {
	uiMSG().error( tr("Cannot get data for the selected well") );
	return;
    }

    const Well::LogSet& logs = wd_->logs();
    for ( int idx=0; idx<logs.size(); idx++ )
	logfld_->addItem( logs.getLog(idx).name() );

    seisChg(0);
}


void uiTrainingWell::logChg( CallBacker* cb )
{
    logdisp_->setEmpty();
    if ( !wd_ )
	return;

    const int selidx = logfld_->currentItem();
    if ( selidx < 0 )
	return;

    const Well::LogSet& logs = wd_->logs();
    const Well::Log& sellog = logs.getLog( selidx );
    logdisp_->setVals( sellog.valArr(), sellog.dahArr(), sellog.size() );
    Interval<float> dahrg = sellog.dahRange();
    logdisp_->yAxis(false)->setBounds( Interval<float>(dahrg.stop,dahrg.start));
    logdisp_->yAxis(false)->updateScene();
}


void uiTrainingWell::seisChg( CallBacker* cb )
{
    trcdisp_->setEmpty();
    if ( !wd_ )
	return;

    const IOObj* selobj = seisselfld_->ioobj( true );
    if ( !selobj )
	return;

    const BinID bid = SI().transform( wd_->info().surfacecoord );
    TrcKeyZSampling tkzs( true );
    tkzs.hsamp_.start = tkzs.hsamp_.stop = bid;
    Seis::RangeSelData* seldata = new Seis::RangeSelData( tkzs );
    SeisTrcReader rdr( selobj );
    rdr.setSelData( seldata );
    SeisTrc trc;
    const int rv = rdr.get( trc.info() );
    if ( rv != 1 )
    {
	uiMSG().error( tr("Position not found in cube") );
	return;
    }

    rdr.get( trc );
    TypeSet<float> zvals( trc.size(), 0.f );
    TypeSet<float> vals( trc.size(), 0.f );
    const int sz = trc.size();
    for ( int idx=0; idx<sz; idx++ )
    {
	zvals[idx] = trc.info().samplePos( idx );
	vals[idx] = trc.get( idx, 0 );
    }

    trcdisp_->setVals( vals.arr(), zvals.arr(), sz );
    trcdisp_->yAxis(false)->setBounds(
		Interval<float>(trc.info().samplePos(sz-1),
				trc.info().samplePos(0)) );
    trcdisp_->yAxis(false)->updateScene();
}


bool uiTrainingWell::acceptOK( CallBacker* )
{
    return true;
}
