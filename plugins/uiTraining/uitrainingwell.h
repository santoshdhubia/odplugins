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


mExpClass(uiTraining) uiTrainingWell : public uiDialog
{ mODTextTranslationClass(uiTrainingWell);
public:

    			uiTrainingWell(uiParent*);
			~uiTrainingWell();

protected:

    uiIOObjSel*		wellselfld_;
    uiListBox*		logfld_;

    void		inpChg(CallBacker*);
    bool		acceptOK(CallBacker*);

};


#endif
