CondorSubArgumentsSet = {
	1:{'Executable'           : ''},
	2:{'Universe'             : 'vanilla'},
	3:{'Getenv'               : 'True'},
	4:{'Arguments'            : ''},
	5:{'Output'               : 'condor_$(Process).out'},
	6:{'Error'                : 'condor_$(Process).err'},
	7:{'Log'                  : 'condor_$(Process).log\n'},
	8:{'Transfer_Input_files' : ''},
	9:{'+IsLocalJob'          : 'true'},
	10:{'Rank'                 : 'TARGET.IsLocalSlot\n'},
	11:{'Queue'                : ''}
}
