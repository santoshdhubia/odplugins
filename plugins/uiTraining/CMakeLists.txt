#_______________________Pmake___________________________________________________
#
#	Makefile : 	Training plugin
# 	March 2018	GERMI
#	RCS :		$Id$
#_______________________________________________________________________________

set(OD_MODULE_NAME uiTraining)
set(OD_IS_PLUGIN 1 )
set(OD_MODULE_DEPS uiODMain)
set(OD_MODULE_SOURCES
	uitrainingwell.cc
	uitrainingpi.cc)
SET( OD_PLUGIN_ALO_EXEC ${OD_MAIN_EXEC} )
#SET( OD_NO_ALO_ENTRY 1 )
OD_INIT_MODULE()
