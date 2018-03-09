#ifndef uitrainingwell_h
#define uitrainingwell_h
/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : R.K. Singh
 * DATE     : June 2007
 * ID       : $Id$
-*/

#include "uitrainingmod.h"
#include "uidialog.h"

class uiListBox;
class uiIOObjSel;
class uiSeisSel;
namespace Well { class Data; }
class uiFunctionDisplay;


mExpClass(uiTraining) uiTrainingWell : public uiDialog
{ mODTextTranslationClass(uiTrainingWell);
public:

    			uiTrainingWell(uiParent*);
			~uiTrainingWell();

protected:

    uiIOObjSel*		wellselfld_;
    uiSeisSel*		seisselfld_;
    uiListBox*		logfld_;
    uiFunctionDisplay*	trcdisp_;
    uiFunctionDisplay*	logdisp_;

    RefMan<Well::Data>	wd_;

    void		wellChg(CallBacker*);
    void		logChg(CallBacker*);
    void		seisChg(CallBacker*);
    void		postFinaliseCB(CallBacker*);
    bool		acceptOK(CallBacker*);

};


#endif
