CondorSubArgumentsSet = {
    1   :  {'Executable'             :  ''},
    2   :  {'Universe'               :  'vanilla'},
    3   :  {'Getenv'                 :  'True'},
    4   :  {'Requirements'           :  ''},
    5   :  {'Arguments'              :  ''},
    6   :  {'Output'                 :  'condor_$(Process).out'},
    7   :  {'Error'                  :  'condor_$(Process).err'},
    8   :  {'Log'                    :  'condor_$(Process).log\n'},
    9   :  {'Transfer_Input_files'   :  ''},
    10  :  {'Transfer_Output_files'  :  ''},
    11  :  {'+IsLocalJob'            :  'true'},
    12  :  {'Rank'                   :  'TARGET.IsLocalSlot\n'},
}
